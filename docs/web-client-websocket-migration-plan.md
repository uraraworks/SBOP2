# Webクライアント WebSocket化・端末識別移行計画

## 1. 背景と目的

現在の `SboCli` はデスクトップ向け TCP クライアントとして `SboSvr` と独自パケット通信を行っている。
将来的には以下を成立させたい。

1. `SboCli` のゲーム部を Web 上で動作可能にする
2. Web クライアントから VPS 上の `SboSvr` と通信し、既存デスクトップ版と同等のゲームプレイを成立させる
3. 既存デスクトップ版をすぐには捨てず、段階的に移行する

本計画はそのうち特に **通信路の WebSocket 化** と **MAC アドレス依存の端末識別を Web 対応へ移行すること** に焦点を当てる。

---

## 2. 調査結果サマリ

### 2.1 現行通信の性質

現行の `SboCli` / `SboSvr` 間通信は、実質的には以下の性質を持つ。

- 順序保証あり
- 信頼性あり
- バイナリパケット列
- 接続/切断イベントあり
- 定期 keepalive あり
- セッション単位の接続管理あり

これは「生の TCP ソケット API」には依存しているが、**ゲームロジックが本当に必要としているのは TCP そのものではなく、上記の通信意味論**である。
そのため、同等の意味論を満たす `WebSocket` バイナリ通信へ移行することは十分可能と判断できる。

### 2.2 現行 `UraraSock` の主要仕様

`SboSockLib/UraraSockTCP.cpp` より:

- ソケット上で独自フレーム `URARASOCK_PACKETINFO { dwSize, dwCRC }` を付与
- 接続直後に precheck 用 `DWORD` を送受信
- 30 秒 keepalive（クライアント側）
- 128 byte 以上は zlib 圧縮する経路あり
- 送受信完了後、上位には `WM_URARASOCK_CONNECT / RECV / DISCONNECT` で通知

### 2.3 上位ゲーム処理の構造

`SboCli` 側:

- `MainFrame::Connect()` が `m_pSock->Connect(...)` を呼ぶ
- 接続完了後 `OnConnect()` でバージョンチェック要求を送る
- 受信時は `OnRecv()` で `CPacketBase` の `byCmdMain / byCmdSub` を見て分岐する

`SboSvr` 側:

- `OnRecv()` で `byCmdMain / byCmdSub` ごとに `RecvProc*` へ分岐する
- 1 接続ごとに `dwSessionID` を割り当ててキャラやアカウントへ紐付ける

この構造から、**transport を差し替えても上位パケット処理はかなり流用可能**である。

### 2.4 移動同期の状況

サーバー/クライアントにはすでに Dead Reckoning 系の移動パケットが導入されている。

- `MOVE_START`
- `MOVE_DIR_CHANGE`
- `MOVE_STOP`
- `POS_SYNC`

時刻逆行チェックや速度超過拒否もサーバー側に入っており、インターネット越しの WebSocket 化と相性は悪くない。

### 2.5 Web 化で最大の差分になる点

最大の差分は通信そのものよりも **MAC アドレス依存のログイン仕様** である。

現状:

- クライアントは `PacketCONNECT_REQ_LOGIN` に MAC 6 byte を入れて送る
- サーバーはそれを `m_strMacAddr` として保存し、軽い多重対策や BAN の補助情報として使う

ブラウザでは MAC アドレスは取得できないため、**Web 版では別の端末識別子が必要**になる。

---

## 3. 端末識別の移行方針

### 3.1 要求水準の整理

今回必要なのは、厳格なハードウェア固定識別ではなく以下である。

- 問題のあるユーザーを軽く追跡できる
- 同一端末からの多重利用を軽く抑止できる
- 誤判定が多すぎない
- Web ブラウザで実装できる

この要件であれば、**MAC アドレスの完全代替は不要**であり、サーバー発行の永続 `clientId` を中心に設計するのが妥当である。

### 3.2 新しい識別子の設計

Web 版では以下を採用する。

