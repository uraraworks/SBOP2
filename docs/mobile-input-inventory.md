# スマホ操作（バーチャルパッド）と PC キー操作の棚卸し

> 調査日: 2026-09-11
> 結論: **「全キーをパッドのボタンにする」は不要。先に PC 操作を棚卸しして“正式なアクション一覧”を確定し、パッドはそれに乗せる。** 追加が必要なパッドボタンは実質 2 つ（座る/立つ、向き固定）。

---

## 1. 現状のバーチャルパッド

実装は `tools/emscripten/sbocli-title.shell.html` の DOM オーバーレイ（`#virtualPad`、`initVirtualPad()` 1796 行〜）。JS から `Module.ccall('SBOP2_BrowserSetPadKey')` で仮想キー配列 `g_abyBrowserPad[256]` を更新し、`CMgrKeyInput::Renew()`（`MgrKeyInput.cpp:164-167,227-231`）で SDL のキー状態と OR 合成される。つまり **パッドは「キーを押した事にする」方式**で、C++ 側にパッド専用のロジックは無い。

| パッド部品 | 送る入力 | 備考 |
|---|---|---|
| アナログスティック | ↑↓←→（8 方向スナップ）＋ `SBOP2_BrowserSetPadFacing` で向き | デッドゾーン 18%、50ms 再送 |
| A | Z | |
| B | X | 長押し 2 秒の溜め攻撃も touchstart/touchend 保持で成立 |
| Tab | Tab | 戦闘モード切替 |
| Space | Space | コマンドメニュー / 気絶メニュー |
| 💬（パッド外） | チャットシートを開く | Enter 相当 |

表示判定は `ontouchstart && pointer:coarse`。`?forcepad=1` で PC でも強制表示。ユーザーが恒久的に ON/OFF する設定は無い。

---

## 2. PC 版のキー操作 全量（マップ画面）

`MgrKeyInput::Create()`（`MgrKeyInput.cpp:67-98`）でポーリング対象に登録されているのは
**Z X S A R I B F G H J K L C P M N V @ Enter Esc Tab ↑↓←→ Shift Ctrl Space** の 29 キー。

### 2-1. 機能があるキー

| キー | 動作 | パッドで可能か | 根拠 |
|---|---|---|---|
| ↑↓←→ | 歩行（2 キーで斜め） | ✅ スティック | StateProcMAP.cpp:1779-2027 |
| Shift+方向 | **移動せず向きだけ変える** | △ スティックの facing は移動を伴う | :2954-2957 |
| X | 攻撃 / 溜め攻撃（2 秒長押し）/ 会話 / 拾う / 追従開始 | ✅ B | :2061-2147 |
| Z | 防御（戦闘中）/ 追従要求（非戦闘） | ✅ A | :2185-2209 |
| Tab | 立ち ⇄ 戦闘モード | ✅ Tab | :2559-2631 |
| Space | コマンドメニュー（気絶中は気絶メニュー） | ✅ Space | :2736-2778 |
| R | 座る / 立つ | ❌ | :2225-2271 |
| B | バッグ（アイテムメニュー） | ○ Space メニュー「バッグ(B)」経由 | :2275-2305 |
| F / L | 戦闘スキル / 生活スキル メニュー | ○ Space メニュー「スキル(S)」→ 戦闘/生活 経由 | :2375-2467 |
| J | ステータスウィンドウ | ○ Space メニュー「キャラクター(C)」経由 | :2407-2435 |
| Esc | システムメニュー | ○ Space メニュー「システム(ESC)」経由 | :2529-2555 |
| Enter | チャット入力 | ✅ 💬 ボタン | :2500-2525 |
| N / V | キャラ名 / アイテム名 表示切替 | ○ Esc → オプション → 表示設定 に同じ項目あり | :2309-2371, WindowOPTION_VIEWSET.cpp:70,73 |
| @ | 吹き出しメッセージコマンド送信 | ❌ | :2471-2496 |
| Shift（単独） | 頭上アイコン表示モード | ❌ | :2651-2654 |
| Ctrl（押して離す） | スクロールモード切替（追従 ⇄ 画面固定） | ❌ | :2687-2732 |
| Shift+Ctrl | 視点を自キャラに戻す | ❌ | :2656-2661 |

### 2-2. 登録されているのに何も起きないキー（PC 側の負債）

