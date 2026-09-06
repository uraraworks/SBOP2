# SboSvr 脱Windows 計画・作業メモ

最終更新日: 2026-09-06
ブランチ: `feature/de-windows`

## 目的

`SboSvr` の Windows 依存を段階的に剥がす。

ただし「Linux で動かすこと」自体は主目的ではない。稼働中の VPS は Windows Server であり、
移設の予定は今のところ無い。狙いは移植の過程で得られる以下の副産物である。

1. **テスタビリティ** — 現状ネットワーク層は `HWND` とメッセージキューが無いと 1 行も動かせず、
   単体テストが書けない。ここを剥がすことが最大のリターン。
2. **サニタイザ / valgrind** — clang/gcc の ASan・UBSan・TSan をかけられるようにする。
   生ポインタと `PBYTE` 手動管理が多いコードベースなので効果が大きい。
3. **CI** — GitHub Actions の ubuntu ランナーでビルド検証を回せるようにする。
4. **Docker 化** — DB 全ゼロコピーやサーバー停止時の書き戻しなど、
   現在「手順で気をつける」でカバーしている事故を構造的に潰す。
5. **ヘッドレス化** — GUI 依存を切ると Windows のままでもサービス化が素直になる。
   これは Linux に行かなくても回収できる。

逆に、VPS 費用の削減（月1,000〜2,000円程度）や性能向上は動機として弱い。期待しないこと。

## 前提: クライアント側は既にほぼ完了している

`SboCli` は `UseOfMfc=false` かつ Emscripten ビルド実績あり。
`SboCli/src/Platform/` に約 5,370 行の互換レイヤ（`CStringCompat.h` / `Win32ApiStubs.h` /
`WndProcCompat.h` / `TCharCompat.h`）があり、**サーバーはこれを流用できる**。

残っている MFC は旧管理画面（凍結済み）・`SboLaunch`・`MakeFileList` のみで、
いずれもゲーム本体の動作には無関係。

**注意: MFC ≠ Win32。** `SboSvr` は `UseOfMfc=false`（MFC を一度も使っていない）にもかかわらず、
リポジトリ内で最も深く Win32 に依存している。MFC を消しても Windows 依存は外れない。

クライアント側の状況は `docs/windows-dependency-status.md` にあるが、
2026-04-04 が最終更新で内容は古い。サーバーの話は含まれていない。

## 現状調査（2026-09-04 時点の実測）

| 依存 | 場所 | 深さ |
|---|---|---|
| `WinMain` + ウィンドウ + メッセージポンプ = メインループ | `SboSvr/src/MainFrame/MainFrame.cpp` | 深い |
| **`WSAAsyncSelect` + 隠しウィンドウ + `PostMessage`** がネットワークの中核 | `SboSockLib/UraraSockTCP.cpp`（1,283行） | **最深** |
| `SetTimer` / `WM_TIMER` で時報・定期保存、`timeGetTime` / `GetTickCount` | MainFrame | 中 |
| ATL `CString`（`atlstr.h`） | `Common/myLib/myString.h` + サーバー16ファイル | 広いが浅い |
| `CRITICAL_SECTION` / `_beginthreadex` | `Common/myLib/mySection.h`, `Web/HttpServer.cpp` | 浅い |
| SQLite | `SboSvr/third_party/sqlite`（アマルガメーション） | **移植不要** |

### GUI 依存の実体は小さい

調べた結果、`WndProc` が扱っているものは以下だけだった。

| メッセージ | 中身 | 対応方針 |
|---|---|---|
| `WM_PAINT` | ステータス表示（読み取り専用） | 文字列生成関数に分離すればコンソールにも出せる |
| `WM_TIMER` × 2 | 時報送信 / 定期保存（`MainFrame.cpp:540`） | ロジックは GUI 無関係。`TimerProc` 内の時刻比較へ移す |
| `WM_COMMAND` | **メニュー2項目のみ**（`IDM_ACCOUNT_DELETEALL` / `IDM_UPDATE_RENEW`） | Web 管理画面へ寄せるか起動引数化 |
| `WM_DISCONNECT` | 自前 `PostMessage`（キープアライブ切断） | 受信キューに相乗り |
| `URARASOCK_*` | ソケットイベント | Step 1 で `SetNotifySink` へ |

さらに `TimerProc()` は既に `PeekMessage` の空きループで呼ばれており
（`MainFrame.cpp:145`）、**ゲームループ本体はもうメッセージポンプから独立している**。

## 段階計画

```
Step 1: SetNotifySink 化（+ 受信キュー）        ← 完了 (2c75fce)
Step 2: タイマー2本を TimerProc へ              ← 完了 (e9f0286)
Step 3: ステータス表示を文字列生成に分離        ← 完了
Step 4: メニュー2項目の行き先を決める           ← 完了(全アカウント削除は廃止)
Step 5: WinMain → main、ウィンドウを起動オプション化   ← 完了。ヘッドレス達成
────────────────────────────────────────────
S3:     WSAAsyncSelect → select/epoll          ← 次はここ。本丸、非Windows へ
```

Step 1・2 を終えた時点で、`WndProc` が扱うのは
`WM_CREATE` / `WM_CLOSE` / `WM_DESTROY` / `WM_PAINT` / `WM_COMMAND` の5つだけになった。

Step 1〜5 は**すべて Windows 上で完結**し、各段階で通常どおり動作確認できる。
非 Windows へ踏み出すのは S3 から。

### Step 1: ソケット通知の `SetNotifySink` 化

現状 `MainFrame.cpp:313` でソケットイベントをウィンドウメッセージとして受けている。

```cpp
if ((msg >= URARASOCK_MSGBASE) && (msg < URARASOCK_MSGBASE + WM_URARASOCK_MAX)) {
    case WM_URARASOCK_ADDCLIENT: OnAddClient(...);
    case WM_URARASOCK_DECCLIENT: OnDecClient(...);
    case WM_URARASOCK_RECV:      OnRecv(...);
```

一方 `SboCli` は既にコールバックへ移行済み（`SboCli/src/MainFrame/MainFrame.cpp:902`）。

```cpp
m_pSock->SetNotifySink(&CMainFrame::OnSocketNotifyThunk, this);
```

`SetNotifySink` は共有インターフェース `CUraraSockTCP` の純粋仮想メンバーとして既に存在し、
サーバーが使っていないだけ。**新しい仕組みを作る必要はなく、クライアントと同じ形に揃えるだけ。**

