from pathlib import Path
import re,json,hashlib,statistics

def values(s):return {k:float(v) for k,v in re.findall(r'([A-Za-z][A-Za-z_0-9]*)=(-?\d+(?:\.\d+)?)',s)}
def stats(rows):
 keys=set().union(*(x.keys() for x in rows)) if rows else set()
 return {k:{'min':min(v),'median':statistics.median(v),'max':max(v)} for k in sorted(keys) if (v:=[x[k] for x in rows if k in x])}
def parse(version,ordinal):
 p=Path('runs')/version/(ordinal+'-run.stderr.log');windows=[];w=None;bridge={};resume={};retire={};raw={};movies=[];offsets=[]
 for line in p.read_text(errors='replace').splitlines():
  if line.startswith('[GPU-RAW-UPLOAD] stream='):raw=values(line)
  elif line.startswith('[GPU-TARGET-RETIRE] enabled='):retire=values(line)
  elif line.startswith('[GPU-TARGET-RESUME] enabled='):resume=values(line)
  elif line.startswith('[GPU-TEXTURE-BRIDGE] enabled='):bridge=values(line)
  elif line.startswith('[GPU-RASTER] draws='):
   if w:windows.append(w)
   w={'raster':values(line),'raw':dict(raw),'bridge':dict(bridge),'resume':dict(resume),'retire':dict(retire),'sync':{}}
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
 return {'input_file':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'windows':len(windows),'selected_heavy_windows':len(heavy),'selection':'GPU triangles + CPU jobs >=1,000,000; pacing intervals >=20; average FPS <40. Reports paired with following pacing/CPU lines. Workload proxy, not exact race phase/AI visibility markers.','fps':stats([w['pacing'] for w in heavy]),'renderer':stats([w['raster'] for w in heavy]),'readback_phases':stats([w.get('readback_phases',{}) for w in heavy]),'retired_bytes_total':sum(w.get('retire',{}).get('surface_map_bytes_avoided',0) for w in heavy),'raw_inputs':stats([w['raw'] for w in heavy]),'upload':stats([w.get('upload',{}) for w in heavy]),'queue':stats([w.get('queue',{}) for w in heavy]),'resume':stats([w.get('resume',{}) for w in heavy]),'retire':stats([w.get('retire',{}) for w in heavy]),'retire_count_total':sum(w.get('retire',{}).get('retired',0) for w in heavy),'last_eight_windows':windows[-8:],'clock_registrations':offsets,'movie_timing':movies}

x=parse('v88','ninety-first')
x['user_context']='User reports no noticeable FPS gain; returned behind AI toward the end. No exact AI visibility transition timestamps.'
x['caveats']=['Selection is a heavy-workload proxy, not an exact race or AI visibility classifier.','Wall timers overlap; do not sum renderer totals.','Map time includes driver work and scheduling as well as waiting for preceding GPU work. It does not isolate staging transfer execution.','Sparse GPU pass timings are not full-frame totals.','No matched A/B speedup or 30 FPS achievement is established.','Build and run exit code files both contain 3; the runtime has a 360-second watchdog. Exit status is not claimed as zero.']
x['v89_scope']='Queue surface transfer from existing immutable snapshot only when final CPU publication survives; batch all required pending transfers before first Map; preserve saved counters and real fences. Snapshot-unavailable, bridge-disabled, capture and compute paths retain original copies.'
x['transfer_savings_caveat']='Retired storage extents are an upper opportunity bound for this workload; V88 still enqueued those transfers. V89 logs actual skipped copy counts/bytes, not an inferred bandwidth or FPS gain.'
Path('v89-v88-evidence.json').write_text(__import__('json').dumps(x,indent=2)+'\n')
print(__import__('json').dumps({k:x[k] for k in ['selected_heavy_windows','fps','readback_phases','retired_bytes_total']},indent=2))
