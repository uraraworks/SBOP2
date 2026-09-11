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
#include <mutex>
#include <vector>

#ifdef _WIN32
// SocketStartup/SocketCleanup が WSAStartup/WSACleanup を使うため、
// windows.h より先に winsock2.h を読ませて winsock.h との衝突を避ける。
#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#include <bcrypt.h>
#include <mmsystem.h>
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "winmm.lib")
#else
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <fcntl.h>
#include <sys/file.h>
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

	// GetPathSeparator() / GetExeDirectory() は Common/Platform/PlatformPath.cpp
	// (Common/SBOGlobal.cpp の GetModuleFilePath() と実装が重複していたため
	// そちらへ集約した)へ移動した。

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

	// GetIniFilePath() も Common/Platform/PlatformPath.cpp へ移動した。

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

	bool	GenerateRandomBytes(void *pBuffer, size_t nLength)
	{
		if (pBuffer == NULL) {
			return false;
		}
		if (nLength == 0) {
			return true;
		}

#ifdef _WIN32
		// PasswordHash.cpp の salt 生成で既に使われていたのと同じ CNG API。
		NTSTATUS status = BCryptGenRandom(
				NULL, (PUCHAR)pBuffer, (ULONG)nLength,
				BCRYPT_USE_SYSTEM_PREFERRED_RNG);
		return BCRYPT_SUCCESS(status) ? true : false;
#else
		// /dev/urandom を読む。read() は要求量より短く返ることがあるため
		// 埋まるまでループする。
		unsigned char *p = (unsigned char *)pBuffer;
		size_t nRemain = nLength;

		FILE *pFile = std::fopen("/dev/urandom", "rb");
		if (pFile == NULL) {
			return false;
		}

		while (nRemain > 0) {
			size_t nRead = std::fread(p, 1, nRemain, pFile);
			if (nRead == 0) {
				// 読めなくなった(エラーまたは予期しない EOF)。安全のため失敗扱いにする。
				std::fclose(pFile);
				return false;
			}
			p += nRead;
			nRemain -= nRead;
		}

		std::fclose(pFile);
		return true;
#endif
	}

#ifdef _WIN32
	namespace
	{
		// SboGrpData.dll のハンドルをプロセス内で使い回す。
		// HttpServer は複数のクライアントスレッドから同時に呼ぶため、
		// このミューテックスで保護する(旧実装はハンドラのインスタンスごとに
		// HMODULE を持っていたが、ここへ集約したのでプロセス内で1つになる)。
		//
		// SpriteSheetHandler / MapPartsHandler 双方の探索パス候補を突き合わせた
		// 結果、どちらも「実行ファイルの隣の SboGrpData.dll」があればそれを、
		// 無ければ LoadLibraryW 自身の既定探索(DLL 検索パス)に任せて
		// "SboGrpData.dll" とだけ渡す、という同一のロジックだった
		// (片方にしか無い候補パスは見つからなかった)。
		std::mutex	g_grpDllMutex;
		HMODULE		g_hGrpDll = NULL;

		// 実行ファイルの隣にある SboGrpData.dll のパスを組み立てる。
		// 見つからなければ LoadLibraryW の既定探索に任せるため素の名前を返す。
		std::wstring	ResolveGrpDllPath()
		{
			wchar_t szModulePath[MAX_PATH];
			DWORD dwLength = GetModuleFileNameW(NULL, szModulePath, MAX_PATH);
			if ((dwLength == 0) || (dwLength >= MAX_PATH)) {
				return L"SboGrpData.dll";
			}

			wchar_t *pSlash = wcsrchr(szModulePath, L'\\');
			if (pSlash != NULL) {
				*(pSlash + 1) = L'\0';
			}

			std::wstring strCandidate = szModulePath;
			strCandidate.append(L"SboGrpData.dll");

			DWORD dwAttributes = GetFileAttributesW(strCandidate.c_str());
			if (dwAttributes == INVALID_FILE_ATTRIBUTES) {
				return L"SboGrpData.dll";
			}
			return strCandidate;
		}

		// g_grpDllMutex を保持した状態で呼ぶこと。
		// 一度ロードしたら保持したままにする(プロセス終了時に OS が解放する。
		// 旧実装はハンドラのデストラクタで FreeLibrary していたが、
		// いずれもプロセス終了間際のシングルトン破棄でしか呼ばれていなかった
		// ので挙動差は無い)。
		bool	EnsureGrpDllLocked()
		{
			if (g_hGrpDll != NULL) {
				return true;
			}
			std::wstring strPath = ResolveGrpDllPath();
			HMODULE hLoaded = LoadLibraryW(strPath.c_str());
			if (hLoaded == NULL) {
				return false;
			}
			g_hGrpDll = hLoaded;
			return true;
		}

		// char* (ASCII 前提) を wstring に変換する。
		// FindResourceW が wchar_t を要求するためここで変換する
		// (呼び出し側のリソース名は char テーブル由来、または printf パターン
		// から組み立てたもので、いずれも ASCII の範囲に収まる)。
		std::wstring	ToWideAscii(const char *pszSrc)
		{
			std::wstring strOut;
			if (pszSrc == NULL) {
				return strOut;
			}
			for (const char *p = pszSrc; *p != '\0'; ++ p) {
				strOut.push_back((wchar_t)(unsigned char)*p);
			}
			return strOut;
		}
	}
