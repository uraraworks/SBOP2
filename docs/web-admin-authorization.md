# Web管理画面 認可ルール棚卸し

最終更新: 2026-04-24

現状の Web 管理画面 (`SboSvr/src/Web/`) で各 API エンドポイントがどの認可レベルを要求しているかの棚卸し。実装の一次ソースと合わせて記載する。

## 一次ソース

- 認証本体: `SboSvr/src/Web/AuthProvider.cpp`
  - `Authenticate(request, pMgrData, outContext)` が `Cookie: SESSID=...` を解釈し、`CLibInfoAccount::GetPtr(sessionId)` で `CInfoAccount` を引いて `AuthContext` を作る。
  - `AuthContext` には `loginId` / `accountId` / `adminLevel` / `roles` が入る。
  - 戻り値は `AuthStatusOk` / `AuthStatusMissingSession` / `AuthStatusBackendUnavailable` / `AuthStatusAccountNotFound`。
- ロール解決: `SboSvr/src/Web/Handlers/AdminRolesHandler.cpp`
  - `AdminRoleCatalog::ResolveRoles(nAdminLevel)` が `adminLevel` (0 / `ADMINLEVEL_ALL`) をロール ID 集合に展開する。
  - ロール一覧は同ファイル先頭の `g_roles` テーブル。
- ロール判定: `AuthProvider::HasRole(ctx, roleId)`。
- ルータ: `SboSvr/src/Web/ApiRouter.cpp` / `SboSvr/src/Web/HttpServer.cpp`。ルータ側では認可を行わず、各ハンドラ内で個別に `Authenticate` → `HasRole` を呼ぶ構造。
- `/ws/admin` WebSocket: `CHttpServer::HandleAdminWsUpgrade()` でアップグレード前に `Authenticate` を呼ぶ。未認証は 401 でハンドシェイクさせない。ロール判定はなし。
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
- `Role:<ID>`: `AuthStatusOk` かつ `HasRole(ctx, <ID>)` を要求
- `AdminPacket`: `adminLevel != NONE` をネイティブ側で要求

### 公開 / ログイン不要

| Method | Path                                 | 認可        | ハンドラ                    |
| ------ | ------------------------------------ | ----------- | --------------------------- |
| GET    | `/health`                            | Public      | `CHealthHandler`            |
| GET    | `/`（静的 HTML / JS / CSS など）     | Public      | `CStaticFileHandler`        |
| GET    | `/api/assets/map-parts/sheets/...`   | Public      | `CMapPartsSheetHandler`     |

### ロールで制限しているエンドポイント

| Method | Path                  | 必要ロール        | ハンドラ                          |
| ------ | --------------------- | ----------------- | --------------------------------- |
| GET    | `/api/server`         | `SERVER_VIEW`     | `CServerInfoHandler`              |
| POST   | `/api/accounts`       | `ACCOUNT_CREATE`  | `CAccountCreateHandler`           |
| GET    | `/api/admin/roles`    | LoginOnly         | `CAdminRolesListHandler`          |
| PUT    | `/api/admin/roles`    | `ROLE_UPDATE`     | `CAdminRolesUpdateHandler`        |
| GET    | `/api/audit-logs`     | `"admin"`（※1）  | `CAuditLogListHandler`            |

※1: `AuditLogHandler.cpp:99` は `HasRole(authContext, "admin")` と比較しているが、`g_roles[]` に `"admin"` ロールは登録されていない。結果として `ResolveRoles` が返すどのロール ID とも一致せず、**現状このエンドポイントは誰からも 403 になる**（下記 TODO 参照）。

### ログイン済みなら誰でも通るエンドポイント

以下はすべて `Authenticate` の `AuthStatusOk` のみで通過する。`adminLevel` の判定はなく、`ADMINLEVEL_NONE` の一般ユーザーでも叩ける。

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
| `/ws/admin` | LoginOnly（ハンドシェイク前判定） | `CHttpServer::HandleAdminWsUpgrade` → `CAdminWsHub`    |

### ネイティブ ADMIN パケット（参考）

`SboCliAdminMfc` 凍結後も SboCli 本体 (`StateProcMAP` の管理者 pick 送信) やレガシークライアントから来る可能性がある経路。

| Command                                  | 認可          | 実装                                                     |
| ---------------------------------------- | ------------- | -------------------------------------------------------- |
| `SBOCOMMANDID_SUB_ADMIN_*` (全体)       | `AdminPacket` | `MainFrameRecvProcADMIN.cpp:44` で `ADMINLEVEL_NONE` なら切断 |

## 現状の抜け穴 / TODO

1. **大半の変更系 API がロール未チェック**
   ログインさえ通れば一般アカウントでも、マップ / アイテム / キャラクター / スキル / エフェクト系の `POST/PUT/DELETE` を全て実行できてしまう。
   実運用前に最低でも以下を決める必要がある:
   - データ変更系 (`POST/PUT/DELETE /api/...`) は `SERVER_ADMIN` など単一ロールを必須にするのか
   - 参照系 (`GET`) はログイン済みなら許可で良いのか
   - ロールを領域別（`MAP_EDIT` / `ITEM_EDIT` / `CHARACTER_EDIT` 等）に細分化するか
2. **`/api/audit-logs` が事実上到達不能**
   `AuditLogHandler.cpp` が要求している `"admin"` ロールは `g_roles[]` に定義されておらず、`ResolveRoles` が返さない。
   正しいロール ID（例: `SERVER_ADMIN` か、新規に `AUDIT_VIEW` を追加）に修正する必要がある。
3. **`/api/selection/pick` のロール要件が未決**
   `SelectionHandler.cpp:91` に TODO コメントあり。管理画面での pick 操作は本来 `MAP_OBJECT_VIEW` / `SERVER_ADMIN` 相当に限定すべき。
4. **`adminLevel` が 2 値しかない**
   `ADMINLEVEL_NONE` と `ADMINLEVEL_ALL` のみ。細粒度のロール運用をする場合、`CInfoAccount` 側の保存スキーマ拡張か、`adminLevel` とは別にロールフラグを持つ必要がある。
5. **`SERVER_ADMIN` ロールはどのハンドラからも要求されていない**
   `g_roles[]` には定義されているが現状デッドロール。要否を決めて使うか削るか整理する。
6. **`/ws/admin` WebSocket にロール判定なし**
   ハンドシェイクは SESSID 認証のみ。`selection_changed` は全購読者に流れるため、一般アカウントが WS を張れば他セッションの選択操作を覗き見できる。
7. **失敗リクエストの監査ログ未記録**
   `AuditLog::Record` は 2xx のみ記録。不正な認可エラー (401/403) や 4xx/5xx は残らない。攻撃検知用途では不足。
