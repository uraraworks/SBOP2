#!/usr/bin/env bash
# ブラウザ版クライアント(Emscripten)の Linux/macOS 向けビルドスクリプト。
# tools/build-sbocli-browser-title.ps1 + tools/test-sbocli-browser-preflight.ps1 の bash 版。
# ソース一覧は tools/browser-sources.txt を共通で読む。
#
# 使い方:
#   source /path/to/emsdk/emsdk_env.sh
#   tools/build-sbocli-browser-title.sh [--out-dir DIR] [--obj-dir DIR] [--jobs N] [--force] [--skip-compile]
#
#   --out-dir DIR    出力先(既定: out/browser-title)
#   --obj-dir DIR    .o/.d の出力先(既定: <out-dir>/obj)
#   --jobs N         並列コンパイル数(既定: CPU 数)
#   --force          差分判定をせず全ファイルを再コンパイル・再リンクする(--rebuild も可)
#   --skip-compile   コンパイルを飛ばしてリンク以降だけ行う
#
# BGM(Release/BGM/*.ogg)は git 管理外なので、無ければ警告だけ出して続行する。
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

OUT_DIR="out/browser-title"
OBJ_DIR=""
JOBS="$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)"
FORCE=0
SKIP_COMPILE=0

while [ $# -gt 0 ]; do
    case "$1" in
        --out-dir) OUT_DIR="$2"; shift 2 ;;
        --obj-dir) OBJ_DIR="$2"; shift 2 ;;
        --jobs|-j) JOBS="$2"; shift 2 ;;
        --force|--rebuild) FORCE=1; shift ;;
        --skip-compile) SKIP_COMPILE=1; shift ;;
        -h|--help) sed -n '2,16p' "$0"; exit 0 ;;
        *) echo "不明なオプション: $1" >&2; exit 2 ;;
    esac
done

