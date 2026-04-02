# ローカル起動・タイトル画面到達ロードマップ

## 0. 2026-04-01 時点の到達点

- browser build は通過済み
- browser/headless でロゴ表示まで確認済み
- ロゴの fade-in / fade-out も browser で進む
- `GAMESTATE_LOGIN` への遷移も発生している
- ただしタイトル表示は未安定で、ロゴ後に黒画面のまま止まるケースが残る

このため、次はタイトル画面だけを個別に直し続けるのではなく、**ロゴ -> タイトル経路に残っている Win32 メッセージ依存を外す** 方針で進める。

## 1. 目的

直近の最初の到達目標を、以下で固定する。

- サーバー通信なし
- アプリが起動する
- ロゴ画面が表示される
- フェードを経てタイトル画面へ遷移する

この段階では、ログイン送信、キャラ選択、マップ入場は対象外とする。

---

## 2. この段階で通る主要経路

タイトル画面到達までに主に通るのは以下である。

1. `CMainFrame::OnInitEnd()`
2. `ChgGameState(GAMESTATE_LOGO)`
3. `CStateProcLOGO::Init()`
4. `CMgrLayer::MakeLOGO()`
5. `CMgrDraw` のフェード処理
6. `CStateProcLOGO::OnMgrDrawEND_FADEOUT()`
7. `ChgGameState(GAMESTATE_LOGIN)`
8. `CStateProcLOGIN::Init()`
9. `CMgrLayer::MakeTITLE()`

この経路から分かること:

- タイトル画面そのものは `MgrLayer` と `MgrDraw` が中心
- `WindowLOGIN` はログイン UI のために存在するが、タイトル背景表示の最小要件ではない
- したがって最初は **`WindowLOGIN` を完全に動かすことではなく、タイトル表示を阻害しないこと** を目標にする

---

## 3. 現時点のボトルネック

### 3.1 描画系

`MgrDraw` は最終出力を SDL テクスチャへ流せるようになっているが、内部はなお `CImg32` / `HDC` / GDI ベースで動いている。

ただし、ロゴ・タイトル表示の段階では、まず「画面に出るか」が重要であり、内部実装が GDI ベースでも直ちにブロッカーとは限らない。

### 3.2 ログイン UI

`StateProcLOGIN::Init()` は `m_pMgrWindow->MakeWindowLOGIN()` を呼ぶ。  
`WindowLOGIN` は `CreateWindow("EDIT")` / `CreateWindow("BUTTON")` を用いて Win32 コントロールを直接生成している。

このため、現時点での `WindowLOGIN` は WebAssembly 化どころか「Win32 非依存化」の観点でも重い依存になっている。

### 3.3 メッセージ依存

ロゴからタイトルへの遷移は `MgrDraw` のフェード終了通知と `WM_MAINFRAME` 系メッセージに依存している。
ここは当面 SDL 上でも成立させる必要がある。

---

## 4. 方針

タイトル画面到達までの方針は次のとおり。

1. **起動 -> ロゴ -> タイトル背景表示** を先に成立させる
2. `WindowLOGIN` は最初から完全 SDL 化しない
3. まずは `WindowLOGIN` がなくても落ちない、または非表示でもタイトルが出る状態を作る
4. その後にログイン UI の SDL 化または代替 UI 化へ進む

要するに、最初の目標は **タイトル画面の成立** であり、**ログイン入力欄の成立ではない**。

---

## 5. 実装順

以下を、タイトル画面到達までの実装順として確定する。

### Step 1: 起動経路の確認とローカル起動モード整理

**目的**

- サーバーへ接続しなくても、起動からタイトルまで進める前提を固める

**作業**

- `CMainFrame::OnInitEnd()` の初期化処理を確認
- タイトル表示前に不要な通信前提や失敗条件がないか整理
- 「ローカル起動モード」の扱いを決める
  - 既存コードのままタイトルまで行けるならそのまま
  - 途中で通信前提があるなら切り離す

**完了条件**

- 起動後に `GAMESTATE_LOGO` へ遷移できる

### Step 2: ロゴ画面表示の成立

**目的**

- `StateProcLOGO` が SDL 経路で正しく見えることを確認する

**作業**

- `CStateProcLOGO::Init()` の描画・フェード開始を確認
- `CMgrLayer::MakeLOGO()` で生成される `LayerLogo` の描画確認
- ロゴ表示に必要な画像リソースロード確認
- フェードイン・フェードアウト通知が `MgrDraw` 経由で動くか確認

**完了条件**

- ロゴが表示され、フェードアウト開始まで進む

### Step 3: ロゴ -> タイトル遷移の成立

**目的**

- フェード終了後に `GAMESTATE_LOGIN` へ遷移できるようにする

**作業**

- `OnMgrDrawEND_FADEOUT()` から `GAMESTATE_LOGIN` への状態遷移確認
- `WM_MAINFRAME` / `WM_MGRDRAW` 系通知が SDL 化後も成立するか確認
- タイトル遷移でクラッシュしないことを確認

**完了条件**

- ロゴ終了後にタイトル状態へ切り替わる

### Step 3.5: Win32 メッセージ依存の縮退

**目的**

- browser でも native でも同じ順序で状態遷移と fade 通知が進むようにする

