# Windows依存状況メモ

最終更新日: 2026-04-04

## 概要

`SboCli` は SDL ベースのメインループへ移行済みだが、Windows 依存が完全になくなった段階ではない。現状は「SDL を入口に据えつつ、Win32 / MFC / GDI / DirectX 系依存を段階的に縮退している途中」と整理するのが正確。

## 結論

- 管理 UI の `SboCliAdminMfc.dll` 分離は実装済み。
- SDL 基盤導入も実装済みで、メインループ、入力入口、描画入口の一部は SDL 主系になっている。
- ただし以下の依存は継続中。
  - `MainFrame` は SDL ウィンドウを Win32 サブクラス化して独自メッセージを受けている。
  - `SDLApp::PollWin32Messages()` は Win32 メッセージループの縮退版として残っている。
  - 描画内部は `CImg32` / `HDC` / GDI ベースが主流。
  - 補助 UI と管理用ツールウィンドウは MFC / Win32 のまま。
  - 音声は `DXAudio` / `AflMusic` / `winmm` / `DirectSound` 系依存が残る。

## 2026-04-01 の追加状況

- browser title build は通過済みで、headless Chrome でもロゴ表示まで確認できた。
- これは「SDL 導入だけでは不十分だが、残課題はかなり具体化できた」ことを意味する。
- 現在の主ボトルネックは以下。
  - `WM_MAINFRAME` / `WM_MGRDRAW` を前提にした状態遷移
  - `PostMessage` と Win32 メッセージキュー前提の遅延通知
  - `HWND` の有無で分岐する初期化順序
  - browser では `emscripten_set_main_loop` 配下で毎フレーム deterministic に処理されないと破綻する箇所

要するに、今後の本丸は「Win32 メッセージ処理からの脱却」である。

## 2026-04-02 の方針合意

- 管理者ダイアログ、各種ツールウィンドウ、補助ネイティブウィンドウは、ひとまず browser 版では使用しない前提で進める。
- browser 版で優先して置き換える対象は、`WindowLOGIN`、`WindowCHAT`、`WindowCHARNAME` などの「入力用 Win32 コントロール依存」である。
- これらは「Win32 コントロールを browser で再現する」のではなく、「自前描画 UI + SDL テキスト入力」へ置き換える方針とする。
- 一般的な SDL アプリの流儀に合わせ、文字入力の入口は `SDL_StartTextInput()` / `SDL_StopTextInput()` / `SDL_TEXTINPUT` / `SDL_TEXTEDITING` / `SDL_KEYDOWN` を使い、入力欄の見た目・カーソル・確定処理はゲーム内 UI 側で持つ。
- つまり、補助ウィンドウは browser 版では切る、ログインとチャットは SDL UI として作り直す、という二本立てで進める。

## 2026-04-02 の追加進捗

- `IGameLoopHost` / `SDLApp` / `MainFrame` に `SDL_KEYDOWN` と `SDL_TEXTINPUT` の橋渡しを追加した。
- `ILoginWindow` に「キー押下」「テキスト入力」「左クリック」の抽象ハンドラを追加し、ログイン UI を Win32 子コントロール前提から少し切り離した。
- `WindowLOGIN` は `__EMSCRIPTEN__` 時に `CreateWindow(Edit/Button)` を使わず、内部状態だけでアカウント欄・パスワード欄・保存チェック・接続ボタンを持つ土台を追加した。
- browser 版 `CMgrWindow::MakeWindowLOGIN()` も、ローカルタイトルモードを含めて `WindowLOGINNull` ではなく `WindowLOGIN` を生成する形にした。
- `WindowCHAT` と `WindowCHARNAME` にも browser 向け SDL テキスト入力の土台を追加し、`CMgrWindow` の browser 分岐でも実ウィンドウを生成するようにした。
- `WindowBase` と `CMgrWindow` に SDL 入力受け口を追加し、アクティブウィンドウへ `SDL_KEYDOWN` / `SDL_TEXTINPUT` / 左クリックを流せる構造へ寄せた。
- まだ「描画が browser 上で最終的に見えること」までは未確認であり、`CImg32` / GDI 系描画の制約は引き続き残る。
- browser タイトル画面では、ロゴフェード後に `WindowLOGIN` が表示されるところまで到達した。
- `WindowLOGIN` の browser 版は、最初の canvas 直接描画では日本語文字化けと位置ずれが強かったため、現在は HTML DOM オーバーレイで入力欄・チェックボックス・接続ボタンを重ねる方式へ切り替えている。
- これにより、ログイン文字列の文字化けは解消し、見た目も native 版へかなり寄せられる状態になった。
- 現時点の残課題は、browser 上の `WindowLOGIN` オーバーレイの縦位置微調整であり、左右位置は概ね合わせ込みできている。

