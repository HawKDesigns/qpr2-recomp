import importlib.util
import json
import tempfile
import unittest
from pathlib import Path


MODULE_PATH = Path(__file__).resolve().parents[1] / "qpr2_pipeline.py"
SPEC = importlib.util.spec_from_file_location("qpr2_pipeline", MODULE_PATH)
pipeline = importlib.util.module_from_spec(SPEC)
assert SPEC and SPEC.loader
SPEC.loader.exec_module(pipeline)


class PipelineTests(unittest.TestCase):
    def setUp(self):
        self.analysis = {
            "title": "ATV Quad Power Racing 2",
            "title_id": "0x4143000A",
            "entry_point": "0x00123456",
            "base_address": "0x00010000",
            "sections": [{"name": ".text"}],
            "kernel_imports": [],
        }

    def test_known_title_is_accepted(self):
        self.assertEqual(pipeline.validate_title(self.analysis), 0x4143000A)

    def test_unknown_title_is_rejected(self):
        bad = dict(self.analysis, title_id="0xDEADBEEF")
        with self.assertRaises(pipeline.PipelineError):
            pipeline.validate_title(bad)

    def test_unknown_title_can_be_explicitly_allowed(self):
        bad = dict(self.analysis, title_id="0xDEADBEEF")
        self.assertEqual(pipeline.validate_title(bad, True), 0xDEADBEEF)

    def test_header_contains_derived_values(self):
        header = pipeline.render_config_header(self.analysis, "a" * 64)
        self.assertIn("0x4143000Au", header)
        self.assertIn("0x00123456u", header)
        self.assertIn('QPR2_XBE_PATH         "game/default.xbe"', header)
        self.assertIn("a" * 64, header)

    def test_load_analysis_requires_core_fields(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "analysis.json"
            path.write_text(json.dumps({"title": "incomplete"}), encoding="utf-8")
            with self.assertRaises(pipeline.PipelineError):
                pipeline.load_analysis(path)

    def test_verified_function_splits_create_real_dispatch_starts(self):
        digest = next(iter(pipeline.KNOWN_FUNCTION_SPLITS))
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "functions.json"
            path.write_text(json.dumps([
                {
                    "start": "0x00026180",
                    "end": "0x00026270",
                    "size": 0xF0,
                    "name": "sub_00026180",
                    "section": ".text",
                    "confidence": 0.88,
                    "detection_method": "tail_jump_alias",
                    "num_instructions": 91,
                    "has_prologue": False,
                    "calls_to": ["0x000E07A0"],
                    "called_by": [],
                },
                {
                    "start": "0x000BCA80",
                    "end": "0x000BCB80",
                    "size": 0x100,
                    "name": "sub_000BCA80",
                    "section": ".text",
                    "confidence": 0.88,
                    "detection_method": "tail_jump_alias",
                    "num_instructions": 65,
                    "has_prologue": False,
                    "called_by": [],
                },
            ]), encoding="utf-8")

            self.assertEqual(
                pipeline.apply_known_function_splits(path, digest), 2)
            functions = json.loads(path.read_text(encoding="utf-8"))
            self.assertEqual([item["start"] for item in functions],
                             ["0x00026180", "0x000261A0",
                              "0x000BCA80", "0x000BCAA0"])
            self.assertEqual(functions[0]["end"], "0x000261A0")
            self.assertEqual(functions[1]["end"], "0x00026270")
            self.assertEqual(functions[1]["detection_method"],
                             "verified_build_split")
            self.assertEqual(functions[2]["end"], "0x000BCAA0")
            self.assertEqual(functions[3]["end"], "0x000BCB80")
            self.assertEqual(functions[3]["detection_method"],
                             "verified_build_split")

    def test_function_split_is_not_applied_to_another_build(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "functions.json"
            original = [{
                "start": "0x00026180", "end": "0x00026270", "size": 0xF0,
            }]
            path.write_text(json.dumps(original), encoding="utf-8")
            self.assertEqual(
                pipeline.apply_known_function_splits(path, "different"), 0)
            self.assertEqual(json.loads(path.read_text(encoding="utf-8")), original)

    def test_verified_pointer_table_ranges_fill_discovery_gaps(self):
        digest = next(iter(pipeline.KNOWN_FUNCTION_RANGES))
        expected = pipeline.KNOWN_FUNCTION_RANGES[digest]
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "functions.json"
            path.write_text(json.dumps([{
                "start": "0x001930D0",
                "end": "0x0019310B",
                "size": 0x3B,
                "name": "sub_001930D0",
                "section": "D3D",
            }, {
                "start": "0x00193150",
                "end": "0x001931A6",
                "size": 0x56,
                "name": "sub_00193150",
                "section": "D3D",
            }]), encoding="utf-8")

            self.assertEqual(
                pipeline.apply_known_function_ranges(path, digest),
                len(expected),
            )
            functions = json.loads(path.read_text(encoding="utf-8"))
            by_start = {pipeline.parse_int(item["start"]): item
                        for item in functions}
            for entry in expected:
                start, end = entry[:2]
                self.assertEqual(pipeline.parse_int(by_start[start]["end"]), end)
                method = (entry[3] if len(entry) == 4
                          else "verified_pointer_table")
                section = entry[2] if len(entry) == 4 else "D3D"
                self.assertEqual(by_start[start]["detection_method"], method)
                self.assertEqual(by_start[start]["section"], section)

            usb = by_start[0x001A7450]
            self.assertEqual(usb["end"], "0x001A74BC")
            self.assertEqual(usb["section"], "XPP")
            self.assertEqual(usb["detection_method"],
                             "verified_runtime_target")

            configured = by_start[0x001A864E]
            self.assertEqual(configured["end"], "0x001A87A2")
            self.assertEqual(configured["section"], "XPP")
            self.assertEqual(configured["detection_method"],
                             "verified_runtime_target")

    def test_pointer_table_range_rejects_overlap(self):
        digest = next(iter(pipeline.KNOWN_FUNCTION_RANGES))
        start, end = pipeline.KNOWN_FUNCTION_RANGES[digest][0][:2]
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "functions.json"
            path.write_text(json.dumps([{
                "start": f"0x{start - 4:08X}",
                "end": f"0x{end:08X}",
                "size": end - start + 4,
            }]), encoding="utf-8")
            with self.assertRaises(pipeline.PipelineError):
                pipeline.apply_known_function_ranges(path, digest)

    def test_runtime_patches_are_hash_locked_and_idempotent(self):
        digest = next(iter(pipeline.KNOWN_RUNTIME_PATCHES))
        with tempfile.TemporaryDirectory() as directory:
            generated = Path(directory)
            path = generated / "recomp_0030.c"
            path.write_text(
                "    MEM32(ebx) = eax;\n\n"
                "loc_0014124A: ;\n"
                "    RECOMP_PC(0x0014124Au);\n",
                encoding="utf-8",
            )
            xpp_path = generated / "recomp_0035.c"
            xpp_path.write_text(
                '    RECOMP_UNIMPL("cli", 0x001ABB3Du); /* TODO: cli */\n'
                "    eax = MEM32(ecx + 8);\n"
                '    RECOMP_UNIMPL("sti", 0x001ABB4Eu); /* TODO: sti */\n',
                encoding="utf-8",
            )

            self.assertEqual(
                pipeline.apply_known_runtime_patches(generated, digest), 3)
            patched = path.read_text(encoding="utf-8")
            self.assertIn("MEM32(ebx) = 0;", patched)
            patched_xpp = xpp_path.read_text(encoding="utf-8")
            self.assertNotIn("RECOMP_UNIMPL", patched_xpp)
            self.assertIn("QPR2 XPP interrupt guard", patched_xpp)
            self.assertEqual(
                pipeline.apply_known_runtime_patches(generated, digest), 0)
            self.assertEqual(path.read_text(encoding="utf-8"), patched)
            self.assertEqual(xpp_path.read_text(encoding="utf-8"), patched_xpp)

            other = generated / "other"
            other.mkdir()
            other_path = other / "recomp_0030.c"
            other_path.write_text(
                "    MEM32(ebx) = eax;\n\n"
                "loc_0014124A: ;\n"
                "    RECOMP_PC(0x0014124Au);\n",
                encoding="utf-8",
            )
            (other / "recomp_0035.c").write_text(
                '    RECOMP_UNIMPL("cli", 0x001ABB3Du); /* TODO: cli */\n'
                '    RECOMP_UNIMPL("sti", 0x001ABB4Eu); /* TODO: sti */\n',
                encoding="utf-8",
            )
            self.assertEqual(
                pipeline.apply_known_runtime_patches(other, "different"), 0)
            self.assertNotIn(
                "MEM32(ebx) = 0;",
                other_path.read_text(encoding="utf-8"),
            )

    def test_recovered_calls_and_nv2a_bridges_survive_clean_generation(self):
        digest = next(iter(pipeline.KNOWN_RECOVERED_STUBS))
        with tempfile.TemporaryDirectory() as directory:
            generated = Path(directory)
            addresses = pipeline.KNOWN_RECOVERED_STUBS[digest]
            stubs = generated / "recomp_stubs_unresolved.c"
            stubs.write_text("".join(
                f"void sub_{address:08X}(void) {{ g_esp += 4; "
                f"/* 0x{address:08X}: not detected */ }}\n"
                for address in addresses
            ), encoding="utf-8")
            header = generated / "recomp_types.h"
            header.write_text(
                "void recomp_memory_fence(uint32_t va);\n", encoding="utf-8"
            )
            bridge = generated / "recomp_0033.c"
            bridge.write_text(
                "void sub_00196CF0(void)\n{\n"
                "    RECOMP_PC(0x00196DF4u);\n"
                + "    MEM32(eax + 0x100410) = ecx;\n" * 3
                + "    MEM32(ecx + 0x40) = edx;\n" * 2
                + "    MEM32(edx + 0x40) = eax;\n"
                + "}\n",
                encoding="utf-8",
            )

            ui = generated / "recomp_0020.c"
            ui.write_text(
                '#include "recomp_funcs.h"\n'
                "void sub_0010FEE0(void) {\n"
                "    RECOMP_PC(0x0010FEE0u);\n"
                "}\n"
                + "".join(
                    f"RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, "
                    f"0x{site:08X}u);\n"
                    for site in (0x0010F45C, 0x0010FF29,
                                 0x0010FF55, 0x0010FF61)
                ),
                encoding="utf-8",
            )

            self.assertEqual(
                pipeline.apply_known_generated_bridges(generated, "different"), 0)
            self.assertEqual(
                pipeline.apply_known_generated_bridges(generated, digest), 25)
            patched = bridge.read_text(encoding="utf-8")
            self.assertIn("requested_fence = MEM32(g_esp + 4)", patched)
            self.assertEqual(patched.count("recomp_nv2a_cache_flush("), 3)
            self.assertEqual(patched.count("recomp_nv2a_submit("), 3)
            self.assertIn("recomp_nv2a_fence_wait(", patched)
            self.assertEqual(ui.read_text().count("QPR2_UI_ICALL("), 4)
            self.assertIn("qpr2_ui_trace(0, 0x0010FEE0u", ui.read_text())
            self.assertIn("void recomp_nv2a_submit(", header.read_text())
            for address in addresses:
                self.assertIn(
                    f"void sub_{address:08X}(void) "
                    f"{{ qpr2_recovered_{address:08X}(); }}",
                    stubs.read_text(encoding="utf-8"))
            self.assertEqual(
                pipeline.apply_known_generated_bridges(generated, digest), 0)
            self.assertEqual(bridge.read_text(encoding="utf-8"), patched)

    def test_recovered_stub_drift_fails_loudly(self):
        digest = next(iter(pipeline.KNOWN_RECOVERED_STUBS))
        with tempfile.TemporaryDirectory() as directory:
            generated = Path(directory)
            (generated / "recomp_stubs_unresolved.c").write_text(
                "void sub_00027E00(void) { g_esp += 8; }\n",
                encoding="utf-8",
            )
            with self.assertRaisesRegex(pipeline.PipelineError, "empty stub"):
                pipeline.apply_known_generated_bridges(generated, digest)



if __name__ == "__main__":
    unittest.main()

