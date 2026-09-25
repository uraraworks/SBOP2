#!/usr/bin/env bash
# ステージング用にブラウザ版(out/browser-title)を仕上げる。
# 本番の tools/publish.ps1 と同じ wss 書換シムを入れ、事前圧縮版(.br/.gz)を作り直す。
#
#   deploy/staging/prepare-browser.sh [ブラウザ版の出力ディレクトリ(既定: out/browser-title)]
#
# シムの中身: https で開かれた時だけ、ゲームが作る ws://<host>:<port> を
# wss://<同じホスト>/__sbop2bridge__ に置き換える(Caddy が 18081 に渡す)。
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
DIR="${1:-$ROOT/out/browser-title}"
HTML="$DIR/sbocli-title.html"
[ -f "$HTML" ] || { echo "見つかりません: $HTML(先に tools/build-sbocli-browser-title.sh を実行)" >&2; exit 1; }

node - "$HTML" <<'JS'
const fs = require('fs');
const file = process.argv[2];
const marker = '/*__SBOP2_WSS_SHIM__*/';
// tools/publish.ps1 のシムと同じ内容
const shim = '<script>' + marker +
  "(function(){if(location.protocol!=='https:')return;" +
  "var N=window.WebSocket;function P(u,p){try{if(typeof u==='string'&&u.indexOf('ws://')===0){" +
  "u='wss://'+location.host+'/__sbop2bridge__';}}catch(e){}" +
  "return (arguments.length>1)?new N(u,p):new N(u);}" +
  'P.prototype=N.prototype;P.OPEN=N.OPEN;P.CONNECTING=N.CONNECTING;P.CLOSING=N.CLOSING;P.CLOSED=N.CLOSED;' +
  'window.WebSocket=P;})();</script>';
let html = fs.readFileSync(file, 'utf8');
if (html.includes(marker)) {
  console.log('[prepare-browser] wss シムは注入済み');
} else {
  const m = html.match(/<\/title>/i);
  if (!m) { console.error('[prepare-browser] </title> が見つからずシムを入れられません'); process.exit(1); }
  html = html.replace(/<\/title>/i, (t) => t + '\n' + shim);
  fs.writeFileSync(file, html, 'utf8');
  console.log('[prepare-browser] wss シムを注入しました');
}
JS

# SboSvr は圧縮版を優先して返すので、シム入りの HTML から作り直す(docs/codebase/pitfalls.md 参照)
rm -f "$HTML.br" "$HTML.gz"
node "$ROOT/tools/emscripten/precompress.mjs" "$DIR" sbocli-title.html
for ext in br gz; do
	if [ "$ext" = br ]; then
		content="$(node -e "process.stdout.write(require('zlib').brotliDecompressSync(require('fs').readFileSync(process.argv[1])).toString())" "$HTML.br")"
	else
		content="$(gzip -dc "$HTML.gz")"
	fi
	case "$content" in
		*'/*__SBOP2_WSS_SHIM__*/'*) ;;
		*) echo "[prepare-browser] $HTML.$ext にシムが入っていません" >&2; exit 1 ;;
	esac
done
echo "[prepare-browser] 完了: $DIR"