- 主識別子: `clientId`
  - 初回接続時にサーバーまたはクライアントで UUID を生成
  - `localStorage` または `IndexedDB` に保存
  - 以後ログイン・接続時に毎回送信
- 補助識別子:
  - IP アドレス
  - アカウント ID
  - 接続頻度
  - 同時接続数
  - 必要なら user agent 等

### 3.3 BAN / 多重対策の単位

今後の BAN / 抑止判定は以下の複合条件へ移行する。

- 強い制御: `accountId`
- 中程度の制御: `clientId`
- 補助情報: `IP`

具体例:

- アカウント BAN: `accountId`
- 軽い端末 BAN: `clientId`
- 一時レート制限: `IP + clientId`
- 多重利用抑止: 同一 `clientId` の同時複数アカウント利用を拒否または警告

### 3.4 既存デスクトップとの両対応方針

移行期間中は、識別方式を次のように分ける。

| クライアント種別 | 主識別子 | 補助識別子 |
|---|---|---|
| 既存デスクトップ版 | MAC | IP, accountId |
| Web 版 | clientId | IP, accountId |

サーバー側では内部表現を以下へ寄せる。

```cpp
enum ClientIdentityType {
    CLIENT_IDENTITY_NONE = 0,
    CLIENT_IDENTITY_MAC,
    CLIENT_IDENTITY_WEB_CLIENT_ID,
};

struct ClientIdentity {
    int type;
    CString strValue;
};
```

これにより、デスクトップの MAC と Web の `clientId` を同列に扱える。

### 3.5 互換性のための基本方針

**ログイン要求をそのまま流用して無理に MAC 欄へ `clientId` を詰めるのは避ける。**

理由:

- 6 byte 固定で表現力が足りない
- 将来 `sessionToken` などを足しにくい
- どのクライアント種別なのか判断しづらい

そのため、ログイン接続系は段階的に以下へ移行する。

1. 既存 `PacketCONNECT_REQ_LOGIN` はデスクトップ互換として維持
2. Web 版用に `PacketCONNECT_REQ_LOGIN2` を追加
3. `LOGIN2` では `clientKind`, `clientId`, 必要なら `resumeToken` を送る
4. サーバーは `LOGIN` / `LOGIN2` の両方を受ける

---

## 4. WebSocket 化アーキテクチャ案

### 4.1 案A: `SboSvr` へ WebSocket 入口を直接追加

**構成**

- ブラウザ/WebAssembly クライアント
- `SboSvr` 内 WebSocket 受付
- 既存ゲーム処理へ直接受け渡し

**利点**

- 構成が単純
- 中継プロセスが不要
- 将来運用が一本化しやすい

**欠点**

- `SboSvr` への改修量が大きい
- 現行 `UraraSock` と HTTP サーバーの両方に加えて WebSocket を抱えることになる
- 初期検証で失敗した時の切り戻し範囲が広い

### 4.2 案B: WebSocket-TCP ゲートウェイを別プロセスで用意

**構成**

- ブラウザ/WebAssembly クライアント
- `SboWsGate`（新規）
- 既存の `SboSvr` TCP

**利点**

- 既存 `SboSvr` の改修を最小化できる
- 初期段階で最も安全
- Web 化の問題とゲームサーバー本体の問題を分離できる

**欠点**

- プロセスが1つ増える
- 認証・ログ・切断処理をどちらが持つか設計が必要
- 長期的には二重の接続層を保守することになる

### 4.3 初期推奨

**Phase 1 は案B（ゲートウェイ方式）を推奨する。**

理由:

- ゲーム成立性の検証を早く行える
- サーバー本体の既存ロジックを大きく壊さずに済む
- Web 化で最初に問題になりやすいのは「通信路」より「ブラウザ側制約」なので、そこを先に切り分けられる

そのうえで、Web クライアントが安定して動くことを確認した後、必要なら案Aへ統合する。

---

## 5. 推奨移行案

### 5.1 方針

以下を正式な推奨移行案とする。