これを最初にやる理由:

- 最小かつ可逆。変更は MainFrame の数十行、ソケット実装は無改造
- 同じリポジトリ内に動いている前例がある
- 単独でコミットでき、途中で止めても壊れない
- 検証が簡単（起動してログイン・移動・チャットができれば OK）
- 以降の全ステップの前提。`WndProc` から通信が消えれば残りは GUI だけになる

#### ⚠️ 最大の注意点: スレッド境界

**コールバックから game logic を直接呼んではいけない。**

現状は `PostMessage` 経由なので、ソケットライブラリのスレッドで起きたイベントが
メッセージキューを通って**自然にメインスレッドへ直列化**されている。
コールバックに変えると、ソケット側スレッドから `OnRecv` が直接走り、
`LibInfoChar` などを別スレッドが同時に触ることになる。

`SboCli` は `PostSocketMessage` で自前キューに積んで回避している。
**サーバーも「キューに積んで、メインループ側で捌く」形にすること。**
直呼びにすると、再現困難な間欠クラッシュという最悪の形でしか症状が出ない。

## 作業環境

master と並行作業するため worktree を使用している。

| 項目 | 内容 |
|---|---|
| worktree | `C:\Users\masakazu\MyProject\SBOP2-dewin` |
| ブランチ | `feature/de-windows`（master `6ba2865` 起点） |
| SDL2 / SDL2_ttf | master へのジャンクション（実体共有、git 管理外のため） |
| ポート | TCP 2017 / HTTP 18090 / WS 18091（master は 2007 / 18080 / 18081） |
| DB | **コピー**（共有厳禁） |

### 環境構築時の落とし穴

1. **`SDL2/` と `SDL2_ttf/` は git 管理外**（追跡ファイル0件）なのに
   `SboCli.vcxproj` が `..\SDL2\include` と相対参照している。
   新しい worktree には存在しないのでジャンクションかコピーが必須。

2. **DB の実体は `SboSvr/Debug/SBODATA/SboData.db`**。
   実行ファイルの隣を見る作り（`MgrData.cpp:171` の `GetModuleFileNameA`）。
   `Release/SBODATA/` には `.db` が無く旧 CP932 の `.dat` だけなので、
   そちらをコピー元にすると空 DB が作られ `.dat` フォールバックで文字化けする。

3. **`BuildProjectReferences=false` は worktree の初回ビルドで使えない。**
   依存プロジェクトの `myLib.lib` が生成されず `LNK1104` でリンクに失敗する。
   あれは成果物が既にある master での再検証用オプション。素の `-Target Build` を使う。

4. DB のコピーはサーバー停止中に行い、コピー後は必ずヘッダ（`SQLite format 3`）を検証する。
   全ゼロ破損はサイズが一致するので気づけない。

5. worktree のサーバーはポート 2017 なので、master の `Release/SboCli.exe`（2007向け）では繋がらない。
   worktree 側の `SboCli` をビルドして使う（`SboCli.ini` は 2017 設定済み）。

### master 側との衝突注意

脱Windows作業が触るのは `SboSockLib/`、`SboSvr/src/MainFrame/`、`Common/myLib/` に集中し、
master 側の日常作業（画像エディタ、Web管理画面、`SboSvr/src/Web/`）とはほぼ重ならない。

唯一のホットスポットが **`Common/myLib/myString.h`**。
ATL `CString` を剥がす作業と master 側の UTF-8 まわりの修正がぶつかりうるので、
触る前に一声かけること。

## S3: WSAAsyncSelect からの脱却（本丸）

### 方針: 既存実装は残し、新実装を並べる

`CUraraSockTCP` は純粋仮想の抽象クラスなので差し替え口が既にある。
20年動いている現行実装は本番の生命線なので消さず、`select` ベースの新実装を
別ファイルに追加し、切り替えて比較できるようにする。新実装が信頼できたら
旧実装を引退させる。

- 新規: `SboSockLib/UraraSockTCPSelect.cpp` に `CUraraSockTCPSelect`
- 選択は `SboSvr.ini` の `[Setting] SockImpl`（既定は従来実装）
- まずサーバーモード（`Host`）から。クライアント（`Connect`）は後回しでも SboSvr は動く

### Windows メッセージが担っている役割（置き換え対象）

| 現行 | 役割 | 新実装での扱い |
|---|---|---|
| `WSAAsyncSelect` | FD_ACCEPT/CONNECT/READ/WRITE/CLOSE の通知 | `select()` の readfds/writefds |
| `SetTimer(KEEPALIVE)` 60秒 | 最終受信から60秒でクローズ | ループ内の経過時間判定 |
| `SetTimer(KEEPALIVE_CLI)` 30秒 | クライアントが空パケット送出 | 同上 |
| `WM_SOCKADDQUE` | 送信キュー投入をワーカースレッドへ渡す | ミューテックス保護のキュー |
| `WM_SOCKCANCELQUE` | 送信キャンセル | 同上 |
| `WM_SOCKPRECHECK` | プリチェック応答の処理 | 同スレッドなので直接呼び出し |
| `WM_INTERNAL_RECV` | 受信完了 → zlib展開 → 通知 | 直接呼び出し |
| `WM_INTERNAL_SEND` | 「キューに入れたので書き込め」の自己通知 | 次の `select` で writefds に入れる |
| `WM_SOCKEVENT + id` | スロット単位のイベント多重化 | fd_set |

**`PostMessage` の FIFO 順序に依存している点に注意。** AddQue → 実送信の順序が
保たれる前提のコードがある。新実装でも順序を壊さないこと。

### プロトコル（変えてはいけない）

- フレーミング: `URARASOCK_PACKETINFO { DWORD dwSize; DWORD dwCRC; }` + 本体
- プリチェック: 接続直後にサーバーが `GetTickCount()` の4バイトを送り、
  クライアントは `(値 & key) * key` を32bitで返す。**この最初のパケットだけ
  CRC を検証しない**（サイズが4かどうかのみ確認）
- CRC-32（RFC1952、初期値0）
- zlib: 128バイト以上で圧縮。先頭 `0xFF` + 元サイズ4バイト + 圧縮データ
- 優先度キュー3本（High/Mid/Low）と `Combine()` による結合送信
- スループット計測 / キュー数 / IPアドレス取得
- 最大接続数 200、ID ベース 100000、受信バッファ 64KB、SO_SNDBUF/RCVBUF 64KB

### 段階

