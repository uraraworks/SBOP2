# クラウド開発・ステージング環境 計画書

最終更新日: 2026-09-24

## 目的

Claude Code のクラウドセッション（GitHub 連携・Linux コンテナ）で SBOP2 の作業を完結できるようにし、
さらにブラウザから触れるステージング環境を用意する。

- クラウドセッションで「修正 → ビルド → テスト → 動作確認（スクショ）→ ブランチに push」まで回す
- 手元での確認は「ブランチを取得してビルド・実行」または「ステージングをブラウザで触る」だけにする

関連: `docs/sbosvr-dewindows-plan.md`（SboSvr の脱Windows 移植。移植自体は完了済みで、残りは非Windows での実リンクと実行）

## 前提

- クラウドセッションは Linux コンテナ。外から内への接続口は無い（外向き通信のみ・許可ドメインのみ）ため、
  クラウドで立てたサーバーに手元ブラウザから直接繋ぐことはできない。確認はクラウド内のヘッドレスブラウザで行う。
- クラウドセッションはこの PC のメモリ（`~/.claude/.../memory`）と `CLAUDE.local.md` を読めない。
  必要な知見はリポジトリ内（`docs/`・`AGENTS.md`・`CLAUDE.md`）に置く。
- 本番 VPS は Windows Server。ステージングは別の Linux ホスト（無料枠）に置く方針。Oracle Cloud はアカウント作成が審査で通らなかったため、Google Cloud の無料枠（e2-micro）にした（2026-09-25）。
  本番 VPS 同居案は、Windows バイナリを作る仕組み（Actions の Windows ランナー）と本番への影響リスクがあるため見送り。
- この PC には Linux ツールチェーン（WSL ディストリ・cmake・g++）が無いが、クラウドセッション自体が Linux ビルド環境になるため、
  手元に WSL を入れなくても S2 以降を進められる。

## 調査結果（2026-09-24）

### SboSvr / SboSvrTest

- 外部ライブラリ
  - sqlite: 同梱ソース `SboSvr/third_party/sqlite/sqlite3.c` を C としてコンパイルするだけ。
  - zlib: 同梱はビルド済み `zlib.lib` と v1.2.2 ヘッダのみ。Linux ではシステムの `-lz`（`zlib1g-dev`）を使う。
  - bcrypt / ws2_32 / winmm: `SvrPlatform.cpp` の非Windows 分岐で代替済み（乱数は `/dev/urandom`）。pthread 程度で足りる。
  - `StdAfx.cpp` の `#pragma comment(lib, ...)` は Linux では効かない。myLib・SboSockLib は CMake で明示リンクする。
- ソース一覧の正は各 `.vcxproj` の `ClCompile`（SboSvr 約185 / SboSockLib 約205 / SboSvrTest 約48 / myLib）。
  `tools/test-sbosvr-portability.ps1` の `$PortableFiles` は回帰チェック用の約40本なので一覧には使えない。
- 差し替えは `MainFrameWindow.cpp` → `MainFrameWindowNone.cpp` の1組だけ（今は `SboSvr.vcxproj` にしか表現が無い）。
- SboSockTestClient は独立 exe ではなく SboSvrTest に組み込まれたソース。
- PCH: `#include "StdAfx.h"` が約338箇所あり、StdAfx.h はプロジェクトごとに別物。
  CMake ではターゲットごとに「そのプロジェクトのディレクトリを include の先頭」に置くこと。
- `SboSvrTest/StdAfx.h` は `#ifdef _WIN32` 分岐が無く Windows 専用のまま → 要修正。
- CMake の Debug 構成は `_DEBUG` を自動定義しない。Debug 相当が欲しい時は明示する。

### 実行時データ

- git 管理外: `SboData.db`・`SboSvr.ini`（`Release/` ごと ignore）、`Release/BGM` の8本。
- git 管理下: `Release/SBODATA/*.dat` の10本、`SboSvr/webroot`、`SboCli/font/subset`。
- 空 DB は起動時に自動生成される（`SboSvr/src/MgrData.cpp` の `CREATE TABLE IF NOT EXISTS sbo_data`）。
  該当データが DB に無いと `.dat` から読む（`SboSvr/src/SaveLoad/SaveLoadInfoBase.cpp`）。
  **ただし旧 `.dat` は CP932 で、過去に名前・地名が文字化けした経路。** ステージング/検証用データは
  「本番 DB から個人情報を除いたコピー」を基本にする。
- ポート: ini `[Setting]` の `Port`（既定2006）・`HttpPort`（既定18080）、WebSocket は HttpPort+1。

