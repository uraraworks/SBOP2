$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
$output = & $msbuild SboCli\SboCli.vcxproj /p:Configuration=Debug /p:Platform=Win32 /t:Build /m /v:minimal 2>&1
$errors = $output | Select-String -Pattern "error " | Where-Object { $_.Line -notmatch "SboCliAdminMfc" }
$success = $output | Select-String -Pattern "SboCli\.exe"
Write-Host "=== SboCli errors (excluding AdminMfc) ==="
if ($errors) { $errors | ForEach-Object { Write-Host $_.Line } } else { Write-Host "(none)" }
Write-Host "=== Link result ==="
if ($success) { $success | ForEach-Object { Write-Host $_.Line } } else { Write-Host "(no exe output found)" }
Write-Host "=== Done ==="