## 実装済みの前進

### 1. 管理 UI 分離

- `SboCli` 側は `AdminApi/AdminUiLoader.cpp` で `LoadLibrary/GetProcAddress` により管理 UI DLL をロードする構成。
- `SboCliAdminMfc/src/Migrated/Wnd/` と `SboCliAdminMfc/src/Migrated/Dialog/` の移管は実装済み。
- `AdminWindow.cpp` も DLL 側へ移管済み。

### 2. SDL 基盤

- `SboCli/src/Platform/SDLApp.*`
- `SboCli/src/Platform/SDLWindow.*`
- `SboCli/src/Platform/SDLInput.*`
- `MainFrame::MainLoop()` は `CSDLApp::Run()` を使用する構成へ移行済み。
- `IGameLoopHost::OnSDLInit()` は `HWND` ではなく `SDL_Window*` を受ける構成へ整理済み。
- `SDLWindow` は `HWND` を保持せず、SDL の窓とレンダラだけを持つラッパーへ整理済み。

### 3. 入力入口の SDL 化

- `MgrKeyInput::Renew()` は `SDL_GetKeyboardState()` を主系として使う構成へ移行済み。
- `GetAsyncKeyState()` フォールバックは除去済み。
- `WindowLOGIN` の `Shift+Tab` 判定は SDL 修飾キー参照へ移行済み。
- `StateProcMAP` の左ボタン押下判定は `SDL_GetMouseState()` 参照へ移行済み。
- `SDLApp` から `MainFrame` へ、以下の SDL イベントが直接橋渡しされる。
  - `SDL_KEYUP`
  - `SDL_MOUSEMOTION`
  - `SDL_MOUSEBUTTONDOWN`
  - `SDL_WINDOWEVENT_FOCUS_GAINED/LOST`
  - `SDL_WINDOWEVENT_RESTORED/MINIMIZED`
- `MainFrame` の旧アクティブ監視タイマは撤去済み。

### 4. Win32 終了経路の整理

- `WM_QUIT` とメインウィンドウ向け `WM_CLOSE` は `SDL_QUIT` へ橋渡しする構成に整理済み。
- `PushSDLQuitEvent()` は `Platform/SDLEventUtil.h` に共通化済み。
- メインフレーム初期化失敗時、ツールチェックタイムアウト時、ログイン拒否時の終了要求は SDL 終了要求へ寄せ済み。
- `MainFrame.h` に残っていた未使用の `OnClose` 宣言は削除済み。

### 5. Win32 ツールウィンドウの切り分け

- `WndSelectMapPartsGrp` / `WndSelectMapShadowGrp` は SDL 移行対象ではなく、MFC ツールウィンドウとして `WM_CLOSE` で自己終了する構造。
- これらは `WM_WNDCLOSE` で親ダイアログへ通知する構成であり、SDL メインウィンドウの終了経路とは分けて考える。
- `WndSelectMapShadowGrp` と `DlgAdminMapShadowEdit` の通知 ID 不整合は修正済み。

### 6. 入力デバイスの SDL 統一

- `DInputUtil` を全プラットフォームで SDL ジョイスティック API に統一した。
- `dinput.h` インクルード、`dinput8.lib` / `dxguid.lib` のリンクを除去した。
- `SetDevice()` から HWND 引数を削除し、呼び出し元（MainFrame / StateProcMAP / MgrKeyInput）も追従した。
- `void*` キャストによる型不整合も解消し、`SDL_Joystick*` を直接保持する構成へ変更した。

## 残っている Windows 依存

### 1. ウィンドウとイベントループ

