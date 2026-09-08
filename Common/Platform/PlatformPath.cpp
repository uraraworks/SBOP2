/// @file PlatformPath.cpp
/// @brief 実行ファイルの場所に関するパス解決 実装ファイル
/// @copyright Copyright(C)URARA-works

#include "PlatformPath.h"

#include <cstring>
#include <cwchar>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#elif defined(__EMSCRIPTEN__)
// ブラウザには実行ファイルの概念が無い。追加の include は不要。
#else
#include <unistd.h>
#endif

namespace SboPlatform
{
	namespace
	{
		// ディレクトリ切り出し・拡張子差し替えは char/wchar_t で共通のロジック
		// なので、ここへ1本にまとめて実行ファイルパス取得側(プラットフォーム
		// 分岐がある部分)とは分離する。

		/// 末尾のファイル名部分を切り落として、ディレクトリ部分(区切り文字込み)を返す
		template <typename Ch>
		std::basic_string<Ch>	ExtractDirectory(
			const std::basic_string<Ch> &strFullPath,
			const Ch *pszSeps,
			const Ch *pszFallback)
		{
			typename std::basic_string<Ch>::size_type nPos = strFullPath.find_last_of(pszSeps);
			if (nPos == std::basic_string<Ch>::npos) {
				return std::basic_string<Ch>(pszFallback);
			}
			return strFullPath.substr(0, nPos + 1);
		}

		/// 拡張子を ini に差し替える
		template <typename Ch>
		std::basic_string<Ch>	ReplaceExtWithIni(
			const std::basic_string<Ch> &strFullPath,
			const Ch *pszSeps,
			Ch chDot,
			const Ch *pszIniExt)
		{
			std::basic_string<Ch> strRet = strFullPath;

			typename std::basic_string<Ch>::size_type nDot = strRet.find_last_of(chDot);
			typename std::basic_string<Ch>::size_type nSep = strRet.find_last_of(pszSeps);
			if ((nDot != std::basic_string<Ch>::npos) &&
				((nSep == std::basic_string<Ch>::npos) || (nDot > nSep))) {
				strRet = strRet.substr(0, nDot);
			}
			strRet += pszIniExt;
			return strRet;
		}

		// 実行ファイルの絶対パス取得だけがプラットフォーム分岐の対象。

		std::string	GetExeFilePathNarrow(void)
		{
			char szPath[1024];
			std::memset(szPath, 0, sizeof(szPath));

#ifdef _WIN32
			// ナロー版は現行 SvrPlatform.cpp と同じく A版を使う。
			// エンコーディング挙動(CP932前提)を変えないため。
			if (GetModuleFileNameA(NULL, szPath, (DWORD)sizeof(szPath)) == 0) {
				return std::string();
			}
#elif defined(__EMSCRIPTEN__)
			// ブラウザには実行ファイルの概念が無い。
			// SboCli/src/Platform/Win32ApiStubs.h の GetModuleFileName スタブが
			// 返していたダミー値("./sbocli.html")と同じにして、既存の ini
			// 保存先(ブラウザ版の設定ファイル)を変えないようにする。
			return std::string("./sbocli.html");
#else
			ssize_t nLen = readlink("/proc/self/exe", szPath, sizeof(szPath) - 1);
			if (nLen <= 0) {
				return std::string();
			}
			szPath[nLen] = '\0';
#endif
			return std::string(szPath);
		}

		std::wstring	GetExeFilePathWide(void)
		{
#ifdef _WIN32
			wchar_t szPath[1024];
			std::memset(szPath, 0, sizeof(szPath));
			if (GetModuleFileNameW(NULL, szPath, (DWORD)(sizeof(szPath) / sizeof(wchar_t))) == 0) {
				return std::wstring();
			}
			return std::wstring(szPath);
#else
			// ナロー版からの変換はここ1箇所に閉じる。
			std::string strNarrow = GetExeFilePathNarrow();
			if (strNarrow.empty()) {
				return std::wstring();
			}
			wchar_t szWide[1024];
			std::memset(szWide, 0, sizeof(szWide));
			std::mbstowcs(szWide, strNarrow.c_str(), (sizeof(szWide) / sizeof(wchar_t)) - 1);
			return std::wstring(szWide);
#endif
		}
	}

	char	GetPathSeparator(void)
	{
#ifdef _WIN32
		return '\\';
#else
		return '/';
#endif
	}

	std::string	GetExeDirectory(void)
	{
		std::string strPath = GetExeFilePathNarrow();
		if (strPath.empty()) {
			return std::string("./");
		}
		return ExtractDirectory<char>(strPath, "\\/", "./");
	}

	std::wstring	GetExeDirectoryW(void)
	{
		std::wstring strPath = GetExeFilePathWide();
		if (strPath.empty()) {
			return std::wstring(L"./");
		}
		return ExtractDirectory<wchar_t>(strPath, L"\\/", L"./");
	}

	std::string	GetIniFilePath(void)
	{
		std::string strPath = GetExeFilePathNarrow();
		if (strPath.empty()) {
			return std::string();
		}
		return ReplaceExtWithIni<char>(strPath, "\\/", '.', ".ini");
	}

	std::wstring	GetIniFilePathW(void)
	{
		std::wstring strPath = GetExeFilePathWide();
		if (strPath.empty()) {
			return std::wstring();
		}
		return ReplaceExtWithIni<wchar_t>(strPath, L"\\/", L'.', L".ini");
	}
}
