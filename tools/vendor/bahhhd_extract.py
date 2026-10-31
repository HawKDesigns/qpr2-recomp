#!/usr/bin/env python3
"""
bad_extract.py - extract files from a "Bark 1.06" .BAD / .BAH archive pair.

    .BAH  header / table of contents (directory tree, offsets, sizes)
    .BAD  data blob (concatenated file entries)

Usage:
    python bad_extract.py ATV.bad                 # reads ATV.bah, extracts to ./ATV/
    python bad_extract.py ATV.bad -o out_dir
    python bad_extract.py ATV.bad --bah other.bah
    python bad_extract.py ATV.bad --list          # list contents only
    python bad_extract.py ATV.bad -f "*.xml" -f "Levels/Dunes/*"   # glob filters

No third-party dependencies (Python 3.7+).


FORMAT NOTES (reverse engineered)
=================================
All integers are little-endian u32.

BAH
---
  0x00  "Bark 1.06\\0"            10-byte magic
  0x0A  2 bytes                   unknown ("rs" in this archive); skipped
  0x0C  <root directory record>   immediately followed by its children

  Directory record:
      u32 name_len                length of the padded name field below
      u32 num_files               files directly inside this directory
      u32 num_subdirs             subdirectories directly inside this directory
      u32 first_offset            offset of the dir's first file (informational)
      char name[name_len]         NUL padded to a multiple of 4
      -- then num_files file records, then num_subdirs directory records
         (each subdirectory record is followed, recursively, by its own children)

  File record:
      u32 name_len
      u32 offset                  position of this file's blob inside the BAD
      u32 stored_size             bytes the blob occupies in the BAD (4-byte aligned)
      u32 size                    size of the file once decompressed
      u32 checksum                content hash (algorithm not identified; unchecked)
      char name[name_len]         NUL padded

  The root directory has an unused name. File blobs tile the BAD exactly.

BAD blob (at `offset`, `stored_size` bytes long, padded to 4 bytes)
-------------------------------------------------------------------
      u32 method                  0 = stored, 1 = LZSS compressed
      u32 size                    uncompressed size (matches the BAH)
      ... payload ...

LZSS payload (Okumura-style)
----------------------------
  * 4096-byte ring buffer, zero-initialised, write pointer starts at 0xFEE.
  * Stream = repeated [flag byte][up to 8 items]; flag bits are read LSB first.
      bit = 1 : literal, 1 byte copied to output
      bit = 0 : match, 2 bytes  b0 b1
                  position = b0 | ((b1 & 0xF0) << 4)   (absolute ring position)
                  length   = (b1 & 0x0F) + 3
  * Decoding stops as soon as `size` bytes have been produced.
"""

import argparse
import fnmatch
import mmap
import os
import struct
import sys

MAGIC = b"Bark 1.06\0"
ROOT_RECORD_OFFSET = 0x0C
RING = 4096
RING_START = 0xFEE


class FormatError(Exception):
    pass


# --------------------------------------------------------------------------- #
# BAH parsing
# --------------------------------------------------------------------------- #
class Entry:
    __slots__ = ("path", "offset", "stored_size", "size", "checksum")

    def __init__(self, path, offset, stored_size, size, checksum):
        self.path = path
        self.offset = offset
        self.stored_size = stored_size
        self.size = size
        self.checksum = checksum


def _read_name(buf, pos, length):
    if length > 1024 or pos + length > len(buf):
        raise FormatError("bad name length %d at 0x%X" % (length, pos - 20))
    raw = bytes(buf[pos:pos + length])
    return raw.split(b"\0", 1)[0].decode("latin-1"), pos + length


def parse_bah(buf):
    """Return a list of Entry objects in archive order."""
    if bytes(buf[:len(MAGIC)]) != MAGIC:
        raise FormatError("not a 'Bark 1.06' BAH file (bad magic %r)" % bytes(buf[:10]))

    entries = []

    def parse_dir(pos, parent, is_root):
        if pos + 16 > len(buf):
            raise FormatError("truncated directory record at 0x%X" % pos)
        name_len, nfiles, nsub, _first = struct.unpack_from("<4I", buf, pos)
        pos += 16
        name, pos = _read_name(buf, pos, name_len)
        path = parent if is_root else parent + [name]

        for _ in range(nfiles):
            if pos + 20 > len(buf):
                raise FormatError("truncated file record at 0x%X" % pos)
            nl, off, stored, size, crc = struct.unpack_from("<5I", buf, pos)
            pos += 20
            fname, pos = _read_name(buf, pos, nl)
            entries.append(Entry("/".join(path + [fname]), off, stored, size, crc))

        for _ in range(nsub):
            pos = parse_dir(pos, path, False)
        return pos

    end = parse_dir(ROOT_RECORD_OFFSET, [], True)
    if end != len(buf):
        print("warning: BAH parse ended at 0x%X but file is 0x%X bytes" % (end, len(buf)),
              file=sys.stderr)
    return entries


