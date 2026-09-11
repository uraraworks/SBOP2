# Web 管理画面 使い勝手改善リスト

> 調査日: 2026-09-11
> 対象: `SboSvr/webroot/` 配下の全画面（コード精読ベース。実機操作は未実施）
> 目的: 「管理者が日常的に使う」目線で、ネイティブ版（MFC）と比べて劣る点・面倒な点を洗い出し、今後のセッションで順に潰す。
> 画面別の具体的な改善案（どう直すか）は `docs/web-admin-usability-proposals.md` を参照。

行番号は調査時点のもの。着手時は必ず現物を確認すること。

---

## 0. 優先度サマリ

| 優先 | テーマ | 理由 |
|---|---|---|
| ★★★ | 横断: 二重送信防止 / 未保存警告 / 保存トースト / 401 共通処理 | ほぼ全画面に共通する欠落。1回の基盤作業で全画面が改善する |
| ★★★ | スポーナー（NPC発生）設定が Web 版では**編集不可** | UI が `dl/dd` 表示のみ、API も GET 出力のみ。Phase A（ダンジョンループ）の前提が崩れている |
| ★★★ | ID 直打ちの解消（名前/プレビュー付き picker） | 種族・モーション種別・エフェクト・BGM・アイテム・スキル・武器情報など随所。ネイティブ版は選択ダイアログがあった |
| ★★ | ゲーム画面クリック → 新規作成の導線 | イベント新規作成が常に (0,0) 始まり、NPC 追加後に編集へ飛べない |
| ★★ | 一覧の検索/ソート/ページング統一 | `list-detail.js` の採用が 1 画面のみで、各画面が自前実装 |
| ★★ | 複製（コピー）機能 | ネイティブの `OnCopy`（アイテム種別/アイテム/武器）と `PacketADMIN_MAP_COPYPARTS` 相当が無い |
| ★ | 運用系（ダッシュボード・監査ログ・プレースホルダー画面） | 日常運用では使うが、創作作業の妨げにはなっていない |

---

## 1. 横断的な改善点（基盤）

### 1-1. 保存まわりのフィードバック

現状: `js/core/api.js:19-39` の `fetchJson` は HTTP エラーで例外を投げず、扱いは各 view 任せ。成功トースト・共通ローディングは存在しない（`toast` で全文検索してもヒットなし）。

- **二重送信防止が全画面で未実装。** 保存/削除/追加ボタンが送信中に `disabled` にならない。連打で PUT/POST が重複する（例: `map-info.js:257-276`, `npc-add.js:229`, `account-create.js:177`, `role-management.js:156`, `char-edit.js:1386-1533`, `map-parts-edit.js:631-660`）。
- 保存成功の表示が画面ごとに `result-message` / `form-feedback` / `alert()` とバラバラ（`char-edit.js:1612,1697` は `alert`、他タブは `setFb`）。共通トーストを 1 つ作って統一したい。
- 保存失敗時に dirty 状態が復元されない画面がある（`map-paint.js:757-766`）。
- 会話イベントの保存はサーバー in-memory 差し替えのみで、クライアントへのリアルタイム反映（`PacketADMIN_CHAR_RENEW_TALKEVENT` 相当）が未実装（`docs/web-admin-migration-status.md` 保留メモ）。

### 1-2. 未保存変更の警告

現状: `list-detail.js:224-249` だけが `confirm()` で dirty 破棄を確認。`workspace-layout.js:224-228` の `beforeunload` はポップアップを閉じる用途で離脱警告ではない。

- タブ閉じ/リロード時の共通 `beforeunload` 警告が無い。
- `char-edit.js` はタブ切替・画面離脱・ゲーム画面で別キャラクリック（`char-edit.js:1831-1837`）のどれでも警告なしに編集内容が消える。
- `item-types` / `item-list` / `weapon-list` / `skill-edit` / `effect-edit` / `balloon-edit` / `motion-edit` は `list-detail.js` を使わず自前実装のため離脱警告なし。
- ブラウザ「戻る」（`hashchange`, `js/main.js:87-95`）でも未保存確認は効かない。

### 1-3. セッション切れ

現状: 401 の明示処理は `app.js:394`（ログイン画面）と `role-management.js:151` / `account-create.js:172` / `image-editor.js` のみ。

