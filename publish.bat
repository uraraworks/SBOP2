@echo off
rem VPS デプロイ用 publish ラッパ。詳細は tools\publish.ps1 を参照。
setlocal
set "SCRIPT_DIR=%~dp0"
pwsh.exe -NoProfile -ExecutionPolicy Bypass -File "%SCRIPT_DIR%tools\publish.ps1" %*
set "RC=%ERRORLEVEL%"
if not "%RC%"=="0" (
    echo.
    echo publish に失敗しました ^(exit=%RC%^)。上のエラーを確認してください。
    echo SboSvr のビルド失敗なら publish_build.log に詳細があります。
    pause
)
exit /b %RC%
