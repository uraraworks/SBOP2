# ログインコード方式への移行計画

アカウント名＋パスワードの入力をやめて、「ログインコード」と「端末トークン」でログインする方式に移行する。

## 目的

- 普段は何も入力せずにゲームを始められるようにする。
- スマホでのパスワード入力・IME・DOM オーバーレイの苦労をなくす。
- localStorage に保存しているパスワード（CryptUtil による難読化のみで、実質平文）を廃止する。

アカウント名は識別子で、秘密ではない。そのため、ID＋PW の2項目を入力させても一要素認証であることに変わりはない。サーバーが生成するランダムなコードは、利用者が決めたパスワードより推測されにくく、他サービスとの使い回しも起きない。

## 用語

| 用語 | 中身 | 保存場所 |
| --- | --- | --- |
| パスワード | 利用者が決める、または自動生成する | サーバー: PBKDF2（既存） |
| ログインコード | サーバーが生成する 16 文字（約 80bit）。例: `K7QM-3XRA-9PWD-HT2F` | サーバー: SHA-256。利用者: ファイル・メモ |
| 端末トークン | コードを入力した端末ごとに発行する 32 バイトの乱数（16 進 64 文字） | サーバー: SHA-256。ブラウザ: localStorage |

- パスワードはコードの発行・再発行に使う。
- コードは新しい端末で端末トークンを作るのに使う。
- 端末トークンはゲームのログインに使う。

## 全体の流れ

```
/（トップ）→ /game/ のタイトル画面
  ├ 端末トークンあり → 「○○さんとしてはじめる」→ トークンでログイン
  │                      └ 無効と返されたら、トークンを消して /account/?reason=expired へ
  ├ 端末トークンなし → 「はじめる」→ /account/
  └ 「別のアカウントで遊ぶ」「アカウント管理」→ /account/

/account/（HTML のページ。ゲームと同じオリジン）
  ├ 新規登録: 名前＋PW（PW は自動生成も選べる）→ コード発行 → 保存 → この端末を登録 → /game/ へ
  ├ ID＋PW でログイン → コードを再発行 → 保存 → この端末を登録 → /game/ へ
  └ コード入力 → この端末を登録 → /game/ へ
```

## 決まったこと（2026-09-18）

- 案2を採用する。タイトル画面は残すが、ログイン入力欄はなくす。入力はすべて `/account/` で行う。
- 新規登録は「名前＋PW」が基本。PW の自動生成も用意する。
  - 保存用ファイルには、名前・PW・コードをまとめて入れる。
- 管理者も、一般アカウントと同じくトークンだけでゲームにログインできる。
  - 管理画面へは別途 ID＋PW でログインしてもらう。
  - ゲームログイン時に管理画面へ自動ログインする `SBOP2RequestWebAdminSession` は廃止する。
- 端末トークンは localStorage に保存する（キー名は `sbop2_device_token`）。
  - IndexedDB と比べて安全性は同じで、EM_JS から扱いやすい。既存の `sbop2_ls_*` も流用できる。
- トークンを URL に載せない。Cookie も使わない（CSRF 対策を不要にするため）。

## コードとトークンの仕様

- **コード**: Crockford Base32（0-9 と A-Z から I, L, O, U を除いた 32 文字）で 16 文字。表示は 4 文字ごとにハイフンで区切る。
  - 入力時は、ハイフン・空白を取り除き、大文字にそろえる。O→0、I/L→1 は同一視する。
  - アカウント名なしで照合するため、総当たりはアカウントをまたいで行える。そのため、IP 単位の試行回数制限を必須にする。
- **再発行**: 古いコードとそのアカウントの全端末トークンを無効にする。コードはハッシュで保存するので、再表示はできない。
- **端末トークン**: 有効期限なし（最終使用日時だけ記録する）。無効になるのは、ログアウト・コード再発行・管理者による失効のとき。
- **サーバー側の保存**: 生成・照合ともに `SboPlatform::GenerateRandomBytes` と `Sha256Hex` を使う（どちらも SboSvr 内限定で、Common から使ってはいけない）。

