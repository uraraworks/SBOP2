# Web管理画面 進捗確認

最終更新: 2026-04-24（管理画面への Web クライアント埋め込みとキャラクタークリック連携を反映）

## 使い方

- 何を作るかは [web-admin-migration-inventory.md](./web-admin-migration-inventory.md) を見る
- どこまで進んだかはこの文書で確認する
- チェックは「コード上で UI と API の根拠が追える状態」を基準にする

## 現在地

### 稼働中

- サーバー情報ダッシュボード
- アカウント作成
- 管理者ロール設定
- Web クライアント常設レイアウト（左: ゲーム画面 / 右: 編集パネル）
- ゲーム画面クリックからキャラクター編集画面を開く連携
- マップパーツ編集
- マップオブジェクト定義編集
- マップオブジェクト配置編集

### UI プレースホルダーあり

- モーション種別管理
- スキル一覧・編集
- ~~マップイベント一覧~~（Wave 2B 実装済み）
- ~~アイテム種別一覧~~（Wave 2D 実装済み）
- ~~アイテム一覧~~（Wave 2D 実装済み）
- ~~武器一覧~~（Wave 2D 実装済み）
- ~~噴出し一覧~~（Wave 2D 実装済み）
- ~~エフェクト一覧~~（Wave 2D 実装済み）
- 初期ステータス設定

### まだ受け皿がない重要項目

- ゲーム画面上のマップパーツ / イベント配置編集オーバーレイ
- Web クライアント側からの map_cell / placement / event pick 連携
- 他管理者との同時編集カーソル / 競合表示

## Wave別チェック

## Wave 1: 運用基盤

- [x] サーバー情報ダッシュボード
- [x] アカウント作成
- [x] 管理者ロール設定
- [x] 認可ルールの棚卸しを文書へ反映（[web-admin-authorization.md](./web-admin-authorization.md)）

## Wave 2A: マップ基盤

- [x] マップパーツ CRUD API
- [x] マップパーツ編集 UI
- [x] マップオブジェクト定義 CRUD API
- [x] マップオブジェクト定義 UI
- [x] マップオブジェクト配置 CRUD API
- [x] マップオブジェクト配置 UI
- [ ] マップ表示の共通コンポーネント化
- [x] クリック対象選択 `pick` API
- [x] サーバー側の選択状態管理
- [x] 管理画面向け WebSocket `/ws/admin`（`CAdminWsHub` + `selection_changed` broadcast）
- [x] 管理画面 JS 側の WS 購読実装（次フェーズ）
- [x] ゲーム画面内の選択ハイライト表示（`MgrData::SetAdminPick` + `LayerMap::DrawAdminPick`）
- [x] Web / ネイティブ両対応の連携方式を文書化（[web-admin-native-coexistence.md](./web-admin-native-coexistence.md)）
- [x] Web クライアントを管理画面に埋め込み（`/game/sbocli-title.html`）
- [x] Debug / Release の `webroot/game` へブラウザ版クライアントを同期するビルド導線
- [x] Web クライアントから親管理画面へ `postMessage` でキャラクター選択を通知
- [x] キャラクター編集画面でゲーム内キャラクタークリック時に詳細を再取得
- [x] ゲーム画面クリックを map_cell / placement / event 選択にも拡張

## Wave 2B: マップ編集の残り

- [x] マップ情報 API
- [x] マップ情報 UI
- [x] マップイベント API（GET/POST/PUT/DELETE /api/maps/events、6種別対応）
- [x] マップイベント UI（マップ選択・一覧・編集・削除）
- [x] マップ影 API（GET/POST/PUT/DELETE /api/maps/shadows、影カタログ CRUD のみ）
- [x] マップ影 UI（影カタログ一覧・編集フォーム・アニメーションコマ編集）
- [x] マップ追加 API
- [x] マップ追加 UI

## Wave 2C: キャラクター管理 ✓ 完了

- [x] キャラクター検索 API
- [x] キャラクター一覧 UI
- [x] キャラクター詳細 API
- [x] キャラクター基本情報 UI
- [x] キャラクターステータス UI
- [x] キャラクター画像設定 UI
- [x] 所持アイテム編集 UI
- [x] 所持スキル編集 UI
- [x] アカウント紐付け / 拒否操作 API
- [x] NPC 追加 API
- [x] NPC 追加 UI

## Wave 2D: アイテム / エフェクト / マスタ

