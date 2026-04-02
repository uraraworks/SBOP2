param(
    [int]$Port = 8080,
    [string]$Root = "out/browser-title"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path -Parent $scriptDir
$rootPath = Join-Path $repoRoot $Root

if (-not (Test-Path $rootPath)) {
    throw "配信ディレクトリが見つかりません: $rootPath"
}

Add-Type -AssemblyName System.Web

$listener = [System.Net.HttpListener]::new()
$prefix = "http://localhost:$Port/"
$listener.Prefixes.Add($prefix)
$listener.Start()

Write-Host "Serving $rootPath"
Write-Host "Open $prefix" -ForegroundColor Green
Write-Host "Ctrl+C で停止します。"

function Get-ContentType([string]$path) {
    switch ([IO.Path]::GetExtension($path).ToLowerInvariant()) {
        ".html" { return "text/html; charset=utf-8" }
        ".js" { return "application/javascript; charset=utf-8" }
        ".wasm" { return "application/wasm" }
        ".css" { return "text/css; charset=utf-8" }
        ".json" { return "application/json; charset=utf-8" }
        ".png" { return "image/png" }
        ".jpg" { return "image/jpeg" }
        ".jpeg" { return "image/jpeg" }
        ".gif" { return "image/gif" }
        ".svg" { return "image/svg+xml" }
        default { return "application/octet-stream" }
    }
}

try {
    while ($listener.IsListening) {
        $context = $listener.GetContext()
        $request = $context.Request
        $response = $context.Response

        try {
            $relativePath = [System.Web.HttpUtility]::UrlDecode($request.Url.AbsolutePath.TrimStart('/'))
            if ($relativePath -eq "favicon.ico") {
                $response.StatusCode = 204
                continue
            }
            if ([string]::IsNullOrWhiteSpace($relativePath)) {
                $relativePath = "sbocli-title.html"
            }

            $safeRelativePath = $relativePath -replace '/', '\'
            $candidatePath = [IO.Path]::GetFullPath((Join-Path $rootPath $safeRelativePath))
            if (-not $candidatePath.StartsWith($rootPath, [System.StringComparison]::OrdinalIgnoreCase)) {
                throw "不正なパスです: $relativePath"
            }

            if (-not (Test-Path $candidatePath -PathType Leaf)) {
                $response.StatusCode = 404
                $bytes = [Text.Encoding]::UTF8.GetBytes("404 Not Found")
                $response.ContentType = "text/plain; charset=utf-8"
                $response.OutputStream.Write($bytes, 0, $bytes.Length)
                continue
            }

            $bytes = [IO.File]::ReadAllBytes($candidatePath)
            $response.StatusCode = 200
            $response.ContentType = Get-ContentType $candidatePath
            $response.Headers['Cache-Control'] = 'no-store, no-cache, must-revalidate, max-age=0'
            $response.Headers['Pragma'] = 'no-cache'
            $response.Headers['Expires'] = '0'
            $response.ContentLength64 = $bytes.Length
            $response.OutputStream.Write($bytes, 0, $bytes.Length)
        }
        catch {
            $response.StatusCode = 500
            $bytes = [Text.Encoding]::UTF8.GetBytes($_.Exception.Message)
            $response.ContentType = "text/plain; charset=utf-8"
            $response.OutputStream.Write($bytes, 0, $bytes.Length)
        }
        finally {
            $response.OutputStream.Close()
        }
    }
}
finally {
    $listener.Stop()
    $listener.Close()
}
