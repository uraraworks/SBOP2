# Windows依存脱却の確認メモ

最終確認日: 2026-03-16

## 目的

SDL 対応を進めるうえで、`SboCli` がどこまで Windows 依存を外せているかを実装ベースで確認し、次の作業順を明確にする。

## 結論

- 管理 UI の `SboCliAdminMfc.dll` 分離は実装上ほぼ完了している。
- SDL 基盤は `SboCli` に導入済みで、メインループ・描画入口・入力入口の一部は SDL 化されている。
- ただし現状は「SDL を導入しつつ Win32 を併用している状態」であり、Windows 依存脱却は未完了。

## 確認済みの前進

### 1. 管理 UI の本体分離

- `SboCli` は `AdminApi/AdminUiLoader.cpp` で `LoadLibrary/GetProcAddress` による DLL ロードを行う。
- `SboCliAdminMfc/src/Migrated/Wnd/` に 6 本の `.cpp` が存在する。
- `SboCliAdminMfc/src/Migrated/Dialog/` に 37 本の `.cpp` が存在する。
- `SboCliAdminMfc/src/Migrated/AdminWindow.cpp` も DLL 側へ移管済み。
- このため、管理画面の MFC 依存は「本体から隔離する」という観点では大きく前進している。

### 2. SDL 基盤の導入

- `SboCli/src/Platform/SDLApp.*`
- `SboCli/src/Platform/SDLWindow.*`
- `SboCli/src/Platform/SDLInput.*`
- `SboCli/SboCli.vcxproj` には `..\SDL2\include` と `..\SDL2\lib\x86`、`SDL2.lib` / `SDL2main.lib` の設定が追加済み。
- `MainFrame::MainLoop()` は `CSDLApp::Run()` を呼ぶ構成へ変更済み。

### 3. 描画と入力の入口置換

- `MgrDraw::Draw()` は `SDL_Renderer*` を受けるインターフェースへ変更済み。
- `MgrKeyInput::Renew()` は `SDL_GetKeyboardState()` を参照する実装へ変更済み。
- `SboCli.cpp` では `SDL_MAIN_HANDLED` を定義して MFC エントリポイントとの競合を回避している。

## 残っている Windows 依存

### 1. ウィンドウとイベントループ

- `SDLWindow` は `SDL_GetWindowWMInfo()` で `HWND` を取得して公開している。
- `MainFrame::OnSDLInit()` は `SetWindowLongPtr()` で SDL ウィンドウをサブクラス化している。
- `SDLApp::PollWin32Messages()` は `PeekMessage()` / `TranslateMessage()` / `DispatchMessage()` を呼んでいる。
- つまり、イベント処理はまだ SDL ネイティブではなく Win32 メッセージ前提。

### 2. 描画内部

- `MgrDraw.cpp` には多数の `HDC` 利用が残る。
- `Img32` は `CreateDIBSection` と `HDC` を前提とした実装。
- `Window/*` と `Layer/*` にも `HDC` ベース描画が広く残存する。
- 現在の SDL 描画は「最終出力を SDL テクスチャへ転送するラッパ」に近い。

### 3. 入力

- `MgrKeyInput.cpp` は `SDL_GetKeyboardState()` に加えて `GetAsyncKeyState()` を併用している。
- `DInputUtil.cpp` は `dinput8.lib` に依存している。
- SDL 入力へ一本化できていないため、まだ Windows 依存が残る。

### 4. 音声

- `SboCli/src/Lib/DXAudio.*` が残存。
- `AflMusic` は `winmm.lib` と `DirectSound` 系 API に依存。
- `SboCli.vcxproj` でも `xaudio2.lib` がリンク対象に入っている。
- 音声系は SDL 化の外側に残っている。

### 5. ゲーム UI / 補助 UI

- `WindowLOGIN.cpp`, `WindowCHAT.cpp`, `WindowCHARNAME.cpp` などで `CreateWindow()` を直接使用。
- チャット入力欄やログイン入力欄は Win32 コントロール前提。
- `DlgMsgLog`、デバッグウィンドウなどの補助 UI も `HWND` / MFC / Win32 に依存。

## 直近の作業順

1. `MainFrame` まわりの `HWND` 依存を減らし、SDL イベントループを主系にする。
2. `MgrKeyInput` から `GetAsyncKeyState` と `DInputUtil` を外す。
3. `MgrDraw` の内部バッファを `HDC` / `CImg32` から SDL テクスチャ系へ寄せる。
4. `Window/*` の `CreateWindow` 依存を整理し、SDL UI または別抽象へ移す。
5. 音声を `DXAudio` / `AflMusic` から切り離し、非 Windows 依存の経路へ移す。

## 判定

- `Phase 1: 管理 UI 分離` は完了寄り。
- `Phase 2: SDL 基盤導入` は完了。
- `Windows依存脱却` の観点では、`Phase 3` 以降の内部置換が本番で、まだ道半ば。
