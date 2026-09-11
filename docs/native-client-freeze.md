# ネイティブクライアントのビルド対象除外

最終更新: 2026-09-11

## 決定（2026-09-11）

ネイティブクライアントと、その配布まわりのツールをビルド対象から外した。**削除はしていない**。`SboCliAdminMfc` を凍結したときと同じやり方で、`SBO.sln` の `GlobalSection(ProjectConfigurationPlatforms)` から対象プロジェクトの `Debug|Win32.Build.0` / `Release|Win32.Build.0` 行だけを削除し、`ActiveCfg` 行は残した。

## 対象

- `SboCli`（Debug / Release 構成のみ。**`BrowserTitle` 構成は現役のまま**）
- `SboLaunch`
- `SboCopy`
- `Tool/MakeFileList`

既に凍結済みの `SboCliAdminMfc` と合わせて、ビルド対象外は5つになった。

## 理由の実測値

- `SboCli` の .cpp 101本のうち98本をブラウザ版ビルド（`BrowserTitle` 構成）がコンパイルしており、ネイティブ固有は3本だけだった。
- `SDL2/` は `.gitignore` 対象の手動配置で、現状のワークツリーには実体が無い。つまりネイティブ版は現状ビルドすらできない状態だった。

## アップデート配布について

`REQ_FILELIST` / `REQ_FILE` / `REQ_FILELISTCHECK` を使っているのはクライアント本体ではなく **`SboLaunch`**（`SboCli` は一切参照していない）。ブラウザ版は HTTP 配信なのでこの仕組みは不要。

ただし**サーバー側の実装（`MainFrameRecvProcVERSION.cpp` と VERSION パケット3種）は今回残した**。消すのは実運用でランチャーが不要と確認できてからにする。

## これで得られたもの

MFC の現役ユーザーが `SboLaunch` と `MakeFileList` だけだったため、**MFC 依存が消えた**。合わせて SDL2 / Audiere.lib / libpng.lib への依存も実質消える。脱ATL、さらには脱 Visual Studio が現実的な射程に入った。

## 戻し方

1. `SBO.sln` に削除した `Build.0` 行を書き戻す。
2. `SDL2/` と `SDL2_ttf/`（2.32.10、`.gitignore` 対象なので手動配置）を置き直す。

コードは一切消していないので、それだけで元に戻る。

## 注意

ブラウザ版クライアントのビルドは `SboCli` の **`BrowserTitle` 構成**で行われている。`SboCli` を「使っていない」と誤解して `.vcxproj` ごと消さないこと。
