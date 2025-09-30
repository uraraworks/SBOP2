# 20240415_Wave1_運用手順書

- 作成者: Ops (BBBB)
- 更新日: 2024-04-15
- レビュワー: Ops リード (EEEE), Dev (YYYY)

## 1. 対象
Wave 1 のサーバー情報表示・アカウント追加・ロール更新機能を本番運用するための手順とフェイルバック方針をまとめる。

## 2. リリース前チェック
1. `admin_web` コンテナの環境変数 `FEATURE_WEB_ADMIN_WAVE1` が `true` であること。
2. OpenAPI スキーマと実装の差分確認 (`tools/scripts/check_api_diff.ps1` を暫定利用)。
3. 管理者ロール `SERVER_ADMIN`, `ACCOUNT_OPERATOR`, `ROLE_MANAGER` のマッピングが Identity サービスに登録済み。

## 3. 通常運用フロー
1. **サーバー情報監視**: `/api/server` から取得したメトリクスを Grafana ダッシュボードに転送し、閾値超過時に Ops PagerDuty へ通知。
2. **アカウント追加**: CS 依頼に基づき権限を持つ管理者が UI から登録。登録後、`ACCOUNT_CREATE` イベントが監査ログに出力されているか確認。
3. **ロール更新**: 定期棚卸しで不要ロールを削除。更新理由 (`comment`) を入力した場合は監査レポートに転記。

## 4. フェイルバック手順
1. フィーチャーフラグ `FEATURE_WEB_ADMIN_WAVE1` を `false` に変更し、Web 管理メニューを非表示にする。
2. MFC 管理ツールを再稼働し、最新の `SboSvr` 接続先に切り替え。ガイドは `docs/runbooks/phase_d_failback_plan.md` を参照。
3. Web 管理で発行された操作ログを `admin_audit` からエクスポートし、MFC 管理ツールにインポート。
4. Ops/CS チャネルで周知し、利用者に再ログインを依頼。

## 5. 障害対応 FAQ
- **API 503 が継続する場合**: `CMgrData` サービスのヘルスチェックを確認し、再起動。改善しない場合は Failback を実施。
- **アカウント登録が 409 で失敗**: 既存アカウントの重複。CS チームに既存アカウントの利用を案内。
- **ロール更新が 409**: 排他制約違反。`ROLE_MANAGER` ロールが他アカウントに付与済みの可能性。棚卸し結果を確認。

## 6. 連絡先
- 当番 Ops: ops-oncall@example.com
- 開発エスカレーション: dev-lead@example.com
- セキュリティ問合せ: security@example.com

以上。
