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
- 本番 VPS は Windows Server。ステージングは別の Linux ホスト（Oracle Cloud Always Free 等の無料枠）に置く方針。
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
