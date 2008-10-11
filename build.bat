call "%VS90COMNTOOLS%vsvars32.bat"

VCBuild myLib\myLib.vcproj "Release|Win32"
VCBuild SboSockLib\SboSockLib.vcproj "Release|Win32"
VCBuild SboCli\SboCli.vcproj "Release|Win32"
VCBuild SboSvr\SboSvr.vcproj "Release|Win32"
VCBuild SboGrpData\SboGrpData.vcproj "Release|Win32"
VCBuild SboSoundData\SboSoundData.vcproj "Release|Win32"
VCBuild SboLaunch\SboLaunch.vcproj "Release|Win32"
VCBuild SboCopy\SboCopy.vcproj "Release|Win32"
VCBuild Tool\MakeFileList\MakeFileList.vcproj "Release|Win32"
call "Release\MakeFileList.exe"

pause