- `MainFrame::InitNativeMainWindow()` は `SDL_GetWindowWMInfo()` により `HWND` を取得している。サブクラス化は除去済み。
- `SDLApp::PollWin32Messages()` は `PeekMessage()` / `TranslateMessage()` / `DispatchMessage()` を使っている。ただし責務は「SDL 管理入力の破棄」「WM_QUIT/WM_CLOSE の SDL ブリッジ」「MFC 補助 UI の DispatchMessage」に限定済み。
- 独自メッセージ（WM_MAINFRAME / WM_WINDOWMSG / WM_ADMINMSG / WM_MGRDRAW）は全て内部キュー経由に移行済み。Win32 メッセージキューを経由しない。
- ソケット通知はコールバック→内部キュー経由に移行済み。`PollWin32Messages` 経由の直送パスは除去済み。
- `IGameLoopHost` インターフェースから `OnWin32Message` は除去済み。
- `MainFrame` の同期プリミティブは `CRITICAL_SECTION` から `std::mutex` に移行済み。
- `MgrData` の `PostMessage` / `SendMessage` フォールバックは除去済み。`HINSTANCE` 保持も除去。
- `StateProcBase` / `WindowBase` の未使用 `HWND` メンバ（`m_hWndMain` / `m_hWndAdmin`）は除去済み。
- `MainFrame::OnInitEnd()` の `HWND` パラメータは除去済み。
- `MessageBox()` は `SDL_ShowSimpleMessageBox()` に統一済み。

### 2. 描画内部

- `MgrDraw.cpp` の `StretchBlt` は全て `CImg32::BltStretchNearest()` に置換済み。Lock/Unlock/HDC 不要。
- `LockDibTmp()` からも不要な `Lock()` を除去済み。
- ただし `CImg32::Create()` は依然として `CreateDIBSection` を使用（ネイティブ版）。Emscripten 版は `CreateWithoutGdi` で GDI 不使用。
- テキスト描画（`TextOut` / `TextOutW`）は `CImg32::Lock()` で HDC を取得して GDI 経由で行う構造が WindowBase / LayerBase / 各 Window/Layer に約 58 ファイルに渡り残存。SDL_ttf 等の導入で置換可能だが未着手。
- `Img32` のピクセル操作系メソッド（Blt / BltAlpha / FillRect / Rectangle 等）は GDI 非依存で動作。

### 3. 入力の残存依存

- DirectInput ベースのジョイパッド経路は **SDL に統一済み**。
- `DInputUtil` は全プラットフォームで SDL ジョイスティック API を使用する構成に整理済み。`dinput8.lib` / `dxguid.lib` 依存は除去。
- `WindowLOGIN` / `WindowCHAT` / `WindowCHARNAME` の Win32 コントロール依存は除去済み。

#### 2026-04-01 作業メモ

