# ブラウザ版デバッグ自動操作 API (`window.sbop2Debug`)

> 目的: Claude がブラウザ枠の `javascript_tool` から、ブラウザ版ゲーム(Emscripten)を
> 確実に操作・状態確認できるようにする。通常プレイのユーザーには影響しない。

## 有効化条件

`window.sbop2Debug` は次のいずれかの場合のみ定義される。通常ルートでは定義されない。

- サーバーの `/debug` リダイレクト経由でアクセスした場合(`&debug=1` が自動付与される)
- URL に直接 `?debug=1` を付けた場合

例: `http://localhost:18080/debug?server=localhost:18081`

実装: `tools/emscripten/sbocli-title.shell.html` の
`document.body.classList.contains('debug-mode')` ガード内。

## API 一覧

すべて `window.sbop2Debug.xxx` として提供する。

| API | 引数 | 戻り値 | 説明 |
|---|---|---|---|
| `press(key, holdMs=150)` | `key`: キー名 or 数値VK, `holdMs`: 押下時間(ms) | `Promise<void>` | 1回押して離す。down→holdMs待ち→up→250ms待ち(実機検証済みの間隔)。 |
| `hold(key, ms)` | `key`, `ms`: 押しっぱなし時間 | `Promise<void>` | 指定時間押しっぱなしにしてから離す。 |
| `sequence(steps)` | `steps`: `['down','down','x']` または `[{key,holdMs,waitMs}, ...]` | `Promise<void>` | 複数キーを順番に `press` する。`waitMs` があれば押した後に追加で待つ。 |
| `releaseAll()` | なし | `void` | 押しっぱなしのキーを全解放する(操作を中断する時の後始末)。 |
| `setCharName(text)` | `text`: 文字列 | `void` | キャラ名入力欄にテキストを設定する(`SBOP2_BrowserCharNameSetText`)。 |
| `submitCharName()` | なし | `void` | キャラ名入力を確定する(`SBOP2_BrowserCharNameSubmit`)。 |
| `state()` | なし | オブジェクト or `null` | 現在のゲーム状態を取得する(下記参照)。 |
| `waitFor(fn or cond, timeoutMs=10000)` | `fn`: `state=>bool` または `{gameState:'MAP'}` のような一致条件, `timeoutMs` | `Promise<state>` | 条件成立まで `state()` を100msごとにポーリングし、成立でresolve、タイムアウトでreject。 |
| `clickStart()` | なし | `boolean` | 起動直後の「クリックしてスタート」オーバーレイを押す。 |
| `startWithToken(timeoutMs=15000)` | `timeoutMs` | `Promise<boolean>` | タイトル画面の主ボタン(端末トークンありなら「◯◯としてはじめる」)を押す。 |
| `others()` | なし | オブジェクト or `null` | 自キャラ以外のキャラの移動同期状態 `{t, chars:[{id,npc,x,y,dx,dy,dir,st,pred,sx,sy,rt,spd}]}` を返す。`x/y`=論理座標、`dx/dy`=描画補間後の座標、`sx/sy`=最後の確定同期座標、`rt`=その受信時刻(SDL_GetTicks)。他プレイヤーの見え方を毎フレーム計測する用途(`SBOP2_DebugGetOthersJson`)。 |

`waitFor` の一致条件は `{gameState:'MAP'}` のように `gameStateName` を文字列名で指定できる
(内部で `s.gameStateName === cond.gameState` として比較する)。それ以外のキーは `state()` の値と
そのまま等値比較する。

`startWithToken` はログインウィンドウのフェードイン中の押下を捨ててしまうため、Promise内部で
パネル表示(`loginOverlay` が非表示でなく、ボタンが有効)を待ってからクリックし、1秒待っても
画面が `LOGIN` のまま・パネルも隠れていなければ押し直す、というリトライループになっている。
`timeoutMs` 経過しても進まなければ reject する。

キー名: `'up' 'down' 'left' 'right' 'x' 'z' 'esc'/'escape' 'space' 'enter'/'return' 'shift' 'tab'`、
英数字1文字(`'a'`〜`'z'`, `'0'`〜`'9'`)、または Windows 仮想キーコードの数値。

