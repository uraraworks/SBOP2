# js/ ディレクトリ — ES Modules 基盤

## 構成

```
js/
  main.js          エントリポイント。移行済みビューのレジストリ管理とルーティングフック
  core/
    router.js      ハッシュルーティングユーティリティ(レガシー app.js と競合しない設計)
    api.js         fetch ラッパ(fetchJson / postJson)
    dom.js         el() / escapeHtml() / qs() / qsa() ヘルパ
  data/
    assets.js      /api/image-categories・/api/sounds キャッシュ取得、スプライト座標計算、画像共有キャッシュ
  components/
    sprite-thumb.js   createSpriteThumb({categoryKey,sub,size}) → {el,update,destroy}  一覧サムネ向け遅延版は createSpriteThumbLazy()
    sprite-picker.js  openSpritePicker({categoryKey,current,allowCategorySwitch,onSelect}) モーダル選択ダイアログ。createSpriteField() でサムネ+input+ボタン横並びフィールドも生成可
    anime-preview.js  createAnimePreview({width,height,scale}) → {el,setFrames(frames),destroy}  多層スプライトをアニメ再生するcanvas+コントロール
    number-spinner.js createNumberSpinner({value,min,max,step,onChange}) → {el,getValue,setValue}  [-][input][+]長押しリピート付き
    drag-list.js      createDragList({items,renderItem,onReorder}) → {el,setItems,getItems}  DnD並べ替え+↑↓ボタン
    sound-picker.js   createSoundPicker({value,onChange}) → {el,getValue,setValue}  /api/sounds から select 生成
    list-detail.js    createListDetail({container,title,fetchList,renderListItem,renderDetail,onSave,onCreate,onDelete,searchKeys}) → {refresh,getSelected}  左リスト+右詳細フォームシェル(dirty管理付き)
  styles/
    components.css    F2 コンポーネント共通スタイル(index.html から link 済み)
  views/           全 27 画面の ES Module ビュー（F3〜F9 フェーズで移行完了）
    map-parts-edit.js         マップパーツ編集 (F3)
    map-shadow-edit.js        マップ影編集 (F3)
    map-parts-place.js        マップパーツ配置 (F4)
    map-object-place.js       マップオブジェクト配置 (F4)
    motion-types.js           モーション種別管理 (F5)
    motion-edit.js            モーション編集 (F5)
    effect-edit.js            エフェクト編集 (F6)
    balloon-edit.js           噴き出し編集 (F6)
    item-types.js             アイテム種別 (F7)
    item-list.js              アイテム一覧 (F7)
    weapon-list.js            武器一覧 (F7)
    npc-add.js                NPC 追加 (F8)
    char-edit.js              キャラクター詳細編集 (F8)
    skill-edit.js             スキル編集 (F8)
    init-status.js            初期ステータス (F8)
    server-dashboard.js       サーバー情報ダッシュボード (F9)
    account-create.js         管理アカウント作成 (F9)
    role-management.js        管理者ロール設定 (F9)
    character-list.js         キャラクター一覧 (F9・スプライトサムネ付き)
    character-account.js      アカウント紐付け・BAN (F9)
    map-info.js               マップ情報編集 (F9)
    map-events.js             マップイベント編集 (F9)
    talk-events.js            会話イベント編集 (F9)
    map-window.js             マップウィンドウ (F9)
    audit-reports.js          監査レポート (F9・プレースホルダー)
    operation-history.js      操作履歴 (F9)
    failback-guide.js         フェイルバック手順 (F9・プレースホルダー)
```

## 全画面移行完了（F9 達成）

F9 フェーズ（2026-06-12）をもって全 27 画面の ES Module 移行が完了した。

- `app.js` は共通初期化・ナビゲーション基盤・WebSocket・共通 picker モジュールのみを残す
  （4346 行 → 約 1020 行）
- 各画面の DOM・状態・イベント登録はすべて `views/` 以下のファイルが担当
- `main.js` に全ルートが `registerRoute` 登録済み
- `index.html` の全 `data-view` コンテナは空コンテナ化済み（各 mount() が描画）

## 画面移行ルール

1. **1画面 = `views/` 以下の1ファイル** (例: `views/map-parts.js`)
2. ファイルは `mount()` 関数を named export する。
3. `main.js` の「移行済みビューの登録」セクションに `registerRoute("route-name", mount)` を追加する。
4. `index.html` の該当 `<div data-view="route-name">` の中身を空コンテナ化する(既存 HTML を削除)。
5. `app.js` の旧コード(当該ビューの関数・DOM 操作)を削除する。

## レガシー連携方針

- `main.js` はレガシー `app.js` の hashchange リスナーと**並存**する。
- `isMigrated(route)` が `false` の間は何も処理しないため、既存動作に影響なし。
- `isMigrated(route)` が `true` になった時点で、そのビューの描画をモジュール側が担う。

## fetch 規約

- `core/api.js` の `fetchJson` を使う。`credentials: "include"` が既定。
- HTTP エラーは例外を投げない。呼び出し元で `response.ok` を確認する。

## スプライト座標計算

`data/assets.js` の `calcSpriteCoord(category, sub)` を参照。

```
sheetIndex = floor(sub / (countX * countY))
cellIndex  = sub % (countX * countY)
x          = (cellIndex % countX) * cellSize
y          = floor(cellIndex / countX) % countY * cellSize
シート URL  = category.sheetUrl + "/" + sheetIndex
```
