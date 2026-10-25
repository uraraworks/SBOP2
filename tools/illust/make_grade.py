"""今のゲーム画面のスクショと目標画像から、画面全体の色補正表(LUT)を作る。

使い方: python3 tools/illust/make_grade.py <今の画面.png> <目標.png> <出力 grade.png> [強さ 0〜1]
チャンネルごとにヒストグラムを目標へ合わせる表を作り、強さの分だけ元の値と混ぜる。
出力は 256x1 の PNG(x 番目の画素が入力 x に対する出力の R/G/B)。
"""
import sys

import numpy as np
from PIL import Image


def main():
	cur = np.asarray(Image.open(sys.argv[1]).convert("RGB")).reshape(-1, 3).astype(np.float64)
	ref = np.asarray(Image.open(sys.argv[2]).convert("RGB")).reshape(-1, 3).astype(np.float64)
	strength = float(sys.argv[4]) if len(sys.argv) > 4 else 0.7
	x = np.arange(256, dtype=np.float64)
	lut = np.zeros((1, 256, 4), np.uint8)
	for c in range(3):
		m1, s1 = cur[:, c].mean(), cur[:, c].std()
		m2, s2 = ref[:, c].mean(), ref[:, c].std()
		# ヒストグラム合わせ: 今の画面で値 x の累積割合と同じ累積割合になる目標の値
		h1 = np.cumsum(np.bincount(cur[:, c].astype(int), minlength=256)) / len(cur)
		h2 = np.cumsum(np.bincount(ref[:, c].astype(int), minlength=256)) / len(ref)
		y = np.interp(h1, h2, x)
		y = x + (y - x) * strength
		lut[0, :, c] = np.clip(np.round(y), 0, 255)
		print("ch", c, "mean", round(m1, 1), "->", round(m2, 1), "std", round(s1, 1), "->", round(s2, 1))
	lut[0, :, 3] = 255
	Image.fromarray(lut).save(sys.argv[3])


if __name__ == "__main__":
	main()