```
S3-1: 要件の洗い出しと設計          ← 完了(本節)
S3-2: select 実装をサーバーモードで追加  ← 完了
S3-3: 新旧両方で同じプロトコル検証を通す  ← 完了
S3-4: 通信層の単体テストを書く      ← 完了。当初の目的を達成
```

### S3-4: 通信層の単体テスト（目的を達成）

**これが脱Windows対応の本来の目的だった。** 既存の WSAAsyncSelect 実装は
ウィンドウが無いと動かせずテストが書けなかったが、select 実装は
ウィンドウに依存しないためテストプロセスから直接起動して検証できる。

```powershell
pwsh tools/invoke-msbuild.ps1 -ProjectOrSolution SboSvrTest\SboSvrTest.vcxproj -Configuration Debug -Platform Win32
.\SboSvrTest\Debug\SboSvrTest.exe        # 引数でテスト名の部分一致フィルタ
```

構成:

- `SboSockLib/SboSockTestClient` — SBO プロトコルを喋る同期クライアント。
  フレーミングと CRC は**自前で組み立てる**（テスト対象と同じコードで検証しても
  意味が無いため）。プレイヤーと同じようにサーバーへ繋ぐ用途にも使える。
- `SboSvrTest` — 外部依存の無い小さな基盤（`TEST` / `CHECK` / `CHECK_EQ`）と
  通信層のテスト13本。待ち受け開始、プリチェックの成否、送受信、zlib の両方向、
  切断、複数接続を検証する。

#### テストが本当に機能するかを故障注入で確認した

`recv()==0` の切断検知を既存実装と同じ握り潰し方に変えると、
切断系2本が落ちて終了コード1になることを確認済み。

**その過程で当初のテストに穴があることが分かった。** テストクライアントの
`Close()` は `SD_BOTH` のため相手に **RST** が届き、`recv()` はエラーを返すので
別経路で検知されてしまう。`ShutdownSend()` で FIN だけ送るテストを足して
`recv()==0` の経路を確実に通すようにした。

最初の故障注入では 13/13 が通ってしまい、「テストが通ったから大丈夫」が
成り立たない例になった。**新しいテストは必ず落ちることを確認すること。**

#### ビルド設定の注意

- `zlibD.lib` が `/MD` でビルドされているため、既定ライブラリ `MSVCRTD` の
  除外（`IgnoreSpecificDefaultLibraries`）が必要。無いと
  `__except_handler4_common` が未解決になる。
- zlib 由来の **VC90 マニフェスト依存**で「サイド バイ サイド構成が正しくありません」
  になるため、SboSvr と同じくリンク後に `mt.exe` でマニフェストを差し替える。
- `Common/GlobalDefine.h` が `map` を前提にしているので、`StdAfx.h` に
  `#include <map>` と `using namespace std;` が要る。

### S3-3 の比較結果

`tools/test-sbosvr-protocol.ps1` を追加した。サーバーを起動した状態で実行する回帰スクリプト。

```powershell
$env:SBO_SOCK_IMPL = "select"   # または "" で既存実装
SboSvr.exe --headless
pwsh tools/test-sbosvr-protocol.ps1 -Port 2017
```

**新旧で結果が完全に一致した（5/5、圧縮後のサイズまで同じ）。**

| 検証 | 既存(WSAAsyncSelect) | select |
|---|---|---|
| VERSIONチェック往復 | OK | OK |
| zlib圧縮した400バイト要求の展開 | OK (圧縮後286バイト) | OK (同じ) |
| ファイルリスト応答の圧縮 | OK (1060バイト) | OK (同じ) |
| 5本同時接続 | OK | OK |
| 切断後の再接続 | OK | OK |

注意点:

- ファイルリスト応答の検証には **`Update\SBOHashList.txt`** が必要
  （`SBODATA` でも実行ファイル直下でもない。`MgrData::ReadHashList()` 参照）。
  無いと応答が6バイトになり、サーバーの圧縮経路を通せず検証にならない。
- スクリプトは日本語を出力するため **BOM 付き** にしてある。
  `tools/` の他スクリプトは BOM 無しだが、BOM 無しの `.ps1` は
  PowerShell 5.1 で文字化けし、偽の失敗を出すため。

### まだ検証していないこと

- 実クライアントでのログイン・移動・チャット（現実的なパケット流量）
- 優先度キューの掃き出し順（外から観測しにくい）
- 200接続の上限付近の挙動
- 長時間の連続稼働

### S3-2 の実装メモ

`SboSockLib/UraraSockTCPSelect.cpp` に `CUraraSockTCPSelect` を追加した。
切り替えは**環境変数** `SBO_SOCK_IMPL=select`（ini ではなく環境変数にしたのは、
`CUraraSockTCPSBO` が SboCli と共有されており ini への依存を持ち込みたくないため）。

```powershell
$env:SBO_SOCK_IMPL = "select"
SboSvr.exe --headless
```

設計:

- メインスレッドからの `Send` / `DeleteClient` / `SendCancel` は **FIFO のコマンドキュー**へ積み、
  select スレッドが毎周まとめて適用する。既存実装が `PostMessage` で行っていた
  スレッド越えと順序保証をそのまま置き換えたもの。
- 送信は優先度ごとにバイト列で保持し、High → Middle → Low の順に掃き出す。
  送信途中で別の優先度へ切り替えるとフレームが壊れるため、途中のバッファを優先する。
- `select` を即座に起こすため**自己宛 UDP ソケット**を使う。Windows の `select` は
  ソケットしか待てないため。POSIX へ移す際は `socketpair` や `eventfd` に置き換えられる。

**既存実装との差異（重要）:**

`recv()` が 0 を返す正常クローズを、明示的に切断として扱っている。
既存の WSAAsyncSelect 版は `FD_CLOSE` イベントで検知しており、`OnFD_READ` の中では
`dwError != 0` の条件で握り潰していた（`recv()==0` はエラー0なので抜けるだけ）。
select 版で同じことをすると「読める」と報告され続けて**空転する**。

現時点ではサーバーモード（`Host`）のみ。`Connect()` は `FALSE` を返す。
SboSvr はサーバーモードしか使わないので実害は無い。

検証: `SBO_SOCK_IMPL=select` でヘッドレス起動し、プリチェックのチャレンジ応答から
VERSION チェックの往復までを2回確認。`--stop` での停止も正常。

## 残っている Windows 依存の棚卸し（2026-09-05 時点）

