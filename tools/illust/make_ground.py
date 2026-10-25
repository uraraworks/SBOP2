"""Codex で生成した地面テクスチャを、縁の枠を落として継ぎ目なし(上下左右に並べられる)にし、
参考画像の色味に寄せて SboGrpData/res/illust/ground/ に書き出す。

使い方: python3 tools/illust/make_ground.py <入力.png> <出力.png> [色の参考画像.png x0 y0 x1 y1]
"""
import sys

import numpy as np
from PIL import Image

CROP = 8	# 生成画像の縁に出る枠の幅
BAND = 40	# 継ぎ目をなじませる幅(片側)
SIZE = 1024
REPEAT = 2	# 敷石を参考画像の縮尺に合わせるため、縮めて REPEAT x REPEAT 枚並べる


def seamless(a):
	"""半分ずらして継ぎ目を中央に寄せ、中央の十字だけ元画像(そこは連続している)と混ぜる"""
	h, w = a.shape[:2]
	r = np.roll(a, (h // 2, w // 2), axis=(0, 1))
	def ramp(n):
		d = np.abs(np.arange(n) - n / 2)
		return np.clip(1 - d / BAND, 0, 1)
	m = np.maximum(ramp(h)[:, None], ramp(w)[None, :])[:, :, None]
	m = m * m * (3 - 2 * m)
	return r * (1 - m) + a * m


def match_color(a, ref):
	"""RGB の平均と標準偏差を参考に合わせる(Reinhard 法の簡易版)"""
	out = (a - a.mean((0, 1))) / (a.std((0, 1)) + 1e-6) * ref.std((0, 1)) + ref.mean((0, 1))
	return np.clip(out, 0, 255)


def main():
	src, dst = sys.argv[1], sys.argv[2]
	a = np.asarray(Image.open(src).convert("RGB")).astype(np.float32)[CROP:-CROP, CROP:-CROP]
	a = seamless(a)
	if len(sys.argv) >= 8:
		x0, y0, x1, y1 = map(int, sys.argv[4:8])
		ref = np.asarray(Image.open(sys.argv[3]).convert("RGB").crop((x0, y0, x1, y1))).astype(np.float32)
		a = match_color(a, ref)
	one = Image.fromarray(a.astype(np.uint8)).resize((SIZE // REPEAT, SIZE // REPEAT), Image.LANCZOS)
	im = Image.new("RGBA", (SIZE, SIZE))
	for y in range(REPEAT):
		for x in range(REPEAT):
			im.paste(one, (x * one.width, y * one.height))
	im.save(dst, optimize=True)


if __name__ == "__main__":
	main()
