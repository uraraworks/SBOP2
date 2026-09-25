# SBOP2 ステージング

無料枠などの Linux ホストで SboSvr とブラウザ版を動かし、スマホやブラウザから触れるようにするための一式。
計画は `docs/cloud-dev-staging-plan.md` の S6。

```
ブラウザ ──https──▶ Caddy(80/443、TLS・Basic 認証)
                     ├─ /__sbop2bridge__ ─▶ 127.0.0.1:18081(ゲームの WebSocket)
                     └─ それ以外        ─▶ 127.0.0.1:18080(HTTP サーバー)
                                              SboSvr(Docker、_DEBUG なし)
```

- 本番の IIS と同じ振り分けにしている。ブラウザ版は https で開かれると `ws://…:18081` を
  `wss://<同じホスト>/__sbop2bridge__` に書き換える(`prepare-browser.sh` が入れるシム)。
- 公開するのは 80/443 だけ。ゲームポート 2006 と 18080/18081 は外に出さない。
- Basic 認証はサイト全体にかける。ただし `/__sbop2bridge__` だけは外す(iPhone の Safari は
  WebSocket に Basic 認証を付けないため)。ここはゲーム自体のログインで守られる。
- DB は Docker の名前付きボリューム `sbop2-staging_sbodata` に入る。イメージを作り直しても消えない。

## ファイル

| ファイル | 役割 |
|---|---|
| `Dockerfile` | SboSvr をビルドし、webroot・ブラウザ版・`.dat` と一緒に入れる(ホストの CPU を問わない) |
| `compose.yaml` | Caddy と SboSvr の2つのコンテナ |
| `Caddyfile` | TLS・Basic 認証・振り分け |
| `staging.env.example` | ドメインと Basic 認証の設定の雛形 |
| `prepare-browser.sh` | ブラウザ版に wss シムを入れ、`.br`/`.gz` を作り直す |
| `deploy.sh` | ホストで SboSvr のイメージを作り直して入れ替える(Actions からも呼ぶ) |
| `sanitize-db.py` | 本番 DB から個人情報を除いたコピーを作る |
| `load-db.sh` | ホストでステージングの DB を入れ替える |
| `../../.github/workflows/staging-deploy.yml` | master への push で自動デプロイ |

## 初回のセットアップ

### 1. ホストを用意する

例: Oracle Cloud Always Free の Ampere A1(arm64)+ Ubuntu 24.04。x86_64 でもそのまま動く。

```bash
# Docker(公式の手順)
curl -fsSL https://get.docker.com | sudo sh
sudo usermod -aG docker "$USER"   # 入り直すと sudo なしで docker が使える
```

80/443 を開ける。Oracle Cloud は2か所ある。

- VCN のセキュリティ・リスト(またはネットワーク・セキュリティ・グループ)に TCP 80/443 のイングレスを足す
- Ubuntu イメージの iptables も既定で閉じているので開ける:

```bash
sudo iptables -I INPUT 6 -p tcp -m multiport --dports 80,443 -j ACCEPT
sudo iptables -I INPUT 6 -p udp --dport 443 -j ACCEPT
sudo netfilter-persistent save
```

### 2. ドメインを向ける

ステージング用のホスト名の A レコードをホストの IP に向ける。証明書は Caddy が Let's Encrypt から自動で取る。

### 3. 設定ファイルを置く

```bash
mkdir -p ~/sbop2-staging/deploy/staging
cd ~/sbop2-staging/deploy/staging
# staging.env.example の中身をコピーして staging.env を作り、値を埋める
docker run --rm caddy:2 caddy hash-password --plaintext 'Basic 認証のパスワード'
```

`staging.env` は raw で読むので、ハッシュの `$` はそのまま書いてよい。

### 4. GitHub の設定

Settings → Secrets and variables → Actions に次を登録する。

| 種類 | 名前 | 値 |
|---|---|---|
| Variable | `SBOP2_STAGING_HOST` | ホスト名か IP |
| Variable | `SBOP2_STAGING_USER` | SSH ユーザー(docker グループに入っていること) |
| Secret | `SBOP2_STAGING_SSH_KEY` | デプロイ専用の SSH 秘密鍵 |
| Secret | `SBOP2_STAGING_KNOWN_HOSTS` | `ssh-keyscan <ホスト>` の出力 |

デプロイ専用の鍵は手元で `ssh-keygen -t ed25519 -f sbop2-staging -N ""` で作り、
公開鍵をホストの `~/.ssh/authorized_keys` に足す。

`SBOP2_STAGING_HOST` が未設定の間、ワークフローは何もしない。

### 5. 初回デプロイ

Actions の「staging-deploy」を手動実行する(以後は master への push で自動)。
ブラウザ版のビルド → wss シム → 転送 → `deploy.sh`(イメージ作成・起動・`/health` 確認)の順に進む。

## DB を本番のコピーにする

1. 本番の `SBODATA` を手元にコピーする(`SboData.db` と、あれば `-wal`/`-shm`、`SboGrpData.db`)。
2. 個人情報を除く(Windows の Python でも動く):

   ```bash
   python3 deploy/staging/sanitize-db.py <本番SBODATAのコピー> <出力先> --admin <管理者名>
   ```

   - アカウント・端末トークン・コード・BAN 情報・拒否 MAC・プレイヤーキャラ(所持品・スキル含む)・
     画像の編集者名と下書きを消す。マップ・NPC・アイテム種別・画像などは残す。
   - `--admin` を付けると管理者アカウントを1件作る。パスワードは対話入力
     (または環境変数 `SBOP2_ADMIN_PASSWORD`)。
   - ログ(`SboSvrLog.txt`、IP・MAC が出る)・ini・旧 `.dat` はコピーしないこと。
3. 出力先をホストに送り、入れ替える:

   ```bash
   scp -r <出力先> <ホスト>:~/sbop2-db
   ssh <ホスト> ~/sbop2-staging/deploy/staging/load-db.sh ~/sbop2-db
   ```

## 運用

```bash
cd ~/sbop2-staging/deploy/staging
docker compose ps
docker compose logs -f sbosvr          # SboSvr のログ
docker compose restart sbosvr          # 再起動(SIGTERM で DB を保存してから止まる)
docker run --rm -v sbop2-staging_sbodata:/d -v "$PWD":/o ubuntu tar czf /o/sbodata-backup.tgz -C /d .   # DB のバックアップ
```

## 注意

- ステージングは `_DEBUG` なしでビルドする(`-DSBO_DEBUG_API=ON` は使わない)。
- BGM(`Release/BGM`)は git 管理外なので、ステージングでは鳴らない(404 になるだけで動作には影響しない)。
- Caddy だけを作り直すと SboSvr のネットワークが切れるので、その時は `docker compose up -d --force-recreate` で両方作り直す。
