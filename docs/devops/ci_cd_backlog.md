# フェーズC向け CI/CD タスク洗い出し

## 1. 目的
- フェーズCで実装する Web 管理機能基盤の品質を自動的に検証し、リグレッションを防止する。
- 将来的な API/フロントエンド拡張に備えて、ビルド・テスト・配布フローを一元化する。

## 2. 対象パイプライン
| パイプライン | 実行トリガー | 主な処理 | 出力/通知 |
| --- | --- | --- | --- |
| `build-and-test` | Pull Request 作成/更新、`work` ブランチへの push | C++ ビルド、ユニットテスト、静的解析 | GitHub Checks、Slack 通知 |
| `frontend-assets` | `web-ui/` 配下の変更検知、日次定時 | Node.js 依存解決、Lint、ユニットテスト、ビルドアーティファクト生成 | アーティファクト保管、Lint 結果通知 |
| `deployment-smoke` | `main` ブランチへのマージ後 | ステージング環境への自動デプロイ、Smoke テスト、ヘルスチェック | ステージングログ、成功/失敗通知 |

## 3. 必要なツールチェーン
- **C++ ビルド/テスト**
  - CMake 3.26 以上
  - Ninja または MSBuild（Windows 用）
  - vcpkg ベースの依存関係取得
  - 単体テスト用に GoogleTest (v1.14)
  - 静的解析: Cppcheck 2.10、clang-tidy 16
- **フロントエンド**（将来の管理 UI 向け）
  - Node.js 18 LTS、npm 9
  - TypeScript 5.x、ESLint、Vitest
  - ビルドツール: Vite または webpack（要検討）
- **デプロイ/検証**
  - PowerShell 7 系（Windows ステージング向け）
  - Curl 7.88（ヘルスチェック用）
  - newman または hoppscotch CLI（API 自動テスト）

## 4. タスク一覧
1. `.github/workflows/build-and-test.yml` を作成し、CMake + Ninja で `SboSvr` をビルド、単体テスト・Cppcheck を実行する。
2. `tools/scripts/setup_toolchain.ps1` を追加し、CI で必要な Visual Studio Build Tools / Ninja / vcpkg を準備できるようにする。
3. `tests/api/healthcheck.http` を用意し、`newman` を通じたヘルスチェックを `deployment-smoke` 内で実行する。
4. フロントエンド用のプレースホルダーディレクトリ `web-ui/` と `package.json` の雛形を追加し、`frontend-assets` パイプラインの雛形を作る。
5. ステージング向けの PowerShell デプロイスクリプト `tools/scripts/deploy_staging.ps1` を作成し、`deployment-smoke` から呼び出す。
6. Runbook（`docs/runbooks/local_dev.md`）に CI/CD 実行フローと開発者向けチェックリストを追記する。

## 5. 依存関係と注意点
- C++ ビルドは Windows ベースのエージェントを想定しているため、`runs-on: windows-latest` と `runs-on: ubuntu-latest` の両方で検証する必要がある。
- newman 実行には Node.js が必要なため、`deployment-smoke` で Node.js セットアップ手順を明示する。
- セキュリティ上の理由から、デプロイスクリプトで利用する資格情報は GitHub Secrets で管理し、ローカル実行時は `.env.example` を提供する。

## 6. 成果物と引き継ぎ
- 各タスク完了後は該当リポジトリに Pull Request を作成し、本ドキュメントへのリンクと実行ログを添付する。
- CI/CD の設定変更は Ops チームと共有し、週次の進捗会議で実施状況をレビューする。

