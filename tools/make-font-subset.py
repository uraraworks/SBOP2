"""ブラウザ版向けフォントサブセットの再生成スクリプト。

SboCli/font/*.otf は Noto Sans CJK の全部入り(汎CJK)で 1 本 16〜17MB ある。
ブラウザ版はこれを丸ごとプリロードしており、初回ロード 47.8MB のうち
32MB がフォントだった。日本語ゲームに必要な文字だけに絞ると 1/4 になる。

収録する文字は tools/font-subset-chars.txt に置いてある。中身は
「CP932 で表現できる全文字 + UI で使う記号ブロック」で、このゲームは
元々 CP932 のデータで作られているため実質すべてカバーできる。
CP932 の範囲外の漢字(JIS第3・第4水準など)は豆腐になる。

生成物 SboCli/font/subset/*.otf はリポジトリにコミットしてある。
ビルドが fonttools に依存しないようにするため、このスクリプトは
文字集合を変えたいときだけ手で実行する。

使い方:
    py -m venv .venv-font
    .venv-font\\Scripts\\python -m pip install fonttools
    .venv-font\\Scripts\\python tools/make-font-subset.py

文字集合そのものを作り直したい場合は --regen-charset を付ける。
"""

import argparse
import codecs
import os
import subprocess
import sys

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FONT_DIR = os.path.join(REPO_ROOT, "SboCli", "font")
SUBSET_DIR = os.path.join(FONT_DIR, "subset")
CHARSET_PATH = os.path.join(REPO_ROOT, "tools", "font-subset-chars.txt")

FONTS = ["NotoSansCJKjp-Regular.otf", "NotoSansCJKjp-Bold.otf"]

# UI で使う記号ブロック(CP932 に無いものも拾っておく)
EXTRA_RANGES = [
    (0x2010, 0x206F),  # 一般句読点
    (0x2190, 0x21FF),  # 矢印
    (0x2460, 0x24FF),  # 丸数字
    (0x2500, 0x257F),  # 罫線
    (0x25A0, 0x25FF),  # 幾何学模様
    (0x2600, 0x26FF),  # その他の記号
    (0x3000, 0x303F),  # CJK の記号と句読点
    (0x3040, 0x309F),  # ひらがな
    (0x30A0, 0x30FF),  # カタカナ
    (0xFF00, 0xFFEF),  # 半角・全角形
]


def build_charset():
    """CP932 で表現できる文字 + 記号ブロックの集合を作る。"""
    chars = set()
    for b1 in range(0x00, 0x100):
        try:
            chars.add(bytes([b1]).decode("cp932"))
        except Exception:
            pass
    for b1 in range(0x81, 0xFD):
        for b2 in range(0x40, 0xFD):
            try:
                chars.add(bytes([b1, b2]).decode("cp932"))
            except Exception:
                pass
    for low, high in EXTRA_RANGES:
        for code in range(low, high + 1):
            chars.add(chr(code))
    return {c for c in chars if c.isprintable() or c == " "}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--regen-charset", action="store_true",
                        help="tools/font-subset-chars.txt を作り直す")
    args = parser.parse_args()

    if args.regen_charset or not os.path.exists(CHARSET_PATH):
        chars = build_charset()
        with codecs.open(CHARSET_PATH, "w", "utf-8") as handle:
            handle.write("".join(sorted(chars)))
        print("charset: %d chars -> %s" % (len(chars), CHARSET_PATH))

    if not os.path.isdir(SUBSET_DIR):
        os.makedirs(SUBSET_DIR)

    for name in FONTS:
        src = os.path.join(FONT_DIR, name)
        dst = os.path.join(SUBSET_DIR, name)
        if not os.path.exists(src):
            print("skip (not found): %s" % src)
            continue
        # pyftsubset はコンソールスクリプトではなくモジュールとして呼ぶ。
        # 仮想環境の Scripts/ に PATH を通さなくても動かせる。
        command = [
            sys.executable, "-m", "fontTools.subset", src,
            "--text-file=" + CHARSET_PATH,
            "--output-file=" + dst,
        ]
        subprocess.check_call(command)
        before = os.path.getsize(src)
        after = os.path.getsize(dst)
        print("%s: %d -> %d bytes (%.1f%%)" % (name, before, after, after * 100.0 / before))


if __name__ == "__main__":
    main()
