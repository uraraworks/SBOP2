# Web 管理画面向けマップパーツ画像の配置方法

ブラウザ版管理画面では、マップパーツ画像を <code>SboSvr/webroot/assets/map-parts</code> 配下から読み込みます。元の MFC 版と同じ PNG を再利用しますが、リポジトリには追加しない方針のため、ビルド工程でコピーする運用を行ってください。

2024 年時点では、SboSvr.vcxproj のビルド後イベントにより <code>SboGrpData/res/MapParts</code> → <code>SboSvr/webroot/assets/map-parts</code> へのコピーが自動化されています。ソースフォルダが存在しない場合は警告のみ表示されるため、開発環境に必要な PNG をあらかじめ配置しておいてください。

## 前提

- 32×32 / 32×48 などのパーツ PNG は、リポジトリ外または既存の <code>SboGrpData/res</code> などに保管しておく。
- Web 管理画面用の配信用ディレクトリ <code>SboSvr/webroot/assets/map-parts</code> は Git では追跡されない (.gitignore に追加済み)。

## 自動コピー用スクリプト

`tools/sync_map_parts_assets.py` を追加しました。既存データから PNG をコピーして Web 版に必要な構成を作る用途を想定しています。

```bash
# 例: 既定の場所 (SboGrpData/res/MapParts) からコピー
python tools/sync_map_parts_assets.py

# 例: 任意フォルダからコピーしつつ出力先を指定
python tools/sync_map_parts_assets.py --source /path/to/map_parts --dest ./SboSvr/webroot/assets/map-parts

# 既存の出力を掃除してから再コピーしたい場合
python tools/sync_map_parts_assets.py --clean
```

主な動作:

- 指定されたソース配下の <code>.png</code> ファイルを再帰的に探索し、ディレクトリ構造ごとコピー。
- 既定では <code>SboGrpData/res/MapParts</code> → <code>SboSvr/webroot/assets/map-parts</code>。
- `--clean` を指定すると出力先の既存ファイルを削除してからコピー。
- 画像が一枚も見つからない場合は警告を出して終了。

## ビルド手順への組み込み例

1. 既存のアセットリポジトリやファイルサーバーから PNG を取得し、<code>SboGrpData/res/MapParts</code> に配置する。
2. Visual Studio で SboSvr プロジェクトをビルドすると、ビルド後イベントにより PNG が <code>SboSvr/webroot/assets/map-parts</code> へコピーされる。
3. Web 管理画面を配布する際は、<code>webroot/assets/map-parts</code> を含めてデプロイする。

ソースが別の場所にある場合や、手動でコピー内容を確認したい場合は引き続き `tools/sync_map_parts_assets.py` を利用できます。

API からマップパーツのメタデータを返せない場合でも、サンプルデータが描画されるようにしているため、まずはコピー手順のみ整備しておけば UI の確認が可能です。
