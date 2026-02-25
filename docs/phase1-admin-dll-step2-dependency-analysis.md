# Phase1 Step2 依存分析（管理UI 低依存ユニット）

最終更新: 2026-02-25

## 目的
- `Wnd/*` および `Dialog/*` の中から Step2 段階移管の対象となる低依存ユニットを特定する。
- `MainFrame` / `MgrDraw` / `Packet` への依存をリスクの主要指標として使用する。

## 調査範囲と方法
- スキャン対象トークン：
  - `MainFrame`
  - `MgrDraw`
  - `Packet`
  - `MgrData`
  - `StateProc`
  - `AdminWindow`
- 対象ファイル：
  - `SboCli/src/Wnd/*.cpp`
  - `SboCli/src/Dialog/*.cpp`

## Wnd 依存スコア一覧
- スコア `0`（依存なし）:
  - `WndToolbar.cpp`
- スコア `1`（`MgrData` のみ）:
  - `ListCtrlEffectInfo.cpp`
  - `ListCtrlItemInfo.cpp`
  - `WndSelectGrp.cpp`
  - `WndSelectItemGrp.cpp`
  - `WndSelectMapPartsGrp.cpp`
  - `WndSelectMapShadowGrp.cpp`
- スコア `2+`（`MgrDraw` または `Packet` を含む）:
  - `StaticGrp.cpp`
  - `StaticMapObjectGrp.cpp`
  - `StaticMapParts.cpp`
  - `StaticMapShadow.cpp`
  - `WndMapPartsAnimeList.cpp`
  - `WndMapPartsGrp.cpp`
  - `WndMapShadowAnimeList.cpp`
  - `WndMapShadowGrp.cpp`
  - `WndMap.cpp`（`Packet` 依存あり）

## Dialog 依存スコア一覧（低依存側）
- スコア `0`（依存なし）:
  - `DlgAdminCharDisableNew.cpp`
  - `DlgAdminItemTypeNewARMSBow.cpp`
- スコア `1`（`MgrData` のみ）:
  - `DlgAdminCharModifyTypeBase.cpp`
  - `DlgAdminCharModifyTypePUTNPC.cpp`
  - `DlgAdminCharMotion.cpp`
  - `DlgAdminCharSkillHEAL.cpp`
  - `DlgAdminCharSkillMOVEATACK.cpp`
  - `DlgAdminCharSkillNONE.cpp`
  - `DlgAdminEfcBalloon.cpp`
  - `DlgAdminEfcEffect.cpp`
  - `DlgAdminItemNew.cpp`
  - `DlgAdminItemTypeNew*.cpp`
  - `DlgAdminMapEvent*.cpp`（イベント詳細ダイアログ）
  - `DlgAdminMapPartsEdit.cpp`
  - `DlgAdminMapShadowEdit.cpp`

## Step2 移管候補（低依存順）
1. `SboCli/src/Wnd/WndToolbar.cpp`
2. `SboCli/src/Wnd/ListCtrlEffectInfo.cpp`
3. `SboCli/src/Wnd/ListCtrlItemInfo.cpp`
4. `SboCli/src/Wnd/WndSelectGrp.cpp`
5. `SboCli/src/Wnd/WndSelectItemGrp.cpp`

## これら5ユニットを選んだ理由
- `MainFrame`、`Packet`、`StateProc`、`AdminWindow` への直接依存がない。
- ユニット単位のロールバックが容易（Step6 のロールバック方針に合致）。
- `WndMap` 系ユニットよりも変更の影響範囲が小さい。

## 実行計画
1. 5ユニットを1つずつ DLL ビルド設定に追加する。
2. 各ユニット追加後に Debug / Release ビルドを検証する。
3. ユニットごとに `SboCli` 側からの除外が安全かを判断し、影響が大きい場合は先送りする。

## Step2 実施結果（2026-02-24〜2026-02-25）
- `WndToolbar.cpp`:
  - `SboCliAdminMfc.vcxproj` に追加し、`Debug|Win32` / `Release|Win32` で検証済み。
- `ListCtrlEffectInfo.cpp`:
  - 依存キャリアとして `Common/myLib/myString.cpp` を同梱し、リンク成立。`Debug|Win32` / `Release|Win32` で検証済み。
- `ListCtrlItemInfo.cpp`:
  - `AdminUiCompat.cpp` に `CLibInfoItem::GetTypeName` / `CLibInfoCharCli::GetPtr` の段階移行シムを実装してリンク成立。`Debug|Win32` / `Release|Win32` で検証済み。
  - シムの動作について（2026-02-25 分析）：
    - `GetTypeName`：実体（`Common/LibInfo/LibInfoItem.cpp`）も `""` を返すだけでシムと同一動作。置換不要。
    - `GetPtr(DWORD)`：シムは NULL を返すが、呼び出し側に NULL チェックあり（所持キャラ名が空欄になるだけ）。実体置換は Step4 相当（`CLibInfoCharBase` 等の追加が必要）。
- 物理移動:
  - `WndToolbar.cpp`、`ListCtrlEffectInfo.cpp`、`ListCtrlItemInfo.cpp` を `SboCliAdminMfc/src/Migrated/Wnd/` へ移動済み。
  - `SboCli.vcxproj` / `SboCliAdminMfc.vcxproj` 双方の参照先を更新済み。
- `WndSelectGrp.cpp` / `WndSelectItemGrp.cpp`:
  - 依存キャリアとして `Img32.cpp`、`lodepng.cpp`（PrecompiledHeader=NotUsing）、`MgrGrpData.cpp` を DLL ビルドに追加。
  - `resource.h` 解決のためインクルードパスに `..\SboGrpData` を追加。
  - 物理ファイルを `SboCliAdminMfc/src/Migrated/Wnd/` へコピー済み。
  - `SboCli.vcxproj` の参照先を Migrated パスへ更新済み。
  - `Debug|Win32` / `Release|Win32` でビルド成功（警告のみ、エラー 0）。
- 結論：
  - Step2 は依存キャリアを段階的に追加しながら、ユニット順序を厳守して実施する。

## リスク注記
- `MgrData` 具体型への依存は Step3 の Host 抽象化対応まで残存する。
- `WndMap` 系ユニットは Step4（高依存移管）まで保留する。

## Step3 前準備の進捗（ソケット依存の除去）
- `DlgAdminMapParts` / `DlgAdminMapShadow`:
  - メンバレベルの `CUraraSockTCPSBO* m_pSock` を除去済み。
  - `Init()` 時の `GetUraraSockTCP()` 取得を除去済み。
  - パケット送信経路は `CDlgAdminBase::SendPacket` で統一済み。
- 現ツリーで残存する `GetUraraSockTCP()` の呼び出し箇所：
  - `AdminUiLoader` のホストコールバックフォールバックパス（`SendAdminPacket`、後方互換のため）。
  - `DlgDbg` のスループット表示パス（管理 DLL 移管の対象外）。
