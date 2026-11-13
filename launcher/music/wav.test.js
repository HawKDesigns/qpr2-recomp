/* Tests for ui/wav.js. Run: node launcher/music/wav.test.js
 * Includes independent reference IMA/Xbox ADPCM encoders so the decoder is checked against real encodes. */
'use strict';
const assert = require('assert');
const path = require('path');
const W = require(path.join(__dirname, '..', 'ui', 'wav.js'));
const { STEP, INDEX } = W;
let passed = 0;
const test = (name, fn) => { fn(); passed++; console.log('  ok  ' + name); };

// ---------- helpers: build WAV files ----------
function u16(n) { return [n & 255, (n >> 8) & 255]; }
function u32(n) { return [n & 255, (n >>> 8) & 255, (n >>> 16) & 255, (n >>> 24) & 255]; }
function chunk(id, body) { const b = Buffer.from(body); return Buffer.concat([Buffer.from(id), Buffer.from(u32(b.length)), b, Buffer.alloc(b.length & 1)]); }
function wav(fmtBody, data, extra, opts) {
  opts = opts || {};
  const d = Buffer.from(data);
  const parts = [Buffer.from('WAVE'), ...(extra ? [Buffer.from(extra)] : []), chunk('fmt ', fmtBody),
    Buffer.from('data'), Buffer.from(u32(opts.dataSize != null ? opts.dataSize : d.length)), d, Buffer.alloc(d.length & 1)];
  const body = Buffer.concat(parts);
  const out = Buffer.concat([Buffer.from('RIFF'), Buffer.from(u32(body.length)), body]);
  return out.buffer.slice(out.byteOffset, out.byteOffset + out.byteLength);
}
function fmt(tag, ch, rate, bits, align, extra) {
  return [...u16(tag), ...u16(ch), ...u32(rate), ...u32(rate * align), ...u16(align), ...u16(bits), ...(extra || [])];
}
function snr(ref, got) {
  const n = Math.min(ref.length, got.length); let s = 0, e = 0;
  for (let i = 0; i < n; i++) { s += ref[i] * ref[i]; const d = ref[i] - got[i]; e += d * d; }
  return 10 * Math.log10(s / Math.max(e, 1e-12));
}
function signal(frames, ch, silentLead) {
  const out = Array.from({ length: ch }, () => new Float32Array(frames));
  let seed = 12345; const rnd = () => (seed = (seed * 1103515245 + 12345) & 0x7fffffff) / 0x7fffffff - 0.5;
  for (let c = 0; c < ch; c++) for (let i = 0; i < frames; i++) {
    if (i < silentLead) continue;
    const t = i / 44100;
    out[c][i] = 0.45 * Math.sin(2 * Math.PI * (220 + 40 * c) * t) + 0.25 * Math.sin(2 * Math.PI * 1320 * t) * Math.sin(2 * Math.PI * 1.7 * t) + 0.02 * rnd();
  }
  return out;
}

// ---------- reference IMA encoder (written independently of the decoder) ----------
const clamp16 = v => v > 32767 ? 32767 : v < -32768 ? -32768 : v;
function encNibble(sample, st) {
  let diff = sample - st.pred, sign = 0; if (diff < 0) { sign = 8; diff = -diff; }
  let step = STEP[st.idx], nib = 0, vp = step >> 3;
  if (diff >= step) { nib |= 4; diff -= step; vp += step; }
  step >>= 1; if (diff >= step) { nib |= 2; diff -= step; vp += step; }
  step >>= 1; if (diff >= step) { nib |= 1; vp += step; }
  st.pred = clamp16(sign ? st.pred - vp : st.pred + vp);
  st.idx = Math.max(0, Math.min(88, st.idx + INDEX[nib]));
  return nib | sign;
}
/* mode 'emit': header = the block's first real sample, then 8*groups more samples (standard WAV IMA).
 * mode 'state': header = decoder state at block start, then 8*groups samples (Xbox-style, 64 per 36 bytes). */
function encodeAdpcm(chans, mode, groups) {
  const ch = chans.length, per = groups * 8, hdr = mode === 'emit' ? 1 : 0, blockSamples = per + hdr;
  const total = chans[0].length, blocks = Math.floor(total / blockSamples), align = 4 * ch + groups * 4 * ch;
  const out = [], st = Array.from({ length: ch }, () => ({ pred: 0, idx: 0 }));
  for (let b = 0; b < blocks; b++) {
    const s0 = b * blockSamples, nibs = Array.from({ length: ch }, () => []);
    for (let c = 0; c < ch; c++) {
      const q = i => clamp16(Math.round(chans[c][i] * 32767));
      if (mode === 'emit') { st[c].pred = q(s0); out.push(...u16(st[c].pred & 0xffff), st[c].idx, 0); for (let i = 0; i < per; i++) nibs[c].push(encNibble(q(s0 + 1 + i), st[c])); }
      else { out.push(...u16(st[c].pred & 0xffff), st[c].idx, 0); for (let i = 0; i < per; i++) nibs[c].push(encNibble(q(s0 + i), st[c])); }
    }
    for (let g = 0; g < groups; g++) for (let c = 0; c < ch; c++) for (let k = 0; k < 4; k++) out.push(nibs[c][g * 8 + k * 2] | (nibs[c][g * 8 + k * 2 + 1] << 4));
  }
  return { data: out, align, blockSamples, blocks };
}

