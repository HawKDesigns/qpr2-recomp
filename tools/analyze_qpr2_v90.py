from pathlib import Path
import re,json,hashlib,statistics

def values(s):return {k:float(v) for k,v in re.findall(r'([A-Za-z][A-Za-z_0-9]*)=(-?\d+(?:\.\d+)?)',s)}
def stats(rows):
 keys=set().union(*(x.keys() for x in rows)) if rows else set()
 return {k:{'min':min(v),'median':statistics.median(v),'max':max(v)} for k in sorted(keys) if (v:=[x[k] for x in rows if k in x])}
def parse(version,ordinal):
 p=Path('runs')/version/(ordinal+'-run.stderr.log');windows=[];w=None;bridge={};resume={};retire={};raw={};defer={};mixed={};movies=[];offsets=[]
 for line in p.read_text(errors='replace').splitlines():
  if line.startswith('[GPU-MIXED-UPLOAD] coalesce='):mixed=values(line)
  elif line.startswith('[GPU-DEFER-READBACK] enabled='):defer=values(line)
  elif line.startswith('[GPU-RAW-UPLOAD] stream='):raw=values(line)
  elif line.startswith('[GPU-TARGET-RETIRE] enabled='):retire=values(line)
  elif line.startswith('[GPU-TARGET-RESUME] enabled='):resume=values(line)
  elif line.startswith('[GPU-TEXTURE-BRIDGE] enabled='):bridge=values(line)
  elif line.startswith('[GPU-RASTER] draws='):
   if w:windows.append(w)
   w={'raster':values(line),'raw':dict(raw),'mixed':dict(mixed),'defer':dict(defer),'bridge':dict(bridge),'resume':dict(resume),'retire':dict(retire),'sync':{}}
  elif w and line.startswith('[GPU-READBACK-TIME]'):w['readback_phases']=values(line)
  elif w and line.startswith('[GPU-UPLOAD]'):w['upload']=values(line)
  elif w and line.startswith('[GPU-READBACK-QUEUE]'):w['queue']=values(line)
  elif w and line.startswith('[FRAME-PACING]'):w['pacing']=values(line)
  elif w and line.startswith('[RENDER-TIME]'):w['cpu']=values(line)
  elif w and line.startswith('[GPU-SYNC]'):w['sync'][re.search(r'reason=(\S+)',line)[1]]=values(line)
  elif line.startswith('[XMV-TIMING]'):movies.append(values(line))
  elif 'Frame counter: device' in line:offsets.append(line.strip())
 if w:windows.append(w)
 heavy=[w for w in windows if w.get('pacing',{}).get('intervals',0)>=20 and w['pacing']['avg_fps']<40 and w['raster']['triangles']+w.get('cpu',{}).get('jobs',0)>=1000000]
 return {'input_file':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'windows':len(windows),'selected_heavy_windows':len(heavy),'selection':'GPU triangles + CPU jobs >=1,000,000; pacing intervals >=20; average FPS <40. Reports paired with following pacing/CPU lines. Workload proxy, not exact race phase/AI visibility markers.','deferred_transfers':stats([w.get('defer',{}) for w in heavy]),'total_avoided_transfers':sum(w.get('defer',{}).get('avoided',0) for w in heavy),'total_avoided_transfer_bytes':sum(w.get('defer',{}).get('avoided_bytes',0) for w in heavy),'mixed_uploads':stats([w.get('mixed',{}) for w in heavy]),'total_mixed_calls_avoided':sum(w.get('mixed',{}).get('avoided_calls',0) for w in heavy),'fps':stats([w['pacing'] for w in heavy]),'renderer':stats([w['raster'] for w in heavy]),'readback_phases':stats([w.get('readback_phases',{}) for w in heavy]),'retired_bytes_total':sum(w.get('retire',{}).get('surface_map_bytes_avoided',0) for w in heavy),'raw_inputs':stats([w['raw'] for w in heavy]),'upload':stats([w.get('upload',{}) for w in heavy]),'queue':stats([w.get('queue',{}) for w in heavy]),'resume':stats([w.get('resume',{}) for w in heavy]),'retire':stats([w.get('retire',{}) for w in heavy]),'retire_count_total':sum(w.get('retire',{}).get('retired',0) for w in heavy),'last_eight_windows':windows[-8:],'clock_registrations':offsets,'movie_timing':movies}



x=parse('v90','ninety-third')
x['user_context']='User reports roughly unchanged performance behind and in front of AI. Exact scene/visibility transition timestamps are absent.'
x['caveats']=['Heavy-window selection is a workload proxy, not a race/AI classifier or controlled A/B.','Map API wall time includes preceding GPU work, driver work and host scheduling; it does not isolate transfer execution.','Sparse legacy GPU timestamps sample the first eligible batch per eight presents, alternating render/copy; these are not full-frame totals and can favor recurring early passes.','Broad CPU wall timers overlap.','Saved build/run exit-code files contain 3, not zero; runtime watchdog is 360 seconds.']
x['v91_scope']='Replace first-eligible sampling with optional deterministic spread sampling across raw/mixed-render, CPU-prepared render and copy scopes; add exclusive CPU flush submission stages. Diagnostic scope, no rendering algorithm or FPS gain claimed.'
Path('v91-v90-evidence.json').write_text(json.dumps(x,indent=2)+'\n')
print(json.dumps({k:x[k] for k in ['selected_heavy_windows','fps','mixed_uploads','total_mixed_calls_avoided']},indent=2))
print('Map',x['readback_phases']['map_wait_ms'])
