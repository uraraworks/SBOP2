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
| `Dockerfile` | SboSvr をビルドし、webroot・ブラウザ版・`.dat` と一緒に入れる |
| `compose.yaml` | Caddy と SboSvr の2つのコンテナ |
| `Caddyfile` | TLS・Basic 認証・振り分け |
| `staging.env.example` | ドメインと Basic 認証の設定の雛形 |
| `prepare-browser.sh` | ブラウザ版に wss シムを入れ、`.br`/`.gz` を作り直す |
| `deploy.sh` | ホストで SboSvr のイメージを入れ替える(`--image` で Actions が作ったイメージを読み込む。無ければホストでビルド) |
| `sanitize-db.py` | 本番 DB から個人情報を除いたコピーを作る |
| `load-db.sh` | ホストでステージングの DB を入れ替える |
| `../../.github/workflows/staging-deploy.yml` | master への push で自動デプロイ |

## 初回のセットアップ

### 1. ホストを用意する(Google Cloud の無料枠)

Compute Engine で VM を1台作る。無料枠の条件は変わることがあるので、作る前に公式の「Google Cloud の無料枠」の説明で確かめること。
念のため「お支払い → 予算とアラート」で少額の予算アラートも作っておく。

| 項目 | 設定 |
|---|---|
| リージョン | `us-west1`・`us-central1`・`us-east1` のどれか(無料枠は米国のこの3つだけ) |
| マシンタイプ | `e2-micro` |
| ブートディスク | Ubuntu 24.04 LTS(x86/64)、**標準永続ディスク**、30GB 以内(バランス永続ディスクは無料枠の対象外) |
| ファイアウォール | 「HTTP トラフィックを許可」「HTTPS トラフィックを許可」にチェック |
| 外部 IP | ドメインを向けるので、VPC ネットワーク → IP アドレスで「静的」に昇格しておく |

メモリが 1GB しかないので、C++ のビルドはホストでは行わない(GitHub Actions でイメージを作って送る)。
念のためスワップを作っておく。

```bash
sudo fallocate -l 2G /swapfile && sudo chmod 600 /swapfile
sudo mkswap /swapfile && sudo swapon /swapfile
echo '/swapfile none swap sw 0 0' | sudo tee -a /etc/fstab
```

Docker を入れる。

```bash
curl -fsSL https://get.docker.com | sudo sh
sudo usermod -aG docker "$USER"   # 入り直すと sudo なしで docker が使える
```

通信量について: 無料で外に出せるのは月 1GB 程度。ブラウザ版の初回読み込みは圧縮後で約 10MB あるので、
キャッシュ無しの読み込みで月 100 回くらいが目安。Actions からの転送(ホストへの受信)は数えない。

ほかの Linux ホスト(国内 VPS など)でもそのまま使える。arm64 のホストでは Actions のイメージ(x86_64)が動かないので、
ホストにリポジトリを置いて `deploy.sh` を引数なしで実行する(ホストでビルドする)。

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
| Secret | `SBOP2_STAGING_SSH_KEY` | デプロイ専用の SSH 秘密鍵(`base64 -w0 <鍵ファイル>` の1行がおすすめ。そのままの形でもよい) |
| Secret | `SBOP2_STAGING_KNOWN_HOSTS` | `ssh-keyscan <ホスト>` の出力 |

デプロイ専用の鍵は `ssh-keygen -t ed25519 -f deploykey -N "" -C <SSHユーザー名>` で作る。
公開鍵(`deploykey.pub`)の登録先はホストによって違う。

- Google Cloud: コンソールの VM の「編集 → セキュリティとアクセス → SSH 認証鍵」に1行で登録する。
  鍵の末尾(コメント)がログインユーザー名になる。**`~/.ssh/authorized_keys` に手で足すと、
  ブラウザの SSH で入った時などに GCP のエージェントが書き直して消してしまう。**
- ほかのホスト: `~/.ssh/authorized_keys` に足す。

秘密鍵(`deploykey`)は Secret に登録したらホストから消す。

ブラウザの SSH 画面から鍵を `cat` してコピーすると、改行や行末の空白が崩れて
`Load key ...: error in libcrypto` で失敗することがある。`base64 -w0` の1行ならコピーで崩れない。

`SBOP2_STAGING_HOST` が未設定の間、ワークフローは何もしない。

### 5. 初回デプロイ

Actions の「staging-deploy」を手動実行する(以後は master への push で自動)。
ブラウザ版のビルド → wss シム → イメージ作成 → 転送 → `deploy.sh --image`(読み込み・起動・`/health` 確認)の順に進む。
ホストには `~/sbop2-staging/deploy/staging` だけが置かれる。

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
- BGM(`Release/BGM/*.ogg`)は git 管理外でイメージに入らない。ホストの `~/sbop2-bgm` を `webroot/game/BGM` としてマウントしているので、
  そこへ ogg を置けば鳴る(置いた時点で反映。デプロイでは消えない)。手元の Windows から送る例:
  `scp -i <鍵> Release\BGM\*.ogg <SSHユーザー>@<ホスト>:~/sbop2-bgm/`。置いていなければ 404 になるだけで動作には影響しない。
- Caddy だけを作り直すと SboSvr のネットワークが切れるので、その時は `docker compose up -d --force-recreate` で両方作り直す。
