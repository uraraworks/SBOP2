#!/usr/bin/env bash
# ステージングを最新にする(ステージングのホストで実行。GitHub Actions からも呼ぶ)。
#
#   deploy/staging/deploy.sh --image <sbop2-staging.tar.gz>   Actions で作ったイメージを読み込んで入れ替える
#   deploy/staging/deploy.sh                                  このホストでイメージを作って入れ替える
#
# メモリの少ないホスト(Google Cloud の e2-micro など)では C++ のビルドが重いので、
# GitHub Actions でイメージを作って送る(--image)。ホストで作る場合は、リポジトリ一式と
# wss シム入りのブラウザ版(prepare-browser.sh 済みの out/browser-title)が要る。
# どちらの場合も、このディレクトリに staging.env が必要。
set -euo pipefail

cd "$(dirname "$0")"
IMAGE_TAR=""
if [ "${1:-}" = "--image" ]; then
	IMAGE_TAR="${2:?--image にはイメージのファイルを指定してください}"
fi
[ -f staging.env ] || { echo "staging.env がありません(staging.env.example をコピーして作成)" >&2; exit 1; }

if [ -n "$IMAGE_TAR" ]; then
	echo "[deploy] イメージを読み込みます: $IMAGE_TAR"
	gzip -dc "$IMAGE_TAR" | docker load
else
	[ -f ../../out/browser-title/sbocli-title.html ] || { echo "ブラウザ版がありません: out/browser-title" >&2; exit 1; }
	grep -q '__SBOP2_WSS_SHIM__' ../../out/browser-title/sbocli-title.html || { echo "ブラウザ版に wss シムが入っていません(prepare-browser.sh)" >&2; exit 1; }
	docker compose build sbosvr
fi

# SboSvr を新しいイメージで作り直す。Caddy はそのまま(証明書も保持)
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
