@echo off
rem VPS デプロイ用 publish ラッパ。詳細は tools\publish.ps1 を参照。
setlocal
set "SCRIPT_DIR=%~dp0"
pwsh.exe -NoProfile -ExecutionPolicy Bypass -File "%SCRIPT_DIR%tools\publish.ps1" %*
exit /b %ERRORLEVEL%