# リポジトリ相対指定を絶対パスへ
case "$OUT_DIR" in /*) ;; *) OUT_DIR="$REPO_ROOT/$OUT_DIR" ;; esac
[ -n "$OBJ_DIR" ] || OBJ_DIR="$OUT_DIR/obj"
case "$OBJ_DIR" in /*) ;; *) OBJ_DIR="$REPO_ROOT/$OBJ_DIR" ;; esac

SOURCES_LIST="$SCRIPT_DIR/browser-sources.txt"
EGL_SWAP_POST="$SCRIPT_DIR/emscripten/egl_swapinterval_post.js"
SHELL_FILE="$SCRIPT_DIR/emscripten/sbocli-title.shell.html"
RES_DIR="$REPO_ROOT/SboGrpData/res"
# ブラウザ版はサブセット版フォントを使う(全部入りは 1 本 16〜17MB あり
# 初回ロードを大きく圧迫するため)。再生成は tools/make-font-subset.py。
FONT_DIR="$REPO_ROOT/SboCli/font/subset"
BGM_DIR="$REPO_ROOT/Release/BGM"
WAV_DIR="$REPO_ROOT/SboSoundData/res/WAVE"
HTML_OUT="$OUT_DIR/sbocli-title.html"

log()  { echo "[browser-build] $*"; }
warn() { echo "[browser-build] 警告: $*" >&2; }
die()  { echo "[browser-build] エラー: $*" >&2; exit 1; }

[ -f "$FONT_DIR/NotoSansCJKjp-Regular.otf" ] || die "サブセットフォントが見つかりません: $FONT_DIR (tools/make-font-subset.py で生成してください)"
[ -f "$SOURCES_LIST" ] || die "ブラウザ版ソース一覧ファイルが見つかりません: $SOURCES_LIST"

# 空行と # 行を除いたソース一覧(CRLF でも読めるよう \r を落とす)
SOURCES=()
while IFS= read -r line || [ -n "$line" ]; do
    line="${line%$'\r'}"
    line="$(echo "$line" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')"
    [ -z "$line" ] && continue
    case "$line" in \#*) continue ;; esac
    SOURCES+=("$line")
done < "$SOURCES_LIST"

# ファイル更新時刻(秒)。GNU stat と BSD stat の両対応
mtime() { stat -c %Y "$1" 2>/dev/null || stat -f %m "$1"; }

sha1_10() {
    if command -v sha1sum >/dev/null 2>&1; then
        sha1sum "$1" | cut -c1-10
    else
        shasum -a 1 "$1" | cut -c1-10
    fi
}

copy_bgm_assets() {
    # BGM は .data に含めず個別ファイルとして配信する。
    if [ ! -d "$BGM_DIR" ]; then
        warn "[browser-bgm] BGM ディレクトリが見つかりません(スキップします): $BGM_DIR"
        return 0
    fi
    mkdir -p "$OUT_DIR/BGM"
    local copied=0 f dest
    for f in "$BGM_DIR"/*.ogg; do
        [ -f "$f" ] || continue
        dest="$OUT_DIR/BGM/$(basename "$f")"
        # 変わっていないものは触らない(タイムスタンプを動かさない)
        if [ -f "$dest" ] && [ ! "$f" -nt "$dest" ]; then
            continue
        fi
        cp -p "$f" "$dest"
        copied=$((copied + 1))
    done
    echo "[browser-bgm] copied $copied ogg file(s) to $OUT_DIR/BGM"
}

precompress_assets() {
    local script="$SCRIPT_DIR/emscripten/precompress.mjs"
    if [ ! -f "$script" ]; then
        warn "[browser-precompress] precompress.mjs が見つかりません。スキップします。"
        return 0
    fi
    if ! command -v node >/dev/null 2>&1; then
        warn "[browser-precompress] node が見つからないため事前圧縮をスキップします。"
        return 0
    fi
    # 既に圧縮済みの ogg/png が大半を占める .data も、フォント(otf)が効くので対象に含める
    if ! node "$script" "$OUT_DIR" sbocli-title.wasm sbocli-title.js sbocli-title.html sbocli-title.data; then
        warn "[browser-precompress] 事前圧縮に失敗しました"
    fi
}

sync_to_webroot() {
    # SboSvr の webroot/game へ同期する(webroot がある構成の時だけ)。
    [ -f "$HTML_OUT" ] || return 0
    local webroot target
    for webroot in "$REPO_ROOT/SboSvr/Debug/webroot" "$REPO_ROOT/SboSvr/Release/webroot"; do
        [ -d "$webroot" ] || continue
        target="$webroot/game"
        mkdir -p "$target"
        cp -f "$OUT_DIR"/sbocli-title.* "$target/"
        # BGM は個別ファイル配信なのでディレクトリごと運ぶ
        if [ -d "$OUT_DIR/BGM" ]; then
            cp -rf "$OUT_DIR/BGM" "$target/"
        fi
        echo "[browser-deploy] copied browser client to $target"
    done
}

# ---- 全体の up-to-date 判定: 出力 html が監視対象より新しければ何もしない ----
if [ "$FORCE" -eq 0 ] && [ -f "$HTML_OUT" ]; then
    newest="$(find "$REPO_ROOT/SboCli/src" "$REPO_ROOT/SboCli/StdAfx.h" "$REPO_ROOT/Common" "$REPO_ROOT/imgui" \
            -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' -o -name '*.c' -o -name '*.cc' \) -newer "$HTML_OUT" -print -quit 2>/dev/null || true)"
    if [ -z "$newest" ]; then
        # ビルドスクリプト群やシェルファイル等の補助ファイルもチェック
        newest="$(find "$SCRIPT_DIR" -type f \( -name '*.sh' -o -name '*.ps1' -o -name '*.js' -o -name '*.mjs' -o -name '*.html' -o -name '*.json' -o -name '*.txt' \) \
                -newer "$HTML_OUT" -print -quit 2>/dev/null || true)"
    fi
    if [ -z "$newest" ]; then
        log "up-to-date, skipping"
        copy_bgm_assets
        sync_to_webroot
        exit 0
    fi
fi

command -v em++ >/dev/null 2>&1 || die "em++ が見つかりません。先に 'source <emsdk>/emsdk_env.sh' を実行してください。"

mkdir -p "$OUT_DIR" "$OBJ_DIR"

# ---- コンパイル(preflight 相当) ----
INCLUDE_DIRS=(
    "."
    "SboCli"
    "SboCli/src"
    "SboCli/src/Dialog"
    "SboCli/src/Info"
    "SboCli/src/Info/InfoChar"
    "SboCli/src/Layer"
    "SboCli/src/Lib"
    "SboCli/src/Lib/Png"
    "SboCli/src/LibInfo"
    "SboCli/src/MainFrame"
    "SboCli/src/StateProc"
    "SboCli/src/Window"
    "SboCli/src/Wnd"
    "Common"
    "Common/Lib"
    "Common/Lib/mfc"
    "Common/Info"
    "Common/Info/InfoMapEvent"
    "Common/Info/InfoItemType"
    "Common/Info/InfoSkill"
    "Common/Info/InfoTalkEvent"
    "Common/myLib"
    "Common/LibInfo"
    "Common/Packet"
    "Common/Packet/ACCOUNT"
    "Common/Packet/CHAR"
    "Common/Packet/CONNECT"
    "Common/Packet/ADMIN"
    "Common/Packet/CONECT"
    "Common/Packet/ITEM"
    "Common/Packet/MAP"
    "Common/Packet/MSGCMD"
    "Common/Packet/SKILL"
    # ps1 版は "myZLib" だが実ディレクトリは myZlib(Linux は大文字小文字を区別する)
    "Common/myLib/myZlib"
    "SboSoundData"
    "SDL2/include"
    "imgui"
    "imgui/backends"
)

COMPILE_ARGS=(
    -O3 -msimd128 -fno-strict-aliasing -c -std=gnu++17
    -D__EMSCRIPTEN__ -DUNICODE -D_UNICODE -DNO_DIRECTMUSIC -D_WFL_NONDMUSIC
    -sUSE_SDL=2 -sUSE_SDL_TTF=2 -sDISABLE_EXCEPTION_CATCHING=0
    -Winvalid-pch
    # -MMD: 依存関係ファイル(.d)を生成(差分ビルド用)
    -MMD
    -include "$REPO_ROOT/Common/rpcsal_fallback.h"
    -include "$REPO_ROOT/SboCli/StdAfx.h"
)
for d in "${INCLUDE_DIRS[@]}"; do
    COMPILE_ARGS+=(-I "$REPO_ROOT/$d")
done

# .o が .d に列挙された依存(ソース・ヘッダ)すべてより新しければ up-to-date
is_up_to_date() {
    local obj="$1" dep="$2" src="$3" f
    [ -f "$obj" ] || return 1
    if [ -f "$dep" ]; then
        # "target: dep1 dep2 \" 形式。行継続を連結し、コロン以降を空白で分割する
        # (スペース入りパスは想定しない)
        while IFS= read -r f; do
            [ -n "$f" ] || continue
            if [ -e "$f" ] && [ "$f" -nt "$obj" ]; then
                return 1
            fi
        done < <(sed -e ':a' -e '/\\$/N; s/\\\n/ /; ta' "$dep" | sed -e 's/^[^:]*:[[:space:]]*//' | tr -s ' \t' '\n')
        return 0
    fi
    [ "$obj" -nt "$src" ]
}

