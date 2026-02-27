# Phase1 管理画面DLL化 ステータス

最終更新: 2026-02-27（後片付け完了）

## 目的
- 管理画面の実体を `SboCli` 本体から分離し、`SboCliAdminMfc.dll` 経由で起動する。
- `SboCli` 側は DLL 境界呼び出しとフォールバック制御に集中させる。

## 現在の達成項目
- [x] `SboCliAdminMfc` プロジェクトを追加
- [x] `SboCli.exe` 側に `LoadLibrary/GetProcAddress` ベースのロード処理を実装
- [x] `CStateProcMAP` から `CAdminWindow` 直接参照を除去
- [x] `WM_ADMINMSG` / `WM_MAINFRAME` の境界中継を実装
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
- [x] 選定ユニット単位で DLL 側ビルドへ移管
  - 完了: `WndToolbar.cpp` を `SboCliAdminMfc` 側でビルド対象化
  - 完了: `ListCtrlEffectInfo.cpp` を DLL 側へ追加し、`Common/myLib/myString.cpp` 同梱でリンク成立
  - 完了: `ListCtrlItemInfo.cpp` を DLL 側へ追加し、`AdminUiCompat.cpp` で `CLibInfoItem::GetTypeName` / `CLibInfoCharCli::GetPtr` の段階移行シムを導入
  - 完了: 上記3ユニットの `.cpp` 実体を `SboCliAdminMfc/src/Migrated/Wnd/` へ物理移動
  - 完了: `WndSelectGrp.cpp` / `WndSelectItemGrp.cpp` を `SboCliAdminMfc/src/Migrated/Wnd/` へ物理移動し、`Img32.cpp` / `lodepng.cpp` / `MgrGrpData.cpp` を依存キャリアとして DLL 側に追加

3. Step 3: ホスト依存の抽象化
- [x] `IAdminUiHost` に必要最小限 API を段階追加
- [x] DLL 側の `SboCli` 具体型 include を削減
  - 完了: `SendAdminPacket` Host API を追加し、`AdminWindow::OnMapAdd` の送信経路を Host 経由化
  - 完了: `SetAdminNotifyTypes` / `GetMoveNoBlock` / `SetMoveNoBlock` / `SetViewGrid` を Host API に追加
  - 完了: `CDlgAdminBase` に Host 注入 (`SetHost`) と共通送信 (`SendPacket`) を追加
  - 完了: `CDlgAdminBase` から `CUraraSockTCPSBO` メンバ依存を除去し、送信は Host API 経由のみに統一
  - 完了: `DlgAdmin*.cpp` の送信を `SendPacket` 経由へ集約
  - 完了: `DlgAdminMapParts` / `DlgAdminMapShadow` の `m_pSock` メンバ依存を除去
  - 完了: `AdminWindow::OnMapAdd` の `UraraSockTCPSBO` 直接フォールバックを除去
  - 完了: `CDlgAdminBase::SendPacket` の `GetUraraSockTCP()` 直接フォールバックを除去

4. Step 4: 高依存ユニットの移管
- [x] Step4-A: `WndMap` の `UraraSockTCPSBO`/`CMgrData` 直接依存を Host API 経由に変換
  - 完了: Host API に `GetMapData` / `GetWndMapSize` / `SetWndMapSize` / `GetSelectMapPartsID` / `SetSelectMapPartsID` の5本を追加
  - 完了: `WndMap.h` の `m_pSock` を除去し `m_pHost` を追加
  - 完了: `WndMap.cpp` の `m_pSock->Send` を Host API 経由に置換
  - 完了: `AdminWindow::OnMapWindow` で `m_pHost` を `WndMap::Create()` に渡すよう変更
- [x] Step4-B: `WndMap.cpp` / `AdminWindow.cpp` / Dialog 群27本の `SboCliAdminMfc` 側への物理移管
  - 完了: `WndMap.cpp` / `AdminWindow.cpp` / Dialog 27本を `SboCliAdminMfc/src/Migrated/` へ物理移動
  - 完了: `AdminUiCompat.cpp` にシムを拡張（CMgrDraw/CMgrLayer/CMgrData/CMgrWindow/CInfoCharCli/CLibInfoCharCli）
  - 完了: Debug/Release 双方でビルド成功
