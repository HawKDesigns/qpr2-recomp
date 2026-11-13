"""Runs the launcher music decoder tests (launcher/music/wav.test.js). Needs Node.js; skipped without it."""
import shutil
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


@unittest.skipUnless(shutil.which("node"), "node is required")
class LauncherWavDecoder(unittest.TestCase):
    def test_decoder(self):
        r = subprocess.run(["node", str(ROOT / "launcher/music/wav.test.js")], capture_output=True, text=True, timeout=180)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("tests passed", r.stdout)


if __name__ == "__main__":
    unittest.main()