OBJECTS=()
TO_COMPILE=()
FAILED=()
SKIPPED=0
declare -A SEEN_BASENAMES=()
for src in "${SOURCES[@]}"; do
    src_path="$REPO_ROOT/$src"
    base="$(basename "${src%.*}")"
    # .o はベース名で置くので同名ファイルがあると上書きし合う(ps1 版と同じ制約)
    if [ -n "${SEEN_BASENAMES[$base]:-}" ]; then
        die "ソース一覧にベース名の重複があります: $src と ${SEEN_BASENAMES[$base]}"
    fi
    SEEN_BASENAMES[$base]="$src"
    obj="$OBJ_DIR/$base.o"
    OBJECTS+=("$obj")
    if [ ! -f "$src_path" ]; then
        warn "source not found: $src"
        FAILED+=("$src")
        continue
    fi
    [ "$SKIP_COMPILE" -eq 1 ] && continue
    if [ "$FORCE" -eq 0 ] && is_up_to_date "$obj" "$OBJ_DIR/$base.d" "$src_path"; then
        SKIPPED=$((SKIPPED + 1))
        continue
    fi
    TO_COMPILE+=("$src")
done

if [ "${#FAILED[@]}" -gt 0 ]; then
    die "ソースが見つかりません: ${FAILED[*]}"
