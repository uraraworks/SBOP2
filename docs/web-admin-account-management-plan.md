# Web管理画面 アカウント整理機能 計画

作成: 2026-09-20

## 背景

管理画面にはアカウント作成(`POST /api/accounts`)と管理者ロール更新(`PUT /api/admin/roles`)しか無く、
**アカウントの一覧・検索が一切できない**。ロール更新画面はアカウントIDを手入力させる作りで、
そのIDを調べる手段が管理画面に無い。放置アカウントの棚卸しもできない。

## 現状の作り(調査結果)

- アカウント実体は `CLibInfoAccount`(メモリ常駐)。永続化は `CSaveLoadInfoAccount` が
  サーバー停止時に `sys_account` / `sys_account_char` を **DELETE + 全INSERT** で書き戻す。
- `CInfoAccount` の保持項目: `m_dwAccountID` / `m_strAccount` / `m_strPassword`(ハッシュ) /
  `m_dwTimeMakeAccount` / `m_dwTimeLastLogin` / `m_dwLoginCount` / `m_nAdminLevel` /
  `m_bDisable`(ログイン拒否) / `m_adwCharID`(キャラIDテーブル) / `m_dwSessionID`(非永続)。
- ログインコード方式は別DB(`sys_account_code` / `sys_account_device`)を `CAccountAuthStore` が直接操作。
  こちらは `sys_account` の全置換とは独立。
- 既存の削除系: `CLibInfoAccount::Delete(DWORD dwAccountID)` あり。
  キャラ側は `DELETE /api/characters/{id}` あり。
- アカウントのログイン拒否トグルは `PUT /api/characters/{id}/disabled` に間借りしている
  (キャラ経由でしか触れない)。

## 設計判断

| 論点 | 決定 |
| --- | --- |
| 「除外(ゴミ箱)」の持ち方 | **管理専用の別テーブル**を新設して状態・理由・日時・実施者を保持。`m_bDisable` はそこから派生させる |
| 完全削除の範囲 | アカウント + 紐付くキャラ + ログインコード + 端末トークンをまとめて削除 |
| パスワード救済 | **ログインコードの再発行**のみ(その場で1回だけ表示)。パスワード再生成・端末トークン全失効は今回見送り |
| 進め方 | 段階実装。S1(一覧+検索) → S2(ゴミ箱/完全削除) → S3(コード再発行) |

`CInfoAccount` に項目を足す案は採らない。旧 .dat 形式の要素テーブル
(`GetDataSize` / `GetDataSizeNo` / `GetWriteData` / `ReadElementData` / `Copy` / `GetTmpData`)
を全部直す必要があり、移植コストに見合わないため。

## S1: アカウント一覧 + 検索

### API: `GET /api/accounts`

クエリ(すべて任意、AND結合):

| パラメータ | 意味 |
| --- | --- |
| `search` | フリーワード。アカウントID完全一致 / アカウント名部分一致 / **所持キャラ名の部分一致** のいずれかに当たればヒット(OR) |
| `q` | アカウント名の部分一致(大文字小文字無視) |
| `accountId` | アカウントID完全一致 |
| `lastLoginBefore` | 最終ログインがこのUNIX秒より前 |
| `lastLoginAfter` | 最終ログインがこのUNIX秒より後 |
| `neverLoggedIn` | `1` で一度もログインしていないアカウントのみ |
| `createdBefore` / `createdAfter` | 作成日時での絞り込み |
| `disabled` | `1`=ログイン拒否のみ / `0`=通常のみ |
| `admin` | `1` で管理者レベル付きのみ |
| `online` | `1` で接続中のみ |
| `minChars` / `maxChars` | 所持キャラ数の範囲 |
| `sort` | `accountId` / `account` / `lastLogin` / `created` / `loginCount` / `charCount`(先頭 `-` で降順) |
| `offset` / `limit` | ページング(既定 limit=100、上限 500) |

レスポンス:

```json
{
  "total": 685,
  "offset": 0,
  "limit": 100,
  "items": [
    {
      "accountId": 12,
      "account": "urara",
      "adminLevel": 2,
      "disabled": false,
      "online": true,
      "charCount": 3,
      "charIds": [101, 102, 103],
      "charNames": ["ほげ", "ふが", "ぴよ"],
      "currentCharId": 101,
      "timeMakeAccount": 1700000000,
      "timeLastLogin": 1758300000,
      "loginCount": 42
    }
  ]
}
```

絞り込み・ソート・ページングはすべてサーバー側で行う(件数が増えても管理画面側が重くならないように)。

### UI: `webroot/js/views/account-list.js`

- ナビ「サーバー管理」グループに「アカウント一覧」を追加(route: `account-list`)。
- 検索フォームの先頭はフリーワード欄1つ(アカウントID / アカウント名 / キャラ名)。
  運用ではゲーム内のキャラ名しか分からないことが多いため、キャラ名から owner を引けるようにする。