- 他画面では 401 が汎用エラー文言止まりで、再ログイン導線が無い。`fetchJson` 側で 401 を検知して共通で再ログインを促す仕組みが欲しい。

### 1-4. ナビゲーション / レイアウト

現状: `index.html:40-93` に 5 グループ約 30 項目を常時全展開。現在地は `app.js:462` の `is-active`。ディープリンクは hash のみ。

- お気に入り / 最近使った画面が無く、日次作業と稀な作業が同列でスクロールが長い。
- フィルタ条件や選択中 ID が URL に載らない（`operation-history.js:100-112` 等）。リロードで消える。
- ゲーム/編集ペインは `styles.css:1158-1178` の固定グリッドで、ドラッグリサイズ不可（`workspace-layout.js:23` の 3 モード固定）。
- Ctrl+S などのグローバルショートカット無し（`app.js:642-651` は picker モーダルの Esc/Enter のみ）。

### 1-5. 共通コンポーネントの一貫性

- `list-detail.js`（検索・dirty 管理付きシェル）の実採用は `motion-types.js` の 1 画面のみ。他は自前実装で検索 UI も統一されていない（views 配下で `type="search"` ヒットなし）。
- ID 入力欄が素のテキスト（`role-management.js:109`, `account-create.js:109`）と number-spinner / picker で混在。
- 削除確認が `confirm()` 方式と `image-editor.js:186-221` の「二度押し確定」方式で不統一。
- ゲーム画面クリック（`sbop2_admin_pick`）の受け口が `app.js:139-220` の `handleAdminGamePick` と、`map-object-place.js:811-841` 等の view 内 `message` リスナーの二重実装になっている。

### 1-6. WebSocket / 同時編集

- `/ws/admin` は `selection_changed` のみ処理し、接続状態の UI 表示なし（`app.js:58-107`、`console.debug` のみ）。
- 他管理者との競合検知・ロックは無い（`docs/web-admin-migration-status.md` の「まだ受け皿がない重要項目」のまま）。
- Web → ネイティブ方向の pick 通知（`PacketADMIN_MAP_SELECTPICK`）は未送信（`docs/web-admin-native-coexistence.md:83,171`）。

---

## 2. マップ編集系

### map-info.js（マップ情報）
- BGM / 天候が ID 直打ち（`map-info.js:71-85`）。BGM 名一覧・試聴が無い。
- 一覧に検索/ソート/ページング無し（`map-info.js:227`）。
- **マップサイズ変更の UI / API が無い**（`map-info.js` に size 系フィールド無し、Web ハンドラに `RENEWMAPSIZE` 相当無し）。ネイティブの `PacketADMIN_MAP_RENEWMAPSIZE` に対応する機能。

### map-paint.js（タイルペイント）
- Undo はあるが Redo 無し。履歴 50 件・離脱で消える（`map-paint.js:388-522`）。
- マップ切替時のみ dirty 警告。ルート切替時の警告は呼び出し側次第。
- 塗り／矩形塗り／スポイトはあるが、範囲選択コピー＆ペーストは無い（ネイティブ `PacketADMIN_MAP_COPYPARTS` 相当）。

### map-parts-place.js / map-parts-edit.js / map-shadow-edit.js
- パーツ・影カタログの一覧に検索/フィルタ無し。件数が多いと目視で探すしかない。
- 保存/削除の連打防止無し（`map-parts-edit.js:631-660,859-872`, `map-shadow-edit.js:458-487`）。
- アニメフレームは並び替え可だが複製不可。

### map-object-place.js（オブジェクト配置）
- ゲーム画面クリック → マップ自動切替 → セル選択 → 詳細表示の導線は実装済み（`map-object-place.js:811-841`）。
- テンプレートに名前検索/フィルタ無し（`map-object-place.js:150-151`）。一覧に検索ボックス無し。

### map-window.js
- cell 以外のピック種別は「未対応」と明示（`map-window.js:254`）。

### map-events.js（マップイベント）
- **ゲーム画面クリックは既存イベントの検索・選択のみ。「そのセル座標で新規作成」ができず、新規は常に (0,0) 始まり**（`map-events.js:282-283`, `app.js:160-202`）。
- 会話イベント editor 起動用の ID 入力がイベント一覧と連動せず手入力（`map-events.js:118-119`）。
- 保存後 0.6 秒待って一覧へ自動遷移（`map-events.js:361`）。連続編集時は煩わしい可能性あり。

