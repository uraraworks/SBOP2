# Web 管理画面 画面別改善案

> 作成日: 2026-09-11
> 前提: 課題の洗い出しは `docs/web-admin-usability-improvements.md`。本書は「どう直すか」の具体案。
> 規模目安: S = 1 セッション内で数画面まとめて可 / M = 1 画面 1 セッション / L = API 追加やゲーム側変更を伴う

---

## 0. 流用できる既存部品（棚卸し結果）

改善案はなるべくここにある部品で組む。新規部品は「新設」と明記。

| 部品 | 場所 | 使える用途 |
|---|---|---|
| `openSpritePicker` / `createSpriteField` | `js/components/sprite-picker.js` | 画像 ID をサムネ付きで選ぶ。既に char-edit / skill / item-types で使用 |
| `createSpriteThumbLazy` | `js/components/sprite-thumb.js` | 一覧行のサムネ表示 |
| `createAnimePreview` | `js/components/anime-preview.js` | 多層スプライトのアニメ再生。effect / motion で使用中 |
| `createListDetail` | `js/components/list-detail.js` | 左一覧＋右詳細＋検索＋dirty 管理のシェル。**採用は motion-types のみ** |
| `createDragList` | `js/components/drag-list.js` | コマ並び替え |
| `createNumberSpinner` | `js/components/number-spinner.js` | 数値入力 |
| `createSoundPicker` | `js/components/sound-picker.js` | 効果音 select（試聴なし） |
| `openTalkEventEditor` | `app.js` | 会話イベント editor モーダル |
| **共通 picker（image/item/effect/mapPart/mapShadow）** | `app.js:565-1030` `openPicker()` | **未 export・未使用**。`data-picker` 属性方式も index.html に 0 件。char-edit は自前 picker（`char-edit.js:882`） |
| `/api/npc-motion-pairs` | `HttpServer.cpp:1077` | NPC 画像 ID ↔ モーション種別の対応一覧。npc-add で未使用 |
| 管理画面 → ゲーム iframe の postMessage | `app.js:24,482`, `map-parts-place.js:89` | `sbop2_set_admin_mode` 等。**この経路を増やせばゲーム側の機能（効果音再生など）を管理画面から呼べる** |
| ゲーム → 管理画面の postMessage | `StateProcMAP.cpp:87,100` | `sbop2_admin_pick`（mapId/cellX/cellY/charId/itemId）、`sbop2_admin_pickup_parts` |
| `/api/sounds` | `SoundCatalogHandler.cpp` | id / key / label。WAV 実体は emscripten の preload FS 内で HTTP 配信されていない |

---

## 1. 共通基盤（全画面に効く。最初にやる）

### 1-1. `js/core/api.js` の拡張 【S】
- `fetchJson` に `onUnauthorized` フックを追加。401 を受けたら共通で「セッション切れ」バナーを出し、`app.js:394` のログイン画面へ戻す。各 view の個別 401 処理（role-management / account-create / image-editor）は削除して一本化。
- `postJson` / `putJson` / `deleteJson` を揃え、成功時に後述トーストを自動で出すオプション `{ toast: "保存しました" }` を持たせる。

### 1-2. 共通トースト 【S・新設】
- `js/components/toast.js` を新設。右下に 3 秒表示、`success / error / info` の 3 種。エラーはクリックで詳細（サーバーの message）を展開。
- 各 view の `result-message` / `form-feedback` / `alert()` をこれに寄せる。フォーム内のインライン表示はバリデーション用に残してよい。

### 1-3. 送信中ロックのヘルパー 【S・新設】
- `js/core/dom.js` に `withBusy(button, asyncFn)` を追加。実行中は `disabled` ＋ 文言を「保存中…」に差し替え、完了後に復元。
- 全画面の保存/削除/追加ハンドラを `withBusy(saveBtn, async () => {...})` で包むだけ。1 画面あたり数行。

