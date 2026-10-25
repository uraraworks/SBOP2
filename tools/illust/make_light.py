"""建物の影と日だまりを、地面に貼るデカール画像としてプログラムで作る。

使い方: python3 tools/illust/make_light.py SboGrpData/res/illust/object
出力: decal_wallshadow.png(建物の下に落ちる帯状の影)、decal_sun.png / decal_sun_s.png(日だまり)
"""
import sys

import numpy as np
from PIL import Image

SHADOW_TINT = (20, 30, 50)	# 影の色(少し青み)
SUN_TINT = (255, 238, 196)	# 日だまりの色(暖色)


def wall_shadow(w, h, alpha):
	"""上が濃く下へ薄くなる帯。左右の端もぼかす"""
	y = np.linspace(0, 1, h)[:, None]
	x = np.linspace(-1, 1, w)[None, :]
	a = (1 - y) ** 1.6 * np.clip((1 - np.abs(x)) * 6, 0, 1)
	return make(a * alpha, SHADOW_TINT)


def sun(w, h, alpha):
	"""ぼかした楕円の日だまり"""
	y = np.linspace(-1, 1, h)[:, None]
	x = np.linspace(-1, 1, w)[None, :]
	d = np.sqrt(x * x + y * y)
	a = np.clip(1 - d, 0, 1) ** 1.5
	return make(a * alpha, SUN_TINT)


def make(a, tint):
	h, w = a.shape
	img = np.zeros((h, w, 4), np.uint8)
	img[:, :, :3] = tint
	img[:, :, 3] = np.clip(a, 0, 255).astype(np.uint8)
	return Image.fromarray(img)


def main():
	out = sys.argv[1]
	wall_shadow(448, 112, 95).save(f"{out}/decal_wallshadow.png", optimize=True)
	sun(420, 280, 120).save(f"{out}/decal_sun.png", optimize=True)
	sun(240, 170, 100).save(f"{out}/decal_sun_s.png", optimize=True)


if __name__ == "__main__":
	main()
