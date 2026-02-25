param(
    [string]$Configuration = "Debug",
    [string]$Platform = "Win32"
)

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path -Parent $scriptDir

& "$scriptDir\invoke-msbuild.ps1" `
    -ProjectOrSolution "$repoRoot\SboCli\SboCli.vcxproj" `
    -Configuration $Configuration `
    -Platform $Platform

exit $LASTEXITCODE
