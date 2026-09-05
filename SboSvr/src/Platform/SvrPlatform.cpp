/// @file SvrPlatform.cpp
/// @brief サーバーのプラットフォーム依存処理 実装ファイル
/// @copyright Copyright(C)URARA-works
///
/// このファイルだけが `#ifdef _WIN32` で分岐する。
/// StdAfx.h は include しない(非Windows で通らなくなるため)。

#include "SvrPlatform.h"

#include <chrono>
#include <cstdio>
#include <ctime>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace SboPlatform
{
	unsigned int	GetTickMs(void)
	{
		// steady_clock は単調増加が保証される。
		// 32bit へ落とすのは GetTickCount() と挙動を揃えるため
		// (差分を取る使い方は折り返しても正しく動く)。
		std::chrono::steady_clock::duration d =
				std::chrono::steady_clock::now().time_since_epoch();
		long long nMs = std::chrono::duration_cast<std::chrono::milliseconds>(d).count();

		return (unsigned int)(nMs & 0xFFFFFFFF);
	}

	void	GetLocalTime(LOCALTIME *pOut)
	{
		if (pOut == NULL) {
			return;
		}
		std::memset(pOut, 0, sizeof(*pOut));

		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);

		std::tm tmLocal;
#ifdef _WIN32
		localtime_s(&tmLocal, &t);
#else
		localtime_r(&t, &tmLocal);
#endif

		long long nMs = std::chrono::duration_cast<std::chrono::milliseconds>(
				now.time_since_epoch()).count();

		pOut->nYear   = tmLocal.tm_year + 1900;
		pOut->nMonth  = tmLocal.tm_mon + 1;
		pOut->nDay    = tmLocal.tm_mday;
		pOut->nHour   = tmLocal.tm_hour;
		pOut->nMinute = tmLocal.tm_min;
		pOut->nSecond = tmLocal.tm_sec;
		pOut->nMilli  = (int)(nMs % 1000);
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
		char szPath[1024];
		std::string strRet;

		std::memset(szPath, 0, sizeof(szPath));

#ifdef _WIN32
		if (GetModuleFileNameA(NULL, szPath, (DWORD)sizeof(szPath)) == 0) {
			return std::string("./");
		}
#elif defined(__EMSCRIPTEN__)
		// ブラウザ上では実行ファイルの概念が無い
		return std::string("./");
#else
		ssize_t nLen = readlink("/proc/self/exe", szPath, sizeof(szPath) - 1);
		if (nLen <= 0) {
			return std::string("./");
		}
		szPath[nLen] = '\0';
#endif

		strRet = szPath;

		// 末尾のファイル名を落とす
		std::string::size_type nPos = strRet.find_last_of("\\/");
		if (nPos == std::string::npos) {
			return std::string("./");
		}
		return strRet.substr(0, nPos + 1);
	}

	bool	MakeDirectory(const char *pszPath)
	{
		if ((pszPath == NULL) || (pszPath[0] == '\0')) {
			return false;
		}

#ifdef _WIN32
		if (_mkdir(pszPath) == 0) {
			return true;
		}
		// 既に存在するなら成功扱い
		{
			DWORD dwAttr = GetFileAttributesA(pszPath);
			return (dwAttr != INVALID_FILE_ATTRIBUTES) &&
				   ((dwAttr & FILE_ATTRIBUTE_DIRECTORY) != 0);
		}
#else
		if (mkdir(pszPath, 0755) == 0) {
			return true;
		}
		{
			struct stat st;
			if (stat(pszPath, &st) != 0) {
				return false;
			}
			return S_ISDIR(st.st_mode) ? true : false;
		}
#endif
	}

	std::string	MakeExeRelativePath(const char *pszRelative)
	{
		std::string strRet = GetExeDirectory();

		if (pszRelative != NULL) {
			strRet += pszRelative;
		}

		// 区切り文字をプラットフォームに合わせる
		char chSep = GetPathSeparator();
		for (std::string::size_type i = 0; i < strRet.size(); ++ i) {
			if ((strRet[i] == '/') || (strRet[i] == '\\')) {
				strRet[i] = chSep;
			}
		}
		return strRet;
	}

	std::string	MakeDataFilePath(const char *pszFileName)
	{
		std::string strDir = MakeExeRelativePath("SBODATA");

		MakeDirectory(strDir.c_str());

		strDir += GetPathSeparator();
		if (pszFileName != NULL) {
			strDir += pszFileName;
		}
		return strDir;
	}

	void	WriteDebugLine(const char *pszText)
	{
		if (pszText == NULL) {
			return;
		}

#ifdef _WIN32
		OutputDebugStringA(pszText);
		if (std::strchr(pszText, '\n') == NULL) {
			OutputDebugStringA("\n");
		}
#else
		std::fputs(pszText, stderr);
		if (std::strchr(pszText, '\n') == NULL) {
			std::fputc('\n', stderr);
		}
#endif
	}
}
