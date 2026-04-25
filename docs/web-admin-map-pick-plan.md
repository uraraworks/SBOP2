# Web管理画面 マップウィンドウ + pick 基盤 詳細設計メモ

最終更新: 2026-04-24

## 位置づけ

この文書は、`web-admin-map-plan.md` の優先順 1〜2 番に相当する  
**マップビュー共通コンポーネント** と **pick / selection API** の詳細設計だけを扱います。  
全体の移植対象一覧・進捗確認は以下の正本を参照してください。

- [web-admin-migration-inventory.md](./web-admin-migration-inventory.md)
- [web-admin-migration-status.md](./web-admin-migration-status.md)

---

## スコープ

3 層に分けて設計・実装する。

| 層 | 担当 | 概要 |
|---|---|---|
| 1 | webroot | 左側に Web クライアントを常設し、右側をツール / 編集パネルとして切り替える |
| 2 | SboSvr | pick REST API + 選択状態モデル |
| 3 | ゲームクライアント | Web クライアントから親管理画面への直接 pick 通知、必要に応じて selection API 連携 |

---

## 決定事項

### 通信方式

- 管理画面に埋め込んだ Web クライアントからの同一ブラウザ内連携は **`postMessage` を優先**
  - サーバー往復なしで右側編集パネルへ反映できる
  - 現在はキャラクター選択のみ実装済み
  - マップセル / 配置物 / イベント選択は同じ通知形に拡張する
- 管理画面への push は **WebSocket を採用**
- 既存の `SboSvr/src/Web/WebSocketBridge.cpp` はゲーム TCP のブリッジ専用だが、  
  SHA-1 / Base64 / フレーム処理の実装は流用可能
- 新規に `/ws/admin` エンドポイントを追加する（バイナリ中継層とは分離）
- **複数管理者同時接続前提**。将来の他管理者カーソル / 選択表示等の拡張余地を残す
- ポーリング・SSE は採用しない
  - 複数人同時編集で push が実質必須
  - 既存 WS 実装資産を活かせる

### pick REST API（たたき台）

| メソッド | パス | 用途 |
|---|---|---|
| `POST` | `/api/selection/pick` | 対象を選択 |
| `GET` | `/api/selection` | 現在の選択状態取得 |
| `DELETE` | `/api/selection` | 選択解除 |

**選択対象型**

```json
{
  "type": "map_cell" | "char" | "placement" | "event",
  "mapId": number,
  "x": number,
  "y": number,
  "refId": number   // 省略可。char ID / placement ID / event ID 等
}
```

- 選択状態のキー: **管理者セッション ID 単位**（セッションをまたがない）
- WS メッセージ例: `{ "kind": "selection_changed", "sessionId": "...", "payload": { ... } }`  
  （詳細スキーマは未決、下記参照）

### 実装順序

1. 左側に Web クライアント iframe を常設し、右側を編集パネルとして使うレイアウトへ変更 ✅ 実装済み
2. `/game/sbocli-title.html` を `SboSvr` から静的配信し、Debug / Release webroot へ同期 ✅ 実装済み
3. pick REST API（WS なしでも単発操作が動く基盤を先に固める）✅ 実装済み（6313b5d）
4. 管理画面向け WebSocket チャネル新設（`/ws/admin`）✅ 実装済み — `WebSocketProtocol.h/cpp`, `AdminWsHub.h/cpp`, `HttpServer` Upgrade 分岐, `CSelectionStore` ChangeCallback
5. Web クライアントからキャラクタークリックを `postMessage` で通知 ✅ 実装済み
6. キャラクター編集画面で、通知された charId の詳細を自動取得 ✅ 実装済み
7. ゲーム画面クリックを map_cell / placement / event 選択へ拡張（クリック通知: ✅ 実装済み / ゲーム画面内ハイライト: ✅ 実装済み）
8. 選択対象に応じて右側編集パネルを切り替え

### 完了条件

（`web-admin-migration-status.md` から再掲）

- マップを見ながら対象タイルや配置物を選べる
- 選択結果でマップ情報 / イベント / 配置物を編集できる
- ゲーム画面が Web / ネイティブどちらでも同じ API で連携できる

---

## 実装済みメモ（2026-04-24）

- 管理画面レイアウトを、左側 Web クライアント常設 / 右側編集パネル構成へ変更。
- `SboSvr` の `/game/` 静的配信で `out/browser-title/sbocli-title.*` を読み込めるようにした。
- `tools/build-sbocli-browser-title.ps1` でブラウザ版クライアントを `SboSvr/Debug/webroot/game` と `SboSvr/Release/webroot/game` へ同期する導線を追加。
- `StateProcMAP.cpp` に Emscripten 専用の `SBOP2_PostAdminCharPick()` を追加。
- キャラクタークリック判定は、実際の描画位置に合わせて `GetDrawMapPos()`、`GetViewCharPos()`、スクロール位置、モーション描画補正を使う。
- `app.js` は `sbop2_admin_char_pick` を受け取り、`character-overview` を開いて `fetchCharacterDetail(charId)` を呼ぶ。

---

## 未決事項

- **マップウィンドウ画面の置き場所と他画面との関係**
  - 独立した `map-window` 中心ではなく、左側の Web クライアントを常設する方針に変更済み。
  - 右側編集パネルを現在の選択対象に応じて切り替える。
  - 既存 map-objects 画面は当面そのまま残し、ゲーム画面連携の対象を段階的に増やす。

- **WebSocket メッセージスキーマ詳細**  
  選択更新以外（ホバー・カーソル同期・エラー通知 等）に何を流すか未定。

- **管理者セッションの識別方法**  
  既存 Web API でのセッション管理の仕組みを要確認。  
  参照先: `SboSvr/src/Web/HttpServer.cpp` の Cookie / トークン実装。

- **ゲームクライアント側の分岐場所**
  キャラクター選択は `CStateProcMAP::OnLButtonDown()` に実装済み。
  マップセル / 配置物 / イベント選択も同じクリック入口で対象種別を判定する方針。

- **認可（管理者ロール）とのすり合わせ**  
  `POST /api/selection/pick` を一般ユーザーが呼んだ場合の扱い未定。

---

## 参照ファイル

| ファイル | 用途 |
|---|---|
| `SboSvr/src/Web/WebSocketBridge.cpp` | WS フレーム処理の流用元 |
| `SboSvr/src/Web/HttpServer.cpp` | REST ルーティング追加先 |
| `SboSvr/src/Web/Handlers/` | 既存ハンドラ置き場（新ハンドラの参考） |
| `SboSvr/webroot/app.js` | フロント側の画面切り替え・API 呼び出し |
| `SboSvr/webroot/index.html` | マップ系 UI の既存実装 |

---

## 方針変更の経緯（2026-04-22）

当初「既存 3 画面（map-parts / map-object-templates / map-objects）のマップ描画を共通コンポーネントへ抽出する」方針だったが、実コードを確認した結果:

- map-parts はパーツギャラリーで、マップ座標系ではない
- map-object-templates はテーブル表示のみで Canvas 描画を持たない
- map-objects のみがマップグリッドを描画している（`renderMapPreview()` @ `SboSvr/webroot/app.js:967`）
- 3 画面で真に共通するのは `grpId → sheet/tileX/tileY` のスプライト座標計算だけ

そのため共通化先行ではなく、**pick 連携の主役となる `map-window` 画面を新設** し、必要になった時点で小さくヘルパーを抽出する方針に変更した。

また、各画面の実装自体がまだ中途半端な段階なので、先行した共通化リファクタはコストに見合わない。
