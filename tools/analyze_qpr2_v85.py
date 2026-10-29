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
x=parse('v85','eighty-eighth');x['comparison_v84']=parse('v84','eighty-seventh')
x['user_context']='User reports lower menu and race FPS in V85; intro remains fine.'
x['caveats']=['No timestamped matched scenes or AI visibility markers; do not infer isolated A/B gain from medians.','Wall timers overlap and include scheduling; sparse GPU samples are not whole frames.','Hardware correctness gates passing does not validate performance.']
p=Path('runs/v85/eighty-eighth-run.stderr.log');lines=p.read_text(errors='replace').splitlines()
x['startup']=[l for l in lines if ('shader' in l and 'setup_ms=' in l) or 'comparison passed' in l or 'hardware adapter=' in l]
x['layout']=[values(l) for l in lines if l.startswith('[GPU-LAYOUT]')]
for key,version,ordinal in [('v85', 'v85','eighty-eighth'),('v84','v84','eighty-seventh')]:
 ls=Path('runs',version,ordinal+'-run.stderr.log').read_text(errors='replace').splitlines()
 phases=[l for l in ls if 'shader' in l and 'setup_ms=' in l]
 x[key+'_shader_setup']={'phases':phases,'total_setup_ms':sum(float(re.search(r'setup_ms=([0-9.]+)',l)[1]) for l in phases),'includes':'Six logged raster shader setup phases only; not total startup time.'}
x['v86_scope']='Restore exact V84 renderer and seven expanded shader strings; preserve the working intro clock and CPU audio. Regression isolation, not a verified driver-level cause or new gain.'

Path('v86-v85-evidence.json').write_text(json.dumps(x,indent=2)+'\n')
for name,v in [('v85',x),('v84',x['comparison_v84'])]:print(name,'windows',v['selected_heavy_windows'],'fps',v['fps'].get('avg_fps'),'map',v['renderer'].get('map_ms'),'copy',v['renderer'].get('copy_ms'),'upload',v['upload'].get('bytes'))
print('layout',stats(x['layout']))
