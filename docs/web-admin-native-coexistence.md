# Web管理画面 / ネイティブクライアント 共存連携方式

最終更新: 2026-04-24

Web 管理画面 (`SboSvr/webroot/` + `/api/...`, `/ws/admin`) とネイティブクライアント (`SboCli`, 旧来のゲームクライアント) が同一サーバーに同時接続したときに、選択状態とデータ変更通知をどう同期するかを整理する。

## 登場人物

- **Web 管理画面** — `SboSvr/webroot/app.js` / `CHttpServer` 経由。
  - 認証は Cookie `SESSID`（セッション ID ＝ `CInfoAccount` の参照キー）。
  - REST API `/api/...` と WebSocket `/ws/admin` で会話する。
- **ネイティブクライアント** — `SboCli` (SDL 化済み) および旧クライアント。
  - TCP ソケット経由で独自バイナリプロトコル（`Common/Packet/` 配下）を使う。
  - 管理者用パケットは `SBOCOMMANDID_SUB_ADMIN_*`。
- **選択ストア** — `CSelectionStore` シングルトン (`SelectionHandler.cpp`)。
  - キーは「セッション ID」文字列。Web は `authContext.sessionId`、ネイティブは固定キー `"game"`。
- **WS Hub** — `CAdminWsHub` シングルトン (`AdminWsHub.cpp`)。
  - `/ws/admin` で接続した管理画面へ JSON をブロードキャスト。
- **ゲームソケット** — `CUraraSockTCPSBO` (`m_pMgrData->GetSock()`)。
  - `SendTo(0, &packet)` で全ネイティブクライアントへブロードキャスト。

## サーバー側の選択状態の持ち方

`CSelectionStore` は `std::map<std::string sessionId, Selection>`。

- **Web 管理画面**: `/api/selection/pick` ハンドラが `authContext.sessionId` (Cookie の SESSID) をキーに `Set/Get/Clear`。管理者が複数同時にログインしても、それぞれ独立した選択を保持する。
- **ネイティブ管理者**: `MainFrameRecvProcADMIN_MAP_SELECTPICK / _SELECTCLEAR` が固定キー `"game"` で `Set/Clear` する。ネイティブ側は複数同時の選択保持を想定していない（現状ゲームクライアント 1 インスタンス前提）。
- **変更時コールバック**: `CSelectionStore::SetChangeCallback` を `CHttpServer::RegisterDefaultHandlers` で登録し、`Set/Clear` の度に `CAdminWsHub::BroadcastSelectionChanged(sessionId, pSel)` を呼ぶ。Web 起点でもネイティブ起点でも同じ経路。

```
               ┌──────────────────────┐
Web管理画面 ───│ POST /api/selection  │── CSelectionStore.Set(webSess, sel)
               └──────────────────────┘              │
                                                     │ ChangeCallback
                        ┌────────────────────────────┘
                        ▼
               ┌──────────────────┐    selection_changed (JSON)
               │  CAdminWsHub     │ ─────────────────────────────▶ 全 /ws/admin 接続
               └──────────────────┘
                        ▲
                        │ ChangeCallback
                        │
ネイティブ管理者 ──ADMIN_MAP_SELECTPICK──▶ CSelectionStore.Set("game", sel)
```

## どの操作が誰に届くか

### 選択操作 (pick / clear)

| 発生元                      | 格納キー             | 届く先                                           |
| --------------------------- | -------------------- | ------------------------------------------------ |
| Web `/api/selection/pick`   | 呼び出した管理者 SESSID | 全 `/ws/admin` 接続（自分も含む）。ネイティブへは送らない。|
| ネイティブ `ADMIN_MAP_SELECTPICK` | 固定キー `"game"`    | 全 `/ws/admin` 接続。ネイティブ自身には返送しない（片方向）。|

つまり選択は「Web ↔ Web」「ネイティブ → Web」の 2 方向。現状 **Web → ネイティブ** の逆方向は実装されていない（ゲーム画面側に Web の選択を反映する要件がない）。

### データ変更（マップ / アイテム / エフェクト 等）

Web 管理画面の変更系 API が成功した際、多くのハンドラはネイティブ用の差分パケットを組み立てて `pSock->SendTo(0, &packet)` で **全ネイティブクライアントに** ブロードキャストする。Web 画面自体には REST のレスポンスと監査ログ (`AuditLog::Record`) で反映が伝わる。

代表的な通知パケット:

