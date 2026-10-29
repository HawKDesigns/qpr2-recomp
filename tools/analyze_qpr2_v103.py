from pathlib import Path
import re,json,statistics,hashlib
def vals(s):return {k:float(v) for k,v in re.findall(r'([A-Za-z][A-Za-z_0-9]*)=(-?\d+(?:\.\d+)?)',s)}
def stats(rows):return {k:{'min':min(v),'median':statistics.median(v),'max':max(v)} for k in sorted(set().union(*(r.keys() for r in rows))) if (v:=[r[k] for r in rows if k in r])}
import argparse
parser=argparse.ArgumentParser(description='Summarize QPR2 V103 report windows without inferring AI visibility.')
parser.add_argument('logfile',type=Path);parser.add_argument('--output',type=Path,default=Path('v104-v103-evidence.json'))
parser.add_argument('--archive',type=Path)
args=parser.parse_args();p=args.logfile;rows=[];w=None
for line in p.read_text(errors='replace').splitlines():
 if line.startswith('[GPU-FIFO]'):
  if w:rows.append(w)
  w={'phases':{},'geometry_cpu':{}}
 if w:
  m=re.match(r'\[([^]]+)\]',line)
  if m and m[1] in ['GPU-GEOMETRY','GPU-MESH-CACHE','GPU-MESH-BALANCE','GPU-RASTER','GPU-READBACK-TIME','GPU-FLUSH-CPU','FRAME-PACING','RENDER-TIME','GPU-UPLOAD','GPU-STREAM-UNION']:w[m[1]]=vals(line)
  if line.startswith('[GPU-GEOMETRY-CPU]'):w['geometry_cpu'][re.search(r'phase=(\S+)',line)[1]+'/'+re.search(r'outcome=(\S+)',line)[1]]=vals(line)
  if line.startswith('[GPU-SPREAD-TIME]'):w['phases'][re.search(r'class=(\S+)',line)[1]+'/'+re.search(r'phase=(\S+)',line)[1]]=vals(line)
if w:rows.append(w)
heavy=[w for w in rows if w.get('FRAME-PACING',{}).get('intervals',0)>=20 and w['FRAME-PACING']['avg_fps']<40 and w.get('GPU-RASTER',{}).get('triangles',0)+w.get('RENDER-TIME',{}).get('jobs',0)>=1000000]
ph={}
for w in heavy:
 for name,v in w['phases'].items():
  t=ph.setdefault(name,dict(samples=0,gpu_ms=0,work=0))
  for k in t:t[k]+=v[k]
x={'file':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'report_windows':len(rows),'selected_heavy_windows':len(heavy),'selection':'intervals>=20, FPS<40, GPU triangles + CPU jobs >=1000000. Workload proxy; no AI visibility marker.','phases':ph,'stats':{n:stats([w.get(n,{}) for w in heavy]) for n in ['FRAME-PACING','GPU-MESH-CACHE','GPU-MESH-BALANCE','GPU-GEOMETRY','GPU-READBACK-TIME','GPU-FLUSH-CPU','RENDER-TIME','GPU-RASTER','GPU-UPLOAD','GPU-STREAM-UNION']},'windows':rows,'caveats':['No matched camera/AI-position baseline. Mixed FPS medians cannot establish gain.','GPU phase timings are sparse samples; CPU timers overlap and readback includes preceding work.','No matched race/AI visibility comparison is available; stable30 FPS is not established.']}
x['user_context']='V103 archive supplied without additional run description. AI visibility transitions are not marked; prior run description must not be assigned to this run.'
log=p.read_text(errors='replace')
x['store_admission']={'comparison_passed':log.count('[GPU-STORE] comparison passed ready_mask=31'),'enabled_reports':log.count('[GPU-STORE] enabled=1 ready_mask=31 rejected=0'),'disabled_reports':log.count('[GPU-STORE] enabled=0'),'caveat':'Admission confirms the feature ran; it does not measure real unchanged-fragment ratio or net speed.'}
union=[w.get('GPU-STREAM-UNION',{}) for w in heavy]
totals={k:sum(r.get(k,0) for r in union) for k in ['lookups','overlapped_lookups','logical_stream_bytes','hashed_stream_bytes','copy_legacy_bytes','copied_bytes','upload_legacy_bytes','uploaded_bytes']}
x['stream_union']={'totals':totals,'enabled_reports':sum(r.get('enabled')==1 for r in union),'rejected_reports':sum(r.get('rejected',0)!=0 for r in union),'savings_percent':{label:100*(1-totals[b]/totals[a]) if totals[a] else None for label,a,b in [('hash_stream_bytes','logical_stream_bytes','hashed_stream_bytes'),('copy_payload_bytes','copy_legacy_bytes','copied_bytes'),('upload_bytes','upload_legacy_bytes','uploaded_bytes')]},'caveat':'Within-run bytes for these same operations. Not a V101 counterfactual: cache hits/evictions may differ. Includes non-race windows if they match the workload proxy.'}
cpu={}
for window in heavy:
 for name,v in window['geometry_cpu'].items():
  assert 0<=v['samples']<=v['calls'] and v['sample_ms']>=0 and v['sample_peak_ms']>=0
  a=cpu.setdefault(name,{'calls':0,'samples':0,'sample_ms':0,'sample_peak_ms':0,'enabled_windows':0})
  for k in ['calls','samples','sample_ms']:a[k]+=v[k]
  a['sample_peak_ms']=max(a['sample_peak_ms'],v['sample_peak_ms']);a['enabled_windows']+=v['enabled']==1
for a in cpu.values():
 a['pooled_sample_mean_ms']=a['sample_ms']/a['samples'] if a['samples'] else None
 a['estimated_mean_ms_per_selected_window']=a['pooled_sample_mean_ms']*a['calls']/len(heavy) if a['samples'] and heavy else None
x['geometry_cpu']=cpu
x['geometry_cpu_caveats']=['Estimated per-window time = pooled sample mean times total calls / selected window count. This is not a median, full capture or paired benchmark.', 'Samples include timer overhead and can miss rare stalls. Mesh/residency include existing submissions/waits. Do not add to enclosing CPU/GPU timings.', 'Sampling isolates mesh lookup/build, not its hashing subphase. Source inspection and local key/lookup experiments motivate word hashing; real-game gains remain unverified.']
if args.archive:x['input_archive_sha256']=hashlib.sha256(args.archive.read_bytes()).hexdigest()
args.output.parent.mkdir(parents=True,exist_ok=True)
args.output.write_text(json.dumps(x,indent=2)+'\n')
print('selected',len(heavy),'of',len(rows));print('sampled phases',json.dumps(ph));
for n,keys in {'FRAME-PACING':['avg_fps','over_33ms'],'GPU-MESH-CACHE':['host_evictions','protected_evictions','slot_evictions'],'GPU-MESH-BALANCE':['protected_host_bytes','protected_gpu_bytes','age_demotions','byte_demotions'],'GPU-GEOMETRY':['mesh_uploads','mesh_upload_bytes','boundary_fallbacks','candidate_triangles'],'GPU-READBACK-TIME':['map_wait_ms'],'RENDER-TIME':['batch_ms','transform_ms','raster_ms']}.items():print(n,{k:x['stats'][n].get(k) for k in keys})

print('CPU geometry estimates',json.dumps(cpu,indent=2))
