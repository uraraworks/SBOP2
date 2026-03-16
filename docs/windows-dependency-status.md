# Windows依存状況メモ

最終更新日: 2026-03-16

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

## 残っている Windows 依存

### 1. ウィンドウとイベントループ

- `MainFrame::OnSDLInit()` は `SDL_GetWindowWMInfo()` により `HWND` を取得している。
- `MainFrame` は `SetWindowLongPtr()` により SDL ウィンドウをサブクラス化している。
- `SDLApp::PollWin32Messages()` は `PeekMessage()` / `TranslateMessage()` / `DispatchMessage()` を使っている。
- 独自メッセージ、子ウィンドウ通知、ソケット通知、タイマ通知は依然として Win32 メッセージ経由。

### 2. 描画内部

- `MgrDraw.cpp` には多数の `HDC` 利用が残る。
- `Img32` は `CreateDIBSection` と `HDC` を前提とした実装。
- `Window/*` と `Layer/*` には GDI ベース描画が広く残存。
- 現在の SDL 描画は「最終出力を SDL に寄せた段階」であり、内部バッファまでは置換されていない。

### 3. 入力の残存依存

- DirectInput ベースのジョイパッド経路は残存。
- `DInputUtil.cpp` は `dinput8.lib` 依存のまま。
- Win32 コントロール入力欄やチャット入力欄は別途 Win32 依存を持つ。

### 4. 音声

- `SboCli/src/Lib/DXAudio.*` が残存。
- `AflMusic` は `winmm` と DirectSound 系 API に依存。
- `SboCli.vcxproj` は `xaudio2.lib` をリンクしている。

### 5. 補助 UI / 管理 UI

- `WindowLOGIN.cpp`, `WindowCHAT.cpp`, `WindowCHARNAME.cpp` などは `CreateWindow()` 系の依存が残る。
- `DlgMsgLog`、デバッグウィンドウ、各種管理ダイアログは `HWND` / MFC / Win32 前提。
- これらは SDL メインウィンドウ移行とは別レイヤで整理が必要。

## 直近の優先タスク

1. `MainFrame` / `SDLApp` に残る `HWND` 取得、サブクラス化、`PeekMessage` 依存をさらに縮退する。
2. `MgrKeyInput` の DirectInput 依存を外し、入力経路を SDL に統一する。
3. `MgrDraw` と `Window/*` / `Layer/*` の `HDC` / `CImg32` / GDI 描画を SDL テクスチャ経路へ移す。
4. `CreateWindow` ベースの補助 UI を SDL UI 化または別レイヤへ分離する。
5. `DXAudio` / `AflMusic` / `winmm` / `DirectSound` 依存を切り分ける。

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
