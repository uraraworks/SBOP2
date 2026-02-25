# 管理UIを設計的に取り出すための方針案

最終更新: 2026-02-24

## 現状のボトルネック
- いまの境界は `LoadLibrary/GetProcAddress` まで実装済みだが、UI実体は `CMgrData` 具体型への直接依存が強い。
- `CAdminUiLoader` はホストコールバックを持つが、実際にはローカルフォールバックで `CAdminWindow` を直接 `new` している。
- `CDlgAdminBase` が `CMgrData` と `CUraraSockTCPSBO` を直接保持しており、Dialog群の依存が伝播する。

参照:
- `SboCli/src/AdminApi/AdminUiLoader.cpp`
- `SboCli/src/AdminWindow.cpp`
- `SboCli/src/Dialog/DlgAdminBase.h`
- `SboCli/src/Dialog/DlgAdminBase.cpp`

## 取り出し方の選択肢

### 案A: Host Facade方式（推奨）
- `IAdminUiHost` を「UIが必要とする最小機能」単位で拡張する。
- DLL側UIは `CMgrData` を知らず、Host APIだけを使う。
- `CMgrData` へのアクセスは exe 側 Facade 実装 (`AdminUiHostFacade`) に閉じ込める。

メリット:
- 依存方向が一方向（DLL -> Host ABI）になり、長期的に安定。
- Step2/Step3/Step4 を自然につなげられる。

デメリット:
- API設計と置換に手間がかかる。

### 案B: Data Snapshot + Command方式
- DLLに渡すのは「表示用スナップショット」と「実行コマンド」のみ。
- 状態参照は pull ではなく push を基本にする。

メリット:
- ABIが明確でテストしやすい。

デメリット:
- 既存Dialogの作りが pull 前提なので改修量が大きい。

### 案C: Thin Proxy方式
- DLL側に旧来ヘッダをそのまま見せ、exe側で必要型を中継する。

メリット:
- 初速が速い。

デメリット:
- 依存を温存しやすく、最終的に取り出しが止まる。

## 推奨: 案Aの段階導入

### Phase P1: 呼び出し抽象の導入（最小）
- `CDlgAdminBase` に `CMgrData*` 直参照ではなく `IAdminUiHost*` を受ける経路を追加。
- 既存コード互換のため、当面は `IAdminUiHost` から `CMgrData` へ委譲する実装を併置。

### Phase P2: 高頻度依存の置換
- まず以下を Host API 化する:
- `SetAdminNotifyTypeL/R/RR`
- `SetAdminWindow`
- `GetMoveNoBlock` / `SetMoveNoBlock`
- `SetViewGrid`
- `SendAdminPacket`（内部で `GetUraraSockTCP()->Send`）

### Phase P3: UI生成責務の分離
- `CAdminUiLoader::CreateLocalAdminUiInternal` の `new CAdminWindow` を、`CreateAdminUiInstance` 経由に置換。
- DLL側で `AdminWindow` 本体を持てるようになった段階で、exe側は生成責務を外す。

### Phase P4: 単位移管の順序固定
- 低依存 `Wnd` から移管（Step2分析済み候補）。
- 次に `DlgAdminBase` 系。
- 最後に `AdminWindow` と `WndMap` 系。

## 実装時の設計ルール
- ABI境界は C関数 + POD のみ（例外、STLコンテナ、MFC型を跨がない）。
- 1ユニットごとに「追加API」「プロジェクト定義」「切り戻し手順」をセットで管理。
- `CMgrData` include を DLL 側から減らすことを完了条件に含める。

## 最初の実装候補（小さい一歩）
1. `IAdminUiHost` に `SendAdminPacket` を追加。
2. `CDlgAdminBase::Init` で `m_pSock` を直接保持しない経路を追加。
3. `AdminWindow::OnMapAdd` の送信処理を Host API 経由へ置換。

この3点が通ると、通信経路の `CMgrData` 直接依存を1本切れるため、次のDialog移管が楽になる。
