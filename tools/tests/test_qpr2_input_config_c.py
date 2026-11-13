"""Compile and run tools/test_qpr2_input_config.c (pure C; needs gcc and libm)."""
import re
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
RUNTIME = ROOT / "third_party/xboxrecomp"


@unittest.skipUnless(shutil.which("gcc"), "gcc is required")
class InputConfigNative(unittest.TestCase):
    def test_native_rules(self):
        with tempfile.TemporaryDirectory(prefix="qpr2-input-") as d:
            exe = Path(d) / "input_config"
            subprocess.run(["gcc", "-std=c11", "-Wall", "-Wextra", "-Werror",
                            str(ROOT / "tools/test_qpr2_input_config.c"), "-lm", "-o", str(exe)], check=True)
            out = subprocess.run([str(exe)], check=True, capture_output=True, text=True, timeout=30)
            self.assertIn("ok", out.stdout)

    def test_retail_callback(self):
        with tempfile.TemporaryDirectory(prefix="qpr2-input-hook-") as directory:
            temp = Path(directory)
            headers = temp / "recomp/gen"
            headers.mkdir(parents=True)
            shutil.copyfile(RUNTIME / "templates/runtime/recomp_types.h", headers / "recomp_types.h")
            # Unrelated callbacks are discarded by the linker. Declare their
            # retail calls without requiring a locally generated dispatch header.
            source = ROOT / "src/qpr2_callbacks.c"
            calls = sorted(set(re.findall(r"\bsub_[0-9A-F]{8}\b", source.read_text())))
            (headers / "recomp_funcs.h").write_text("\n".join(f"void {name}(void);" for name in calls))
            exe = temp / "input_hook"
            subprocess.run([
                "gcc", "-std=gnu11", "-O1", "-Werror=implicit-function-declaration",
                "-ffunction-sections", "-fdata-sections", "-I", str(temp), "-I", str(ROOT / "src"),
                "-I", str(RUNTIME / "src"), "-I", str(RUNTIME / "src/kernel"),
                str(ROOT / "tools/test_qpr2_input_callback.c"), str(source),
                str(ROOT / "src/qpr2_input_config.c"), str(RUNTIME / "src/usb/usb_gamepad.c"),
                "-Wl,--gc-sections", "-lm", "-o", str(exe),
            ], check=True)
            out = subprocess.run([str(exe)], check=True, capture_output=True, text=True, timeout=30)
            self.assertIn("PASS: default report publication", out.stdout)


if __name__ == "__main__":
    unittest.main()
