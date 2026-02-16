# フロントエンド構造ガイド

Web管理画面（`SboSvr/webroot/`）の構造と状態管理を把握するためのリファレンス。
新しいAPIビューを追加する際に参照すること。

---

## ファイル一覧と役割

| ファイル | 行数 | 役割 |
|---------|------|------|
| `SboSvr/webroot/index.html` | 539行 | SPAのHTML本体。`data-view`属性でビュー管理 |
| `SboSvr/webroot/app.js` | 1906行 | 全UI処理。フレームワーク不使用のバニラJS |
| `SboSvr/webroot/styles.css` | 779行 | スタイル定義 |

---

## ルーティングシステム

URLハッシュ（`window.location.hash`）ベースのシングルページアプリ。

**唯一のルート切り替え関数**:
```javascript
activateRoute(route, options = {})
```

**仕組み**:
- `data-view="xxx"` 属性を持つ要素が各ビュー（`is-active`クラスで表示切り替え）
- `data-route="xxx"` 属性を持つ `<a>` タグがナビリンク（`is-active`クラスで強調）

---

## 実装済みルート一覧

| ルート（`data-view`値） | 機能 | 実装状況 |
|---|---|---|
| `server-dashboard` | サーバー情報ダッシュボード | ✅ Wave1 |
| `account-create` | アカウント作成 | ✅ Wave1 |
| `role-management` | 管理者ロール設定 | ✅ Wave1 |
| `map-objects` | マップオブジェクト配置 | ✅ Wave2 |
| `map-parts` | マップパーツ編集 | ✅ Wave2 |
| `char-edit`, `char-list` 等 | キャラクター管理 | ❌ 未実装 |
| `item-type`, `item-list` 等 | アイテム管理 | ❌ 未実装 |
| `audit-report`, `op-history` | 監査・運用 | ❌ 未実装 |

---

## 状態管理オブジェクト

フレームワーク不使用。モジュールスコープの `const` オブジェクトで管理（`app.js` 冒頭）。

### `mapObjectState`（マップオブジェクト用）

```javascript
const mapObjectState = {
  maps: [],                  // APIから取得したマップ一覧（normalizeMapEntry後）
  selectedMapId: null,       // 選択中マップID（文字列）
  selectedObjectId: null,    // 選択中オブジェクトID（文字列）
  filterType: "all",         // "all" | "collision" | "no-collision"
  previewSelection: null,    // グリッドプレビューの選択セル
  isLoading: false,
  loadError: null
};
```

### `mapPartsState`（マップパーツ用）

```javascript
const mapPartsState = {
  parts: [],                 // APIから取得したパーツ一覧（normalizeMapPart後）
  filtered: [],              // フラグフィルタ後のパーツ
  flagFilter: "all",         // "all" | "block" | "pile" | ...
  selectedPartId: null,      // 選択中パーツID（数値）
  sheetBaseUrl: "/api/assets/map-parts/sheets",
  tileSize: 16,              // タイルサイズ（px）
  sheetTileWidth: 32,        // シート内のタイル列数
  sheetTileHeight: 32,       // シート内のタイル行数
  sheetCount: 0,             // シート総数
  sheets: [],                // シートキャッシュ配列
  isLoading: false,
  loadError: null
};
```

---

## API通信

**全API呼び出しは `fetchJson()` を経由する**:

```javascript
// 使い方
const { response, data, text } = await fetchJson("/api/server?includeMetrics=true");
// response: Fetch Response オブジェクト
// data: JSONパース済みオブジェクト（失敗時null）
// text: レスポンスボディの生テキスト

// POST/PUT の場合
const { response, data } = await fetchJson("/api/accounts", {
  method: "POST",
  body: JSON.stringify({ loginId, password, displayName, roles })
});
```

**特徴**:
- `credentials: "include"` が設定済み（CookieのSESSIDが自動送信される）
- `Accept: "application/json"` が設定済み

---

## app.js 関数の分類

### データ正規化（APIレスポンス → 内部モデル変換）

| 関数名 | 変換対象 |
|--------|---------|
| `normalizeMapEntry(rawMap)` | `/api/maps/objects` のマップ1件 |
| `normalizeMapObject(rawObject)` | オブジェクト1件（idHex, typeLabel等を追加） |
| `normalizeMapPart(raw)` | `/api/maps/parts` のパーツ1件 |
| `normalizeMapPartSprite(rawSprite)` | スプライト情報 |

### データ読み込み（非同期・状態更新）

