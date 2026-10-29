"""Sequential three-run before/after comparison of the actual worker source.

Usage: python3 tools/benchmark_qpr2_batches.py PREVIOUS_RENDERER OUTPUT_JSON
Uses the batch regression fixture and native API shims; not Windows game FPS.
"""
from pathlib import Path
import json
import statistics
import subprocess
import sys

def main():
    baseline = Path(sys.argv[1]).resolve()
    destination = Path(sys.argv[2]).resolve()
    root = Path(__file__).resolve().parents[1]
    fixture = root/'tools/test_qpr2_raster_batches.py'
    current = root/'third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c'
    runs = {}
    for version, path in [('v45', baseline), ('v46', current)]:
        runs[version] = []
        for repeat in range(3):
            result = subprocess.run([sys.executable, str(fixture), str(path)],
                                    capture_output=True, text=True)
            if result.returncode:
                print(result.stdout, end='')
                print(result.stderr, end='', file=sys.stderr)
                result.check_returncode()
            records = [json.loads(line) for line in result.stdout.splitlines()
                       if line.startswith('{')]
            assert len(records) == 2
            runs[version].append(records)
            print(version, repeat + 1, records, flush=True)
    summary = {}
    for index, size in enumerate([16, 32]):
        old = statistics.median(r[index]['terrain_ms'] for r in runs['v45'])
        new = statistics.median(r[index]['terrain_ms'] for r in runs['v46'])
        hashes = {r[index]['pixel_checksum'] for v in runs.values() for r in v}
        counts = {r[index]['pixels'] for v in runs.values() for r in v}
        assert len(hashes) == len(counts) == 1
        summary[size] = {'v45_ms': old, 'v46_ms': new,
                         'speedup': round(old/new, 3), 'checksum': next(iter(hashes))}
    result = {'scope': 'Actual Windows worker source via Linux pthread event/API '
              'shims, 8-thread pool, 30 triangles per draw, two bilinear DXT5 '
              'textures and two register-combiner stages; not Windows game FPS.',
              'runs': runs, 'median': summary}
    destination.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps(summary), flush=True)

if __name__ == '__main__':
    main()
