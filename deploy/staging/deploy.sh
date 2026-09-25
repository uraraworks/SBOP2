#!/usr/bin/env bash
# ステージングを最新にする(ステージングのホストで実行。GitHub Actions からも呼ぶ)。
#
#   deploy/staging/deploy.sh
#
# 前提: このディレクトリに staging.env があり、../../out/browser-title に
# wss シム入りのブラウザ版(prepare-browser.sh 済み)が置かれていること。
set -euo pipefail

cd "$(dirname "$0")"
[ -f staging.env ] || { echo "staging.env がありません(staging.env.example をコピーして作成)" >&2; exit 1; }
[ -f ../../out/browser-title/sbocli-title.html ] || { echo "ブラウザ版がありません: out/browser-title" >&2; exit 1; }
grep -q '__SBOP2_WSS_SHIM__' ../../out/browser-title/sbocli-title.html || { echo "ブラウザ版に wss シムが入っていません(prepare-browser.sh)" >&2; exit 1; }

# SboSvr のイメージを作り直して入れ替える。Caddy はそのまま(証明書も保持)
docker compose build sbosvr
docker compose up -d
docker image prune -f >/dev/null

# 起動確認(Caddy を通さず、コンテナ内から直接叩く)
for _ in $(seq 1 30); do
	if docker compose exec -T caddy wget -q -O - http://127.0.0.1:18080/health 2>/dev/null; then
		echo
		echo "[deploy] 起動を確認しました"
		exit 0
	fi
	sleep 2
done
echo "[deploy] /health に応答がありません" >&2
docker compose logs --tail 50 sbosvr >&2
exit 1
