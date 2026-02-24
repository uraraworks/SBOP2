# Phase1 管理画面DLL化 ステータス

最終更新: 2026-02-24

## 目的
- 管理画面の実体を `SboCli` 本体から分離し、`SboCliAdminMfc.dll` 経由で起動する。
- `SboCli` 側は DLL 境界呼び出しとフォールバック制御に集中させる。

## 現在の達成項目
- [x] `SboCliAdminMfc` プロジェクトを追加
- [x] `SboCli.exe` 側に `LoadLibrary/GetProcAddress` ベースのロード処理を実装
- [x] `CStateProcMAP` から `CAdminWindow` 直接参照を除去
- [x] `WM_ADMINMSG` / `WM_MAINFRAME` の境界中継を実装
- [x] DLLロード失敗時のフォールバック動作を実装
- [x] `SBO.sln` に DLL プロジェクトを追加
- [x] `SboCli.vcxproj` に `SboCliAdminMfc` への依存（ProjectReference）を追加
- [x] Admin UI ABI の実体を `SboCliAdminMfc/include/AdminUiApi.h` へ集約
- [x] `SboCli` 側 `AdminUiApi.h` を互換フォワーダ化

## Phase1 追加計画との同期（段階移管）
1. Step 1: 境界と所有権の固定
- [x] ABI 所有を DLL 側に集約
- [x] `SboCli` 側は境界コード中心の責務に整理

2. Step 2: 低依存ユニットの先行移管
- [ ] `Wnd/*` の依存分析（MainFrame/MgrDraw/Packet観点）
- [ ] 先行移管候補の選定（低依存順）
- [ ] 選定ユニット単位で DLL 側ビルドへ移管

3. Step 3: ホスト依存の抽象化
- [ ] `IAdminUiHost` に必要最小限 API を段階追加
- [ ] DLL 側の `SboCli` 具体型 include を削減

4. Step 4: 高依存ユニットの移管
- [ ] `AdminWindow` 本体の DLL 側ビルド化
- [ ] 高依存 `Dialog` 群の段階移管
- [ ] 一時アダプタ層の最小化/撤去

5. Step 5: Phase1完了判定
- [ ] `SboCli` が管理UI本体を直接 `new` しない
- [ ] `AdminWindow` + 主要 `Dialog/Wnd` が DLL 側でビルドされる
- [ ] DLL未配置時に非管理者プレイ継続可能
- [ ] Debug/Release の両構成でビルド成功

6. Step 6: ロールバック方針運用
- [ ] ユニット単位で切り戻せる変更セット運用を文書化
- [ ] 変更単位（境界ヘッダ/プロジェクト定義/対象ユニット）のテンプレ化

## 現時点の整理
- 管理UI「呼び出し境界」は DLL 化済み。
- 管理UI「実体コード（AdminWindow/Dialog/Wnd）」の物理移管は段階対応中。
- 一括移管は依存が広く、段階移管（低依存 -> 抽象化 -> 高依存）で進める。

## 次アクション
- Step2 の依存分析結果を作成し、先行移管対象を 3〜5 ユニット選定する。
