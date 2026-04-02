# Webクライアント化 全体ロードマップ

## 1. 背景と目的

現在の目標は、最終的に `SboCli` を Web 上で動作させ、VPS 上で稼働する `SboSvr` と通信して、既存デスクトップ版と同等のゲームプレイを成立させることである。

ただし、この目標は単一の作業ではなく、以下の 3 つの大きな段階に分かれる。

1. `SboCli` の Windows 依存を縮退し、SDL ベースで動くゲームクライアントとして自立させる
2. `SboCli` を WebAssembly 化できる構造に整理する
3. Web クライアントと `SboSvr` の通信路を WebSocket 化し、Web 用の端末識別へ移行する

本ドキュメントは、この 3 つをつなぐ **親計画** である。

---

## 2. 結論

実作業の優先順位は、以下で固定する。

1. **最優先: Windows依存脱却の継続**
2. **次優先: WebAssembly 化に向けたクライアント構造整理**
3. **その後: WebSocket 化と Web 用端末識別導入**

つまり、

- `WebSocket` 化は必要
- しかし **SDL 化と Windows 依存脱却が先**
- その後に通信層の差し替えへ進む

という順序が、このリポジトリではもっとも自然である。

加えて、直近の最初の到達目標は次で固定する。

- **サーバー通信なしで起動し、タイトル画面まで表示できること**

この段階では、ログイン要求送信やサーバー接続成功は目標に含めない。
まず「ローカルで起動して、ロゴからタイトル画面へ遷移するところまで」を最小マイルストーンとする。

---

## 3. 既存計画との関係

この親計画は、既存の以下 2 本を上位から束ねる。

- [sbocli-mfc-dll-sdl-implementation-plan.md](/C:/Users/masakazu/MyProject/SBOP2/docs/sbocli-mfc-dll-sdl-implementation-plan.md)
- [web-client-websocket-migration-plan.md](/C:/Users/masakazu/MyProject/SBOP2/docs/web-client-websocket-migration-plan.md)

加えて、Windows 依存の棚卸しは以下を参照する。

- [windows-dependency-status.md](/C:/Users/masakazu/MyProject/SBOP2/docs/windows-dependency-status.md)

役割分担は次のとおり。

| ドキュメント | 役割 |
|---|---|
| `sbocli-mfc-dll-sdl-implementation-plan.md` | SDL 化・Windows依存脱却の個別実装計画 |
| `web-client-websocket-migration-plan.md` | WebSocket 化・端末識別移行の個別実装計画 |
| `web-client-overall-roadmap.md` | 全体の順序とマイルストーンを決める親計画 |

---

## 4. 現状認識

### 4.1 SDL 化の現状

すでに SDL 基盤自体は導入済みであり、以下は進んでいる。

- SDL ウィンドウ生成
- SDL メインループ
- SDL 入力入口の一部
- SDL レンダラ入口の一部
- 管理 UI の DLL 分離

一方で、Windows 依存はまだ多数残っている。

- `HWND` 取得
- SDL ウィンドウの Win32 サブクラス化
- `PeekMessage` / `DispatchMessage`
- `HDC` / `CImg32` / GDI 描画
- DirectInput
- `CreateWindow` ベースの補助 UI
- `DXAudio` / `AflMusic` / `winmm` / `DirectSound`

この状態では、まだ WebAssembly 化の入口に立った段階であり、**通信層の前にクライアント本体の非 Windows 化が必要**である。

### 4.1.5 タイトル画面到達の観点での補足

`StateProcLOGO` と `StateProcLOGIN` を見る限り、

- ロゴ表示
- フェード
- タイトルレイヤ表示

までは、主に `MgrDraw` / `MgrLayer` / `MgrSound` の動作で成立する。

一方で、ログイン画面の入力 UI は `WindowLOGIN` が `CreateWindow("EDIT")` / `CreateWindow("BUTTON")` に依存しているため、現時点では Windows 固有要素が強い。

したがって、当面の切り分けは次のようにする。

1. **起動してタイトル画面まで表示**
2. **その次にログイン UI の SDL 化または代替 UI 化**
3. **その後に通信接続**

この順序により、「タイトル表示のために不要な Win32 入力欄対策」へ早く手を広げすぎることを防ぐ。

### 4.2 通信の現状

通信自体は、ゲームロジック上は以下の意味論に依存している。

- 順序保証
- 信頼性
- バイナリパケット
- 接続/切断イベント
- keepalive

これ自体は WebSocket 化可能である。  
ただし、現時点ではブラウザでクライアント本体を動かす準備が整っていないため、**通信は後段**とする。

### 4.3 端末識別の現状

デスクトップ版ログインは MAC アドレスを送る設計であり、軽い多重利用抑止や BAN 補助に使われている。

Web 版では MAC が取れないため、

- デスクトップ版: `MAC`
- Web 版: `clientId`

