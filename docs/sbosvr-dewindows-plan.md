# SboSvr 脱Windows 計画・作業メモ

最終更新日: 2026-09-05
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
Step 4: メニュー2項目の行き先を決める           ← GUI分離のみ完了。移管先は要判断
Step 5: WinMain → main、ウィンドウを起動オプション化   ← ヘッドレス達成
────────────────────────────────────────────
S3:     WSAAsyncSelect → select/epoll          ← 本丸、ここで初めて非Windows
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

#### Step 4 の状況（未完了・要判断）

GUI 依存の切り離しだけ先に済ませた。`OnCommandACCOUNT_DELETEALL()` から
`DeleteAllAccount()` を切り出し、`MessageBox` を含まない実処理にした。
もう一方（アップデートファイル更新）は `ReadHashList()` を呼ぶだけで UI 依存が無い。

**移管先は未定。以下は判断が必要。**

Web 管理画面の既存 API を確認したところ、**どちらの機能も存在しない**
（`/api/accounts` は POST の作成のみ。ハッシュリスト再読込のエンドポイントも無し）。

| 項目 | 性質 | 選択肢 |
|---|---|---|
| 全アカウント削除 | **破壊的** | (a) 管理画面に公開（認証＋監査ログ必須）<br>(b) 起動引数化<br>(c) 廃止してDB操作手順で代替 |
| アップデートファイル更新 | 無害 | 管理画面に POST を追加するのが自然 |

全アカウント削除については、認証があるとはいえ HTTP からワンクリックで全消しできる
経路を作るのはリスクが高い。**(b) 起動引数か (c) 廃止を推す。**
本番 VPS が稼働している以上、(a) を選ぶ場合は監査ログと二段確認を必須にすべき。

#### Step 5 で決めること

`SboSvr.ini` の `[Pos]` にウィンドウ位置を保存する作りになっている（`OnClose`）。
ヘッドレス時にどう扱うかを決める必要がある。**ヘッドレス時は読み書きしない**のが
素直だと考えている（既定値でウィンドウを作らないため、保存する値も無い）。
