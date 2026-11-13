'use strict';
/* Launcher soundtrack playback and now-playing display. */
(function () {
  const M = window.Music = {
    ready: false, ctx: null, gain: null, src: null, cur: null, token: 0, startedAt: 0,
    loading: false, error: '', blocked: false, userPaused: false, holds: new Set(),
    queue: [], history: [], failed: new Set(),
    cache: new Map(), worker: null, jobs: new Map(), jobId: 0, applied: {}, sig: '', pauseTimer: 0, warned: new Set()
  };

  // ------------------------------------------------------------------ settings & tracks
  const opt = () => ({ on: val('ui_music') === '1', vol: Math.max(0, Math.min(100, +val('ui_music_volume') || 0)), shuffle: val('ui_music_shuffle') === '1', adpcm: val('ui_music_adpcm') });
  const tracks = () => (S.st && S.st.music && S.st.music.tracks) || [];
  const byN = n => tracks().find(t => t.n === n) || null;
  const enabled = () => !!(S.st && opt().on && tracks().length);
  const effectivePaused = () => M.userPaused || M.holds.size > 0 || !enabled();
  const nowText = t => t ? (t.artist ? t.artist + ' \u2014 ' + t.title : t.title) : '';

  // ------------------------------------------------------------------ decoding (off the UI thread)
  function ensureCtx() {
    if (M.ctx) return M.ctx;
    const AC = window.AudioContext || window.webkitAudioContext;
    M.ctx = new AC(); M.gain = M.ctx.createGain(); M.gain.gain.value = 0; M.gain.connect(M.ctx.destination);
    return M.ctx;
  }
  function workerDecode(buf, opts) {
    return new Promise((resolve, reject) => {
      let w = M.worker;
      if (!w) {
        try { w = M.worker = new Worker('wavworker.js'); } catch (e) { reject(e); return; }
        w.onmessage = ev => { const j = M.jobs.get(ev.data.id); if (!j) return; M.jobs.delete(ev.data.id); ev.data.ok ? j.resolve(ev.data) : j.reject(new Error(ev.data.error)); };
        w.onerror = () => { M.jobs.forEach(j => j.reject(new Error('The decoder stopped.'))); M.jobs.clear(); try { w.terminate(); } catch (e) { /* gone */ } M.worker = null; };
      }
      const id = ++M.jobId; M.jobs.set(id, { resolve, reject });
      w.postMessage({ id, buf, opts }, [buf]);
    });
  }
  function decodeTrack(n) {
    const layout = opt().adpcm;
    const key = n + ':' + layout;
    const hit = M.cache.get(key); if (hit) return hit;
    const p = (async () => {
      const r = await fetch('/music/' + n);
      if (!r.ok) throw new Error('The launcher could not read Track' + n + '.wav (' + r.status + ').');
      const d = await workerDecode(await r.arrayBuffer(), { xbox: layout });
      const ab = ensureCtx().createBuffer(d.channels.length, d.frames, d.rate);
      d.channels.forEach((c, i) => ab.copyToChannel(c, i));
      ab.qprInfo = { codec: d.codec, layout: d.layout };
      return ab;
    })();
    M.cache.set(key, p);
    p.catch(() => { if (M.cache.get(key) === p) M.cache.delete(key); });
    // keep memory bounded: the current track and the next one only
    if (M.cache.size > 2) { const keep = new Set([key, M.cur ? M.cur.n + ':' + opt().adpcm : '']); for (const k of M.cache.keys()) { if (M.cache.size <= 2) break; if (!keep.has(k)) M.cache.delete(k); } }
    return p;
  }

  // ------------------------------------------------------------------ queue
  function refill() {
    const all = tracks().map(t => t.n).sort((a, b) => a - b);
    if (!all.length) { M.queue = []; return; }
    const last = M.cur ? M.cur.n : -1;
    if (opt().shuffle) {
      const bag = all.slice(); for (let i = bag.length - 1; i > 0; i--) { const j = Math.floor(Math.random() * (i + 1)); [bag[i], bag[j]] = [bag[j], bag[i]]; }
      if (bag.length > 1 && bag[0] === last) bag.push(bag.shift());
      M.queue = bag;
    } else {
      const at = all.findIndex(n => n > last);
      M.queue = at < 0 ? all : all.slice(at).concat(all.slice(0, at));
    }
  }
  function peekNext() { if (!M.queue.length) refill(); return M.queue[0]; }
  function takeNext() { if (!M.queue.length) refill(); return M.queue.shift(); }

  // ------------------------------------------------------------------ playback
  function stopSource() { const s = M.src; M.src = null; if (s) { s.onended = null; try { s.stop(); } catch (e) { /* not started */ } try { s.disconnect(); } catch (e) { /* ok */ } } }
  const gainFor = () => Math.pow(opt().vol / 100, 2);
  function ramp(v, secs) { const g = M.gain && M.gain.gain; if (!g) return; const t = M.ctx.currentTime; g.cancelScheduledValues(t); g.setValueAtTime(g.value, t); g.linearRampToValueAtTime(v, t + secs); }
  function applyPause() {
    if (!M.ctx) return;
    clearTimeout(M.pauseTimer);
    if (effectivePaused()) {
      ramp(0, .25);
      M.pauseTimer = setTimeout(() => { if (effectivePaused() && M.ctx.state === 'running') M.ctx.suspend().catch(() => { }); }, 320);
    } else {
      M.ctx.resume().then(() => { M.blocked = false; if (!effectivePaused()) ramp(gainFor(), .4); emit(); }).catch(() => { M.blocked = true; emit(); });
      // Browsers that block autoplay leave resume() pending until the person presses something.
      setTimeout(() => { if (!effectivePaused() && M.ctx.state !== 'running') { M.blocked = true; emit(); } }, 900);
    }
  }
  async function start(n) {
    const t = byN(n); if (!t) return;
    const my = ++M.token; stopSource(); M.cur = t; M.loading = true; M.error = ''; emit();
    let buf;
    try { ensureCtx(); buf = await decodeTrack(n); }
    catch (e) { if (my === M.token) { M.loading = false; fail(t, e); } return; }
    if (my !== M.token) return;
    M.loading = false;
    const src = M.ctx.createBufferSource(); src.buffer = buf; src.connect(M.gain);
    src.onended = () => { if (M.src === src && my === M.token) advance(); };
    M.src = src; M.startedAt = M.ctx.currentTime; src.start();
    M.failed.delete(n);
    applyPause(); emit();
    const nx = peekNext(); if (nx != null && nx !== n) decodeTrack(nx).catch(() => { });
  }
  function fail(t, e) {
    console.warn('[music] Track' + t.n + ':', e.message);
    M.failed.add(t.n);
    if (!M.warned.has(t.n)) { M.warned.add(t.n); toast('Could not play ' + (t.artist ? t.artist + ' \u2013 ' : '') + t.title + ': ' + e.message, 'bad'); }
    if (M.failed.size >= tracks().length) { M.error = e.message; M.cur = null; stopSource(); emit(); return; }
    advance(true);
  }
  function advance(skipHistory) {
    if (!enabled()) return;
    if (M.cur && !skipHistory) M.history.push(M.cur.n);
    if (M.history.length > 50) M.history.shift();
    let n = takeNext(), guard = 0;
    while (M.failed.has(n) && guard++ < tracks().length) n = takeNext();
    if (n != null) start(n);
  }

  M.next = function () { if (enabled()) { M.userPaused = false; advance(); } };
  M.prev = function () {
    if (!enabled()) return;
    M.userPaused = false;
    const into = M.ctx && M.src ? M.ctx.currentTime - M.startedAt : 0;
    if (into > 3 || !M.history.length) { if (M.cur) start(M.cur.n); return; }
    const back = M.history.pop(); if (M.cur) M.queue.unshift(M.cur.n); start(back);
  };
  M.toggle = function () { if (!enabled()) return; M.userPaused = !M.userPaused; applyPause(); emit(); };
  M.playNumber = function (n) { if (!enabled() || !byN(n)) return; M.userPaused = false; if (M.cur) M.history.push(M.cur.n); M.queue = M.queue.filter(x => x !== n); start(n); };
  M.hold = function (why) { if (!M.holds.has(why)) { M.holds.add(why); applyPause(); emit(); } };
  M.release = function (why) { if (M.holds.delete(why)) { applyPause(); emit(); } };
  M.nudge = function () { if (M.ctx && M.ctx.state !== 'running' && !effectivePaused()) applyPause(); };
  M.state = function () {
    return { enabled: enabled(), playing: !!M.src && !effectivePaused(), paused: effectivePaused(), loading: M.loading, blocked: M.blocked, error: M.error,
      current: M.cur ? M.cur.n : null, text: nowText(M.cur), ctx: M.ctx ? M.ctx.state : 'none', holds: Array.from(M.holds), userPaused: M.userPaused, queue: M.queue.slice(), history: M.history.slice(),
      info: M.src && M.src.buffer && M.src.buffer.qprInfo || null, volume: opt().vol };
  };

  // ------------------------------------------------------------------ marquee
  let np = null, npKey = '';
  function buildMarquee() {
    const top = document.getElementById('top'); if (!top || document.getElementById('np')) return;
    np = document.createElement('div'); np.id = 'np'; np.className = 'np hidden'; np.title = 'Click to pause or resume';
    np.innerHTML = '<span class="np-ic" aria-hidden="true"><i></i><i></i><i></i></span><div class="np-view"><div class="np-track"></div></div>';
    const chips = document.getElementById('chips'); top.insertBefore(np, chips);
    np.addEventListener('click', () => { M.toggle(); });
    if (window.ResizeObserver) new ResizeObserver(() => layoutMarquee(true)).observe(np.querySelector('.np-view'));
  }
  function layoutMarquee(force) {
    if (!np) return;
    const text = nowText(M.cur), calm = document.body.classList.contains('calm');
    const view = np.querySelector('.np-view'), track = np.querySelector('.np-track');
    const key = text + '|' + calm + '|' + view.clientWidth;
    if (!force && key === npKey) return;
    npKey = key;
    np.classList.toggle('static', calm);
    const esc2 = s => String(s).replace(/[&<>]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;' }[c]));
    if (calm) { track.innerHTML = '<span class="np-t">' + esc2(text) + '</span>'; track.style.removeProperty('--np-shift'); return; }
    const unit = '<span class="np-t">' + esc2(text) + '</span><span class="np-sep">\u25C6</span>';
    track.innerHTML = unit;
    const w = track.scrollWidth || 1, vw = view.clientWidth || 1;
    const copies = Math.max(2, Math.ceil(vw / w) + 1);
    track.innerHTML = unit.repeat(copies);
    const rem = parseFloat(getComputedStyle(document.documentElement).fontSize) || 16;
    track.style.setProperty('--np-shift', (-w) + 'px');
    track.style.setProperty('--np-dur', (w / (rem * 4.4)).toFixed(2) + 's');     // about 4.4rem per second
  }
  function renderMarquee() {
    if (!np) return;
    const show = enabled() && !!M.cur;
    np.classList.toggle('hidden', !show);
    np.classList.toggle('paused', effectivePaused() || M.blocked);
    np.classList.toggle('loading', M.loading);
    if (show) layoutMarquee(false);
  }

  // ------------------------------------------------------------------ Launcher page card
  M.card = function () {
    const list = tracks();
    const hint = '<p class="mhint">Controller: X pauses or resumes, LT and RT skip back and forward. Keyboard: M, B and N. Click the scrolling title to pause.</p>';
    if (!list.length) {
      return '<div class="card"><h3>Music player</h3><p>No soundtrack tracks found. Use the soundtrack in your prepared <span class="mono">game/ATV.zip</span> or put <span class="mono">Track1.wav</span>, <span class="mono">Track2.wav</span> and so on in <span class="mono">game/Licensed</span>.</p></div>';
    }
    const st = M.state();
    const now = M.error ? `<small>Problem</small>${esc(M.error)}` : (M.cur ? `<small>${st.paused ? 'Paused' : (M.loading ? 'Loading' : 'Now playing')}</small>${esc(nowText(M.cur))}` : '<small>Music</small>Nothing playing');
    const rows = list.map(t => `<div class="f li${M.cur && M.cur.n === t.n ? ' now' : ''}" tabindex="0" data-fid="m-t${t.n}" data-act="musicPlay" data-arg="${t.n}"><span class="nm">${esc(t.artist ? t.artist + ' \u2014 ' + t.title : t.title)}</span><span class="meta">Track ${t.n}</span></div>`).join('');
    return `<div class="card" id="music-card"><h3><span>Music player</span><span class="muted" style="letter-spacing:0;text-transform:none;font-size:1.1rem">${list.length} track${list.length > 1 ? 's' : ''}</span></h3>
      <div class="mnow">${now}</div>
      <div class="btns"><button class="f btn" data-fid="m-prev" data-act="musicPrev">Previous</button><button class="f btn primary" data-fid="m-toggle" data-act="musicToggle">${st.paused ? 'Play' : 'Pause'}</button><button class="f btn" data-fid="m-next" data-act="musicNext">Next</button></div>
      <div class="mlist">${rows}</div>${hint}</div>`;
  };

  // ------------------------------------------------------------------ events
  let emitPending = false;
  function emit() {
    renderMarquee();
    if (emitPending) return; emitPending = true;
    requestAnimationFrame(() => { emitPending = false; if (S.page === 'launcher' && !S.overlay && !S.listen) softRender(); });
  }

  M.sync = function () {
    if (!M.ready) return;
    const o = opt(), en = enabled(), prev = M.applied;
    const sig = JSON.stringify(tracks());
    if (sig !== M.sig) {
      M.sig = sig;
      M.queue = M.queue.filter(n => byN(n));
      M.failed.clear();
      M.error = '';
      M.cache.clear();
      if (M.cur && !byN(M.cur.n)) { ++M.token; M.loading = false; M.cur = null; stopSource(); }
      else if (M.cur) M.cur = byN(M.cur.n);
    }
    if (prev.adpcm !== undefined && prev.adpcm !== o.adpcm) {
      M.cache.clear(); M.failed.clear(); M.warned.clear(); M.error = '';
      if (M.cur && en) start(M.cur.n);
    }
    if (prev.shuffle !== undefined && prev.shuffle !== o.shuffle) M.queue = [];
    if (prev.vol !== undefined && prev.vol !== o.vol && M.ctx && !effectivePaused()) ramp(gainFor(), .15);
    if (prev.on !== o.on) {
      if (!o.on) { M.token++; stopSource(); M.loading = false; if (M.ctx) applyPause(); }
      else if (en && !M.cur) advance(true);
    }
    if (en && !M.cur && !M.loading && !M.error && !M.failed.size) advance(true);
    if (en && M.cur && !M.src && !M.loading && !M.error) start(M.cur.n);
    M.applied = o;
    const calm = document.body.classList.contains('calm'); if (calm !== M.calm) { M.calm = calm; layoutMarquee(true); }
    renderMarquee();
  };

  M.syncJob = function () {
    const j = S.job;
    if (j && j.state === 'running' && !j.buildOnly && (j.gameRunning || j.phase === 'run')) M.hold('game');
    else M.release('game');
  };

  M.keys = function (event) {
    if (!M.ready || event.repeat || S.listen || S.overlay) return;
    M.nudge();
    const key = event.key.toLowerCase();
    if (key === 'm') M.toggle();
    else if (key === 'n') M.next();
    else if (key === 'b') M.prev();
  };

  M.pad = function (edge, any) {
    if (!M.ready || S.overlay) return;
    if (any) M.nudge();
    if (edge(2)) M.toggle();
    if (edge(6)) M.prev();
    if (edge(7)) M.next();
  };

  M.init = function () {
    buildMarquee();
    M.ready = true;
    M.syncJob();
    M.sync();
    document.addEventListener('pointerdown', M.nudge, { passive: true });
    if (document.fonts && document.fonts.ready) document.fonts.ready.then(() => layoutMarquee(true));
  };
})();