Step1〜5 と S3 を終えた時点で、残りは以下。

### 判断が要るもの

| 項目 | 状況 |
|---|---|
| **パスワードハッシュ**（`PasswordHash.cpp` / Windows CNG） | **調査済み。互換性の心配はほぼ無い**（下記） |
| ~~サーバー情報の FTP アップロード（WinINet）~~ | **廃止済み。`/api/server` が役目を引き継いだ** |

#### パスワードハッシュは「総当たり」が要らないと分かった

保存形式 `$s1$<反復回数>$<salt>$<hash>`（salt 16 / hash 32）に**必要なパラメータが全部入って
おり**、中身は素の PBKDF2-HMAC-SHA256。CNG 固有の味付けは無い。
したがって**規格に準拠した実装同士は定義上一致する**。

`SboSvrTest/TestPasswordHash.cpp` で公開テストベクタ（P=password / S=salt / dkLen=32、
c=1 / 2 / 4096）との一致を確認済み。**これが通る限り、OpenSSL でも mbedTLS でも
自前実装でも自動的に互換になる。** ベクタは .NET の `Rfc2898DeriveBytes` で独立計算して裏取り済み。

**より現実的なリスクは平文のバイト列。** `PasswordHash` は変換せず生バイトを渡すため、
呼び出し側が `GetUtf8Pointer()` を使わず `(LPCSTR)` 素キャストすると CP932 が渡り、
同じパスワードでも別のハッシュになる。UTF-8 と CP932 で結果が異なることもテストで固定した。

### 機械的な作業（判断は不要）

| 項目 | 規模 | 備考 |
|---|---|---|
| Web 層のソケット | `closesocket` 25 / `recv` 12 ほか | POSIX とほぼ同じ。**同期は既に `std::mutex`**（49箇所）で移植可能。Windows 固有はスレッド生成 `_beginthreadex` 7箇所のみ |
| ATL `CString` / TCHAR | 212行 | SboCli の `CStringCompat.h` / `TCharCompat.h` が流用可能 |
| `OutputDebugString` | 136行 | ログ出力に置換するだけ。最多だが最も簡単 |
| ini 読み書き | 20行 | 自前パーサか設定形式の変更 |
| パス/ディレクトリ | 22行 | `GetModuleFileName` / `CreateDirectory` |
| 時刻 | 31行 | `std::chrono` で置換可 |

### もう問題にならないもの

- **ウィンドウ / GDI** — `--headless` で回避済み。`MainLoopWindow` と `OnPaint` に閉じ込め済みで、
  非Windows ビルドではまるごと `#ifdef` で落とせる
- **通信の中核** — `select` 実装の Win32 固有 API は 57行のみ
- **SQLite** — アマルガメーションなので移植不要

## 移植作業の進め方（2026-09-05〜06 で確立）

### 検証は em++ で行う

`C:\emsdk` の em++（clang / 非Windows ターゲット）でコンパイルが通れば、
Windows 依存が無い証明になる。リンクはしない。

```powershell
pwsh tools/test-sbosvr-portability.ps1
```

移植が進むたびに `$PortableFiles` / `$PortableHeaders` へ足していく。

**ヘッダは「単体 include」で検証すること。** Windows ビルドが通っても
非Windows で通るとは限らない。実際、共有化した互換ヘッダはどれも自前の依存を
include しておらず（`BrowserCompat.h` が先に `<vector>` 等を取り込む前提）、
単体では通らなかった。

### 各ステップは二重に検証する

1. **Windows で回帰なし** — 3プロジェクトのビルド + テスト46本 + プロトコル検証
2. **非Windows で通る** — 移植チェック

### 完了したもの

| 対象 | 置き換え先 |
|---|---|
| 時刻（31行） | `SboPlatform::GetTickMs` / `GetLocalTime` |
| パス・ディレクトリ（22行） | `MakeExeRelativePath` / `MakeDataFilePath` |
| `OutputDebugString`（139行） | `SboPlatform::WriteDebugLine` |
| ini 読み書き（16箇所） | `GetIniInt` / `GetIniString` / `SetIniString` |
| `CmySection` | `std::recursive_mutex` |
| 残る `CRITICAL_SECTION` | `std::mutex`（TextOutput / select実装） |
| 互換ヘッダの共有化 | `Common/Platform/` へ移動 |
| **`SboSvr/StdAfx.h` の条件化** | `SboSvr/src/Platform/SvrCompat.h` |
| サーバーのパス取得3箇所 | `GetModuleFilePath()` → `SboPlatform::GetExeDirectory()`（`5ef24a0`） |

`StdAfx.h` が非Windows で通るようになったのが大きい。**これ以前はサーバーの
ソースを1つも em++ でコンパイルできず、移植を検証する手段が無かった。**

### 踏んだ落とし穴

- **一括置換は自分自身も書き換える。** `OutputDebugStringA` の置換で
  `SvrPlatform.cpp` の実装まで置き換わり、`WriteDebugLine` が自分を呼ぶ
  無限再帰になった。
- **重複実装は移植で初めて見つかることがある。** `WebSocketBridge.cpp` が
  `WebSocketProtocol.cpp` と同じ SHA-1/Base64 を持っていた。片方だけ直すと
  「移植したのに通らない」で混乱する。同じ Windows API が複数ファイルに
  出てきたら、まず重複を疑うこと。
- **エラーは1つ潰すと次が出てくる。** 棚卸しは `-ferror-limit=1` で「最初の
  エラー」しか見ていないため、1件直すと同じファイルで別の依存が顔を出す。
  残数の減り方が鈍っても異常ではない。
- **`CmySection` は再帰可能でなければならない。** `CRITICAL_SECTION` は
  同一スレッドから何度でも入れる仕様で、`CLibInfoBase` が継承している。
  素の `std::mutex` にすると自己デッドロックする。
- **`PlatformDefs.h` の `CRITICAL_SECTION` が `SDL_mutex` を参照していた。**
  前方宣言で回避。実体を触るのは SboCli の `Win32ApiStubs.h` だけ。
- **include の相対パスがずれていても vcxproj のインクルードディレクトリで
  偶然通る。** 正しい相対パスに直すこと。

### 残っているもの（2026-09-06 時点・続報反映）

全 .cpp 317本のうち **311本が em++ で通る**ようになった
（着手時285本 → 前半308本 → 暗号2本＋エントリポイント1本を片付けて311本）。
移植チェック `tools/test-sbosvr-portability.ps1` への登録は33件、
テストは81本。残り6本は以下。

