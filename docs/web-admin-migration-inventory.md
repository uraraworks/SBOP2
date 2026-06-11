# Web管理画面 移植対象一覧

最終更新: 2026-06-11

## 目的

凍結済みの `SboCliAdminMfc` を仕様書代わりに参照しながら、Web 管理画面へ移植すべき機能を一覧化する。  
この文書は「何を置き換える必要があるか」を整理するための正本とする。

## 参照元

- ネイティブ版メニュー: `SboCliAdminMfc/AdminResources.rc2`
- ネイティブ版ダイアログ: `SboCliAdminMfc/src/Migrated/Dialog/`
- ネイティブ版補助ウィンドウ: `SboCliAdminMfc/src/Migrated/Wnd/`
- 現行 Web UI: `SboSvr/webroot/index.html`, `SboSvr/webroot/app.js`
- 現行 Web API: `SboSvr/src/Web/HttpServer.cpp`, `SboSvr/src/Web/Handlers/`

## ステータス凡例

- `実装済み`: Web UI と API があり、現行コードで動作根拠がある
- `UI雛形`: 画面プレースホルダーのみあり、実処理は未接続
- `未着手`: Web UI / API ともに未整備
- `要再設計`: 1:1 置換ではなく、Web 向けに構成を変えるべき機能

## 運用基盤

| 機能群 | ネイティブ版の参照 | 現行 Web 側 | 状態 | メモ |
|---|---|---|---|---|
| サーバー情報 | `IDM_SERVER_INFO`, `DlgAdminServerInfo` | `server-dashboard`, `GET /api/server` | 実装済み | 既に Web 側の入口として機能している |
| アカウント追加 | `IDM_ACCOUNT_ADD`, `DlgAdminAccountAdd` | `account-create`, `POST /api/accounts` | 実装済み | MFC の「その他 > アカウント」を置換中 |
| 管理者権限設定 | `IDM_CHAR_ADMIN`, `DlgAdminCharAdmin` | `role-management`, `GET/PUT /api/admin/roles` | 実装済み | ネイティブは選択キャラ起点、Web はアカウント/ロール起点に再編されている |
| 初期ステータス設定 | `IDM_SYSTEM_SET_INITCHARSTATUS`, `DlgAdminSystemSetInitCharStatus` | `initial-status`, `GET/PUT /api/initial-status` | 実装済み | Wave 2D 完了。単一レコード編集フォームで保存時に全クライアントへ `CPacketSYSTEM_INFO` を配信 |
| デバッグ: グリッド表示 | `IDM_DEBUG_GRID_OFF`, `IDM_DEBUG_GRID`, `IDM_DEBUG_GRID_HALF` | なし | 未着手 | Web/ネイティブ共通のマップ表示基盤ができてから再判断 |
| デバッグ: 移動あたり判定無効 | `IDM_DEBUG_MOVENOBLOCK` | なし | 未着手 | 恒久機能ではなく、内部管理用途として後回しでよい |

## キャラクター管理

| 機能群 | ネイティブ版の参照 | 現行 Web 側 | 状態 | メモ |
|---|---|---|---|---|
| 選択キャラ編集: 基本情報 | `IDM_CHAR_MODIFY`, `DlgAdminCharModify` | `character-overview` | UI雛形 | 旧 UI は「選択中キャラ」前提。Web では検索 + 詳細パネルへ再設計したい |
| 選択キャラ編集: ステータス | `IDM_CHAR_MODIFY_STATUS`, `DlgAdminCharModifyStatus` | `character-overview` に統合想定 | 未着手 | 基本情報と別画面にせず統合候補 |
| 選択キャラ編集: 画像設定 | `IDM_CHAR_MODIFY_GRP`, `DlgAdminCharModifyGrp` | `character-overview` に統合想定 | 未着手 | 画像選択コンポーネントが別途必要 |
| 選択キャラ編集: 所持アイテム | `IDM_CHAR_MODIFY_ITEM`, `DlgAdminCharModifyItem` | `character-overview` か専用詳細タブ | 未着手 | アイテム選択 UI とセットで設計する |
| 選択キャラ編集: 所持スキル | `IDM_CHAR_MODIFY_SKILL`, `DlgAdminCharModifySkill` | `skill-management` またはキャラ詳細タブ | 実装済み | Wave 2C でキャラ詳細タブ (cd-tab-skills) に実装済み |
| キャラクター一覧 | `IDM_CHAR_LIST`, `DlgAdminCharList` | `character-list` | UI雛形 | API 未実装 |
| キャラとアカウントの紐付け | `DlgAdminCharAccountInfo` | `character-account` | UI雛形 | API 未実装 |
| NPC 追加 | `IDM_CHAR_ADDNPC`, `DlgAdminCharAddNPC` | `npc-management` | UI雛形 | マップ pick 基盤が必要 |
| モーション編集 | `IDM_CHAR_MOTION`, `DlgAdminCharMotion*` | `motion-management` | 実装済み | /api/motion-types + /api/motions CRUD。プレビューは未実装・要再設計 |
| スキル一覧・編集 | `IDM_CHAR_SKILL`, `DlgAdminCharSkill*` | `skill-management` | 実装済み | /api/skills CRUD。MOVEATACK/HEAL 派生フィールド対応。typeMain/typeSub で UI 切替 |
| 拒否 / 凍結 | `IDM_CHAR_DISABLE`, `DlgAdminCharDisable*` | `character-account` に統合想定 | 未着手 | アカウント状態変更と同じ文脈で扱う想定 |
| 会話設定 | `DlgAdminTalkSet`, `DlgAdminTalkEvent*` | なし | 未着手 | キャラ編集とマップオブジェクト配置編集の両方から使う共通エディタにする |

