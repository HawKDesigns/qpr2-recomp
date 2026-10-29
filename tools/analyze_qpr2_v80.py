from pathlib import Path
import json,re,hashlib,statistics
root=Path('runs/v80');src=root/'eighty-third-run.stderr.log';text=src.read_text(errors='replace')
def values(s):
 return {k:float(v) for k,v in re.findall(r'([A-Za-z][A-Za-z_0-9]*)=(-?\d+(?:\.\d+)?)',s)}
def summary(items):
 return {k:{'min':min(v),'median':statistics.median(v),'max':max(v)} for k in sorted(set().union(*(x.keys() for x in items))) if (v:=[x[k] for x in items if k in x])}
windows=[];current=None;audio=[];passes=[]
for line in text.splitlines():
 if line.startswith('[GPU-RASTER] draws='):
  if current:windows.append(current)
  current={'raster':values(line),'sync':{}}
 elif current and line.startswith('[GPU-READBACK-QUEUE]'):current['queue']=values(line)
 elif current and line.startswith('[FRAME-PACING]'):current['pacing']=values(line)
 elif current and line.startswith('[RENDER-TIME]'):current['cpu']=values(line)
 elif current and line.startswith('[GPU-SYNC]'):
  m=re.search(r'reason=(\S+)',line)
  if m:current['sync'][m[1]]=values(line)
 elif line.startswith('[AUDIO-WORK]'):audio.append(values(line))
 elif line.startswith('[GPU-PASS-TIME]'):passes.append({'phase':re.search('phase=(\\S+)',line)[1],**values(line)})
if current:windows.append(current)
selected=[w for w in windows if w.get('pacing',{}).get('intervals',0)>=20 and w['pacing']['avg_fps']<40 and w['raster'].get('triangles',0)+w.get('cpu',{}).get('jobs',0)>=1000000]
tail=windows[-4:]
result={'input_file':src.name,'sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'build_exitcode':int((root/'diagnostic-v80-build.exitcode.txt').read_text()),'run_exitcode':int((root/'eighty-third-run.exitcode.txt').read_text()),'run_exit_context':'WATCHDOG no exit after 360s diagnostic time limit; not evidence of a spontaneous crash','windows':len(windows),'selection':'GPU triangles + CPU jobs >= 1,000,000; >=20 frame intervals; avg FPS <40. Proxy only; no scene/AI visibility identification, no matched A/B. Reports paired with subsequent pacing/CPU rows.','selected_heavy_windows':len(selected),'selected_fps':summary([w['pacing'] for w in selected]),'selected_renderer':summary([w['raster'] for w in selected]),'selected_queue':summary([w.get('queue',{}) for w in selected]),'last_four_windows':tail,'last_40_audio_work':summary(audio[-40:]),'last_gpu_pass_samples':passes[-4:],'caveats':['CPU wall timers overlap and include preemption; do not add these categories.','GPU pass samples cover selected small batches, not all frame work.','V80/V79 camera, AI visibility and race timing are not matched; no controlled speedup inferred.','V81 performance remains unmeasured.'], 'v81_scope':'GPU-resident handoff for exact supported ARGB target textures; original guest completion and CPU publication semantics retained. Audio sources unchanged from V80.'}
Path('v81-v80-evidence.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({k:result[k] for k in ['sha256','windows','selected_heavy_windows','selected_fps','last_40_audio_work']},indent=2))
