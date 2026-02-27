# Phase1 管理画面DLL化 ステータス

最終更新: 2026-02-28

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
- [x] `IAdminUiHost` に必要最小限 API を段階追加
- [x] DLL 側の `SboCli` 具体型 include を削減
  - 完了: `SendAdminPacket` Host API を追加し、`AdminWindow::OnMapAdd` の送信経路を Host 経由化（フォールバック付き）
  - 完了: `SetAdminNotifyTypes` / `GetMoveNoBlock` / `SetMoveNoBlock` / `SetViewGrid` を Host API に追加し、`AdminWindow` の該当処理を Host 経由化
  - 完了: `CDlgAdminBase` に Host 注入 (`SetHost`) と共通送信 (`SendPacket`) を追加
  - 完了: `CDlgAdminBase` から `CUraraSockTCPSBO` メンバ依存を除去し、送信は Host API 経由のみに統一
  - 完了: `DlgAdmin*.cpp` の送信を `SendPacket` 経由へ集約（`m_pSock->Send` 直接呼び出しを除去）
  - 完了: `DlgAdminMapParts` / `DlgAdminMapShadow` の `m_pSock` メンバ依存と `GetUraraSockTCP()` 初期化を除去
  - 完了: `AdminWindow::OnMapAdd` の `UraraSockTCPSBO` 直接フォールバックを除去（`Host->SendAdminPacket` 呼び出しのみに統一、`#include "UraraSockTCPSBO.h"` を除去）
  - 完了: `CDlgAdminBase::SendPacket` の `GetUraraSockTCP()` 直接フォールバックを除去（`Host->SendAdminPacket` 呼び出しのみに統一、`#include "UraraSockTCPSBO.h"` を除去）

4. Step 4: 高依存ユニットの移管
- [x] Step4-A: `WndMap` の `UraraSockTCPSBO`/`CMgrData` 直接依存を Host API 経由に変換
  - 完了: Host API に `GetMapData` / `GetWndMapSize` / `SetWndMapSize` / `GetSelectMapPartsID` / `SetSelectMapPartsID` の5本を追加
  - 完了: `AdminUiLoader` に対応する Host 実装関数を追加
  - 完了: `WndMap.h` の `m_pSock` を除去し `m_pHost` を追加（`Create()` に `pHost` 引数追加）
  - 完了: `WndMap.cpp` の `m_pSock->Send` を `SendPacket(Host API 経由)` に置換
  - 完了: `WndMap.cpp` の `m_pMgrData->GetSelectMapPartsID/SetSelectMapPartsID` を Host API 経由に置換
  - 完了: `AdminWindow::OnMapWindow` で `m_pHost` を `WndMap::Create()` に渡すよう変更
- [x] Step4-B: `WndMap.cpp` / `AdminWindow.cpp` / Dialog 群の `SboCliAdminMfc` 側への物理移管
  - 完了: `WndMap.cpp` を `SboCliAdminMfc/src/Migrated/Wnd/` へ物理移動（MgrDraw.cpp 依存キャリア化は不要とし AdminUiCompat.cpp シムで対応）
  - 完了: `AdminWindow.cpp` を `SboCliAdminMfc/src/Migrated/` へ物理移動
  - 完了: Dialog 群27ファイルを `SboCliAdminMfc/src/Migrated/Dialog/` へ物理移動
  - 完了: `AdminUiCompat.cpp` にシムを拡張（CMgrDraw/CMgrLayer/CMgrData/CMgrSound/CMgrWindow/CInfoCharCli/CLibInfoCharCli）
  - 完了: 依存キャリアとして StaticGrp/StaticMapObjectGrp/StaticMapParts/StaticMapShadow/Wnd系/LibSboSoundLoader/Packet系を追加
  - 完了: Debug/Release 双方でビルド成功（SboCliAdminMfc.dll + SboCli.exe）
- [ ] 高依存 `Dialog` 群の段階移管（TalkEvent 系など今回対象外のもの）
- [ ] 一時アダプタ層の最小化/撤去

5. Step 5: Phase1完了判定
- [ ] `SboCli` が管理UI本体を直接 `new` しない（AdminWindow の new は SboCli 側で残存。フォールバック除去が必要）
- [x] `AdminWindow` + 主要 `Dialog/Wnd` が DLL 側でビルドされる（Step4-B 完了）
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

## AdminWindow.cpp の残存依存（2026-02-28 時点）
- `MgrData.h`：`m_pMgrData` メンバ経由で参照が残存。DLL 側へ物理移管済みだが AdminUiCompat.cpp の CMgrData シムで対応済み。
- `WndMap.h`：物理移管済み。依存は Host API 経由に変換済み。
- `UraraSockTCPSBO.h`：除去済み（2026-02-26）。
- Dialog 系 include 群：27ファイル物理移管済み（Step4-B 完了）。

## AdminUiCompat.cpp シム状況（2026-02-28 更新）
- `CLibInfoItem::GetTypeName`：実体（`Common/LibInfo/LibInfoItem.cpp`）も `""` を返すだけでシムと動作が同一。置換不要。
- `CLibInfoCharCli::GetPtr(DWORD)`：シムは NULL を返す。呼び出し側（`ListCtrlItemInfo.cpp`）には NULL チェックあり（所持キャラ名が空欄になるだけ）。実体置換には `CLibInfoCharBase` 等の追加が必要で Step4 相当。現状維持で問題なし。
- `CInfoCharCli`：ctor/dtor + 仮想メソッド群をシム提供。DlgAdminCharModify* が一時オブジェクトとして生成する用途のみ。
- `CMgrDraw`：ctor/dtor + DrawMapParts (x2) / DrawMapShadow / DrawIcon をシム提供。WndMap/Static*.cpp から呼ばれる描画は DLL 側では no-op。
- `CMgrLayer`/`CMgrWindow`/`CMgrData`/`CMgrSound`：最小限シム提供（連鎖依存を断ち切るため MgrDraw.cpp/MgrLayer.cpp 等は vcxproj に含めない）。

## SboCli 側元ファイルのビルド対象外状況（2026-02-25 判定）
- `SboCli/src/Wnd/WndSelectGrp.cpp` / `WndSelectItemGrp.cpp` 等：`SboCli.vcxproj` は既に Migrated パスを参照。元ファイルはビルド対象外。ロールバック用途で物理ファイルは保持。Step5 完了時に削除を判断する。

## 次アクション（2026-02-28 時点）
- **Step4-B 完了**。次は Step5 に向けた整理。
- `SboCli` 側の AdminWindow 直接 `new`（フォールバック用）の扱いを判断する（除去するか残すか）。
- TalkEvent 系など今回対象外の Dialog 群の移管タイミングを判断する。
- `AdminUiCompat.cpp` のシム群のうち、実体に置換可能なものを検討する（特に `CInfoCharCli`）。
