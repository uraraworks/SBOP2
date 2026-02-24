# SboCli 管理画面DLL分離 + ゲーム部SDL化 実装計画

## 1. 背景と目的

既存の `docs/web-admin-map-plan.md` では、MFC依存を段階的に減らし、将来的なSDLクロスプラットフォーム化の下地を作る方針が示されている。
本計画はその上位計画として、以下を優先順で実施する。

1. **最優先**: クライアント管理画面（MFC）を `SboCli` 本体から分離し、別DLL化する
2. **次優先**: `SboCli` のゲーム実行部分（描画・入力・ループ）をSDLベースへ移行する

## 2. 現状整理（SboCli）

- `SboCli` は MFC静的リンク構成（`UseOfMfc=Static`）
- 管理画面系は主に以下に集中
  - `SboCli/src/AdminWindow.*`
  - `SboCli/src/Dialog/*`
  - `SboCli/src/Wnd/*`（管理系カスタムウィンドウ）
- ゲーム実行系は以下に分散
  - `SboCli/src/MainFrame/*`（メイン処理、受信処理）
  - `SboCli/src/StateProc/*`（状態遷移、マップ実行）
  - `SboCli/src/MgrDraw.*`, `SboCli/src/MgrKeyInput.*`, `SboCli/src/MgrWindow.*`
- `CStateProcMAP` が `CAdminWindow` に直接依存し、`WM_ADMINMSG` / `WM_MAINFRAME` 経由で密結合

## 3. 目標アーキテクチャ

- `SboCli.exe`
  - ゲームコア（ネットワーク、状態遷移、ロジック）
  - SDLプラットフォーム層（Window / Render / Input / Timer）
  - 管理画面プラグインローダ
- `SboCliAdminMfc.dll`
  - 既存MFC管理UI（Dialog/Wnd/AdminWindow）
  - `IAdminUiHost`（ホスト側API）経由でゲームコアと連携
- 共通境界
  - `SboCli/src/AdminApi/`（新設）
  - C ABI + 最小限のPOD構造でDLL境界を定義

## 4. 非目標（この計画で実施しない）

- 管理画面のWeb完全移行（`docs/web-admin-map-plan.md` で継続）
- ゲームロジック/プロトコル仕様の大規模変更
- 初期段階での64bit化・マルチプラットフォーム同時対応

## 5. 実装フェーズ

### Phase 0: 依存可視化と境界定義（1〜2週）

**作業**
- 管理UIが参照する `MgrData/MainFrame/StateProc` APIを棚卸し
- DLL境界ヘッダ作成
  - `IAdminUiModule`（Create/Destroy/Show/Hide/Notify）
  - `IAdminUiHost`（SendPacket/QueryState/PostMainFrameEvent など）
- 既存 `WM_ADMINMSG` 定数群を境界イベントとして整理

**完了条件**
- 管理UIとゲームコア間の依存が「境界ヘッダのみ」で説明できる
- 新規境界に対する最小スタブ実装でビルドが通る

### Phase 1: 管理画面MFCのDLL分離（2〜4週）

**作業**
- 新規プロジェクト追加: `SboCliAdminMfc`（Dynamic Library, Win32, MFC Shared推奨）
- `AdminWindow/Dialog/Wnd` と関連リソースをDLL側へ移動
- `SboCli.exe` 側に `LoadLibrary/GetProcAddress` ローダ実装
- `CStateProcMAP` から `CAdminWindow` 直接参照を除去し、`IAdminUiModule` 呼び出しへ置換
- 障害時フォールバック
  - DLLロード失敗時: 管理機能を無効化してゲーム継続

**完了条件**
- 管理者ログイン時にDLL由来の管理画面が起動
- 管理操作（通知、編集、反映）が既存同等で動作
- DLL未配置でも通常プレイ（非管理者）が可能

#### Phase 1 追加計画: 管理UI関連コードの段階的移管（現実解）
`AdminWindow/Dialog/Wnd` を一括で DLL 側へ移すと、`MainFrame/MgrData/MgrDraw/Info*/Packet*` への暗黙依存でリンク破綻しやすいため、Phase1 では以下の順で段階移管する。

1. **Step 1: 境界と所有権の固定（先行実施）**
- ABI 定義の所有を `SboCliAdminMfc/include` に集約し、`SboCli` 側は互換ヘッダのみを保持。
- `SboCli` は `LoadLibrary/GetProcAddress` と `AdminApi` 境界コードのみを保持する。

2. **Step 2: 低依存ユニットの先行移管**
- 依存が比較的浅い `Wnd/*` と一部 `Dialog/*` を選別し、DLL 側プロジェクトでコンパイル可能にする。
- 選別基準:
- `MainFrame` や描画マネージャへ直接依存しない
- `Packet` 依存が限定的
- リソース参照が DLL 側で閉じる
- 各ユニット移管後に `SboCli` から該当 `.cpp/.h` を除外し、単位でビルド確認する。

