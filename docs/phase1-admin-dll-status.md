# Phase1 管理画面DLL化 ステータス

最終更新: 2026-02-25

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
- [x] `Wnd/*` の依存分析（MainFrame/MgrDraw/Packet観点）
- [x] 先行移管候補の選定（低依存順）
- [ ] 選定ユニット単位で DLL 側ビルドへ移管
  - 進行中: `WndToolbar.cpp` を `SboCliAdminMfc` 側でビルド対象化（Debug/Release 成功）
  - 進行中: `ListCtrlEffectInfo.cpp` を DLL 側へ追加し、`Common/myLib/myString.cpp` 同梱でリンク成立（Debug/Release 成功）
  - 進行中: `ListCtrlItemInfo.cpp` を DLL 側へ追加し、`AdminUiCompat.cpp` で `CLibInfoItem::GetTypeName` / `CLibInfoCharCli::GetPtr` の段階移行シムを導入（Debug/Release 成功）
  - 進行中: 上記3ユニットの `.cpp` 実体を `SboCliAdminMfc/src/Migrated/Wnd/` へ物理移動し、`SboCli` / `SboCliAdminMfc` 双方の `.vcxproj` 参照先を更新
  - 完了: `WndSelectGrp.cpp` / `WndSelectItemGrp.cpp` を `SboCliAdminMfc/src/Migrated/Wnd/` へ物理移動し、`Img32.cpp` / `lodepng.cpp` / `MgrGrpData.cpp` を依存キャリアとして DLL 側に追加（Debug/Release 成功）

3. Step 3: ホスト依存の抽象化
- [ ] `IAdminUiHost` に必要最小限 API を段階追加
- [ ] DLL 側の `SboCli` 具体型 include を削減
  - 進行中: `SendAdminPacket` Host API を追加し、`AdminWindow::OnMapAdd` の送信経路を Host 経由化（フォールバック付き）
  - 進行中: `SetAdminNotifyTypes` / `GetMoveNoBlock` / `SetMoveNoBlock` / `SetViewGrid` を Host API に追加し、`AdminWindow` の該当処理を Host 経由化
  - 進行中: `CDlgAdminBase` に Host 注入 (`SetHost`) と共通送信 (`SendPacket`) を追加
  - 進行中: `CDlgAdminBase` から `CUraraSockTCPSBO` メンバ依存を除去し、送信フォールバックを `CMgrData->GetUraraSockTCP()` へ集約
  - 進行中: `DlgAdmin*.cpp` の送信を `SendPacket` 経由へ集約（`m_pSock->Send` 直接呼び出しを除去）
  - 進行中: `DlgAdminMapParts` / `DlgAdminMapShadow` の `m_pSock` メンバ依存と `GetUraraSockTCP()` 初期化を除去

4. Step 4: 高依存ユニットの移管
- [ ] `AdminWindow` 本体の DLL 側ビルド化
- [ ] 高依存 `Dialog` 群の段階移管
- [ ] 一時アダプタ層の最小化/撤去

5. Step 5: Phase1完了判定
- [ ] `SboCli` が管理UI本体を直接 `new` しない
- [ ] `AdminWindow` + 主要 `Dialog/Wnd` が DLL 側でビルドされる
- [ ] DLL未配置時に非管理者プレイ継続可能
- [x] Debug/Release の両構成でビルド成功

6. Step 6: ロールバック方針運用
- [x] ユニット単位で切り戻せる変更セット運用を文書化
- [x] 変更単位（境界ヘッダ/プロジェクト定義/対象ユニット）のテンプレ化
  - `docs/phase1-admin-dll-rollback-template.md` を追加

## 現時点の整理
- 管理UI「呼び出し境界」は DLL 化済み。
- 管理UI「実体コード（AdminWindow/Dialog/Wnd）」の物理移管は段階対応中。
- 一括移管は依存が広く、段階移管（低依存 -> 抽象化 -> 高依存）で進める。
- Step2 依存分析結果は `docs/phase1-admin-dll-step2-dependency-analysis.md` に集約済み。

## AdminUiCompat.cpp シム状況（2026-02-25 分析）
- `CLibInfoItem::GetTypeName`：実体（`Common/LibInfo/LibInfoItem.cpp`）も `""` を返すだけでシムと動作が同一。置換不要。
- `CLibInfoCharCli::GetPtr(DWORD)`：シムは NULL を返す。呼び出し側（`ListCtrlItemInfo.cpp`）には NULL チェックあり（所持キャラ名が空欄になるだけ）。実体置換には `CLibInfoCharBase` 等の追加が必要で Step4 相当。現状維持で問題なし。

## SboCli 側元ファイルのビルド対象外状況（2026-02-25 判定）
- `SboCli/src/Wnd/WndSelectGrp.cpp` / `WndSelectItemGrp.cpp` 等：`SboCli.vcxproj` は既に Migrated パスを参照。元ファイルはビルド対象外。ロールバック用途で物理ファイルは保持。Step5 完了時に削除を判断する。

## 次アクション
- Step3 として `AdminUiLoader` の `GetUraraSockTCP()` フォールバックを Host API 必須化へ寄せるかを決定する。
- Step4 へ向けた `AdminWindow` 本体の DLL 側ビルド化の前準備（依存整理）を開始する。
