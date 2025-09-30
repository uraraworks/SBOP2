#!/usr/bin/env python3
"""マップパーツ PNG を webroot へ同期するスクリプト。

ビルド時に等倍 PNG を Web 配信用フォルダへコピーするためのユーティリティです。
Git では追跡しないフォルダ (<repo>/SboSvr/webroot/assets/map-parts) を出力先とします。
"""

from __future__ import annotations

import argparse
import shutil
import sys
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="マップパーツ PNG を webroot へコピーします")
    parser.add_argument(
        "--source",
        type=Path,
        default=Path("SboGrpData/res/MapParts"),
        help="コピー元ディレクトリ (既定: SboGrpData/res/MapParts)",
    )
    parser.add_argument(
        "--dest",
        type=Path,
        default=Path("SboSvr/webroot/assets/map-parts"),
        help="コピー先ディレクトリ (既定: SboSvr/webroot/assets/map-parts)",
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


def validate_paths(source: Path, dest: Path) -> None:
    if not source.exists():
        sys.exit(f"コピー元が存在しません: {source}")
    if not source.is_dir():
        sys.exit(f"コピー元がディレクトリではありません: {source}")
    try:
        source.relative_to(dest)
    except ValueError:
        return
    else:
        sys.exit("出力先がコピー元の配下にあります。--dest を見直してください。")


def collect_png_files(source: Path) -> list[Path]:
    return sorted(path for path in source.rglob("*.png") if path.is_file())


def ensure_destination(dest: Path, clean: bool, dry_run: bool) -> None:
    if clean and dest.exists():
        print(f"[INFO] 出力先を削除します: {dest}")
        if not dry_run:
            shutil.rmtree(dest)
    if not dest.exists():
        print(f"[INFO] 出力先を作成します: {dest}")
        if not dry_run:
            dest.mkdir(parents=True, exist_ok=True)


def copy_files(files: list[Path], source: Path, dest: Path, dry_run: bool, verbose: bool) -> int:
    copied = 0
    for src_path in files:
        relative = src_path.relative_to(source)
        dest_path = dest / relative
        if verbose:
            print(f"[COPY] {relative}")
        if dry_run:
            continue
        dest_path.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src_path, dest_path)
        copied += 1
    return copied


def main() -> None:
    args = parse_args()
    source = args.source.resolve()
    dest = args.dest.resolve()

    validate_paths(source, dest)
    png_files = collect_png_files(source)

    if not png_files:
        sys.exit(f"PNG ファイルが見つかりませんでした: {source}")

    ensure_destination(dest, args.clean, args.dry_run)
    copied_count = copy_files(png_files, source, dest, args.dry_run, args.verbose)

    if args.dry_run:
        print(f"[DRY-RUN] {len(png_files)} 件のファイルをコピー予定です")
    else:
        print(f"[DONE] {copied_count} 件のファイルをコピーしました → {dest}")


if __name__ == "__main__":
    main()
