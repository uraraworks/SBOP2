# Web管理画面 認可ルール棚卸し

最終更新: 2026-04-28

現状の Web 管理画面 (`SboSvr/src/Web/`) で各 API エンドポイントがどの認可レベルを要求しているかの棚卸し。実装の一次ソースと合わせて記載する。

## 一次ソース

- 認証本体: `SboSvr/src/Web/AuthProvider.cpp`
  - `Authenticate(request, pMgrData, outContext)` が `Cookie: SESSID=...` を解釈し、`CLibInfoAccount::GetPtr(sessionId)` で `CInfoAccount` を引いて `AuthContext` を作る。
  - `AuthContext` には `loginId` / `accountId` / `adminLevel` / `roles` が入る。
  - 戻り値は `AuthStatusOk` / `AuthStatusMissingSession` / `AuthStatusBackendUnavailable` / `AuthStatusAccountNotFound`。
- 管理セッション API: `SboSvr/src/Web/Handlers/AuthSessionHandler.cpp`
  - `POST /api/auth/admin-login` がログイン ID / パスワードをサーバー側で再検証し、`ADMINLEVEL_NONE` より大きいアカウントにだけ `SESSID` Cookie を発行する。
  - `GET /api/auth/me` が現在の Cookie から認証状態と `adminLevel` / `roles` を返す。
  - `POST /api/auth/logout` が `SESSID` Cookie を期限切れにする。Web 画面ではヘッダーの「ログアウト」ボタンと `?logout=1` がこの API を使う。
- ロール解決: `SboSvr/src/Web/Handlers/AdminRolesHandler.cpp`
  - `AdminRoleCatalog::ResolveRoles(nAdminLevel)` が `adminLevel` (0 / `ADMINLEVEL_ALL`) をロール ID 集合に展開する。
  - ロール一覧は同ファイル先頭の `g_roles` テーブル。
- ロール判定: `AuthProvider::HasRole(ctx, roleId)`。
- ルータ: `SboSvr/src/Web/ApiRouter.cpp` / `SboSvr/src/Web/HttpServer.cpp`。
  - `HttpServer` は `/api/auth/*` を除く `/api/...` 全体に共通ガードをかける。未認証は 401、一般アカウント (`adminLevel <= ADMINLEVEL_NONE`) は 403。
  - その後、各ハンドラ内の個別 `Authenticate` / `HasRole` チェックが追加で走る。
- `/ws/admin` WebSocket: `CHttpServer::HandleAdminWsUpgrade()` でアップグレード前に `Authenticate` と `adminLevel > ADMINLEVEL_NONE` を確認する。未認証は 401、一般アカウントは 403 でハンドシェイクさせない。
- Web 入口: `SboSvr/webroot/index.html` / `app.js`
  - 一般プレイヤーと管理者で同じ URL を使う。初期状態はゲーム iframe のみ表示。
  - 管理者ログインに成功すると、ページを再読み込みせずに管理 UI を展開する。iframe 内のゲーム状態は維持する。
  - `?logout=1` またはヘッダーのログアウトボタンで管理セッションだけを破棄し、一般アカウントのテストに戻せる。
- ネイティブ管理パケット (ADMIN 系) 側: `SboSvr/src/MainFrame/MainFrameRecvProcADMIN.cpp` 先頭で `pInfoAccount->m_nAdminLevel == ADMINLEVEL_NONE` なら即切断。こちらは Web 側認可とは別経路。

## 認可レベルの定義

現状 `CInfoAccount::m_nAdminLevel` は 2 値しかない。

| adminLevel 値     | 意味                        | 持つロール                                                             |
| ----------------- | --------------------------- | ---------------------------------------------------------------------- |
| `ADMINLEVEL_NONE` | 一般ログイン済みアカウント  | `SERVER_VIEW`, `MAP_OBJECT_VIEW`                                       |
| `ADMINLEVEL_ALL`  | 管理者                      | `SERVER_ADMIN`, `ACCOUNT_CREATE`, `ROLE_UPDATE` + 上記 2 つ            |

ロール ID の定義と説明は `g_roles[]` (`AdminRolesHandler.cpp:20`):

| ロール ID         | 用途                                                        |
| ----------------- | ----------------------------------------------------------- |
| `SERVER_VIEW`     | サーバーダッシュボード閲覧                                  |
| `ACCOUNT_CREATE`  | 管理者アカウント新規作成                                    |
| `ROLE_UPDATE`     | 既存アカウントの admin ロール更新                           |
| `SERVER_ADMIN`    | サーバー制御操作（現時点でどのハンドラも直接要求していない）|
| `MAP_OBJECT_VIEW` | マップオブジェクト閲覧                                      |