| 対象             | Web ハンドラ                         | 通知パケット                                   | 送信先             |
| ---------------- | ------------------------------------ | ---------------------------------------------- | ------------------ |
| 武器情報         | `CWeaponCreate/UpdateHandler`        | `CPacketITEM_RES_ITEMWEAPONINFO`               | 全ネイティブ       |
| 武器削除         | `CWeaponDeleteHandler`               | （未実装：再接続時の全件同期に依存）           | ―                  |
| アイテム種別     | `CItemType*Handler`                  | `CPacketITEM_RES_ITEMINFO` 系                  | 全ネイティブ       |
| アイテム削除     | `CItemDeleteHandler`                 | `CPacketITEM_DELETEITEMINFO`                   | 全ネイティブ       |
| マップパーツ     | `CMapParts*Handler`                  | マップパーツ更新パケット                       | 全ネイティブ       |
| マップオブジェクト | `CMapObject*Handler`               | マップオブジェクト更新パケット                 | 全ネイティブ       |
| マップ影         | `CMapShadow*Handler`                 | マップ影更新パケット                           | 全ネイティブ       |
| マップイベント   | `CMapEvent*Handler`                  | マップイベント更新パケット                     | 全ネイティブ       |
| 噴出し           | `CEfcBalloon*Handler`                | 噴出し更新パケット                             | 全ネイティブ       |
| エフェクト       | `CEfc*Handler`                       | エフェクト更新パケット                         | 全ネイティブ       |
| 初期ステータス   | `CInitialStatusUpdateHandler`        | `CPacketSYSTEM_INFO`                           | 全ネイティブ       |
| 会話イベント     | `CTalkEvent*Handler`                 | （未実装：次回セッション時に反映）             | ―                  |

方針のまとめ:

- `RES_<カテゴリ>INFO` 系 — 「1 件分の最新状態を送る」更新・新規作成パケット。ネイティブ側はこれを受けてライブラリを上書き / 追加する。
- `DELETE<カテゴリ>INFO` 系 — 1 件削除の明示通知。対応パケットが無い領域（武器・会話イベントなど）は、ネイティブ側の再接続時の全件同期に任せている。
- `SYSTEM_INFO` — 初期ステータスなどサーバー全体設定の一括反映に使う。

### WS ブロードキャストは Web 専用

`CAdminWsHub::BroadcastJson` は `/ws/admin` 購読者にしか配らない。Web 側のデータ変更を他の Web 画面にリアルタイム反映したい場合は、各ハンドラから `BroadcastJson` を直接呼ぶ必要がある（現在は `selection_changed` のみ）。

## シーケンス図

### A. Web 管理者が pick したとき（複数 Web + ネイティブ同時接続）

```
Web管理者A           サーバー (CHttpServer + Hub + Store)      Web管理者B     ネイティブ
   │                         │                                   │              │
   │ POST /api/selection/pick│                                   │              │
   │ (SESSID=A, body)        │                                   │              │
   ├────────────────────────▶│                                   │              │
   │                         │ Authenticate(A) -> AuthStatusOk   │              │
   │                         │ Store.Set("sessA", sel)           │              │
   │                         │   └─ChangeCallback                │              │
   │                         │      └─Hub.BroadcastSelectionChanged│            │
   │                         │          "sessA", sel             │              │
   │                         │ ───────────────────────────────▶  │              │
   │                         │ ───────────────────────────────── selection_changed│(送らない)
   │ 200 OK {selection:...}  │                                   │              │
   │◀────────────────────────│                                   │              │
```

### B. ネイティブ管理者が pick したとき

```
ネイティブ            サーバー (MainFrame + Store + Hub)         Web管理者
   │                         │                                       │
   │ PacketADMIN_MAP_SELECTPICK                                      │
   ├────────────────────────▶│                                       │
   │                         │ adminLevel != NONE を確認             │
   │                         │ Store.Set("game", sel)                │
   │                         │   └─ChangeCallback                    │
   │                         │      └─Hub.BroadcastSelectionChanged  │
   │                         │ ─────────────────────────────────────▶│ selection_changed
   │(返送なし)                │                                       │
```

### C. Web 管理者がマップパーツを更新したとき

```
Web管理者            CHttpServer / Handler            ネイティブ全接続    Web管理者(他)
   │                       │                                  │                │
   │ PUT /api/maps/parts   │                                  │                │
   ├──────────────────────▶│                                  │                │
   │                       │ 認証・ライブラリ更新             │                │
   │                       │ pSock->SendTo(0, &packet)        │                │
   │                       ├─────────────────────────────────▶│(全員) マップパーツ更新│
   │                       │ AuditLog.Record(...)             │                │
   │ 200 OK                │                                  │                │
   │◀──────────────────────│                                  │                │
   │                       │ (他 Web には WS 通知なし — 次操作時に GET で再取得)│
```

## 未対応 / 注意点

- **Web → ネイティブ pick 通知** は未実装。Web で選択してもゲーム画面には反映されない。必要なら `CAdminWsHub::BroadcastSelectionChanged` 経由ではなく、`pSock->SendTo` で ADMIN パケットを送る経路を追加する。
- **Web → 他 Web のデータ変更通知** も選択以外は未実装。各ハンドラで `CAdminWsHub::BroadcastJson` を呼んで `{kind:"<resource>_changed", ...}` を流せば購読 UI を更新できる。
- **ネイティブの選択キーが固定 `"game"`** のため、ネイティブ管理者が複数並列するケースは未対応。必要になれば `dwSessionID` を文字列化してキーに使う。
- **選択削除を伴う切断の後始末** は未対応。Web 管理者がブラウザを閉じても `Store` にはキーが残る。定期クリーンアップか、`/ws/admin` 切断時に `Clear` を呼ぶなどの追加が必要。
- **会話イベント / 武器削除** の RenewTalkEvent 系ブロードキャストは TODO（`migration-status.md` の保留メモ参照）。
