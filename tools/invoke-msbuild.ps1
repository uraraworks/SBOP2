param(
    [Parameter(Mandatory = $true)]
    [string]$ProjectOrSolution,
    [string]$Configuration = "Debug",
    [string]$Platform = "Win32",
    [string]$Target = "Build",
    [string]$AdditionalArgs = "",
    [switch]$Parallel
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Get-VsWherePath {
    $candidates = @(
        "$env:ProgramFiles(x86)\Microsoft Visual Studio\Installer\vswhere.exe",
        "$env:ProgramFiles\Microsoft Visual Studio\Installer\vswhere.exe"
    )
    foreach ($path in $candidates) {
        if (Test-Path $path) {
            return $path
        }
    }
    return $null
}

function Resolve-VsDevCmd {
    $vswhere = Get-VsWherePath
    if ($vswhere) {
        $found = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find Common7\Tools\VsDevCmd.bat
        if ($found) {
            return $found | Select-Object -First 1
        }
    }

    $fallback = Get-ChildItem "C:\Program Files\Microsoft Visual Studio" -Recurse -Filter VsDevCmd.bat -ErrorAction SilentlyContinue |
        Select-Object -First 1 -ExpandProperty FullName
    return $fallback
}

function Resolve-MsBuildExe {
    $vswhere = Get-VsWherePath
    if ($vswhere) {
        $found = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\Current\Bin\MSBuild.exe
        if ($found) {
            return $found | Select-Object -First 1
        }
    }

    $fallback = Get-ChildItem "C:\Program Files\Microsoft Visual Studio" -Recurse -Filter MSBuild.exe -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -like "*\MSBuild\Current\Bin\MSBuild.exe" } |
        Select-Object -First 1 -ExpandProperty FullName
    return $fallback
}

$projectPath = Resolve-Path $ProjectOrSolution
$vsDevCmd = Resolve-VsDevCmd
$msbuildExe = Resolve-MsBuildExe

if (-not $vsDevCmd) {
    throw "VsDevCmd.bat not found. Please install Visual Studio Build Tools with C++ workload."
}
if (-not $msbuildExe) {
    throw "MSBuild.exe not found. Please install MSBuild components."
}

$cmd = @(
    "call `"$vsDevCmd`"",
    "&&",
    "`"$msbuildExe`"",
    "`"$projectPath`"",
    "/t:$Target",
    "/p:Configuration=$Configuration;Platform=$Platform",
    $(if ($Parallel) { "/m" } else { "/m:1" })
)

if ($AdditionalArgs -ne "") {
    $cmd += $AdditionalArgs
}

$cmdLine = [string]::Join(" ", $cmd)
Write-Host "[invoke-msbuild] $cmdLine"

cmd.exe /d /s /c $cmdLine
exit $LASTEXITCODE
