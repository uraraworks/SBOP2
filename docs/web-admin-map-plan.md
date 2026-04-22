# Web管理画面 マップ系計画

最終更新: 2026-04-22

## 位置づけ

この文書は、Web 管理画面のうち **マップ系機能だけを追うための短い実行メモ** です。  
全体の移植対象一覧と進捗確認は、以下の文書を正本とします。

- [web-admin-migration-inventory.md](./web-admin-migration-inventory.md)
- [web-admin-migration-status.md](./web-admin-migration-status.md)

## 参照元

- ネイティブ版メニュー: `SboCliAdminMfc/AdminResources.rc2`
- ネイティブ版ダイアログ: `SboCliAdminMfc/src/Migrated/Dialog/`
- 現行 Web UI: `SboSvr/webroot/index.html`, `SboSvr/webroot/app.js`
- 現行 Web API: `SboSvr/src/Web/HttpServer.cpp`, `SboSvr/src/Web/Handlers/`

## マップ系の現状

### Web で実用段階

- マップパーツ編集
  - UI: `map-parts`
  - API: `GET/PUT/POST/DELETE /api/maps/parts`
- マップオブジェクト定義
  - UI: `map-object-templates`
  - API: `GET/POST/PUT/DELETE /api/maps/objects/templates`
- マップオブジェクト配置
  - UI: `map-objects`
  - API: `POST/PUT/DELETE /api/maps/placements`

### UI プレースホルダーあり

- マップ情報編集
  - UI: `map-info`
  - API: 未実装
- マップイベント一覧
  - UI: `map-events`
  - API: 未実装

### まだ Web 側に受け皿がないもの

- マップ影編集
- マップ追加
- マップウィンドウ
- クリック対象選択ブリッジ
  - ゲーム画面クリックからキャラ、イベント、配置物、座標を管理画面へ渡す仕組み

## 当面の優先順

1. マップウィンドウ相当の Web 表示基盤を作る
2. `pick` / `selection` API を作り、Web / ネイティブ両方のゲーム画面から使えるようにする
3. マップ情報編集を実装する
4. マップイベント編集を実装する
5. マップ影編集を実装する
6. 必要ならマップ追加を実装する

## 方針メモ

- 旧 MFC の「別ウィンドウ通知」をそのまま再現するのではなく、サーバー側に選択状態を持たせる
- ゲーム画面が Web / ネイティブのどちらでも、クリック結果は API 経由でサーバーへ送る
- 管理画面はサーバー上の選択状態を見る
- マップイベント、NPC 配置、オブジェクト配置などは「同じ pick 基盤」の上に載せる

## 完了条件

- マップを見ながら対象タイルや配置物を選べる
- その選択結果を使ってマップ情報、イベント、配置物を編集できる
- ゲーム画面が Web / ネイティブどちらでも同じサーバー API で連携できる