### 1-4. 未保存変更の共通ガード 【M・新設】
- `js/core/dirty-guard.js` を新設。`registerDirty(() => boolean)` で「現在の view が dirty か」を登録。
- `beforeunload` と `hashchange`（`js/main.js:87-95`）の前段、および `handleAdminGamePick`（`app.js:139`）の前段で確認ダイアログを出す。
- `list-detail.js` の既存 dirty 管理はこのガードに接続する。char-edit はタブ単位の dirty フラグを立ててここに登録。

### 1-5. ゲーム画面クリックの受け口を一本化 【M】
- `app.js:139` の `handleAdminGamePick` を「ルート → ハンドラ」のレジストリ方式に変え、各 view が `registerPickHandler("map-events", fn)` で登録する。
- `map-object-place.js:811` 等の view 内 `message` リスナーを撤去して二重実装を解消。npc-add / character-account / init-status も登録可能になる。

### 1-6. 共通 picker の ES Module 化 【M】
- `app.js:565-1030` の `openPicker`（item / effect / mapPart / mapShadow）を `js/components/entity-picker.js` へ移して export。app.js 側は薄いラッパに。
- 併せて **モーション種別 picker**、**種族（NPC テンプレ）picker**、**マップ picker**、**キャラ picker** を同じ型で追加。データ源はそれぞれ `/api/motion-types`、`/api/characters?npc=1`、`/api/maps`、`/api/characters`。一覧行にサムネ（`createSpriteThumbLazy`）と名前を出し、検索 input 付き。
- `createSpriteField` と同じ「サムネ＋ID input＋選択ボタン」のフィールド生成関数 `createEntityField({ type, value, onChange })` を用意し、各画面は ID 直打ち input をこれに置換する。

### 1-7. 効果音の試聴 【M・ゲーム側小改修】
- `sound-picker.js` に ▶ ボタンを追加。押すと `adminGameFrame.contentWindow.postMessage({ kind: "sbop2_admin_play_sound", soundId })` を送る。
- ゲーム側（ブラウザ版）で `sbop2_admin_play_sound` を受けて `PlaySound(soundId)` を呼ぶ EM_JS ハンドラを 1 つ追加。WAV を HTTP 配信し直す必要が無く、実機と同じ音が鳴る。

### 1-8. ナビゲーション 【S】
- `index.html:40-93` の `<details open>` を「最後に開いていた状態」を localStorage で記憶する方式に。
- 「最近使った画面」5 件をメニュー最上部に自動表示（hash 遷移時に記録）。
- 一覧系のフィルタ条件・選択 ID を `#route?key=val` のクエリに反映し、リロードで復元（`router.js` にクエリ解析を追加）。
- `Ctrl+S` を `app.js` でグローバル捕捉し、現在 view が登録した `onSave` を呼ぶ（1-4 の dirty 登録と同じレジストリに載せる）。

---

## 2. マップ編集系

### map-info.js（マップ情報）
- BGM: `/api/sounds` の BGM 系（key で判別可能なら）を select 化し、1-7 の ▶ で試聴。【S】
- 天候 / 暗さレベル: 数値ではなく enum のラジオ/セレクト（晴/雨/雪…、暗さ 0-N のスライダー）に。値の意味は `MapHandler.cpp` の定数から拾う。【S】
- 一覧に検索 input（名前/ID）と「最近編集した順」ソートを追加。【S】
- **マップサイズ変更**: フォームに幅/高さを追加し、サーバーに `PUT /api/maps/{id}/size` を新設（ネイティブ `PacketADMIN_MAP_RENEWMAPSIZE` と同じ処理を呼ぶ）。縮小時はタイル欠落の確認ダイアログ。【L】
- 保存後に一覧へ自動で戻らず、その場に留まりトーストのみ。「一覧へ戻る」は明示ボタンで。【S】