入力注入は既存の仮想パッド経路 `Module.ccall('SBOP2_BrowserSetPadKey', ...)`
(`SboCli/src/MgrKeyInput.cpp`)を使う。SDL の実キー受け口 `#sbop2ImeInput` を経由しないため、
フォーカス状態に関係なく確実に届く。

## `state()` が返すJSONの例

```json
{
  "gameState": 6,
  "gameStateName": "LOGINMENU",
  "windows": [
    { "id": 3, "name": "LOGINMENU", "active": true, "show": true, "pos": 0 }
  ],
  "activeWindow": { "id": 3, "name": "LOGINMENU" },
  "keyInput": true,
  "player": null,
  "systemMessages": [],
  "messages": []
}
```

マップ画面にいる時の `player` の例:

```json
"player": { "name": "てすと", "mapID": 1, "x": 10, "y": 12, "direction": 2,
            "charID": 3, "equipCloth": 8, "equipArmsRight": 0, "items": [7] }
```

- `gameState` / `gameStateName`: `SboCli/src/SboCli_priv.h` の `GAMESTATE_*`
  (LOGO/TITLE/LOGIN/DISCONNECT/INFO/LOGINMENU/MAP)。
- `windows`: 開いているウィンドウ一覧(`WINDOWTYPE_*` のID/名前、アクティブ/表示フラグ、カーソル位置 `pos`)。
- `activeWindow`: キー入力を受け付けているウィンドウ(無ければ `null`)。
- `player`: マップ画面でのみ非 `null`。名前はUTF-8。`equipCloth` / `equipArmsRight` は装備中のアイテムID、
  `items` はバッグのアイテムID一覧(管理画面から装備を変えたときの反映確認用)。
- `systemMessages`: C++側 `CMgrData` が保持するシステムメッセージの末尾5件(UTF-8)。
- `messages`: ブラウザ側チャットログ(`window.sbop2ChatLogGetSnapshot()`)のスナップショットを
  `state()` 呼び出し時に合成したもの。取得できない場合は `null`。
- 取れない項目は `null` になる。

C++側の実装: `SboCli/src/BrowserMain.cpp` の
`extern "C" EMSCRIPTEN_KEEPALIVE const char *SBOP2_DebugGetStateJson(void)`。
文字列はすべて `.GetUtf8Pointer()` で取得しており、`(LPCSTR)` 素キャストや
`Format(_T("%s"), wide)` は使っていない(ブラウザ版で文字化け/空文字になる既知の罠を回避)。

## 典型手順の例

### ログイン → キャラ作成 → マップ画面 → ESCメニュー

```js
// 1. 起動直後のオーバーレイを押す
sbop2Debug.clickStart();

// 2. タイトル画面(端末トークンありなら1クリックでログイン開始)
await sbop2Debug.waitFor({ gameStateName: 'TITLE' });
sbop2Debug.startWithToken();

// 3. キャラ作成に来たらキャラ名を入力
await sbop2Debug.waitFor(function(s) {
  return s.windows.some(function(w) { return w.name === 'CHARNAME' && w.active; });
});
sbop2Debug.setCharName('てすと');
sbop2Debug.submitCharName();

// 4. 性別選択などは press() で進める
await sbop2Debug.press('x');

// 5. マップ画面に来るまで待つ
const mapState = await sbop2Debug.waitFor({ gameStateName: 'MAP' }, 20000);
console.log(mapState.player);

// 6. ESCでシステムメニューを開く
await sbop2Debug.press('esc');
const menuState = await sbop2Debug.state();
console.log(menuState.windows);

// 後始末
sbop2Debug.releaseAll();
```

## サーバー側テスト準備 API `POST /api/debug/fixture`

実装: `SboSvr/src/Web/Handlers/DebugFixtureHandler.cpp`
(接続元判定は `SboSvr/src/Web/DebugFixtureGuard.cpp`)。

- **Debug ビルドのみ**存在する(`#ifdef _DEBUG`)。Release には含まれない。
- **127.0.0.1 (または `::1`) からの直接リクエストのみ**受け付ける。
  `X-Forwarded-For` / `X-Real-IP` のどちらかのヘッダーが付いていれば
  リバースプロキシ経由とみなし、条件を満たしていても拒否する。
  条件を満たさない場合は存在しないAPIとして **404** を返す。