### talk-events.js（会話イベント）
- 新規作成が `prompt()` で ID 直打ち（`talk-events.js:527`）。キャラ picker 無し。
- 残課題は `docs/web-admin-migration-status.md` 保留メモの通り: ADDSKILL のスキル picker、PAGE 条件のアイテム picker、MSG プレビュー、ページ単位グルーピング、Undo、コミット前バリデーション（MENU ジャンプ先の存在確認）。

---

## 3. キャラクター管理系

### 「NPC を 1 体置いて敵として動かす」までの手数
最低 3 画面: NPC 追加 → キャラ一覧で再検索（またはゲーム画面クリック）→ char-edit の移動/グラフィックタブ。追加直後に編集へ飛ぶショートカットが無い。

### character-list.js
- ページング 20 件固定・変更不可（`character-list.js:15,203`）。
- マップは ID 直打ち（`character-list.js:43-45`）。ソート機能無し。
- 一覧から複製・削除・一括操作不可。

### npc-add.js
- 二重送信防止無し（`npc-add.js:229`）。成功時 `showFeedback` が同内容で 2 回呼ばれている（`npc-add.js:283-289`、残骸）。
- 追加後に char-edit へ遷移するリンク無し。
- 種族 ID・モーション種別 ID が直打ち（`npc-add.js:106-107,171-174`）。`char-edit.js` の `openNpcMotionPicker` が流用されていない。
- iframe クリック連携がこの画面では効かない（`app.js:147-159` の分岐対象外。`character-account` / `init-status` も同様）。

### char-edit.js（9 タブ）
- **「NPC発生」タブは読み取り専用（`char-edit.js:1004-1032`、`dl/dd` 表示のみ、form 無し）。サーバー側も `CharacterDetailHandler.cpp:375-380` の GET 出力のみで、PUT で `m_dwPutCycle` 等を書く経路が Web ハンドラに存在しない。** `docs/web-admin-features.md` の「保存可（実機確認済み）」は誤り。
- 「モーション種別ID」が基本タブ（`:205`）と移動タブ（`:987`）に重複。グラフィックタブの NPC picker は basic 側だけ更新（`:844-847,1258`）。
- タブごとの個別保存で、未保存のまま別タブ・別キャラへ移ると消える（1-2 参照）。
- 種族 ID・アイテム ID・スキル ID が直打ち（`:206,1048-1050,1081-1083`）。
- バリデーション表示が `alert()` と `form-feedback` で混在。

### character-account.js
- `char-edit.js` のアカウントタブ（`:1104-1194`）と API・UI がほぼ同一の重複画面。片方だけ直すと食い違う。統合または片方を薄いラッパにしたい。
- disabled 状態表示が固定文言（`character-account.js:166`）。

### init-status.js
- 良好な部類。二重送信防止のみ無し（`init-status.js:248-274`）。

---

## 4. マスタデータ系

### item-types.js
- 使用時エフェクト ID・武器情報 ID が数値スピナーのみ（`item-types.js:207-209,232`）。
- 「詳細パラメータ」が種別（0-6）で意味が変わるのに出し分け無し（`:224-238`）。
- 削除時の参照整合性チェック無し（`:483-503`）。複製無し（ネイティブ `DlgAdminItemTypeList::OnCopy` 相当）。

### item-list.js
- 種別 ID・charId・mapId が直打ち（`item-list.js:84-89,168-174`）。ページング無し。複製無し。

### weapon-list.js
- 攻撃/クリティカルエフェクト ID が **カンマ区切り文字列**（`weapon-list.js:164-178`, `parseIdListFromCsv:43-51`）。typo で壊れる。
- 立ち/すり足モーション ID が数値のみでプレビュー無し（`:141-149`）。作った武器の見た目をこの画面で確認できない。
- 削除時に参照する item-types があっても警告無し。複製無し（ネイティブ `OnCopy` 相当）。

### skill-edit.js
- エフェクト ID（ヒット/4 方向）が数値のみ（`skill-edit.js:205-209,241`）。参照整合性チェック無し。