の両対応へ移行する必要がある。

---

## 5. 最終目標アーキテクチャ

最終的に目指す構成は以下である。

### クライアント

- デスクトップ版
  - SDL ベース
  - Windows 固有依存は最小化
  - transport は TCP 実装または将来共通化された transport

- Web 版
  - SDL + Emscripten / WebAssembly
  - ブラウザ UI / 入力 / 音声 / 描画へ対応
  - transport は WebSocket
  - 端末識別は `clientId`

### サーバー

- `SboSvr`
  - 既存ゲームロジック
  - 既存 TCP 経路
  - 将来的な `LOGIN2` / `ClientIdentity` 両対応

- 初期導入時は必要に応じて `SboWsGate`
  - WebSocket <-> TCP 1対1中継

---

## 6. 全体フェーズ

以下を全体ロードマップとする。

### Phase A0: ローカル起動・タイトル画面到達

**目的**

- サーバー通信なしで起動し、ロゴからタイトル画面まで遷移できる最小実行状態を作る

**主作業**

- `MainFrame` 初期化でタイトル表示までに必須な依存を整理
- `StateProcLOGO` -> `StateProcLOGIN` の遷移をローカルで成立させる
- タイトル表示に不要な通信前提コードを切り離す
- タイトル表示に不要な Win32 依存を後回しにする

**この段階で目標に含めるもの**

- アプリ起動
- ロゴ表示
- フェード
- タイトル背景表示
- BGM 再生の確認

**この段階で目標に含めないもの**

- サーバー接続
- ログイン成功
- キャラ選択
- マップ入場

**完了基準**

- 通信しなくても、起動からタイトル画面まで安定して到達できる

### Phase A: Windows依存脱却の継続

**目的**

- `SboCli` を「Windows の上で SDL を使っているアプリ」から、「SDL ベースで動くゲームクライアント」へ近づける

**主作業**

- `HWND` 取得、サブクラス化、Win32 メッセージループの縮退
- DirectInput の排除
- `HDC` / `CImg32` / GDI 描画の SDL テクスチャ化
- 補助 UI の SDL 化または別レイヤ分離
- 音声の Windows 固有依存切り分け

**参照**

- [sbocli-mfc-dll-sdl-implementation-plan.md](/C:/Users/masakazu/MyProject/SBOP2/docs/sbocli-mfc-dll-sdl-implementation-plan.md)
- [windows-dependency-status.md](/C:/Users/masakazu/MyProject/SBOP2/docs/windows-dependency-status.md)

**完了基準**

- コアゲームプレイが Win32 サブクラス化や GDI に依存せずに成立する

### Phase B: WebAssembly 化準備

**目的**

- SDL 化されたクライアントを Emscripten 対応しやすい構造へ整理する

**主作業**

- プラットフォーム差分の整理
- ファイルアクセス、入力、音声、タイマの抽象化
- Emscripten で使えない API の排除
- ビルド構成の整理

**完了基準**

- 「通信なし・ローカル実行のみ」の WebAssembly 試作が可能

### Phase C: 通信層抽象化

**目的**

- 既存 TCP 実装と将来の WebSocket 実装を切り替えられるようにする

**主作業**

- `CUraraSockTCPSBO` 依存箇所の整理
- transport interface 導入
- デスクトップ用 TCP transport ラッパー化

**完了基準**

- 上位ゲームロジックが transport 実装差分を直接知らない

### Phase D: サーバー側端末識別抽象化

**目的**

- Web 版を受け入れるために MAC 専用設計を解消する

**主作業**

- `ClientIdentity` 導入
- MAC 依存箇所の局所化
- BAN / 多重抑止ロジックの複合化

**完了基準**

- デスクトップ版 `MAC` と Web 版 `clientId` の両方を扱える

### Phase E: `LOGIN2` と WebSocket 入口

**目的**

- Web クライアントからゲームサーバーへ接続可能にする

**主作業**

- `PacketCONNECT_REQ_LOGIN2`
- 必要なら `RES_LOGIN2`
- `clientId` 運用
- `SboWsGate` 試作または本体 WebSocket 受付

**完了基準**

- Web クライアントがログインからゲーム開始まで通る

### Phase F: Web クライアント統合

**目的**

- Web 上での実プレイ成立性を確認する

**主作業**

- マップ表示
- 移動
- チャット
- 他プレイヤー同期
- 切断/復帰
- 高遅延検証

**完了基準**

- デスクトップ版と比較して致命的差異なくプレイできる

### Phase G: 運用移行

**目的**

- Web 版を継続運用できる状態へ持っていく

**主作業**

- `clientId` ベースの軽い多重対策
- Web 用ログ整備
- VPS 上の `wss://` 運用
- ゲートウェイ方式継続か本体統合かの判断

**完了基準**

- 実運用方針が決まり、保守可能な構成になる

---

## 7. 実作業の推奨順序

