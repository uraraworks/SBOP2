#!/usr/bin/env python3
"""system.png の枠やパネルを「1 ドット = 2x2 ピクセル」の絵に描き直す。

使い方: python3 tools/pixelart/system-2x.py <入力.png> <出力.png>

元の絵は 1 ピクセルの線で描かれているので、ゲーム画面（2 倍ドット）と並べると線だけ細く見える。
TILES に挙げた部品を次のように作り直す。

- 外周 4 ピクセル（黒い縁・内側の線・角の丸め）は 2 倍に引き伸ばして 8 ピクセルにする。
  枠は 8 ピクセル単位で切り貼りして描くので、縁の絵が切れ目をまたがない。
- 内側は偶数座標の 2x2 マスごとに色をそろえる。いちばん多い色を使い、同数なら暗い色を残す
  （1 ピクセルの線は消えずに 2 ピクセルの線になる）。
- パレットは変えない（元の画像にある色だけを使う）。0 番（マゼンタ）が透過色。

小さなアイコンや文字入りの絵は 1 ドットの細かい絵なので、ここでは触らない。
"""
import sys
from collections import Counter
from PIL import Image

# 外周として 2 倍にする幅（元の絵のピクセル数）
BORDER = 4

# (左, 上, 幅, 高さ)
TILES = [
    (0, 0, 48, 48),                                    # 枠 0 番
    (0, 48, 48, 48), (48, 48, 48, 48), (96, 48, 48, 48),  # 枠 1〜3 番
    (0, 96, 48, 48),                                   # 枠 4 番（お知らせ）
    (0, 144, 24, 24), (24, 144, 24, 24), (0, 168, 24, 24),  # 名前表示などの小さい枠
    (0, 816, 48, 48), (48, 816, 48, 48), (96, 816, 48, 48),  # 枠 5〜7 番（ステータスの欄など）
]


def main():
    src, dst = sys.argv[1], sys.argv[2]
    im = Image.open(src)
    assert im.mode == 'P'
    pal = im.getpalette()
    px = im.load()
    out = im.copy()
    opx = out.load()

    def lum(i):
        r, g, b = pal[i * 3:i * 3 + 3]
        return r * 299 + g * 587 + b * 114

    def pick(cols):
        cnt = Counter(cols)
        best = max(cnt.values())
        cand = [c for c in cnt if cnt[c] == best]
        # 同数なら透過より絵、絵どうしなら暗い色
        cand.sort(key=lambda c: (c == 0, lum(c)))
        return cand[0]

    def src_coord(v, size):
        # 外周は 2 倍に引き伸ばし、内側はそのまま
        if v < BORDER * 2:
            return v // 2, True
        if v >= size - BORDER * 2:
            return size - 1 - (size - 1 - v) // 2, True
        return v, False

    for x0, y0, w, h in TILES:
        for y in range(0, h, 2):
            for x in range(0, w, 2):
                sx, bx = src_coord(x, w)
                sy, by = src_coord(y, h)
                if bx or by:
                    # 外周: 対応する 1 ピクセルをそのまま 2x2 に
                    if bx and by:
                        c = px[x0 + sx, y0 + sy]
                    elif bx:
                        c = pick([px[x0 + sx, y0 + y], px[x0 + sx, y0 + y + 1]])
                    else:
                        c = pick([px[x0 + x, y0 + sy], px[x0 + x + 1, y0 + sy]])
                else:
                    c = pick([px[x0 + x, y0 + y], px[x0 + x + 1, y0 + y],
                              px[x0 + x, y0 + y + 1], px[x0 + x + 1, y0 + y + 1]])
                for dy in (0, 1):
                    for dx in (0, 1):
                        opx[x0 + x + dx, y0 + y + dy] = c
    out.save(dst, optimize=True)


if __name__ == '__main__':
    main()
