"""Codex で生成したデカールのシート(4x4、背景マゼンタ)を1枚ずつ切り出し、
SboGrpData/res/illust/object/decal_<名前>.png に書き出す(影なし)。

使い方: python3 tools/illust/make_decals.py <decals.png> <出力フォルダ>
"""
import sys

from PIL import Image

sys.path.insert(0, __file__.rsplit("/", 1)[0])
from make_obj import key_out  # noqa: E402

NAMES = [
	"moss_l", "moss_s", "crack", "chip",
	"leaves", "weed", "weed_row", "dirt",
	"stain", "pebbles", "petals", "stone_dark",
	"stone_light", "moss_joint", "grass_up", "grass_down",
]
SCALE = 0.45	# 256px のマスに描かれた物を 960 描画での大きさに縮める率


def main():
	sheet = Image.open(sys.argv[1]).convert("RGBA")
	cell = sheet.width // 4
	for i, name in enumerate(NAMES):
		c = sheet.crop(((i % 4) * cell, (i // 4) * cell, (i % 4 + 1) * cell, (i // 4 + 1) * cell))
		im = key_out(c)
		im = im.resize((max(1, round(im.width * SCALE)), max(1, round(im.height * SCALE))), Image.LANCZOS)
		im.save(f"{sys.argv[2]}/decal_{name}.png", optimize=True)
		print(name, im.size)


if __name__ == "__main__":
	main()
