# Web管理画面 - マップ関連機能 移行計画

## 概要

MFCデスクトップクライアント(SboCli)のマップ管理ダイアログをWeb管理画面に移行する。
これにより、クライアントからMFC依存を段階的に除去し、将来のSDLクロスプラットフォーム化の下地を作る。

## 現状

### 既にWeb実装済み (読み取り専用)

| 機能 | エンドポイント | 状態 |
|------|--------------|------|
| マップパーツ一覧 | `GET /api/maps/parts` | 閲覧のみ |
| スプライトシート画像 | `GET /api/assets/map-parts/sheets/{id}.png` | 動作中 |
| マップオブジェクト一覧 | `GET /api/maps/objects` | 閲覧のみ |

### MFCにのみ存在する機能

| 機能 | MFCダイアログ | 概要 |
|------|-------------|------|
| マップパーツ編集 | DlgAdminMapParts / Edit | パーツのCRUD、フラグ/アニメ設定 |
| マップオブジェクト管理 | DlgAdminMapObject / Edit | オブジェクトテンプレートのCRUD |
| オブジェクト配置 | DlgAdminMapObjectData / Edit | マップ上へのオブジェクト配置CRUD |
| マップ情報編集 | DlgAdminMapInfo | マップ名/サイズ/BGM/天気/戦闘フラグ等 |
| マップイベント | DlgAdminMapEvent + 各種 | MOVE/MAPMOVE/LIGHT等のイベント管理 |
| マップ影 | DlgAdminMapShadow | 影パターンの編集 |

---

## 移行フェーズ

### Phase 1: マップパーツ編集のWeb化

マップの基礎データであるパーツの編集機能を最優先で実装する。

#### 1-1. マップパーツ編集API

- [x] `PUT /api/maps/parts` - パーツ情報更新
  - viewType, animeType, animeCount, level
  - flags (block, pile, pileBack, fishing, drawLast, counter)
  - blockDirection, moveDirection
  - sprites (base/overlay のグラフィックID)
  - viewPosition
- [x] `POST /api/maps/parts` - 新規パーツ追加
- [x] `DELETE /api/maps/parts` - パーツ削除
- [x] 更新時にクライアントへパケット通知 (PacketMAP_MAPPARTS / PacketMAP_DELETEPARTS)
- [x] JsonUtils に TryGetBool / TryGetUInt 追加

#### 1-2. Web UI - パーツ編集画面

- [x] パーツ選択時の詳細編集パネル
- [x] フラグのチェックボックス編集 (block, pile, pileBack, fishing, drawLast, counter)
- [x] 方向別ブロック設定 (上下左右)
- [x] 移動方向設定
- [x] viewType / animeType / level / viewPosition 編集
- [x] グラフィックID (base/overlay) 編集
- [x] 保存ボタン (PUT) / 新規作成ボタン (POST) / 削除ボタン (DELETE)
- [ ] スプライト選択UI (シートから視覚的に選択) ※未着手

---

### Phase 2: マップオブジェクト管理のWeb化（2026-02-27 完了）

#### 2-1. オブジェクトテンプレートAPI

- [x] `GET /api/maps/objects/templates` - オブジェクトテンプレート一覧
- [x] `POST /api/maps/objects/templates` - 新規テンプレート作成（body: name, attribute, hideY, width, height, hasCollision）
- [x] `PUT /api/maps/objects/templates` - テンプレート更新（body: objectId 含む）
- [x] `DELETE /api/maps/objects/templates` - テンプレート削除（body: objectId）
  - 削除後は `PacketMAP_MAPOBJECT::Make(CLibInfoMapObject*)` で全クライアントに再送

#### 2-2. オブジェクト配置API

- [x] `POST /api/maps/placements` - オブジェクト配置追加（body: mapId, objectId, x, y）
- [x] `PUT /api/maps/placements` - 配置更新（body: mapId, dataId, objectId, x, y）
- [x] `DELETE /api/maps/placements` - 配置削除（body: mapId, dataId）
  - 削除後は `PacketMAP_MAPOBJECTDATA::Make(mapId, CLibInfoMapObjectData*)` で再送

#### 2-3. Web UI - オブジェクト管理画面

- [x] オブジェクトテンプレートの一覧・編集（`#map-object-templates` ルート）
  - テンプレート一覧テーブル（行クリックで選択）
  - 編集フォーム（名称・属性・当たり判定・サイズ・隠れY）
  - 新規追加・更新・削除ボタン