- [x] アイテム種別 API / UI
- [x] アイテム一覧 API / UI
- [x] 武器情報 API / UI
- [x] 噴出し API / UI（GET/POST/PUT/DELETE /api/efc-balloons、コマ単位 flat CRUD）
- [x] エフェクト API / UI（GET/POST/PUT/DELETE /api/effects、animes を nested 配列で扱う）
- [x] 初期ステータス API / UI（GET/PUT /api/initial-status、単一レコード編集フォーム）

## 共通コンポーネント

- [x] 画像 picker
- [x] アイテム picker
- [x] エフェクト picker
- [x] マップパーツ picker
- [x] マップ影 picker
- [x] 会話イベント editor
- [x] 差分確認 / 監査ログ共通表示（最小実用セット: in-memory リングバッファ + 一覧 UI）

## 次の実装順

1. ~~Web クライアントのクリック通知をキャラクター以外（マップセル / 配置 / イベント）へ拡張~~ （実装済み）
2. ~~右側編集パネルで、現在の選択対象に応じた編集 UI を自動切り替え~~ （実装済み: `handleAdminGamePick` で char/item/cell に応じて自動ナビゲーション）
3. ~~Web クライアント内に管理者向け選択ハイライト / オーバーレイを表示~~ （実装済み: `MgrData` に AdminPick 状態を追加、`LayerMap::DrawAdminPick` でセル黄色枠・キャラ赤枠を描画）

## 保留メモ

- 旧 MFC の「選択キャラ」「選択マップ座標」は、管理画面内に埋め込んだ Web クライアントから
  `postMessage` で直接 Web 管理画面へ通知する経路を優先する。サーバー側 selection API / WS は、
  複数管理者間同期やネイティブ管理者との連携が必要な場面で併用する。
- 直接連携は char / placement / cell の3種対応済み。`StateProcMAP.cpp` で `sbop2_admin_pick`
  メッセージ（mapId / cellX / cellY / charId / itemId を含む）を親画面へ送信する。
  後方互換のため旧 `sbop2_admin_char_pick` メッセージも `handleAdminGameMessage` で受信できる。
- 管理画面側は `app.js` の `handleAdminGamePick()` で優先度 char > item > cell に応じて
  自動ナビゲーション（character-overview / map-objects / map-info）を行う。
- `IDM_DEBUG_*` は恒久 UI ではなく、必要なら開発者向けツールとして別枠に分離する
- `DlgAdminTalkEvent*` の Web 版（会話イベント editor）は実装済み。共通コンポーネントとして
  `openTalkEventEditor({ talkEventId, onSaved })` で呼び出し可能。宣言的には
  `<button data-talk-event="<id>">` または `data-talk-event-target="<input id>"` でも開ける。
  扱うコマンド種別: NONE / PAGE / MSG / MENU / ADDSKILL。
- 会話イベント editor の残課題（TODO コメントは app.js に記載）:
  - ADDSKILL でのスキル picker 連携（現状は ID 直接入力）
  - PAGE 条件「アイテムあり/なし」の data 欄にアイテム picker を差し込む
  - MSG テキストのプレビュー / 改行コード整形
  - ページ単位でのグルーピング表示（現状は events[] 線形リスト）
  - 行の Undo / 変更差分表示
  - コミット前バリデーション（MENU のジャンプ先ページが存在するか等）
  - ネイティブ側への通知（PacketADMIN_CHAR_RENEW_TALKEVENT 経由）が未実装。
    現状は `CLibInfoTalkEvent` の in-memory を差し替えるだけで、クライアントへの
    リアルタイム反映は次回保存時まで待たせている。必要なら同パケット相当の
    ブロードキャストを追加する。
- 監査ログ（operation-history ビュー / GET /api/audit-logs）の残課題:
  - 現状ストレージは in-memory リングバッファ（最大 500 件）。サーバー再起動で消える。
    長期保管が必要なら SQLite or 外部ログ基盤への書き出しを追加する。
  - 記録しているのは変更系リクエストの「リクエストボディの要約（最大 256 文字）」のみ。
    before/after の full JSON diff 表示には、各ハンドラ側で対象レコードの事前 snapshot を
    添えて Record() を呼ぶ拡張が必要。
  - 4xx/5xx の失敗リクエスト記録は現状オフ。必要なら `AuditLog::Record` の 2xx 判定を
    緩めてフィルタで切り替え可能にする。
  - CSV / JSON エクスポート、日付範囲フィルタ、ページング未対応。
  - /api/selection 系は大量に飛ぶためフィルタ除外済み（AuditLog::ShouldAudit 参照）。