- `SboCli/src/Lib/DInputUtil.*` に SDL ジョイスティック経路を追加し、Windows 以外では DirectInput ではなく SDL ベースで列挙・選択・入力取得できるようにした。
- これにより `MgrKeyInput` と入力設定ウィンドウは既存 API のまま非 Windows ビルドへ寄せられるようになった。
- ただし Windows 版は引き続き DirectInput 主系であり、完全統一は未完。
- `MainFrame` 内部で発行していた `WM_MAINFRAME` は、受信処理ごとの `PostMessage` ではなく内部保留キュー経由で `OnFrame()` 先頭に配送する構造へ一段縮退した。
- これにより `MainFrameRecvProc*` と `TimerProc()` からの主要な自己通知は Win32 メッセージキュー非依存になった。
- `MgrData::PostMainFrameMessage()` / `DispatchMainFrameMessage()` を追加し、`StateProc*`・`WindowLOGIN`・`LibInfoCharCli` からの主要な `WM_MAINFRAME` 発行も `HWND` 直叩きから共通経路へ寄せた。
- `WM_MGRDRAW` も `MgrData::PostMgrDrawMessage()` / `DispatchMgrDrawMessage()` と `MainFrame` 内部キュー経由へ寄せ、フェード開始・終了通知の自己 `PostMessage` を削減した。
- `WM_WINDOWMSG` / `WM_ADMINMSG` についても `MgrData::PostWindowMessage()` / `PostAdminMessage()` を追加し、`Window/*`・`DlgMsgLog`・`StateProcLOGINMENU`・`MainFrameRecvProcADMIN|CHAR|MAP` の主要な発行元を共通経路へ移した。
- この結果、`SDLApp::PollWin32Messages()` で必ずさばく必要がある独自メッセージは、ソケット通知や Win32/MFC 補助 UI 由来のものへかなり絞られてきた。
- 2026-04-04: Windows 版も SDL ジョイスティック主系に統一し、DirectInput コード・`dinput8.lib` 依存を完全除去した。HWND 引数も `SetDevice()` API から削除した。
- さらに browser 方針として、管理者ダイアログ関連は `StateProcMAP` 側で `__EMSCRIPTEN__` 時に生成・通知・通知種別設定を無効化した。現時点では「ブラウザ版は管理 UI なし」で固定し、ネイティブ専用のまま切り分ける。
- `IGameLoopHost::OnWin32Message()` を追加し、`SDLApp::PollWin32Messages()` からメインウィンドウ向けの `WM_TIMER` / `WM_COMMAND` / `WM_MAINFRAME` / `WM_WINDOWMSG` / `WM_ADMINMSG` / `WM_MGRDRAW` / `URARASOCK` 通知を `MainFrame` へ直接配送するようにした。
- これにより `DispatchMessage()` へ残るのは、`WM_CTLCOLORSTATIC` のような同期応答が必要なメッセージや補助 UI 起点のメッセージが中心になり、Win32 サブクラス化の責務をさらに限定できた。
- あわせて `MainFrame::OnSDLInit()` から `SDL_GetWindowWMInfo()` / サブクラス化 / 保存位置復元を `InitNativeMainWindow()` へ切り出し、SDL 主系初期化とネイティブ専用初期化の境界を明示した。
- 旧 `WM_CREATE` / `WM_INITEND` ベースのメインフレーム初期化経路は削除し、初期化は `OnSDLInit() -> OnInitEnd()` の 1 本に統一した。
- ローカルタイトルモードではメインウィンドウのサブクラス化とツールチェックタイマー開始を行わないようにし、ネイティブ専用責務を通常クライアント時に限定した。
- `WM_CTLCOLORSTATIC` に残っていた `IDC_SAVEPASSWORD` の直接処理は `WindowLOGIN::HandleCtlColorStatic()` へ寄せ、`MainFrame` からログインコントロール固有の知識を減らした。
- 2026-04-04(2): ネイティブ版の Win32 メッセージ経路をさらに縮退。
  - `MgrData` の `PostMessage` / `SendMessage` フォールバックを全除去し、`m_pMainFrame` 直接経由に統一した。
  - `IGameLoopHost::OnWin32Message()` 仮想メソッドを除去し、`SDLApp::PollWin32Messages()` からのソケット直送パスも除去した。
  - `MainFrame::OnWin32Message()` は private メソッドに変更（内部キューの `FlushPendingSocketMessages` からのみ呼ばれる）。
  - `MainFrame` の 5 つの `CRITICAL_SECTION` を `std::mutex` + `std::lock_guard` に置換し、ポータビリティを向上。
  - `StateProcBase::m_hWndMain` / `m_hWndAdmin`、`WindowBase::m_hWndMain` を未使用として除去。
  - `MgrData::SetWindowInfo(HINSTANCE, HWND)` を `SetMainWindow(HWND)` に改名し、未使用の `m_hInstance` / `GetInstance()` を除去。
  - `MainFrame::OnInitEnd(HWND)` の HWND パラメータを除去し、`m_hWnd` 直接参照に統一。
  - `MessageBox()` を `SDL_ShowSimpleMessageBox()` に統一。
  - `MgrWindow::SetFocus()` に `#if !defined(__EMSCRIPTEN__)` ガードを追加。
- 2026-04-04(3): 描画と音声の Windows 依存を縮退。
  - `MgrDraw.cpp` の全 `StretchBlt`（15箇所）を `CImg32::BltStretchNearest()` に置換。Lock/Unlock/HDC が不要になった。
  - `LockDibTmp()` から不要になった `Lock()` 呼び出しを除去。
  - `DXAudio.cpp` を XAudio2 実装から SDL_audio 実装に全面書き換え。
  - SE は最大 32 チャンネル同時再生、BGM は専用チャンネルでループ対応。
  - `xaudio2.lib` のリンクを `SboCli.vcxproj` から除去。
  - BGM ファイル読み込みを `CreateFileA`/`ReadFile` から `fopen`/`fread` に変更。
  - メモリ管理を `CoTaskMemAlloc`/`CoTaskMemFree` から `malloc`/`free` に変更。

### 4. 音声

- `DXAudio` は XAudio2 から SDL_audio に全面移行済み。`xaudio2.lib` のリンクは除去済み。
- SE/BGM の再生は SDL_OpenAudioDevice + コールバックベースのミキサーで動作。
- WAV 解析は既存パーサー + SDL_AudioCVT、OGG は stb_vorbis + SDL_AudioCVT で出力フォーマットに変換。
- BGM ファイル読み込みは `fopen`/`fread` に変更（`CreateFileA`/`ReadFile` 依存を除去）。
- メモリ管理は `CoTaskMemAlloc`/`CoTaskMemFree` から `malloc`/`free` に変更。
- SE リソース読み込み（`SboSoundData.dll` からの `LoadResource`/`LockResource`）は依然として Windows 依存。
- `AflMusic`（MIDI 再生）は `winmm` / DirectMusic 依存のまま。`aflMusic.dll` として動的ロード。

