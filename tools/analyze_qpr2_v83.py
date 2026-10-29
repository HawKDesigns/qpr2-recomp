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
  elif w and line.startswith('[GPU-READBACK-QUEUE]'):w['queue']=values(line)
  elif w and line.startswith('[FRAME-PACING]'):w['pacing']=values(line)
  elif w and line.startswith('[RENDER-TIME]'):w['cpu']=values(line)
  elif w and line.startswith('[GPU-SYNC]'):w['sync'][re.search(r'reason=(\S+)',line)[1]]=values(line)
  elif line.startswith('[XMV-TIMING]'):movies.append(values(line))
  elif 'Frame counter: device' in line:offsets.append(line.strip())
 if w:windows.append(w)
 heavy=[w for w in windows if w.get('pacing',{}).get('intervals',0)>=20 and w['pacing']['avg_fps']<40 and w['raster']['triangles']+w.get('cpu',{}).get('jobs',0)>=1000000]
 return {'input_file':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'windows':len(windows),'selected_heavy_windows':len(heavy),'selection':'GPU triangles + CPU jobs >=1,000,000; pacing intervals >=20; average FPS <40. Reports paired with following pacing/CPU lines. Workload proxy, not exact race phase/AI visibility markers.','fps':stats([w['pacing'] for w in heavy]),'renderer':stats([w['raster'] for w in heavy]),'queue':stats([w.get('queue',{}) for w in heavy]),'resume':stats([w.get('resume',{}) for w in heavy]),'retire':stats([w.get('retire',{}) for w in heavy]),'retire_count_total':sum(w.get('retire',{}).get('retired',0) for w in heavy),'last_eight_windows':windows[-8:],'clock_registrations':offsets,'movie_timing':movies}
x=parse('v83','eighty-sixth');x['comparison_v82']=parse('v82','eighty-fifth');x['last_good_v80_movie_clock']=parse('v80','eighty-third')['movie_timing']
x['build_exitcode']=int(Path('runs/v83/diagnostic-v83-build.exitcode.txt').read_text());x['run_exitcode']=int(Path('runs/v83/eighty-sixth-run.exitcode.txt').read_text());x['run_exit_context']='360-second diagnostic watchdog; not a reported spontaneous crash.'
x['user_run_context']='V83 contains time behind and ahead of AI, per user. V82 was mostly ahead of AI. Exact scene/visibility transition timestamps unavailable; no matched A/B speedup inferred.'
x['intro_cause']={'introduced_in':'V81','incorrect_registration_offset':'0x1981','retail_reader_offset':'0x1980','mechanism':'Broad numeric revision replacement changed suffix 80u in 0x1980u. Writing the DWORD one byte late advances the actual guest reader by 256 per tick.','v84_registration_offset':'0x1980','fixed_gameplay_fps_verified':False}
x['caveats']=['CPU wall timers overlap and include preemption; categories are not additive.','GPU pass samples cover selected batches, not whole frames.','Behind/ahead user note does not timestamp individual reports.','V84 Windows playback/FPS remains unverified.']
x['v84_scope']='Restore exact title movie clock registration, reject unaligned 32-bit registrations, test actual startup calls against retail clock reader. GPU algorithms and seven shader strings unchanged from V83.'
Path('v84-v83-evidence.json').write_text(json.dumps(x,indent=2)+'\n')
print(json.dumps({'fps':x['fps']['avg_fps'],'heavy':x['selected_heavy_windows'],'map_ms':x['renderer']['map_ms'],'copy_ms':x['renderer']['copy_ms'],'retire':x['retire'],'clock':x['clock_registrations']},indent=2))