**作業**

- `WM_MAINFRAME` / `WM_MGRDRAW` を高レベル制御の前提から外す
- `StateProcLOGO::OnMgrDrawEND_FADEOUT()` の状態変更通知を内部イベント化または直接呼び出しへ寄せる
- `StateProcLOGIN::OnMgrDrawSTART_FADEIN()` の呼び出し順が `MakeTITLE()` 後に必ず成立するよう整理する
- `MainFrame::OnMainFrame()` / `OnMgrDraw()` を「Win32 入口」ではなく「内部通知ハブ」として扱える形へ寄せる

**完了条件**

- `HWND` の有無で状態遷移の意味が変わらない
- browser で shell 側の補助が最小になってもロゴ遷移が再現しやすくなる

### Step 4: タイトル背景・BGM 表示の成立

**目的**

- `StateProcLOGIN` の最小責務のうち、タイトル背景と BGM を先に成立させる

**作業**

- `CStateProcLOGIN::Init()` の `m_pMgrLayer->MakeTITLE()` を確認
- `LayerTitle` が描画されるようにする
- タイトル画面 BGM の再生確認
- タイトルフェードイン処理の確認

**完了条件**

- タイトル背景が表示され、BGM が鳴り、クラッシュしない

### Step 5: `WindowLOGIN` の切り分け

**目的**

- Win32 入力欄がタイトル表示を壊さない状態を先に作る

**作業**

- `StateProcLOGIN::Init()` の `MakeWindowLOGIN()` 呼び出しを見直す
- まずは以下のどちらかを選ぶ
  - A. `WindowLOGIN` を生成しても落ちない状態に留める
  - B. タイトル表示中は `WindowLOGIN` 生成を遅延または無効化する
- `WindowLOGIN` がなくても `StateProcLOGIN::TimerProc()` が破綻しないように整理する

**推奨**

- まずは B を推奨する
- つまり、タイトル画面到達の段階では `WindowLOGIN` を後回しにする

**完了条件**

- `WindowLOGIN` の Win32 依存があっても、タイトル表示のブロッカーにならない

### Step 6: タイトル画面到達の安定化

**目的**

- 毎回安定して起動からタイトルまで到達する状態にする

**作業**

- 複数回起動で再現確認
- 画像未ロード、フェード未完了、BGM 初期化失敗などの例外経路確認
- 起動ログが必要なら最小限追加

**完了条件**

- 連続起動しても、サーバー通信なしでタイトル画面まで到達する

---

## 6. 作業対象ファイルの優先度

タイトル表示までの優先対象は以下。

### 優先度 A

- `SboCli/src/MainFrame/MainFrame.cpp`
- `SboCli/src/StateProc/StateProcLOGO.cpp`
- `SboCli/src/StateProc/StateProcLOGIN.cpp`
- `SboCli/src/MgrDraw.cpp`
- `SboCli/src/MgrLayer.cpp`
- `SboCli/src/Layer/LayerLogo.*`
- `SboCli/src/Layer/LayerTitle.*`

### 優先度 B

- `SboCli/src/MgrSound.*`
- `SboCli/src/MgrGrpData.*`
- `SboCli/src/Window/WindowLOGIN.*`

### この段階では後回し

- `StateProcLOGINMENU`
- `StateProcMAP`
- `WindowCHAT`
- `WindowTEXTMSG`
- 通信層
- `ClientIdentity`
- `LOGIN2`
- WebSocket ゲートウェイ

---

## 7. マイルストーン

### M0-1: 起動してロゴ画面が出る

- クラッシュせずロゴが出る

### M0-2: ロゴからタイトルへ遷移する

- フェードが動作し、タイトル状態へ移る

### M0-3: タイトル背景と BGM が出る

- タイトル画面が視覚・音声ともに成立する

### M0-4: `WindowLOGIN` なしでもタイトル画面が安定する

- Win32 入力欄依存を切り離してもタイトル到達できる

### M0 完了

- サーバー通信なしで、起動からタイトル画面まで毎回到達できる

---

## 8. 受け入れ基準

- アプリが起動する
- `GAMESTATE_LOGO` に入る
- ロゴが描画される
- フェードアウト後に `GAMESTATE_LOGIN` へ遷移する
- タイトル背景が描画される
- タイトル BGM が再生される
- この過程でサーバー接続を要求しない
- `WindowLOGIN` の完成有無に関係なくタイトル表示までクラッシュしない

---

## 9. この段階の次の課題

タイトル画面到達が完了したら、次は以下へ進む。

1. ログイン UI の SDL 化または代替 UI 化
2. その後に transport 抽象化
3. 最後に WebSocket 化

つまり、タイトル画面到達は **Windows依存脱却の最初の実働マイルストーン** と位置付ける。

---

## 次セッションの着手順

1. `MainFrame` / `MgrDraw` / `StateProcLOGO` / `StateProcLOGIN` の通知経路を洗い出す
2. `WM_MAINFRAME` / `WM_MGRDRAW` を内部イベントまたは直接通知へ寄せる最小単位を決める
3. ロゴ -> タイトル遷移を shell 依存少なめで再現する
4. その後に title 画面の描画未表示を再確認する
