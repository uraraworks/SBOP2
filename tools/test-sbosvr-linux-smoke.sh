#!/usr/bin/env bash
# Linux ビルドした SboSvr のスモークテスト
#
# 空の DB で --headless 起動し、HTTP とゲーム用 TCP ポートが応答することを確かめてから
# --stop で止める。CI（.github/workflows/linux.yml）と手元の両方で使う。
#
#   tools/test-sbosvr-linux-smoke.sh [SboSvr 実行ファイル]   （既定: out/cmake-linux/SboSvr）
#
# ポートは既定値（ゲーム 2006 / HTTP 18080 / WebSocket 18081）を使う。

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
EXE="${1:-$ROOT/out/cmake-linux/SboSvr}"
RUN_DIR="$(mktemp -d)"
HTTP="http://127.0.0.1:18080"
NG=0

cleanup() {
	if [ -x "$RUN_DIR/SboSvr" ]; then
		(cd "$RUN_DIR" && ./SboSvr --stop >/dev/null 2>&1) || true
	fi
	rm -rf "$RUN_DIR"
}
trap cleanup EXIT

ok() { echo "  OK   $1"; }
ng() { echo "  NG   $1"; NG=$((NG + 1)); }

# 実行ファイルの隣に webroot と SBODATA(.dat) を置く。DB は空から自動生成される
cp "$EXE" "$RUN_DIR/SboSvr"
cp -r "$ROOT/SboSvr/webroot" "$RUN_DIR/webroot"
mkdir -p "$RUN_DIR/SBODATA"
cp "$ROOT"/Release/SBODATA/*.dat "$RUN_DIR/SBODATA/"

echo "=== SboSvr Linux スモークテスト ==="
(cd "$RUN_DIR" && ./SboSvr --headless > "$RUN_DIR/stdout.log" 2>&1 &)

# HTTP が上がるまで待つ（最大30秒）
for _ in $(seq 1 60); do
	if curl --noproxy '*' -s -o /dev/null --max-time 2 "$HTTP/health"; then
		break
	fi
	sleep 0.5
done

http_code() { curl --noproxy '*' -s -o /dev/null --max-time 5 -w '%{http_code}' "$HTTP$1" || true; }

BODY="$(curl --noproxy '*' -s --max-time 5 "$HTTP/health" || true)"
if [ "$BODY" = '{"status":"ok","onlinePlayers":0}' ]; then ok "/health → $BODY"; else ng "/health → '$BODY'"; fi
[ "$(http_code /admin/)" = "200" ] && ok "/admin/ → 200" || ng "/admin/ → $(http_code /admin/)"
[ "$(http_code /account/)" = "200" ] && ok "/account/ → 200" || ng "/account/ → $(http_code /account/)"
[ "$(http_code '/admin/%2e%2e/%2e%2e/SboSvr')" = "400" ] && ok "親参照は 400" || ng "親参照 → $(http_code '/admin/%2e%2e/%2e%2e/SboSvr')"

# ゲーム用 TCP ポート: 接続直後にプリチェックのチャレンジ（ヘッダ8 + 本体4バイト）が届くか
if python3 - <<'PY'
import socket, struct, sys
s = socket.create_connection(("127.0.0.1", 2006), timeout=5)
d = b""
while len(d) < 12:
    b = s.recv(12 - len(d))
    if not b:
        break
    d += b
sys.exit(0 if len(d) == 12 and struct.unpack("<I", d[:4])[0] == 4 else 1)
PY
then ok "ゲームポート 2006 でプリチェックを受信"; else ng "ゲームポート 2006"; fi

# --stop で正常終了し、DB が保存されること
if (cd "$RUN_DIR" && ./SboSvr --stop >/dev/null); then ok "--stop で停止"; else ng "--stop"; fi
[ -s "$RUN_DIR/SBODATA/SboData.db" ] && ok "DB が保存された" || ng "DB が無い"

echo "=== NG $NG ==="
if [ "$NG" -ne 0 ]; then
	echo "--- サーバーログ（末尾） ---"
	tail -n 30 "$RUN_DIR/SboSvrLog.txt" 2>/dev/null || true
	exit 1
fi
