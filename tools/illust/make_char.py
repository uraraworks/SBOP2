"""Codex で生成したキャラの歩行シート(4x4、背景マゼンタ、1マス256px)を、
ゲーム用のイラスト版キャラ(4x4、1セル128px、RGBA)に変換する。

使い方: python3 tools/illust/make_char.py <入力.png> <出力.png>
全コマを同じ倍率で縮め、足元(不透明部分の下端)をセル下端から FOOT px 上・横中央にそろえる。
行: 下 / 左 / 右 / 上、列: 歩行4コマ。
"""
import sys

from PIL import Image

CELL = 128
HEIGHT = 116	# キャラの高さ(全コマ中の最大)を何 px にするか
FOOT = 4		# 足元をセル下端から何 px 上に置くか


def key_out_keep(im):
	"""マゼンタ背景を透過する(切り抜きはしない)"""
	im = im.convert("RGBA")
	px = im.load()
	for y in range(im.height):
		for x in range(im.width):
			r, g, b, a = px[x, y]
			if r > 170 and b > 170 and g < 120:
				px[x, y] = (0, 0, 0, 0)
			elif r > 150 and b > 150 and g < 150:
				px[x, y] = (r, g, b, 90)
	return im


def main():
	src = key_out_keep(Image.open(sys.argv[1]))
	n = src.width // 4
	cells = [src.crop(((i % 4) * n, (i // 4) * n, (i % 4 + 1) * n, (i // 4 + 1) * n)) for i in range(16)]
	boxes = [c.getbbox() for c in cells]
	scale = HEIGHT / max(b[3] - b[1] for b in boxes)
	out = Image.new("RGBA", (CELL * 4, CELL * 4), (0, 0, 0, 0))
	for i, (c, b) in enumerate(zip(cells, boxes)):
		part = c.crop(b)
		part = part.resize((max(1, round(part.width * scale)), max(1, round(part.height * scale))), Image.LANCZOS)
		# 横はマスの中心を基準に、元の位置関係を保ったまま縮める(歩行で体が左右に揺れるのを残す)
		cx = (b[0] - n / 2) * scale
		x = (i % 4) * CELL + round(CELL / 2 + cx)
		y = (i // 4) * CELL + CELL - FOOT - part.height
		out.alpha_composite(part, (x, y))
	out.save(sys.argv[2], optimize=True)
	print("scale", round(scale, 3))


if __name__ == "__main__":
	main()