`S A I G H K C P M` の 9 キーはポーリングされているが `KeyProc` の `switch` に到達しない、または `return FALSE` のみ（`StateProcBase.cpp:57-80,160-178`, `StateProcMAP.cpp:2218-2221`）。

一方、Space のコマンドメニューは項目名に **「キャラクター(C)」「スキル(S)」「バッグ(B)」「マップ(M)」「招待(I)」「パーティー(P)」「ギルド(G)」「システム(ESC)」「ヘルプ(H)」「ＧＭコール」** とショートカット表記を出している（`WindowCOMMANDMENU.cpp:218-286`）。このうち実際にキーが効くのは B / ESC だけで、C / S / M / I / P / G / H は表記だけ。さらにメニュー選択しても動くのは キャラクター / スキル / バッグ / システム の 4 つで、招待は `case 3` がコメントアウト、クエスト / マップ / パーティー / ギルド / ヘルプ / GM コールは選んでも何も起きない（`StateProcMAP.cpp:3816-3831`）。

→ **PC 版自体が「表記と実装が食い違った状態」**なので、これを直さずにパッドへ写すと食い違いをスマホにも持ち込む。

### 2-3. 状態で意味が変わるキー

- マップ画面: X = アクション、Z = 防御。ウィンドウ内: X = 決定、Z / B / Space = キャンセル（`WindowITEMMENU.cpp:307,390`, `WindowCOMMANDMENU.cpp:174,193`）。
- パッドのボタン表記が「Z」「X」というキー名なので、スマホユーザーには「どっちが決定か」が伝わらない。

---

## 3. 判断: パッド全部盛り vs PC 棚卸し

### パッドに全部ボタンを足す案 → 不採用
- 足すべき候補が R / N / V / @ / Shift / Ctrl / Shift+Ctrl / Shift+方向 の 8 種。スマホ画面の下 1/4 に 12 ボタンは現実的でない。
- そのうち N / V はオプション画面に同じ設定があり、B / F / L / J / Esc は Space メニューで到達済み。ボタン化の必要が無い。
- PC 側の負債（2-2）をそのまま引き継ぐ。

### PC 操作を棚卸ししてから乗せる案 → 採用
理由: パッドは「キー注入」方式なので、**PC 側で正式なアクション一覧が決まれば、パッドはその最小集合を写すだけ**で済む。実装量も小さい。

---

## 4. 提案する作業順

### Step 1: PC 操作の棚卸し（C++、小〜中）
1. 死にキー `S A I G H K C P M` を `MgrKeyInput::Create()` の登録から外す、または以下で実キーに接続する。
2. コマンドメニューの表記と実装を一致させる:
   - 動く 4 項目（キャラクター / スキル / バッグ / システム）にはショートカット C / S / B / Esc を実装（C・S は新規、`OnJ`/`OnF` の流用）。
   - 動かない項目（クエスト / マップ / 招待 / パーティー / ギルド / ヘルプ / GM コール）は**メニューから消す**か、「準備中」表示にする。将来機能の予約なら `#ifdef` で隠す。
3. N / V はオプション設定と重複しているので、ショートカットとして残すかどうか決める（残すなら「表示設定」画面に「N/V キーで切替」と注記）。
4. `@` 吹き出しコマンドの現在の用途を確認し、使われていなければ削除、使うなら Space メニュー配下に入れる。
5. Ctrl のスクロールモード・Shift+Ctrl の視点リセット・Shift の頭上アイコンは「視点系」としてまとめ、Space メニュー「システム」配下か表示設定に移す。スマホでは画面固定スクロールの意味が薄いのでパッドには載せない。
6. 成果物: `docs/` に「正式キー割当表」（キー / アクション / メニュー経路 / パッド対応）を 1 枚作り、以後の変更はこの表を更新してから行う。

### Step 2: パッドの最小追加（shell.html、小）
| 追加 | 送る入力 | 理由 |
|---|---|---|
| 「座る」ボタン | R | 戦闘後の回復で頻用。メニュー経由では手数が多すぎる |
| 「向き固定」 | Shift 押しっぱなし相当（トグル or 長押し） | スティックだけでは「動かずに向く」ができない。Shift+方向の代替。実装は `SBOP2_BrowserSetPadKey(VK_SHIFT)` を ON にするだけ |

