from pathlib import Path
import re,json,hashlib,statistics

def values(s):return {k:float(v) for k,v in re.findall(r'([A-Za-z][A-Za-z_0-9]*)=(-?\d+(?:\.\d+)?)',s)}
def stats(rows):
 keys=set().union(*(x.keys() for x in rows)) if rows else set()
 return {k:{'min':min(v),'median':statistics.median(v),'max':max(v)} for k in sorted(keys) if (v:=[x[k] for x in rows if k in x])}
def parse(version,ordinal):
 p=Path('runs')/version/(ordinal+'-run.stderr.log');windows=[];w=None;bridge={};resume={};retire={};movies=[];offsets=[]
 for line in p.read_text(errors='replace').splitlines():
  if line.startswith('[GPU-TARGET-RETIRE] enabled='):retire=values(line)
  elif line.startswith('[GPU-TARGET-RESUME] enabled='):resume=values(line)
  elif line.startswith('[GPU-TEXTURE-BRIDGE] enabled='):bridge=values(line)
  elif line.startswith('[GPU-RASTER] draws='):
   if w:windows.append(w)
   w={'raster':values(line),'bridge':dict(bridge),'resume':dict(resume),'retire':dict(retire),'sync':{}}
  elif w and line.startswith('[GPU-UPLOAD]'):w['upload']=values(line)
  elif w and line.startswith('[GPU-READBACK-QUEUE]'):w['queue']=values(line)
  elif w and line.startswith('[FRAME-PACING]'):w['pacing']=values(line)
  elif w and line.startswith('[RENDER-TIME]'):w['cpu']=values(line)
  elif w and line.startswith('[GPU-SYNC]'):w['sync'][re.search(r'reason=(\S+)',line)[1]]=values(line)
  elif line.startswith('[XMV-TIMING]'):movies.append(values(line))
  elif 'Frame counter: device' in line:offsets.append(line.strip())
 if w:windows.append(w)
 heavy=[w for w in windows if w.get('pacing',{}).get('intervals',0)>=20 and w['pacing']['avg_fps']<40 and w['raster']['triangles']+w.get('cpu',{}).get('jobs',0)>=1000000]
 return {'input_file':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'windows':len(windows),'selected_heavy_windows':len(heavy),'selection':'GPU triangles + CPU jobs >=1,000,000; pacing intervals >=20; average FPS <40. Reports paired with following pacing/CPU lines. Workload proxy, not exact race phase/AI visibility markers.','fps':stats([w['pacing'] for w in heavy]),'renderer':stats([w['raster'] for w in heavy]),'upload':stats([w.get('upload',{}) for w in heavy]),'queue':stats([w.get('queue',{}) for w in heavy]),'resume':stats([w.get('resume',{}) for w in heavy]),'retire':stats([w.get('retire',{}) for w in heavy]),'retire_count_total':sum(w.get('retire',{}).get('retired',0) for w in heavy),'last_eight_windows':windows[-8:],'clock_registrations':offsets,'movie_timing':movies}
x=parse('v86','eighty-ninth')
x['user_context']='User confirms menu and race performance have returned to the previous baseline after V85 rollback; intro remains fixed.'
x['caveats']=['Workload filtering is not a timestamped race or AI-visibility classifier.','Wall timers overlap and include scheduling; GPU pass samples cover selected batches, not whole frames.','No matched run speedup or sustained crowded-race 30 FPS achievement is inferred.']
x['shader_setup']=[l for l in Path('runs/v86/eighty-ninth-run.stderr.log').read_text(errors='replace').splitlines() if 'shader' in l and 'setup_ms=' in l]
Path('v87-v86-evidence.json').write_text(json.dumps(x,indent=2)+'\n')
print('windows',x['selected_heavy_windows'],'fps',x['fps']['avg_fps'],'Map',x['renderer']['map_ms'],'submit',x['renderer']['submit_ms'])