## マップ管理

| 機能群 | ネイティブ版の参照 | 現行 Web 側 | 状態 | メモ |
|---|---|---|---|---|
| マップ情報編集 | `IDM_MAP_INFO`, `DlgAdminMapInfo` | `map-info` | UI雛形 | API 未実装 |
| マップイベント編集 | `IDM_MAP_EVENT`, `DlgAdminMapEvent*` | `map-events` | UI雛形 | API 未実装。タイプ別フォームが必要 |
| マップパーツ編集 | `IDM_MAP_PARTS`, `DlgAdminMapParts*` | `map-parts`, `GET/PUT/POST/DELETE /api/maps/parts` | 実装済み | 視覚的なシート選択 UI はまだ弱い |
| マップ影編集 | `IDM_MAP_SHADOW`, `DlgAdminMapShadow*` | なし | 未着手 | パーツ編集に似るが別データ系統 |
| マップ追加 | `IDM_MAP_ADD` | なし | 未着手 | マップ情報編集 API と同時実装が自然 |
| オブジェクト定義編集 | `IDM_MAP_OBJECT`, `DlgAdminMapObject*` | `map-object-templates`, `GET/POST/PUT/DELETE /api/maps/objects/templates` | 実装済み | 定義 CRUD は置換済み |
| オブジェクト配置編集 | `IDM_MAP_OBJECTDATA`, `DlgAdminMapObjectData*` | `map-objects`, `POST/PUT/DELETE /api/maps/placements` | 実装済み | グリッド編集あり。ゲーム画面との連携は未統合 |
| マップウィンドウ | `IDM_MAP_WINDOW`, `WndMap`, `WndMapPartsGrp`, `WndMapShadowGrp` | なし | 要再設計 | Web / ネイティブ双方のゲーム画面と連携するマップビューへ置換したい |
| クリックで対象選択 | `StateProcMAP`, `ADMINNOTIFYTYPE_*` | なし | 要再設計 | `pick` API とサーバー側選択状態に置き換える |

## アイテム管理

| 機能群 | ネイティブ版の参照 | 現行 Web 側 | 状態 | メモ |
|---|---|---|---|---|
| アイテム種別一覧・編集 | `IDM_ITEM_TYPELIST`, `DlgAdminItemTypeList`, `DlgAdminItemTypeNew*` | `item-types` | UI雛形 | API 未実装 |
| アイテム一覧・編集 | `IDM_ITEM_LIST`, `DlgAdminItemList`, `DlgAdminItemNew` | `item-list` | UI雛形 | API 未実装 |
| 武器情報一覧・編集 | `IDM_ITEM_WEAPONLIST`, `DlgAdminItemWeaponList`, `DlgAdminItemWeaponNew` | `weapon-list` | UI雛形 | API 未実装 |
| アイテム選択ダイアログ | `DlgAdminSelectItem`, `WndSelectItemGrp` | なし | 未着手 | 共通 picker として再利用したい |

## エフェクト管理

| 機能群 | ネイティブ版の参照 | 現行 Web 側 | 状態 | メモ |
|---|---|---|---|---|
| 噴出し一覧・編集 | `IDM_EFC_BALLOON_LIST`, `DlgAdminEfcBalloon*` | `effect-balloon` | UI雛形 | API 未実装 |
| エフェクト一覧・編集 | `IDM_EFC_EFFECT_LIST`, `DlgAdminEfcEffect*` | `effect-library` | UI雛形 | API 未実装 |
| エフェクト選択ダイアログ | `DlgAdminSelectEffect`, `ListCtrlEffectInfo` | なし | 未着手 | 共通 picker として再利用したい |

## 共通部品として移植するもの

| 部品 | ネイティブ版の参照 | 状態 | メモ |
|---|---|---|---|
| 画像選択 picker | `WndSelectGrp`, `WndSelectMapPartsGrp`, `WndSelectMapShadowGrp`, `WndSelectItemGrp` | 未着手 | マップ、キャラ、アイテム、エフェクトで共通利用したい |
| 会話イベント editor | `DlgAdminTalkEvent*`, `DlgAdminTalkSet` | 未着手 | キャラ/NPC/オブジェクトから呼べる共通コンポーネントにする |
| マップ上の選択状態 | `WndMap`, `StateProcMAP`, `ADMINNOTIFYTYPE_*` | 要再設計 | サーバー側 selection state を持つ |
| 権限モデル | `DlgAdminCharAdmin` と Web ロール API | 継続中 | MFC の「選択キャラへ付与」から Web の「アカウント/ロール」へ寄せる |

## 未反映ギャップ

現行 `SboSvr/webroot/index.html` のナビゲーションには、ネイティブ版に存在した以下がまだ出ていない。

- マップ影編集
- マップ追加
- マップウィンドウ
- デバッグ系機能
- 共通 picker 群
- 会話イベント editor

逆に、以下は Web 側で先行追加された運用メニューであり、ネイティブ版の 1:1 置換ではない。

- 管理者ロール設定
- 監査レポート
- 操作履歴
- フェイルバック手順

## 補足方針

- 旧 MFC ダイアログを 1 ダイアログ 1 ページで移す必要はない
- キャラ編集は「基本情報 / ステータス / 見た目 / 所持物 / 会話」を一つの詳細画面へ統合する余地が大きい
- マップ系は `pick` API とマップビュー基盤を先に作らないと、NPC 追加やイベント編集が前に進みにくい
- アイテム、エフェクト、画像選択は共通 picker を先に整えると後続実装が軽くなる