3. **Step 3: ホスト依存の抽象化**
- 依存が重い箇所（`MainFrame/MgrData/MgrDraw`）を `IAdminUiHost` 経由に寄せる。
- 追加する Host API は必要最小限で段階導入し、都度 DLL 境界仕様を更新する。
- 目標: DLL 側が `SboCli` の具体型を直接 include しない状態を増やす。

4. **Step 4: 高依存ユニットの移管**
- `AdminWindow` 本体と高依存 `Dialog` 群を最後に移管する。
- 必要に応じて一時アダプタ層（薄い中継クラス）を `SboCli` 側に残し、段階的に縮退する。

5. **Step 5: Phase1完了判定の拡張**
- 判定条件を以下に更新する:
- `SboCli` は管理UI本体を直接 `new` しない
- 管理UIの主要コンパイル単位（少なくとも `AdminWindow` + 主要 `Dialog/Wnd`）が DLL 側でビルドされる
- `SboCliAdminMfc.dll` 未配置時に非管理者プレイが継続可能
- Debug/Release の両構成でビルド成功

6. **ロールバック方針（Phase1内）**
- 単位移管で不安定化した場合は、該当ユニットのみ `SboCli` へ戻す（全面ロールバックしない）。
- 変更は「境界ヘッダ」「プロジェクト定義」「対象ユニット」の3点セットで管理し、切り戻し可能性を常に維持する。

### Phase 2: SDL基盤導入（1〜2週）

**作業**
- 新規プラットフォーム層作成
  - `Platform/SDLApp`, `Platform/SDLWindow`, `Platform/SDLInput`
- 既存メインループを抽象化
  - `IGameLoopHost` 経由で `MainFrame` 更新処理を呼ぶ
- キー入力マッピングを `MgrKeyInput` 互換で実装

**完了条件**
- SDLウィンドウ生成、イベントループ、終了処理が動作
- ログイン〜マップ遷移までの基本操作が可能

### Phase 3: 描画・入力のSDL置換（3〜6週）

**作業**
- `MgrDraw` 依存APIを段階的に SDL renderer へ置換
- テキスト・スプライト描画の互換レイヤ実装
- `MgrWindow` のWin32依存部を分離し、ゲームUI側をSDLイベント駆動化
- 音声周辺は既存実装維持（必要最小限の呼び出し調整のみ）

**完了条件**
- マップ描画、キャラ移動、チャット表示、基本UI表示がSDL経由で動作
- 主要シーンで旧版比較の目視差分が許容範囲

### Phase 4: 統合・安定化（2〜3週）

**作業**
- クラッシュダンプ/ログ強化（DLL境界、SDLイベント）
- 長時間プレイ、管理画面同時操作、再接続の回帰試験
- CI/ローカルビルド手順更新（exe + dll 配布構成）

**完了条件**
- 既知のBlockerバグがゼロ
- リリース候補ビルドで管理者/一般ユーザー双方の主要導線が通る

## 6. 技術方針

- DLL境界は例外/RTTI/STLコンテナをまたがない（C ABI優先）
- 共有データは「ID + Plain struct」で受け渡す
- MFC資産はDLL内に閉じ込め、`SboCli.exe` からMFC型を参照しない
- SDL導入初期は「描画・入力・ループ」限定。ロジック改変を避ける

## 7. リスクと対策

- **リスク**: `MainFrame` への暗黙依存が多く分離が破綻
  - **対策**: Phase 0で依存一覧を作成し、境界外アクセスをコンパイルエラー化
- **リスク**: MFC DLL化でリソース参照不整合
  - **対策**: リソースハンドル切替を共通ヘルパ化し、ダイアログ生成時に統一適用
- **リスク**: SDL置換で描画品質・入力遅延が劣化
  - **対策**: 旧版比較チェックリスト（FPS、入力応答、表示崩れ）を運用
- **リスク**: 工程長期化
  - **対策**: フェーズ毎に「動く最小構成」でマイルストーンを固定

## 8. 受け入れ基準（優先事項ベース）

### A. 管理画面DLL分離（最優先）

- `SboCli.exe` は管理UI実装詳細を直接知らない
- 管理UIは `SboCliAdminMfc.dll` の差し替えで更新可能
- 管理系クラッシュがゲーム本体へ連鎖しない（無効化復帰可能）

### B. ゲーム部SDL化

- ログイン、マップ移動、チャット、基本操作がSDL実装で成立
- 既存サーバープロトコルとの互換を維持
- 主要解像度で表示破綻がない

## 9. 成果物

- 実装
  - `SboCliAdminMfc.dll`（新規）
  - `SboCli` SDLプラットフォーム層（新規）
  - 境界APIヘッダ/ローダ/互換レイヤ
- ドキュメント
  - DLL境界仕様書
  - SDL移行差分表（旧API→新API）
  - 運用手順（起動、ログ取得、障害時切り戻し）

## 10. 既存計画との整合

- `docs/web-admin-map-plan.md` のWeb管理移行は並行継続
- 本計画により、管理機能の実体を「本体から分離」し、
  将来的に「MFC DLL → Web/別UI」へ置換しやすい構造を先行して作る
- 優先順は一貫して以下
  1. 管理画面のDLL分離
  2. ゲーム部のSDL化
