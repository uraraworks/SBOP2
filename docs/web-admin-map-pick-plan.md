# Web管理画面 マップウィンドウ + pick 基盤 詳細設計メモ

最終更新: 2026-04-22

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
| 1 | webroot | マップウィンドウ画面新設 + 描画ヘルパー抽出 |
| 2 | SboSvr | pick REST API + 選択状態モデル |
| 3 | ゲームクライアント | ネイティブ / Web 両クライアントからの pick 送信 |

---

## 決定事項

### 通信方式

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

1. マップウィンドウ画面 `map-window` を webroot に新設（map-objects の `renderMapPreview()` を土台に、ピック対象種別の切替を想定した作り）
2. `grpId → sheet/tileX/tileY` 変換と sheet URL 取得をヘルパー関数に抽出（map-window の実装中に必要になった箇所だけ）
3. pick REST API（WS なしでも単発操作が動く基盤を先に固める）
4. 管理画面向け WebSocket チャネル新設（`/ws/admin`）
5. マップウィンドウから WS 購読・pick 連動
6. ゲーム画面側（ネイティブ / Web）からの pick 送信対応

### 完了条件

（`web-admin-migration-status.md` から再掲）

- マップを見ながら対象タイルや配置物を選べる
- 選択結果でマップ情報 / イベント / 配置物を編集できる
- ゲーム画面が Web / ネイティブどちらでも同じ API で連携できる

---

## 未決事項

- **マップウィンドウ画面の置き場所と他画面との関係**
  - 候補: webroot/app.js に `renderMapWindow` 的な関数を追加、もしくはファイル分割
  - 既存 map-objects 画面は当面そのまま残し、map-window とは独立運用する
  - 現状 webroot は単一 app.js (約 2900 行) に集約されている

- **WebSocket メッセージスキーマ詳細**  
  選択更新以外（ホバー・カーソル同期・エラー通知 等）に何を流すか未定。

- **管理者セッションの識別方法**  
  既存 Web API でのセッション管理の仕組みを要確認。  
  参照先: `SboSvr/src/Web/HttpServer.cpp` の Cookie / トークン実装。

- **ゲームクライアント側の分岐場所**  
  「管理者モード時のみ pick 送信」をどのクラス / タイミングに入れるか未定。  
  ネイティブは MFC レス SDL2 ベース、Web は Emscripten ビルド。

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