#endif

	bool	LoadEmbeddedPng(const char *pszResourceName, std::vector<unsigned char> &outPng)
	{
		if ((pszResourceName == NULL) || (pszResourceName[0] == '\0')) {
			return false;
		}

#ifdef _WIN32
		std::lock_guard<std::mutex> lock(g_grpDllMutex);
		if (!EnsureGrpDllLocked()) {
			return false;
		}

		std::wstring strName = ToWideAscii(pszResourceName);
		HRSRC hResInfo = FindResourceW(g_hGrpDll, strName.c_str(), L"PNG");
		if (hResInfo == NULL) {
			return false;
		}
		HGLOBAL hRes = LoadResource(g_hGrpDll, hResInfo);
		if (hRes == NULL) {
			return false;
		}
		DWORD dwResourceSize = SizeofResource(g_hGrpDll, hResInfo);
		if (dwResourceSize == 0) {
			return false;
		}
		const BYTE *pResourceData = static_cast<const BYTE *>(LockResource(hRes));
		if (pResourceData == NULL) {
			return false;
		}

		outPng.assign(pResourceData, pResourceData + dwResourceSize);
		return true;
#else
		// PE のリソースという概念自体が非Windowsには無い。
		// 呼び出し側は画像ストア(DB) / ファイル(res/) 等、前段のフォールバックで賄う。
		return false;
#endif
	}

	// ここから下はヘッドレスサーバーの多重起動防止と停止要求のやり取り。
	// サーバーは1プロセスにつき1インスタンスしか動かさない前提なので、
	// 状態はここ(無名 namespace)に file-static で持つ。

	namespace
	{
#ifdef _WIN32
		// 稼働中を示すミューテックスと停止通知イベント。
		// 旧 CMainFrame::m_hRunMutex / m_hQuitEvent をそのまま移した。
		HANDLE	g_hRunMutex = NULL;
		HANDLE	g_hQuitEvent = NULL;

		// InstallStopSignalHandler() に渡された関数。
		// ConsoleCtrlHandlerThunk は別スレッドから呼ばれるため、
		// ここに置いた関数ポインタ経由で呼び出す。
		void	(*g_pfnRequestQuit)(void) = NULL;
		bool	(*g_pfnIsQuitting)(void) = NULL;

		// 停止通知イベント名を作る(ポートで区別する)
		void	MakeQuitEventName(char *pszName, size_t nMax, unsigned short wPort)
		{
			_snprintf_s(pszName, nMax, _TRUNCATE, "SboSvr_Quit_%u", (unsigned int)wPort);
		}

		// 稼働中ミューテックス名を作る
		void	MakeRunMutexName(char *pszName, size_t nMax, unsigned short wPort)
		{
			_snprintf_s(pszName, nMax, _TRUNCATE, "SboSvr_Running_%u", (unsigned int)wPort);
		}

		// コンソール終了シグナルの受け口(旧 CMainFrame::ConsoleCtrlHandler)
		//
		// 別スレッドで呼ばれるため、終了要求を立てるだけにする。停止処理
		// (DB の書き戻しを含む)はメインスレッドが行うので、ここでは
		// g_pfnIsQuitting() が完了を返すまで待つ。ここで返すと OS に
		// プロセスを落とされ、DB の書き戻しが行われない。
		BOOL WINAPI	ConsoleCtrlHandlerThunk(DWORD dwCtrlType)
		{
			switch (dwCtrlType) {
			case CTRL_C_EVENT:
			case CTRL_BREAK_EVENT:
			case CTRL_CLOSE_EVENT:
			case CTRL_LOGOFF_EVENT:
			case CTRL_SHUTDOWN_EVENT:
				if (g_pfnRequestQuit != NULL) {
					g_pfnRequestQuit();
				}
				while ((g_pfnIsQuitting != NULL) && !g_pfnIsQuitting()) {
					Sleep(50);
				}
				return TRUE;
			}
			return FALSE;
		}

		// マルチメディアタイマーの精度(BeginHighResolutionTimer で取得し、
		// EndHighResolutionTimer で対にして返す)。
		UINT	g_uTimerPeriodMin = 0;
#else
		// 単一インスタンス保証と停止要求の待ち合わせは、pid ファイルへの
		// flock(2) で表現する。ロックが取れる = 誰も稼働していない、
		// という対応関係が Windows のミューテックスと同じになる。
		int	g_nPidFileFd = -1;

		// SIGTERM(停止要求) を受けたかどうかのフラグ。
		// シグナルハンドラ内で触れるのはこの型の変数への代入だけ、という
		// 制約があるため sig_atomic_t にする。
		volatile sig_atomic_t	g_bStopRequested = 0;

		// InstallStopSignalHandler() に渡された関数(SIGINT で呼ぶ)
		void	(*g_pfnRequestQuit)(void) = NULL;

		// 旧 SIGTERM/SIGINT ハンドラの復元用
		struct sigaction	g_oldSigTerm;
		struct sigaction	g_oldSigInt;
		bool	g_bSigTermInstalled = false;
		bool	g_bSigIntInstalled = false;

		void	StopRequestSignalHandler(int)
		{
			g_bStopRequested = 1;
		}

		void	QuitSignalHandler(int)
		{
			// POSIX はハンドラから戻ってきても OS にプロセスを落とされない
			// ため、Windows 版のように完了を待つ必要が無い。
			// (待つ処理は呼び出し側の IsStopRequested() 相当のポーリングに任せる)
			if (g_pfnRequestQuit != NULL) {
				g_pfnRequestQuit();
			}
		}

		// pid ファイルのパスを組み立てる(SBODATA/sbosvr-<port>.pid)
		std::string	MakePidFilePath(unsigned short wPort)
		{
			char szFileName[64];
			std::snprintf(szFileName, sizeof(szFileName), "sbosvr-%u.pid", (unsigned int)wPort);
			return MakeDataFilePath(szFileName);
		}
#endif
	}

	// サーバーの多重起動を防ぐロックを取得する

	bool	AcquireServerInstanceLock(unsigned short wPort)
	{
#ifdef _WIN32
		char szName[64];

		// 稼働中を示すミューテックスを確保する。--stop 側はこれが
		// 取れるかどうかで終了を判定する(WaitForServerExit)。
		MakeRunMutexName(szName, sizeof(szName), wPort);
		g_hRunMutex = CreateMutexA(NULL, TRUE, szName);
		if (g_hRunMutex == NULL) {
			return false;
		}
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			// 同じポートのサーバーが既に動いている
			CloseHandle(g_hRunMutex);
			g_hRunMutex = NULL;
			return false;
		}
		return true;