`AdminRoleCatalog::DetermineAdminLevel(roles)` は `SERVER_ADMIN` / `ACCOUNT_CREATE` / `ROLE_UPDATE` のいずれかがあれば `ADMINLEVEL_ALL` に昇格させる（ロール編集 UI からの逆引き）。

## エンドポイント × 必要認可 一覧

凡例:

- `Public`: 認証なしで応答する（HTML / 静的アセット 等）
- `LoginOnly`: `AuthStatusOk` のみ要求（ロール判定なし）
- `AdminGate`: `HttpServer` 共通ガードで `AuthStatusOk` かつ `adminLevel > ADMINLEVEL_NONE` を要求
- `Role:<ID>`: `AuthStatusOk` かつ `HasRole(ctx, <ID>)` を要求
- `AdminPacket`: `adminLevel != NONE` をネイティブ側で要求

### 公開 / ログイン不要

| Method | Path                                 | 認可        | ハンドラ                    |
| ------ | ------------------------------------ | ----------- | --------------------------- |
| GET    | `/health`                            | Public      | `CHealthHandler`            |
| GET    | `/`（静的 HTML / JS / CSS など）     | Public      | `CStaticFileHandler`        |
| GET    | `/game/...`                          | Public      | `CStaticFileHandler`        |
| GET    | `/api/assets/map-parts/sheets/...`   | Public      | `CMapPartsSheetHandler`     |
| GET    | `/api/auth/me`                       | Public      | `CAuthMeHandler`            |
| POST   | `/api/auth/admin-login`              | Public      | `CAdminLoginHandler`        |
| POST   | `/api/auth/logout`                   | Public      | `CAuthLogoutHandler`        |

### 共通管理者ガード + ロールで制限しているエンドポイント

`/api/auth/*` を除く `/api/...` は、まず `HttpServer` 共通ガードで `AdminGate` を要求する。以下はその後に各ハンドラが追加で要求する条件。

| Method | Path                  | 必要ロール        | ハンドラ                          |
| ------ | --------------------- | ----------------- | --------------------------------- |
| GET    | `/api/server`         | AdminGate + `SERVER_VIEW`     | `CServerInfoHandler`              |
| POST   | `/api/accounts`       | AdminGate + `ACCOUNT_CREATE`  | `CAccountCreateHandler`           |
| GET    | `/api/admin/roles`    | AdminGate + LoginOnly         | `CAdminRolesListHandler`          |
| PUT    | `/api/admin/roles`    | AdminGate + `ROLE_UPDATE`     | `CAdminRolesUpdateHandler`        |
| GET    | `/api/audit-logs`     | AdminGate + `"admin"`（※1）  | `CAuditLogListHandler`            |

※1: `AuditLogHandler.cpp:99` は `HasRole(authContext, "admin")` と比較しているが、`g_roles[]` に `"admin"` ロールは登録されていない。結果として `ResolveRoles` が返すどのロール ID とも一致せず、**現状このエンドポイントは誰からも 403 になる**（下記 TODO 参照）。

### 管理者なら通るエンドポイント

以下は、個別ハンドラ側では `Authenticate` の `AuthStatusOk` のみで通過するが、`HttpServer` 共通ガードにより事前に `AdminGate` が必要。