console.log('PCM and float');
test('PCM16 stereo round trip', () => {
  const s = signal(2000, 2, 0), bytes = [];
  for (let i = 0; i < 2000; i++) for (let c = 0; c < 2; c++) bytes.push(...u16(Math.round(s[c][i] * 32767) & 0xffff));
  const r = W.decode(wav(fmt(1, 2, 44100, 16, 4), bytes));
  assert.strictEqual(r.rate, 44100); assert.strictEqual(r.channels.length, 2); assert.strictEqual(r.frames, 2000);
  assert(snr(s[0], r.channels[0]) > 80 && snr(s[1], r.channels[1]) > 80);
});
test('PCM8 unsigned, PCM24, PCM32', () => {
  const r8 = W.decode(wav(fmt(1, 1, 8000, 8, 1), [0, 128, 255]));
  assert(Math.abs(r8.channels[0][0] + 1) < 1e-6 && r8.channels[0][1] === 0 && r8.channels[0][2] > 0.99);
  const r24 = W.decode(wav(fmt(1, 1, 44100, 24, 3), [0, 0, 0x80, 0xff, 0xff, 0x7f]));
  assert(Math.abs(r24.channels[0][0] + 1) < 1e-6 && r24.channels[0][1] > 0.9999);
  const r32 = W.decode(wav(fmt(1, 1, 44100, 32, 4), [0, 0, 0, 0x80, 0xff, 0xff, 0xff, 0x7f]));
  assert(Math.abs(r32.channels[0][0] + 1) < 1e-6 && r32.channels[0][1] > 0.9999);
});
test('float32 and float64', () => {
  const f32 = Buffer.alloc(8); f32.writeFloatLE(0.5, 0); f32.writeFloatLE(-0.25, 4);
  const r = W.decode(wav(fmt(3, 1, 44100, 32, 4), f32)); assert.strictEqual(r.channels[0][0], 0.5); assert.strictEqual(r.channels[0][1], -0.25);
  const f64 = Buffer.alloc(8); f64.writeDoubleLE(0.125, 0);
  assert.strictEqual(W.decode(wav(fmt(3, 1, 44100, 64, 8), f64)).channels[0][0], 0.125);
});
test('WAVE_FORMAT_EXTENSIBLE resolves the sub-format', () => {
  const ext = [...u16(22), ...u16(16), ...u32(3), ...u16(1), ...new Array(14).fill(0)];
  const f = fmt(0xFFFE, 2, 48000, 16, 4, ext); f.splice(16, 0);
  const body = [...u16(0xFFFE), ...u16(2), ...u32(48000), ...u32(192000), ...u16(4), ...u16(16), ...u16(22), ...u16(16), ...u32(3), ...u16(1), ...new Array(14).fill(0)];
  const r = W.decode(wav(body, [0, 64, 0, 192]));
  assert.strictEqual(r.codec, 'pcm'); assert.strictEqual(r.channels.length, 2); assert(r.channels[0][0] === 0.5 && r.channels[1][0] === -0.5);
});

console.log('Container handling');
test('LIST chunk before data and odd-size chunk padding', () => {
  const list = chunk('LIST', [1, 2, 3]);          // odd length, padded
  const r = W.decode(wav(fmt(1, 1, 22050, 16, 2), [0, 64, 0, 192], list)); assert.strictEqual(r.frames, 2);
});
test('data size 0 or oversize (streamed or truncated) uses what is there', () => {
  assert.strictEqual(W.decode(wav(fmt(1, 1, 44100, 16, 2), new Array(40).fill(1), null, { dataSize: 0 })).frames, 20);
  assert.strictEqual(W.decode(wav(fmt(1, 1, 44100, 16, 2), new Array(40).fill(1), null, { dataSize: 0xFFFFFFFF })).frames, 20);
});
test('errors are clear', () => {
  assert.throws(() => W.decode(new ArrayBuffer(4)), /Not a RIFF/);
  const raw = Buffer.from('RIFF\0\0\0\0WAVEjunkjunk'); // pooled buffer: slice to exactly these bytes
  assert.throws(() => W.decode(raw.buffer.slice(raw.byteOffset, raw.byteOffset + raw.byteLength)), /no fmt/);
  assert.throws(() => W.decode(wav(fmt(0x55, 2, 44100, 0, 1), [1, 2, 3, 4])), /Unsupported WAV format 0x0055/);
  assert.throws(() => W.decode(wav(fmt(1, 1, 44100, 16, 2), [])), /no audio|contains no audio/);
});