#else
		std::string strPath = MakePidFilePath(wPort);

		int fd = open(strPath.c_str(), O_CREAT | O_RDWR, 0644);
		if (fd < 0) {
			return false;
		}
		if (flock(fd, LOCK_EX | LOCK_NB) != 0) {
			// 取れなかった = 同じポートのサーバーが既に動いている
			close(fd);
			return false;
		}

		// 自分の pid を書いておく(SendStopRequest がここから読む)。
		// ftruncate してから書くことで、前回の残骸より短い内容でも
		// 古い末尾が残らないようにする。
		char szPid[32];
		int nLen = std::snprintf(szPid, sizeof(szPid), "%ld", (long)getpid());
		if (ftruncate(fd, 0) == 0) {
			lseek(fd, 0, SEEK_SET);
			(void)write(fd, szPid, (size_t)nLen);
		}

		g_nPidFileFd = fd;
		return true;
#endif
	}

	// AcquireServerInstanceLock() で取得したロックを解放する

	void	ReleaseServerInstanceLock(void)
	{
#ifdef _WIN32
		if (g_hRunMutex != NULL) {
			ReleaseMutex(g_hRunMutex);
			CloseHandle(g_hRunMutex);
			g_hRunMutex = NULL;
		}
#else
		if (g_nPidFileFd >= 0) {
			// close で flock は自動的に外れる。pid ファイル自体は
			// unlink しない(他プロセスが同時に AcquireServerInstanceLock
			// を試みている最中に消すと、そちらが新規作成したファイルではなく
			// 自分がロックを取っていた実体を消してしまう競合があり得るため)。
			// 残骸が残っても次回の判定は flock の可否だけで行うので無害。
			close(g_nPidFileFd);
			g_nPidFileFd = -1;
		}
#endif
	}

	// 停止要求の受け口を用意する(サーバー本体側で呼ぶ)

	bool	OpenStopRequestChannel(unsigned short wPort)
	{
#ifdef _WIN32
		char szName[64];

		MakeQuitEventName(szName, sizeof(szName), wPort);
		g_hQuitEvent = CreateEventA(NULL, TRUE, FALSE, szName);
		return (g_hQuitEvent != NULL);
#else
		(void)wPort;
		g_bStopRequested = 0;

		struct sigaction sa;
		std::memset(&sa, 0, sizeof(sa));
		sa.sa_handler = StopRequestSignalHandler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		if (sigaction(SIGTERM, &sa, &g_oldSigTerm) != 0) {
			return false;
		}
		g_bSigTermInstalled = true;
		return true;
#endif
	}

	// OpenStopRequestChannel() で用意した受け口を閉じる

	void	CloseStopRequestChannel(void)
	{
#ifdef _WIN32
		if (g_hQuitEvent != NULL) {
			CloseHandle(g_hQuitEvent);
			g_hQuitEvent = NULL;
		}
#else
		if (g_bSigTermInstalled) {
			sigaction(SIGTERM, &g_oldSigTerm, NULL);
			g_bSigTermInstalled = false;
		}
#endif
	}

	// 停止要求が来ているかを非ブロックで確認する

	bool	IsStopRequested(void)
	{
#ifdef _WIN32
		if (g_hQuitEvent == NULL) {
			return false;
		}
		return (WaitForSingleObject(g_hQuitEvent, 0) == WAIT_OBJECT_0);
#else
		return (g_bStopRequested != 0);
#endif
	}

	// 稼働中のサーバーへ停止を要求する(--stop 側で呼ぶ)

	bool	SendStopRequest(unsigned short wPort)
	{
#ifdef _WIN32
		char szName[64];

		MakeQuitEventName(szName, sizeof(szName), wPort);
		HANDLE hEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, szName);
		if (hEvent == NULL) {
			// 稼働中のヘッドレスサーバーが見つからない
			return false;
		}
		SetEvent(hEvent);
		CloseHandle(hEvent);
		return true;
