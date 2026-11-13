import json
from pathlib import Path
import sys
from tempfile import TemporaryDirectory
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from qpr2_debug_metadata import export_memory_map, export_symbols, lookup


class DebugMetadataTests(unittest.TestCase):
    def setUp(self):
        directory = TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.root = Path(directory.name)
        self.analysis = self.root / "analysis"
        self.generated = self.root / "generated"
        self.source = self.root / "src"
        self.digest = "5e9788296601bae4401e31ae1f5e25a8a732cb0bd0588576182d3aa69f859383"
        self.game = {"base_address": "0x10000", "entry_point": "0xCA76D", "xbe_sha256": self.digest}
        self.write("analysis/qpr2_manifest.json", {"game": self.game})
        self.write("analysis/qpr2_analysis.json", {
            **self.game, "kernel_thunk_addr": "0x001ADDA0",
            "sections": [{"name": ".text", "virtual_addr": "0x11000", "virtual_size": 0x12C400,
                          "writable": False, "executable": True, "flags": "PRE, X"}],
        })

    def write(self, relative_path, data):
        path = self.root / relative_path
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(data if isinstance(data, str) else json.dumps(data), encoding="utf-8")
        return path

    def test_catalog_joins_real_dispatch_and_preserves_name_provenance(self):
        self.write("analysis/disasm/functions.json", [
            {"start": "0x7F9F0", "end": "0x7FB86", "called_by": ["0x81234"]},
            {"start": "0xAB8B0", "end": "0xAB8EA"}, {"start": "0xCA770", "end": "0xCA779"},
        ])
        self.write("analysis/func_id/identified_functions.json", [
            {"start": "0xCA770", "identified_name": "sdk_guess", "confidence": 0.67, "method": "pattern"},
            {"start": "0xAB8B0", "vtable_addr": "0x1AFF70", "vtable_index": 2},
        ])
        self.write("analysis/abi/abi_functions.json", [{"address": "0xAB8B0", "calling_convention": "thiscall"}])
        self.write("analysis/recomp/manual-functions.json", {"0007F9F0": "sub_0007F9F0"})
        self.write("src/qpr2_callbacks.def", "QPR2_CALLBACK(0007F9F0, 1)\nQPR2_CALLBACK(001A7D43, 0)\n")
        self.write("generated/recomp_dispatch.c", """
    { 0x0007F9F0u, (recomp_func_t)sub_0007F9F0 },
    { 0x000AB8B0u, (recomp_func_t)sub_000AB8B0 },
    { 0x000CA770u, (recomp_func_t)sdk_guess_000CA770 },
""")
        catalog = export_symbols(self.analysis, self.generated, self.source)
        path = self.write("guest-symbols.json", catalog)
        preview, = lookup(path, "preview path")
        self.assertEqual(preview["host_symbol"], "qpr2_recovered_0007F9F0")
        self.assertEqual(preview["direct_override_symbol"], "sub_0007F9F0")
        self.assertEqual(preview["name_kind"], "confirmed_role")
        self.assertTrue(preview["level_callback"])
        self.assertEqual(lookup(path, "0007F9F2"), [preview])
        race, = lookup(path, "0xAB8B0")
        self.assertEqual((race["vtable_slot"], race["abi"]["calling_convention"]), (2, "thiscall"))
        guess, = lookup(path, "sdk_guess")
        self.assertEqual((guess["name_kind"], guess["name_confidence"]), ("inferred", 0.67))
        self.assertIsNone(lookup(path, "001A7D43")[0]["generated_dispatch_symbol"])
        self.game["xbe_sha256"] = "a" * 64
        self.write("analysis/qpr2_manifest.json", {"game": self.game})
        alternate = export_symbols(self.analysis, self.generated, self.source)
        self.assertEqual(alternate["functions"][0]["name_kind"], "unknown")
        self.assertEqual(alternate["functions"][0]["implementation"], "generated_dispatch")

    def test_memory_map_uses_observed_ranges_and_guest_addresses(self):
        run = {"run_id": "test", "runtime_pin": "example", "xbe_sha256": self.digest}
        self.write("run/run.json", run)
        self.write("run/run.stderr.log", """
xbox_MemoryLayoutInit: mapped 65536 KB at 0x0000000000010000
  Contiguous window: 64 MB at Xbox VA 0x80000000
  Tiled aperture alias verified (tiled 0xF0000000 == contiguous 0x80000000)
  Tiled aperture: 64 MB at Xbox VA 0xF0000000 (aliases the contiguous window)
  APU: 0xFE800000..0xFE830000 trapped for MMIO (GP/EP DSP memory left as RAM)
  MCPX device aperture: 8 MB at Xbox VA 0xFE800000
  Stack: 8192 KB at Xbox VA 0x00780000 (ESP = 0x00F7FFF0)
xbox_MemoryLayoutInit: complete
[PB-FIFO] registered device=001A50E8 bounds=+24/+28
""")
        data = export_memory_map(self.analysis, self.root / "run")
        path = self.write("memory-map.json", data)
        tiled, = lookup(path, "0xF03D8000")
        self.assertEqual(tiled["end_exclusive"], "0xF4000000")
        self.assertTrue(tiled["alias_verified"])
        self.assertEqual(tiled["alias_guest_va"], "0x80000000")
        self.assertEqual(tiled["canonical_guest_va"], "0x803D8000")
        self.assertEqual(lookup(path, "0xF4000000"), [])
        self.assertEqual([r["name"] for r in lookup(path, "0xFE800080")],
                         ["APU MMIO trap", "MCPX device aperture"])
        self.assertEqual(data["landmarks"]["D3D device pointer slot"], "0x001A50E8")
        self.assertEqual(data["d3d_device_field_offsets"]["FIFO limit"], "0x00000028")
        self.write("run/run.json", {**run, "xbe_sha256": "f" * 64})
        with self.assertRaisesRegex(ValueError, "different SHA-256"):
            export_memory_map(self.analysis, self.root / "run")


if __name__ == "__main__":
    unittest.main()