### map-paint.js（タイルペイント）
- Redo を追加（Undo スタックの対で実装、`map-paint.js:388-522` の履歴構造を流用）。【S】
- **範囲選択 → コピー → 貼り付け**（矩形ドラッグで選択、Ctrl+C/Ctrl+V、貼り付けはプレビュー表示してからクリック確定）。ネイティブの COPYPARTS 相当をクライアント側だけで実現でき、API 追加不要。【M】
- 保存失敗時に dirty を復元し、トーストで再試行を促す（`map-paint.js:757-766`）。【S】
- ツールバーに「塗り / 矩形 / スポイト / 選択」のモード切替ボタンを置き、現在モードを常時表示（今は Shift/右クリックの暗黙操作）。【S】

### map-parts-place.js / map-parts-edit.js / map-shadow-edit.js
- パレットに検索 input（名前/ID）とレイヤー種別フィルタ（通常/背景/装飾/イベント/水辺/特殊）を追加。【S】
- 「最近使ったパーツ」列をパレット先頭に固定表示（localStorage、10 件）。配置作業のクリック数が大きく減る。【S】
- パーツ詳細にアニメフレームの「複製」ボタン（motion-edit の `addFrame` 複製と同じ作り）。【S】
- パーツ自体の「複製して新規」ボタン（既存レコードを POST し直す。API 追加不要）。【S】

### map-object-place.js（オブジェクト配置）
- テンプレート一覧に名前検索 input。【S】
- 配置一覧の行に「ゲーム画面で位置を表示」ボタン（`sbop2_admin_focus_cell` を iframe に送り、ゲーム側で黄色枠を出す。`LayerMap::DrawAdminPick` を流用）。【M・ゲーム側小改修】
- 1-5 のレジストリに乗せ換え、view 内 `message` リスナーを削除。【S】

### map-events.js（マップイベント）
- **クリックしたセルで新規作成**: ゲーム画面クリック時、該当セルにイベントが無ければ「ここに新規作成」ボタンを出し、座標 1 を埋めた状態でフォームを開く（`map-events.js:282-283` の初期値を pick 座標に）。【S】
- 種別 MAPMOVE の「移動先マップ ID / X / Y」に **マップ picker**（1-6）と「移動先をゲーム画面でクリックして指定」ボタン（クリック待ち状態にして次の pick を移動先に入れる）。ポータル作成が両端ともクリックで完結する。【M】
- 一覧行に種別アイコンと座標を出し、行クリックでゲーム画面のセルをハイライト（上記 `sbop2_admin_focus_cell`）。【M】
- 会話イベント ID 入力は、種別が会話系のとき自動で `data-talk-event-target` を紐付け、「編集」ボタンで `openTalkEventEditor` を直接開く（`map-events.js:118-119` の手入力を廃止）。【S】
- 保存後の 0.6 秒待ち自動遷移（`:361`）を廃止し、その場に留まる。【S】

### talk-events.js（会話イベント）
- 新規作成の `prompt()`（`:527`）を廃止。「新規」で空の editor を開き、ID は自動採番（サーバーが max+1 を返す or POST 時に採番）。【S】
- editor 内: ADDSKILL 行にスキル picker、PAGE 条件「アイテムあり/なし」にアイテム picker（どちらも 1-6 の entity-picker）。【S】
- ページ単位の折りたたみ表示（PAGE 行を見出しにしてその下の行をグルーピング）。【M】
- 保存前バリデーション: MENU のジャンプ先ページが存在するか、MSG が空でないかをチェックし、該当行を赤枠で示す。【S】
- MSG のプレビュー（ゲーム内の吹き出し幅で折り返した見た目を右側に表示）。【M】
- サーバー側: 保存時に `PacketADMIN_CHAR_RENEW_TALKEVENT` 相当をブロードキャストし、接続中クライアントに即反映。【L】

### map-window.js
- 用途が「セル選択の補助」に限られるので、map-events / map-object-place 側に埋め込む小型ミニマップ部品へ縮退させ、メニューから外す候補。【判断待ち】

---

## 3. キャラクター管理系