## 段階

### S1: サーバー側の保存と公開 API

**DB（新規テーブル）**

- `sys_account_code`（AccountID PK, CodeHash, TimeIssued）
- `sys_account_device`（TokenHash PK, AccountID, TimeCreated, TimeLastUsed, Label）
- 発行・失効したら、その場で SQLite に書き込む。
  - 既存の sys_account は「全件消して入れ直す」定期保存だが、同じやり方にすると、保存前にサーバーが落ちたときに発行済みコードが消える。コードは利用者が保存済みなので、消えると困る。

**API**（`/api/account/*`。管理者認証ゲートの対象外にし、IP 単位の試行回数制限をかける）

| メソッド・パス | 入力 | 出力 |
| --- | --- | --- |
| `POST /api/account/register` | account, password | code（新規作成） |
| `POST /api/account/issue-code` | account, password | code（再発行） |
| `POST /api/account/redeem` | code, label | deviceToken, account |
| `POST /api/account/me` | deviceToken | account（タイトル表示用・有効か確認） |
| `POST /api/account/logout` | deviceToken | ok（この端末のトークンを失効） |

**実装上の注意**

- 認証ゲートの例外判定 `IsAuthApiPath`（`SboSvr/src/Web/HttpServer.cpp:288`）に `/api/account/` を加える。
- アカウントを触るときは、`AccountCreateHandler` と同じく `pAccountLib->Enter()` で排他を取る。
- 名前の検証は、ゲームの自動作成と同じ条件（ASCII 0x21〜0x7E）にそろえる。管理者名と同じ名前の登録は拒否する。
- 試行回数制限は、管理画面ログインの「5 回失敗で 300 秒ロック」の仕組みを参考に、IP 単位で行う。
  - IIS 経由のときの実 IP の取り方は、既存の admin-login に合わせる。
- テストは SboSvrTest に追加する。
  - 必ず落ちることを先に確認する。
  - 確認する項目: 生成・正規化・照合・再発行で旧トークンが失効すること・試行制限

### S2: アカウントページ `/account/`

- 置き場所は `SboSvr/webroot/account/`。HttpServer で `/account/` に StaticFileHandler を登録する。
- publish.ps1 の収集対象に含まれているか確認する（管理画面は Debug のコピーを配信するので、同期も忘れない）。
- 画面構成
  - 新規登録
  - ID＋PW でコード発行
  - コード入力
  - この端末からログアウト
- コード発行後の画面
  - 「📄 ファイルで保存」「📤 共有（navigator.share）」「📋 コピー」の3ボタンを置く。
  - 「保存しました」にチェックしないと次へ進めない。
- 保存ファイル（`SBOP2_ログインコード_<名前>.txt`）の中身: 名前・PW（新規登録時のみ）・コード・発行日・URL・注意書き
- コード入力後はトークンを localStorage に保存し、`/`（`?server=` 付きのリダイレクト）へ戻る。
- `?reason=expired` のときは「ログインコードが無効になりました。もう一度入力してください」と表示する。

### S3: ゲーム側をトークンログインに切り替える

- **新しいパケット** `CPacketCONNECT_REQ_LOGIN_TOKEN`（トークン＋MAC ダミー）
  - サーバーは、トークンのハッシュからアカウントを引く。そのあとの処理（拒否判定・ログイン済み判定・RES_LOGIN）は既存の `RecvProcCONNECT_REQ_LOGIN` と共通の関数に切り出す。
  - 失敗時の結果として `LOGINRES_NG_TOKEN` を追加する。
- **クライアント**
  - バージョンチェック応答のあと、トークンがあれば LOGIN_TOKEN を送る。
  - `LOGINRES_NG_TOKEN` を受けたら、トークンを消して `/account/?reason=expired` へ移動する。
- **タイトル画面**
  - `CWindowLOGINBrowser` の DOM 入力欄の代わりに、次のボタンを置く。
    - 「○○さんとしてはじめる」
    - 「別のアカウントで遊ぶ」
    - 「アカウント管理」
  - アカウント名は `/api/account/me` で取得する。
