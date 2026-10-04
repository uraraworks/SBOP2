#!/usr/bin/env python3
"""指カーソルと名前横のマークを「1 ドット = 2x2 ピクセル」で描いて system.png に書き込む。

使い方: python3 tools/pixelart/cursor-mark-2x.py <入力.png> <出力.png> [<確認用拡大.png>]

絵は下の文字の並びで描く（1 文字 = 1 ドット）。色はパレットの番号で指定する。
- 指カーソル（12x12 ドット = 24x24px）は元と同じ場所に上書きする。
  右上（72,0）の斜めの指はコマンドメニュー専用で、メニューごと 2 倍にするのでそのまま残す。
- 名前横のマーク（10x10 ドット = 20x20px）は空いている MARK_X, MARK_Y から横に並べる。
"""
import sys
from PIL import Image

# 文字 → パレット番号
COLORS = {
    '.': 0,    # 透過
    'K': 2,    # 黒
    'W': 1,    # 白
    'G': 13,   # 灰
    'B': 84,   # 影（青紫）
    'Y': 149,  # 黄
    'g': 177,  # 黄緑
    'D': 192,  # 緑
    'S': 3,    # 肌
    'U': 68,   # 青
    'R': 123,  # 赤
}

CURSORS = {
    # 指さし（メニュー用）
    (48, 0): """
............
....KK......
...KWWK.....
KKKKWWKKKKK.
KWKWWWWWWWWK
KWKWWWWKKKK.
KWKWWWWWWK..
KWKGWWWWWK..
KKKGGWWWK...
.BKKKKKKK...
..BBBBBBB...
............
""",
    # 開いた手（アイテム・スキル選択用）
    (48, 24): """
............
............
KKK.KKKKKK..
KWKKWWWWWWK.
KWKWWWWGGGWK
KWKWWWWWWWWK
KWKWWWWGGGWK
KWKGWWWWWWK.
KKKGGKKKKK..
.BKKKBBBBB..
..BBB.......
............
""",
    # にぎった手（ドラッグ中）
    (72, 24): """
............
............
KKK.KKKKK...
KWKKWWWWWK..
KWKWWWWWWWK.
KWKWWGWGWWK.
KWKWWWWWWWK.
KWKGWWWWWK..
KKKGGKWWK...
.BKKKBKKB...
..BBB.BB....
............
""",
}

MARK_X, MARK_Y = 480, 640
MARKS = [
    # 1: 初心者（若葉）
    """
.WWW..WWW.
WKKKWWKKKW
WKYYKKDDKW
WKYYYDDDKW
WKYYYDDDKW
WKYYYDDDKW
.WKYYDDKW.
..WKYDKW..
...WKKW...
....WW....
""",
    # 2: 青い兜の顔
    """
..WWWWWW..
.WKKKKKKW.
WKUUUUUUKW
WKUYYYYUKW
WKSSSSSSKW
WKSKSSKSKW
WKSSSSSSKW
.WKSSSSKW.
..WKKKKW..
...WWWW...
""",
    # 3: 赤い髪の顔
    """
.WW....WW.
WYKWWWWKYW
WKRKKKKRKW
WKRRRRRRKW
WKRSSSSRKW
WKSKSSKSKW
WKRSSSSRKW
WKRKSSKRKW
.WKWKKWKW.
..W.WW.W..
""",
    # 4: 輪
    """
..........
..WWWWWW..
.WKKKKKKW.
WKYYYYYYKW
WKYKKKKYKW
WKYKKKKYKW
WKYYYYYYKW
.WKKKKKKW.
..WWWWWW..
..........
""",
    # 5: 羽根
    """
......WWWW
.....WKKKW
....WKWWKW
...WKWWGKW
..WKWWGKW.
.WKWWGKW..
WKWWGKKW..
WKYKKKW...
WKKWWW....
.WW.......
""",
    # 6: クローバー
    """
..WW..WW..
.WKKWWKKW.
WKggKKggKW
WKgDggDgKW
.WKggggKW.
WKgDggDgKW
WKggKKggKW
.WKKWKKKW.
..WWWKgKW.
.....WKW..
""",
]


def put(px, x0, y0, art):
    rows = [r for r in art.strip('\n').split('\n')]
    for dy, row in enumerate(rows):
        for dx, ch in enumerate(row):
            c = COLORS[ch]
            for yy in (0, 1):
                for xx in (0, 1):
                    px[x0 + dx * 2 + xx, y0 + dy * 2 + yy] = c


def main():
    im = Image.open(sys.argv[1])
    assert im.mode == 'P'
    px = im.load()
    for (x0, y0), art in CURSORS.items():
        for y in range(24):
            for x in range(24):
                px[x0 + x, y0 + y] = 0
        put(px, x0, y0, art)
    for i, art in enumerate(MARKS):
        x0 = MARK_X + i * 20
        for y in range(20):
            for x in range(20):
                px[x0 + x, MARK_Y + y] = 0
        put(px, x0, MARK_Y, art)
    im.save(sys.argv[2], optimize=True)
    if len(sys.argv) > 3:
        rgb = im.convert('RGB')
        a = rgb.crop((48, 0, 96, 48)).resize((192, 192), Image.NEAREST)
        b = rgb.crop((MARK_X, MARK_Y, MARK_X + 20 * len(MARKS), MARK_Y + 20)).resize((20 * len(MARKS) * 4, 80), Image.NEAREST)
        out = Image.new('RGB', (max(a.width, b.width), a.height + b.height + 8), (40, 120, 40))
        out.paste(a, (0, 0))
        out.paste(b, (0, a.height + 8))
        out.save(sys.argv[3])


if __name__ == '__main__':
    main()