console.log('ADPCM');
for (const ch of [1, 2]) {
  test('IMA ADPCM (0x11) ' + (ch === 1 ? 'mono' : 'stereo') + ', 256-byte blocks', () => {
    const s = signal(30000, ch, 0), groups = (256 / ch - 4) / 4 | 0;
    const e = encodeAdpcm(s, 'emit', groups);
    const r = W.decode(wav(fmt(0x11, ch, 44100, 4, e.align, [...u16(2), ...u16(e.blockSamples)]), e.data));
    assert.strictEqual(r.layout, 'emit'); assert.strictEqual(r.frames, e.blocks * e.blockSamples);
    for (let c = 0; c < ch; c++) assert(snr(s[c], r.channels[c]) > 28, 'SNR ' + snr(s[c], r.channels[c]));
  });
  for (const conv of ['state', 'emit']) {
    test('Xbox ADPCM (0x69) ' + (ch === 1 ? 'mono' : 'stereo') + ', header as ' + conv + ' -> auto-detected and decoded', () => {
      const s = signal(60000, ch, 0), e = encodeAdpcm(s, conv, 8);
      assert.strictEqual(e.align, 36 * ch);
      const file = wav(fmt(0x69, ch, 44100, 4, e.align, [...u16(2), ...u16(64)]), e.data);
      const r = W.decode(file);
      assert.strictEqual(r.codec, 'xbox'); assert.strictEqual(r.layout, conv);
      for (let c = 0; c < ch; c++) assert(snr(s[c], r.channels[c]) > 28, conv + ' SNR ' + snr(s[c], r.channels[c]));
    });
  }
}
test('the wrong layout genuinely sounds wrong (so detection matters)', () => {
  const s = signal(60000, 2, 0), e = encodeAdpcm(s, 'state', 8);
  const file = wav(fmt(0x69, 2, 44100, 4, e.align, [...u16(2), ...u16(64)]), e.data);
  const good = W.decode(file, { xbox: 'state' }), bad = W.decode(file, { xbox: 'emit' });
  assert(snr(s[0], good.channels[0]) > 28); assert(snr(s[0], bad.channels[0]) < 15, 'wrong layout SNR ' + snr(s[0], bad.channels[0]));
});
test('detection survives a long silent intro, and a manual override wins', () => {
  for (const conv of ['state', 'emit']) {
    const s = signal(120000, 2, 40000), e = encodeAdpcm(s, conv, 8);
    const file = wav(fmt(0x69, 2, 44100, 4, e.align, [...u16(2), ...u16(64)]), e.data);
    assert.strictEqual(W.decode(file).layout, conv);
    assert.strictEqual(W.decode(file, { xbox: conv === 'state' ? 'emit' : 'state' }).layout, conv === 'state' ? 'emit' : 'state');
  }
});
test('detection on pure silence or a tiny file does not crash and returns the documented default', () => {
  const z = encodeAdpcm([new Float32Array(20000)], 'state', 8);
  assert.strictEqual(W.decode(wav(fmt(0x69, 1, 44100, 4, z.align, [...u16(2), ...u16(64)]), z.data)).layout, 'state');
});
test('truncated ADPCM data decodes the whole blocks that exist', () => {
  const s = signal(6500, 1, 0), e = encodeAdpcm(s, 'state', 8), cut = e.data.slice(0, e.data.length - 10);
  const r = W.decode(wav(fmt(0x69, 1, 44100, 4, e.align, [...u16(2), ...u16(64)]), cut));
  assert.strictEqual(r.frames, (e.blocks - 1) * 64);
});
test('decode speed: 4 minutes of stereo Xbox ADPCM', () => {
  const s = signal(44100 * 240, 2, 0), e = encodeAdpcm(s, 'state', 8);
  const file = wav(fmt(0x69, 2, 44100, 4, e.align, [...u16(2), ...u16(64)]), e.data);
  const t0 = process.hrtime.bigint(), r = W.decode(file), ms = Number(process.hrtime.bigint() - t0) / 1e6;
  assert(r.frames >= 44100 * 239); console.log('        decoded ' + (r.frames / 44100 | 0) + ' s of audio in ' + ms.toFixed(0) + ' ms');
  assert(ms < 4000);
});
console.log('\n' + passed + ' tests passed');
