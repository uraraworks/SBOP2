# SboCli 管理画面DLL分離 + ゲーム部SDL化 実装計画

## 1. 背景と目的

既存の `docs/web-admin-map-plan.md` では、MFC 依存を段階的に減らし、将来的な SDL クロスプラットフォーム化の下地を作る方針が示されている。
本計画はその上位計画として、以下を優先順で実施する。

1. 最優先: クライアント管理画面（MFC）を `SboCli` 本体から分離し、別 DLL 化する
2. 次優先: `SboCli` のゲーム実行部分（描画・入力・ループ）を SDL ベースへ移行する

## 1.5 現在の確認結果（2026-03-16）

- `Phase 1` は実装上ほぼ完了している
  - `SboCliAdminMfc/src/Migrated/Wnd/` に 6 本の `.cpp`
  - `SboCliAdminMfc/src/Migrated/Dialog/` に 37 本の `.cpp`
  - `AdminWindow.cpp` も DLL 側へ移管済み
  - `SboCli` 側は `AdminApi/AdminUiLoader.cpp` で `LoadLibrary/GetProcAddress` により DLL をロードする構成
- `Phase 2` は導入済み
  - `SboCli/src/Platform/SDLApp.*`
  - `SboCli/src/Platform/SDLWindow.*`
  - `SboCli/src/Platform/SDLInput.*`
  - `MainFrame::MainLoop()` は `CSDLApp::Run()` を使用する構成へ移行済み
  - `IGameLoopHost::OnSDLInit()` は `SDL_Window*` を受ける構成へ整理済み
  - `SDLWindow` は `HWND` をキャッシュしない構成へ整理済み
- `Phase 3` も一部前倒しで着手済み
  - `MgrDraw::Draw()` は `SDL_Renderer*` を受ける形へ変更済み
  - `MgrKeyInput::Renew()` は `SDL_GetKeyboardState()` を参照する構成へ変更済み
  - `GetAsyncKeyState()` フォールバックは除去済み
  - `WindowLOGIN` の修飾キー参照は SDL 化済み
  - `StateProcMAP` の左ボタン押下判定は SDL マウス状態参照へ移行済み
  - `SDLApp` から `MainFrame` へ、マウス移動・左右クリック・キーアップ・フォーカス変化を SDL イベント直送する経路を追加済み
  - `PollWin32Messages()` は SDL 管理済み入力を捨て、それ以外だけを流す役割へ整理済み
  - `WM_QUIT` / `WM_CLOSE` から `SDL_QUIT` への橋渡し helper は共通化済み
- ただし、現時点は SDL 導入 = Windows 依存脱却完了ではない
  - `MainFrame::OnSDLInit()` は `SDL_GetWindowWMInfo()` で `HWND` を取得している
  - `PeekMessage` / `DispatchMessage` / `SetWindowLongPtr` など Win32 メッセージ依存が残存
  - 描画内部では `CImg32` / `HDC` / GDI ベースの処理が大量に残存
  - 入力では DirectInput のジョイパッド経路が残存
  - サウンドでは `DXAudio` / `AflMusic` / `winmm` / `DirectSound` 依存が残存

詳細な棚卸しは `docs/windows-dependency-status.md` を参照。

## 2. 現状整理（SboCli）

- `SboCli` は MFC 静的リンク構成（`UseOfMfc=Static`）
- 管理画面系は主に以下に集中
  - `SboCli/src/AdminWindow.*`
  - `SboCli/src/Dialog/*`
  - `SboCli/src/Wnd/*`（管理系カスタムウィンドウ）
- ゲーム実行系は以下に分散
  - `SboCli/src/MainFrame/*`
  - `SboCli/src/StateProc/*`
  - `SboCli/src/MgrDraw.*`, `SboCli/src/MgrKeyInput.*`, `SboCli/src/MgrWindow.*`
- `CStateProcMAP` が `CAdminWindow` に直接依存し、`WM_ADMINMSG` / `WM_MAINFRAME` 経由で密結合

## 3. 目標アーキテクチャ

- `SboCli.exe`
  - ゲームコア（ネットワーク、状態遷移、ロジック）
  - SDL プラットフォーム層（Window / Render / Input / Timer）
  - 管理画面プラグインローダ
- `SboCliAdminMfc.dll`
  - 既存 MFC 管理 UI（Dialog / Wnd / AdminWindow）
  - `IAdminUiHost` 経由でゲームコアと連携
- 共通境界
  - `SboCli/src/AdminApi/`
  - C ABI + 最小限の POD 構造で DLL 境界を定義

## 4. 非目標

- 管理画面の Web 完全移行
- ゲームロジック / プロトコル仕様の大規模変更
- 初期段階での 64bit 化やマルチプラットフォーム同時対応

## 5. 実装フェーズ