# --------------------------------------------------------------------------- #
# LZSS decompression
# --------------------------------------------------------------------------- #
def lzss_decompress(src, size):
    """
    Decode the Bark LZSS stream. Returns (output_bytes, source_bytes_consumed).

    The ring buffer is just a 4096-byte sliding window over the output, so we
    keep 4096 virtual zero bytes in front of the output and translate each
    absolute ring position into a back-distance.
    """
    out = bytearray(RING)            # zero pre-roll == zero-initialised ring
    base = RING                      # index where real output begins
    target = base + size
    i = 0
    n = len(src)

    while len(out) < target:
        if i >= n:
            raise FormatError("compressed stream ended early")
        flags = src[i]
        i += 1
        for bit in range(8):
            if len(out) >= target:
                break
            if (flags >> bit) & 1:
                if i >= n:
                    raise FormatError("compressed stream ended early")
                out.append(src[i])
                i += 1
            else:
                if i + 1 >= n:
                    raise FormatError("compressed stream ended early")
                b0 = src[i]
                b1 = src[i + 1]
                i += 2
                pos = b0 | ((b1 & 0xF0) << 4)
                length = (b1 & 0x0F) + 3
                r = (RING_START + len(out) - base) & (RING - 1)   # write pointer
                dist = (r - pos) & (RING - 1) or RING
                start = len(out) - dist
                if dist >= length:
                    out += out[start:start + length]
                else:  # overlapping copy: repeat the `dist`-byte pattern
                    pattern = out[start:start + dist]
                    out += (pattern * (length // dist + 1))[:length]

    del out[target:]                 # a final match may overshoot
    return bytes(out[base:]), i


def extract_blob(bad, entry):
    """Return the uncompressed bytes for an entry, validating as we go."""
    off, stored = entry.offset, entry.stored_size
    if off + stored > len(bad) or stored < 8:
        raise FormatError("entry lies outside the BAD file")
    method, size = struct.unpack_from("<II", bad, off)
    if size != entry.size:
        raise FormatError("size mismatch (BAH %d vs BAD %d)" % (entry.size, size))
    payload = bad[off + 8:off + stored]

    if method == 0:
        if len(payload) < size:
            raise FormatError("stored payload shorter than declared size")
        return bytes(payload[:size])
    if method == 1:
        data, used = lzss_decompress(payload, size)
        if len(payload) - used > 3:          # only 4-byte alignment padding allowed
            raise FormatError("%d unused bytes after compressed stream"
                              % (len(payload) - used))
        return data
    raise FormatError("unknown compression method %d" % method)


# --------------------------------------------------------------------------- #
# Driver
# --------------------------------------------------------------------------- #
def safe_join(root, rel):
    """Join an archive path onto root, refusing anything that escapes it."""
    parts = [p for p in rel.replace("\\", "/").split("/") if p not in ("", ".")]
    if any(p == ".." for p in parts):
        raise FormatError("unsafe path in archive: %r" % rel)
    dest = os.path.normpath(os.path.join(root, *parts))
    root_abs = os.path.abspath(root)
    if os.path.commonpath([root_abs, os.path.abspath(dest)]) != root_abs:
        raise FormatError("unsafe path in archive: %r" % rel)
    return dest


def find_bah(bad_path):
    stem, _ = os.path.splitext(bad_path)
    for ext in (".bah", ".BAH", ".Bah"):
        if os.path.exists(stem + ext):
            return stem + ext
    # case-insensitive fallback
    folder = os.path.dirname(os.path.abspath(bad_path))
    want = os.path.basename(stem).lower() + ".bah"
    for f in os.listdir(folder):
        if f.lower() == want:
            return os.path.join(folder, f)
    raise SystemExit("could not find a matching .BAH next to %s (use --bah)" % bad_path)


def main():
    ap = argparse.ArgumentParser(description="Extract a Bark 1.06 .BAD/.BAH archive.")
    ap.add_argument("bad", help="path to the .BAD data file")
    ap.add_argument("--bah", help="path to the .BAH header (default: same name next to the BAD)")
    ap.add_argument("-o", "--output", help="output directory (default: BAD file name without extension)")
    ap.add_argument("-l", "--list", action="store_true", help="list contents, do not extract")
    ap.add_argument("-f", "--filter", action="append", metavar="GLOB",
                    help="only handle paths matching this glob (repeatable, case-insensitive)")
    ap.add_argument("-q", "--quiet", action="store_true", help="don't print each file")
    args = ap.parse_args()

    bah_path = args.bah or find_bah(args.bad)
    with open(bah_path, "rb") as f:
        bah = f.read()
    try:
        entries = parse_bah(bah)
    except FormatError as e:
        sys.exit("BAH error: %s" % e)

    if args.filter:
        pats = [p.lower() for p in args.filter]
        entries = [e for e in entries if any(fnmatch.fnmatch(e.path.lower(), p) for p in pats)]

    if args.list:
        total = 0
        for e in entries:
            print("%10d  %s" % (e.size, e.path))
            total += e.size
        print("%d files, %d bytes uncompressed" % (len(entries), total))
        return

    out_root = args.output or os.path.splitext(os.path.basename(args.bad))[0]
    ok = failed = 0
    total = 0
    seen = {}

    with open(args.bad, "rb") as f, mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ) as bad:
        for n, e in enumerate(entries, 1):
            try:
                data = extract_blob(bad, e)
                dest = safe_join(out_root, e.path)
                # avoid silently clobbering entries that collide on this filesystem
                key = os.path.normcase(dest)
                if key in seen:
                    seen[key] += 1
                    root, ext = os.path.splitext(dest)
                    dest = "%s.dup%d%s" % (root, seen[key], ext)
                    print("note: duplicate path, writing %s" % dest, file=sys.stderr)
                else:
                    seen[key] = 0
                os.makedirs(os.path.dirname(dest), exist_ok=True)
                with open(dest, "wb") as out:
                    out.write(data)
                ok += 1
                total += len(data)
                if not args.quiet:
                    print("[%d/%d] %s (%d bytes)" % (n, len(entries), e.path, len(data)))
            except (FormatError, OSError) as ex:
                failed += 1
                print("FAILED %s: %s" % (e.path, ex), file=sys.stderr)

    print("Done: %d extracted (%d bytes), %d failed -> %s" % (ok, total, failed, out_root))
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