| ファイル | 残っている依存 | 扱い |
|---|---|---|
| `Common\Lib\LayoutHelper.cpp` | `GetClientRect`（GDI） | **意図的に Windows 専用のまま**。ウィンドウ表示用 |
| `Common\Lib\mfc\LogViewCtrl.h` | MFC | 同上 |
| `SboSvr\src\MainFrame\MainFrame.cpp` | `CreateFont` 等（GDI） | 同上。非Windows ビルドでは丸ごと `#ifdef` で落とす想定 |
| `Common\SBOGlobal.cpp` | `GetModuleFileName` | サーバー側の利用者は `5ef24a0` でゼロになった。ファイル自体は SboCli とも共有のためまだ残る。**S2 で `Common/Platform/` へ移す方針決めが要る**（後述） |
| `SboSvr\src\Web\Handlers\MapPartsHandler.cpp` / `SpriteSheetHandler.cpp` | `LoadLibrary` / `FreeLibrary` | `SboGrpData.dll` からの画像リソース読み込み。**master 側の画像エディタ対応が一段落するまで保留**（2026-09-06 判断、後述） |

**残っているのは「意図的に残す3本」と「設計判断が要る3本」だけになった。
機械的に片付く分は無くなった。**

これで当初の「本丸」だった Web 層のスレッド（`_beginthreadex` 7箇所 + `HANDLE` のイベント）に加え、
暗号論的乱数・PBKDF2・エントリポイントも片付いた。確立したパターンは次項を参照。

### スレッド移植で確立したパターン（2026-09-06 後半）

Web層の `_beginthreadex` / `HANDLE` を `std::thread` へ移す作業を通じて確立した対応表。
次にスレッドを移植する人はここを見ればよい。

| Windows API | 置き換え先 | 備考 |
|---|---|---|
| `HANDLE m_hStopEvent`（手動リセット） | `std::atomic<bool>` | 既存コードの `WaitForSingleObject(m_hStopEvent, 0) == WAIT_TIMEOUT` は0秒待ち＝単なるフラグ確認なので意味が完全に一致する。ここが素直に移せたのは幸運だった |
| `HANDLE m_hStartedEvent` + `WaitForSingleObject(.., 5000)` | `std::promise<bool>` / `future.wait_for(5s)` | 起動完了通知 |
| `WaitForSingleObject(h, 0) == WAIT_OBJECT_0`（非ブロッキングの完了判定） | `future.wait_for(0s) == ready` | `HttpServer::PruneClientThreadsLocked` で使用 |
| `_beginthreadex` のスレッド + タイムアウト付き終了待ち | `std::thread` + `std::promise<void>` | **`std::thread` にはタイムアウト付き join が無い。唯一1:1対応できない箇所。** スレッド終了時に `set_value()` する `promise<void>` を持たせ、`future.wait_for(5000ms)` で待ち、間に合えば `join()`、タイムアウトなら `detach()`（従来の `WaitForSingleObject` タイムアウト後＝追跡放棄と同じ結末） |
| `WaitForMultipleObjects(..., TRUE, timeout)` | 締切時刻ベースのループ | **素直にループへ直すと待ち時間が本数倍になる。** 締切時刻を先に決めて「締切までの残り時間」を各待ちに渡すこと。HttpServer は最大32本なので放置すると最悪32倍になっていた |
| `WSAStartup` / `WSACleanup` | `SboPlatform::SocketStartup()` / `SocketCleanup()` | 参照カウント方式なので複数箇所から呼んでも安全 |

### 移植方針の切り分け：「#ifdef を外す」ではなく「Windows専用APIの呼び出し箇所」を減らす

ユーザーから「`#if !defined(_WIN32)` を外していく方がより脱Windowsではないか」という
指摘があり、以下の整理で合意した。

- **(A) 名前だけが Windows 方言のもの**（`ZeroMemory` / `closesocket` / `SetRect` /
  `_vsnprintf` / `timeGetTime` など）は互換ヘッダで吸収してよい。中身は標準C++や
  POSIXと同じで、置き換えても得られるのは見た目だけ。加えて `Common/` は SboCli と
  共有で、ブラウザ版が既に同じ互換レイヤ（約5,370行）の上に乗っているため、
  ここを剥がすと「サーバーの脱Windows」のはずがクライアント全体の大改修に化ける。
- **(B) 設計ごと Windows に縛られているもの**（`WSAAsyncSelect` / `CreateFile` /
  `_beginthreadex` / `bcrypt` / GDI）はシムで誤魔化さず本当に置き換える。誤魔化すと
  「移植できたつもり」になるだけで、テスト可能性もサニタイザも手に入らない
  ＝そもそもの目的（冒頭「目的」節参照）を失う。
- **評価軸は「`#ifdef` の数」ではなく「Windows専用API の呼び出し箇所数」**とする。
  `#ifdef _WIN32` 自体は害ではなく、両OSに実体のある分岐は残ってよい。
- **`.cpp` には `#ifdef` を書かない**という規律は維持する。分岐が避けられない場合は
  `SvrPlatform.cpp` の中だけに閉じ込める。

### サーバー側のパス取得を集約（2026-09-06 続報・`5ef24a0`）

サーバー内でパス取得に `Common/SBOGlobal.cpp` の `GetModuleFilePath()` を
呼んでいた3箇所を `SboPlatform::GetExeDirectory()` へ置き換えた。

**これで `Common/SBOGlobal.cpp` のパス関数（`GetModuleFilePath()` /
`GetModuleIniPath()`）を呼ぶサーバー側のコードはゼロになった。**
残る呼び出しは SboCli 側の5箇所のみ。

検証は実プロトコルで行った。PowerShell で SBO プロトコル（プリチェックの
チャレンジ応答＋CRC32 フレーミング）を喋るクライアントを書き、以下3経路を確認した。

- `REQ_FILELISTCHECK` … 空文字列の MD5 を送って NG が返る＝実ファイルを読めている
- `REQ_FILE` … `SBOHashList.txt` の先頭40バイトを要求し、ディスク上の実ファイルと
  バイト完全一致
- `REQ_FILELIST` … zlib 展開後の件数19件が実ファイルの行数19件と一致

**パスの組み立てを変える変更は「ログに出して目視」で済ませず、実際に
ファイルの中身が返ることまで見ること。**

### `Common/SBOGlobal.cpp` をどう扱うか（2026-09-06 続報）