1. デスクトップ版は当面 `TCP + MAC` を維持
2. Web 版は `WebSocket + clientId` を採用
3. サーバー側は `LOGIN` / `LOGIN2` 両対応にする
4. 初期導入は `WebSocket <-> TCP` ゲートウェイ方式
5. ある程度安定した後に、必要なら `SboSvr` へ WebSocket 受付を内蔵する

### 5.2 セッション管理

ゲートウェイ方式では **1 WebSocket 接続 = 1 backend TCP 接続 = 1 `dwSessionID`** とする。

多重化しない理由:

- 現行 `SboSvr` が接続単位の `dwSessionID` を前提にしている
- ログイン状態、キャラ状態、切断処理が 1 接続 1 セッションで整理されている
- 多重化すると既存コードへの影響が大きすぎる

### 5.3 ゲートウェイの責務

`SboWsGate` の責務は最小限に留める。

- WebSocket の受付
- バイナリメッセージの送受信
- 必要なら Web 用ハンドシェイクや認証前段
- `UraraSock` の外側フレームへの変換
- backend TCP との 1 対 1 中継

原則として:

- ゲームロジックは持たない
- パケット内容はできるだけ解釈しない
- ただし `LOGIN2` のような Web 専用パケットだけは変換してもよい

### 5.4 ログイン互換の段階設計

#### Step 1

- デスクトップは現行 `PacketCONNECT_REQ_LOGIN`
- Web は `PacketCONNECT_REQ_LOGIN2`
- ゲートウェイは `LOGIN2` をそのままサーバーへ送る
- サーバーは `LOGIN` / `LOGIN2` の両方を受理

#### Step 2

- サーバー内部で `ClientIdentity` を導入
- `InfoAccount` などの MAC 依存処理を `ClientIdentity` ベースへ置換

#### Step 3

- 運用が安定したら、デスクトップ版も `LOGIN2` に寄せられるようにする
- その後、旧 `LOGIN` を段階廃止するか判断する

---

## 6. 作業ロードマップ

以下を実作業の正式ロードマップとする。

### Phase 0: 設計固定

**目的**

- 移行方針を確定し、ブレを止める

**作業**

- 本ドキュメントを基準計画として採用
- Web 版は `clientId` ベース識別とする方針を確定
- 初期導入はゲートウェイ方式とする方針を確定

**完了基準**

- 通信路・端末識別・互換方針に迷いがない

### Phase 1: 通信層の抽象化

**目的**

- `SboCli` 上位ロジックから transport 実装差分を切り離す

**作業**

- `CUraraSockTCPSBO` 依存箇所を整理
- `connect / destroy / send / recv callback` 相当の最小インターフェースを定義
- デスクトップ実装は既存 `UraraSock` ラッパーとして残す

**完了基準**

- 上位ゲームコードが「TCP 実装」ではなく「transport interface」を使う形になる

### Phase 2: サーバー側の端末識別抽象化

**目的**

- MAC 専用設計をやめ、Web 版識別子を受け入れられるようにする

**作業**

- `ClientIdentity` 構造を追加
- `InfoAccount` / ログイン処理 / BAN 補助処理の MAC 依存箇所を洗い出す
- `MAC` と `clientId` の両方を保持できる内部設計へ変更

**完了基準**

- サーバー内部で「端末識別 = MAC だけ」という前提が消える

### Phase 3: `LOGIN2` パケット導入

**目的**

- Web 版専用のログイン入口を追加する

**作業**

- `PacketCONNECT_REQ_LOGIN2`
- 必要なら `PacketCONNECT_RES_LOGIN2`
- `clientKind`
- `clientId`
- 将来用 `resumeToken` 領域

**完了基準**

- サーバーが `LOGIN` と `LOGIN2` を両受理できる

### Phase 4: WebSocket ゲートウェイ試作

**目的**

- Web クライアントと既存 `SboSvr` を実際につなぐ

**作業**

- `SboWsGate` の新規作成
- 1 WebSocket = 1 backend TCP の中継
- `wss://` 前提の運用構成を用意
- `LOGIN2` の通過または必要最小限の変換

**完了基準**

- テストクライアントから VPS 上の `SboSvr` へログインできる

### Phase 5: Web クライアント transport 実装