### Linux で直す必要がある箇所

- `SboSvr/src/Web/HttpServer.cpp` の `CHttpServer::ResolveWebRootPath()` が非Windows で常に false
  → 静的ファイル（ブラウザ版・管理画面）が配信されない。
- 同ファイルで静的ファイルのパスを `webRoot + L"\\account"` のようにバックスラッシュで組み立てている箇所がある。
  `Common/Platform/PlatformPath` 側の区切り処理に寄せる。
- `SboSvrTest/StdAfx.h` の非Windows 分岐。

### デバッグ API の安全性

- `/api/debug/fixture` は `#ifdef _DEBUG` の時だけ登録（`HttpServer.cpp`）。
- 実行時も loopback 直結のみ許可し、`X-Forwarded-For` / `X-Real-IP` 付きは拒否（`SboSvr/src/Web/DebugFixtureGuard.cpp`）。
- **ステージングは `_DEBUG` なしでビルドすること。** クラウド内の自動確認（S5）は Debug ビルド＋loopback で使ってよい。

### ブラウザ版クライアント

- ビルド本体は `tools/build-sbocli-browser-title.ps1`、コンパイルは `tools/test-sbocli-browser-preflight.ps1`。
  ソース一覧は build スクリプトに直書き（約185〜540行目）。
