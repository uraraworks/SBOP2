#!/usr/bin/env python3
"""マップパーツ PNG を webroot へ同期するスクリプト。

Git では追跡しないフォルダ (<repo>/SboSvr/webroot/assets/map-parts) を出力先とします。
"""

from __future__ import annotations

import argparse
import shutil
import sys
from pathlib import Path
from typing import Iterable, Tuple

ManifestEntry = Tuple[Path, Path]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="マップパーツ PNG を webroot へコピーします")
    parser.add_argument(
        "--source",
        type=Path,
        default=Path("SboGrpData/res"),
        help="コピー元ディレクトリ (既定: SboGrpData/res)",
    )
    parser.add_argument(
        "--dest",
        type=Path,
        default=Path("SboSvr/webroot/assets/map-parts"),
        help="コピー先ディレクトリ (既定: SboSvr/webroot/assets/map-parts)",
    )
    parser.add_argument(
        "--manifest",
        type=Path,
        default=Path("SboSvr/webroot/map-parts-assets.manifest"),
        help="コピー対象を定義したマニフェスト (既定: SboSvr/webroot/map-parts-assets.manifest)",
    )
    parser.add_argument(
        "--clean",
        action="store_true",
        help="コピー前に出力先を削除します",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="ファイル操作を行わず処理内容だけ表示します",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="コピーするファイルを詳細表示します",
    )
    return parser.parse_args()


def validate_paths(source: Path, dest: Path, manifest: Path) -> None:
    if not source.exists():
        sys.exit(f"コピー元が存在しません: {source}")
    if not source.is_dir():
        sys.exit(f"コピー元がディレクトリではありません: {source}")
    if not manifest.exists():
        sys.exit(f"マニフェストが存在しません: {manifest}")
    if not manifest.is_file():
        sys.exit(f"マニフェストがファイルではありません: {manifest}")
    try:
        source.relative_to(dest)
    except ValueError:
        return
    else:
        sys.exit("出力先がコピー元の配下にあります。--dest を見直してください。")


def load_manifest(manifest: Path) -> list[ManifestEntry]:
    entries: list[ManifestEntry] = []
    with manifest.open("r", encoding="utf-8") as fp:
        for index, raw_line in enumerate(fp, start=1):
            line = raw_line.strip()
            if not line or line.startswith("#"):
                continue
            if ">" in line:
                src_raw, dst_raw = line.split(">", 1)
            else:
                src_raw, dst_raw = line, ""
            src_rel = Path(src_raw.strip().replace("\\", "/"))
            dst_raw = dst_raw.strip()
            dst_rel = Path(dst_raw.replace("\\", "/")) if dst_raw else src_rel
            if src_rel == Path(".") or any(part == ".." for part in src_rel.parts):
                print(
                    f"[WARN] マニフェスト {manifest} の {index} 行目に無効なパスがあります: '{raw_line.rstrip()}'"
                )
                continue
            if dst_rel == Path(".") or any(part == ".." for part in dst_rel.parts):
                print(
                    f"[WARN] マニフェスト {manifest} の {index} 行目に無効な出力先があります: '{raw_line.rstrip()}'"
                )
                continue
            entries.append((src_rel, dst_rel))
    return entries


def ensure_destination(dest: Path, clean: bool, dry_run: bool) -> None:
    if clean and dest.exists():
        print(f"[INFO] 出力先を削除します: {dest}")
        if not dry_run:
            shutil.rmtree(dest)
    if not dest.exists():
        print(f"[INFO] 出力先を作成します: {dest}")
        if not dry_run:
            dest.mkdir(parents=True, exist_ok=True)


def copy_entries(
    entries: Iterable[ManifestEntry],
    source: Path,
    dest: Path,
    dry_run: bool,
    verbose: bool,
) -> tuple[int, int]:
    copied = 0
    missing = 0
    for src_rel, dst_rel in entries:
        src_path = (source / src_rel).resolve()
        dest_path = dest / dst_rel
        display = f"{src_rel} -> {dst_rel}"
        if verbose:
            print(f"[COPY] {display}")
        if not src_path.exists():
            print(f"[WARN] コピー元が見つかりません: {src_path}")
            missing += 1
            continue
        if dry_run:
            continue
        dest_path.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src_path, dest_path)
        copied += 1
    return copied, missing


def main() -> None:
    args = parse_args()
    source = args.source.resolve()
    dest = args.dest.resolve()
    manifest = args.manifest.resolve()

    validate_paths(source, dest, manifest)
    entries = load_manifest(manifest)

    if not entries:
        sys.exit(f"マニフェストに有効なエントリがありません: {manifest}")

    ensure_destination(dest, args.clean, args.dry_run)
    copied_count, missing_count = copy_entries(entries, source, dest, args.dry_run, args.verbose)

    total = len(entries)
    if args.dry_run:
        print(f"[DRY-RUN] {total - missing_count} / {total} 件のファイルをコピー予定です")
    else:
        print(f"[DONE] {copied_count} / {total} 件のファイルをコピーしました → {dest}")
    if missing_count:
        print(f"[WARN] コピー元が見つからなかった項目: {missing_count} 件")


if __name__ == "__main__":
    main()