- **ゲーム内の設定メニュー**に「アカウント管理」と「この端末からログアウト」を追加する。

### S4: 後片付け（実施済み・2026-09-18）

- ゲーム内の ID＋PW 入力欄を削除した。
  - `tools/emscripten/sbocli-title.shell.html`: `#loginAccount` / `#loginPassword` / `#loginSaveWrap` / `#loginConnect` などの DOM 要素、それらを隠すだけだった CSS、対応する JS（フォーカス・input・change・Enter キーのイベント結線）を削除。
  - `SboCli/src/Window/WindowLOGINBrowser.{h,cpp}`: `SBOP2_BrowserLoginSetAccount/SetPassword/SetSavePassword/SetFocus`（EMSCRIPTEN_KEEPALIVE 関数）、`SetAccountFromBrowser/SetPasswordFromBrowser/SetSavePasswordFromBrowser/SetFocusIndex`、`NormalizeLoginText`、`m_strAccount`/`m_strPassword`/`m_bSavePassword`/`m_nFocusIndex` を削除。`ILoginWindow`（ネイティブ版と共有）はそのままにし、`GetAccount()`/`GetPassword()` は互換のため空文字を返すだけにした（ブラウザ版はトークンログイン専用で、この2つは呼ばれない）。
- localStorage の旧値を廃止した。
  - `SboCli/src/MgrData.cpp`: `SaveIniData`/`ReadIniData` のブラウザ分岐から `sbop2_account`/`sbop2_password`/`sbop2_savepw` の読み書きを削除し、`ReadIniData` 起動時に3キーとも `removeItem` するようにした（未使用になった `sbop2_ls_get`/`sbop2_ls_set` も削除、`sbop2_ls_remove` のみ残す）。
- 知らない名前でのアカウント自動作成を廃止した（`SboSvr/src/MainFrame/MainFrameRecvProcCONNECT.cpp` の `RecvProcCONNECT_REQ_LOGIN`）。
  - 未登録名は作成せず、既存アカウントのパスワード不一致と同じ `LOGINRES_NG_PASSWORD` を返す（アカウントの有無を区別させない）。
  - 判定を `LoginCode::ShouldAutoCreateAccountOnUnknownLogin()`（常に false、`SboSvr/src/Account/LoginCode.{h,cpp}`）に切り出し、`SboSvrTest/TestLoginCode.cpp` に「false であること」のテストを追加した。true に書き換えると当該テストが落ちることを確認済み。
  - 自動作成に付随していた「管理者名と一致したら ADMINLEVEL_ALL を付与」処理も削除した。DB の `sys_account` を確認したところ管理者アカウント（`admin`, AdminLevel=1）は既に存在しており、影響はない。
  - 既存アカウントの ID＋PW ログイン（`CPacketCONNECT_REQ_LOGIN` / `SBOCOMMANDID_SUB_CONNECT_REQ_LOGIN`）自体は、キャッシュに残った旧クライアントや移行期間のため引き続き受け付ける。受付停止は本番反映後に別途判断する。
- `SBOP2RequestWebAdminSession` は呼び出し元が無かった（`SboSvr/webroot/app.js` は `postMessage` の受信側であり送信側ではない）ため、関数定義ごと `tools/emscripten/sbocli-title.shell.html` から削除した。

### S5: おまかせ登録（アカウント名・パスワードの自動生成）

「アカウント名を考えるのが面倒」という利用者向けに、`POST /api/account/register` に
アカウント名・パスワードをサーバー側で自動生成する経路を追加した
（`SboSvr/src/Web/Handlers/AccountAuthHandler.cpp` の `CAccountRegisterHandler`）。

**API 仕様（`POST /api/account/register`）**

| account | password | 動作 |
| --- | --- | --- |
| 省略/空 | 省略/空 | おまかせ登録。両方をサーバーが自動生成する |
| 指定あり | 指定あり | 従来どおり（後方互換） |
| 片方だけ指定 | — | `400 invalid_request` |