### character-list.js（一覧）
- 表示件数 select（20/50/100）、列ヘッダクリックでソート（名前/レベル/マップ/更新順）。【S】
- マップ ID input を **マップ picker** に。【S】
- 一覧行に操作ボタン: 「編集」「複製」「削除」「ゲーム画面で表示」。複製は `GET /api/characters/{id}` → 名前に「のコピー」を付けて `POST /api/characters/npc`（既存 API で可能か要確認、無ければ `POST /api/characters/{id}/clone` 新設）。【M】
- チェックボックスで複数選択 → 一括削除 / 一括マップ移動。【M】

### npc-add.js（NPC 追加）
- 追加成功後、トーストに「編集を開く」リンクを付け、`window._charEditMount(newId)` で char-edit へ直行。【S】
- 種族 ID → **種族 picker**（既存 NPC をサムネ付きで一覧し、選ぶと画像 ID・モーション種別・移動種別を丸ごとコピー）。「テンプレートから作る」体験に変える。【M】
- モーション種別 ID → `/api/npc-motion-pairs` を使った picker（char-edit の `openNpcMotionPicker` を共通化して流用）。【S】
- 座標 X/Y に「ゲーム画面でクリック」ボタン（1-5 のレジストリ登録で pick を受ける）。【S】
- `showFeedback` の二重呼び出し（`:283-289`）を削除。【S】

### char-edit.js（キャラ詳細）
- **NPC発生タブの form 化 ＋ `PUT /api/characters/{id}/npc-spawn` 新設**（`CharacterDetailHandler.cpp` に `m_dwPutCycle / m_nPutMoveType / m_nMaxPutCount / m_nPutAverage / m_ptPutArea` の書き込みを追加）。発生範囲 X/Y は「ゲーム画面で矩形ドラッグ」で指定できると理想（まずは数値入力で可）。【L・最優先】
- 「モーション種別ID」の重複（`:205` と `:987`）を移動タブ側に一本化し、グラフィックタブの NPC picker が更新する input もそちらに揃える。【S】
- タブごとの dirty フラグを持ち、タブ見出しに「●」表示。1-4 のガードに登録し、別タブ/別キャラ/離脱時に確認。【M】
- 「すべて保存」ボタンをヘッダに置き、dirty なタブだけ順に PUT する。【M】
- 種族 ID / アイテム ID / スキル ID（`:206,1048-1050,1081-1083`）を entity-picker に置換。所持アイテムタブはアイテム種別 picker から「追加」で 1 クリック投入。【S】
- `alert()` バリデーション（`:1612,1697`）をインライン表示＋トーストに統一。【S】
- ヘッダに「ゲーム画面で表示」「複製」「削除」ボタン。【S】

### character-account.js
- char-edit のアカウントタブと機能が同一なので、**この画面は「アカウント ID で検索 → 該当キャラの char-edit アカウントタブを開く」薄いランチャに縮退**させる。ロジックは char-edit 側 1 本に。【S】

### init-status.js
- 1-3 の送信ロックのみ。他は現状維持。【S】

---

## 4. マスタデータ系

### item-types.js（アイテム種別）
- 使用時エフェクト ID → エフェクト picker（1-6、サムネ＋名前＋アニメプレビュー）。武器情報 ID → 武器 picker（`/api/weapons` から名前一覧）。【S】
- 「詳細パラメータ」を種別（0-6）で出し分け。関係ない項目は非表示、または「この種別では未使用」とグレー表示。対応表は `ItemTypeHandler.cpp` / `LibInfoItemType` の使用箇所から作る。【S】
- 「複製して新規」ボタン（ネイティブ `OnCopy` 相当。既存レコードを POST）。【S】
- 削除前に `GET /api/items?typeId=` で使用中インスタンス数を取得し、「N 個のアイテムが参照しています」を確認ダイアログに出す。【S】

