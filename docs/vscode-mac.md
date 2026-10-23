# VSCode で開く場合（Mac）

SBOP2 フォルダを VSCode で開いて、ビルド・実行・デバッグを行う手順の要点。

1. 推奨拡張を入れる（開くと案内が出る）: CMake Tools / clangd / CodeLLDB。C/C++（cpptools）は clangd と競合するので入れない。
2. コマンドパレットで `CMake: Select Configure Preset` → `mac-debug` を選ぶ。
3. `Cmd+Shift+B` でサーバーをビルド（webroot 同期まで行う）。
4. `F5` で「SboSvr をデバッグ（headless）」を起動する。テストは「SboSvrTest をデバッグ」または「サーバー: テスト」タスク。
5. ブラウザ版は「ブラウザ版: ビルド」（`~/emsdk`、環境変数 `EMSDK` があればそちら）。やり直しは「ブラウザ版: 全再ビルド」。確認は http://localhost:18080/game/sbocli-title.html 。
6. サーバーは DB を書き戻すため、「サーバー: 停止」タスク（`./SboSvr --stop`）で止める。
