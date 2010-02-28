call "%VS90COMNTOOLS%vsvars32.bat"

devenv myLib\myLib.vcproj /build Release
devenv SboSockLib\SboSockLib.vcproj /build Release
devenv SboCli\SboCli.vcproj /build Release
devenv SboSvr\SboSvr.vcproj /build Release
devenv SboGrpData\SboGrpData.vcproj /build Release
devenv SboSoundData\SboSoundData.vcproj /build Release
devenv SboLaunch\SboLaunch.vcproj /build Release
devenv SboCopy\SboCopy.vcproj /build Release
devenv Tool\MakeFileList\MakeFileList.vcproj /build Release
call "Release\MakeFileList.exe"

pause
