#!/usr/bin/env python3
"""Prepare game/ATV.zip from the user's extracted ATV.BAD/ATV.BAH pair.

The ZIP contains every extracted file relative to game/ATV/ (no ATV/ prefix).
It is created only when missing or incomplete, with fixed ZIP metadata so
identical extracted files produce identical bytes on different installations.
No retail files are included in the source repository.
"""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import uuid
from zipfile import BadZipFile, ZipFile, ZipInfo, ZIP_STORED

from vendor.bahhhd_extract import FormatError, parse_bah


def pair_in(game: Path) -> tuple[Path, Path] | None:
    pairs = []
    if not game.is_dir():
        return None
    for bad in game.rglob("*"):
        if not bad.is_file() or bad.name.casefold() != "atv.bad":
            continue
        bahs = [p for p in bad.parent.iterdir()
                if p.is_file() and p.name.casefold() == "atv.bah"]
        if len(bahs) == 1:
            pairs.append((bad, bahs[0]))
    if len(pairs) > 1:
        raise ValueError("multiple ATV.BAD/ATV.BAH pairs under game/; keep one matching pair")
    return pairs[0] if pairs else None


def file_info(root: Path) -> dict[str, int]:
    if not root.is_dir():
        return {}
    files = {}
    for path in root.rglob("*"):
        if path.is_symlink():
            raise ValueError(f"refusing linked path inside extracted ATV folder: {path}")
        if path.is_file():
            name = path.relative_to(root).as_posix()
            files[name] = path.stat().st_size
    return files


def source_info(bah: Path) -> dict[str, int]:
    entries = parse_bah(bah.read_bytes())
    result = {}
    for entry in entries:
        name = entry.path.replace("\\", "/")
        if name in result:
            raise ValueError(f"duplicate archive path needs manual inspection: {name}")
        result[name] = entry.size
    return result


def has_vertex_shaders(files: dict[str, int]) -> bool:
    return any(name.startswith("VertexShader/Assembled/VS")
               and size > 4 for name, size in files.items())


def zip_info(archive: Path) -> dict[str, int]:
    if not archive.is_file():
        return {}
    try:
        with ZipFile(archive) as zf:
            # Compress-Archive commonly includes directory entries. They are
            # harmless; the inventory is over actual files only.
            items = [item for item in zf.infolist() if not item.is_dir()]
            if len(items) != len({item.filename for item in items}):
                return {}
            if any(item.filename.startswith("/") or
                   ".." in Path(item.filename).parts for item in items):
                return {}
            return {item.filename: item.file_size for item in items}
    except (OSError, ValueError, BadZipFile):
        return {}


def write_zip(root: Path, dest: Path, files: dict[str, int]) -> None:
    if not has_vertex_shaders(files):
        raise ValueError(f"{root} lacks VertexShader/Assembled/VS* files")
    with tempfile.NamedTemporaryFile(prefix=".ATV-", suffix=".zip.tmp",
                                     dir=dest.parent, delete=False) as tmp:
        temporary = Path(tmp.name)
    try:
        with ZipFile(temporary, "w", allowZip64=True) as zf:
            for name in sorted(files):
                meta = ZipInfo(name, (1980, 1, 1, 0, 0, 0))
                meta.compress_type = ZIP_STORED
                meta.create_system = 3
                meta.external_attr = 0o100644 << 16
                with (root / Path(name)).open("rb") as source, zf.open(meta, "w") as target:
                    shutil.copyfileobj(source, target, length=1024 * 1024)
        if zip_info(temporary) != files:
            raise ValueError("new ATV.zip inventory does not match extracted folder")
        os.replace(temporary, dest)
    finally:
        temporary.unlink(missing_ok=True)


def prepare(game: Path) -> Path:
    game = game.resolve()
    archive, folder = game / "ATV.zip", game / "ATV"
    pair = pair_in(game)
    expected = source_info(pair[1]) if pair else None
    existing = zip_info(archive)
    local = file_info(folder)
    if has_vertex_shaders(existing) and (
        (expected is not None and existing == expected) or
        (expected is None and (not local or existing == local))
    ):
        print(f"[ATV-PREP] Using complete {archive} ({len(existing)} files)")
        return archive

    if has_vertex_shaders(local) and (expected is None or local == expected):
        print(f"[ATV-PREP] Packing all {len(local)} files from {folder}")
        write_zip(folder, archive, local)
        return archive

    if pair is None:
        raise ValueError(
            "Need game/ATV.BAD and game/ATV.BAH from your extracted ISO, "
            "or a complete game/ATV/ folder containing VertexShader/Assembled/VS*"
        )

    bad, bah = pair
    extractor = Path(__file__).resolve().parent / "vendor" / "bahhhd_extract.py"
    print(f"[ATV-PREP] Extracting {bad} with BAHHHD-Extract", flush=True)
    with tempfile.TemporaryDirectory(prefix=".ATV-stage-", dir=game) as stage:
        extracted = Path(stage) / "ATV"
        subprocess.run([sys.executable, str(extractor), str(bad),
                        "--bah", str(bah), "-o", str(extracted), "--quiet"],
                       check=True)
        files = file_info(extracted)
        if files != expected or not has_vertex_shaders(files):
            raise ValueError("extracted ATV files differ from BAH inventory or lack vertex shaders")
        write_zip(extracted, archive, files)
        if folder.exists():
            backup = game / ("ATV.backup-" + uuid.uuid4().hex[:12])
            folder.rename(backup)
            print(f"[ATV-PREP] Kept earlier incomplete folder at {backup}")
        extracted.rename(folder)
    print(f"[ATV-PREP] Ready {archive} ({len(files)} files from {folder})")
    return archive


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--game-dir", type=Path, default=Path(__file__).resolve().parents[1] / "game")
    args = parser.parse_args()
    try:
        prepare(args.game_dir)
    except (OSError, ValueError, FormatError, subprocess.CalledProcessError) as exc:
        print(f"[ATV-PREP] {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
