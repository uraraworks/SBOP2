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
#include <cstdlib>
#include <vector>

#ifdef _WIN32
// SocketStartup/SocketCleanup が WSAStartup/WSACleanup を使うため、
// windows.h より先に winsock2.h を読ませて winsock.h との衝突を避ける。
#include <winsock2.h>
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

	void	GetSystemTime(LOCALTIME *pOut)
	{
		if (pOut == NULL) {
			return;
		}
		std::memset(pOut, 0, sizeof(*pOut));

		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);

		std::tm tmUtc;
#ifdef _WIN32
		gmtime_s(&tmUtc, &t);
#else
		gmtime_r(&t, &tmUtc);
#endif

		long long nMs = std::chrono::duration_cast<std::chrono::milliseconds>(
				now.time_since_epoch()).count();

		pOut->nYear   = tmUtc.tm_year + 1900;
		pOut->nMonth  = tmUtc.tm_mon + 1;
		pOut->nDay    = tmUtc.tm_mday;
		pOut->nHour   = tmUtc.tm_hour;
		pOut->nMinute = tmUtc.tm_min;
		pOut->nSecond = tmUtc.tm_sec;
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

	std::string	GetIniFilePath(void)
	{
		char szPath[1024];
		std::string strRet;

		std::memset(szPath, 0, sizeof(szPath));

#ifdef _WIN32
		if (GetModuleFileNameA(NULL, szPath, (DWORD)sizeof(szPath)) == 0) {
			return std::string();
		}
#elif defined(__EMSCRIPTEN__)
		return std::string();
#else
		{
			ssize_t nLen = readlink("/proc/self/exe", szPath, sizeof(szPath) - 1);
			if (nLen <= 0) {
				return std::string();
			}
			szPath[nLen] = '\0';
		}
#endif

		strRet = szPath;

		// 拡張子を ini に差し替える
		std::string::size_type nDot = strRet.find_last_of('.');
		std::string::size_type nSep = strRet.find_last_of("\\/");
		if ((nDot != std::string::npos) &&
			((nSep == std::string::npos) || (nDot > nSep))) {
			strRet = strRet.substr(0, nDot);
		}
		strRet += ".ini";

		return strRet;
	}

	namespace
	{
		// 大文字小文字を区別せずに比較する(Windows の ini API と同じ扱い)
		bool	EqualsNoCase(const std::string &a, const std::string &b)
		{
			if (a.size() != b.size()) {
				return false;
			}
			for (std::string::size_type i = 0; i < a.size(); ++ i) {
				char ca = a[i];
				char cb = b[i];
				if ((ca >= 'A') && (ca <= 'Z')) { ca = (char)(ca - 'A' + 'a'); }
				if ((cb >= 'A') && (cb <= 'Z')) { cb = (char)(cb - 'A' + 'a'); }
				if (ca != cb) {
					return false;
				}
			}
			return true;
		}

		std::string	Trim(const std::string &s)
		{
			std::string::size_type nStart = 0;
			std::string::size_type nEnd = s.size();

			while ((nStart < nEnd) &&
				   ((s[nStart] == ' ') || (s[nStart] == '\t') ||
					(s[nStart] == '\r') || (s[nStart] == '\n'))) {
				nStart ++;
			}
			while ((nEnd > nStart) &&
				   ((s[nEnd - 1] == ' ') || (s[nEnd - 1] == '\t') ||
					(s[nEnd - 1] == '\r') || (s[nEnd - 1] == '\n'))) {
				nEnd --;
			}
			return s.substr(nStart, nEnd - nStart);
		}

		// "[名前]" ならセクション名を返す。違えば空文字列。
		std::string	ParseSection(const std::string &strLine)
		{
			std::string s = Trim(strLine);

			if ((s.size() < 2) || (s[0] != '[') || (s[s.size() - 1] != ']')) {
				return std::string();
			}
			return Trim(s.substr(1, s.size() - 2));
		}

		bool	ReadLines(const char *pszFile, std::vector<std::string> *pvecOut)
		{
			std::FILE *pFile = std::fopen(pszFile, "rb");
			std::string strAll;
			char szBuf[4096];
			size_t nRead;

			if (pFile == NULL) {
				return false;
			}
			while ((nRead = std::fread(szBuf, 1, sizeof(szBuf), pFile)) > 0) {
				strAll.append(szBuf, nRead);
			}
			std::fclose(pFile);

			// 改行で分割する(CRLF/LF どちらでも。区切りは保持しない)
			std::string strLine;
			for (std::string::size_type i = 0; i < strAll.size(); ++ i) {
				if (strAll[i] == '\n') {
					pvecOut->push_back(strLine);
					strLine.clear();
				} else if (strAll[i] != '\r') {
					strLine += strAll[i];
				}
			}
			if (!strLine.empty()) {
				pvecOut->push_back(strLine);
			}
			return true;
		}
	}

	std::string	GetIniString(const char *pszFile, const char *pszSection,
			const char *pszKey, const char *pszDefault)
	{
		std::vector<std::string> vecLine;
		std::string strDefault = (pszDefault != NULL) ? pszDefault : "";
		std::string strCurSection;
		bool bInTarget = false;

		if ((pszFile == NULL) || (pszSection == NULL) || (pszKey == NULL)) {
			return strDefault;
		}
		if (!ReadLines(pszFile, &vecLine)) {
			return strDefault;
		}

		for (size_t i = 0; i < vecLine.size(); ++ i) {
			std::string strSection = ParseSection(vecLine[i]);
			if (!strSection.empty()) {
				strCurSection = strSection;
				bInTarget = EqualsNoCase(strCurSection, pszSection);
				continue;
			}
			if (!bInTarget) {
				continue;
			}

			std::string strLine = Trim(vecLine[i]);
			if (strLine.empty() || (strLine[0] == ';') || (strLine[0] == '#')) {
				continue;
			}
			std::string::size_type nPos = strLine.find('=');
			if (nPos == std::string::npos) {
				continue;
			}
			if (EqualsNoCase(Trim(strLine.substr(0, nPos)), pszKey)) {
				return Trim(strLine.substr(nPos + 1));
			}
		}
		return strDefault;
	}

	int	GetIniInt(const char *pszFile, const char *pszSection, const char *pszKey, int nDefault)
	{
		std::string strValue = GetIniString(pszFile, pszSection, pszKey, "");

		if (strValue.empty()) {
			return nDefault;
		}

		// Windows の GetPrivateProfileInt は先頭の数字だけを読む
		char *pszEnd = NULL;
		long nValue = std::strtol(strValue.c_str(), &pszEnd, 10);
		if (pszEnd == strValue.c_str()) {
			return nDefault;
		}
		return (int)nValue;
	}

	bool	SetIniString(const char *pszFile, const char *pszSection,
			const char *pszKey, const char *pszValue)
	{
		std::vector<std::string> vecLine;
		std::string strCurSection;
		std::string strEntry;
		bool bWritten = false;
		int nTargetSectionEnd = -1;

		if ((pszFile == NULL) || (pszSection == NULL) || (pszKey == NULL)) {
			return false;
		}

		strEntry = std::string(pszKey) + "=" + ((pszValue != NULL) ? pszValue : "");

		// 既存の内容を読む(無ければ新規作成扱い)
		ReadLines(pszFile, &vecLine);

		for (size_t i = 0; i < vecLine.size(); ++ i) {
			std::string strSection = ParseSection(vecLine[i]);
			if (!strSection.empty()) {
				if (EqualsNoCase(strCurSection, pszSection)) {
					// 対象セクションが終わった位置を覚えておく
					nTargetSectionEnd = (int)i;
				}
				strCurSection = strSection;
				continue;
			}
			if (!EqualsNoCase(strCurSection, pszSection)) {
				continue;
			}

			std::string strLine = Trim(vecLine[i]);
			if (strLine.empty() || (strLine[0] == ';') || (strLine[0] == '#')) {
				continue;
			}
			std::string::size_type nPos = strLine.find('=');
			if (nPos == std::string::npos) {
				continue;
			}
			if (EqualsNoCase(Trim(strLine.substr(0, nPos)), pszKey)) {
				vecLine[i] = strEntry;
				bWritten = true;
				break;
			}
		}

		if (!bWritten) {
			if (EqualsNoCase(strCurSection, pszSection)) {
				// 対象セクションがファイル末尾まで続いていた
				vecLine.push_back(strEntry);
			} else if (nTargetSectionEnd >= 0) {
				// 対象セクションの末尾へ挿入する
				vecLine.insert(vecLine.begin() + nTargetSectionEnd, strEntry);
			} else {
				// セクションごと追加する
				vecLine.push_back(std::string("[") + pszSection + "]");
				vecLine.push_back(strEntry);
			}
		}

		std::FILE *pFile = std::fopen(pszFile, "wb");
		if (pFile == NULL) {
			return false;
		}
		for (size_t i = 0; i < vecLine.size(); ++ i) {
			// 従来の ini と同じ CRLF で書く
			std::fwrite(vecLine[i].c_str(), 1, vecLine[i].size(), pFile);
			std::fwrite("\r\n", 1, 2, pFile);
		}
		std::fclose(pFile);
		return true;
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

	bool	GmTimeUtc(std::time_t t, GMTIME *pOut)
	{
		if (pOut == NULL) {
			return false;
		}
		std::memset(pOut, 0, sizeof(*pOut));

		std::tm tmUtc;
		std::memset(&tmUtc, 0, sizeof(tmUtc));
#ifdef _WIN32
		if (gmtime_s(&tmUtc, &t) != 0) {
			return false;
		}
#else
		if (gmtime_r(&t, &tmUtc) == NULL) {
			return false;
		}
#endif
		pOut->nYear    = tmUtc.tm_year + 1900;
		pOut->nMonth   = tmUtc.tm_mon + 1;
		pOut->nDay     = tmUtc.tm_mday;
		pOut->nHour    = tmUtc.tm_hour;
		pOut->nMinute  = tmUtc.tm_min;
		pOut->nSecond  = tmUtc.tm_sec;
		pOut->nWeekDay = tmUtc.tm_wday;
		return true;
	}

	bool	TimeGmUtc(const GMTIME &in, std::time_t *pOut)
	{
		if (pOut == NULL) {
			return false;
		}

		std::tm tmUtc;
		std::memset(&tmUtc, 0, sizeof(tmUtc));
		tmUtc.tm_year  = in.nYear - 1900;
		tmUtc.tm_mon   = in.nMonth - 1;
		tmUtc.tm_mday  = in.nDay;
		tmUtc.tm_hour  = in.nHour;
		tmUtc.tm_min   = in.nMinute;
		tmUtc.tm_sec   = in.nSecond;
		tmUtc.tm_isdst = 0;

#ifdef _WIN32
		std::time_t t = _mkgmtime(&tmUtc);
#else
		std::time_t t = timegm(&tmUtc);
#endif
		if (t == (std::time_t)-1) {
			return false;
		}
		*pOut = t;
		return true;
	}

	bool	SocketStartup(void)
	{
#ifdef _WIN32
		// WSAStartup は呼び出し回数を内部で参照カウントする方式なので、
		// Web層の複数箇所(WebSocketBridge / HttpServer 等)から重ねて呼んでも
		// 安全(対応する WSACleanup も同数呼べばよい)。
		WSADATA wsaData;
		std::memset(&wsaData, 0, sizeof(wsaData));
		return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
		// 非Windowsではソケットライブラリの初期化は不要
		return true;
#endif
	}

	void	SocketCleanup(void)
	{
#ifdef _WIN32
		WSACleanup();
#endif
	}
}