- そのほかの条件: 状態(すべて・通常・ログイン拒否) / 管理者のみ / 接続中のみ /
  最終ログイン(「指定日以前」「一度も無し」) / キャラ数。
- よく使う条件はプリセットボタンで置く(例: 「90日以上ログイン無し」「キャラ0件」「一度もログイン無し」)。
- 一覧テーブル: ID / 名前 / 状態 / キャラ数 / キャラ名 / 作成日 / 最終ログイン / ログイン回数 / 操作。
  キャラ名で引いた時にどのキャラでヒットしたか分かるよう、キャラ名列を出す。
- 行から「管理者ロール設定」へアカウントIDを引き継いで遷移できるようにする
  (ロール管理のID手入力を解消する)。

## S2: ゴミ箱と完全削除(予定)

- 新テーブル `sys_account_admin(AccountID PRIMARY KEY, Status TEXT, Reason TEXT, TimeChanged INTEGER, ActorAccountID INTEGER)`。
  `Status` は `banned` / `trashed`(通常状態の行は持たない)。
- 起動時のアカウント読み込み後にこのテーブルを読み、`trashed` / `banned` なら `m_bDisable = TRUE` を立てる。
- `POST /api/accounts/{id}/trash` / `DELETE /api/accounts/{id}/trash`(復帰) / `DELETE /api/accounts/{id}`(完全削除)。
- 完全削除は接続中アカウントには実行させない(先に切断させる)。
- 完全削除の手順: キャラ削除 → `sys_account_code` / `sys_account_device` 削除 →
  `CLibInfoAccount::Delete` → `sys_account_admin` 行削除。すべて監査ログに残す。
- 一覧に「ゴミ箱」タブを置き、まとめて完全削除できるようにする。

## BAN (アカウント一覧から直接)

既存の「ログイン拒否」(`PUT /api/characters/{id}/disabled`) はキャラ側の `m_dwAccountID` が
ログイン中しか設定されないため、**オフラインのキャラには効かない**(`no_account_linked` で400)。
つまり相手がログイン中しか BAN できなかった。これをアカウント側から塞ぐ。

- `POST /api/accounts/{id}/ban` / `DELETE /api/accounts/{id}/ban`。`sys_account_admin` の
  `Status='banned'` を使い、スキーマはゴミ箱と共用する
- BAN とゴミ箱は排他。ゴミ箱の中身への BAN は 409 `already_trashed`
- BAN 済みをゴミ箱に入れるのは許可し、`PrevStatus` / `PrevReason` に控える。
  ゴミ箱から戻すと **BAN 中(理由も元のまま)に復帰する**
- BAN 中のアカウントは通常の一覧に出す(ゴミ箱とは別扱い)

### 安全ガード

BAN・ゴミ箱・完全削除の3つは、対象が以下なら 409 で拒否する:

- 実施者本人 → `cannot_target_self`
- 管理者権限を持つアカウント → `cannot_target_admin`

管理者を BAN すると管理APIが全て401になり、画面から復旧できなくなる(実際に踏んで
SQLite を直接編集する羽目になった)。解除系(BAN解除・ゴミ箱から戻す)には**ガードをかけない**。
復旧手段を塞がないため。管理者アカウントを整理したい場合は、先にロール設定で権限を外す。

## S3: ログインコード再発行

- `POST /api/accounts/{id}/login-code` → 新コードを発行し、**レスポンスで1回だけ**平文を返す
  (`{"code":"ABCD-EFGH-IJKL-MNOP"}`)。保存は `CAccountAuthStore::IssueCode`(SHA-256ハッシュ)。
- **`IssueCode` は既存コードに加えて、そのアカウントの全端末トークンも失効させる。**
  つまり再発行すると本人が今ログイン中の端末も切れる。画面にその注意を明示している。
- 画面には発行済みコードを再表示する手段を持たせない(ハッシュしか残らないので技術的にも不可)。
  平文はサーバーログにも出さない(記録するのはアカウントIDと成否だけ)。
- ゴミ箱・BAN中のアカウントには発行しない(409 `account_disabled`)。ログインできないアカウントに
  コードを渡しても、受け取った側が混乱するだけのため。
- **自分自身と他の管理者は対象にできる**(BAN等と方針が逆)。コードを無くした管理者を
  救済できなくなるため、`CheckMutationGuard` は使わない。
- 操作は1件ずつ(一括にしない)。取り違えると他人のアカウントを乗っ取れてしまうため、
  行ごとのボタンにして、表示パネルにもアカウント名とIDを出す。
- 発行操作は監査ログに残る(誰がどのアカウントのコードを再発行したか)。

## 実装上の注意

- 新規 .cpp は `SboSvr.vcxproj` への追加が必要。Release は PCH 必須なので先頭で `StdAfx.h` をインクルードする。
- `webroot` の修正は `SboSvr/Debug/webroot` に同期しないと動作確認に反映されない。