**目的**

- WebAssembly 版から実際にゲームサーバーへ接続できるようにする

**作業**

- `WebSocketTransport` 実装
- 接続/切断/再接続
- バイナリ送受信
- keepalive の WebSocket 対応
- `clientId` のブラウザ保存

**完了基準**

- Web クライアントが `VERSION -> LOGIN2 -> PLAY` を通過できる

### Phase 6: ゲーム成立性の検証

**目的**

- ログインだけではなく、実際に遊べるかを確認する

**作業**

- マップ遷移
- チャット
- 移動同期
- 他人表示
- アイテム操作
- 切断/復帰
- 高遅延環境での Dead Reckoning の体感確認

**完了基準**

- デスクトップ版と比べて致命的差異なくプレイできる

### Phase 7: 運用向け制御の追加

**目的**

- Web 版でも軽い多重対策と BAN 補助を成立させる

**作業**

- `clientId` ベース制限
- `accountId + clientId + IP` の複合監視
- 同時接続ポリシー
- Web 用ログ出力整備

**完了基準**

- MAC がなくても運用上困らない最低限の制御がある

### Phase 8: WebSocket 受付の本体統合検討

**目的**

- ゲートウェイ方式のまま続けるか、本体統合するか判断する

**作業**

- ゲートウェイ運用コストの評価
- 本体統合時の改修量評価
- 必要なら `SboSvr` 直 WebSocket 化の設計

**完了基準**

- 長期運用アーキテクチャが決まる

---

## 7. 実作業の推奨順序

最初の着手順は以下を推奨する。

1. `SboCli` 通信層の抽象化
2. `SboSvr` 側 `ClientIdentity` 導入
3. `LOGIN2` パケット追加
4. 簡易 WebSocket ゲートウェイ作成
5. Web クライアント側 `clientId` + `WebSocketTransport`

理由:

- 先に transport 抽象を作らないと Web 側実装で既存コードへ無理が出る
- `clientId` を受ける入口が先にないと Web 側ログイン実装が固まらない
- ゲートウェイはゲームロジックより前に単独で疎通試験できる

---

## 8. リスクと対策

| リスク | 対策 |
|---|---|
| ブラウザでは MAC が取れず既存多重対策が崩れる | `clientId + IP + accountId` の複合制御へ移行 |
| WebSocket 化で遅延が悪化する | Dead Reckoning と `POS_SYNC` を前提に体感検証する |
| ゲートウェイが単なる障害点になる | 初期は責務を最小化し、1対1中継に徹する |
| `SboSvr` に Web 専用分岐が増えすぎる | `LOGIN2` と `ClientIdentity` だけに局所化する |
| 旧デスクトップ互換が壊れる | `LOGIN` は当面維持し、段階移行する |
| `clientId` が消されて回避される | 軽い抑止と割り切り、BAN の主軸は accountId も併用する |
| `ws://` / `wss://` 運用で証明書周りが詰まる | 早期に VPS 実機で `wss://` 構成を検証する |

---

## 9. 受け入れ基準

### A. 互換性

- デスクトップ版は従来通り `SboSvr` に接続できる
- Web 版は `LOGIN2` により接続できる

### B. 識別

- デスクトップ版は MAC 識別を維持できる
- Web 版は `clientId` を安定送信できる
- サーバーは両者を同じ運用ルール上で扱える

### C. 通信

- Web クライアントは VPS 上の `SboSvr` とログイン、マップ遷移、移動、チャットができる
- 重大な同期破綻がない

### D. 運用

- 問題ユーザーを `accountId` または `clientId` ベースで追跡できる
- 同一端末からの軽い多重利用抑止ができる

---

## 10. まとめ

結論として、

- **WebSocket 化でゲーム成立性は十分狙える**
- **最大の差分は MAC アドレスではなく端末識別設計の見直し**
- **移行初期はゲートウェイ方式 + `clientId` 導入 + `LOGIN2` 両対応が最も安全**

である。

今後は本計画に沿って、まず `通信層抽象化` と `サーバー側の識別抽象化` から着手する。