| Method              | Path                                           | ハンドラ                          |
| ------------------- | ---------------------------------------------- | --------------------------------- |
| GET/POST/PUT/DELETE | `/api/maps`                                    | `CMapInfo*Handler`                |
| GET/POST/PUT/DELETE | `/api/maps/parts`                              | `CMapParts*Handler`               |
| GET                 | `/api/maps/objects`                            | `CMapObjectListHandler`           |
| GET/POST/PUT/DELETE | `/api/maps/objects/templates`                  | `CMapObjectTemplate*Handler`      |
| POST/PUT/DELETE     | `/api/maps/placements`                         | `CMapPlacement*Handler`           |
| GET/POST/PUT/DELETE | `/api/maps/events`                             | `CMapEvent*Handler`               |
| GET/POST/PUT/DELETE | `/api/maps/shadows`                            | `CMapShadow*Handler`              |
| GET/PUT/DELETE      | `/api/talk-events`                             | `CTalkEvent*Handler`              |
| GET                 | `/api/characters`                              | `CCharacterListHandler`           |
| GET                 | `/api/characters/{id}`                         | `CCharacterDetailHandler`         |
| PUT                 | `/api/characters/{id}[/status\|/equipment\|/graphics]` | `CCharacterUpdateHandler` |
| GET/POST/DELETE     | `/api/characters/{id}/items[/slot]`            | `CCharacterItemHandler`           |
| GET/POST/DELETE     | `/api/characters/{id}/skills[/slot]`           | `CCharacterItemHandler`           |
| GET/POST/PUT/DELETE | `/api/item-types`                              | `CItemType*Handler`               |
| GET/POST/PUT/DELETE | `/api/items`                                   | `CItem*Handler`                   |
| GET/POST/PUT/DELETE | `/api/weapons`                                 | `CWeapon*Handler`                 |
| GET/POST/PUT/DELETE | `/api/efc-balloons`                            | `CEfcBalloon*Handler`             |
| GET/POST/PUT/DELETE | `/api/effects`                                 | `CEfc*Handler`                    |
| GET/PUT             | `/api/initial-status`                          | `CInitialStatus*Handler`          |
| GET                 | `/api/image-categories`                        | `CImageCatalogHandler`            |
| POST                | `/api/selection/pick`                          | `CSelectionPickHandler`           |
| GET                 | `/api/selection`                               | `CSelectionGetHandler`            |
| DELETE              | `/api/selection`                               | `CSelectionDeleteHandler`         |

### WebSocket

| Path        | 認可                               | 処理                                                   |
| ----------- | ---------------------------------- | ------------------------------------------------------ |
| `/ws/admin` | AdminGate（ハンドシェイク前判定） | `CHttpServer::HandleAdminWsUpgrade` → `CAdminWsHub`    |

### ネイティブ ADMIN パケット（参考）

`SboCliAdminMfc` 凍結後も SboCli 本体 (`StateProcMAP` の管理者 pick 送信) やレガシークライアントから来る可能性がある経路。

| Command                                  | 認可          | 実装                                                     |
| ---------------------------------------- | ------------- | -------------------------------------------------------- |
| `SBOCOMMANDID_SUB_ADMIN_*` (全体)       | `AdminPacket` | `MainFrameRecvProcADMIN.cpp:44` で `ADMINLEVEL_NONE` なら切断 |

## 現状の抜け穴 / TODO

1. **細粒度ロールは未整理**
   `HttpServer` 共通ガードにより一般アカウントから `/api/...` と `/ws/admin` は塞いだが、マップ / アイテム / キャラクター / スキル / エフェクト系の個別ロールはまだ粗い。
   実運用前に、領域別ロール（`MAP_EDIT` / `ITEM_EDIT` / `CHARACTER_EDIT` 等）へ細分化するか、`SERVER_ADMIN` 単一で管理するかを決める必要がある。
2. **`/api/audit-logs` が事実上到達不能**
   `AuditLogHandler.cpp` が要求している `"admin"` ロールは `g_roles[]` に定義されておらず、`ResolveRoles` が返さない。
   正しいロール ID（例: `SERVER_ADMIN` か、新規に `AUDIT_VIEW` を追加）に修正する必要がある。
3. **`/api/selection/pick` のロール要件が未決**
   `SelectionHandler.cpp:91` に TODO コメントあり。管理画面での pick 操作は本来 `MAP_OBJECT_VIEW` / `SERVER_ADMIN` 相当に限定すべき。
4. **`adminLevel` が 2 値しかない**
   `ADMINLEVEL_NONE` と `ADMINLEVEL_ALL` のみ。細粒度のロール運用をする場合、`CInfoAccount` 側の保存スキーマ拡張か、`adminLevel` とは別にロールフラグを持つ必要がある。
5. **`SERVER_ADMIN` ロールはどのハンドラからも要求されていない**
   `g_roles[]` には定義されているが現状デッドロール。要否を決めて使うか削るか整理する。
6. **Web 管理セッションは簡易 Cookie**
   `SESSID` にはログイン ID を入れている。現状の既存 `AuthProvider` に合わせた最小実装であり、推測困難なランダムセッション ID、期限、CSRF 対策、Secure 属性などは今後の本番運用前に整理する。
7. **失敗リクエストの監査ログ未記録**
   `AuditLog::Record` は 2xx のみ記録。不正な認可エラー (401/403) や 4xx/5xx は残らない。攻撃検知用途では不足。