- 既存 A / B のラベルを「Z」「X」から役割アイコンに変える（マップ中: 盾 / 剣、ウィンドウ中: ✕ / ○）。ウィンドウ開閉時に C++ から `EM_ASM` で JS に状態通知を 1 本足せば切替可能。
- Space ボタンのラベルを「メニュー」に。
- パッド ON/OFF をオプション「入力設定」（`WindowOPTION_INPUTSET.cpp`）に追加し、localStorage に保存。

### Step 3（任意）: アクション層の導入（C++、中）
`StateProcMAP::KeyProc` の `switch(キー)` を「キー → アクション ID」テーブル経由にする。PC のキーコンフィグ、ゲームパッド（`CDInputUtil`）、バーチャルパッドが同じテーブルを共有でき、Step 1 の表がそのままコードになる。今すぐ必須ではないが、Step 1 で表を作った時点で半分できているので、キーコンフィグ機能が欲しくなった時に着手。

---

## 5. レイアウト改善案（縦持ち / 横持ち）

> 2026-09-11 追記。iPhone 16 の Chrome とホーム画面追加（PWA スタンドアロン）の実機スクショから。

### 5-1. 現状の症状と原因

| 持ち方 | 見えている症状 | 原因 |
|---|---|---|
| 縦 | ゲーム画面（幅いっぱいの正方形）の下に約 300px の何もない暗い帯、その下に 💬 ボタン、最下部にパッド。ゲームとパッドが遠く、親指の移動距離が長い | `.canvas-stage` が `min(100vw, 100dvh - var(--pad-h))` で **393px の正方形**になり `flex-start` で上寄せ、パッドは `position:fixed; bottom:0` で最下部固定。間が余る（`shell.html:344-358, 375-385, 391-406`） |
| 横 | ゲーム画面が中央上に **約 230px 角**で極小。パッドが下半分を占め、左右に巨大な余白 | 同じ式で `100dvh(393) - 160 = 233px` になる。横持ちでもパッドが「下に横一列」なので高さを食う |
| 縦 | SYSTEM ログがゲーム画面の左上（DOM ミニログ）と左下（C++ 描画のログ）に二重に出て、地図を隠す | `#chatMiniLog` が `position:fixed; top:8px; left:8px` でキャンバスに重なる（`:608-618`）＋ ゲーム内ログはキャンバス内描画 |
| PWA | ブラウザ UI が無い分さらに縦に余るが、使い道が無い | 同上 |

補足: `<meta name="viewport">` に `viewport-fit=cover` が無く `env(safe-area-inset-*)` も未使用（grep 0 件）。横持ちでノッチ側にボタンを置くと欠ける可能性がある。

### 5-2. 目指す配置

**縦持ち（portrait）** 上から順に:
1. ゲーム画面（幅いっぱいの正方形、現状どおり）
2. **バーチャルパッド**（ゲーム直下。スティック左・ボタン右。高さは現状の 160px でよい）
3. **チャットログ欄**（残りの高さ全部。スクロール可、最新が下。💬 ボタンはこの欄の右下に置く）

→ 親指がゲーム画面のすぐ下で完結し、余った帯がログ欄として仕事をする。ゲーム画面上の DOM ミニログは縦持ちでは非表示にできる。

**横持ち（landscape）** 左から順に:
1. **左カラム**: アナログスティック（縦中央）。上下の余りに 💬 と Tab
2. **ゲーム画面**: `100dvh` 角（393px → 現状の約 1.7 倍）を中央に
3. **右カラム**: Z / X / Space を縦または菱形に配置
4. チャットログは左右カラムの下端に小さく、または横持ちではミニログ（現状のフェード式）を左カラム上に流用

→ iPhone 16 横 852×393 で、ゲーム 393 + 左右 約 230px ずつ。スティック 120px、ボタン 64px は十分収まる。

### 5-3. 実装の方針（shell.html の CSS が中心、C++ 変更なし）

1. **キャンバスの寸法式を向きで分ける**
   - portrait: `width = height = 100vw`（`--pad-h` を引かない。パッドは下に「積む」）
   - landscape: `width = height = 100dvh`
   - `@media (orientation: landscape)` を `body.mobile-pad` 配下に追加。`admin-embed` 用の `--embed-height` 版も同様に分岐。
