/// @file Win32ApiStubs.h
/// @brief 非Windows環境向け その他Win32 APIスタブ群
/// @date 2026/04/07

#pragma once

#if !defined(_WIN32)

// -----------------------------------------------------------------------
// CRITICAL_SECTION 操作（SDL_mutex にマッピング）
// -----------------------------------------------------------------------

/// InitializeCriticalSection: SDL_CreateMutex でミューテックスを生成する
inline void InitializeCriticalSection(LPCRITICAL_SECTION pSection)
{
	if (pSection == NULL) {
		return;
	}
	pSection->pMutex = SDL_CreateMutex();
}

/// DeleteCriticalSection: SDL_DestroyMutex でミューテックスを破棄する
inline void DeleteCriticalSection(LPCRITICAL_SECTION pSection)
{
	if ((pSection == NULL) || (pSection->pMutex == NULL)) {
		return;
	}
	SDL_DestroyMutex(pSection->pMutex);
	pSection->pMutex = NULL;
}

/// EnterCriticalSection: SDL_LockMutex でロックする
inline void EnterCriticalSection(LPCRITICAL_SECTION pSection)
{
	if ((pSection == NULL) || (pSection->pMutex == NULL)) {
		return;
	}
	SDL_LockMutex(pSection->pMutex);
}

/// LeaveCriticalSection: SDL_UnlockMutex でアンロックする
inline void LeaveCriticalSection(LPCRITICAL_SECTION pSection)
{
	if ((pSection == NULL) || (pSection->pMutex == NULL)) {
		return;
	}
	SDL_UnlockMutex(pSection->pMutex);
}

// -----------------------------------------------------------------------
// タイム系
// -----------------------------------------------------------------------

/// GetLocalTime: std::localtime から SYSTEMTIME に変換する
inline void GetLocalTime(LPSYSTEMTIME pSystemTime)
{
	if (pSystemTime == NULL) {
		return;
	}

	std::time_t now = std::time(NULL);
	std::tm *pTm = std::localtime(&now);
	if (pTm == NULL) {
		ZeroMemory(pSystemTime, sizeof(SYSTEMTIME));
		return;
	}

	pSystemTime->wYear = (WORD)(pTm->tm_year + 1900);
	pSystemTime->wMonth = (WORD)(pTm->tm_mon + 1);
	pSystemTime->wDay = (WORD)pTm->tm_mday;
	pSystemTime->wDayOfWeek = (WORD)pTm->tm_wday;
	pSystemTime->wHour = (WORD)pTm->tm_hour;
	pSystemTime->wMinute = (WORD)pTm->tm_min;
	pSystemTime->wSecond = (WORD)pTm->tm_sec;
	pSystemTime->wMilliseconds = 0;
}

/// timeGetTime: SDL_GetTicks にマッピング
inline DWORD timeGetTime(void)
{
	return (DWORD)SDL_GetTicks();
}

/// GetTickCount: timeGetTime エイリアス
inline DWORD GetTickCount(void)
{
	return timeGetTime();
}

/// Sleep スタブ（何もしない）
inline void Sleep(DWORD)
{
}

// -----------------------------------------------------------------------
// モジュール情報取得スタブ
// -----------------------------------------------------------------------

/// GetModuleFileName: ダミーパスを返す
inline DWORD GetModuleFileName(HINSTANCE, LPTSTR pszFileName, DWORD nSize)
{
	static const wchar_t kDummyPath[] = L"./sbocli.html";
	if ((pszFileName == NULL) || (nSize == 0)) {
		return 0;
	}
	wcsncpy(pszFileName, kDummyPath, nSize - 1);
	pszFileName[nSize - 1] = L'\0';
	return (DWORD)wcslen(pszFileName);
}

/// GetModuleHandle スタブ（NULL を返す）
inline HINSTANCE GetModuleHandle(LPCTSTR)
{
	return NULL;
}

// -----------------------------------------------------------------------
// パス操作スタブ
// -----------------------------------------------------------------------

/// PathRemoveFileSpec: パスの末尾ファイル名部分を除去する
inline BOOL PathRemoveFileSpec(LPTSTR pszPath)
{
	if ((pszPath == NULL) || (pszPath[0] == L'\0')) {
		return FALSE;
	}
	LPTSTR pszLastSlash = _tcsrchr(pszPath, L'\\');
	if (pszLastSlash == NULL) {
		pszLastSlash = _tcsrchr(pszPath, L'/');
	}
	if (pszLastSlash == NULL) {
		pszPath[0] = L'\0';
		return FALSE;
	}
	*pszLastSlash = L'\0';
	return TRUE;
}

