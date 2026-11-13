/* WAV decoder for the launcher's music player.
 * Browsers only play plain PCM WAV, but console games often ship ADPCM, so this decodes the formats
 * a game soundtrack is likely to use: PCM (8/16/24/32 bit), IEEE float, IMA ADPCM (0x11) and Xbox ADPCM (0x69).
 * Pure functions with no DOM or WebAudio dependency, so it runs in a Worker and under Node for tests. */
(function (root, factory) {
  if (typeof module === 'object' && module.exports) module.exports = factory();
  else root.QprWav = factory();
})(typeof self !== 'undefined' ? self : this, function () {
  'use strict';

  const STEP = [7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767];
  const INDEX = [-1, -1, -1, -1, 2, 4, 6, 8];

  function fourcc(dv, o) { return String.fromCharCode(dv.getUint8(o), dv.getUint8(o + 1), dv.getUint8(o + 2), dv.getUint8(o + 3)); }

  /* Reads the header only. Returns { tag, codec, channels, rate, bits, blockAlign, samplesPerBlock, dataOffset, dataLength }. */
  function parse(buf) {
    const dv = new DataView(buf), n = buf.byteLength;
    if (n < 12 || fourcc(dv, 0) !== 'RIFF' || fourcc(dv, 8) !== 'WAVE') throw new Error('Not a RIFF/WAVE file.');
    let off = 12, fmt = null, data = null;
    while (off + 8 <= n) {
      const id = fourcc(dv, off), body = off + 8;
      let size = dv.getUint32(off + 4, true);
      if (id === 'fmt ') {
        const sz = Math.min(size, n - body);
        if (sz < 16) throw new Error('The fmt chunk is too short.');
        fmt = { tag: dv.getUint16(body, true), channels: dv.getUint16(body + 2, true), rate: dv.getUint32(body + 4, true), blockAlign: dv.getUint16(body + 12, true), bits: dv.getUint16(body + 14, true), samplesPerBlock: 0 };
        if (sz >= 20) fmt.samplesPerBlock = dv.getUint16(body + 18, true);
        if (fmt.tag === 0xFFFE && sz >= 26) fmt.tag = dv.getUint16(body + 24, true);   // WAVE_FORMAT_EXTENSIBLE: real tag is the sub-format
      } else if (id === 'data') {
        const avail = n - body;
        if (size === 0 || size > avail) size = avail;                                   // streamed or truncated files
        data = { offset: body, length: size };
      }
      off = body + size + (size & 1);                                                   // chunks are padded to even sizes
    }
    if (!fmt) throw new Error('The WAV file has no fmt chunk.');
    if (!data) throw new Error('The WAV file has no audio data.');
    if (fmt.channels < 1 || fmt.channels > 8) throw new Error('Unsupported channel count: ' + fmt.channels + '.');
    const codec = { 1: 'pcm', 3: 'float', 0x11: 'ima', 0x69: 'xbox' }[fmt.tag];
    return { tag: fmt.tag, codec: codec || null, channels: fmt.channels, rate: fmt.rate, bits: fmt.bits, blockAlign: fmt.blockAlign, samplesPerBlock: fmt.samplesPerBlock, dataOffset: data.offset, dataLength: data.length };
  }

  function decodePcm(dv, info) {
    const ch = info.channels, bytes = info.bits >> 3;
    if (![1, 2, 3, 4].includes(bytes) || (info.bits & 7)) throw new Error('Unsupported PCM bit depth: ' + info.bits + '.');
    const frames = Math.floor(info.dataLength / (bytes * ch)), out = Array.from({ length: ch }, () => new Float32Array(frames));
    let p = info.dataOffset;
    for (let i = 0; i < frames; i++) {
      for (let c = 0; c < ch; c++, p += bytes) {
        let v;
        if (bytes === 2) v = dv.getInt16(p, true) / 32768;
        else if (bytes === 1) v = (dv.getUint8(p) - 128) / 128;
        else if (bytes === 3) { let x = dv.getUint8(p) | (dv.getUint8(p + 1) << 8) | (dv.getUint8(p + 2) << 16); if (x & 0x800000) x -= 0x1000000; v = x / 8388608; }
        else v = dv.getInt32(p, true) / 2147483648;
        out[c][i] = v;
      }
    }
    return out;
  }

  function decodeFloat(dv, info) {
    const ch = info.channels, bytes = info.bits >> 3;
    if (bytes !== 4 && bytes !== 8) throw new Error('Unsupported float bit depth: ' + info.bits + '.');
    const frames = Math.floor(info.dataLength / (bytes * ch)), out = Array.from({ length: ch }, () => new Float32Array(frames));
    let p = info.dataOffset;
    for (let i = 0; i < frames; i++) for (let c = 0; c < ch; c++, p += bytes) out[c][i] = bytes === 4 ? dv.getFloat32(p, true) : dv.getFloat64(p, true);
    return out;
  }

  /* IMA ADPCM block: per channel a 4-byte header (int16 predictor, uint8 step index, 0), then 4-byte groups
   * per channel, interleaved, each holding 8 nibbles (low nibble first).
   * mode 'emit'  - the header predictor is itself the first output sample (standard WAV IMA: 1 + 8n samples).
   * mode 'state' - the header is only the starting decoder state; every nibble is a sample (Xbox: 64 per 36 bytes). */
  function decodeAdpcm(dv, info, mode) {
    const ch = info.channels, align = info.blockAlign;
    if (align < 4 * ch + 4 * ch || (align - 4 * ch) % (4 * ch)) throw new Error('Unsupported ADPCM block size: ' + align + '.');
    const blocks = Math.floor(info.dataLength / align), groups = (align - 4 * ch) / (4 * ch), per = groups * 8 + (mode === 'emit' ? 1 : 0);
    const out = Array.from({ length: ch }, () => new Float32Array(blocks * per));
    for (let b = 0; b < blocks; b++) {
      const base = info.dataOffset + b * align;
      for (let c = 0; c < ch; c++) {
        let pred = dv.getInt16(base + 4 * c, true), idx = Math.min(88, dv.getUint8(base + 4 * c + 2)), o = b * per;
        const dst = out[c];
        if (mode === 'emit') dst[o++] = pred / 32768;
        for (let g = 0; g < groups; g++) {
          const p = base + 4 * ch + (g * ch + c) * 4;
          for (let k = 0; k < 4; k++) {
            const byte = dv.getUint8(p + k);
            for (let h = 0; h < 2; h++) {
              const nib = h ? byte >> 4 : byte & 15, step = STEP[idx];
              let d = step >> 3;
              if (nib & 4) d += step; if (nib & 2) d += step >> 1; if (nib & 1) d += step >> 2;
              pred += (nib & 8) ? -d : d; pred = pred > 32767 ? 32767 : pred < -32768 ? -32768 : pred;
              idx += INDEX[nib & 7]; idx = idx < 0 ? 0 : idx > 88 ? 88 : idx;
              dst[o++] = pred / 32768;
            }
          }
        }
      }
    }
    return out;
  }

  /* Xbox ADPCM files exist in both conventions. If the header only carries decoder state, the predictor of
   * block k+1 equals where block k finished. If the header is a real sample, it differs by about one sample of
   * movement. Compare the two across the file and choose. Silence carries no information and is skipped. */
  function detectLayout(dv, info) {
    const ch = info.channels, align = info.blockAlign, blocks = Math.floor(info.dataLength / align), groups = (align - 4 * ch) / (4 * ch);
    if (blocks < 4) return 'state';
    const stride = Math.max(1, Math.floor((blocks - 1) / 400));
    let exact = 0, total = 0, jump = 0, motion = 0, motionN = 0;
    for (let b = 0; b + 1 < blocks; b += stride) {
      const base = info.dataOffset + b * align;
      let pred = dv.getInt16(base, true), idx = Math.min(88, dv.getUint8(base + 2)), any = pred !== 0, prev = pred;
      for (let g = 0; g < groups; g++) {
        const p = base + 4 * ch + g * ch * 4;
        for (let k = 0; k < 4; k++) {
          const byte = dv.getUint8(p + k);
          for (let h = 0; h < 2; h++) {
            const nib = h ? byte >> 4 : byte & 15, step = STEP[idx];
            let d = step >> 3;
            if (nib & 4) d += step; if (nib & 2) d += step >> 1; if (nib & 1) d += step >> 2;
            pred += (nib & 8) ? -d : d; pred = pred > 32767 ? 32767 : pred < -32768 ? -32768 : pred;
            idx += INDEX[nib & 7]; idx = idx < 0 ? 0 : idx > 88 ? 88 : idx;
            if (nib) any = true;
            motion += Math.abs(pred - prev); motionN++; prev = pred;
          }
        }
      }
      const next = dv.getInt16(base + align, true);
      if (!any && next === 0) continue;
      total++; const d = Math.abs(next - pred); jump += d; if (d <= 1) exact++;
    }
    if (total < 3) return 'state';
    if (exact / total >= 0.6) return 'state';
    const avgMotion = motionN ? motion / motionN : 0;
    if (avgMotion > 0 && jump / total < 0.25 * avgMotion) return 'state';
    return 'emit';
  }

  /* opts.xbox: 'auto' (default), 'state' (64 samples per block) or 'emit' (header sample is output). */
  function decode(buf, opts) {
    opts = opts || {};
    const info = parse(buf), dv = new DataView(buf);
    let channels, layout = null;
    if (info.codec === 'pcm') channels = decodePcm(dv, info);
    else if (info.codec === 'float') channels = decodeFloat(dv, info);
    else if (info.codec === 'ima') { layout = 'emit'; channels = decodeAdpcm(dv, info, 'emit'); }
    else if (info.codec === 'xbox') {
      layout = opts.xbox === 'state' || opts.xbox === 'emit' ? opts.xbox : detectLayout(dv, info);
      channels = decodeAdpcm(dv, info, layout);
    } else throw new Error('Unsupported WAV format 0x' + info.tag.toString(16).toUpperCase().padStart(4, '0') + '. Use PCM, IMA ADPCM or Xbox ADPCM.');
    const frames = channels[0].length;
    if (!frames) throw new Error('The WAV file contains no audio.');
    return { rate: info.rate, channels, frames, codec: info.codec, layout, info };
  }

  return { parse, decode, detectLayout, STEP, INDEX };
});