### item-list.js（アイテム）
- 種別 ID → アイテム種別 picker、charId → キャラ picker、mapId → マップ picker。【S】
- ページング（サーバー側 `limit/offset`、`/api/items` に追加）と件数表示。【M】
- 「複製」「ゲーム画面で位置表示」ボタン。【S】
- フィルタの「落下中」を初期選択にし、管理者が最も見たい「地面に落ちている物」から始める。【S】

### weapon-list.js（武器）
- **カンマ区切りエフェクト ID（`:164-178`）を廃止**し、エフェクト picker で選んだ項目をチップ（削除 × 付き）として並べる UI に。内部表現は従来どおり配列 → CSV 変換。【S】
- 立ち/すり足モーション ID → モーション picker（`/api/motions` を種別で絞る）＋ 右側に `createAnimePreview` で選択モーションを再生。「武器を作ったらこの画面で見た目確認まで完結」。【M】
- 削除前に `/api/item-types` から `weaponInfoId` 一致件数を数えて警告。【S】
- 「複製して新規」。【S】

### skill-edit.js（スキル）
- エフェクト ID 群（`:205-209,241`）→ エフェクト picker。4 方向のものは 4 つ横並びに「同じにする」ボタン。【S】
- 「複製して新規」。【S】

### effect-edit.js（エフェクト）
- **画像種別（effect32/64）をサーバーの実データから復元**: `EfcHandler` のレスポンスに `grpCategory` を含めるか、`grpIdMain` の範囲からクライアントで判定するヘルパーを追加（`:313-316` のコメントの解消）。【M】
- フレーム操作: 「選択コマの後ろに挿入」「一括 wait 設定」「全コマの alpha を一括」ボタン。【S】
- 「複製して新規」。【S】
- 削除前に weapon / skill / item-types の参照件数を集計して警告（3 API を並列 GET）。【S】

### balloon-edit.js（噴出し）
- **1 コマ = 1 レコードのフラット構造を UI 上で種別ごとにネスト表示**。左一覧は種別（listId）単位、右詳細でその種別のコマを `createDragList` で並べ、追加/削除/並び替え。保存時に animeId を振り直して flat CRUD に分解する（API は現状のまま）。【M】
- 種別 ID の新規は「未使用の最小 ID」を自動提案。【S】
- `createAnimePreview` で連続再生プレビュー。【S】

### motion-edit.js / motion-types.js（モーション）
- 一括 wait 設定ボタン。【S】
- リスト削除（`:827-843`）を `DELETE /api/motions?listId=` の一括 API に置換し、途中失敗を無くす。それまでは `response.ok` を見て失敗時に停止＋トースト。【S→M】
- モーション種別 picker を共通化（1-6）し、npc-add / char-edit / weapon-list から流用。【S】
- 再生プレビューは既に `createAnimePreview` があるので「要再設計・保留」の状態を見直し、ドキュメントを現状に合わせる。【判断待ち】

### image-editor.js（画像エディタ）
- 新規シート追加（`:84`）: 「同カテゴリの既存シートをコピーして空にする」方式なら寸法制約を満たしたまま追加できる。【M】
- アップロード寸法違いは拒否ではなく「左上に配置してパディング/トリム」の選択肢を出す。【M】
- 二度押し確定方式（`:186-221`）は良い作りなので、これを共通の `armConfirmButton` として `dom.js` に移し、他画面の `confirm()`（ポップアップ裏に出る罠あり）を順次置き換える。【S】

---

## 5. 運用・権限系

### server-dashboard.js
- 接続中プレイヤー一覧（キャラ名 / アカウント / マップ / 接続時間 / IP）をテーブル表示。行に「キック」「キャラ編集を開く」「メッセージ送信」。サーバー側に `GET /api/server/sessions`、`POST /api/server/kick`、`POST /api/server/broadcast` を新設（既存の管理者パケット処理を呼ぶだけ）。【L】
- 5 秒ポーリングを `/ws/admin` のプッシュに置換（接続数変化時のみ送る）。【M】
- 「サーバー保存（DB 書き戻し）」ボタン（`PacketADMIN_SERVER_SAVEINFO` 相当の API 新設）。停止せずに DB を確定させられる。【M】