`GetModuleFilePath()` / `GetModuleIniPath()` は `GetModuleFileName` + shlwapi に
依存していて非Windows で通らない。調べた結果、**SHA-1 のときと同じ「重複実装」の
構図**だった。

- `SboPlatform::GetExeDirectory()` / `GetIniFilePath()` が**まったく同じ仕事**を
  しており、そちらは移植済み。
- クライアントのブラウザ版では `GetModuleFileName` が**ダミーパス
  （`./sbocli.html`）を返すスタブ**で満たされている。つまり Web 版では元々
  戻り値が本物ではない。

**厄介なのは `SBOGlobal.cpp` がサーバーのプロジェクトにも入っている点。**
サーバーが2関数を使わなくなっても、ファイル自体はコンパイルされるので、
非Windows ビルドに到達するには結局どこかで手を入れる必要がある。段階を分けた。

- **S1（完了 `5ef24a0`）** … サーバー側の利用者を減らす。SboSvr 内で完結し
  SboCli に触らないので安全。共有層へ移すときに **SboSvr 側の利用者がゼロ**に
  なっていて移動が安全になる、という下準備でもある。
- **S2（未着手）** … `Common/Platform/` に実体を1つ置き、SboSvr と SboCli の
  両方を乗せ替える。互換ヘッダを共有化した `4b66d35` / `5bab3b8` と同じ流れ。
  **SboCli のネイティブ＋ブラウザ両方のビルド検証が要る**ので、作業が SboSvr の
  外へ広がる。ブラウザ版のダミーパス分岐も共有実装の中へ集約できる。

### 画像ハンドラの `LoadLibrary` は保留（2026-09-06 判断）

`MapPartsHandler.cpp` / `SpriteSheetHandler.cpp` の `LoadLibrary` /
`FreeLibrary` は、**master 側で並行して進んでいる管理画面の画像エディタ対応と
領域が重なるため、あちらが一段落するまで着手しない**。DLL 依存を残すか
res/ の PNG 直読みへ寄せるかは、あちらの結論と揃えるのが自然。

### 次にやること（2026-09-06 続報更新）

暗号2本・エントリポイント・サーバー側パス取得3箇所が片付き、機械的に
片付く分は無くなった。残る2件は他作業の結論待ちで、その次にリンク検証が来る。

1. **S2: `Common/Platform/` への共有**（`Common/SBOGlobal.cpp`）。SboCli を
   巻き込むので、master 側の作業と足並みを見てから着手する。
2. **画像ハンドラ2本の `LoadLibrary`**（`MapPartsHandler` / `SpriteSheetHandler`）。
   master 側の画像エディタ対応の結論待ち。
3. 上記2つが片付けば、あとは Windows 専用として残す3本
   （`LayoutHelper.cpp` / `LogViewCtrl.h` / `MainFrame.cpp`）を `#ifdef` で
   括るだけで**非Windows ビルドの実際のリンクに挑戦できる段階**になる。
   これまでは「コンパイルが通る」までしか確認していないので、
   **次の山はリンクと実行**。

## 作業ログ

### 2026-09-04

- 脱Windows対応のメリット/コストを調査。方針合意。
- worktree `SBOP2-dewin` と `feature/de-windows` を作成。
- SDL2 / SDL2_ttf のジャンクション、DB 3ファイル + ini のコピー、ポート変更を実施。
- `SboSvr` のビルド確認（0 エラー）。`SboGrpData.dll` も生成して配置。
- スモークテスト成功。3ポートすべて待受、`/health` が `{"status":"ok","onlinePlayers":0}` を返し、
  正常終了後も DB は健全、master 側 DB は無変更を確認。

### 2026-09-05

- 実装の入り口を Step 1（`SetNotifySink` 化）に決定。
- 本ドキュメントを作成。
- **Step 1 完了**（`2c75fce`）。`WM_DISCONNECT` の `PostMessage` 17箇所も
  `RequestDisconnect()` へ置き換え、同じキューに載せた。
- **Step 2 完了**（`e002d9f`）。`SetTimer`/`WM_TIMER` を経過時間判定へ移行。

#### Step 1 の検証方法

`OnAddClient()` が空実装のため、単純な TCP 接続では何も観測できない。
SDL クライアントへの `SendKeys` も届かなかった。最終的に以下で決定的に確認した。

1. 通知サンクと `ProcSockNotify()` に `GetCurrentThreadId()` 付きの一時トレースを仕込む
2. プリチェックハンドシェイクと VERSION チェック要求を送る簡易クライアントを PowerShell で書く
3. 接続 / 受信 / 切断がソケットスレッド → メインスレッドへ渡ることをログで確認
4. トレースを除去して再ビルド・再確認

結果は notify がスレッド 5352、proc がスレッド 5104 で、狙いどおり分離されていた。
応答は `VERSIONCHECKRES_OK`。notify から proc までの遅延は 1〜4ms。

プロトコルの要点（簡易クライアントを再度書く場合の参考）:

- フレームは `URARASOCK_PACKETINFO { DWORD dwSize; DWORD dwCRC; }` + 本体
- 接続直後にサーバーが 4 バイトのチャレンジを送る。クライアントは
  `(challenge & URARASOCK_PRECHECK) * URARASOCK_PRECHECK` を 32bit で返す
- プリチェックのパケットだけは CRC が検証されない（サイズが 4 かどうかのみ）
- 以降は CRC-32（RFC1952、初期値 0）が必要
- VERSION チェック要求は `[main=1][sub=1][DWORD VERSIONVAL]` の 6 バイト

#### 判明した副作用

パケット処理に**最大 1ms の遅延**が入る。従来は `PostMessage` が `TimerProc` 末尾の
`MsgWaitForMultipleObjects(..., 1, QS_ALLINPUT)` を即時起床させていたが、
通知がウィンドウメッセージを経由しなくなったため毎回 1ms タイムアウトする。
ゲームサーバーのティック粒度では無視できるが、気になるならイベント起床に変更できる。

#### 新たに判明した落とし穴

