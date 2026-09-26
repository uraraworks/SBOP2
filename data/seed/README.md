# data/seed

ローカル確認・E2E 用のゲームデータ(DB)です。

- `SboData.db`: 本番サーバーと同じワールドデータ(マップ・敵・NPC・アイテム・スキルなど)。
  アカウントとプレイヤーキャラは消してあり、個人情報は入っていません。

## 使い方

`SboSvr` の隣の `SBODATA/` に `SboData.db` としてコピーして起動します。
`tools/test-browser-e2e-linux.sh` は自動でこれを使います(`SBO_E2E_EMPTY_DB=1` で使わずに空の DB から起動)。

```bash
cp data/seed/SboData.db <実行ディレクトリ>/SBODATA/SboData.db
```

`Release/SBODATA/` には置いていません(手元の開発用 DB を上書きしないため。`Release/` は .gitignore 対象でもあります)。

## 入っているもの・いないもの

| テーブル | 中身 |
|---|---|
| `sys_char` | NPC・敵の発生地点など(IsNPC=1)だけ。プレイヤーキャラは 0 件 |
| `sys_account*` | 0 件 |
| `sys_item` | 地面やNPCが持っているアイテム |

作り直すときは、本番の SBODATA のコピーからアカウントとプレイヤーキャラを消し、`VACUUM` して
消したデータがファイルに残らないようにしてください(`deploy/staging/sanitize-db.py` が参考になります)。
入れる前に、メールアドレス・IP・パスワードハッシュなどが残っていないか `strings` などで確認します。