### 5. 補助 UI / 管理 UI

- `WindowLOGIN` / `WindowCHAT` / `WindowCHARNAME` の `CreateWindow()` / HWND ベースコントロール依存は除去済み。SDL テキスト入力ベースに移行完了。
- `DlgMsgLog`、デバッグウィンドウ、各種管理ダイアログは `HWND` / MFC / Win32 前提。
- これらは SDL メインウィンドウ移行とは別レイヤで整理が必要。

#### 2026-04-02 時点の整理方針

- `DlgMsgLog`、管理者ダイアログ、デバッグウィンドウ、各種 `Wnd*` ツールは browser 版では無効化または Null 実装で切る。
- `WindowLOGIN`、`WindowCHAT`、`WindowCHARNAME` は browser 版で必要なため、Win32 コントロールをやめて SDL テキスト入力ベースへ移行する。
- 先行対象は `WindowLOGIN`。ここで SDL テキスト入力の土台を作り、その後 `WindowCHAT` と `WindowCHARNAME` に横展開する。
- 2026-04-04: `WindowLOGIN` / `WindowCHAT` / `WindowCHARNAME` とも Win32 コントロール依存は除去完了。デッドコード（HWND / WNDPROC メンバ、WndProc コールバック）も削除済み。

## 直近の優先タスク

1. ~~`WindowLOGIN` を「自前描画 UI + SDL テキスト入力」へ置き換え~~ → 完了
2. `WindowLOGIN` / `WindowCHAT` / `WindowCHARNAME` の browser 側描画を実機で確認し、必要なら `CImg32` / 文字描画まわりの見え方を補強する。
3. browser 版で不要な補助 UI / 管理 UI / デバッグ UI を Null 実装または無効化で切る。
4. ~~`MainFrame` / `SDLApp` に残る `HWND` 取得、サブクラス化、`PeekMessage` 依存をさらに縮退する。~~ → 概ね完了。残りは MFC 補助 UI の `DispatchMessage` のみ。
5. ~~`MgrKeyInput` の DirectInput 依存を外し、入力経路を SDL に統一する。~~ → 完了
6. ~~`MgrDraw` の `HDC` / GDI 描画を SDL 経路へ移す。~~ → StretchBlt 全除去完了。残りはテキスト描画の GDI 依存（SDL_ttf 導入時に対応）。
7. ~~`DXAudio` / `AflMusic` / `winmm` / `DirectSound` 依存を切り分ける。~~ → DXAudio は SDL_audio に移行完了。AflMusic（MIDI）は未着手。
8. `SDLApp::PollWin32Messages()` の残責務を MFC 補助 UI の廃止にあわせて除去する。

## 次セッション開始点

- 最初の実装対象は `WindowLOGIN`。
- ここでは以下を目標にする。
  - アカウント欄とパスワード欄を SDL テキスト入力で扱えるようにする。
  - Tab / Shift+Tab / Enter / Escape の遷移を Win32 コントロールではなく SDL イベントと内部状態で処理する。
  - 「パスワードを記録する」チェックと「接続」ボタンも自前描画 UI として持つ。
  - `WM_CTLCOLORSTATIC`、`CreateWindow()`、子コントロールのサブクラス化に依存しない `WindowLOGIN` を作る。
- この土台ができたら、同じ入力モデルを `WindowCHAT` と `WindowCHARNAME` に適用する。

## 補足

- `Phase 1: 管理 UI 分離` は完了。
- `Phase 2: SDL 基盤導入` は完了。
- `Windows 依存縮退` の観点では以下が完了済み。
  - SDL キーボード状態を主系化
  - SDL マウス状態の一部導入
  - SDL イベントからのマウス / キーアップ / フォーカス橋渡し
  - `SDLWindow` の `HWND` 保持撤去
  - 終了要求の `SDL_QUIT` helper 共通化
- 今後の本丸は `Phase 3`、特に描画内部と補助 UI の置換。
- ただし browser タイトル到達の観点では、その前段として `Phase 3` の入口にある「メッセージ依存の縮退」を優先する。