### operation-history.js
- リングバッファを SQLite テーブル `sys_audit_log` に永続化（サーバー側）。日付範囲フィルタ・ページング・CSV エクスポートはこれが前提。【L】
- 詳細に before/after diff: 各ハンドラで更新前スナップショットを `AuditLog::Record` に渡す拡張（`docs/web-admin-migration-status.md` 保留メモの通り）。まずは「変更されたキーだけハイライト」の簡易版から。【M】

### audit-reports.js / failback-guide.js
- audit-reports: 上記永続化ができたら CSV ダウンロードに置き換える。それまでは **メニューから外す**。【S】
- failback-guide: ネイティブ管理画面は凍結・ビルド除外済みなので、内容を「サーバー停止/DB バックアップ/publish の切り戻し手順」に置き換えるか、メニューから外す。【判断待ち】

### role-management.js / account-create.js
- 自分自身（`/api/auth/me` の accountId）の管理者ロールを外す操作をクライアントで禁止し、サーバー側でも拒否。【S】
- ロール変更・アカウント作成は確認ダイアログ（1-1 の armConfirmButton 方式）を挟む。【S】
- アカウント ID を自由入力から `/api/accounts` の候補検索（datalist）に。【S】

---

## 6. 推奨の進め方

| 順 | 内容 | 規模 | 効果 |
|---|---|---|---|
| 1 | 1-1〜1-4（api 拡張・トースト・送信ロック・dirty ガード） | M | 全画面の安心感が一気に上がる |
| 2 | char-edit NPC発生タブの保存（3 章） | L | Phase A の前提を満たす |
| 3 | 1-6 entity-picker 化 ＋ 各画面の ID 直打ち置換 | M×2 | ID 暗記からの解放 |
| 4 | map-events「クリックで新規」「移動先クリック指定」、npc-add「追加後に編集」 | M | ポータル/NPC 設置が iframe だけで完結 |
| 5 | 1-7 効果音試聴 ＋ weapon-list のモーションプレビュー | M | 音・見た目の確認に他ツール不要 |
| 6 | 複製ボタン一式 ＋ 削除前の参照件数警告 | S×多 | ネイティブ版で出来ていたことの回収 |
| 7 | 一覧の検索/ソート/ページング統一、ナビ改善 | M | 件数が増えても困らない |
| 8 | 運用系（セッション一覧・キック・監査ログ永続化） | L | ライブ運営（GM 体験）の土台 |

### 進捗（2026-09-11）

- 順1 完了: core/api.js（401 共通フック setUnauthorizedHandler、putJson/deleteJson、toast オプション）、components/toast.js 新設、dom.js withBusy、core/dirty-guard.js 新設（beforeunload / hashchange / ゲーム画面クリック前段、list-detail 接続）。適用画面: map-info, npc-add, account-create, role-management, map-parts-edit, map-shadow-edit, init-status, map-paint。未適用: char-edit のタブ単位 dirty・その他の画面（順次）。
- 順2 完了: PUT /api/characters/{id}/npc-spawn 新設、char-edit NPC発生タブを form 化。実機で保存・400 バリデーション確認済み。発生させる移動種別はまだ数値入力（順3 の picker 化で対応）。

### 進捗（2026-09-12）

