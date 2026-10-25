"""手元の DB に入れたイラスト調のワールドデータ(パーツのイラスト版タイル番号・一枚絵オブジェクトと配置)を
JSON に書き出す。シード DB や本番 DB へは apply_world.py で反映する。

使い方: python3 tools/illust/export_world.py <手元の SboData.db> <基準の SboData.db> <出力.json>
基準の DB(シード DB など)と比べて、「重なる」系ビットを外したパーツも記録する。
"""
import base64
import json
import sqlite3
import sys

PILE_BITS = 2 | 4 | 16	# BIT_PARTSHIT_PILE / PILEBACK / DRAWLAST


def main():
	src = sqlite3.connect(sys.argv[1])
	base = sqlite3.connect(sys.argv[2])
	base_type = dict(base.execute("select PartsID, PartsType from sys_map_parts"))

	parts = []
	for pid, illust, ptype in src.execute(
			"select PartsID, GrpIDIllust, PartsType from sys_map_parts where GrpIDIllust <> 0 order by PartsID"):
		clear = base_type.get(pid, ptype) & ~ptype & PILE_BITS
		parts.append({"partsId": pid, "grpIdIllust": illust, "clearPartsType": clear})

	objects = []
	ids = []
	for row in src.execute(
			"select ObjectID, Attr, HideY, GrpCX, GrpCY, Hit, Name, HitData, IllustFile, IllustOffX, IllustOffY"
			" from sys_map_object where IllustFile <> '' order by ObjectID"):
		oid, attr, hide, cx, cy, hit, name, hitdata, f, ox, oy = row
		ids.append(oid)
		objects.append({"objectId": oid, "attr": attr, "hideY": hide, "grpCx": cx, "grpCy": cy, "hit": hit,
						"name": name, "hitData": base64.b64encode(hitdata or b"").decode(),
						"illustFile": f, "illustOffX": ox, "illustOffY": oy})

	places = []
	q = "select MapID, DataID, ObjectID, PosX, PosY from sys_map_objectdata where ObjectID in (%s) order by MapID, DataID" % ",".join("?" * len(ids))
	for mid, did, oid, x, y in src.execute(q, ids):
		places.append({"mapId": mid, "dataId": did, "objectId": oid, "x": x, "y": y})

	with open(sys.argv[3], "w", encoding="utf-8") as fp:
		json.dump({"parts": parts, "objects": objects, "places": places}, fp, ensure_ascii=False, indent=1)
	print("parts", len(parts), "objects", len(objects), "places", len(places))


if __name__ == "__main__":
	main()
