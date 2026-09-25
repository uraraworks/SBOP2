#!/usr/bin/env bash
# ステージングの DB を入れ替える(ステージングのホストで実行)。
#
#   deploy/staging/load-db.sh <SboData.db があるディレクトリ>
#
# 入れる DB は必ず sanitize-db.py で個人情報を除いたものにすること。
# SboData.db と、あれば SboGrpData.db をボリューム(SBODATA)に上書きする。
set -euo pipefail

cd "$(dirname "$0")"
SRC="$(cd "${1:?SboData.db があるディレクトリを指定してください}" && pwd)"
[ -f "$SRC/SboData.db" ] || { echo "見つかりません: $SRC/SboData.db" >&2; exit 1; }

# 保存させてから止める(SIGTERM で DB を書き出して終わる)
docker compose stop sbosvr
docker compose run --rm --no-deps -v "$SRC:/in:ro" --entrypoint sh sbosvr -c '
	set -e
	rm -f SBODATA/SboData.db-wal SBODATA/SboData.db-shm
	cp /in/SboData.db SBODATA/SboData.db
	if [ -f /in/SboGrpData.db ]; then cp /in/SboGrpData.db SBODATA/SboGrpData.db; fi
	ls -l SBODATA/*.db'
docker compose start sbosvr
echo "[load-db] 入れ替えました"
