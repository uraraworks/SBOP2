# SboSvr / SboSvrTest の非Windows（Linux）ビルド定義
#
#   cmake -S . -B out/cmake-linux -DCMAKE_BUILD_TYPE=Release
#   cmake --build out/cmake-linux -j
#   ctest --test-dir out/cmake-linux --output-on-failure   （SboSvrTest を実行）
#
# Windows の正式ビルドは従来どおり SBO.sln（Visual Studio）で行う。
# ここはクラウドセッション／ステージング用（docs/cloud-dev-staging-plan.md の S2）。
#
# 方針
# - ソース一覧は各 .vcxproj の ClCompile を直接読む（cmake/VcxprojSources.cmake）。
# - MainFrameWindow.cpp（ウィンドウ表示専用）は MainFrameWindowNone.cpp に差し替える。
# - _DEBUG は付けない。CMake の Debug 構成も _DEBUG を自動定義しないので、
#   デバッグ API（/api/debug/fixture）はどの構成でも入らない。
# - StdAfx.h はプロジェクトごとに別物なので、各ターゲットで
#   「そのプロジェクトのディレクトリ」を include の先頭に置く。

cmake_minimum_required(VERSION 3.16)
project(SBOP2 C CXX)

enable_testing()

if(WIN32)
	message(FATAL_ERROR "Windows では SBO.sln（Visual Studio）でビルドしてください。")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "ビルド構成" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/VcxprojSources.cmake)

find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)