2. **パッドを fixed オーバーレイからフローレイアウトへ**
   - portrait: `.layout` を `flex-direction: column` のまま、`canvas-stage → #virtualPad → #chatLogArea` の順で積む。`#virtualPad` の `position:fixed; bottom:0` を外し、`padding-bottom: var(--pad-h)` の逃げも不要になる。
   - landscape: `.layout` を `flex-direction: row` にし、`#virtualPad` を **2 つの子（`.pad-left` スティック / `.pad-right` ボタン）に分けて `order` でキャンバスの両脇に置く**。DOM 上は 1 つの `#virtualPad` のままでよく、CSS `display: contents` で子を `.layout` 直下に見せる方法が JS 無改修で済む。
   - チャットシート（`#chatBottomSheet`）と `chat-sheet-open` 時のパッド非表示（`:782`）は現状維持。
3. **チャットログ欄（portrait 用）を新設**
   - `#chatLogArea`（`flex:1; overflow-y:auto`）を追加し、`#chatMiniLog` の行を portrait ではここへ流す（`appendMiniLog` 相当の関数で出力先を切り替える。`:2159-2176` 付近）。フェードは portrait では無効、landscape では現状どおり。
   - `#chatOpenBtn` の `bottom: calc(var(--pad-h) + 8px)`（`:641`）は fixed 前提なので、portrait では `#chatLogArea` 内の右下へ移す。
   - キャンバス内の C++ 描画ログ（左下の SYSTEM 行）は残してよいが、二重表示が気になるなら `sbop2_set_mobile_layout` の postMessage/EM_JS で「モバイル時はキャンバス内ログを描かない」フラグを C++ に渡す（`WindowOPTION_VIEWSET` の「発言を表示する」相当の既存フラグを流用できる可能性あり。要確認）。
4. **safe-area 対応**
   - `<meta name="viewport" content="... viewport-fit=cover">` を追加し、landscape の左右カラムに `padding-left/right: env(safe-area-inset-left/right)`、portrait の最下部に `env(safe-area-inset-bottom)`。
5. **iOS キーボード追従との整合**
   - `body.mobile-pad` の `position:fixed + --app-height` 方式（`:314-328`、[[project_browser_mobile_keyboard]]）はそのまま。キャンバス寸法を `--app-height` に連動させない方針（`:377-380` のコメント）も維持し、キーボード表示時は portrait の下側（ログ欄）が隠れるだけにする。
6. **回転時の再計算**
   - `orientationchange` / `resize` で `updateIntegerGameScale()`（`:1017`）相当を呼び直す。現状 `--game-scale` は PC 向けの整数スケールで、mobile-pad では CSS 側の `min()` が効いているので、mobile-pad 時はスケール計算をスキップしてよい。

### 5-4. 手順の目安

| 順 | 作業 | 規模 |
|---|---|---|
| 1 | portrait: キャンバス式変更 ＋ パッドをフローに ＋ ログ欄新設 | M |
| 2 | landscape: `orientation` メディアクエリで row レイアウト ＋ パッド左右分割 | M |
| 3 | safe-area ＋ 回転時再計算 ＋ PWA スタンドアロン確認 | S |
| 4 | Step 2（4 章）の「座る」「向き固定」ボタン追加は、この新レイアウト確定後に右カラム/右グリッドへ足す | S |

### 進捗

> 2026-09-13 追記。

- 5-4 の 1〜3 完了: 縦持ちはキャンバスを 100vw の正方形にしてパッドをフローで直下に積み、残りの高さを新設のチャットログ欄 `#chatLogArea` が埋める（未ログインでも空で表示。ミニログは縦持ちではログ欄へ出す）。横持ちは `@media (orientation: landscape)` で `.layout` を横並びにし、`#virtualPad` を `display: contents` にしてスティックとボタンをキャンバスの左右に配置。切り替えは `body.mobile-pad` と向きだけに依存（pointer/タッチ判定には依存しない）。`viewport-fit=cover` と `env(safe-area-inset-*)`、回転時の再描画、mobile-pad 時の整数スケール計算スキップも実施。
- 不具合修正: `initVirtualPad` が `#virtualPad` に `style.display='flex'` を直書きしていたため横持ちの `display: contents` が効かず、キャンバスが幅 2px に潰れていた → 直書きを削除して CSS に委譲。
- 未確認（実機のみ）: iPhone のノッチ側の欠け、PWA スタンドアロン、iOS キーボード表示時の挙動。
- 次: 4 章 Step 2（「座る」「向き固定」ボタン、ボタン表記の役割化、パッド表示の ON/OFF 設定）。