/// PathAddBackslash: パス末尾にバックスラッシュを付加する
inline BOOL PathAddBackslash(LPTSTR pszPath)
{
	if (pszPath == NULL) {
		return FALSE;
	}
	const size_t nLength = wcslen(pszPath);
	if (nLength == 0) {
		return FALSE;
	}
	if ((pszPath[nLength - 1] == L'\\') || (pszPath[nLength - 1] == L'/')) {
		return TRUE;
	}
	pszPath[nLength] = L'\\';
	pszPath[nLength + 1] = L'\0';
	return TRUE;
}

// -----------------------------------------------------------------------
// ディレクトリ操作スタブ
// -----------------------------------------------------------------------

/// GetCurrentDirectory: GetModuleFileName にマッピング
inline DWORD GetCurrentDirectory(DWORD nSize, LPTSTR pszBuffer)
{
	return GetModuleFileName(NULL, pszBuffer, nSize);
}

/// SetCurrentDirectory スタブ（何もしない）
inline BOOL SetCurrentDirectory(LPCTSTR)
{
	return TRUE;
}

/// CreateDirectory スタブ（何もしない）
inline BOOL CreateDirectory(LPCTSTR, LPVOID)
{
	return TRUE;
}

/// GetLastError スタブ（常に 0 を返す）
inline DWORD GetLastError(void)
{
	return 0;
}

// -----------------------------------------------------------------------
// iniファイル読み書きヘルパー（compat_ini_ プレフィックスで衝突回避）
// -----------------------------------------------------------------------

/// wchar_t パスを UTF-8 文字列に変換する
inline std::string compat_ini_wcs_to_utf8(const wchar_t* pwsz)
{
	if (pwsz == NULL) return std::string();
	// mbstowcs の逆：wcstombs でロケール依存変換
	char buf[4096];
	size_t n = wcstombs(buf, pwsz, sizeof(buf) - 1);
	if (n == (size_t)-1) return std::string();
	buf[n] = '\0';
	// パス区切り '\' → '/'
	for (size_t i = 0; buf[i] != '\0'; ++i) {
		if (buf[i] == '\\') buf[i] = '/';
	}
	return std::string(buf);
}

/// 文字列前後の空白を除去する
inline std::string compat_ini_trim(const std::string& s)
{
	size_t b = s.find_first_not_of(" \t\r\n");
	if (b == std::string::npos) return std::string();
	size_t e = s.find_last_not_of(" \t\r\n");
	return s.substr(b, e - b + 1);
}

/// iniファイルをパースして map<section, map<key, value>> を返す
inline std::map<std::string, std::map<std::string, std::string>>
compat_ini_load(const std::string& path)
{
	std::map<std::string, std::map<std::string, std::string>> data;
	std::ifstream ifs(path.c_str());
	if (!ifs.is_open()) return data;

	std::string line;
	std::string curSection;
	while (std::getline(ifs, line)) {
		std::string t = compat_ini_trim(line);
		if (t.empty() || t[0] == ';' || t[0] == '#') continue; // コメント行スキップ
		if (t[0] == '[') {
			// セクション行
			size_t end = t.find(']');
			if (end != std::string::npos) {
				curSection = compat_ini_trim(t.substr(1, end - 1));
			}
		} else {
			// キー=値 行
			size_t eq = t.find('=');
			if (eq != std::string::npos) {
				std::string key = compat_ini_trim(t.substr(0, eq));
				std::string val = compat_ini_trim(t.substr(eq + 1));
				data[curSection][key] = val;
			}
		}
	}
	return data;
}

/// iniデータを map から指定パスのファイルへ書き出す
inline bool compat_ini_save(
	const std::string& path,
	const std::map<std::string, std::map<std::string, std::string>>& data)
{
	std::ofstream ofs(path.c_str());
	if (!ofs.is_open()) return false;

	for (auto& sec : data) {
		ofs << "[" << sec.first << "]\n";
		for (auto& kv : sec.second) {
			ofs << kv.first << "=" << kv.second << "\n";
		}
		ofs << "\n";
	}
	return true;
}

// -----------------------------------------------------------------------
// GetPrivateProfileInt: iniファイルからキーを読んで整数返す
// -----------------------------------------------------------------------
inline UINT GetPrivateProfileInt(
	LPCTSTR pszSection, LPCTSTR pszKey, int nDefault, LPCTSTR pszFileName)
{
	if (pszSection == NULL || pszKey == NULL || pszFileName == NULL) {
		return (UINT)nDefault;
	}
	std::string path    = compat_ini_wcs_to_utf8(pszFileName);
	std::string section = compat_ini_wcs_to_utf8(pszSection);
	std::string key     = compat_ini_wcs_to_utf8(pszKey);

	auto data = compat_ini_load(path);
	auto itSec = data.find(section);
	if (itSec == data.end()) return (UINT)nDefault;
	auto itKey = itSec->second.find(key);
	if (itKey == itSec->second.end()) return (UINT)nDefault;

	return (UINT)atoi(itKey->second.c_str());
}