直近で進める順序は以下で固定する。

1. 起動からタイトル画面までに実際に通る経路を固定する
2. `MainFrame` / `SDLApp` に残る Win32 メッセージ依存のうち、タイトル表示を妨げる箇所を優先して縮退する
3. `MgrDraw` / `Layer` のうち、ロゴ・タイトル表示に必要な描画経路を優先して SDL 主系へ寄せる
4. タイトル表示後も残る `WindowLOGIN` の Win32 入力欄依存を、次段階の課題として分離する
5. その後に `MgrKeyInput` の DirectInput 依存排除を進める
6. 補助 UI と音声経路の Windows 固有依存整理を進める
7. そこまで進んだ後に transport 抽象化
8. さらにその後に `ClientIdentity` / `LOGIN2`
9. 最後に WebSocket ゲートウェイまたは本体 WebSocket 化

つまり、**当面は通信関連より SDL 化と Windows依存脱却を優先する。**

さらに細かく言うと、**最初の実装対象は「タイトル表示に必要な部分」へ限定する。**

### 7.1 タイトル画面到達までの確定実装順

以下を、直近の実装順として確定する。

1. `MainFrame::OnInitEnd()` から `ChgGameState(GAMESTATE_LOGO)` までの初期化経路を確認し、タイトル表示に不要な失敗要因を切り分ける
2. `StateProcLOGO` の表示・フェード・効果音が SDL 経路で成立するようにする
3. `StateProcLOGIN` の `m_pMgrLayer->MakeTITLE()` とタイトル BGM 再生を成立させる
4. `WindowLOGIN` は当面「非表示でも落ちない」状態を目標にし、タイトル表示の阻害要因になっている Win32 入力欄生成を切り分ける
5. 「起動 -> ロゴ -> タイトル画面」までの安定化後に、ログイン UI の SDL 化または別実装へ進む

ポイントは、**`WindowLOGIN` をいきなり完全 SDL 化しないこと**である。
タイトル画面表示の最小目標に必要なのは入力欄ではなく、タイトル背景・レイヤ・フェード・BGM の成立だからである。

---

## 8. 直近マイルストーン

次のマイルストーンを設定する。

### M0: 起動してタイトル画面まで表示

- ロゴ表示
- フェード
- タイトル背景表示
- 通信なしで安定起動

### M1: Windowsメッセージ依存の大幅縮退

- `HWND` 取得やサブクラス化の使用箇所を縮退
- `PeekMessage` / `DispatchMessage` 依存を減らす

### M2: 入力と描画の SDL 主系化

- DirectInput を除去
- GDI 中心描画を SDL 側へ寄せる
- ログイン UI の SDL 化または代替 UI 化を含む

### M3: 補助 UI / 音声の整理

- WebAssembly 化の障害になる補助経路を切り分ける

### M4: transport 抽象化

- ここで初めて通信層へ入る

### M5: Web ログイン試験

- `clientId`
- `LOGIN2`
- WebSocket 経路

### M6: Web 実プレイ試験

- ログイン、移動、チャット、同期の確認

---

## 9. リスクと対策

| リスク | 対策 |
|---|---|
| SDL 化の途中で WebSocket 側へ手を広げて工程が散る | 親計画で「SDL 化先行」を固定する |
| Windows 依存が想定以上に深い | `windows-dependency-status.md` を更新しつつ段階的に削る |
| 通信層抽象化を早くやりたくなっても、クライアント本体が Web で動かない | 先に WebAssembly 化障害を潰す |
| MAC 依存の移行で運用感が変わる | `clientId + IP + accountId` の複合制御へ移る |
| WebSocket 化の時点で別の UI/入力依存が残る | Phase A/B 完了基準を厳密に置く |

---

## 10. 受け入れ基準

### A. クライアント構造

- `SboCli` のゲームプレイ主要部分が Win32 / GDI / DirectInput へ本質依存しない

### A0. 最初の到達点

- サーバー通信なしで起動できる
- ロゴ画面が表示される
- タイトル画面へ遷移できる
- タイトル表示まででクラッシュしない

### B. WebAssembly 準備

- 通信なしでも Web 上でゲーム画面が成立する

### C. 通信

- Web クライアントが `SboSvr` とログイン・移動・チャット・同期できる

### D. 運用

- デスクトップ版と Web 版が当面共存できる
- Web 版でも軽い多重対策と BAN 補助が可能

---

## 11. まとめ

今後の判断基準は明確である。

- **まず SDL 化と Windows依存脱却**
- **次に WebAssembly 化のための構造整理**
- **その後に WebSocket 化**

この順序を崩さないことが、全体を最短で前進させるうえで重要である。

したがって、直近の実装フェーズは `web-client-websocket-migration-plan.md` ではなく、まず `sbocli-mfc-dll-sdl-implementation-plan.md` と `windows-dependency-status.md` に沿って進める。