### 5-5. 検証チェックリスト（実機）
- 縦: ゲーム下端とパッド上端が接している / ログ欄が下に出る / チャットシートを開いてもパッドが消えて入力欄が見える
- 横: ゲームが画面高いっぱい / スティックとボタンが左右 / ノッチ側のボタンが欠けない
- 回転: 縦⇄横で崩れず、ゲーム画面が即座に再配置される
- PWA（ホーム画面追加）: Chrome と同じ配置になる
- 管理画面埋め込み（`admin-embed`）: PC 側のレイアウトが変わっていない

---

### 5-6. iOS Chrome のキーボード上に出る「鍵 / カード / ピン」アイコン列

> 2026-09-11 追記。Chrome でチャット入力欄をタップすると、キーボードの上にパスワード・クレジットカード・住所の自動入力アイコンが出て入力欄が狭くなる件。

**正体**: Chrome for iOS 自身が `<input>` / `<textarea>` にフォーカスが入ると必ず出す **ネイティブの自動入力アクセサリバー**。ページ側の CSS や属性では消せない。`#chatSheetInput` には既に `autocomplete="off"`・`autocapitalize="off"`・`autocorrect="off"`・`spellcheck="false"` が付いている（`shell.html:898-900`）が、これらは候補表示を抑えるだけでバーそのものは消えない。Safari と PWA スタンドアロン（WebKit）ではこのバーは出ない（Safari が出すのはパスワード欄と判定した時だけ）。

**消す方法（有力）**: Chrome の自動入力スクリプトはフォーム部品（input/textarea/select）だけを監視するので、**入力欄を `<div contenteditable="plaintext-only">` に置き換える**とバーが出ない見込み。実機未確認なので、まず `chatSheetInput` だけを差し替えて確認する。

差し替え時に必要な対応:
- 送信テキストは `el.textContent` から取る。改行は送らないので `Enter` の `keydown` で `preventDefault` → 送信。`enterkeyhint="send"` は contenteditable にも効く。
- プレースホルダは `[contenteditable]:empty::before { content: attr(data-placeholder); }` で代替。
- IME 変換中の Enter 誤送信を防ぐため `isComposing` / `compositionend` を見る（`#sbop2ImeInput` 側で既にやっている処理と同じ考え方、[[project_browser_dom_overlay_coords]]）。
- ペーストはプレーンテキスト化（`plaintext-only` 非対応ブラウザ向けに `paste` で `insertText`）。
- `focus()` で iOS のソフトキーボードを出す挙動は input と同じ（ユーザー操作起点であれば可）。既存の「シートを開く → 同期フォーカス」（`:2297-2316`）はそのまま使える。
- `chatOpenBtn` 経由以外で `focusGameInput()` が奪うフォーカス制御（`isChatInputActive()`、`:1012-1015`）の id 判定に新要素を追加する。

**代替（消せない前提の緩和策）**:
- キーボード表示時にチャットシートの高さを `visualViewport.height` に合わせて縮め、ログ行数を減らして入力欄を必ず見せる（スクショでは入力欄の下端がバーに隠れかけている。`--app-height` 追従はしているが、シート内の入力欄が最下部固定でないと欠ける）。
- 案内文で「ホーム画面に追加して起動すると出ません」と伝える。PWA で出ないことは要実機確認。

**判断**: contenteditable 化は M 規模で、5-3 のレイアウト改修と同時に入れるのが手戻りが少ない。先に「PWA / Safari で出ないか」を実機で確認し、出ないなら Chrome だけの問題として優先度を下げてもよい。

---

## 6. 未確認事項
- Chrome for iOS の自動入力バーが `contenteditable` で本当に出ないか、PWA スタンドアロンで出ないか（5-6）。
- スティック操作中に別指で A/B を押す複合操作の実機挙動（イベント構造上は独立に処理される）。
- `@` 吹き出しコマンドの実際の用途（サーバー側の受け口が生きているか）。
- DirectInput ゲームパッド（`MgrKeyInput.cpp:163-191`）のボタン割当の詳細。