// -----------------------------------------------------------------------
// GetPrivateProfileString: iniファイルからキーを読んでバッファにコピー
// -----------------------------------------------------------------------
inline DWORD GetPrivateProfileString(
	LPCTSTR pszSection, LPCTSTR pszKey, LPCTSTR pszDefault,
	LPTSTR pszReturnedString, DWORD nSize, LPCTSTR pszFileName)
{
	if ((pszReturnedString == NULL) || (nSize == 0)) {
		return 0;
	}

	// 読み込み結果を格納する文字列（デフォルト値で初期化）
	const wchar_t* pwszFallback = (pszDefault != NULL) ? pszDefault : _T("");
	std::string fallbackUtf8 = compat_ini_wcs_to_utf8(pwszFallback);
	std::string resultUtf8   = fallbackUtf8;

	if (pszSection != NULL && pszKey != NULL && pszFileName != NULL) {
		std::string path    = compat_ini_wcs_to_utf8(pszFileName);
		std::string section = compat_ini_wcs_to_utf8(pszSection);
		std::string key     = compat_ini_wcs_to_utf8(pszKey);

		auto data = compat_ini_load(path);
		auto itSec = data.find(section);
		if (itSec != data.end()) {
			auto itKey = itSec->second.find(key);
			if (itKey != itSec->second.end()) {
				resultUtf8 = itKey->second;
			}
		}
	}

	// UTF-8 → wchar_t に変換してバッファへコピー
	wchar_t wbuf[4096];
	size_t n = mbstowcs(wbuf, resultUtf8.c_str(), sizeof(wbuf) / sizeof(wbuf[0]) - 1);
	if (n == (size_t)-1) n = 0;
	wbuf[n] = L'\0';

	_tcscpy_s(pszReturnedString, nSize, wbuf);
	return (DWORD)_tcslen(pszReturnedString);
}

// -----------------------------------------------------------------------
// WritePrivateProfileString: iniファイルのキーを更新または追加して書き戻す
// -----------------------------------------------------------------------
inline BOOL WritePrivateProfileString(
	LPCTSTR pszSection, LPCTSTR pszKey, LPCTSTR pszString, LPCTSTR pszFileName)
{
	if (pszSection == NULL || pszKey == NULL || pszFileName == NULL) {
		return FALSE;
	}
	std::string path    = compat_ini_wcs_to_utf8(pszFileName);
	std::string section = compat_ini_wcs_to_utf8(pszSection);
	std::string key     = compat_ini_wcs_to_utf8(pszKey);
	std::string value   = (pszString != NULL) ? compat_ini_wcs_to_utf8(pszString) : std::string();

	// 既存データを読み込んでキーを更新（なければ新規追加）
	auto data = compat_ini_load(path);
	if (pszString != NULL) {
		data[section][key] = value;
	} else {
		// pszString == NULL はキー削除を意味する
		auto itSec = data.find(section);
		if (itSec != data.end()) {
			itSec->second.erase(key);
		}
	}

	return compat_ini_save(path, data) ? TRUE : FALSE;
}

// -----------------------------------------------------------------------
// ファイル属性・存在確認
// -----------------------------------------------------------------------

/// GetFileAttributes: stat() を使ってファイル属性を返す
inline DWORD GetFileAttributes(LPCTSTR pszPath)
{
	if (pszPath == NULL) {
		return INVALID_FILE_ATTRIBUTES;
	}

	char szUtf8[MAX_PATH * 4];
	size_t nConverted = wcstombs(szUtf8, pszPath, sizeof(szUtf8) - 1);
	if (nConverted == (size_t)-1) {
		return INVALID_FILE_ATTRIBUTES;
	}
	szUtf8[nConverted] = '\0';
	for (size_t i = 0; szUtf8[i] != '\0'; ++i) {
		if (szUtf8[i] == '\\') {
			szUtf8[i] = '/';
		}
	}

	struct stat st;
	if (stat(szUtf8, &st) != 0) {
		return INVALID_FILE_ATTRIBUTES;
	}

#if defined(S_ISDIR)
	return S_ISDIR(st.st_mode) ? FILE_ATTRIBUTE_DIRECTORY : 0;
#else
	return ((st.st_mode & _S_IFDIR) != 0) ? FILE_ATTRIBUTE_DIRECTORY : 0;
#endif
}

/// PathFileExists: GetFileAttributes が INVALID_FILE_ATTRIBUTES でなければ TRUE
inline BOOL PathFileExists(LPCTSTR pszPath)
{
	return (GetFileAttributes(pszPath) != INVALID_FILE_ATTRIBUTES) ? TRUE : FALSE;
}