6. **改行コードの規約がファイルごとに違う。編集ツールが CR を落とすと全行差分になる。**

   このリポジトリは `core.autocrlf=true` だが、**格納されている改行はファイルごとにバラバラ**。

   | ファイル | リポジトリ内の格納 |
   |---|---|
   | `SboSvr/src/MainFrame/MainFrame.cpp` | **LF** |
   | `SboSvr/src/MainFrame/MainFrame.h` | **CRLF** |

   `sed -i` や一部の編集ツールは CR を落とすため、そのままコミットすると
   全行が差分になる（実際 Step2・Step3 で各1回踏んだ）。master と異なる改行で
   格納するとマージ時に大量の衝突を生むので、必ず元の形に合わせること。

   確認は Git Bash では信用できない（リダイレクトで変換が入り誤った値が出る）。
   PowerShell でバイト単位に数えること:

   ```powershell
   $b = [System.IO.File]::ReadAllBytes($path)
   $lf = 0; $crlf = 0
   for ($i=0; $i -lt $b.Length; $i++) { if ($b[$i] -eq 10) { $lf++; if ($i -gt 0 -and $b[$i-1] -eq 13) { $crlf++ } } }
   ```

   格納状態を変えずにコミットするには `git -c core.autocrlf=false add <file>` を使う。
   コミット後は必ず `git show --stat HEAD` で差分行数が想定どおりか確認すること。

7. **`SboSoundData` は現状ビルドできない。** `.rc` が参照する日本語ファイル名を
   `rc.exe` が解決できず `RC2135` で失敗する（36 件が非 ASCII 名）。
   worktree 固有ではなく元からの問題で、master は古い DLL を使い回しているだけ。
   Step 1・2 とは無関係なので深追いせず、master の既存 DLL を流用した。

8. **MSBuild の `/p:DefineConstants` は C++ には効かない**（C# 用）。
   一時的なマクロはソースに `#define` を書くのが早い。

#### Step 3 の内容と検証

`OnPaint` に直書きされていた4項目（稼動時間 / 接続数 / 処理キャラ数 / 処理マップ数）を
`GetServerStateItem()` へ切り出した。GDI にもウィンドウにも依存しないので、
ヘッドレス化後はこの結果をコンソールやログへ流せばよい。

検証はサーバーウィンドウのキャプチャで行った。ただし `SetForegroundWindow` +
`CopyFromScreen` では前面に出ず別ウィンドウが写るため、**`PrintWindow` を使うこと**。
z オーダーに関係なくウィンドウ自身の内容が取れる。

#### Step 5 の内容（ヘッドレス達成）

`--headless` でウィンドウを作らずに動作する。`WinMain` は `SboSvrMain()` へ薄く
委譲するだけにしたので、非Windows へ移す際はここを `main()` から呼べばよい。

初期化・終了は `InitServer()` / `TermServer()` に切り出し、`OnCreate` / `OnClose` は
ウィンドウ固有の処理だけを行う。ウィンドウ位置はヘッドレス時に読み書きしない
（位置を持たないのに書くと、次にウィンドウ付きで起動したときの位置を壊すため）。

**停止手段は設計上いちばん気を遣った箇所。**

- コンソールシグナル（Ctrl+C）だけでは、コンソールに接続できない起動で止められない。
  強制終了しか手段が無くなり、**停止時の DB 書き戻しが飛ぶ**。
- そこで名前付きイベント `SboSvr_Quit_<Port>` を用意し、`--stop` で立てる。
- 終了判定にはサーバーが稼働中だけ保持するミューテックス `SboSvr_Running_<Port>` を使う。
  **イベントの存在有無で判定してはいけない** — 監視ツール等が一時的にハンドルを
  開いているだけで終了を検知できなくなる（実際に 30 秒待たされる不具合が出た）。
- ミューテックスは**初期化より前に確保する**。後に回すと、二重起動時に既存
  インスタンスと同じポートを一時的に奪ってから競合に気づくことになる。

終了コード: `0`=正常 / `1`=初期化失敗または停止対象なし / `2`=同じポートで起動済み。

```bash
SboSvr.exe --headless    # ヘッドレス起動
SboSvr.exe --stop        # 停止(同じ ini のポートを対象にする)
```

検証結果:

| ケース | 結果 |
|---|---|
| `--stop` | 終了コード0・1秒で停止・DB書き戻しあり |
| 二重起動 | 終了コード2・ログ追加0行（ポートを奪わない）・既存は生存し疎通継続 |
| 対象なしの `--stop` | 終了コード1 |
| ウィンドウあり | 表示・`[Pos]`保存とも従来どおり。`--stop` は効かない |

#### Step 4 の結果

**全アカウント削除は廃止した。** 破壊的な操作を HTTP やメニューから残す利点が無いため。
同等の操作が必要なときはサーバー停止中に DB を直接操作する。
`DeleteAllAccount()` / `OnCommandACCOUNT_DELETEALL()` / `IDM_ACCOUNT_DELETEALL` と、
項目が1つだけだった「アカウント」ポップアップを削除した。

残るメニューは「アップデートファイル更新」のみ。`ReadHashList()` を呼ぶだけで
UI 依存が無いため、そのまま残している。ヘッドレス時はメニューが無いので
この操作は使えない。必要になったら管理画面に POST を足すのが自然。

なお Web 管理画面の既存 API を確認したが、**どちらの機能も存在しなかった**
（`/api/accounts` は POST の作成のみ）。

### 2026-09-06

- **全 .cpp を一括プローブしたら 317本中 285本が既に通っていた。** 1ファイルずつ
  `tools/test-sbosvr-portability.ps1` へ足していく想定だったが、着手前の想定より
  遥かに進んでいたことが分かった。棚卸しを先にやる価値が大きかった。
- 発見のきっかけは速度。em++ は1ファイルずつ起動すると約10秒かかり317本には
  使えないが、`-fsyntax-only` に複数ファイルをまとめて渡すと劇的に速い
  （3ファイルで2.9秒）。この発見が棚卸しを可能にした。棚卸し用スクリプトを
  `tools/scan-sbosvr-portability.ps1` として追加した。
- 実施した4コミット: 小物Win32補完（11ファイル） / ファイルIOのstdio化 /
  WebSocketのSHA-1自前化 / StaticFileHandlerのFILETIME→time_t化。
  **32→17ファイルに減った。**
- **注意点:** StaticFileHandler の ETag は精度が 100ns から秒へ落ちたため値が
  変わる。既にキャッシュを持っているクライアントは**初回だけ 45MB を再取得する**
  （以降は従来どおり304）。実害は無いが、デプロイ後に転送量が跳ねても異常ではない。
- テストは 46本 → 60本に増えた（SHA-1 8本、StaticFileHandler 6本）。いずれも
  故障注入で赤くなることを確認済み。