- [x] マップ上のオブジェクト配置エディタ（`#map-objects` ルート）
  - マップ選択後、グリッドまたはテーブル行クリックで配置選択
  - objectId セレクトをテンプレート一覧から動的生成
  - 配置の新規追加・更新・削除をAPI経由で実行
- [ ] ドラッグ&ドロップでの配置操作（未実装、Phase 3 以降）

#### 実装メモ
- URLパラメータは `CApiRouter` が非対応のため、ID は全て request body に含める
- テンプレート削除パケットは専用パケットなし → 全オブジェクト再送で対応
- `CInfoMapObject::m_aInfoAnime`（アニメーション）はAPIから除外（Phase 3 以降）
- `SetStringFromUtf8` ヘルパーを MapObjectHandler.cpp に追加（UTF-8 → CP932/UTF-16 変換）

---

### Phase 3: マップ情報・イベント管理のWeb化

#### 3-1. マップ情報API

- [ ] `GET /api/maps/{mapId}` - マップ詳細情報
- [ ] `PUT /api/maps/{mapId}` - マップ情報更新 (名前/BGM/天気/戦闘/回復)
- [ ] `POST /api/maps` - 新規マップ作成
- [ ] `DELETE /api/maps/{mapId}` - マップ削除

#### 3-2. マップイベントAPI

- [ ] `GET /api/maps/{mapId}/events` - イベント一覧
- [ ] `POST /api/maps/{mapId}/events` - イベント追加
- [ ] `PUT /api/maps/{mapId}/events/{eventId}` - イベント更新
- [ ] `DELETE /api/maps/{mapId}/events/{eventId}` - イベント削除
- イベントタイプ: NONE, MOVE, MAPMOVE, LIGHT, GRPIDTMP, INITSTATUS

#### 3-3. マップ影API

- [ ] `GET /api/maps/shadows` - 影パターン一覧
- [ ] `POST /api/maps/shadows` - 影パターン追加
- [ ] `PUT /api/maps/shadows/{shadowId}` - 影パターン更新
- [ ] `DELETE /api/maps/shadows/{shadowId}` - 影パターン削除

#### 3-4. Web UI

- [ ] マップ情報編集フォーム
- [ ] イベントエディタ（タイプ別の編集フォーム）
- [ ] 影パターンエディタ

---

## 技術方針

### サーバー側 (SboSvr)

- 既存の `HttpServer` + `ApiRouter` 構成を継続
- ハンドラーは `SboSvr/src/Web/Handlers/` に追加
- `CMgrData` 経由でゲームデータにアクセス（既存パターンに従う）
- 書き込み操作時はミューテックスで排他制御

### クライアント側 (webroot)

- 既存の `app.js` (vanilla JS SPA) に画面を追加
- 既存のダークテーマCSS・コンポーネント構造を活用

### データ構造 (参照)

- `CInfoMapParts` - パーツ定義 (Common/Info/)
- `CInfoMapObject` - オブジェクトテンプレート
- `CInfoMapObjectData` - オブジェクト配置
- `CInfoMapBase` - マップ基本情報
- `CLibInfoMapParts` 等 - 各ライブラリマネージャ

---

## 進捗

| フェーズ | 状態 | 備考 |
|---------|------|------|
| Phase 1: マップパーツ編集 | **ほぼ完了** | API (CRUD) + UI実装済み。スプライト視覚選択UIのみ未着手 |
| Phase 2: マップオブジェクト管理 | **完了** | テンプレートCRUD API + 配置CRUD API + Web UI (2026-02-27) |
| Phase 3: マップ情報・イベント | 未着手 | |

### 2025-02-13 更新: Phase 1 詳細

**サーバー側 (実装完了)**
- `CMapPartsUpdateHandler` (PUT) - パーツ情報更新 + クライアントへパケット通知
- `CMapPartsCreateHandler` (POST) - 新規パーツ追加 + クライアントへパケット通知
- `CMapPartsDeleteHandler` (DELETE) - パーツ削除 + クライアントへパケット通知
- `JsonUtils::TryGetBool` / `TryGetUInt` 追加
- `HttpServer` にルーティング登録済み (PUT/POST/DELETE /api/maps/parts)

**Web UI (実装完了)**
- 編集フォーム: viewType, animeType, level, 移動方向, 表示位置, グラフィックID
- フラグチェックボックス: block, pile, pileBack, fishing, drawLast, counter
- 方向別ブロック: 上下左右
- 保存/新規作成/削除ボタン

**その他**
- 不要な過去ドキュメント (docs/admin_phase_*, docs/apis/, docs/handovers/, etc.) を削除整理