set(SBO_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# 旧コード由来の大量の警告でログが埋まらないよう、ここでは既定の警告レベルのままにする
set(SBO_COMMON_DEFINES NDEBUG)

# Common 配下の include ディレクトリ（各 .vcxproj の AdditionalIncludeDirectories 相当）
set(SBO_COMMON_INCLUDES
	${SBO_ROOT}/Common
	${SBO_ROOT}/Common/Lib
	${SBO_ROOT}/Common/Lib/mfc
	${SBO_ROOT}/Common/Lib/md5
	${SBO_ROOT}/Common/Info
	${SBO_ROOT}/Common/Info/InfoMapEvent
	${SBO_ROOT}/Common/Info/InfoItemType
	${SBO_ROOT}/Common/Info/InfoSkill
	${SBO_ROOT}/Common/Info/InfoTalkEvent
	${SBO_ROOT}/Common/myLib
	${SBO_ROOT}/Common/myLib/myZlib
	${SBO_ROOT}/Common/LibInfo
	${SBO_ROOT}/Common/Packet
	${SBO_ROOT}/Common/Packet/ACCOUNT
	${SBO_ROOT}/Common/Packet/ADMIN
	${SBO_ROOT}/Common/Packet/BATTLE
	${SBO_ROOT}/Common/Packet/CHAR
	${SBO_ROOT}/Common/Packet/CONNECT
	${SBO_ROOT}/Common/Packet/EFFECT
	${SBO_ROOT}/Common/Packet/ITEM
	${SBO_ROOT}/Common/Packet/MAP
	${SBO_ROOT}/Common/Packet/MSGCMD
	${SBO_ROOT}/Common/Packet/SKILL
	${SBO_ROOT}/Common/Packet/VERSION
)

# ---------------------------------------------------------------------------
# myLib
# ---------------------------------------------------------------------------
sbo_read_vcxproj_sources(MYLIB_SOURCES ${SBO_ROOT}/myLib/myLib.vcxproj)
# CmyThread は非Windows で SDL2 を使う実装で、SboSvr からは使われない。
# SDL2 を入れずに済ませるため Linux ビルドでは外す。
list(REMOVE_ITEM MYLIB_SOURCES ${SBO_ROOT}/Common/myLib/myThread.cpp)
add_library(myLib STATIC ${MYLIB_SOURCES})
target_include_directories(myLib BEFORE PRIVATE ${SBO_ROOT}/myLib)
target_include_directories(myLib PRIVATE ${SBO_COMMON_INCLUDES})
target_compile_definitions(myLib PRIVATE ${SBO_COMMON_DEFINES})

# ---------------------------------------------------------------------------
# SboSockLib
# ---------------------------------------------------------------------------
sbo_read_vcxproj_sources(SBOSOCKLIB_SOURCES ${SBO_ROOT}/SboSockLib/SboSockLib.vcxproj)
add_library(SboSockLib STATIC ${SBOSOCKLIB_SOURCES})
target_include_directories(SboSockLib BEFORE PRIVATE ${SBO_ROOT}/SboSockLib)
target_include_directories(SboSockLib PRIVATE ${SBO_COMMON_INCLUDES})
target_compile_definitions(SboSockLib PRIVATE ${SBO_COMMON_DEFINES})

# ---------------------------------------------------------------------------
# SboSvr
# ---------------------------------------------------------------------------
sbo_read_vcxproj_sources(SBOSVR_SOURCES ${SBO_ROOT}/SboSvr/SboSvr.vcxproj)

# ウィンドウ表示版を外し、非Windows 版（ヘッドレス専用）へ差し替える
list(REMOVE_ITEM SBOSVR_SOURCES ${SBO_ROOT}/SboSvr/src/MainFrame/MainFrameWindow.cpp)
list(APPEND SBOSVR_SOURCES ${SBO_ROOT}/SboSvr/src/MainFrame/MainFrameWindowNone.cpp)

# 非Windows の TCharCompat.h が使う SJIS 変換の実体。Windows では Win32 API で
# 変換するため .vcxproj には載っていない。
list(APPEND SBOSVR_SOURCES ${SBO_ROOT}/Common/Platform/SjisConvert.cpp)

add_executable(SboSvr ${SBOSVR_SOURCES})
target_include_directories(SboSvr BEFORE PRIVATE ${SBO_ROOT}/SboSvr)
target_include_directories(SboSvr PRIVATE
	${SBO_ROOT}/SboSvr/src
	${SBO_ROOT}/SboSvr/src/Info
	${SBO_ROOT}/SboSvr/src/Info/InfoChar
	${SBO_ROOT}/SboSvr/src/Lib
	${SBO_ROOT}/SboSvr/src/LibInfo
	${SBO_ROOT}/SboSvr/src/MainFrame
	${SBO_ROOT}/SboSvr/src/SaveLoad
	${SBO_ROOT}/SboSvr/third_party/sqlite
	${SBO_ROOT}/SboSvr/third_party/lodepng
	${SBO_COMMON_INCLUDES}
)
target_compile_definitions(SboSvr PRIVATE ${SBO_COMMON_DEFINES})
target_link_libraries(SboSvr PRIVATE SboSockLib myLib ZLIB::ZLIB Threads::Threads ${CMAKE_DL_LIBS})

# ---------------------------------------------------------------------------
# SboSvrTest（テスト対象のソースを直接取り込む単体テスト exe）
# ---------------------------------------------------------------------------
sbo_read_vcxproj_sources(SBOSVRTEST_SOURCES ${SBO_ROOT}/SboSvrTest/SboSvrTest.vcxproj)
# SboSvr と同じく、非Windows の TCharCompat.h が使う SJIS 変換の実体を足す
list(APPEND SBOSVRTEST_SOURCES ${SBO_ROOT}/Common/Platform/SjisConvert.cpp)

add_executable(SboSvrTest ${SBOSVRTEST_SOURCES})
target_include_directories(SboSvrTest BEFORE PRIVATE ${SBO_ROOT}/SboSvrTest)
# .vcxproj の AdditionalIncludeDirectories 相当
target_include_directories(SboSvrTest PRIVATE
	${SBO_ROOT}/SboSvr/src
	${SBO_ROOT}/SboSvr/src/Web
	${SBO_ROOT}/SboSvr/src/Web/Handlers
	${SBO_ROOT}/Common
	${SBO_ROOT}/Common/myLib
	${SBO_ROOT}/Common/Lib
	${SBO_ROOT}/Common/Packet
	${SBO_ROOT}/Common/Packet/CONNECT
	${SBO_ROOT}/SboSockLib
)
target_compile_definitions(SboSvrTest PRIVATE ${SBO_COMMON_DEFINES})
target_link_libraries(SboSvrTest PRIVATE ZLIB::ZLIB Threads::Threads ${CMAKE_DL_LIBS})

add_test(NAME SboSvrTest COMMAND SboSvrTest WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
