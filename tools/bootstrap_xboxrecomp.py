#!/usr/bin/env python3
"""Fetch the XboxRecomp revision pinned by this project.

This is mainly for source-archive users. Normal Git clones should use:
    git submodule update --init --recursive
"""

from __future__ import annotations

import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DESTINATION = ROOT / "third_party" / "xboxrecomp"
REPOSITORY = "https://github.com/HawKDesigns/xboxrecomp.git"
COMMIT = "fb18ab9f37b2375d963a4c1989a5adce135fd237"


def run(*args: str, cwd: Path | None = None) -> None:
    print("+", " ".join(args))
    subprocess.run(args, cwd=cwd, check=True)


def main() -> int:
    if (DESTINATION / ".git").exists():
        run("git", "fetch", "origin", COMMIT, cwd=DESTINATION)
    elif DESTINATION.exists() and any(DESTINATION.iterdir()):
        print(
            f"{DESTINATION} already contains a source-archive copy. "
            "No download is needed."
        )
        return 0
    else:
        DESTINATION.parent.mkdir(parents=True, exist_ok=True)
        run("git", "clone", "--no-checkout", REPOSITORY, str(DESTINATION))
    run("git", "checkout", "--detach", COMMIT, cwd=DESTINATION)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

