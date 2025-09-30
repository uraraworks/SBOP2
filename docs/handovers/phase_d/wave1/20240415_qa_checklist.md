# 20240415_Wave1_QAチェックリスト

- Wave: Wave 1（サーバー情報 / アカウント追加 / 管理者ロール更新）
- 対象リリース: 2024年4月 Web 管理 Wave1 デプロイ
- 作成者: QA (CCCC)
- レビュワー: QA リード (DDDD)
- 版数: v0.2（排他制約と監査コメントの確認項目を追加）
- 更新日: 2024-04-16

## 1. 共通確認
- [ ] 管理者 SSO 認証がない状態では `/admin` 配下にアクセスできない。
- [ ] `FEATURE_WEB_ADMIN_WAVE1` を無効化すると Web 側メニューが非表示になる。
- [ ] `docs/runbooks/phase_d_failback_plan.md` に沿ってフェイルバック後、再度 Web 管理へ戻れる。
- [ ] OpenAPI 差分チェックスクリプトの結果を `logs/` に保管した。

## 2. Wave1 固有項目
| No. | カテゴリ | 観点 | 期待結果 | 実施状況/備考 |
| --- | --- | --- | --- | --- |
| 1 | サーバー情報 | `/api/server?includeMetrics=true` のレスポンスが 200 でメトリクス配列を返す。 | `metrics` に CPU/メモリの 2 系統が含まれる。 |  |
| 2 | アカウント作成 | 409 エラー時に既存アカウント候補が UI に表示される。 | バックエンドが `error=account_exists` を返す。 |  |
| 3 | ロール更新 | `SERVER_ADMIN` ロールを同時に 2 ユーザーへ付与できない。 | 2 件目は 409 と `conflictRole=SERVER_ADMIN` を返す。 |  |
| 4 | 監査ログ | ロール更新時にコメントを入力すると監査ログへ転記される。 | `OutputDebugString`（暫定監査出力）にコメントが残る。 |  |
| 5 | ロール更新 UI | コメント未入力でも送信でき、既定値は空文字で保存される。 | API が 204 を返し UI が成功トーストを表示する。 |  |

## 3. 添付 / 参照
- テスト結果ログ: `docs/handovers/phase_d/wave1/logs/202404_wave1_manual.xlsx`（作成予定）
- 参照資料: `20240415_Wave1_テスト結果サマリー`, OpenAPI (`docs/apis/admin/wave1/*.yaml`), 運用手順書。

以上。
