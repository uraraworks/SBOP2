# Phase1 Admin DLL Rollback Template

最終更新: 2026-02-24

## 目的
- ユニット単位の移管変更を、安全に切り戻せる形で管理する。
- 「境界ヘッダ」「プロジェクト定義」「対象ユニット」の3点セットで差分を固定化する。

## 変更セット単位
- 単位名: 例 `Step2-WndToolbar`
- 対象種別:
- `boundary` (ABI / Host API / Loader)
- `project` (`.vcxproj` / `.filters` / `.sln`)
- `unit` (`.cpp/.h` 実装)

## 事前チェック
1. 変更対象ファイル一覧を確定する（追加/更新/削除）。
2. 影響ビルドを確定する（最低 `Debug|Win32`, `Release|Win32`）。
3. フォールバック経路の有無を確認する（DLL未配置時の動作）。

## 実施テンプレート
1. 変更ID:
- `YYYYMMDD-<step>-<unit>`
2. 目的:
- 何を移管/抽象化したかを1行で記載
3. 変更ファイル:
- `boundary`:
- `project`:
- `unit`:
4. ビルド結果:
- `SboCliAdminMfc Debug|Win32`:
- `SboCliAdminMfc Release|Win32`:
- `SboCli Debug|Win32`:
- `SboCli Release|Win32`:
5. 既知制約:
- 未解決依存や後続タスクを列挙

## ロールバック手順
1. `project` 差分を戻す（対象ユニットを DLL 側ビルドから外す）。
2. `boundary` 差分を戻す（API追加を巻き戻す場合のみ）。
3. `unit` 差分を戻す（フォールバック実装を優先維持）。
4. `SboCliAdminMfc` と `SboCli` の `Debug|Win32` を再ビルド。
5. 管理者UI起動と非管理者起動を最小確認。

## 判定基準
- ロールバック後、`SboCli.exe` が起動し通常プレイ可能であること。
- DLL 未配置時フォールバックが維持されること。
- ビルドエラーが追加されていないこと。

