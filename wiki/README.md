# wiki/

GitHub Wiki（https://github.com/uraraworks/SBOP2/wiki）の原稿です。
master に入ると `.github/workflows/wiki-sync.yml` が Wiki のリポジトリへ書き写します。

- ファイル名がそのままページ名になります（`遊び方ガイド.md` → 「遊び方ガイド」ページ）。
- 画像は `images/` に置き、本文からは `images/xxx.png` で参照します。
- Wiki はこのフォルダの内容で丸ごと上書きされます。Wiki の画面で直接編集した内容は、次の反映で消えるので、直すときはこのフォルダを直してください。
- この README.md だけは Wiki に書き写しません。
