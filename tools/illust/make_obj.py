"""Codex で生成したオブジェクト画像(背景マゼンタ)を、ゲーム用の一枚絵と影に変換する。

使い方: python3 tools/illust/make_obj.py <入力フォルダ> <出力フォルダ> [名前 ...]
入力フォルダの obj_<名前>.png を読み、下の TABLE の大きさ(960 描画での px)に縮小して
<名前>.png(本体) と <名前>_shadow.png(影) を同じキャンバスサイズで書き出す。
キャンバスの下端中央が足元より SHADOW 分だけ下になるので、オブジェクトの位置補正Yに
表示される offY を入れる。
"""
import json
import random
import sys

from PIL import Image, ImageChops, ImageDraw, ImageFilter

# 名前: (幅px or None, 高さpx or None, 木漏れ日のまだら影にするか[, 元画像の名前, 左右反転するか])
TABLE = {
	"fountain": (340, None, False),
	"tree": (340, None, True),
	"lamp": (None, 210, False),
	"bench": (210, None, False),
	"bench_v": (64, None, False),
	"board": (120, None, False),
	"entrance": (440, None, False),
	"planter": (None, 52, False),
	"bush": (100, None, False),
	"stairs": (190, None, False),
	"gate": (320, None, False),
	"stairs2": (256, None, False),
	"stairs2_r": (256, None, False, "stairs2", True),	# 左右反転(右側の階段用)
	"planter2": (192, None, False),
}
SQUASH = 0.28	# 影の高さ(本体の高さに対する比)
SHEAR = 0.75	# 影を右へ倒す量
ALPHA = 90		# 影の濃さ
TINT = (20, 30, 40)	# 影の色(少し青み)
DROP_X = 0.10	# 横に広い物の影を右へずらす量(本体の幅に対する比)
DROP_Y = 0.07	# 同 下へずらす量(本体の高さに対する比)


def key_out(im):
	"""マゼンタ背景を透過し、不透明部分の外接矩形で切り抜く"""
	im = im.convert("RGBA")
	px = im.load()
	for y in range(im.height):
		for x in range(im.width):
			r, g, b, a = px[x, y]
			if r > 170 and b > 170 and g < 120:
				px[x, y] = (0, 0, 0, 0)
			elif r > 150 and b > 150 and g < 150:
				px[x, y] = (r, g, b, 90)
	return im.crop(im.getbbox())


def dapple_mask(sh):
	"""木漏れ日のまだら模様で影を間引く"""
	rnd = random.Random(1)
	m = Image.new("L", sh.size, 255)
	d = ImageDraw.Draw(m)
	for _ in range(sh.width * sh.height // 900):
		x, y = rnd.randrange(sh.width), rnd.randrange(sh.height)
		r = rnd.randrange(3, 9)
		d.ellipse((x - r, y - r, x + r, y + r), fill=rnd.randrange(60, 160))
	return ImageChops.multiply(sh, m.filter(ImageFilter.GaussianBlur(2)))


def make_drop_shadow(sprite, dapple):
	"""形をそのまま写した影(位置は呼び出し側でずらす)"""
	sh = sprite.split()[3].point(lambda v: ALPHA if v > 60 else 0).filter(ImageFilter.GaussianBlur(4))
	if dapple:
		sh = dapple_mask(sh)
	img = Image.new("RGBA", sh.size, TINT + (0,))
	img.putalpha(sh)
	return img


def make_shadow(sprite, dapple):
	"""左上光源の影: 足元を基点に右下へ倒した平たいシルエット"""
	w, h = sprite.size
	s = int(h * SQUASH)
	mask = sprite.split()[3]
	out_w = w + int(SHEAR * s)
	sh = mask.transform((out_w, s), Image.AFFINE,
						(1, -SHEAR, 0, 0, -1 / SQUASH, h), resample=Image.BILINEAR)
	sh = sh.point(lambda v: ALPHA if v > 60 else 0).filter(ImageFilter.GaussianBlur(3))
	if dapple:
		sh = dapple_mask(sh)
	img = Image.new("RGBA", sh.size, TINT + (0,))
	img.putalpha(sh)
	return img


def main():
	src_dir, out_dir = sys.argv[1], sys.argv[2]
	info = {}
	only = sys.argv[3:]	# 名前を並べるとそれだけ作る
	for name, spec in TABLE.items():
		if only and name not in only:
			continue
		w, h, dapple = spec[:3]
		src = spec[3] if len(spec) > 3 else name
		im = key_out(Image.open(f"{src_dir}/obj_{src}.png"))
		if len(spec) > 4 and spec[4]:
			im = im.transpose(Image.FLIP_LEFT_RIGHT)
		if w:
			im = im.resize((w, round(im.height * w / im.width)), Image.LANCZOS)
		else:
			im = im.resize((round(im.width * h / im.height), h), Image.LANCZOS)
		bw, bh = im.size
		if bw > bh * 0.6:
			# 噴水や木のように横に広い物: 形をなぞるように右下へずらした影
			dx, dy = round(bw * DROP_X), round(bh * DROP_Y)
			sh = make_drop_shadow(im, dapple)
			cw = bw + 2 * dx
			ch = bh + dy
			left = (cw - bw) // 2
			pos = (left + dx, dy)
		else:
			# 街灯のように細い物: 足元から右下へ倒した影
			sh = make_shadow(im, dapple)
			cw = bw + 2 * (sh.width - bw)
			ch = bh + sh.height
			left = (cw - bw) // 2
			pos = (left, bh)
		body = Image.new("RGBA", (cw, ch), (0, 0, 0, 0))
		body.paste(im, (left, 0))
		shadow = Image.new("RGBA", (cw, ch), (0, 0, 0, 0))
		shadow.paste(sh, pos)
		body.save(f"{out_dir}/{name}.png", optimize=True)
		shadow.save(f"{out_dir}/{name}_shadow.png", optimize=True)
		info[name] = {"canvas": [cw, ch], "body": [bw, bh], "offY": ch - bh}
	print(json.dumps(info, ensure_ascii=False, indent=1))


if __name__ == "__main__":
	main()
