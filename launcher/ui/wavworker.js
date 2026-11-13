/* Decodes WAV files off the UI thread so the launcher stays smooth while a track loads. */
importScripts('wav.js');
self.onmessage = function (e) {
  const m = e.data;
  try {
    const r = QprWav.decode(m.buf, m.opts);
    self.postMessage({ id: m.id, ok: true, rate: r.rate, frames: r.frames, codec: r.codec, layout: r.layout, channels: r.channels }, r.channels.map(c => c.buffer));
  } catch (err) {
    self.postMessage({ id: m.id, ok: false, error: String(err && err.message || err) });
  }
};
