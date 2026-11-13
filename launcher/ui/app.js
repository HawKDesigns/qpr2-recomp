'use strict';
/* ATV: Quad Power Racing 2 launcher UI.
 * Talks to Launcher.ps1 over a token-guarded localhost API. Add ?demo to the URL (served by any static
 * server) to run without the backend, e.g. ?demo=firstrun or ?demo=broken. */

// ------------------------------------------------------------------ helpers
const $ = (s, r = document) => r.querySelector(s);
const $$ = (s, r = document) => Array.from(r.querySelectorAll(s));
const esc = s => String(s == null ? '' : s).replace(/[&<>"']/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[c]));
const sleep = ms => new Promise(r => setTimeout(r, ms));
const clamp = (v, a, b) => Math.min(b, Math.max(a, v));
const fmtBytes = n => { n = +n || 0; if (n < 1024) return n + ' B'; const u = ['KB', 'MB', 'GB', 'TB']; let i = -1; do { n /= 1024; i++; } while (n >= 1024 && i < 3); return (n >= 100 ? n.toFixed(0) : n.toFixed(1)) + ' ' + u[i]; };
const fmtDur = s => { s = Math.max(0, Math.round(+s || 0)); const h = Math.floor(s / 3600), m = Math.floor((s % 3600) / 60); if (h) return h + 'h ' + String(m).padStart(2, '0') + 'm'; if (m) return m + ' min'; return s + ' s'; };
const fmtClock = s => { s = Math.max(0, Math.round(+s || 0)); const h = Math.floor(s / 3600), m = Math.floor((s % 3600) / 60), x = s % 60; return (h ? h + ':' + String(m).padStart(2, '0') : m) + ':' + String(x).padStart(2, '0'); };
const fmtDate = iso => { const d = new Date(iso); if (isNaN(d)) return ''; return d.toLocaleString(undefined, { day: 'numeric', month: 'short', hour: '2-digit', minute: '2-digit' }); };
const fmtAgo = iso => { const d = new Date(iso); if (isNaN(d)) return 'Never'; const m = Math.round((Date.now() - d) / 60000); if (m < 2) return 'Just now'; if (m < 60) return m + ' min ago'; const h = Math.round(m / 60); if (h < 24) return h + ' h ago'; const dd = Math.round(h / 24); return dd === 1 ? 'Yesterday' : dd + ' days ago'; };

const ICON = (p, f) => `<svg viewBox="0 0 24 24" fill="${f ? 'currentColor' : 'none'}" stroke="${f ? 'none' : 'currentColor'}" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">${p}</svg>`;
const ICONS = {
  play: ICON('<path d="M7 4.5v15l13-7.5z"/>', true),
  performance: ICON('<path d="M4.5 18a8.5 8.5 0 1 1 15 0"/><path d="M12 14l4.2-5.2"/><circle cx="12" cy="14.5" r="1.2"/>'),
  audio: ICON('<path d="M4 9.5v5h4l5 4v-13l-5 4z"/><path d="M16.5 9a4 4 0 0 1 0 6"/><path d="M19 6.5a7.5 7.5 0 0 1 0 11"/>'),
  controls: ICON('<rect x="2.5" y="7" width="19" height="10.5" rx="5.2"/><path d="M7.5 10.2v4M5.5 12.2h4"/><circle cx="16" cy="11" r=".6"/><circle cx="18.2" cy="13.2" r=".6"/>'),
  data: ICON('<path d="M3 7.5A2.5 2.5 0 0 1 5.5 5H9l2 2.2h7.5A2.5 2.5 0 0 1 21 9.7v7.8a2.5 2.5 0 0 1-2.5 2.5h-13A2.5 2.5 0 0 1 3 17.5z"/>'),
  launcher: ICON('<path d="M4 7h9M17 7h3M4 17h3M11 17h9"/><circle cx="15" cy="7" r="2"/><circle cx="9" cy="17" r="2"/>'),
  about: ICON('<circle cx="12" cy="12" r="9"/><path d="M12 11v5.5M12 7.6v.1"/>'),
  display: ICON('<rect x="3" y="4.5" width="18" height="12" rx="2"/><path d="M8.5 20h7M12 16.5V20"/>'),
  buttons: ICON('<circle cx="12" cy="6" r="2.4"/><circle cx="12" cy="18" r="2.4"/><circle cx="6" cy="12" r="2.4"/><circle cx="18" cy="12" r="2.4"/>'),
  pad: ICON('<rect x="2.5" y="7" width="19" height="10.5" rx="5.2"/><path d="M7.5 10.2v4M5.5 12.2h4"/>'),
  kbd: ICON('<rect x="2.5" y="6" width="19" height="12" rx="2.5"/><path d="M6.5 10h.1M10 10h.1M13.5 10h.1M17 10h.1M7 14h10"/>')
};
const PAGES = [
  { id: 'play', label: 'Play' }, { id: 'display', label: 'Display' }, { id: 'performance', label: 'Performance' }, { id: 'audio', label: 'Audio' },
  { id: 'controls', label: 'Controls' }, { id: 'buttons', label: 'Buttons' }, { id: 'data', label: 'Saves & Data' }, { id: 'launcher', label: 'Launcher' }, { id: 'about', label: 'About' }
];
const PAGE_INFO = {
  display: ['Display', 'Window size, fullscreen, monitor and how the picture is drawn. The game renders at the original 640x480 (4:3) and is letterboxed to fit any window. A frame-rate cap and widescreen are not available.'],
  buttons: ['Buttons', 'Choose which button triggers each game control. A mapping applies to controllers and to the keyboard. Binding a button does not take it away from its own job, so to swap two buttons set both.'],
  performance: ['Performance', 'Choose the renderer and compare individual runtime paths. Reset this page to restore the clean v1 settings.'],
  audio: ['Audio', 'Buffering and mixer options for the emulated audio processor.'],
  controls: ['Controls', 'Choose which inputs reach the game, tune the sticks and triggers, and check that your controller is healthy.'],
  data: ['Saves & Data', 'Keep your progress safe and your disk tidy.'],
  launcher: ['Launcher', 'How this launcher looks and behaves.']
};
const TIPS = [
  'First launch builds the game from your own disc files. It can take a while.',
  'Shaders still on fallbacks? Play a 3D menu, a preview and a race, close the game, then launch again.',
  'Press Start (or P on the keyboard) to launch from anywhere.',
  'Press Y (or Delete) on a setting to put it back to its default.',
  'Use Benchmark for performance comparisons. Debug tracks guest addresses and adds diagnostics that can affect frame pacing.',
  'If the picture looks wrong after an update, clear the shader cache in Saves & Data.',
  'Saves are backed up before launch whenever they have changed.',
  'Hold Shift while starting to open the launcher when \u201cSkip launcher\u201d is on.'
];
const GLYPHS = {
  xbox: { confirm: ['A', 'A'], back: ['B', 'B'], alt: ['Y', 'Y'], lb: ['LB', 'pill'], rb: ['RB', 'pill'], start: ['Menu', 'pill'], select: ['View', 'pill'], dpad: ['D-pad', 'pill'] },
  ps: { confirm: ['\u2715', 'cross'], back: ['\u25CB', 'circle'], alt: ['\u25B3', 'triangle'], lb: ['L1', 'pill'], rb: ['R1', 'pill'], start: ['Options', 'pill'], select: ['Create', 'pill'], dpad: ['D-pad', 'pill'] },
  kbd: { confirm: ['Enter', 'key'], back: ['Esc', 'key'], alt: ['Del', 'key'], lb: ['PgUp', 'key'], rb: ['PgDn', 'key'], start: ['P', 'key'], select: ['F1', 'key'], dpad: ['\u2190\u2191\u2193\u2192', 'key'] }
};
const PAD_LABELS = {
  xbox: ['A', 'B', 'X', 'Y', 'LB', 'RB', 'LT', 'RT', 'View', 'Menu', 'L3', 'R3', '\u2191', '\u2193', '\u2190', '\u2192'],
  ps: ['\u2715', '\u25CB', '\u25A1', '\u25B3', 'L1', 'R1', 'L2', 'R2', 'Create', 'Options', 'L3', 'R3', '\u2191', '\u2193', '\u2190', '\u2192']
};
const ACCENTS = { orange: '#ff7a1a', yellow: '#ffd23a', cyan: '#25d0ff', red: '#ff4a5a' };

// ------------------------------------------------------------------ state
const S = {
  st: null, page: 'play', demo: false, job: null, jobNext: 0, jobLines: [], overlay: null, ovResolve: null, ovReturn: null,
  input: 'kbd', pad: { type: 'xbox', name: '', connected: false, devices: [] }, listen: null, mem: {}, autoAccent: null, logoDark: false, tipI: 0,
  pingFail: 0, busy: false, jobTimer: null, shownResult: null
};

// ------------------------------------------------------------------ API (+ demo)
async function api(path, body) {
  if (S.demo) return Mock.handle(path, body);
  const opt = body === undefined ? {} : { method: 'POST', headers: { 'X-QPR2': '1', 'Content-Type': 'application/json' }, body: JSON.stringify(body) };
  let r;
  try { r = await fetch(path, opt); } catch (e) { throw new Error('The launcher service is not responding.'); }
  let j = null;
  try { j = await r.json(); } catch (e) { throw new Error('Unexpected reply from the launcher service.'); }
  if (!r.ok || j.ok === false) throw new Error(j.error || ('Request failed (' + r.status + ').'));
  return j;
}
async function safe(fn, okMsg) {
  try { const r = await fn(); if (okMsg) toast(okMsg, 'ok'); return r; }
  catch (e) { toast(e.message || String(e), 'bad'); snd('error'); return null; }
}

// ------------------------------------------------------------------ settings access
const defs = () => S.st.schema.settings;
const def = id => defs().find(d => d.id === id);
const bagOf = id => (def(id).scope === 'ui' ? S.st.settings.ui : S.st.settings.values);
const isMod = id => Object.prototype.hasOwnProperty.call(bagOf(id), id);
const val = id => (isMod(id) ? String(bagOf(id)[id]) : String(S.st.defaults[id]));
async function setVal(id, v) {
  v = String(v);
  const bag = bagOf(id);
  const old = Object.prototype.hasOwnProperty.call(bag, id) ? bag[id] : undefined;
  if (v === String(S.st.defaults[id])) delete bag[id]; else bag[id] = v;
  applyUiPrefs(); softRender();
  try { const r = await api('/api/setting', { id, value: v }); S.st.settings = r.settings; }
  catch (e) { if (old === undefined) delete bag[id]; else bag[id] = old; applyUiPrefs(); softRender(); toast(e.message, 'bad'); }
}
function choiceIndex(d) { const v = val(d.id); return Math.max(0, d.choices.findIndex(c => String(c.value) === v)); }
function stepChoice(id, dir, wrap) {
  const d = def(id);
  if (d.type === 'toggle') { snd(val(id) === '1' ? 'off' : 'on'); return setVal(id, val(id) === '1' ? '0' : '1'); }
  let i = choiceIndex(d) + dir; const n = d.choices.length;
  if (wrap) i = (i + n) % n;
  else if (i < 0 || i >= n) { snd('edge'); return; }
  snd('move'); return setVal(id, d.choices[i].value);
}

// ------------------------------------------------------------------ rendering: shared
function glyph(name) {
  const set = S.input === 'pad' ? (S.st && val('ui_glyphs') !== 'auto' ? val('ui_glyphs') : S.pad.type) : (S.st && val('ui_glyphs') !== 'auto' ? val('ui_glyphs') : 'kbd');
  const g = (GLYPHS[set] || GLYPHS.kbd)[name];
  return `<span class="glyph ${g[1] === 'pill' || g[1] === 'key' ? g[1] : g[1]}">${esc(g[0])}</span>`;
}
function wordmark(big) {
  return big
    ? `<div class="hero-word">ATV Quad<b>Power Racing 2</b></div>`
    : `<div class="wordmark">ATV <b>Quad Power</b> Racing 2<small>RECOMP LAUNCHER</small></div>`;
}
function logoImg(cls) {
  const a = S.st && S.st.art;
  if (!a || !a.logo) return null;
  return `<img class="${cls}${S.logoDark ? ' logo-dark' : ''}" alt="ATV Quad Power Racing 2" src="/assets/logo?v=${a.logoVersion}" onerror="logoFailed(this)" onload="logoLoaded(this)">`;
}
window.logoFailed = img => { const big = img.classList.contains('hero-logo'); img.outerHTML = wordmark(big); };
window.logoLoaded = img => {
  try {
    const c = document.createElement('canvas'); const w = 48, h = Math.max(8, Math.round(48 * img.naturalHeight / img.naturalWidth)); c.width = w; c.height = h;
    const x = c.getContext('2d'); x.drawImage(img, 0, 0, w, h); const d = x.getImageData(0, 0, w, h).data; let sum = 0, n = 0;
    for (let i = 0; i < d.length; i += 4) { const al = d[i + 3] / 255; if (al > .5) { sum += (0.2126 * d[i] + 0.7152 * d[i + 1] + 0.0722 * d[i + 2]) / 255; n++; } }
    const dark = n > 20 && sum / n < 0.3;
    if (dark !== S.logoDark) { S.logoDark = dark; $$('img.hero-logo, .brand img').forEach(e => e.classList.toggle('logo-dark', dark)); }
  } catch (e) { /* canvas unavailable: leave as is */ }
};
function badgeHTML(t) { return t ? `<span class="badge">${esc(t)}</span>` : ''; }

function srcLabel(d, v) { const c = d.choices.find(x => String(x.value) === String(v)); return c ? c.label : v; }
function disabledDependency(d) {
  for (let id = d.requires; id; id = def(id).requires) {
    if (val(id) !== '1') return def(id);
  }
  return null;
}
function settingRow(d) {
  const v = val(d.id), mod = isMod(d.id);
  const dependency = disabledDependency(d);
  const dim = !!dependency;
  let ctl, extra = '';
  if (d.widget === 'bind') {
    const own = srcLabel(d, d.control);
    return `<div class="f row" tabindex="0" data-fid="s-${d.id}" data-id="${d.id}" data-kind="bind">
      <div class="txt"><div class="lbl">${mod ? '<i class="mod" title="Changed from default"></i>' : ''}${esc(d.label)}</div></div>
      <div class="bindval ${mod ? '' : 'def'}">${esc(mod ? srcLabel(d, v) : own)}${mod ? '' : '<small>Default</small>'}</div></div>`;
  }
  if (d.type === 'toggle') ctl = `<div class="switch ${v === '1' ? 'on' : ''}"><span>${v === '1' ? 'On' : 'Off'}</span><div class="trk"></div></div>`;
  else {
    ctl = `<div class="seg">${d.choices.map(c => `<div class="opt ${String(c.value) === v ? 'on' : ''}" data-val="${esc(c.value)}">${esc(c.label)}</div>`).join('')}</div>`;
    const cur = d.choices[choiceIndex(d)];
    if (d.id === 'audio_latency' && cur && cur.note) {
      const m = /(\d+)\s*x\s*(\d+)/.exec(cur.note);
      extra = `<div class="extra">${esc(cur.note)} buffers${m ? ' \u00b7 about ' + Math.round(m[1] * m[2] / 48) + ' ms of audio queued' : ''}</div>`;
    }
  }
  if (dependency) extra += `<div class="extra" style="color:var(--faint)">Applies when ${esc(dependency.label)} is on.</div>`;
  return `<div class="f row${dim ? ' dim' : ''}" tabindex="0" data-fid="s-${d.id}" data-id="${d.id}" data-kind="${d.type}">
    <div class="txt"><div class="lbl">${mod ? '<i class="mod" title="Changed from default"></i>' : ''}${esc(d.label)}${badgeHTML(d.badge)}</div>${d.help ? `<div class="help">${esc(d.help)}</div>` : ''}${extra}</div>${ctl}</div>`;
}
function settingsGroups(pageId) {
  const list = defs().filter(d => d.page === pageId && d.id !== 'play_mode');
  const order = []; list.forEach(d => { if (!order.includes(d.group)) order.push(d.group); });
  return order.map(g => {
    const rows = list.filter(d => d.group === g), binds = rows.every(d => d.widget === 'bind');
    return `<div class="group-title">${esc(g)}</div>` + (binds ? `<div class="bindgrid">${rows.map(settingRow).join('')}</div>` : rows.map(settingRow).join(''));
  }).join('');
}
function pageHead(id) { const [t, s] = PAGE_INFO[id]; return `<h1 class="h1">${esc(t)}</h1><p class="sub">${esc(s)}</p>`; }
function resetBtn(pageId) {
  const any = defs().some(d => d.page === pageId && isMod(d.id));
  return any ? `<div class="btns" style="margin-top:2rem"><button class="f btn" data-fid="reset-${pageId}" data-act="resetPage" data-arg="${pageId}">Reset this page to defaults</button></div>` : '';
}

// ------------------------------------------------------------------ rendering: pages
function checkIcon(s) { return { ok: '\u2713', warn: '!', bad: '\u2715', todo: '\u2026' }[s] || '?'; }
function renderPlay() {
  const st = S.st, mode = val('play_mode'), md = def('play_mode');
  const modeNote = (md.choices.find(c => c.value === mode) || {}).note || '';
  const label = !st.canPlay ? 'Setup needed' : (st.firstRun ? 'Build & Play' : (mode === 'Benchmark' ? 'Run benchmark' : mode === 'Debug' ? 'Launch debug' : 'Play'));
  const hero = logoImg('hero-logo') || wordmark(true);
  const cur = st.stats && st.stats.lastPlayedUtc ? fmtAgo(st.stats.lastPlayedUtc) : 'Never';
  const last = (st.runs || [])[0];
  const lastPill = last ? `<span class="pill ${last.exitCode === 0 || last.status === 'exited' ? 'ok' : (last.status === 'timed_out' ? 'warn' : 'bad')}">${esc(last.status === 'exited' ? 'Exited cleanly' : last.status.replace('_', ' '))}</span>` : '';
  const banner = st.firstRun && st.canPlay
    ? `<div class="banner"><div>${ICONS.about.replace('<svg', '<svg width="22" height="22"')}</div><div><b>First launch takes a while.</b> The launcher translates your game executable and compiles it. It returns here when it is done.</div></div>` : '';
  return `<div class="play">
    <section class="hero">
      ${hero}
      <div class="tag">XBOX STATIC RECOMPILATION &nbsp;\u00b7&nbsp; EXPERIMENTAL</div>
      <button class="f playbtn ${st.canPlay ? '' : 'blocked'}" data-fid="play" data-act="${st.canPlay ? 'launch' : 'open'}" data-arg="game">${st.canPlay ? ICONS.play : ''}<span>${esc(label)}</span></button>
      <div class="modes">
        <div class="f seg" tabindex="0" data-fid="s-play_mode" data-id="play_mode" data-kind="choice">${md.choices.map(c => `<div class="opt ${c.value === mode ? 'on' : ''}" data-val="${esc(c.value)}">${esc(c.label)}</div>`).join('')}</div>
        <div class="note">${esc(modeNote)}</div>
      </div>
      <div class="btns sec-btns">${st.canPlay ? `<button class="f btn small" data-fid="build" data-act="build">Prepare / rebuild only</button>` : ''}<button class="f btn small" data-fid="opengame" data-act="open" data-arg="game">Open game folder</button></div>
    </section>
    <section>
      ${banner}
      <div class="card"><h3>Setup</h3><ul class="checks">${st.checks.filter(c => !(c.status === 'ok' && (c.id === 'runtime' || c.id === 'tools'))).map(c => `<li class="check"><span class="ic ${c.status}">${checkIcon(c.status)}</span><div><div class="nm">${esc(c.label)}</div><div class="dt">${esc(c.detail)}</div></div>${c.action ? `<button class="f btn small go" data-fid="chk-${c.id}" data-act="open" data-arg="game">Open folder</button>` : ''}</li>`).join('')}</ul></div>
      <div class="card"><h3><span>Activity</span>${lastPill}</h3>
        <div class="stats"><div class="stat"><div class="v">${fmtDur(st.stats.seconds || 0)}</div><div class="k">Play time</div></div><div class="stat"><div class="v">${st.stats.sessions || 0}</div><div class="k">Sessions</div></div><div class="stat"><div class="v" style="font-size:1.5rem;padding-top:.35rem;white-space:nowrap">${esc(cur)}</div><div class="k">Last played</div></div></div>
      </div>
    </section></div>`;
}

function renderControls() {
  const names = PAD_LABELS[S.pad.type];
  return pageHead('controls') + settingsGroups('controls') + resetBtn('controls') + `
    <div class="group-title">Controller test</div>
    <div class="card"><div class="tester">
      <div class="sticks"><div class="stick"><div class="ring"><i id="t-ls"></i></div><small id="t-lsv">L  0.00 / 0.00</small></div><div class="stick"><div class="ring"><i id="t-rs"></i></div><small id="t-rsv">R  0.00 / 0.00</small></div></div>
      <div><div class="btnmap" id="t-btns">${names.map((n, i) => `<div class="bt" data-i="${i}">${esc(n)}</div>`).join('')}</div>
      <div class="trig"><div><div class="bar"><i id="t-lt"></i></div></div><div><div class="bar"><i id="t-rt"></i></div></div></div></div>
    </div><p class="tester-note" id="t-note">No controller detected. Press any button on your controller to wake it.</p><p class="tester-note" id="t-dev" style="margin-top:.4rem"></p></div>`;
}

function backupLabel(b) {
  const m = /^(manual|auto|prerestore)-(\d{4})(\d{2})(\d{2})-(\d{2})(\d{2})(\d{2})/.exec(b.name);
  const kind = { manual: 'Manual', auto: 'Automatic', prerestore: 'Before restore' }[b.kind === 'safety' ? 'prerestore' : b.kind] || 'Backup';
  return m ? `${kind} \u00b7 ${fmtDate(`${m[2]}-${m[3]}-${m[4]}T${m[5]}:${m[6]}:${m[7]}`)}` : b.name;
}
function renderData() {
  const st = S.st, sz = st.sizes || {};
  const bk = (st.backups || []).slice(0, 5);
  const runs = (st.runs || []).slice(0, 4);
  return pageHead('data') + settingsGroups('data') + `
    <div class="group-title">Saves</div>
    <div class="card"><h3><span>Save folder</span><span class="muted" style="letter-spacing:0;text-transform:none;font-size:1.1rem">${fmtBytes(sz.saves)} in hdd</span></h3>
      <div class="btns"><button class="f btn primary" data-fid="bk-now" data-act="backup">Back up now</button><button class="f btn" data-fid="open-saves" data-act="open" data-arg="saves">Open save folder</button><button class="f btn" data-fid="open-backups" data-act="open" data-arg="backups">Open backups</button></div>
      <div style="margin-top:1.6rem">${bk.length ? bk.map(b => `<div class="f li" tabindex="0" data-fid="bk-${esc(b.name)}" data-act="backupMenu" data-arg="${esc(b.name)}"><span class="nm">${esc(backupLabel(b))}</span><span class="meta">${fmtBytes(b.bytes)}</span></div>`).join('') : '<div class="empty">No backups yet.</div>'}</div></div>
    <div class="group-title">Disk</div>
    <div class="two">
      <div class="card"><h3><span>Shader cache</span><span class="muted" style="letter-spacing:0;text-transform:none;font-size:1.1rem">${fmtBytes(sz.cache)}</span></h3><p class="muted" style="font-size:1.08rem;margin-bottom:1.2rem">Compiled GPU shaders. Safe to delete; they are rebuilt on the next run. Try this if the picture looks stale or corrupted.</p><div class="btns"><button class="f btn danger" data-fid="clr-cache" data-act="clearShaders">Clear shader cache</button></div></div>
      <div class="card"><h3><span>Run logs</span><span class="muted" style="letter-spacing:0;text-transform:none;font-size:1.1rem">${fmtBytes(sz.logs)}</span></h3><p class="muted" style="font-size:1.08rem;margin-bottom:1.2rem">Every run writes logs. Keeps the newest five and removes the rest.</p><div class="btns"><button class="f btn" data-fid="clr-logs" data-act="cleanLogs">Clean old logs</button><button class="f btn" data-fid="open-logs" data-act="open" data-arg="logs">Open</button></div></div>
    </div>
    <div class="group-title">Recent runs</div>
    <div class="card">${runs.length ? runs.map(r => `<div class="f li" tabindex="0" data-fid="run-${esc(r.id)}" data-act="openRun" data-arg="${esc(r.id)}"><span class="nm">${esc(fmtDate(r.startedUtc || r.modifiedUtc))} \u00b7 ${esc(r.mode || 'Play')}</span><span class="pill ${r.status === 'exited' ? 'ok' : (r.status === 'timed_out' ? 'warn' : (r.status === 'running' || r.status === 'built' ? '' : 'bad'))}">${esc(r.status.replace('_', ' '))}</span>${r.hasResultsZip ? '<span class="meta">results zip</span>' : ''}</div>`).join('') : '<div class="empty">No runs yet.</div>'}</div>
    ${resetBtn('data')}`;
}

function renderButtons() {
  return pageHead('buttons') + settingsGroups('buttons') + resetBtn('buttons') + `
    <div class="group-title">Keyboard layout</div>
    <div class="card"><div class="kbgrid"><div class="kb"><span class="key">Z</span><span>A</span></div><div class="kb"><span class="key">X</span><span>B</span></div><div class="kb"><span class="key">C</span><span>X</span></div><div class="kb"><span class="key">V</span><span>Y</span></div><div class="kb"><span class="key">Q</span><span>White</span></div><div class="kb"><span class="key">E</span><span>Black</span></div><div class="kb"><span class="key">1</span><span>Left trigger</span></div><div class="kb"><span class="key">3</span><span>Right trigger</span></div><div class="kb"><span class="key">W / S / A / D</span><span>Left stick</span></div><div class="kb"><span class="key">I / K / J / L</span><span>Right stick</span></div><div class="kb"><span class="key">Arrow keys</span><span>D-pad</span></div><div class="kb"><span class="key">Enter</span><span>Start</span></div><div class="kb"><span class="key">Backspace</span><span>Back</span></div><div class="kb"><span class="key">Shift</span><span>Left stick click</span></div><div class="kb"><span class="key">Ctrl</span><span>Right stick click</span></div></div>
    <p class="tester-note">The keyboard keys are fixed for now. The mapping above still applies to them, so a key can be made to do another button\u2019s job.</p></div>`;
}

function renderLauncher() {
  return pageHead('launcher') + `<div class="btns" style="margin-top:1.6rem"><button class="f btn" data-fid="fs" data-act="fullscreen">Toggle fullscreen <span class="muted">F11</span></button></div>` + settingsGroups('launcher') + resetBtn('launcher');
}

function renderAbout() {
  const a = S.st.app;
  return `<h1 class="h1">About</h1><p class="sub">ATV: Quad Power Racing 2 \u2014 an experimental native Windows recompilation of the original Xbox game, built on XboxRecomp. You supply the game files from your own disc.</p>
    <div class="group-title">Build</div>
    <div class="card"><dl class="kv"><dt>Launcher</dt><dd>${esc(a.version)}</dd><dt>Title ID</dt><dd>${esc(a.titleId)} (AC-010)</dd><dt>Supported XBE</dt><dd class="mono">${esc((a.xbeSha256 || '').slice(0, 16))}\u2026</dd><dt>Project folder</dt><dd class="mono">${esc(a.root)}</dd></dl></div>
    <div class="group-title">Tools</div>
    <div class="btns">
      <button class="f btn" data-fid="ab-root" data-act="open" data-arg="root">Open project folder</button>
      <button class="f btn" data-fid="ab-art" data-act="fetchArt">Refresh artwork</button>
      <button class="f btn" data-fid="ab-artdir" data-act="open" data-arg="art">Use your own artwork</button>
      ${S.st.app.platform === 'windows' ? '<button class="f btn" data-fid="ab-lnk" data-act="shortcut">Create desktop shortcut</button>' : ''}
    </div>
    <div class="btns" style="margin-top:1rem"><button class="f btn danger" data-fid="ab-reset" data-act="resetAll">Reset all settings</button><button class="f btn" data-fid="ab-quit" data-act="quit">Quit launcher</button></div>
    <div class="group-title">Notes</div>
    <div class="card"><p class="muted" style="font-size:1.1rem;margin:0">Independent fan project, unaffiliated with and not endorsed by Climax Brighton, Acclaim Entertainment, or Microsoft. No game files are included; everything is generated locally from your own disc. Artwork is downloaded to <span class="mono">launcher/assets</span> for personal use and stays out of version control. Drop your own <span class="mono">logo.png</span> and <span class="mono">background.jpg</span> there to replace it.</p></div>`;
}

function renderNav() {
  const st = S.st;
  const dot = (!st.canPlay) ? '<i class="dot bad"></i>' : ((st.checks || []).some(c => c.status === 'warn') ? '<i class="dot"></i>' : '');
  $('#nav').innerHTML = PAGES.map(p => `<button class="f nav-item ${p.id === S.page ? 'active' : ''}" data-page="${p.id}" data-fid="nav-${p.id}" aria-label="${esc(p.label)}">${ICONS[p.id]}<span>${esc(p.label)}</span>${p.id === 'play' ? dot : ''}</button>`).join('') + '<div class="nav-sp"></div>';
}
function renderBrand() {
  $('#brand').innerHTML = logoImg('') || wordmark(false);
}
function renderChips() {
  const connected = S.pad.connected;
  const name = connected ? (S.pad.name || 'Controller').replace(/\(.*?\)/g, '').replace(/\s+/g, ' ').trim().slice(0, 28) : 'Keyboard & mouse';
  const running = S.job && S.job.state === 'running';
  $('#chips').innerHTML = (running ? `<span class="chip live">${ICONS.play}${S.job.gameRunning ? 'Game running' : 'Working'}</span>` : '') +
    `<span class="chip ${connected ? 'live' : ''}">${connected ? ICONS.pad : ICONS.kbd}${esc(name)}</span><span class="clock" id="clock"></span>`;
  tickClock();
}
function tickClock() { const c = $('#clock'); if (c) c.textContent = new Date().toLocaleTimeString(undefined, { hour: '2-digit', minute: '2-digit' }); }

function pageHTML() {
  switch (S.page) {
    case 'play': return renderPlay();
    case 'controls': return renderControls();
    case 'data': return renderData();
    case 'buttons': return renderButtons();
    case 'launcher': return renderLauncher();
    case 'about': return renderAbout();
    default: return pageHead(S.page) + settingsGroups(S.page) + resetBtn(S.page);
  }
}
function renderPage(enter) {
  const pg = $('#page'); const fid = document.activeElement && document.activeElement.dataset ? document.activeElement.dataset.fid : null;
  pg.innerHTML = pageHTML();
  if (enter) { pg.classList.remove('enter'); void pg.offsetWidth; pg.classList.add('enter'); }
  document.body.dataset.page = S.page;
  if (fid && !S.overlay) { const el = $(`[data-fid="${CSS.escape(fid)}"]`); if (el && document.activeElement !== el) el.focus({ preventScroll: true }); }
  renderPrompts();
}
function softRender() { renderPage(false); renderNav(); }
function renderAll() { renderBrand(); renderNav(); renderChips(); renderPage(false); renderTip(true); }
function setPage(id, opts) {
  if (S.page === id) return;
  S.page = id; $$('.nav-item').forEach(n => n.classList.toggle('active', n.dataset.page === id));
  $('#main').scrollTop = 0; renderPage(true);
}
function renderTip(first) {
  const t = $('#tip'); if (!t) return;
  if (first) S.tipI = Math.floor(Math.random() * TIPS.length);
  t.textContent = TIPS[S.tipI % TIPS.length];
}
function renderPrompts() {
  const p = $('#prompts'); if (!p) return;
  const pr = (g, l) => `<span class="pr">${glyph(g)}${esc(l)}</span>`;
  let h;
  if (S.overlay === 'run' || S.overlay === 'result') h = pr('confirm', 'Select');
  else if (S.overlay === 'listen') h = `<span class="pr">Press a controller button</span>` + pr('back', 'Cancel');
  else if (S.overlay) h = pr('confirm', 'Select') + pr('back', 'Cancel');
  else {
    const el = document.activeElement; const row = el && el.dataset && el.dataset.id && isMod(el.dataset.id);
    h = pr('confirm', 'Select') + pr('back', 'Back') + pr('lb', 'Prev') + pr('rb', 'Next') + (row ? pr('alt', 'Reset') : '') + (S.st && S.st.canPlay ? pr('start', 'Play') : '') + pr('select', 'Help');
  }
  p.innerHTML = h;
}

// ------------------------------------------------------------------ overlays & toasts
function toast(msg, kind) {
  const t = document.createElement('div'); t.className = 'toast ' + (kind || ''); t.textContent = msg; $('#toasts').appendChild(t);
  setTimeout(() => { t.style.transition = 'opacity .4s'; t.style.opacity = 0; setTimeout(() => t.remove(), 450); }, 3600);
}
function openOverlay(kind, html) {
  if (!S.overlay) S.ovReturn = document.activeElement && document.activeElement.dataset ? document.activeElement.dataset.fid : null;
  S.overlay = kind; const o = $('#overlay'); o.innerHTML = html; o.classList.add('on'); renderPrompts();
  if (kind === 'run') { if (document.activeElement && document.activeElement.blur) document.activeElement.blur(); return; }
  const f = $('.f[data-first]', o) || $('.f', o); if (f) f.focus({ preventScroll: true });
}
function closeOverlay() {
  S.overlay = null; const o = $('#overlay'); o.classList.remove('on'); o.innerHTML = '';
  const r = S.ovResolve; S.ovResolve = null; if (r) r(null);
  renderPrompts();
  if (S.ovReturn) { const el = $(`[data-fid="${CSS.escape(S.ovReturn)}"]`); if (el) el.focus({ preventScroll: true }); }
}
function confirmDlg(title, body, okLabel, danger) {
  return new Promise(res => {
    openOverlay('confirm', `<div class="dlg"><h2>${esc(title)}</h2><p>${esc(body)}</p><div class="btns"><button class="f btn" data-first data-fid="dlg-no" data-act="dlg" data-arg="0">Cancel</button><button class="f btn ${danger ? 'danger' : 'primary'}" data-fid="dlg-yes" data-act="dlg" data-arg="1">${esc(okLabel)}</button></div></div>`);
    S.ovResolve = v => res(v === '1');
  });
}
function menuDlg(title, items) {
  return new Promise(res => {
    openOverlay('menu', `<div class="dlg"><h2>${esc(title)}</h2><div class="menu">${items.map((it, i) => `<button class="f btn ${it.danger ? 'danger' : ''}" ${i === 0 ? 'data-first' : ''} data-fid="mn-${i}" data-act="dlg" data-arg="${i}">${esc(it.label)}</button>`).join('')}<button class="f btn" data-fid="mn-x" data-act="dlg" data-arg="-1">Cancel</button></div></div>`);
    S.ovResolve = v => res(v === null ? -1 : +v);
  });
}
function helpDlg() {
  const row = (g, t) => `<span class="pr">${glyph(g)}${esc(t)}</span>`;
  openOverlay('help', `<div class="dlg"><h2>Controls</h2><div class="helpgrid">${row('dpad', 'Move around')}${row('confirm', 'Select / change a value')}${row('back', 'Back to the menu')}${row('alt', 'Reset a setting')}${row('lb', 'Previous page')}${row('rb', 'Next page')}${row('start', 'Launch the game')}${row('select', 'This help')}</div><p>Left and right change a setting\u2019s value. The right stick scrolls long pages. Mouse and keyboard work too, and F11 toggles fullscreen.</p><div class="btns"><button class="f btn primary" data-first data-fid="dlg-ok" data-act="dlg" data-arg="1">Got it</button></div></div>`);
}

// ------------------------------------------------------------------ job overlay
const STEP_ORDER = ['prepare', 'build', 'run'];
function runOverlayHTML(j) {
  const steps = j.buildOnly ? ['prepare', 'build'] : STEP_ORDER; const names = { prepare: 'Prepare', build: 'Build', run: 'Play' };
  return `<div class="dlg run"><div class="steps">${steps.map(s => `<span data-step="${s}">${names[s]}</span>`).join('')}</div>
    <div class="phase" id="r-phase"></div><div class="timer" id="r-timer"></div><div class="prog" id="r-prog"><i></i></div>
    <p class="msg" id="r-msg"></p><div class="logbox" id="r-log"></div>
    <div class="btns" style="margin-top:2rem"><button class="f btn danger" data-fid="job-stop" data-act="stop">Stop</button></div></div>`;
}
function resultHTML(j) {
  const r = j.result || {}; const o = r.outcome;
  const cfg = {
    ok: ['ok', '\u2713', r.sessionSeconds > 60 ? 'Nice run' : 'Run finished', r.sessionSeconds > 5 ? `You played for ${fmtDur(r.sessionSeconds)}.` : 'The game closed.'],
    built: ['ok', '\u2713', 'Build complete', 'Everything is prepared. You can play now.'],
    stopped: ['warn', '\u25A0', 'Stopped', 'The run was stopped.'],
    timeout: ['warn', '!', 'Time limit reached', 'The game was stopped by the time limit set in Launcher \u203a Advanced.'],
    crashed: ['bad', '\u2715', 'The game closed unexpectedly', `Exit code ${r.exitCode}. Open the run folder for the logs, and share the results zip when reporting a problem.`],
    build_failed: ['bad', '\u2715', j.buildOnly ? 'Build failed' : 'Could not start', r.message || 'The build did not finish. The log below shows where it stopped.']
  }[o] || ['warn', '?', 'Finished', ''];
  const showLog = o === 'build_failed' || o === 'crashed';
  const lines = (S.jobLines || []).slice(-9).map(l => `<div class="${/error|fail|exception|0xC0/i.test(l) ? 'err' : ''}">${esc(l)}</div>`).join('');
  const again = (o === 'ok' || o === 'built') ? '' : `<button class="f btn" data-fid="job-retry" data-act="retry">Try again</button>`;
  const playNow = o === 'built' ? `<button class="f btn primary" data-first data-fid="job-play" data-act="playNow">Play now</button>` : '';
  const logsBtn = r.runId ? `<button class="f btn" data-fid="job-open" data-act="openRun" data-arg="${esc(r.runId)}">Open run folder</button>` : `<button class="f btn" data-fid="job-open" data-act="open" data-arg="logs">Open logs</button>`;
  return `<div class="dlg run"><div class="big-ic ${cfg[0]}">${cfg[1]}</div><div class="phase">${esc(cfg[2])}</div><p class="msg">${esc(cfg[3])}</p>
    ${showLog ? `<div class="logbox">${lines}</div>` : ''}
    <div class="btns" style="margin-top:2rem">${playNow}<button class="f btn ${playNow ? '' : 'primary'}" ${playNow ? '' : 'data-first'} data-fid="job-done" data-act="dismiss">Back to launcher</button>${o === 'ok' || o === 'built' || o === 'stopped' ? '' : logsBtn}${again}</div></div>`;
}
function updateRunOverlay(j) {
  const set = (id, t) => { const e = $(id); if (e && e.textContent !== t) e.textContent = t; };
  set('#r-phase', j.gameRunning ? 'Game running' : (j.phaseText || 'Working'));
  set('#r-timer', j.gameRunning ? 'Session ' + fmtClock(j.sessionSeconds) : 'Elapsed ' + fmtClock(j.elapsed));
  const prog = $('#r-prog'); if (prog) prog.style.display = j.gameRunning ? 'none' : '';
  set('#r-msg', j.gameRunning ? 'The launcher comes back when you quit the game.' : '');
  const cur = j.phase === 'finish' ? 'run' : j.phase; const ci = STEP_ORDER.indexOf(cur);
  $$('.steps span').forEach(sp => { const i = STEP_ORDER.indexOf(sp.dataset.step); sp.className = i < ci ? 'done' : (i === ci ? 'cur' : ''); });
  const lb = $('#r-log'); if (lb) lb.innerHTML = (S.jobLines || []).slice(-8).map(l => `<div class="${/error|fail|exception|0xC0/i.test(l) ? 'err' : ''}">${esc(l)}</div>`).join('');
}
async function pollJob() {
  clearTimeout(S.jobTimer);
  let j;
  try { j = await api('/api/job?since=' + S.jobNext); S.pingFail = 0; } catch (e) { S.jobTimer = setTimeout(pollJob, 2000); return; }
  S.job = j;
  if (j.state === 'idle') { if (S.overlay === 'run' || S.overlay === 'result') closeOverlay(); S.shownResult = null; renderChips(); return; }
  if (j.lines && j.lines.length) { S.jobLines = S.jobLines.concat(j.lines).slice(-80); }
  S.jobNext = j.next;
  if (j.state === 'running') {
    if (S.overlay !== 'run') { S.jobLines = S.jobLines.length ? S.jobLines : []; openOverlay('run', runOverlayHTML(j)); }
    updateRunOverlay(j); renderChips();
    S.jobTimer = setTimeout(pollJob, 900);
  } else if (j.state === 'finished') {
    if (S.shownResult !== j.id) {
      S.shownResult = j.id; snd(j.result && (j.result.outcome === 'ok' || j.result.outcome === 'built') ? 'select' : 'error');
      openOverlay('result', resultHTML(j)); renderChips();
      const st = await safe(() => api('/api/state')); if (st) { S.st = st; applyArtAndTheme(); softRender(); renderBrand(); }
    }
  }
}
async function startJob(kind) {
  if (S.job && S.job.state === 'running') return;
  S.jobLines = []; S.jobNext = 0; S.shownResult = null;
  const r = await safe(() => api(kind === 'build' ? '/api/build' : '/api/launch', {}));
  if (r) { snd('launch'); S.job = { state: 'running', phase: 'prepare', phaseText: 'Checking your setup', buildOnly: kind === 'build', elapsed: 0 }; openOverlay('run', runOverlayHTML(S.job)); updateRunOverlay(S.job); pollJob(); }
}

// ------------------------------------------------------------------ actions
const Actions = {
  launch: () => startJob('launch'),
  build: () => startJob('build'),
  retry: async () => { const b = S.job && S.job.buildOnly; await api('/api/dismiss', {}); S.job = null; closeOverlay(); startJob(b ? 'build' : 'launch'); },
  playNow: async () => { await api('/api/dismiss', {}); S.job = null; closeOverlay(); startJob('launch'); },
  stop: async () => {
    const run = S.job && S.job.gameRunning;
    const ok = await confirmDlg(run ? 'Stop the game?' : 'Stop this run?', run ? 'The game closes immediately. Unsaved progress is lost.' : 'The build or setup in progress is cancelled. You can start it again later.', 'Stop', true);
    if (ok) await safe(() => api('/api/stop', {}));
  },
  dismiss: async () => { await safe(() => api('/api/dismiss', {})); S.job = null; S.jobLines = []; closeOverlay(); renderChips(); },
  open: async (t) => { await safe(() => api('/api/open', { target: t })); },
  openRun: async (id) => { await safe(() => api('/api/open', { target: 'run', run: id })); },
  dlg: (arg) => { const r = S.ovResolve; S.ovResolve = null; closeOverlay(); if (r) r(arg); },
  backup: async () => { const r = await safe(() => api('/api/saves/backup', {})); if (r) { toast('Backup saved.', 'ok'); await reloadState(); } },
  backupMenu: async (name) => {
    const b = S.st.backups.find(x => x.name === name); if (!b) return;
    const i = await menuDlg(backupLabel(b), [{ label: 'Restore this backup' }, { label: 'Delete this backup', danger: true }]);
    if (i === 0) {
      const ok = await confirmDlg('Restore this backup?', 'Your current saves will be replaced. A safety copy of them is made first, so you can undo this.', 'Restore', true);
      if (ok) { const r = await safe(() => api('/api/saves/restore', { name })); if (r) { toast('Saves restored. A safety copy was kept.', 'ok'); await reloadState(); } }
    } else if (i === 1) {
      const ok = await confirmDlg('Delete this backup?', 'This cannot be undone.', 'Delete', true);
      if (ok) { await safe(() => api('/api/saves/delete', { name }), 'Backup deleted.'); await reloadState(); }
    }
  },
  clearShaders: async () => {
    const ok = await confirmDlg('Clear the shader cache?', 'Compiled shaders are rebuilt on the next run, so the first minutes of play may stutter more than usual.', 'Clear cache', true);
    if (ok) { await safe(() => api('/api/cache/clear', { what: 'shaders' }), 'Shader cache cleared.'); await reloadState(); }
  },
  cleanLogs: async () => {
    const ok = await confirmDlg('Clean old logs?', 'Keeps the newest five runs and removes the rest, including their results zips.', 'Clean up', false);
    if (ok) { const r = await safe(() => api('/api/cache/clear', { what: 'logs' })); if (r) { toast(r.removed ? `Removed ${r.removed} old run${r.removed > 1 ? 's' : ''}.` : 'Nothing to remove.', 'ok'); await reloadState(); } }
  },
  fetchArt: async () => {
    toast('Downloading artwork\u2026');
    const r = await safe(() => api('/api/art/fetch', { force: true }));
    if (r) { const bad = Object.values(r.results).filter(v => v === 'failed').length; toast(bad ? 'Some artwork could not be downloaded. You can add your own images instead.' : 'Artwork updated.', bad ? 'bad' : 'ok'); await reloadState(); renderBrand(); applyArtAndTheme(); }
  },
  shortcut: async () => { const r = await safe(() => api('/api/shortcut', {})); if (r) toast('Shortcut added to your desktop.', 'ok'); },
  resetPage: async (p) => { const ok = await confirmDlg('Reset this page?', 'Every setting on this page goes back to its default.', 'Reset', false); if (ok) { const r = await safe(() => api('/api/reset', { scope: p })); if (r) { S.st.settings = r.settings; applyUiPrefs(); softRender(); toast('Defaults restored.', 'ok'); } } },
  resetAll: async () => { const ok = await confirmDlg('Reset all settings?', 'Launcher and game settings return to their defaults. Saves and backups are not touched.', 'Reset everything', true); if (ok) { const r = await safe(() => api('/api/reset', { scope: 'all' })); if (r) { S.st.settings = r.settings; applyUiPrefs(); applyArtAndTheme(); softRender(); toast('All settings reset.', 'ok'); } } },
  quit: async () => {
    const ok = await confirmDlg('Quit the launcher?', 'Any running game stays open.', 'Quit', false);
    if (!ok) return;
    await leaveFullscreen();
    await api('/api/quit', {}).catch(() => { });
    showFatal('Launcher closed', 'You can close this window.');
  },
  fullscreen: async () => { try { if (document.fullscreenElement) await document.exitFullscreen(); else await document.documentElement.requestFullscreen(); } catch (e) { toast('Press F11 to toggle fullscreen.'); } }
};

async function leaveFullscreen() {
  if (document.fullscreenElement) {
    // A browser-initiated exit can race this request during focus loss.
    await document.exitFullscreen().catch(() => { });
  }
}

function updateWindowFocus() {
  const inactive = document.hidden || !document.hasFocus();
  document.body.classList.toggle('inactive', inactive);
  if (inactive) void leaveFullscreen();
}
async function activate(el) {
  if (!el) return;
  if (el.classList.contains('nav-item')) { setPage(el.dataset.page); focusContent(); snd('select'); return; }
  const kind = el.dataset.kind;
  if (kind === 'bind') { snd('select'); bindMenu(el.dataset.id); return; }
  if (kind === 'toggle' || kind === 'choice') { stepChoice(el.dataset.id, 1, true); return; }
  const fn = Actions[el.dataset.act]; if (fn) { snd(el.dataset.act === 'dlg' && el.dataset.arg === '0' ? 'back' : 'select'); fn(el.dataset.arg, el); }
}
async function reloadState() { const st = await safe(() => api('/api/state')); if (st) { S.st = st; applyUiPrefs(); softRender(); renderBrand(); } }

// ------------------------------------------------------------------ button binding
const IDX_TO_SRC = ['a', 'b', 'x', 'y', 'black', 'white', 'lt', 'rt', 'back', 'start', 'l3', 'r3', 'dpad_up', 'dpad_down', 'dpad_left', 'dpad_right'];
async function bindSet(id, src) {
  const d = def(id), own = d.control;
  const v = src === own ? '' : src;                    // binding a control to itself is the default
  await setVal(id, v);
  if (v && v !== 'none') {
    const other = def('map_' + v);                     // offer the matching swap when it is still untouched
    if (other && val('map_' + v) === '' && v !== own) {
      const ok = await confirmDlg('Swap them?', `${d.label} now uses ${srcLabel(d, v)}. Also make ${other.label} use ${srcLabel(d, own)} so the two trade places?`, 'Swap', false);
      if (ok) await setVal('map_' + v, own);
    }
  }
}
async function bindMenu(id) {
  const d = def(id);
  const i = await menuDlg(`${d.label}: choose a button`, [{ label: 'Press a controller button\u2026' }, { label: 'Pick from a list\u2026' }, { label: 'Disable this control' }, { label: 'Use the default' }]);
  if (i === 0) startListen(id);
  else if (i === 1) {
    const list = d.choices.filter(c => c.value !== '');
    const k = await menuDlg(`${d.label} is triggered by`, list.map(c => ({ label: c.label })));
    if (k >= 0) bindSet(id, list[k].value);
  } else if (i === 2) bindSet(id, 'none');
  else if (i === 3) setVal(id, '');
}
function startListen(id) {
  const d = def(id);
  S.listen = { id, armed: false, until: performance.now() + 8000 };
  openOverlay('listen', `<div class="dlg run"><h2>${esc(d.label)}</h2><p>Press the controller button you want to use.</p><div class="phase" id="l-count">8</div>
    <p class="msg">Waiting for a button\u2026 Esc or the button below cancels.</p><div class="btns" style="margin-top:2rem"><button class="f btn" data-first data-fid="listen-cancel" data-act="cancelListen">Cancel</button></div></div>`);
}
function cancelListen() { if (!S.listen) return; S.listen = null; closeOverlay(); snd('back'); }
function padListen(gp, now) {
  const L = S.listen; if (!L) return;
  const left = Math.max(0, Math.ceil((L.until - now) / 1000)); const c = $('#l-count'); if (c && c.textContent !== String(left)) c.textContent = left;
  if (now > L.until) { cancelListen(); toast('No button pressed.'); return; }
  if (!gp) return;
  let pressed = -1; for (let i = 0; i < Math.min(16, gp.buttons.length); i++) if (gp.buttons[i] && gp.buttons[i].pressed) { pressed = i; break; }
  if (!L.armed) { if (pressed < 0) L.armed = true; return; }   // wait for the press that opened this dialog to be released
  if (pressed < 0) return;
  const id = L.id; S.listen = null; closeOverlay(); snd('select'); bindSet(id, IDX_TO_SRC[pressed]);
}
Actions.cancelListen = cancelListen;

// ------------------------------------------------------------------ theme, art, sound
function hexToRgb(h) { const n = parseInt(h.slice(1), 16); return [(n >> 16) & 255, (n >> 8) & 255, n & 255]; }
function setAccent(hex) {
  const [r, g, b] = hexToRgb(hex), mix = (c, t) => Math.round(c + (255 - c) * t);
  const r2 = document.documentElement.style;
  r2.setProperty('--accent', hex); r2.setProperty('--accent-rgb', `${r}, ${g}, ${b}`);
  r2.setProperty('--accent-2', `rgb(${mix(r, .28)}, ${mix(g, .28)}, ${mix(b, .28)})`);
  const lum = (0.2126 * r + 0.7152 * g + 0.0722 * b) / 255; r2.setProperty('--on-accent', lum > .5 ? '#16100a' : '#ffffff');
}
function applyUiPrefs() {
  if (!S.st) return;
  document.documentElement.style.setProperty('--ui', val('ui_scale'));
  document.body.classList.toggle('calm', val('ui_reduce_motion') === '1');
  const a = val('ui_accent'); setAccent(a === 'auto' ? (S.autoAccent || ACCENTS.orange) : (ACCENTS[a] || ACCENTS.orange));
}
function sampleAccent(img) {
  try {
    const w = 64, h = 36, c = document.createElement('canvas'); c.width = w; c.height = h; const x = c.getContext('2d'); x.drawImage(img, 0, 0, w, h);
    const d = x.getImageData(0, 0, w, h).data, bins = Array.from({ length: 24 }, () => ({ w: 0, r: 0, g: 0, b: 0 }));
    for (let i = 0; i < d.length; i += 4) {
      const r = d[i] / 255, g = d[i + 1] / 255, b = d[i + 2] / 255, mx = Math.max(r, g, b), mn = Math.min(r, g, b), l = (mx + mn) / 2, dl = mx - mn;
      if (dl < .08 || l < .12 || l > .92) continue;
      const s = dl / (1 - Math.abs(2 * l - 1)); let hh = mx === r ? ((g - b) / dl) % 6 : mx === g ? (b - r) / dl + 2 : (r - g) / dl + 4; hh = (hh * 60 + 360) % 360;
      const wgt = s * s * (1 - Math.abs(2 * l - 1)), k = bins[Math.floor(hh / 15) % 24]; k.w += wgt; k.r += d[i] * wgt; k.g += d[i + 1] * wgt; k.b += d[i + 2] * wgt;
    }
    const best = bins.reduce((p, q) => (q.w > p.w ? q : p), bins[0]); if (best.w < 1.5) return null;
    let r = best.r / best.w / 255, g = best.g / best.w / 255, b = best.b / best.w / 255;
    const mx = Math.max(r, g, b), mn = Math.min(r, g, b); let l = (mx + mn) / 2, s = (mx - mn) / (1 - Math.abs(2 * l - 1)) || 0;
    s = clamp(s, .8, 1); l = clamp(l, .52, .62);
    const hue = (() => { const dl = mx - mn || 1; let hh = mx === r ? ((g - b) / dl) % 6 : mx === g ? (b - r) / dl + 2 : (r - g) / dl + 4; return (hh * 60 + 360) % 360; })();
    const cc = (1 - Math.abs(2 * l - 1)) * s, xx = cc * (1 - Math.abs(((hue / 60) % 2) - 1)), m = l - cc / 2; let rgb;
    if (hue < 60) rgb = [cc, xx, 0]; else if (hue < 120) rgb = [xx, cc, 0]; else if (hue < 180) rgb = [0, cc, xx]; else if (hue < 240) rgb = [0, xx, cc]; else if (hue < 300) rgb = [xx, 0, cc]; else rgb = [cc, 0, xx];
    return '#' + rgb.map(v => Math.round((v + m) * 255).toString(16).padStart(2, '0')).join('');
  } catch (e) { return null; }
}
function applyArtAndTheme() {
  const a = S.st.art, bg = $('.bg-img');
  if (a.background) {
    const url = `/assets/background?v=${a.backgroundVersion}`;
    if (bg.dataset.src !== url) {
      bg.dataset.src = url; const im = new Image();
      im.onload = () => {
        // Pre-blur once by downscaling: far cheaper than a live CSS blur on a full-screen layer.
        let src = url;
        try { const c = document.createElement('canvas'); c.width = 320; c.height = Math.max(90, Math.round(320 * im.naturalHeight / im.naturalWidth)); c.getContext('2d').drawImage(im, 0, 0, c.width, c.height); src = c.toDataURL('image/jpeg', .82); } catch (e) { /* use the original */ }
        bg.style.backgroundImage = `url("${src}")`; bg.classList.add('on'); S.autoAccent = sampleAccent(im); applyUiPrefs();
      };
      im.onerror = () => bg.classList.remove('on'); im.src = url;
    }
  } else { bg.classList.remove('on'); bg.dataset.src = ''; S.autoAccent = null; }
  applyUiPrefs();
}
let AC = null;
function snd(kind) {
  if (!S.st || val('ui_sounds') !== '1') return;
  try {
    AC = AC || new (window.AudioContext || window.webkitAudioContext)(); if (AC.state === 'suspended') AC.resume();
    const tone = (f, t0, dur, type, vol) => { const o = AC.createOscillator(), g = AC.createGain(); o.type = type || 'sine'; o.frequency.setValueAtTime(f, AC.currentTime + t0); g.gain.setValueAtTime(0.0001, AC.currentTime + t0); g.gain.exponentialRampToValueAtTime(vol, AC.currentTime + t0 + .008); g.gain.exponentialRampToValueAtTime(0.0001, AC.currentTime + t0 + dur); o.connect(g); g.connect(AC.destination); o.start(AC.currentTime + t0); o.stop(AC.currentTime + t0 + dur + .02); };
    switch (kind) {
      case 'move': tone(720, 0, .045, 'sine', .028); break;
      case 'edge': tone(300, 0, .05, 'sine', .02); break;
      case 'select': tone(520, 0, .06, 'triangle', .04); tone(780, .05, .09, 'triangle', .04); break;
      case 'back': tone(420, 0, .07, 'triangle', .035); tone(300, .05, .08, 'triangle', .03); break;
      case 'on': tone(600, 0, .06, 'triangle', .04); tone(900, .05, .07, 'triangle', .04); break;
      case 'off': tone(520, 0, .06, 'triangle', .035); tone(360, .05, .07, 'triangle', .03); break;
      case 'launch': [392, 523, 659, 784].forEach((f, i) => tone(f, i * .07, .12, 'triangle', .045)); break;
      case 'error': tone(170, 0, .16, 'square', .03); break;
    }
  } catch (e) { /* audio unavailable */ }
}

// ------------------------------------------------------------------ input: spatial navigation
const zoneOf = el => (el.closest('#nav') ? 'nav' : (el.closest('#overlay') ? 'ov' : 'main'));
function focusables() {
  const root = S.overlay ? $('#overlay') : document;
  return $$('.f', root).filter(e => e.offsetParent !== null && !e.disabled && !e.classList.contains('off'));
}
function focusEl(el, noSound) {
  if (!el) return; el.focus({ preventScroll: true });
  if (el.closest('#main') || el.closest('#overlay')) el.scrollIntoView({ block: 'nearest', behavior: document.body.classList.contains('calm') ? 'auto' : 'smooth' });
  if (!noSound) snd('move');
}
function focusContent() {
  const mem = S.mem[S.page]; let el = mem && $(`#main [data-fid="${CSS.escape(mem)}"]`);
  if (!el) el = $('#main .f'); if (el) focusEl(el, true);
}
function focusNav() { const a = $('.nav-item.active') || $('.nav-item'); if (a) a.focus({ preventScroll: true }); }
function move(dir) {
  const cur = document.activeElement && document.activeElement.classList && document.activeElement.classList.contains('f') ? document.activeElement : null;
  if (!cur) { const list = focusables(); if (list[0]) focusEl(list[0]); return; }
  // Value controls use left/right to change their value.
  if ((dir === 'left' || dir === 'right') && (cur.dataset.kind === 'choice' || cur.dataset.kind === 'toggle')) { stepChoice(cur.dataset.id, dir === 'right' ? 1 : -1, false); return; }
  const z = zoneOf(cur), r = cur.getBoundingClientRect(), cx = r.left + r.width / 2, cy = r.top + r.height / 2;
  if (z === 'nav' && dir === 'right') { focusContent(); return; }
  let best = null, bs = Infinity;
  for (const el of focusables()) {
    if (el === cur) continue; const ez = zoneOf(el);
    if ((dir === 'up' || dir === 'down') && ez !== z) continue;
    if ((dir === 'left' || dir === 'right') && z === 'main' && ez === 'nav') continue;
    const e = el.getBoundingClientRect(), ex = e.left + e.width / 2, ey = e.top + e.height / 2;
    let prim, sec;
    if (dir === 'down') { prim = e.top - r.bottom + r.height * .5; sec = Math.abs(ex - cx); if (ey <= cy + 2) continue; }
    else if (dir === 'up') { prim = r.top - e.bottom + r.height * .5; sec = Math.abs(ex - cx); if (ey >= cy - 2) continue; }
    else if (dir === 'right') { prim = e.left - r.right + r.width * .5; sec = Math.abs(ey - cy); if (ex <= cx + 2) continue; }
    else { prim = r.left - e.right + r.width * .5; sec = Math.abs(ey - cy); if (ex >= cx - 2) continue; }
    const score = Math.max(prim, 0) + sec * 2.4; if (score < bs) { bs = score; best = el; }
  }
  if (best) focusEl(best); else if (dir === 'left' && z === 'main') focusNav(); else snd('edge');
}
function back() {
  if (S.overlay === 'listen') { cancelListen(); return; }
  if (S.overlay === 'run') return;
  if (S.overlay) { if (S.overlay === 'result') return; closeOverlay(); snd('back'); return; }
  const cur = document.activeElement;
  if (cur && cur.closest && cur.closest('#main')) { focusNav(); snd('back'); }
}
function resetFocused() {
  const el = document.activeElement; if (S.overlay || !el || !el.dataset || !el.dataset.id) return;
  if (isMod(el.dataset.id)) { snd('back'); setVal(el.dataset.id, S.st.defaults[el.dataset.id]); toast('Reset to default.'); }
}
function pagesStep(d) {
  if (S.overlay) return; const i = PAGES.findIndex(p => p.id === S.page), n = PAGES[(i + d + PAGES.length) % PAGES.length];
  setPage(n.id); snd('move'); const nav = document.activeElement && document.activeElement.closest && document.activeElement.closest('#nav');
  if (nav) { const a = $('.nav-item.active'); if (a) a.focus({ preventScroll: true }); } else focusContent();
}
function launchShortcut() { if (S.overlay) return; if (!S.st.canPlay) { toast('Setup needed: check the Play page.', 'bad'); snd('error'); return; } startJob('launch'); }
function setInput(m) {
  if (S.input === m) return; S.input = m; document.body.dataset.input = m;
  if (S.st) { renderPrompts(); }
}

document.addEventListener('keydown', e => {
  if (!S.st || e.ctrlKey || e.altKey || e.metaKey) return;
  setInput('kbd');
  const k = e.key;
  const map = { ArrowUp: 'up', ArrowDown: 'down', ArrowLeft: 'left', ArrowRight: 'right' };
  if (map[k]) { e.preventDefault(); move(map[k]); }
  else if (k === 'Enter' || k === ' ') { const el = document.activeElement; if (el && el.classList.contains('f')) { e.preventDefault(); activate(el); } }
  else if (k === 'Escape' || k === 'Backspace') { e.preventDefault(); back(); }
  else if (k === 'PageUp') { e.preventDefault(); pagesStep(-1); }
  else if (k === 'PageDown') { e.preventDefault(); pagesStep(1); }
  else if (k === 'Delete') { e.preventDefault(); resetFocused(); }
  else if (k === 'F1' || k === '?') { e.preventDefault(); if (!S.overlay) helpDlg(); }
  else if (k === 'p' || k === 'P') { launchShortcut(); }
});
let lastMouse = 0;
document.addEventListener('mousemove', e => { if (Math.abs(e.movementX) + Math.abs(e.movementY) < 2) return; lastMouse = Date.now(); setInput('mouse'); });
document.addEventListener('mouseover', e => {
  if (S.input !== 'mouse') return; const el = e.target.closest && e.target.closest('.f'); if (!el || el === document.activeElement) return;
  if (S.overlay ? !el.closest('#overlay') : el.closest('#overlay')) return; el.focus({ preventScroll: true });
});
document.addEventListener('click', e => {
  const el = e.target.closest && e.target.closest('.f'); if (!el) return; setInput('mouse');
  if (el.dataset.kind === 'choice') { const o = e.target.closest('.opt'); if (o && o.dataset.val !== undefined) { snd('select'); setVal(el.dataset.id, o.dataset.val); return; } }
  activate(el);
});
document.addEventListener('focusin', e => {
  const el = e.target; if (!el.classList || !el.classList.contains('f')) return;
  if (el.classList.contains('nav-item')) { if (S.input !== 'mouse') setPage(el.dataset.page); }
  else if (el.closest('#main') && el.dataset.fid) S.mem[S.page] = el.dataset.fid;
  renderPrompts();
});

// ------------------------------------------------------------------ input: gamepad
// Browsers list every HID device with buttons as a "gamepad" - headsets (e.g. Corsair HS80), dongles, mice.
// Score each one, ignore non-controllers, and follow whichever real controller is being used.
const PAD = { prevBy: {}, held: {}, active: -1, activeAt: 0, tester: null };
const NOT_A_PAD = /headset|headphone|earbud|earphone|corsair|\bhs\d{2}\b|arctis|astro|hyperx|razer (?:kraken|barracuda)|audio|speaker|microphone|\bmic\b|mouse|keyboard|receiver|dongle|trackpad|touchpad|hub/i;
const LOOKS_PAD = /xbox|xinput|045e|dualsense|dualshock|054c|pro controller|joy-?con|8bitdo|gamepad|controller/i;
function padScore(g) {
  const id = g.id || '';
  if (NOT_A_PAD.test(id) && !/xbox|xinput|dualsense|dualshock|pro controller/i.test(id)) return -1;
  let s = 0;
  if (g.mapping === 'standard') s += 100;
  if (g.axes.length >= 4) s += 30;
  if (g.buttons.length >= 10) s += 30;
  if (LOOKS_PAD.test(id)) s += 20;
  return s;
}
const PAD_MIN_SCORE = 60;
function padType(id) { id = id || ''; if (/xbox|045e/i.test(id)) return 'xbox'; return /054c|dualshock|dualsense|playstation|^wireless controller/i.test(id) ? 'ps' : 'xbox'; }
function padNiceName(id) { return String(id || 'Controller').replace(/\(.*?\)/g, '').replace(/\s+/g, ' ').trim().slice(0, 30) || 'Controller'; }
function padActivity(g) {
  for (let i = 0; i < g.buttons.length; i++) if (g.buttons[i] && g.buttons[i].pressed) return true;
  for (let i = 0; i < Math.min(4, g.axes.length); i++) if (Math.abs(g.axes[i]) > .6) return true;
  return false;
}
function padFire(name, now, down, first, rep) {
  const h = PAD.held;
  if (!down) { h[name] = 0; return false; }
  if (!h[name]) { h[name] = now + first; return true; }
  if (now >= h[name]) { h[name] = now + rep; return true; }
  return false;
}
function pickPad() {
  const all = navigator.getGamepads ? Array.from(navigator.getGamepads()).filter(g => g && g.connected) : [];
  const scored = all.map(g => ({ g, score: padScore(g) }));
  const usable = scored.filter(x => x.score >= PAD_MIN_SCORE);
  // The controller that is actually being pressed wins; otherwise the best-looking one.
  const t = performance.now();
  for (const x of usable) if (padActivity(x.g)) { PAD.active = x.g.index; PAD.activeAt = t; }
  let chosen = usable.find(x => x.g.index === PAD.active) || usable.sort((a, b) => b.score - a.score)[0] || null;
  S.pad.devices = scored.map(x => ({ name: padNiceName(x.g.id), used: !!chosen && chosen.g === x.g, ignored: x.score < PAD_MIN_SCORE }));
  return chosen ? chosen.g : null;
}
function padLoop(now) {
  requestAnimationFrame(padLoop);
  if (!S.st) return;
  const gp = pickPad();
  const was = S.pad.connected;
  if (!gp) { if (S.listen) padListen(null, now); if (was) { S.pad.connected = false; S.pad.name = ''; renderChips(); updateTester(null); } else if (S.page === 'controls') updateTester(null); return; }
  if (!was || S.pad.name !== gp.id) {
    S.pad.connected = true; S.pad.name = gp.id; S.pad.type = padType(gp.id); renderChips();
    if (S.page === 'controls' || S.page === 'buttons') renderPage(false);
  }
  const b = i => (gp.buttons[i] ? gp.buttons[i].pressed : false);
  const ax = i => gp.axes[i] || 0;
  const prev = PAD.prevBy[gp.index] || (PAD.prevBy[gp.index] = gp.buttons.map(x => x.pressed));
  const edge = i => b(i) && !prev[i];
  let any = false; for (let i = 0; i < gp.buttons.length; i++) if (b(i)) any = true;
  if (Math.hypot(ax(0), ax(1)) > .6) any = true;
  if (any) setInput('pad');
  if (S.listen) { padListen(gp, now); PAD.prevBy[gp.index] = gp.buttons.map(x => x.pressed); return; }
  if (S.input === 'pad') {
    const lx = ax(0), ly = ax(1), mag = Math.hypot(lx, ly), horiz = Math.abs(lx) > Math.abs(ly);
    const up = b(12) || (mag > .6 && !horiz && ly < 0), down = b(13) || (mag > .6 && !horiz && ly > 0), left = b(14) || (mag > .6 && horiz && lx < 0), right = b(15) || (mag > .6 && horiz && lx > 0);
    if (padFire('up', now, up, 420, 115)) move('up'); if (padFire('down', now, down, 420, 115)) move('down');
    if (padFire('left', now, left, 420, 130)) move('left'); if (padFire('right', now, right, 420, 130)) move('right');
    if (edge(0)) { const el = document.activeElement; if (el && el.classList.contains('f')) activate(el); else { const l = focusables(); if (l[0]) focusEl(l[0]); } }
    if (edge(1)) back(); if (edge(3)) resetFocused();
    if (edge(4)) pagesStep(-1); if (edge(5)) pagesStep(1);
    if (edge(9)) launchShortcut(); if (edge(8)) { if (!S.overlay) helpDlg(); }
    const ry = ax(3); if (Math.abs(ry) > .35 && !S.overlay) $('#main').scrollTop += ry * 26;
  }
  PAD.prevBy[gp.index] = gp.buttons.map(x => x.pressed);
  if (S.page === 'controls' && !S.overlay) updateTester(gp);
}
function updateTester(gp) {
  const note = $('#t-note'); if (!note) return;
  const dev = $('#t-dev'); if (dev) { const t = padDevicesText(); if (dev.textContent !== t) dev.textContent = t; }
  if (!gp) { note.textContent = 'No controller detected. Press any button on your controller to wake it.'; $$('.bt.on').forEach(e => e.classList.remove('on')); return; }
  const dot = (id, x, y) => { const e = $(id); if (e) { e.style.left = (50 + x * 38) + '%'; e.style.top = (50 + y * 38) + '%'; } };
  const lx = gp.axes[0] || 0, ly = gp.axes[1] || 0, rx = gp.axes[2] || 0, ry = gp.axes[3] || 0;
  dot('#t-ls', lx, ly); dot('#t-rs', rx, ry);
  const f = n => (n >= 0 ? ' ' : '') + n.toFixed(2);
  const a = $('#t-lsv'), c = $('#t-rsv'); if (a) a.textContent = `L ${f(lx)} / ${f(ly)}`; if (c) c.textContent = `R ${f(rx)} / ${f(ry)}`;
  $$('#t-btns .bt').forEach(e => { const i = +e.dataset.i; const on = gp.buttons[i] && gp.buttons[i].pressed; e.classList.toggle('on', !!on); });
  const lt = $('#t-lt'), rt = $('#t-rt'); if (lt) lt.style.width = ((gp.buttons[6] ? gp.buttons[6].value : 0) * 100) + '%'; if (rt) rt.style.width = ((gp.buttons[7] ? gp.buttons[7].value : 0) * 100) + '%';
  const idle = gp.buttons.every(x => !x.pressed), m = Math.max(Math.hypot(lx, ly), Math.hypot(rx, ry));
  const nm = padNiceName(gp.id);
  const dz = Math.max(+val('pad_dz_left') || 0, +val('pad_dz_right') || 0);
  note.textContent = idle && m < .35 ? (m > .1 ? `${nm}: a stick rests at ${m.toFixed(2)}. That is drift${dz ? '' : '; raise the stick deadzone in the settings above'}.` : `${nm}: sticks rest cleanly (${m.toFixed(2)}).`) : `${nm}: move the sticks and press buttons to check them. The test shows the raw controller, before your deadzone and mapping.`;
}
function padDevicesText() {
  const d = (S.pad.devices || []);
  if (!d.length) return '';
  const used = d.filter(x => x.used).map(x => x.name), ign = d.filter(x => x.ignored).map(x => x.name);
  return (used.length ? 'Using: ' + used.join(', ') : '') + (ign.length ? (used.length ? ' \u00b7 ' : '') + 'Ignored (not a game controller): ' + ign.join(', ') : '');
}

// ------------------------------------------------------------------ boot
function showFatal(title, msg) { const d = document.createElement('div'); d.className = 'fatal'; d.innerHTML = `<div><h2>${esc(title)}</h2><p>${esc(msg)}</p></div>`; document.body.appendChild(d); }

async function ping() {
  try { await api('/api/ping', {}); S.pingFail = 0; }
  catch (e) { if (++S.pingFail >= 3 && !S.demo) showFatal('Launcher stopped', 'The launcher service is no longer running. Start it again with QPR2 Launcher.bat.'); }
}

async function boot() {
  const q = new URLSearchParams(location.search);
  if (q.has('demo')) { S.demo = true; await Mock.init(q.get('demo')); }
  const st = await api('/api/state').catch(e => { showFatal('Could not reach the launcher', 'Start it with QPR2 Launcher.bat. ' + e.message); return null; });
  if (!st) return;
  S.st = st; document.body.dataset.input = 'kbd';
  applyUiPrefs(); applyArtAndTheme(); renderAll();
  setInterval(tickClock, 15000); setInterval(ping, 5000); setInterval(() => { S.tipI++; renderTip(false); }, 14000);
  window.addEventListener('blur', updateWindowFocus);
  window.addEventListener('focus', updateWindowFocus);
  document.addEventListener('fullscreenchange', updateWindowFocus);
  document.addEventListener('visibilitychange', () => {
    updateWindowFocus();
    if (!document.hidden) { reloadState(); if (S.job && S.job.state === 'running') pollJob(); }
  });
  updateWindowFocus();
  requestAnimationFrame(padLoop);
  const play = $('[data-fid="play"]'); if (play) play.focus({ preventScroll: true });
  if (st.job && st.job.state !== 'idle') { S.jobNext = 0; S.jobLines = []; pollJob(); }
  // First run: fetch the artwork once, quietly.
  if (!st.art.logo && !st.art.background && val('ui_art_auto') === '1' && !st.art.tried && !S.demo) {
    const r = await api('/api/art/fetch', {}).catch(() => null);
    if (r) { await reloadState(); renderBrand(); applyArtAndTheme(); }
  }
}

// ------------------------------------------------------------------ demo backend (development only)
const Mock = {
  st: null, job: null, t0: 0, kind: 'ready',
  async init(kind) {
    this.kind = kind || 'ready';
    const schema = await fetch('../settings-schema.json').then(r => r.json());
    schema.settings.forEach(d => { if (d.choices_ref) d.choices = schema.choice_sets[d.choices_ref]; if (d.choices_from === 'monitors') d.choices = [{ value: '', label: 'Auto' }, { value: 'DISPLAY1', label: 'Display 1: 2560 x 1440 (primary)' }, { value: 'DISPLAY2', label: 'Display 2: 3840 x 2160' }]; });
    const defaults = {}; schema.settings.forEach(d => { defaults[d.id] = d.default; });
    ['keyboard', 'usb_pads', 'virtual_pad', 'point_bounds', 'partial_readback'].forEach(k => { defaults[k] = '1'; });
    const ok = (id, label, detail) => ({ id, label, status: 'ok', detail });
    let checks = [ok('xbe', 'Game files', 'default.xbe found and verified.'), ok('archive', 'ATV archive', 'game\\ATV.zip is ready.'), ok('runtime', 'XboxRecomp runtime', 'Pinned runtime source is in place.'), ok('lift', 'Translated game code', 'Generated from your executable.'),
      { id: 'shaders', label: 'Fast-path shaders', status: 'warn', detail: 'Using generic fallbacks, so races run slower. Play through a 3D menu, a preview and a race, close the game, then launch again.' }, ok('build', 'Game build', 'Built 12 Nov 2026, 18:30.'), ok('tools', 'Build tools', 'CMake, Visual Studio and Python were found.')];
    let canPlay = true, firstRun = false;
    if (this.kind === 'firstrun') { checks = checks.map(c => ['lift', 'build'].includes(c.id) ? { ...c, status: 'todo', detail: c.id === 'build' ? 'Compiled on first launch. Expect several minutes.' : 'Generated from your executable on first launch. This takes a few minutes.' } : c); firstRun = true; }
    if (this.kind === 'broken') { checks[0] = { id: 'xbe', label: 'Game files', status: 'bad', detail: 'game\\default.xbe not found. Extract your own disc into the game folder.', action: 'open-game' }; checks[1] = { id: 'archive', label: 'ATV archive', status: 'bad', detail: 'ATV.BAD and ATV.BAH were not found under game\\.', action: 'open-game' }; canPlay = false; }
    this.st = { ok: true, app: { version: '1.0.0', platform: 'windows', titleId: '0x4143000A', xbeSha256: '5e9788296601bae4401e31ae1f5e25a8a732cb0bd0588576182d3aa69f859383', root: 'D:\\Games\\qpr2-recomp' }, schema, defaults, settings: { values: {}, ui: {} }, checks, canPlay, firstRun,
      shaders: { present: 1, total: 3, fixture: false, lastRun: 'fallback' }, stats: { sessions: 7, seconds: 15420, lastPlayedUtc: new Date(Date.now() - 86400000).toISOString() },
      sizes: { saves: 184320, cache: 52428800, logs: 8388608, backups: 921600 }, art: { logo: false, background: false, tried: true, logoVersion: 0, backgroundVersion: 0 },
      backups: [{ name: 'auto-20261112-184500.zip', bytes: 182400, kind: 'auto', modifiedUtc: '' }, { name: 'manual-20261110-210033.zip', bytes: 180100, kind: 'manual', modifiedUtc: '' }],
      runs: [{ id: 'clean-v1-20261112-184500-1-ab', status: 'exited', exitCode: 0, mode: 'Play', startedUtc: '2026-11-12T18:45:00Z', modifiedUtc: '2026-11-12T19:08:00Z', hasResultsZip: true }, { id: 'clean-v1-20261111-201500-2-cd', status: 'timed_out', exitCode: 124, mode: 'Benchmark', startedUtc: '2026-11-11T20:15:00Z', modifiedUtc: '2026-11-11T20:21:00Z', hasResultsZip: true }], job: { state: 'idle' } };
    if (this.kind === 'firstrun') { this.st.stats = { sessions: 0, seconds: 0 }; this.st.runs = []; this.st.backups = []; }
  },
  async handle(path, body) {
    await sleep(60); const st = this.st; const p = path.split('?')[0];
    if (p === '/api/state') return JSON.parse(JSON.stringify(st));
    if (p === '/api/ping') return { ok: true };
    if (p === '/api/setting') { const d = st.schema.settings.find(x => x.id === body.id); const bag = d.scope === 'ui' ? st.settings.ui : st.settings.values; if (String(body.value) === String(st.defaults[body.id])) delete bag[body.id]; else bag[body.id] = String(body.value); return { ok: true, settings: st.settings }; }
    if (p === '/api/reset') { st.settings = { values: {}, ui: {} }; return { ok: true, settings: st.settings }; }
    if (p === '/api/launch' || p === '/api/build') { this.t0 = Date.now(); this.job = { id: 'demo' + this.t0, kind: 'full', build: p === '/api/build', lines: [], sent: 0, stopped: false, fail: this.kind === 'fail' }; return { ok: true }; }
    if (p === '/api/stop') { if (this.job) this.job.stopped = true; return { ok: true }; }
    if (p === '/api/dismiss') { this.job = null; return { ok: true }; }
    if (p === '/api/job') {
      const j = this.job; if (!j) return { state: 'idle' }; const el = (Date.now() - this.t0) / 1000;
      const script = [[0, 'prepare', 'Checking your setup', '[PYTHON] python.exe: tools/qpr2_pipeline.py all'], [1.2, 'prepare', 'Translating game code', 'Lifting 41,022 functions'], [2.6, 'build', 'Configuring the build', '-- Configuring done'], [3.4, 'build', 'Building the game', 'MSBuild version 17.10 for .NET Framework'], [4.4, 'build', 'Building the game', '  qpr2_callbacks.c'], [5.4, 'run', 'Starting the game', 'Starting QPR2 clean v1.']];
      let ph = script[0]; script.forEach(s => { if (el >= s[0]) ph = s; }); const lines = script.filter(s => el >= s[0]).map(s => s[3]);
      const out = lines.slice(j.sent); j.sent = lines.length; const run = el >= 5.4 && !j.build;
      const end = j.build ? 5.0 : 9; const done = j.stopped || el > end;
      const base = { id: j.id, kind: 'full', mode: 'Play', buildOnly: j.build, phase: ph[1], phaseText: ph[2], elapsed: Math.round(el), sessionSeconds: run ? Math.round(el - 5.4) : 0, gameRunning: run && !done, lines: out, next: lines.length };
      if (!done) return { state: 'running', ...base };
      const outcome = j.stopped ? 'stopped' : (j.fail ? 'build_failed' : (j.build ? 'built' : 'ok'));
      return { state: 'finished', ...base, gameRunning: false, result: { outcome, exitCode: 0, message: j.fail ? 'CMake Error: could not find Visual Studio 17 2022.' : '', sessionSeconds: run ? Math.round(el - 5.4) : 0, runId: 'clean-v1-demo' } };
    }
    if (p === '/api/saves/backup') { st.backups.unshift({ name: 'manual-20261113-120000.zip', bytes: 184000, kind: 'manual', modifiedUtc: '' }); return { ok: true }; }
    return { ok: true, results: {}, removed: 2, settings: st.settings };
  }
};

boot();