- ユーザーから方針の指摘（「`#ifdef` を外す方が脱Windowsでは」）を受け、
  上記「移植方針の切り分け」節の整理で合意した。
- **後半: 本丸だった Web層のスレッドを一気に片付けた。** コミット順は以下。

  | コミット | 内容 |
  |---|---|
  | `78e68bd` | AdminWsHub の recv ループスレッドを `std::thread` へ |
  | `d45ae87` | WebSocketBridge のメイン/セッションスレッドを `std::thread` へ。`SboPlatform::SocketStartup()/SocketCleanup()` を新設して `WSAStartup` を追い出した |
  | `9591b5d` | WebSocketBridge に重複していた SHA-1/Base64/ComputeAcceptKey を削除し WebSocketProtocol へ統合（105行削除、`wincrypt.h` と `advapi32.lib` 依存も解消） |
  | `b98cd47` | HttpServer のメイン/クライアントスレッドを `std::thread` へ |
  | `26f7f86` | Web層の細かい依存を除去（`GetModuleFileNameA`→`GetExeDirectory`、`wsprintfA`→`snprintf`、型のためだけの `windows.h` 削除） |
  | `80ab8fb` | `WSAETIMEDOUT` 補完、`GetSystemTime` の UTC 版を SvrPlatform へ新設、`in_addr.S_un` を `ntohl`+シフトへ、MgrData の `GetModuleFileName` を `GetExeDirectory` へ |

  これで当初の「本丸」だった `_beginthreadex` 7箇所 + `HANDLE` のイベントは全て
  `std::thread` 系へ移り、全 .cpp 317本中 **308本**が em++ で通るようになった
  （着手時は285本）。移植中に確立したスレッド移植パターンは上記「スレッド移植で
  確立したパターン」節にまとめた。残り9本と次の優先順は「残っているもの」
  「次にやること」の両節を参照。

- **続報: 暗号2本とエントリポイントも片付け、311本まで進んだ。** コミット順は以下。

  | コミット | 内容 |
  |---|---|
  | `11a5f88` | 暗号論的に安全な乱数生成を `SboPlatform::GenerateRandomBytes()` に集約。`SessionStore` の `CryptGenRandom` と `PasswordHash` の salt 生成（`BCryptGenRandom`）を統合。非Windows は `/dev/urandom`（短く返る可能性に備えループ読み） |
  | `4b651b7` | PBKDF2-HMAC-SHA256 を Windows CNG から自前実装へ置換。`bcrypt.h` / `bcrypt.lib` 依存が消滅 |
  | `e2d46e4` | 非Windows 用の `main()` を追加。`__argc` / `__argv` 依存を解消 |

  移植チェックへの登録は33件、テストは81本（着手時比+21本）に増えた。

  #### パスワードハッシュ移植は後方互換が最優先だった

  本番 DB に既存ハッシュが685件あるため、実装を差し替える前に手順を決めた。

  1. 差し替える**前に** CNG 実装が生成した `$s1$...` 形式のハッシュを4件採取
     （`password123` / 非ASCII / 空文字列 / 記号入り）
  2. それをテストにハードコードし、新実装の `VerifyPassword()` が受理することを
     テストにした
  3. その上で実装を差し替えた

  結果、4件とも新実装で受理。加えて**実際にサーバーを起動し、採取した値で
  アカウントを作って HTTP ログインが通ることまで確認**した（テストだけで
  済ませない）。誤パスワードは 401。

  追加したテストベクタ:

  - SHA-256 単体7本（空 / `"abc"` / **55, 56, 63, 64, 65 バイトの境界** / 長文）
  - HMAC-SHA256 の RFC 4231 ベクタ5本（**鍵がブロック長より長いケース**を含む）
  - CNG 後方互換ベクタ5本（上記の採取分）

  故障注入（SHA-256 の初期定数を1ビット変更）で22本が落ちることを確認済み。

  **性能**: `VerifyPassword` は Release / 反復10万回で
  **CNG版 約166ms → 新実装 約73ms（約2.3倍高速）**。HMAC の鍵吸収を反復ごとに
  繰り返さない標準的な最適化による。**出力は完全に同一**（テストベクタで
  固定済み）。ただし**検証が速くなった＝総当たりの単価も同じだけ下がった**
  ということでもある。保存形式に反復回数が入っているので、**新規ハッシュの
  反復回数を引き上げるかは別途の判断**（既存ハッシュは各自の反復回数で検証
  され続けるので、上げても互換は壊れない）。

  #### エントリポイントの扱い

  `SboSvrMain()` へ委譲する形が既にできていたので、非Windows 用の `main()` を
  足すだけで済んだ。**エントリポイントの選択だけは `#ifdef` が避けられない**
  （言語仕様の問題でプラットフォーム層に隠せない）ため、`SboSvr.cpp` のこの
  1箇所に限って `#ifdef _WIN32` を認めた。`SboSvrMain()` の中身には持ち込んで
  いない。`hInstance` はウィンドウ生成にしか使われていなかったので、
  非Windows では NULL を流すだけで足りる。

  起動4パターンの回帰も確認済み: `--headless` 稼働 / `--stop` 終了コード0 /
  対象なし `--stop` は1 / 二重起動は2 / 引数なしはウィンドウ表示
  （`PrintWindow` でキャプチャして表示内容まで確認）。

  残る6本（意図的に残す3本 + 設計判断が要る3本）は「残っているもの」
  「次にやること」の両節を参照。

- **続報2: サーバーのパス取得3箇所を `SboPlatform::GetExeDirectory()` へ集約**
  （`5ef24a0`）。これでサーバー側で `Common/SBOGlobal.cpp` のパス関数を呼ぶ
  コードはゼロになった（残るは SboCli 側の5箇所のみ）。実プロトコルクライアント
  （PowerShell）で `REQ_FILELISTCHECK` / `REQ_FILE` / `REQ_FILELIST` の3経路を
  叩き、実ファイルの中身が正しく返ることまで確認した。
  `Common/SBOGlobal.cpp` 自体の扱いは S1（今回完了）/ S2（`Common/Platform/`
  への共有、未着手）に段階分けした。画像ハンドラの `LoadLibrary` は master 側の
  画像エディタ対応と領域が重なるため保留と判断。詳細は「サーバー側のパス取得を
  集約」「`Common/SBOGlobal.cpp` をどう扱うか」「画像ハンドラの `LoadLibrary` は
  保留」の各節を参照。
