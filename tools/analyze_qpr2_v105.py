"""Keep material GPU samples, CPU windows and guest timings distinct."""
import argparse,json,re,hashlib
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('log',type=Path);p.add_argument('--output',type=Path,default=Path('v105-timing-analysis.json'));a=p.parse_args()
text=a.log.read_text(errors='replace');windows=[];w=None;game=[]
def fields(line):return {k:float(v) for k,v in re.findall(r'([A-Za-z][A-Za-z_0-9]*)=(-?\d+(?:\.\d+)?)',line)}
for number,line in enumerate(text.splitlines(),1):
 if line.startswith('[GPU-FIFO]'):
  if w:windows.append(w)
  w={'line':number,'materials':{},'geometry_cpu':{},'sync':{}}
 if line.startswith('[GAME-TOTAL]'):
  row=fields(line);row['function']=re.search(r'fn=([0-9A-Fa-f]+)',line)[1];row['line']=number;row.pop('fn',None);game.append(row)
 if not w:continue
 m=re.match(r'\[([^]]+)\]',line)
 if not m:continue
 tag=m[1]
 if tag=='GPU-MATERIAL-TIME':w['materials'][re.search(r'class=(\S+)',line)[1]]=fields(line)
 elif tag=='GPU-GEOMETRY-CPU':w['geometry_cpu']['/'.join(re.search(r'phase=(\S+) outcome=(\S+)',line).groups())]=fields(line)
 elif tag=='GPU-SYNC':w['sync'][re.search(r'reason=(\S+)',line)[1]]=fields(line)
 elif tag in ['GPU-FIFO','FRAME-PACING','RENDER-TIME','GPU-READBACK-TIME','GPU-RASTER','GPU-MATERIAL-PROFILE','GPU-SPREAD','GPU-PROFILE']:w[tag]=fields(line)
if w:windows.append(w)
materials={}
for w in windows:
 for label,row in w['materials'].items():
  r=materials.setdefault(label,{'draws':0,'triangles':0,'samples':0,'sampled_triangles':0,'gpu_ms':0,'range_weighted_ms':0,'range_weights':0,'max_ms':0})
  for k in r:
   if k=='max_ms':r[k]=max(r[k],row.get(k,0))
   else:r[k]+=row.get(k,0)
result={'source':a.log.name,'sha256':hashlib.sha256(a.log.read_bytes()).hexdigest(),'materials':materials,'windows':windows,'independent_guest_function_windows':game,'caveats':['Material classes are shader groups, not identified rider/ATV/track assets. No automatic scene labels.','One pseudorandom range per sampled batch. Raw gpu_ms is sampled time only; group selection depends on ranges per batch. range_weighted_ms weights by that count, before the outer batch sampling rate.','Completed query results may arrive in a later report than their draw populations. Missing/dropped/invalid samples are not zero cost.','Material intervals are inside the pixel interval. Do not add them, CPU inclusive timers, or readback waits to obtain total frame time.','Guest function windows are independent and inclusive. Their host timestamps support alignment, not exclusive CPU attribution.','No FPS gain or confirmed single root cause is inferred.']}
a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'windows':len(windows),'guest_windows':len(game),'material_samples':{k:v['samples'] for k,v in materials.items()}},indent=2))
