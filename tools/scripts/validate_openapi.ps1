[CmdletBinding()]
param(
    [string[]]$Files,
    [string]$Root = (Resolve-Path (Join-Path $PSScriptRoot '..' '..')),
    [string]$ReportDirectory,
    [switch]$FailFast,
    [string]$RedoclyPackage = '@redocly/cli@latest'
)

# 日本語コメント: スクリプトの厳格モードとエラー処理を設定
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

if (-not $ReportDirectory) {
    $ReportDirectory = Join-Path $Root 'docs/apis/validation_logs'
}
if (-not (Test-Path $ReportDirectory)) {
    New-Item -ItemType Directory -Path $ReportDirectory | Out-Null
}

function Resolve-RelativePath {
    param([string]$FullPath)
    return [System.IO.Path]::GetRelativePath($Root, $FullPath)
}

function Get-DefaultOpenApiFiles {
    param([string]$RootPath)
    $searchRoot = Join-Path $RootPath 'docs/apis'
    if (-not (Test-Path $searchRoot)) {
        return @()
    }
    return Get-ChildItem -Path $searchRoot -Filter '*.yaml' -Recurse | Sort-Object FullName | ForEach-Object { $_.FullName }
}

$npxCommand = Get-Command npx -ErrorAction SilentlyContinue
if (-not $npxCommand) {
    throw 'npx コマンドが見つかりません。Node.js 18 以上をインストールしてください。'
}

if (-not $Files -or $Files.Count -eq 0) {
    $Files = Get-DefaultOpenApiFiles -RootPath $Root
}

if (-not $Files -or $Files.Count -eq 0) {
    Write-Warning '検証対象の OpenAPI ファイルが見つかりません。'
    return
}

$hasFailure = $false

foreach ($file in $Files) {
    try {
        $resolved = (Resolve-Path $file).Path
    } catch {
        Write-Host "=== $file を検証します ===" -ForegroundColor Cyan
        Write-Host '  -> エラー: ファイルが見つかりません。' -ForegroundColor Red
        $hasFailure = $true
        if ($FailFast.IsPresent) {
            break
        }
        continue
    }
    $relative = Resolve-RelativePath -FullPath $resolved
    Write-Host "=== $relative を検証します ===" -ForegroundColor Cyan

    $arguments = @($RedoclyPackage, 'lint', $resolved, '--format=json', '--max-problems=200')
    $output = & $npxCommand.Source @arguments 2>&1
    $exitCode = $LASTEXITCODE

    $logFileName = ($relative -replace '[:\\/]', '__') + '.json'
    $logPath = Join-Path $ReportDirectory $logFileName
    if ($output) {
        $output | Out-File -FilePath $logPath -Encoding utf8
        $logRelative = Resolve-RelativePath -FullPath ((Resolve-Path $logPath).Path)
        Write-Host "  -> 検証ログ: $logRelative"
    }

    if ($exitCode -ne 0) {
        Write-Host "  -> エラー: OpenAPI 検証に失敗しました (exit code $exitCode)" -ForegroundColor Red
        $hasFailure = $true
        if ($FailFast.IsPresent) {
            break
        }
    } else {
        Write-Host '  -> 成功: ルール違反は検出されませんでした。' -ForegroundColor Green
    }
}

if ($hasFailure) {
    exit 1
}
