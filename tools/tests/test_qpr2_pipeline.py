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
                    "start": "0x000AB0F0",
                    "end": "0x000AB8AB",
                    "section": ".text",
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
                pipeline.apply_known_function_splits(path, digest), 3)
            functions = json.loads(path.read_text(encoding="utf-8"))
            self.assertEqual([item["start"] for item in functions],
                             ["0x00026180", "0x000261A0",
                              "0x000AB0F0", "0x000AB410",
                              "0x000BCA80", "0x000BCAA0"])
            self.assertEqual(functions[0]["end"], "0x000261A0")
            self.assertEqual(functions[1]["end"], "0x00026270")
            self.assertEqual(functions[1]["detection_method"],
                             "verified_build_split")
            self.assertEqual(functions[2]["end"], "0x000AB410")
            self.assertEqual(functions[3]["end"], "0x000AB8AB")
            self.assertTrue(functions[3]["external_entry"])
            self.assertEqual(functions[4]["end"], "0x000BCAA0")
            self.assertEqual(functions[5]["end"], "0x000BCB80")
            self.assertEqual(functions[5]["detection_method"],
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

    def test_v017_tail_aliases_are_promoted_to_verified_entries(self):
        digest = next(iter(pipeline.KNOWN_FUNCTION_RANGES))
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "functions.json"
            path.write_text(json.dumps([
                {"start": "0x000261A0", "end": "0x00026270", "section": ".text",
                 "detection_method": "tail_jump_alias", "confidence": 0.4},
                {"start": "0x00193110", "end": "0x00193150", "section": "D3D",
                 "detection_method": "tail_jump_alias", "size": 64,
                 "num_instructions": 25, "called_by": [0x00194000]},
                {"start": "0x00193150", "end": "0x001931A6", "section": "D3D",
                 "detection_method": "call_target"},
                {"start": "0x001A7450", "end": "0x001A74BC", "section": "XPP",
                 "detection_method": "tail_jump_alias"},
                {"start": "0x000AB410", "end": "0x000AB8AB", "section": ".text",
                 "detection_method": "tail_jump_alias"},
                {"start": "0x000AB8B0", "end": "0x000AB8F0", "section": ".text",
                 "detection_method": "tail_jump_alias"},
                {"start": "0x000BCAA0", "end": "0x000BCB80", "section": ".text",
                 "detection_method": "tail_jump_alias"},
            ]), encoding="utf-8")
            self.assertEqual(pipeline.apply_known_function_splits(path, digest), 3)
            self.assertEqual(pipeline.apply_known_function_ranges(path, digest), 22)
            by_start = {item["start"]: item
                        for item in json.loads(path.read_text(encoding="utf-8"))}
            table_entry = by_start["0x00193110"]
            self.assertEqual(table_entry["end"], "0x0019314E")
            self.assertEqual(table_entry["size"], 0x3E)
            self.assertEqual(table_entry["detection_method"], "verified_pointer_table")
            self.assertEqual(table_entry["called_by"], [0x00194000])
            self.assertNotIn("num_instructions", table_entry)
            self.assertEqual(by_start["0x00193150"]["end"], "0x001931A6")
            self.assertEqual(by_start["0x001A7450"]["detection_method"],
                             "verified_runtime_target")
            self.assertEqual(by_start["0x000261A0"]["detection_method"],
                             "verified_build_split")
            self.assertEqual(by_start["0x000AB8B0"]["end"], "0x000AB8EA")
            self.assertTrue(by_start["0x000AB8B0"]["external_entry"])
            self.assertEqual(pipeline.apply_known_function_splits(path, digest), 0)
            self.assertEqual(pipeline.apply_known_function_ranges(path, digest), 0)

    def test_pointer_table_range_rejects_truncated_tail_alias(self):
        digest = next(iter(pipeline.KNOWN_FUNCTION_RANGES))
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "functions.json"
            path.write_text(json.dumps([
                {"start": "0x00193110", "end": "0x00193120", "section": "D3D",
                 "detection_method": "tail_jump_alias"},
            ]), encoding="utf-8")
            with self.assertRaisesRegex(pipeline.PipelineError, "unexpected end"):
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
        digest = next(iter(pipeline.KNOWN_DIRECT_OVERRIDES))
        with tempfile.TemporaryDirectory() as directory:
            generated = Path(directory)
            addresses = pipeline.KNOWN_DIRECT_OVERRIDES[digest]
            manifest = generated / "manual-functions.json"
            pipeline.write_manual_entry_points(generated, manifest, digest)
            wrappers = generated / "qpr2_manual_entries.c"
            header = generated / "recomp_types.h"
            header.write_text(
                "void recomp_memory_fence(uint32_t va);\n", encoding="utf-8"
            )
            def write_unit(filename, address, instructions):
                path = generated / filename
                path.write_text('#include "recomp_funcs.h"\n'
                                f"void sub_{address:08X}(void)\n{{\n"
                                + instructions + "}\n", encoding="utf-8")
                return path

            bridge = write_unit("recomp_0033.c", 0x00196CF0,
                                "    RECOMP_PC(0x00196DF4u);\n")
            flush = "    MEM32(eax + 0x100410) = ecx;\n"
            submit_edx = "    MEM32(ecx + 0x40) = edx;\n"
            submit_eax = "    MEM32(edx + 0x40) = eax;\n"
            other_units = [
                write_unit("recomp_0034.c", 0x00196FB0, flush + submit_edx + submit_eax),
                write_unit("recomp_0035.c", 0x001970C0, flush + submit_edx),
                write_unit("recomp_0036.c", 0x001971F0, flush),
            ]
            unrelated = write_unit("recomp_0037.c", 0x001128E0, submit_edx)
            xid = write_unit("recomp_0038.c", 0x001A830C,
                             "    RECOMP_PC(0x001A831Au);\n"
                             "    eax = MEM32(esp + 0x10);\n"
                             "    RECOMP_PC(0x001A83B1u);\n"
                             "    RECOMP_PC(0x001A83C5u);\n"
                             "    _cf = 0; /* logical op clears CF */\n"
                             "    MEM8(esi + 4) = MEM8(esi + 4) | ((8) & 0xFFu);\n"
                             "    RECOMP_PC(0x001A83CBu);\n")
            connect = write_unit("recomp_0039.c", 0x001A9BA0,
                                 "    RECOMP_PC(0x001A9BE7u);\n"
                                 "    PUSH32(esp, ecx);\n")
            ui_first = write_unit("recomp_0019.c", 0x0010F440,
                                  "    RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0010F45Cu);\n")
            ui = write_unit("recomp_0020.c", 0x0010FEE0,
                            "    RECOMP_PC(0x0010FEE0u);\n" + "".join(
                                f"    RECOMP_ICALL_SAFE_AT{suffix}(_icall_target, _icall_esp, "
                                f"0x{site:08X}u);\n"
                                for site, suffix in ((0x0010FF29, "_CC"),
                                                     (0x0010FF55, ""), (0x0010FF61, ""))))

            self.assertEqual(
                pipeline.apply_known_generated_bridges(generated, "different"), 0)
            self.assertEqual(
                pipeline.apply_known_generated_bridges(generated, digest), 21)
            patched = bridge.read_text(encoding="utf-8")
            self.assertIn("requested_fence = MEM32(g_esp + 4)", patched)
            self.assertIn("recomp_nv2a_fence_wait(", patched)
            others = "".join(path.read_text(encoding="utf-8") for path in other_units)
            self.assertEqual(others.count("recomp_nv2a_cache_flush("), 3)
            self.assertEqual(others.count("recomp_nv2a_submit("), 3)
            self.assertNotIn("recomp_nv2a_submit(", unrelated.read_text())
            self.assertIn("QPR2_UI_ICALL(", ui_first.read_text())
            self.assertIn('#include "../../qpr2_ui_trace.h"', ui_first.read_text())
            self.assertEqual(ui.read_text().count("QPR2_UI_ICALL("), 2)
            self.assertIn("QPR2_UI_ICALL_CC(", ui.read_text())
            self.assertIn("qpr2_ui_trace(0, 0x0010FEE0u", ui.read_text())
            self.assertIn("void recomp_nv2a_submit(", header.read_text())
            self.assertIn("void qpr2_xid_trace_completion(", header.read_text())
            self.assertIn("qpr2_xid_trace_completion(eax);", xid.read_text())
            self.assertIn("qpr2_xid_trace_match(esi, 1);", xid.read_text())
            self.assertIn("qpr2_xid_trace_match(esi, 0);", xid.read_text())
            self.assertIn("((8) & 0xFFu);\n    qpr2_xid_publish_connection();",
                          xid.read_text())
            self.assertIn("if (MEM32(ebp + 4) == 0x001A83C5u) {",
                          connect.read_text())
            self.assertIn("qpr2_xid_defer_connection(eax, MEM32(esi + 0x14));",
                          connect.read_text())
            for address in addresses:
                self.assertIn(
                    f"void sub_{address:08X}(void) "
                    f"{{ qpr2_recovered_{address:08X}(); }}",
                    wrappers.read_text(encoding="utf-8"))
            self.assertEqual(
                pipeline.apply_known_generated_bridges(generated, digest), 0)
            self.assertEqual(bridge.read_text(encoding="utf-8"), patched)

    def test_manual_overrides_require_the_verified_executable(self):
        with tempfile.TemporaryDirectory() as directory:
            generated = Path(directory)
            manifest = generated / "manual-functions.json"
            pipeline.write_manual_entry_points(generated, manifest, "unknown")
            self.assertEqual(manifest.read_text(encoding="utf-8").strip(), "{}")
            wrappers = (generated / "qpr2_manual_entries.c").read_text(encoding="utf-8")
            self.assertNotIn("qpr2_recovered_", wrappers)


if __name__ == "__main__":
    unittest.main()