fi

if [ "${#TO_COMPILE[@]}" -gt 0 ]; then
    log "compile: ${#TO_COMPILE[@]} file(s) (skipped: $SKIPPED, jobs: $JOBS)"
    FAIL_LIST="$OBJ_DIR/.failed"
    : > "$FAIL_LIST"
    compile_one() {
        local src="$1" base
        base="$(basename "${src%.*}")"
        echo "[browser-preflight] compile: $src"
        if ! em++ "${COMPILE_ARGS[@]}" "$REPO_ROOT/$src" -o "$OBJ_DIR/$base.o" -MF "$OBJ_DIR/$base.d"; then
            # 失敗した .o は残さない(次回 up-to-date と誤判定しないため)
            rm -f "$OBJ_DIR/$base.o"
            echo "$src" >> "$FAIL_LIST"
        fi
    }
    # 並列コンパイル。ジョブ数を超えたら 1 本終わるのを待つ
    running=0
    for src in "${TO_COMPILE[@]}"; do
        compile_one "$src" &
        running=$((running + 1))
        if [ "$running" -ge "$JOBS" ]; then
            wait -n || true
            running=$((running - 1))
        fi
    done
    wait || true
    if [ -s "$FAIL_LIST" ]; then
        die "browser preflight failed: $(tr '\n' ' ' < "$FAIL_LIST")"
    fi
    rm -f "$FAIL_LIST"
    log "compile success (compiled: ${#TO_COMPILE[@]}, skipped: $SKIPPED)"
else
    log "compile: all up-to-date (skipped: $SKIPPED)"
fi

# ---- リンク ----
LINK_ARGS=(
    -O3
    -msimd128
    -sUSE_SDL=2
    -sUSE_SDL_TTF=2
    -sALLOW_MEMORY_GROWTH=1
    -sASSERTIONS=0
    -sSTACK_SIZE=2097152
    "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','HEAPU8','FS']"
    "-sEXPORTED_FUNCTIONS=['_main','_malloc','_free','_SBOP2_SetWebAdminMode','_SBOP2_SetWebAdminSelectedPartsID','_SBOP2_AdminPlaySound','_SBOP2_BrowserChatSetComposition','_SBOP2_BrowserChatCommitText','_SBOP2_BrowserSetPadKey','_SBOP2_BrowserSetPadFacing']"
    -sDISABLE_EXCEPTION_CATCHING=0
    -lwebsocket
    -sENVIRONMENT=web
    -Wl,--error-limit=0
    --preload-file "$RES_DIR@/SboGrpData/res"
    --preload-file "$FONT_DIR@/font"
    # BGM は .data に同梱せず out/browser-title/BGM/ に個別配置し、
    # 起動後に emscripten_async_wget で取得する(初回ロードを軽くするため)。
    --preload-file "$WAV_DIR@/WAVE"
    --shell-file "$SHELL_FILE"
    --post-js "$EGL_SWAP_POST"
    -o "$HTML_OUT"
)