- [x] TalkEvent 系 Dialog 10本の物理移管（2026-02-27 完了）
  - 移管対象: `DlgAdminTalkSet` / `DlgAdminTalkEventBase` / `DlgAdminTalkEventNONE` / `DlgAdminTalkEventMSG` / `DlgAdminTalkEventMENU` / `DlgAdminTalkEventMENUSet` / `DlgAdminTalkEventPAGE` / `DlgAdminTalkEventSet` / `DlgAdminTalkEventTab` / `DlgAdminTalkEventADDSKILL`
  - 完了: 各 .cpp から不要な `UraraSockTCPSBO.h` include を除去
  - 完了: `SboCliAdminMfc.vcxproj` 参照を `src\Migrated\Dialog\` パスへ更新
  - 完了: `SboCli.vcxproj` 参照を `SboCliAdminMfc\src\Migrated\Dialog\` パスへ更新
  - 完了: Debug/Release 双方でビルド成功

5. Step 5: Phase1完了判定
- [x] `SboCli` が管理UI本体を直接 `new` しない（2026-02-27 完了）
  - `AdminUiModule.cpp` を DLL が直接 `CAdminWindow` を管理する実装に変更
  - `AdminUiLoader.cpp` から `CreateLocalAdminUiInternal` / `DestroyLocalAdminUiInternal` 等のフォールバック機能を除去
  - `AdminWindow.h` の include を `SboCli` 側 `AdminUiLoader.cpp` から除去
  - DLL 未配置時は管理者UI作成失敗（FALSE 返却）、ゲーム本体は継続可能
- [x] `AdminWindow` + 主要 `Dialog/Wnd` が DLL 側でビルドされる（Step4-B 完了）
- [x] DLL未配置時に非管理者プレイ継続可能（DLL ロード失敗 → `Create()` が FALSE を返す → ゲーム継続）
- [x] Debug/Release の両構成でビルド成功

6. Step 6: ロールバック方針運用
- [x] ユニット単位で切り戻せる変更セット運用を文書化
- [x] 変更単位（境界ヘッダ/プロジェクト定義/対象ユニット）のテンプレ化
  - `docs/phase1-admin-dll-rollback-template.md` を追加

## 現時点の整理（2026-02-27）
- Phase1 は **完了**。
  - DLL 側が `CAdminWindow` を直接 new して管理（`AdminUiModule.cpp`）
  - `SboCli` 側はフォールバックなし（`AdminWindow.h` の include を除去済み）
  - TalkEvent 系 Dialog 10本を含む全管理UI関連ファイルが DLL 側でビルド

## AdminUiCompat.cpp シム状況（2026-02-27 後片付け後）
- `CLibInfoItem::GetTypeName`：実体も `""` を返すだけ。シムと同一。置換不要。
- `CLibInfoCharCli::GetPtr(DWORD)`：シムは NULL を返す。呼び出し側に NULL チェックあり。現状維持で問題なし。
- `CInfoCharCli`：ctor/dtor + 仮想メソッド群をシム提供。DlgAdminCharModify* 用途のみ。
- `CMgrDraw`：ctor/dtor + DrawMapParts (x2) / DrawMapShadow / DrawIcon をシム提供。DLL 側では no-op。
- `CMgrLayer`/`CMgrWindow`/`CMgrData`/`CMgrSound`：最小限シム提供（連鎖依存を断ち切るため）。
- `CMgrData::GetInfoTalkEvent`：**シム削除済み**。`DlgAdminCharModify` は `m_pHost->GetInfoTalkEvent` 経由で実体にアクセス。

## AdminUiApi.h のHost API フィールド状況（2026-02-27）
- `CreateLocalAdminUi` / `DestroyLocalAdminUi` / `NotifyLocalAdminUi` / `GetLocalAdminUiWindow`：
  DLL 側が自前で AdminWindow を管理するため不要。`AdminUiLoader.cpp` では NULL をセット。
  ABI 互換のためフィールドは残存。

## SboCli 側元ファイルのビルド対象外状況
- `SboCli/src/Wnd/WndSelectGrp.cpp` 等：元ファイルはロールバック用途で物理保持。Phase1 完了で削除判断可能。

## 後片付け実施内容（2026-02-27）
- `GetInfoTalkEvent` を Host API 化：
  - `AdminUiApi.h` に `GetInfoTalkEvent` フィールドを追加
  - `AdminUiLoader.cpp/h` に `HostGetInfoTalkEvent` を実装（`m_pMgrData->GetInfoTalkEvent()` を呼ぶ実体）
  - `DlgAdminCharModify.cpp` の `m_pMgrData->GetInfoTalkEvent()` を `m_pHost->GetInfoTalkEvent(host->userData)` に置換
  - `AdminUiCompat.cpp` の `CMgrData::GetInfoTalkEvent` シムを削除（これで TalkEvent 編集が実際のデータで動作）
- `SboCli/src/Wnd/WndSelectGrp.cpp` / `WndSelectItemGrp.cpp` の元ファイルを削除（Migrated 側への移行完了）
- `SboCliAdminMfc.vcxproj` の PCH メモリ上限を `/Zm500` → `/Zm1000` に引き上げ（Release ビルドでの PCH 不足対策）
- Debug/Release 双方のビルド成功確認

## 次アクション（Phase2 候補）
- Phase1 後片付け完了。次の作業を選択：
  - Web管理画面 Phase 2（マップオブジェクト管理の Web 化）
  - `AdminUiApi.h` の不要フィールド（`CreateLocalAdminUi` 等）の ABI 整理