### effect-edit.js
- 比較的良好（drag-list 並び替え・anime-preview あり）。
- **画像種別（effect32/64）が既存データから復元できず常に effect32 始まり**（`effect-edit.js:313-316` コメント）。設計上の欠陥。
- フレーム一括 wait 変更無し。コマ追加は末尾のみ（`:141-146`）。

### balloon-edit.js
- 1 コマ = 1 レコードのフラット構造で、複数コマの噴出しを作る手順が煩雑。一覧が種別でグルーピングされない（`balloon-edit.js:274-313`）。種別 ID・コマ番号が直打ち（`:98-106`）。

### motion-edit.js / motion-types.js
- 機能は充実（フレーム複製・合成プレビュー・描画順 drag-list）。
- フレーム一括 wait 変更無し。
- モーションリスト削除が 1 件ずつ直列 DELETE で `response.ok` 未確認・途中失敗時のロールバック無し（`motion-edit.js:827-843`）。
- モーション再生プレビューは「要再設計・保留」のまま（`docs/web-admin-migration-status.md`）。

### image-editor.js
- 作り込み深い。新規シート追加のみ明確に未対応（`image-editor.js:84`）。アップロードは同一寸法 PNG 限定（`:159-172`）。

### 共通部品
- **`sound-picker.js` に試聴（再生）機能が無い**（`Audio` / `play` ヒットなし）。効果音 ID を選ぶ全画面に効く。ネイティブの `PacketADMIN_PLAYSOUND` 相当。

---

## 5. 運用・権限系

- **server-dashboard.js**（`:39-53`）: CPU/メモリ/遅延/オンライン人数のみ。接続中プレイヤー一覧・キック・ブロードキャスト等の操作無し。
- **operation-history.js**: メモリ上 500 件・再起動で消える（`:33`）。詳細は JSON 整形のみで diff 無し（`:135-149`）。日付範囲フィルタ・ページング・エクスポート無し。
- **audit-reports.js / failback-guide.js**: プレースホルダーのまま（`:5` コメント「Wave 3 で実装予定」）。メニューから消すか実装するか決める。
- **role-management.js**（`:100-203`）: 自分自身の管理者ロールを外せてしまう。確認ダイアログ無し。`account-create.js` も確認なしで即送信。

---

## 6. ネイティブ版にあって Web 版に無いもの（コードで裏付けが取れた分のみ）

- アイテム種別 / アイテム / 武器の「コピー」ボタン（`DlgAdminItemTypeList.cpp` 等の `OnCopy`）。
- マップパーツの範囲コピー（`PacketADMIN_MAP_COPYPARTS`）。
- マップサイズ変更（`PacketADMIN_MAP_RENEWMAPSIZE`）。
- 効果音の試聴（`PacketADMIN_PLAYSOUND`）。
- スポーナー設定の保存（3 章参照）。
- デバッグ操作: グリッド表示切替 `IDM_DEBUG_GRID*`、当たり判定無効 `IDM_DEBUG_MOVENOBLOCK`（`SboCli/resource.h:373-380`）。
- Web → ネイティブ方向の選択同期。

※ `docs/web-admin-migration-inventory.md` には「未着手」と書かれているが実際は実装済みの項目が多い（影編集・マップ追加・会話イベント・キャラ一覧 API など）。同ファイルは古いので棚卸し時に更新すること。

---

## 7. 着手順の提案

1. **基盤 1 本**: `fetchJson` 拡張（401 共通処理）＋ 共通トースト ＋ 「送信中 disabled」ヘルパー ＋ `beforeunload`/ルート切替の dirty 確認フック。各 view は数行の置き換えで乗る。
2. **スポーナー保存**: `char-edit.js` NPC発生タブを form 化 ＋ `CharacterDetailHandler` に PUT 追加 ＋ `docs/web-admin-features.md` の記述修正。
3. **picker 拡充**: エフェクト picker（既存の共通 picker 群に追加）→ モーション種別 picker → 種族 picker → BGM/効果音 picker（試聴付き）。
4. **導線**: map-events「クリックしたセルで新規作成」、npc-add「追加後に編集へ」。
5. **複製**: item-types / weapon / item / マップパーツ範囲コピー。
6. **一覧統一**: `list-detail.js` 採用拡大 or 検索/ページングの共通化。
7. **運用系**: プレースホルダー画面の整理、監査ログ永続化、ロール自己降格ガード。
