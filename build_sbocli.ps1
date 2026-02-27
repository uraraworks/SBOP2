# SboCli Debug リビルドスクリプト
$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
$proj = "C:\Users\masakazu\MyProject\SBOP2\SboCli\SboCli.vcxproj"
$log = "C:\Users\masakazu\MyProject\SBOP2\build_debug.log"
& $msbuild $proj /p:Configuration=Debug /p:Platform=Win32 /t:Rebuild /m:1 /flp:"logfile=$log;verbosity=minimal"
Write-Host "EXIT: $LASTEXITCODE"
Get-Content $log | Select-Object -Last 20