- 順3 完了: `js/components/entity-picker.js` 新設（openEntityPicker / createEntityField / invalidateEntityCache / fetchCharacterDetail。type: item/itemType/effect/weapon/skill/motionType/map/character/npcTemplate/mapPart/mapShadow）。モーダルは呼び出し元の ownerDocument に生成し別ウィンドウ対応。app.js の未使用レガシー picker と index.html の #picker-modal を削除。`js/data/move-types.js`（CHARMOVETYPE 全 11 値）・`js/data/family-types.js` 新設。適用: char-edit（種族 select・移動種別 select・モーション種別を移動タブに一本化・アイテム/スキル追加欄）、npc-add（種族 select＋テンプレート（既存NPC）から画像等をコピー）、item-types、item-list、character-list、weapon-list（エフェクトをチップ化）、skill-edit（4 方向「上と同じ」）、talk-events（ADDSKILL=skill、PAGE 条件=itemType）、map-events（移動先マップ）。
- 計画の訂正: 「種族 ID → 種族（NPC テンプレ）picker」は誤り。familyId はキャラ ID ではなく FAMILYTYPE_* の列挙値（体・耳画像の切替用）。種族は select、テンプレートは別欄にした。
- 残課題: エフェクトのサムネが effect32/64 判定の推測で半数空白（effect-edit の画像種別復元と同じ根本課題）、weapon-list の立ち/すり足モーション ID、char-edit のタブ単位 dirty ガード。
- 運用メモ: 稼働中の SboSvr は SboSvr\Debug\webroot のコピーを配信する（PostBuildEvent でビルド時のみ更新）。webroot を直したら xcopy で同期してから実機確認。
- 順4 完了: `js/core/game-pick.js` 新設（1-5: registerPickHandler / requestNextPick（次の 1 回のクリックを横取り、案内トーストに中止ボタン、Esc・画面遷移で自動解除）/ openCharacterEditor）。app.js の map-events 分岐と window._mapEvents* を廃止し map-events.js へ移設、map-object-place.js の独自 message リスナーを撤去。toast.js に action ボタン・duration オプション。map-events: クリックしたセルにイベントが無ければ「ここに新規作成」、MAPMOVE の移動先をクリックで指定、保存後の自動遷移を廃止（新規保存後に採番 id を反映し二重 POST を防止）。npc-add: 座標をクリックで指定（セル×32 でピクセルへ。MAPPARTSSIZE=32、左上基準）、追加後トーストの「編集を開く」でキャラ詳細へ。
- 座標単位メモ: ゲームからの pick はセル座標。マップイベントの pos と MAPMOVE 移動先はセル単位（変換不要）、キャラの x/y はピクセル単位（×32）。
- 残課題（順4 関連）: 一覧行クリックでゲーム画面のセルをハイライト（sbop2_admin_focus_cell、ゲーム側改修が要る）、npc-add の通常クリック連動は見送り。
- 順5 完了: 効果音の試聴（1-7）＝ sound-picker.js に ▶ ボタン → ゲーム iframe へ postMessage `sbop2_admin_play_sound` → tools/emscripten/sbocli-title.shell.html → `Module.ccall("SBOP2_AdminPlaySound")`（SboCli/src/StateProc/StateProcMAP.cpp、build-sbocli-browser-title.ps1 の EXPORTED_FUNCTIONS に追加）→ CMgrSound::PlaySound。ゲームがマップに入っている間だけ鳴る。編集ペインを別ウィンドウに出していても opener 経由で iframe に届く。weapon-list: 立ち/すり足モーション ID は CHARMOTIONLISTID の固定値（モーション種別ではない）と判明し select 化、プレビュー用モーション種別を選ぶと createAnimePreview で正面向きの動きを再生（立ち/すり足切替）。
- 残課題（順5 関連）: 武器自体の重ね描き画像（grpIdSub）は武器 API に無くプレビュー未反映。
- 順6 完了: js/core/dom.js に `armConfirmButton(button, { onConfirm, armedLabel?, timeoutMs?, message? })`（二度押し確定。message は文字列か、アーム時に呼ぶ関数（Promise 可。取得中はボタン無効＝件数を見ずに消せない））。confirm() は別ウィンドウ時に裏に隠れるため削除確認はこれに統一。「複製して新規」: item-types / weapon-list / skill-edit / effect-edit（コマごと）/ map-parts-edit / map-shadow-edit、item-list は「複製」（所有者・マップ・座標を 0 にリセットして未配置で作成）。map-parts / map-shadow はアニメのコマ単位「複製」も追加。いずれの POST もサーバー採番で body の ID は無視されるため既存を上書きしない（ItemTypeHandler.cpp:336 / WeaponHandler.cpp:328 / SkillHandler.cpp:386 / EfcHandler.cpp:449 / MapPartsHandler.cpp:712 / MapShadowHandler.cpp:273 / ItemHandler.cpp:408）。削除前の参照件数: item-types＝参照アイテム数、weapon＝参照アイテム種別数、effect＝武器・スキル・アイテム種別の件数（押すたびに取り直し）。実機で 656 件・2 件・「武器 2 件・アイテム種別 1 件」と一致を確認。
- 残課題（順6 関連）: スキル削除の参照件数（キャラ横断のスキル一覧 API が無い）、キャラの複製（API 未確認）。
- 順7 完了: js/core/router.js に parseHash / getRouteParams / setRouteParams（`#route?q=…` のクエリ対応。setRouteParams は history.replaceState で hashchange を起こさない＝再 mount も未保存確認も起きない。変更時に `sbop2:routeparamschange` を出し dirty-guard がキャンセル時の戻り先を更新）。app.js もクエリを除いてルート判定（`#route?…` で既定画面に飛ぶ不具合を解消）。js/core/save-shortcut.js（Ctrl+S / Cmd+S で現在画面の保存。押しっぱなしは無視。ポップアウト先の window にも登録）。js/components/list-toolbar.js（検索・並び替え・表示件数・ページ送り。クライアント側 applyToRows とサーバー側モード）。ナビ: メニューの開閉を localStorage に保存、「最近使った画面」5 件をメニュー上部に表示。適用: map-info / item-types / weapon-list / skill-edit / effect-edit / item-list / map-events（q/sort/page と選択 id を URL に保持し読み込み直しで復元、Ctrl+S 保存）、character-list（サーバー側ページング。並び替えはサーバー未対応のため表示中ページ内のみ）、map-parts-place（パーツ ID 検索・最近使ったパーツ 10 件）、map-object-place（テンプレート名検索）。
- 実機確認: 6 画面で検索→URL に q、行選択→URL に id、絞り込み中の hashchange 0 回、スキル画面を ?id=1&q=… で読み込み直して復元、Ctrl+S が各画面の PUT に繋がることを保存を止めた状態で確認。
- 残課題（順7 関連）: character-list の全件並び替え（サーバー側 sort パラメータが必要）、マップパーツの種類フィルタは現データの viewType が全件 0 のため実質非表示。
- 順8（一部）完了: `GET /api/server/sessions`（新設、SERVER_VIEW）で接続中プレイヤー一覧（アカウント・キャラ名・マップ・座標・最終通信・ログイン日時）。キャラはアカウントの m_dwCharID（サーバー側で未設定の死にフィールド）ではなく、ログイン中キャラ（CLibInfoCharSvr::GetPtrLogIn）とセッション ID で照合。ダッシュボードに一覧表（検索・並び替え、キャラ名からキャラ詳細へ）、接続時間を表示。
- 不具合修正: オンライン人数が常に 0（AddOnline/DecOnline がどこからも呼ばれていなかった）→ 接続中アカウント数を数えるよう修正。ダッシュボードの CPU/メモリ/遅延がオンライン人数から作った疑似値だった → プロセスの実測値（CPU%・メモリ%・MB）に置き換え、測れない遅延は「未計測」。m_dwTimeLastLogin は「前回」ではなく今回のログイン時刻。「最近使った画面」のチップが明るいサイドバーで読めなかった配色を修正。
- 実機確認: ログイン中キャラ「春うらら」がマップ・座標・接続時間付きで一覧に出ること、オンライン人数 1、キャラ名からキャラ詳細が開くことを確認。
- 残課題: IP 列は常に空（ブラウザ版は WebSocketBridge 経由で全員 127.0.0.1 に見えるため。IP による接続拒否も効いていない。別タスク化済み）。キック・ブロードキャスト・サーバー保存・監査ログ永続化は未着手。狭いペインで一覧の列が折り返される。
