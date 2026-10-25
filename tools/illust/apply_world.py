"""export_world.py で書き出したイラスト調のワールドデータを、別の SboData.db に反映する。
何度実行しても同じ結果になる。**サーバーを止めてから**実行すること(動いているとサーバーの停止時に上書きされる)。

使い方: python3 tools/illust/apply_world.py <data/illust/world.json> <反映先の SboData.db>
- 列が無ければ追加する(サーバーが起動時に足すのと同じ列)
- 同じ ObjectID / (MapID, DataID) に一枚絵でない別のものがあれば、何も変えずに止まる
"""
import base64
import json
import sqlite3
import sys


def ensure_column(db, table, column, decl):
	cols = [r[1] for r in db.execute(f"pragma table_info({table})")]
	if column not in cols:
		db.execute(f"alter table {table} add column {column} {decl}")


def main():
	data = json.load(open(sys.argv[1], encoding="utf-8"))
	db = sqlite3.connect(sys.argv[2])
	ensure_column(db, "sys_map_parts", "GrpIDIllust", "INTEGER DEFAULT 0")
	ensure_column(db, "sys_map_object", "IllustFile", "TEXT DEFAULT ''")
	ensure_column(db, "sys_map_object", "IllustOffX", "INTEGER DEFAULT 0")
	ensure_column(db, "sys_map_object", "IllustOffY", "INTEGER DEFAULT 0")

	# 衝突の確認(一枚絵オブジェクト以外を上書きしない)
	for o in data["objects"]:
		r = db.execute("select IFNULL(IllustFile, ''), Name from sys_map_object where ObjectID=?", (o["objectId"],)).fetchone()
		if r is not None and r[0] == "":
			sys.exit(f"ObjectID {o['objectId']} は既に「{r[1]}」で使われています。中止しました")
	for p in data["places"]:
		r = db.execute("select o.ObjectID, IFNULL(o.IllustFile, '') from sys_map_objectdata d left join sys_map_object o"
					   " on d.ObjectID = o.ObjectID where d.MapID=? and d.DataID=?", (p["mapId"], p["dataId"])).fetchone()
		if r is not None and r[1] == "":
			sys.exit(f"マップ{p['mapId']}の配置 {p['dataId']} は既に別のオブジェクト({r[0]})です。中止しました")

	for p in data["parts"]:
		db.execute("update sys_map_parts set GrpIDIllust=?, PartsType = PartsType & ~? where PartsID=?",
				   (p["grpIdIllust"], p["clearPartsType"], p["partsId"]))
	for o in data["objects"]:
		n = o["grpCx"] * o["grpCy"]
		db.execute("delete from sys_map_object where ObjectID=?", (o["objectId"],))
		db.execute("delete from sys_map_object_anime where ObjectID=?", (o["objectId"],))
		db.execute("insert into sys_map_object(ObjectID, Attr, HideY, GrpCX, GrpCY, Hit, Name, HitData, IllustFile, IllustOffX, IllustOffY)"
				   " values(?,?,?,?,?,?,?,?,?,?,?)",
				   (o["objectId"], o["attr"], o["hideY"], o["grpCx"], o["grpCy"], o["hit"], o["name"],
					base64.b64decode(o["hitData"]), o["illustFile"], o["illustOffX"], o["illustOffY"]))
		db.execute("insert into sys_map_object_anime(ObjectID, Slot, Wait, Level, GrpIDData) values(?,0,0,0,?)",
				   (o["objectId"], bytes(2 * n)))
	for p in data["places"]:
		db.execute("delete from sys_map_objectdata where MapID=? and DataID=?", (p["mapId"], p["dataId"]))
		db.execute("insert into sys_map_objectdata(MapID, DataID, ObjectID, PosX, PosY) values(?,?,?,?,?)",
				   (p["mapId"], p["dataId"], p["objectId"], p["x"], p["y"]))
	db.commit()
	print("parts", len(data["parts"]), "objects", len(data["objects"]), "places", len(data["places"]))


if __name__ == "__main__":
	main()