#else
		std::string strPath = MakePidFilePath(wPort);

		int fd = open(strPath.c_str(), O_RDWR);
		if (fd < 0) {
			// pid ファイルが無い = 稼働中のサーバーが見つからない
			return false;
		}
		if (flock(fd, LOCK_EX | LOCK_NB) == 0) {
			// ロックが取れてしまった = 誰も稼働していない
			flock(fd, LOCK_UN);
			close(fd);
			return false;
		}

		// 稼働中。ファイルから pid を読んで SIGTERM を送る。
		char szPid[32];
		std::memset(szPid, 0, sizeof(szPid));
		ssize_t nRead = read(fd, szPid, sizeof(szPid) - 1);
		close(fd);
		if (nRead <= 0) {
			return false;
		}

		pid_t nPid = (pid_t)std::atol(szPid);
		if (nPid <= 0) {
			return false;
		}
		kill(nPid, SIGTERM);
		return true;
#endif
	}

	// SendStopRequest() で要求した相手が終了するのを待つ(--stop 側で呼ぶ)

	bool	WaitForServerExit(unsigned short wPort, unsigned int uTimeoutMs)
	{
#ifdef _WIN32
		char szName[64];

		// サーバーが手放すまで待つ。取得できた時点で相手は終了している。
		MakeRunMutexName(szName, sizeof(szName), wPort);
		HANDLE hMutex = OpenMutexA(SYNCHRONIZE, FALSE, szName);
		if (hMutex == NULL) {
			// 既に終了している
			return true;
		}

		DWORD dwWait = WaitForSingleObject(hMutex, uTimeoutMs);
		bool bExited = ((dwWait == WAIT_OBJECT_0) || (dwWait == WAIT_ABANDONED));
		if (bExited) {
			ReleaseMutex(hMutex);
		}
		CloseHandle(hMutex);
		return bExited;
#else
		std::string strPath = MakePidFilePath(wPort);
		unsigned int uElapsedMs = 0;

		// ミューテックス待ちと同じ意味: 相手が flock を手放した(= 終了した)
		// 時点でこちらがロックを取得できるようになる。取れたら即座に
		// 手放し、成功として返す。
		for (;;) {
			int fd = open(strPath.c_str(), O_RDWR);
			if (fd < 0) {
				// pid ファイルが無い = 既に終了している
				return true;
			}
			if (flock(fd, LOCK_EX | LOCK_NB) == 0) {
				flock(fd, LOCK_UN);
				close(fd);
				return true;
			}
			close(fd);

			if (uElapsedMs >= uTimeoutMs) {
				return false;
			}
			unsigned int uSleepMs = 50;
			if (uElapsedMs + uSleepMs > uTimeoutMs) {
				uSleepMs = uTimeoutMs - uElapsedMs;
			}
			struct timespec ts;
			ts.tv_sec = uSleepMs / 1000;
			ts.tv_nsec = (long)(uSleepMs % 1000) * 1000000L;
			nanosleep(&ts, NULL);
			uElapsedMs += uSleepMs;
		}
#endif
	}

	// コンソール終了シグナルの受け口を設置する

	void	InstallStopSignalHandler(void (*pfnRequestQuit)(void), bool (*pfnIsQuitting)(void))
	{
#ifdef _WIN32
		g_pfnRequestQuit = pfnRequestQuit;
		g_pfnIsQuitting = pfnIsQuitting;
		SetConsoleCtrlHandler(&ConsoleCtrlHandlerThunk, TRUE);
#else
		(void)pfnIsQuitting;
		g_pfnRequestQuit = pfnRequestQuit;

		struct sigaction sa;
		std::memset(&sa, 0, sizeof(sa));
		sa.sa_handler = QuitSignalHandler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		if (sigaction(SIGINT, &sa, &g_oldSigInt) == 0) {
			g_bSigIntInstalled = true;
		}
#endif
	}

	// InstallStopSignalHandler() で設置したハンドラを外す

	void	UninstallStopSignalHandler(void)
	{
#ifdef _WIN32
		SetConsoleCtrlHandler(&ConsoleCtrlHandlerThunk, FALSE);
		g_pfnRequestQuit = NULL;
		g_pfnIsQuitting = NULL;
#else
		if (g_bSigIntInstalled) {
			sigaction(SIGINT, &g_oldSigInt, NULL);
			g_bSigIntInstalled = false;
		}
		g_pfnRequestQuit = NULL;
#endif
	}

	// 親プロセスのコンソールへ接続する(あれば)

	void	AttachParentConsole(void)
	{
#ifdef _WIN32
		FILE *pFile;

		if (AttachConsole(ATTACH_PARENT_PROCESS) == FALSE) {
			return;
		}
		freopen_s(&pFile, "CONOUT$", "w", stdout);
		freopen_s(&pFile, "CONOUT$", "w", stderr);
#else
		// 非Windows では標準出力/エラー出力は最初から親から継承されている。
#endif
	}

	// 高分解能タイマーの利用を開始する

	void	BeginHighResolutionTimer(void)
	{
#ifdef _WIN32
		TIMECAPS tc;

		timeGetDevCaps(&tc, sizeof(TIMECAPS));
		timeBeginPeriod(tc.wPeriodMin);
		g_uTimerPeriodMin = tc.wPeriodMin;
#else
		// POSIX の OS タイマーは元から高分解能なので何もしない。
#endif
	}

	// BeginHighResolutionTimer() と対にして呼ぶ

	void	EndHighResolutionTimer(void)
	{
#ifdef _WIN32
		timeEndPeriod(g_uTimerPeriodMin);
#else
		// 何もしない(BeginHighResolutionTimer 参照)。
#endif
	}

	// 短時間 CPU を手放す(TimerProc の周回ペース調整用)

	void	SleepMs(unsigned int uMs)
	{
#ifdef _WIN32
		Sleep(uMs);
#else
		usleep(uMs * 1000);
#endif
	}
}