// -----------------------------------------------------------------------
// COM スタブ
// -----------------------------------------------------------------------

/// CoCreateGuid: 簡易 GUID を生成する
inline HRESULT CoCreateGuid(GUID *pGuid)
{
	static DWORD s_dwCounter = 1;
	if (pGuid == NULL) {
		return E_FAIL;
	}

	ZeroMemory(pGuid, sizeof(GUID));
	pGuid->Data1 = s_dwCounter++;
	pGuid->Data2 = 0x5342;
	pGuid->Data3 = 0x4f50;
	pGuid->Data4[0] = 'W';
	pGuid->Data4[1] = 'E';
	pGuid->Data4[2] = 'B';
	return S_OK;
}

/// CoInitializeEx スタブ（S_OK を返す）
inline HRESULT CoInitializeEx(LPVOID, DWORD)
{
	return S_OK;
}

/// CoUninitialize スタブ（何もしない）
inline void CoUninitialize(void)
{
}

/// StringFromGUID2: GUID を文字列表現に変換する
inline int StringFromGUID2(const GUID &guid, LPWSTR pszBuffer, int cchMax)
{
	if ((pszBuffer == NULL) || (cchMax <= 0)) {
		return 0;
	}
	int nRet = swprintf(
		pszBuffer,
		(size_t)cchMax,
		L"{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1,
		guid.Data2,
		guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	return (nRet < 0) ? 0 : (nRet + 1);
}

/// CLSIDFromString スタブ（GUID をゼロクリアして S_OK を返す）
inline HRESULT CLSIDFromString(LPCWSTR, GUID *pGuid)
{
	if (pGuid == NULL) {
		return E_FAIL;
	}
	ZeroMemory(pGuid, sizeof(GUID));
	return S_OK;
}

// -----------------------------------------------------------------------
// DLL 操作スタブ
// -----------------------------------------------------------------------

/// LoadLibrary スタブ（NULL を返す）
inline HMODULE LoadLibrary(LPCTSTR)
{
	return NULL;
}

/// GetProcAddress スタブ（NULL を返す）
inline FARPROC GetProcAddress(HMODULE, LPCSTR)
{
	return NULL;
}

/// FreeLibrary スタブ（TRUE を返す）
inline BOOL FreeLibrary(HMODULE)
{
	return TRUE;
}

// -----------------------------------------------------------------------
// リソース操作スタブ
// -----------------------------------------------------------------------

/// FindResource スタブ（NULL を返す）
inline HRSRC FindResource(HMODULE, LPCTSTR, LPCTSTR)
{
	return NULL;
}

/// SizeofResource スタブ（0 を返す）
inline DWORD SizeofResource(HMODULE, HRSRC)
{
	return 0;
}

/// LoadResource スタブ（NULL を返す）
inline HGLOBAL LoadResource(HMODULE, HRSRC)
{
	return NULL;
}

/// LockResource スタブ（NULL を返す）
inline LPVOID LockResource(HGLOBAL)
{
	return NULL;
}

/// LoadString スタブ（空文字列をセットして 0 を返す）
inline int LoadString(HINSTANCE, UINT, LPTSTR pszBuffer, int nMaxCount)
{
	if ((pszBuffer != NULL) && (nMaxCount > 0)) {
		pszBuffer[0] = L'\0';
	}
	return 0;
}

// -----------------------------------------------------------------------
// ファイル操作
// -----------------------------------------------------------------------

/// BrowserCompatWideToUtf8: wchar_t 文字列を UTF-8 std::string に変換する
inline std::string BrowserCompatWideToUtf8(LPCTSTR pszSrc)
{
	if (pszSrc == NULL) {
		return std::string();
	}
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(pszSrc);
}

/// _tfopen_s: ワイド文字パスを UTF-8 変換して fopen する
inline errno_t _tfopen_s(FILE **ppFile, LPCTSTR pszFileName, LPCTSTR pszMode)
{
	if (ppFile == NULL) {
		return EINVAL;
	}
	*ppFile = NULL;
	if ((pszFileName == NULL) || (pszMode == NULL)) {
		return EINVAL;
	}
	std::string strFileName = BrowserCompatWideToUtf8(pszFileName);
	const std::string strMode = BrowserCompatWideToUtf8(pszMode);
	for (size_t i = 0; i < strFileName.size(); ++i) {
		if (strFileName[i] == '\\') {
			strFileName[i] = '/';
		}
	}
	*ppFile = fopen(strFileName.c_str(), strMode.c_str());
	return (*ppFile == NULL) ? errno : 0;
}

#endif // !_WIN32