log "linking sbocli-title.html"
# コマンドライン長制限を避けるためレスポンスファイルを使う(ps1 版と同じ)
RSP_FILE="$OUT_DIR/link_args.rsp"
: > "$RSP_FILE"
for a in "${LINK_ARGS[@]}" "${OBJECTS[@]}"; do
    # レスポンスファイルは空白区切りなので、空白を含む引数は引用符で囲む
    case "$a" in
        *[[:space:]]*) printf '"%s"\n' "$a" >> "$RSP_FILE" ;;
        *) printf '%s\n' "$a" >> "$RSP_FILE" ;;
    esac
done
em++ "@$RSP_FILE" || die "browser link に失敗しました。"

# ---- キャッシュバスターと表示バージョンの埋め込み ----
# ファイル別ハッシュを付けて、通常のリロードだけで最新の .js/.wasm/.data を取らせる。
# 失敗してもビルドは止めない(ps1 版と同じく警告だけ)。
embed_cachebust() {
    local js="$OUT_DIR/sbocli-title.js" wasm="$OUT_DIR/sbocli-title.wasm" data="$OUT_DIR/sbocli-title.data"
    local js_hash wasm_hash data_hash version build_stamp data_size
    js_hash="$(sha1_10 "$js")" || return 1
    wasm_hash="$(sha1_10 "$wasm")" || return 1
    data_hash="$(sha1_10 "$data")" || return 1
    # 表示バージョンは Common/SBOVersion.h の VERTEXT を単一ソースとして読み取る
    version="$(sed -n 's/.*VERTEXT[[:space:]]*"\([^"]*\)".*/\1/p' "$REPO_ROOT/Common/SBOVersion.h" | head -n1)"
    [ -n "$version" ] || { warn "SBOVersion.h から VERTEXT を取得できませんでした。"; return 1; }
    # ビルド識別子はビルド日時(コミット有無に依存せず常に正直な値)
    build_stamp="$(date '+%Y-%m-%d %H:%M')"
    data_size="$(wc -c < "$data" | tr -d ' ')"

    # 置換は node で行う(sed だと引用符のエスケープが崩れやすいため)
    command -v node >/dev/null 2>&1 || { warn "node が見つかりません。"; return 1; }
    JS_HASH="$js_hash" WASM_HASH="$wasm_hash" DATA_HASH="$data_hash" \
    DISPLAY_VERSION="$version" BUILD_STAMP="$build_stamp" DATA_SIZE="$data_size" \
    node - "$HTML_OUT" <<'EOF' || return 1
const fs = require('fs');
const file = process.argv[2];
const e = process.env;
let html = fs.readFileSync(file, 'utf8');
// (1) script の js 参照にクエリを付与する(Emscripten 生成物は引用符なしのこともある)
html = html.replace(/(src=["']?sbocli-title\.js)(["'\s>])/g, (_, a, b) => `${a}?v=${e.JS_HASH}${b}`);
// (2) プレースホルダの meta をアセットハッシュ表+表示バージョン情報の埋め込み JS に置換する
const inject =
  `window.__SBOP2_ASSET_HASHES__={"sbocli-title.wasm":"${e.WASM_HASH}","sbocli-title.data":"${e.DATA_HASH}"};` +
  `window.SBOP2_BUILD={display:"${e.DISPLAY_VERSION}",build:"${e.BUILD_STAMP}"};` +
  `window.__SBOP2_DATA_SIZE__=${e.DATA_SIZE};`;
const placeholder = /<meta\s+name\s*=\s*["']?sbop2-build-inject["']?\s*\/?>/;
if (!placeholder.test(html)) {
  console.error('生成HTMLにキャッシュバスター埋め込み用プレースホルダが見つかりませんでした。');
  process.exit(1);
}
html = html.replace(placeholder, () => `<script>${inject}</script>`);
fs.writeFileSync(file, html, 'utf8');
EOF
    echo "[browser-cachebust] embedded asset hashes (js=$js_hash wasm=$wasm_hash data=$data_hash) version=$version build=$build_stamp"
}
embed_cachebust || warn "[browser-cachebust] キャッシュバスター埋め込みに失敗しました"

copy_bgm_assets
precompress_assets
sync_to_webroot
echo "[browser-link] success"