| 関数名 | 呼び出すAPI |
|--------|-----------|
| `loadServerInfo(showToast)` | `GET /api/server` |
| `startServerPolling()` | 30秒間隔で `loadServerInfo` を繰り返す |
| `loadRoles()` | `GET /api/admin/roles` |
| `loadMapObjectData(forceReload)` | `GET /api/maps/objects` |
| `loadMapPartsData(forceReload)` | `GET /api/maps/parts` + 各シート画像 |

### レンダリング（DOM更新）

| 関数名 | 役割 |
|--------|------|
| `populateServerDisplay(data)` | サーバー情報の各要素を更新 |
| `renderRoleOptions(roles)` | ロール選択チェックボックスを生成 |
| `renderMapComponents()` | マップUI全体（セレクト・テーブル・プレビュー）を再描画 |
| `renderMapObjectTable(map)` | オブジェクト一覧テーブルを生成 |
| `renderMapPreview(map)` | マップグリッドプレビューを生成 |
| `renderMapPartsFlagList(part)` | フラグリストのチェックボックスを描画 |
| `renderMapPartsGallery()` | パーツギャラリーカードを生成 |
| `updateSpriteLayer(canvasEl, sprite)` | Canvas にスプライト画像を描画 |

### イベントハンドラ

| 関数名 | トリガー |
|--------|---------|
| `handleAccountSubmit(event)` | アカウント作成フォームのsubmit |
| `handleRoleSubmit(event)` | ロール更新フォームのsubmit |
| `handleMapSelectChange()` | マップセレクトの変更 |
| `handleMapObjectFormSubmit(event)` | マップオブジェクトフォームのsubmit |
| `selectMapObject(objectId)` | オブジェクト行またはグリッドセルのクリック |
| `selectMapPart(partId)` | パーツギャラリーカードのクリック |

---

## 新しいAPIビューを追加する際のチェックリスト

1. **`index.html`** に `<div class="view" data-view="your-view">` を追加
2. **`index.html`** のナビゲーションに `<a class="nav-link" data-route="your-view">` を追加
3. **`app.js` の `activateRoute()`** にルート切り替え時の処理を追加
   - 初回アクティブ時にデータを読み込む処理
   - 必要に応じてポーリングの開始/停止
4. **状態オブジェクト** を定義: `const xxxState = { ... };`
5. **データ読み込み関数** を実装: `async function loadXxxData(forceReload = false)`
6. **レンダリング関数** を実装: `function renderXxx()`
7. **`window.addEventListener("load")`** 内でイベントリスナーを登録

---

## マップパーツシート画像の仕組み

`/api/assets/map-parts/sheets/{index}` でPNG画像を取得する流れ。

```
SboGrpData プロジェクト（SboGrpData/）
  → グラフィックスデータをDLL（SboGrpDataDLL.dll）に埋め込み

CMapPartsResourceProvider（SboSvr/src/Web/Handlers/MapPartsHandler.h）
  → LoadLibrary() でDLLをロード
  → GetProcAddress() でシートPNGのバイナリを取得
  → キャッシュ（std::map<int, std::vector<unsigned char>>）に保存

app.js
  → mapPartsState.sheetBaseUrl + "/" + index でシート画像を <img> として参照
  → tileSize=16, sheetTileWidth=32, sheetTileHeight=32 で座標計算
  → CSS background-position でタイルを切り出して表示
  → MAP_PARTS_GALLERY_SCALE=4, MAP_PARTS_DETAIL_SCALE=6 で拡大表示
```

---

## 定数・ラベルの場所

`app.js` 冒頭で定義されている定数一覧:

| 定数名 | 内容 |
|--------|------|
| `POLL_INTERVAL_MS` | サーバー情報の自動更新間隔（30秒） |
| `MAP_OBJECT_TYPE_OPTIONS` | オブジェクトタイプ選択肢（当たり判定あり/なし） |
| `MAP_PARTS_FLAG_LABELS` | フラグのラベル（block/pile/pileBack等） |
| `MAP_PARTS_DIRECTION_LABELS` | 方向ラベル（上/下/左/右） |
| `MAP_PARTS_VIEW_TYPE_LABELS` | ビュータイプラベル（通常/背景/装飾等） |
| `MAP_PARTS_ANIME_TYPE_LABELS` | アニメタイプラベル（静止/往復/循環/ランダム） |
| `MAP_PARTS_GALLERY_SCALE` | ギャラリー表示倍率（4倍） |
| `MAP_PARTS_DETAIL_SCALE` | 詳細表示倍率（6倍） |

---

## 関連ドキュメント

- 全体アーキテクチャ: `docs/codebase/architecture_overview.md`
- APIハンドラ追加手順: `docs/codebase/api_handler_cookbook.md`