- コンパイル/リンクフラグ（`-O3 -msimd128 -std=gnu++17 -include StdAfx.h`、`-sUSE_SDL=2 -sUSE_SDL_TTF=2 -lwebsocket` 等）は Linux でもそのまま使える。
  SDL2 は Emscripten port で賄っており、リポジトリ外の `SDL2\` には依存していない。
- 後処理: ハッシュ埋め込み → BGM コピー（`Release/BGM`、git 管理外）→ `precompress.mjs` で .br/.gz（node 必須）→ webroot 同期。
- Windows 前提: `C:\emsdk` と `em++.bat` の探索、`node.exe` 探索、em++ 不在時の cl.exe フォールバック、`.d` のパス区切り変換。
- 接続先は `?server=host:port`（shell.html → `window.sbop2ServerAddr/Port` → `SboCli/src/MgrData.cpp`）。
- 自動操作は `docs/debug-automation.md`（`/debug?server=...` → fixture → `window.sbop2Debug` で MAP まで約13秒）。

## 段階計画

| 段 | 内容 | 作業場所 |
|---|---|---|
| S0 | メモリ・`CLAUDE.local.md` の知見のうち、クラウドでも要る落とし穴を `docs/`・`AGENTS.md` へ移す | 手元 |
| S1 | ソース一覧の一本化。vcxproj から共有リストを生成し、ズレを検出する仕組みを入れる | 手元 |
| S2 | SboSvr の CMake 化と Linux ビルド（webroot 解決・パス区切り修正込み）。空 DB で起動し HTTP 応答まで確認 | クラウド |
| S3 | SboSvrTest の Linux 対応と GitHub Actions（ubuntu）CI | クラウド |
| S4 | ブラウザ版ビルドの bash 版（S1 の共有リストを使う。BGM 不在でも止まらない） | クラウド |
| S5 | クラウド内でサーバー＋ブラウザ版を起動し、Playwright＋fixture で MAP まで入ってスクショ | クラウド |
| S6 | 無料枠 Linux にステージング（Docker＋Caddy で TLS/wss/Basic 認証、Actions で自動デプロイ、個人情報を除いた本番 DB コピー） | 両方 |

区切り: S2 で「クラウドでビルドできる」、S5 で「クラウドで確かめられる」、S6 で「スマホから触れる」。

## クラウドセッションの運用

- クラウドは作業ブランチに push する。手元では `git fetch origin` → `git switch <branch>` → 関連プロジェクトのみビルドして確認。
- 手元で確認する時の注意: webroot 修正は `SboSvr/Debug/webroot` へ同期しないと反映されない／
  ブランチ切り替え後に古い obj が残ることがあるので、変更 .cpp を touch してビルドし直す。
- クラウド環境のセットアップ（apt: cmake, g++, zlib1g-dev / emsdk / node / Playwright）は S2〜S5 で
  セットアップスクリプトとして整備する。ネットワーク許可ドメインの設定が必要になる可能性あり。

## 作業ログ

### 2026-09-24

- 計画作成。調査結果は上記。
- S0 完了: 落とし穴を `docs/codebase/pitfalls.md` に集約し、`CLAUDE.md` から参照（公開リポジトリのため未修正のセキュリティ課題・本番情報・ローカル環境情報は除外）。
- S1 完了: ブラウザ版のソース一覧を `tools/browser-sources.txt`（314件）に切り出し、ビルド/プリフライト両スクリプトが読む形に。旧配列と完全一致・実ビルド成功を確認。サーバー側は S2 で CMake が `.vcxproj` の `ClCompile` を直接読む方針（一覧を増やさない）。
- S2 完了: ルートの `CMakeLists.txt` で myLib・SboSockLib・SboSvr（同梱 sqlite 含む）を Linux 向けにビルドできるようにした。
  - ソース一覧は `cmake/VcxprojSources.cmake` が各 `.vcxproj` の `ClCompile` を直接読む（Release|Win32 の `ExcludedFromBuild` に従うのでデバッグ専用ファイルは入らない。大文字小文字の違いは自動で吸収）。
  - Linux 側だけの差し替え: `MainFrameWindow.cpp` → `MainFrameWindowNone.cpp`、`Common/Platform/SjisConvert.cpp` を追加、myLib から SDL2 依存の `myThread.cpp`（SboSvr 未使用）を除外。
  - `_DEBUG` は付けない（全ファイル `-DNDEBUG` のみ。`/api/debug/fixture` はバイナリに含まれない）。
  - webroot の解決（`ResolveWebRootPath` / `ResolveBrowserGamePath`）とパス連結を実行環境の区切り文字に寄せ、`StaticFileHandler` の非Windows 版ファイル読み込みを実装。あわせて `select` の第1引数・ソケットタイムアウト・`socklen_t` など、HTTP が Linux で応答しなかった原因を修正。
  - 確認: 空の DB で `--headless` 起動 → `/health` が `{"status":"ok","onlinePlayers":0}`、`/admin/`・`/account/` が webroot のファイルをそのまま返す（304 再検証・`..` 拒否も確認）→ `--stop` で DB 保存して正常終了。
  - 手順: `cmake -S . -B out/cmake-linux -DCMAKE_BUILD_TYPE=Release && cmake --build out/cmake-linux -j` → 実行ファイルと同じ場所に `webroot/`（`SboSvr/webroot` のコピー）と `SBODATA/`（`Release/SBODATA/*.dat`）を置いて `./SboSvr --headless`。必要パッケージは cmake・g++・zlib1g-dev。
  - 残課題（S3 で解消）: Linux のゲーム用 TCP ポート（2006）がスタブで待ち受けなかった。Windows ビルドはこのセッションでは未確認。
- S3 完了（S2 と同じ PR「Linux 対応」にまとめる方針）:
  - `UraraSockTCPSelect` を POSIX 化（`std::thread`・`select` の最大 fd・`socklen_t`・`SO_REUSEADDR`・`MSG_NOSIGNAL`）し、Linux の `CUraraSockTCPSBO` は常に select 版を使う。ゲームポート 2006 が待ち受けるようになった。
  - `SboSvrTest` を CMake に追加（ソース一覧は `.vcxproj` から）。テスト内の Win32 API は `SboSvrTest/TestPlatform.h` に寄せ、Linux で全306件成功。
  - テストで見つかった Linux 固有の差を修正: ワイド書式 `%s` の意味の違いと 4096 文字制限（`CStringCompat.h`）、CP932 変換（iconv）、`StaticFileHandler` の日本語パス、SIGPIPE。
  - `tools/test-sbosvr-linux-smoke.sh`（空 DB で起動 → HTTP・ゲームポート・`--stop` を確認）と `.github/workflows/linux.yml`（ubuntu でビルド・`ctest`・スモークテスト）を追加。
  - 実機確認: 2006 に対してプリチェック → VERSION 往復・zlib 圧縮要求の展開・5本同時接続・不正な応答での切断がすべて期待どおり。`.dat` から入った日本語名（マップ名・アイテム名等）も正しく DB に入る。
- S4 完了（同じ PR「Linux 対応」に追加）:
  - `tools/build-sbocli-browser-title.sh` を追加。ps1 版（build + preflight）と同じフラグ・同じ `tools/browser-sources.txt` で、`.d` による差分コンパイル（並列）→ リンク → ハッシュ埋め込み → BGM コピー → `precompress.mjs` → webroot 同期まで行う。BGM（`Release/BGM`）が無くても警告だけで続行する。
  - 使い方: `source <emsdk>/emsdk_env.sh` → `tools/build-sbocli-browser-title.sh`（`--force` で全再ビルド、`--jobs N`）。出力は ps1 版と同じ `out/browser-title/`。
  - クラウドでは Emscripten ports のアーカイブ取得が塞がれているので、先に `tools/emscripten/prefetch-ports-via-git.sh` を実行する（git clone で取り込む）。
  - Linux 固有の修正は `LayerCloud.cpp` の `#include` の大文字小文字だけ。
  - 確認: emsdk（emcc 6.0.10）で 314 ファイルをコンパイル・リンク成功。ハッシュ・バージョン埋め込み、.br/.gz 生成、2回目は up-to-date でスキップ。Linux 版 SboSvr の `/game/` から html/js/wasm/data が br で配信されることも確認。ブラウザで実際に動かす確認は S5。
- S5 完了（同じ PR「Linux 対応」に追加）:
  - `tools/test-browser-e2e-linux.sh` を追加。空 DB でサーバーを起動し、`out/browser-title` を `webroot/game` に置いて、Playwright（ヘッドレス Chromium・SwiftShader）で `/api/debug/fixture` → `/debug` → `sbop2Debug` でログイン → MAP → ESC メニューまで進め、`out/e2e/map.png`・`map-menu.png` を撮る。ブラウザのコンソールと 4xx の URL は `out/e2e/console.log` に残る。
  - fixture は `_DEBUG` 限定なので、CMake に `-DSBO_DEBUG_API=ON`（既定 OFF）を追加した。ON の時だけ `_DEBUG` を付け、`.vcxproj` の Debug 構成のソース一覧（DebugFixture 系入り）を使う。**ステージング・本番のビルドには使わない**（ビルドディレクトリも `out/cmake-linux-debugapi` に分ける）。
  - 手順: `cmake -S . -B out/cmake-linux-debugapi -DCMAKE_BUILD_TYPE=Release -DSBO_DEBUG_API=ON && cmake --build out/cmake-linux-debugapi -j --target SboSvr` → `tools/build-sbocli-browser-title.sh` → `tools/test-browser-e2e-linux.sh`。
  - 確認: 起動から MAP まで約10秒、3回続けて成功。MAP に自キャラ・マップ名・チャット、ESC でシステムメニューが出ることをスクショで確認。404 は git 管理外の BGM（`hisyou.ogg`・`fairytale.ogg`）だけ。

### 2026-09-25

- S6 のリポジトリ側を用意（同じ PR「Linux 対応」に追加）。手順・構成は `deploy/staging/README.md`。
  - `deploy/staging/`: Docker（SboSvr をイメージ内でビルドするのでホストが arm64 でも可）＋Caddy（Let's Encrypt・Basic 認証）。本番の IIS と同じく `/__sbop2bridge__` を 18081 に、それ以外を 18080 に渡す。公開は 80/443 だけ。
  - ブラウザ版には本番と同じ wss 書換シムを `prepare-browser.sh` で入れる（`.br`/`.gz` も作り直して検証）。
  - Basic 認証はサイト全体。ただし iPhone の Safari が WebSocket に資格情報を付けないので、`/__sbop2bridge__` だけ外した（ゲームのログインで守られる）。
  - `sanitize-db.py`: 本番 DB からアカウント・端末トークン・コード・BAN 情報・拒否 MAC・プレイヤーキャラ（所持品・スキル）・画像編集者名と下書きを消し、`--admin` で管理者を1件作る。IP・メールは DB に保存されていない（ログにだけ出る）。
  - `.github/workflows/staging-deploy.yml`: master への push／手動で、ブラウザ版ビルド → シム → 転送 → ホストで `deploy.sh`。リポジトリ変数 `SBOP2_STAGING_HOST` が無い間は何もしない。
  - 確認（クラウド内の Docker）: イメージのビルド、`localhost` の自己署名 TLS 経由で Basic 認証（なし 401／あり 200）、`/` のリダイレクト、br 配信、`load-db.sh` での DB 入れ替え、Playwright で https＋wss 越しに MAP まで入れること、`deploy.sh`、`docker compose stop` で DB を保存して止まることを確認。`sanitize-db.py` は fixture で作ったアカウント2件入りの DB で、アカウント・PC が消え、作った管理者で `/api/auth/admin-login` が通る（消したアカウントは 401）ことを確認。
  - 残り（手元側）: ホストの用意、ドメイン、GitHub の変数・Secret、`staging.env`、本番 DB のコピー。
- ホストを Google Cloud の無料枠（e2-micro、メモリ 1GB）にしたので、SboSvr のイメージは Actions で作って `docker save` で送り、ホストでは `deploy.sh --image` で読み込むだけにした（ホストでの C++ ビルドをやめた）。ホストに置くのは `deploy/staging` だけ。VM の作り方は `deploy/staging/README.md`。