### Phase 0: 依存可視化と境界定義

完了済み。

### Phase 1: 管理画面 MFC の DLL 分離

完了済み。

### Phase 2: SDL 基盤導入

**完了条件**

- SDL ウィンドウ生成、イベントループ、終了処理が動作
- ログインからマップ遷移までの基本操作が可能

**2026-03-16 時点の評価**

- プラットフォーム層の追加は完了
- `MainFrame` のループ移行も完了
- `SDLWindow` 自体は `HWND` を公開しない整理まで完了
- ただし `MainFrame::OnSDLInit()` と `SDLApp::PollWin32Messages()` はなお `SDL_GetWindowWMInfo()` と Win32 メッセージ処理に依存
- よって Phase 2 は「SDL 基盤導入済み、Windows 非依存化は未完」の状態

### Phase 3: 描画・入力の SDL 置換

**完了条件**

- マップ描画、キャラ移動、チャット表示、基本 UI 表示が SDL 経由で動作
- 主要シーンで旧版比較の目視差分が許容範囲

**2026-03-16 時点の評価**

- `MgrDraw::Draw(SDL_Renderer*)` と `MgrKeyInput` の SDL 化は進んでいる
- 入力入口については、以下まで進行済み
  - SDL キーボード状態を主系化
  - SDL マウス押下状態を一部導入
  - `SDL_KEYUP` / `SDL_MOUSEMOTION` / `SDL_MOUSEBUTTONDOWN` / フォーカスイベントの橋渡し
  - `MainFrame` の旧アクティブ監視タイマ撤去
- ただし描画の中核データは依然として `CImg32` / `HDC` / GDI に依存している
- `Window/*` と `Layer/*` には `HDC` ベース描画が広く残存している
- 入力も DirectInput のジョイパッド経路と Win32 補助 UI 依存が残り、置換完了とは言えない
- Phase 3 は「入口を SDL に置換済み、内部実装の置換はこれから」が現状

### Phase 4: 統合・安定化

未着手。

## 6. 技術方針

- DLL 境界は例外 / RTTI / STL コンテナをまたがない
- 共有データは「ID + Plain struct」で受け渡す
- MFC 資産は DLL 内に閉じ込め、`SboCli.exe` から MFC 型を参照しない
- SDL 導入初期は「描画・入力・ループ」限定で、ロジック改変を避ける

## 7. リスクと対策

- `MainFrame` への暗黙依存が多く分離が破綻する
  - Phase 0 の境界整理を維持し、境界外アクセスを増やさない
- MFC DLL 化でリソース参照不整合が起きる
  - リソースハンドル切替を共通化する
- SDL 置換で描画品質・入力遅延が劣化する
  - 旧版比較チェックリストを維持する
- 工程が長期化する
  - フェーズ毎に「動く最小構成」でマイルストーンを固定する

## 7.5 Windows依存脱却の直近優先タスク

1. `MainFrame` / `SDLApp` に残る `HWND` 取得、サブクラス化、`PeekMessage` 依存をさらに縮退する。
2. `MgrKeyInput` の DirectInput 依存を外し、SDL イベントまたは `SDL_GetKeyboardState()` に統一する。
3. `MgrDraw` と `Window/*` / `Layer/*` の `HDC` / `CImg32` / GDI 描画を SDL テクスチャ経路へ段階置換する。
4. `DXAudio` / `AflMusic` / `winmm` / `DirectSound` まわりの Windows 固有音声経路を分離する。
5. `CreateWindow` ベースの入力欄や MFC / Win32 補助 UI を SDL 上の UI または別レイヤへ退避する。

## 8. 受け入れ基準

### A. 管理画面 DLL 分離

- `SboCli.exe` は管理 UI 実装詳細を直接知らない
- 管理 UI は `SboCliAdminMfc.dll` の差し替えで更新可能
- 管理系クラッシュがゲーム本体へ連鎖しない

### B. ゲーム部 SDL 化

- ログイン、マップ移動、チャット、基本操作が SDL 実装で成立
- 既存サーバープロトコルとの互換を維持
- 主要解像度で表示破綻がない

## 9. 成果物

- 実装
  - `SboCliAdminMfc.dll`
  - `SboCli` SDL プラットフォーム層
  - 境界 API ヘッダ / ローダ / 互換レイヤ
- ドキュメント
  - DLL 境界仕様書
  - SDL 移行差分表
  - 運用手順

## 10. 既存計画との整合

- `docs/web-admin-map-plan.md` の Web 管理移行は並行継続
- 本計画により、管理機能の実体を「本体から分離」し、将来的に「MFC DLL -> Web / 別 UI」へ置換しやすい構造を先行して作る
- 優先順は一貫して以下
  1. 管理画面の DLL 分離
  2. ゲーム部の SDL 化