応答は `{"account":"...", "password":"...", "code":"..."}`。
`password` フィールドは、おまかせ登録で自動生成したときだけ含める
（利用者側で指定した場合は、応答に含めなくても本人が知っているため）。

- **アカウント名の自動生成**: `player-` + 小文字英数字6文字。
  文字は `LoginCode::GenerateAutoAccountName()`（`SboSvr/src/Account/LoginCode.{h,cpp}`）が
  ログインコードと同じ Crockford Base32（I, L, O, U を含まない）を使って生成し、
  `LoginCode::NormalizeAccountName()` で小文字化する。表示すると `player-7f3a2b` のような形になり、
  タイトル画面には「player-7f3a2b としてはじめる」のように出せる。
  - **一意性はサーバー側で担保する**。`CAccountRegisterHandler` が `pAccountLib->Enter()` の
    排他区間内で最大10回まで生成をやり直し、`CLibInfoAccount::GetPtr` で衝突と管理者名との
    一致を確認する。クライアントに 409 は返さない（10回とも衝突した場合のみ
    `500 account_name_generate_failed`）。
- **パスワードの自動生成**: `LoginCode::GenerateAutoPassword()` が英数字12文字
  （Crockford Base32、大文字のみ）を `SboPlatform::GenerateRandomBytes` から生成する。
  `PasswordHash::IsAcceptable` の条件（ASCII 0x21〜0x7E）を満たす文字種にしてあるので、
  検証には通常のパスワードと同じ経路を通す。
- **ログ**: 自動生成した名前・パスワードはログに出さない。アカウント名は、従来どおり
  `[AccountRegister] success account=...` の1行だけ出す（自動生成かどうかに関わらず名前は出してよい）。

**連打対策（登録の成功回数制限）**

1クリックで登録できるようになったため、失敗回数制限（IP単位・5回失敗で300秒ロック、
`CIpRateLimiter`）とは別に、**登録が成功した回数**も IP 単位で制限する。

- 同一 IP から **1時間に10件を超える新規登録（成功）は `429 too_many_attempts`**
  （`retryAfterSeconds` 付き）。
- 実装は失敗回数制限と同じ `CIpRateLimiter` クラスを、"失敗"ではなく"登録成功"を記録する
  用途にそのまま転用している。専用のインスタンス
  `GetAccountRegisterSuccessLimiter()`（`AccountAuthHandler.cpp` 内、`CIpRateLimiter(10, 3600)`）
  を新設し、失敗回数制限用のインスタンスとは完全に独立させた。
- IP の取り方は既存の試行回数制限と同じ（`request.clientIp`）。

**テスト**（`SboSvrTest`）

- `TestLoginCode.cpp`: 自動生成アカウント名の形式（`player-`+6文字・使用文字種・紛らわしい
  文字を含まないこと・登録API入口の検証を通ること）、自動生成パスワードの長さ・
  `PasswordHash::IsAcceptable` を通ることを確認。
- `TestIpRateLimiter.cpp`: 登録成功回数制限（10回まで許可・11回目で1時間ロック・
  1時間経過で解除・失敗回数制限とは別インスタンスで独立していること）を確認。
- いずれもテストを追加する際、意図的に実装を壊して当該テストが落ちることを確認済み。

## 移行期間の扱い

- S1〜S3 の間は、旧方式（ゲーム内の ID＋PW）もそのまま使える。
- 既存の利用者は、`/account/` で ID＋PW を入力するとコードが発行され、その端末もそのまま登録される。
- S3 が本番に入ったら S4 に進む。

## 既知の制約

- iOS Safari は、7 日間操作しないとサイトの保存データを消すことがある。その場合はコードの再入力が必要になる。ホーム画面に追加した PWA ならほぼ回避できる。
- コードも PW もなくした場合は救済しない（現状の方針どおり）。
- 保存ファイルには PW とコードが一緒に入る。ファイルを盗まれると、コードを再発行されて本人が締め出される可能性がある。注意書きで「人に見せない」ことを伝える。