- 入力は body の JSON で `{account, password, charName}` を任意指定できる(全省略可、
  省略分はランダム生成)。
  - 指定した `account` が既存アカウントの場合、`password` が一致すれば
    そのアカウントで端末トークンを新規発行する(不一致・無効化済みなら **409**)。
    キャラを1つも持っていなければ新規作成し、既に持っていれば1件目をそのまま使う(冪等)。
  - 未使用の `account` ならアカウント・キャラ・端末トークンを新規作成する。
- 出力(200時): `{account, password, deviceToken, accountId, charId, charName}`。
- キャラ作成は `CLibInfoCharSvr::CreatePlayerCharacter`(`MAKECHAR` パケット経路と共用、
  排他も同じ)を使う。

### 実証済みの通し手順(起動〜マップ画面〜ESCメニューまで約13秒)

```js
// 1) テスト準備(どのページからでも呼べる。/api/debug/fixture は同一オリジン)
const j = await (await fetch('/api/debug/fixture', {
  method: 'POST',
  headers: { 'Content-Type': 'application/json' },
  body: '{}'
})).json();
localStorage.setItem('sbop2_device_token', j.deviceToken);
localStorage.setItem('sbop2_device_account', j.account);

// 2) http://localhost:18080/debug を開いてから
const d = window.sbop2Debug;
await d.clickStart();
await d.waitFor({ gameState: 'LOGIN' }, 15000);
await d.startWithToken();
await d.waitFor({ gameState: 'LOGINMENU' });
await d.press('x'); await new Promise(r => setTimeout(r, 600)); await d.press('x'); // キャラ選択 → 1人目
await d.waitFor({ gameState: 'MAP' }, 15000);
await d.press('esc');
d.state(); // activeWindow: SYSTEMMENU、windows[0].pos でカーソル位置
```

### クラウド（Linux）で通しを自動実行する

`tools/test-browser-e2e-linux.sh` が上の通し手順を Playwright で実行し、MAP と ESC メニューのスクショを
`out/e2e/` に保存する。サーバーは `-DSBO_DEBUG_API=ON` でビルドしたもの（fixture 入り）を使う。
手順は `docs/cloud-dev-staging-plan.md` の S5 を参照。

### 注意点

- ブラウザ枠の `computer` によるキー入力は SDL の実キー受け口(`#sbop2ImeInput` 固定)には
  届かない。必ず `sbop2Debug` 経由で操作すること。
- ページ遷移(リロード)を挟むと、直前の `javascript_tool` 呼び出しの戻り値は失われる。
  遷移をまたぐ値(端末トークン等)は `localStorage` 経由で受け渡す。
- `state().systemMessages` は空配列のことが多い。チャット内容は `messages`(ブラウザ側
  チャットログのスナップショット)側に入る。

## 他プレイヤーの見え方を計測する(2人同時ログイン)

- `/api/debug/fixture` でアカウントを2つ作り、同一オリジンのページに iframe を2つ並べて
  それぞれログインさせる。端末トークンは `localStorage` 共有なので、**1つ目がマップに入ってから**
  2つ目用のトークンに書き換えて読み込むこと(先に書き換えると両方同じアカウントになる)。
- ブラウザ枠が非表示だと `requestAnimationFrame` が止まりゲームも進まない。計測中は枠を表示しておく。
- 見る側で `others()` を毎フレーム取り、動かす側の `state().player` と比べると、遅れ・停止・後退が数値で出る。
- 移動系パケットの受信履歴は `Module.ccall('SBOP2_DebugGetMoveTraceJson','string',['number'],[1])`
  (引数1で読み出し後にクリア)。要素は `[受信時刻, charID, 種別, x, y, 向き, update]`、
  種別は 1=MOVE_START 2=MOVE_DIR_CHANGE 3=MOVE_STOP 4=POS_SYNC 5=POS_SYNC(中継が新しいので無視)。
- 1枚の枠で2ゲームを動かすと、枠自体のフレーム落ち(数十〜百数十ms)が混ざる。細かい数値は
  本人側の停止(moverStall)と比べてノイズを差し引いて読む。

