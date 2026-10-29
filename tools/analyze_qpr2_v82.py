from pathlib import Path
import re,json,hashlib,statistics

def values(s):return {k:float(v) for k,v in re.findall(r'([A-Za-z][A-Za-z_0-9]*)=(-?\d+(?:\.\d+)?)',s)}
def stats(rows):
 keys=set().union(*(x.keys() for x in rows)) if rows else set()
 return {k:{'min':min(v),'median':statistics.median(v),'max':max(v),'total':sum(v)} for k in sorted(keys) if (v:=[x[k] for x in rows if k in x])}
def parse(folder,ordinal):
 p=Path('runs')/folder/(ordinal+'-run.stderr.log');windows=[];w=None;audio=[];last_bridge={};last_resume={};samples=[]
 for line in p.read_text(errors='replace').splitlines():
  if line.startswith('[GPU-TARGET-RESUME] enabled='):last_resume=values(line)
  elif line.startswith('[GPU-TEXTURE-BRIDGE] enabled='):last_bridge=values(line)
  elif line.startswith('[GPU-RASTER] draws='):
   if w:windows.append(w)
   w={'raster':values(line),'bridge':dict(last_bridge),'resume':dict(last_resume),'sync':{}}
  elif w and line.startswith('[GPU-READBACK-QUEUE]'):w['queue']=values(line)
  elif w and line.startswith('[FRAME-PACING]'):w['pacing']=values(line)
  elif w and line.startswith('[RENDER-TIME]'):w['cpu']=values(line)
  elif w and line.startswith('[GPU-SYNC]'):w['sync'][re.search(r'reason=(\S+)',line)[1]]=values(line)
  elif line.startswith('[AUDIO-WORK]'):audio.append(values(line))
  elif line.startswith('[GPU-PASS-TIME]'):samples.append({'phase':re.search(r'phase=(\S+)',line)[1],**values(line)})
 if w:windows.append(w)
 heavy=[w for w in windows if w.get('pacing',{}).get('intervals',0)>=20 and w['pacing']['avg_fps']<40 and w['raster']['triangles']+w.get('cpu',{}).get('jobs',0)>=1000000]
 return {'input_file':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'windows':len(windows),'selected_heavy_windows':len(heavy),'selection':'GPU triangles + CPU jobs >=1,000,000; pacing intervals >=20; average FPS <40. Reports paired with following pacing/CPU lines. Workload proxy only, not scene or AI visibility matching.','fps':stats([w['pacing'] for w in heavy]),'renderer':stats([w['raster'] for w in heavy]),'queue':stats([w.get('queue',{}) for w in heavy]),'resume':stats([w.get('resume',{}) for w in heavy]),'bridge':stats([w.get('bridge',{}) for w in heavy]),'all_bridge_totals':stats([w.get('bridge',{}) for w in windows]),'last_four_windows':windows[-4:],'audio_work_last_40':stats(audio[-40:]),'last_gpu_pass_samples':samples[-4:]}
x=parse('v82','eighty-fifth');x['comparison_v81']=parse('v81','eighty-fourth')
x['user_run_context']='User reports most of V82 was racing ahead of the AI. This is primarily a clear-view workload, not a crowded-race performance test. No exact visibility timestamps are available.'
x['build_exitcode']=int(Path('runs/v82/diagnostic-v82-build.exitcode.txt').read_text());x['run_exitcode']=int(Path('runs/v82/eighty-fifth-run.exitcode.txt').read_text())
x['run_exit_context']='Diagnostic WATCHDOG at 360 seconds; not a reported spontaneous crash.'
x['caveats']=['V82 was mostly ahead of AI; V81 is not matched by camera/race/AI visibility. No controlled speedup or crowded-race gain inferred.','CPU wall timers overlap and include preemption. Do not add categories.','GPU samples cover selected batches, not whole-frame GPU execution.','V83 retirement uptake and FPS remain unmeasured.']
x['v83_scope']='Retire a superseded target version only after its full GPU image resumes and all dirty publication obligations transfer to that later version. Retain visibility counters, coherent CPU reads and guest completions. No HLSL or audio changes.'
Path('v83-v82-evidence.json').write_text(json.dumps(x,indent=2)+'\n')
print(json.dumps({k:x[k] for k in ['selected_heavy_windows','fps','resume','queue']},indent=2))
