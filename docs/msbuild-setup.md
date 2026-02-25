# MSBuild Setup (PowerShell)

最終更新: 2026-02-24

## 目的
- PowerShell 端末から Visual Studio 環境初期化込みで `msbuild` を実行できるようにする。

## 追加スクリプト
- `tools/invoke-msbuild.ps1`
  - `VsDevCmd.bat` と `MSBuild.exe` を自動検出
  - 既定で `/m:1`（PDB競合回避）
- `tools/build-sbocli.ps1`
  - `SboCli/SboCli.vcxproj` 用ラッパー

## 使い方
```powershell
powershell -ExecutionPolicy Bypass -File tools/build-sbocli.ps1 -Configuration Debug -Platform Win32
```

## 補足
- 並列ビルドしたい場合は `tools/invoke-msbuild.ps1` に `-Parallel` を指定する。
- 既存プロジェクトでは並列時に `C1041`（PDB競合）が出る場合があるため、既定はシリアルにしている。
