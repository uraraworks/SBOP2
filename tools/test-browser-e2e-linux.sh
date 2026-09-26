#!/usr/bin/env bash
# クラウド(Linux)内でサーバー＋ブラウザ版を起動し、Playwright でログイン → MAP まで入ってスクショを撮る。
# 続けて、テスト準備 API で敵を置いて戦い、気絶メニューが出るまでを確かめる。
# docs/cloud-dev-staging-plan.md の S5。
#
# 事前準備:
#   1) デバッグ API 入りのサーバー(自動確認専用。ステージングには使わない)
#        cmake -S . -B out/cmake-linux-debugapi -DCMAKE_BUILD_TYPE=Release -DSBO_DEBUG_API=ON
#        cmake --build out/cmake-linux-debugapi -j --target SboSvr
#   2) ブラウザ版: tools/build-sbocli-browser-title.sh(出力 out/browser-title)
#   3) node と playwright(npm i -g playwright など。Chromium はクラウド環境に同梱)
#
#   tools/test-browser-e2e-linux.sh [SboSvr 実行ファイル] [スクショ出力先]
#     既定: out/cmake-linux-debugapi/SboSvr / out/e2e
#
# ワールドデータは data/seed/SboData.db(本番の敵・NPC・アイテム入り、プレイヤーデータなし)を使う。
# SBO_E2E_EMPTY_DB=1 にすると、以前のように .dat だけから作った空の DB で動かす。
#
# ポートは既定値(ゲーム 2006 / HTTP 18080 / WebSocket 18081)を使う。
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
EXE="${1:-$ROOT/out/cmake-linux-debugapi/SboSvr}"
OUT_DIR="${2:-$ROOT/out/e2e}"
GAME_DIR="$ROOT/out/browser-title"
HTTP="http://127.0.0.1:18080"

[ -x "$EXE" ] || { echo "SboSvr が見つかりません: $EXE（-DSBO_DEBUG_API=ON でビルドしてください）" >&2; exit 1; }
[ -f "$GAME_DIR/sbocli-title.html" ] || { echo "ブラウザ版が見つかりません: $GAME_DIR（tools/build-sbocli-browser-title.sh でビルドしてください）" >&2; exit 1; }

# グローバルに入れた playwright を require できるようにする
if [ -z "${NODE_PATH:-}" ] && command -v npm >/dev/null 2>&1; then
	export NODE_PATH="$(npm root -g)"
fi

RUN_DIR="$(mktemp -d)"
cleanup() {
	if [ -x "$RUN_DIR/SboSvr" ]; then
		(cd "$RUN_DIR" && ./SboSvr --stop >/dev/null 2>&1) || true
	fi
	rm -rf "$RUN_DIR"
}
trap cleanup EXIT

# 実行ファイルの隣に webroot（game にブラウザ版）と SBODATA(.dat と seed の DB) を置く
cp "$EXE" "$RUN_DIR/SboSvr"
cp -r "$ROOT/SboSvr/webroot" "$RUN_DIR/webroot"
mkdir -p "$RUN_DIR/webroot/game" "$RUN_DIR/SBODATA"
cp "$GAME_DIR"/sbocli-title.* "$RUN_DIR/webroot/game/"
if [ -d "$GAME_DIR/BGM" ]; then
	cp -r "$GAME_DIR/BGM" "$RUN_DIR/webroot/game/"
fi
cp "$ROOT"/Release/SBODATA/*.dat "$RUN_DIR/SBODATA/"
if [ "${SBO_E2E_EMPTY_DB:-0}" != "1" ]; then
	cp "$ROOT/data/seed/SboData.db" "$RUN_DIR/SBODATA/SboData.db"
fi

(cd "$RUN_DIR" && ./SboSvr --headless > "$RUN_DIR/stdout.log" 2>&1 &)
for _ in $(seq 1 60); do
	if curl --noproxy '*' -s -o /dev/null --max-time 2 "$HTTP/health"; then
		break
	fi
	sleep 0.5
done

mkdir -p "$OUT_DIR"
status=0
# プロキシ環境でも loopback へ直接つなぐ
NO_PROXY="127.0.0.1,localhost" no_proxy="127.0.0.1,localhost" \
	node "$ROOT/tools/e2e/browser-enter-map.cjs" "$HTTP" "$OUT_DIR" || status=$?
# 敵を置いて戦い、反撃で気絶するまで(テスト準備 API /api/debug/npc・char-status を使う)
NO_PROXY="127.0.0.1,localhost" no_proxy="127.0.0.1,localhost" \
	node "$ROOT/tools/e2e/browser-battle.cjs" "$HTTP" "$OUT_DIR" || status=$?
cp "$RUN_DIR/stdout.log" "$OUT_DIR/server-stdout.log" 2>/dev/null || true
echo "スクショ・ログ: $OUT_DIR"
exit $status
