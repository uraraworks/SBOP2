/// @file TCharCompat.h
/// @brief 非Windows環境向け TCHAR系文字列操作関数・変換関数スタブ
/// @date 2026/04/07

#pragma once

#if !defined(_WIN32)

// -----------------------------------------------------------------------
// TCHAR 系文字列操作関数
// -----------------------------------------------------------------------

/// wchar_t 文字列長を返す
inline int _tcslen(LPCTSTR pStr)
{
	return (pStr == NULL) ? 0 : (int)wcslen(pStr);
}

/// wchar_t 文字列を安全にコピーする（バッファサイズ付き）
inline TCHAR *_tcscpy_s(LPTSTR pszDst, size_t nDstCount, LPCTSTR pszSrc)
{
	if ((pszDst == NULL) || (nDstCount == 0)) {
		return NULL;
	}
	if (pszSrc == NULL) {
		pszDst[0] = L'\0';
		return pszDst;
	}
	wcsncpy(pszDst, pszSrc, nDstCount - 1);
	pszDst[nDstCount - 1] = L'\0';
	return pszDst;
}

/// wchar_t 文字列をコピーする（バッファサイズなし版）
inline TCHAR *_tcscpy_s(LPTSTR pszDst, LPCTSTR pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL)) {
		return pszDst;
	}
	wcscpy(pszDst, pszSrc);
	return pszDst;
}

/// wchar_t 文字列をコピーする（_tcscpy エイリアス）
inline TCHAR *_tcscpy(LPTSTR pszDst, LPCTSTR pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL)) {
		return pszDst;
	}
	wcscpy(pszDst, pszSrc);
	return pszDst;
}

/// wchar_t 文字列を安全に連結する（バッファサイズ付き）
inline TCHAR *_tcscat_s(LPTSTR pszDst, size_t nDstCount, LPCTSTR pszSrc)
{
	if ((pszDst == NULL) || (nDstCount == 0) || (pszSrc == NULL)) {
		return pszDst;
	}
	size_t nLen = wcslen(pszDst);
	if (nLen >= nDstCount) {
		return pszDst;
	}
	wcsncat(pszDst, pszSrc, nDstCount - nLen - 1);
	pszDst[nDstCount - 1] = L'\0';
	return pszDst;
}

/// wchar_t 文字列を連結する（バッファサイズなし版）
inline TCHAR *_tcscat_s(LPTSTR pszDst, LPCTSTR pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL)) {
		return pszDst;
	}
	wcscat(pszDst, pszSrc);
	return pszDst;
}

/// wchar_t 文字列から指定文字を末尾から検索する（非 const 版）
inline LPTSTR _tcsrchr(LPTSTR pszStr, TCHAR ch)
{
	return (pszStr == NULL) ? NULL : wcsrchr(pszStr, ch);
}

/// wchar_t 文字列から指定文字を末尾から検索する（const 版）
inline LPCTSTR _tcsrchr(LPCTSTR pszStr, TCHAR ch)
{
	return (pszStr == NULL) ? NULL : wcsrchr(pszStr, ch);
}

/// wchar_t 文字列を先頭 nCount 文字まで比較する
inline int _tcsncmp(LPCTSTR pszLeft, LPCTSTR pszRight, size_t nCount)
{
	if ((pszLeft == NULL) && (pszRight == NULL)) {
		return 0;
	}
	if (pszLeft == NULL) {
		return -1;
	}
	if (pszRight == NULL) {
		return 1;
	}
	return wcsncmp(pszLeft, pszRight, nCount);
}

/// wchar_t 文字列から指定文字を先頭から検索する（非 const 版）
inline LPTSTR _tcschr(LPTSTR pszStr, TCHAR ch)
{
	return (pszStr == NULL) ? NULL : wcschr(pszStr, ch);
}

/// wchar_t 文字列から指定文字を先頭から検索する（const 版）
inline LPCTSTR _tcschr(LPCTSTR pszStr, TCHAR ch)
{
	return (pszStr == NULL) ? NULL : wcschr(pszStr, ch);
}

/// wchar_t バッファへ書式整形する（バッファサイズ付き）
inline int _stprintf_s(LPTSTR pszDst, size_t nDstCount, LPCTSTR pszFormat, ...)
{
	if ((pszDst == NULL) || (nDstCount == 0) || (pszFormat == NULL)) {
		return -1;
	}
	va_list args;
	va_start(args, pszFormat);
	int nRet = vswprintf(pszDst, nDstCount, pszFormat, args);
	va_end(args);
	if (nRet < 0) {
		pszDst[0] = L'\0';
	}
	return nRet;
}

/// wchar_t バッファへ書式整形する（バッファサイズなし版）
inline int _stprintf_s(LPTSTR pszDst, LPCTSTR pszFormat, ...)
{
	if ((pszDst == NULL) || (pszFormat == NULL)) {
		return -1;
	}
	va_list args;
	va_start(args, pszFormat);
	int nRet = vswprintf(pszDst, 1024, pszFormat, args);
	va_end(args);
	if (nRet < 0) {
		pszDst[0] = L'\0';
	}
	return nRet;
}

/// wchar_t バッファへ va_list で書式整形する
inline int _vsntprintf_s(LPTSTR pszDst, size_t nDstCount, size_t, LPCTSTR pszFormat, va_list args)
{
	if ((pszDst == NULL) || (nDstCount == 0) || (pszFormat == NULL)) {
		return -1;
	}
	int nRet = vswprintf(pszDst, nDstCount, pszFormat, args);
	if (nRet < 0) {
		pszDst[0] = L'\0';
	}
	return nRet;
}

/// wchar_t 文字列を整数に変換する
inline int _wtoi(LPCWSTR pszValue)
{
	return (pszValue == NULL) ? 0 : (int)wcstol(pszValue, NULL, 10);
}

/// TCHAR 文字列を整数に変換する（_wtoi エイリアス）
inline int _ttoi(LPCTSTR pszValue)
{
	return _wtoi(pszValue);
}

/// TCHAR 文字列を double に変換する
inline double _tcstod(LPCTSTR pszValue, LPTSTR *ppEnd)
{
	return (pszValue == NULL) ? 0.0 : wcstod(pszValue, ppEnd);
}

// -----------------------------------------------------------------------
// char 系文字列操作関数（MSC 非互換環境向け）
// -----------------------------------------------------------------------

#if !defined(_MSC_VER)
/// char バッファへ安全に書式整形する（バッファサイズ付き）
inline int sprintf_s(char *pszDst, size_t nDstCount, const char *pszFormat, ...)
{
	if ((pszDst == NULL) || (nDstCount == 0) || (pszFormat == NULL)) {
		return -1;
	}
	va_list args;
	va_start(args, pszFormat);
	int nRet = vsnprintf(pszDst, nDstCount, pszFormat, args);
	va_end(args);
	if (nRet < 0) {
		pszDst[0] = '\0';
	}
	return nRet;
}

/// char バッファへ安全に書式整形する（バッファサイズなし版）
inline int sprintf_s(char *pszDst, const char *pszFormat, ...)
{
	if ((pszDst == NULL) || (pszFormat == NULL)) {
		return -1;
	}
	va_list args;
	va_start(args, pszFormat);
	int nRet = vsnprintf(pszDst, 1024, pszFormat, args);
	va_end(args);
	if (nRet < 0) {
		pszDst[0] = '\0';
	}
	return nRet;
}

/// char 文字列を安全にコピーする（バッファサイズ付き）
inline char *strcpy_s(char *pszDst, size_t nDstCount, const char *pszSrc)
{
	if ((pszDst == NULL) || (nDstCount == 0)) {
		return NULL;
	}
	if (pszSrc == NULL) {
		pszDst[0] = '\0';
		return pszDst;
	}
	strncpy(pszDst, pszSrc, nDstCount - 1);
	pszDst[nDstCount - 1] = '\0';
	return pszDst;
}

/// char 文字列をコピーする（バッファサイズなし版）
inline char *strcpy_s(char *pszDst, const char *pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL)) {
		return pszDst;
	}
	strcpy(pszDst, pszSrc);
	return pszDst;
}

/// char 文字列を安全に連結する（バッファサイズ付き）
inline char *strcat_s(char *pszDst, size_t nDstCount, const char *pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL) || (nDstCount == 0)) {
		return pszDst;
	}
	const size_t nLength = strlen(pszDst);
	if (nLength >= nDstCount) {
		return pszDst;
	}
	strncat(pszDst, pszSrc, nDstCount - nLength - 1);
	pszDst[nDstCount - 1] = '\0';
	return pszDst;
}

/// char 文字列を連結する（バッファサイズなし版）
inline char *strcat_s(char *pszDst, const char *pszSrc)
{
	if ((pszDst == NULL) || (pszSrc == NULL)) {
		return pszDst;
	}
	strcat(pszDst, pszSrc);
	return pszDst;
}
#endif // !_MSC_VER

/// 大文字小文字を無視して char 文字列を比較する
inline int _stricmp(const char *pszLeft, const char *pszRight)
{
	if ((pszLeft == NULL) && (pszRight == NULL)) {
		return 0;
	}
	if (pszLeft == NULL) {
		return -1;
	}
	if (pszRight == NULL) {
		return 1;
	}
	while ((*pszLeft != '\0') && (*pszRight != '\0')) {
		const char chLeft = static_cast<char>(std::tolower(static_cast<unsigned char>(*pszLeft)));
		const char chRight = static_cast<char>(std::tolower(static_cast<unsigned char>(*pszRight)));
		if (chLeft != chRight) {
			return (chLeft < chRight) ? -1 : 1;
		}
		++pszLeft;
		++pszRight;
	}
	if (*pszLeft == *pszRight) {
		return 0;
	}
	return (*pszLeft == '\0') ? -1 : 1;
}

/// TCHAR 文字列長を返す（lstrlen エイリアス）
inline int lstrlen(LPCTSTR pStr)
{
	return _tcslen(pStr);
}

/// wchar_t 文字列長を返す
inline int lstrlenW(LPCWSTR s) { return s ? (int)wcslen(s) : 0; }

/// MessageBox スタブ（何もしない）
inline int MessageBox(HWND, LPCTSTR, LPCTSTR, UINT)
{
	return 0;
}

// -----------------------------------------------------------------------
// MultiByteToWideChar / WideCharToMultiByte（完全版）
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// UTF-8 / UTF-16(wchar_t) 変換ヘルパー（std::codecvt_utf8_utf16 代替）
// -----------------------------------------------------------------------

/// UTF-8 バイト列をワイド文字列(UTF-32/16)に変換（内部ヘルパー）
inline std::wstring Utf8ToWstring(const char *pszSrc, size_t nSrcLen)
{
	std::wstring result;
	const unsigned char *p = reinterpret_cast<const unsigned char *>(pszSrc);
	const unsigned char *pEnd = p + nSrcLen;
	while (p < pEnd) {
		uint32_t cp = 0;
		if (*p < 0x80) {
			cp = *p++;
		} else if ((*p & 0xE0) == 0xC0) {
			cp = (*p++ & 0x1F) << 6;
			if (p < pEnd) cp |= (*p++ & 0x3F);
		} else if ((*p & 0xF0) == 0xE0) {
			cp = (*p++ & 0x0F) << 12;
			if (p < pEnd) cp |= (*p++ & 0x3F) << 6;
			if (p < pEnd) cp |= (*p++ & 0x3F);
		} else if ((*p & 0xF8) == 0xF0) {
			cp = (*p++ & 0x07) << 18;
			if (p < pEnd) cp |= (*p++ & 0x3F) << 12;
			if (p < pEnd) cp |= (*p++ & 0x3F) << 6;
			if (p < pEnd) cp |= (*p++ & 0x3F);
		} else {
			++p; // 無効バイトはスキップ
			continue;
		}
		if (sizeof(wchar_t) == 2 && cp >= 0x10000) {
			// UTF-16 サロゲートペア
			cp -= 0x10000;
			result.push_back(static_cast<wchar_t>(0xD800 | (cp >> 10)));
			result.push_back(static_cast<wchar_t>(0xDC00 | (cp & 0x3FF)));
		} else {
			result.push_back(static_cast<wchar_t>(cp));
		}
	}
	return result;
}

/// ワイド文字列をUTF-8バイト列に変換（内部ヘルパー）
inline std::string WstringToUtf8(const wchar_t *pszSrc, size_t nSrcLen)
{
	std::string result;
	const wchar_t *p = pszSrc;
	const wchar_t *pEnd = p + nSrcLen;
	while (p < pEnd) {
		uint32_t cp = static_cast<uint32_t>(*p++);
		// UTF-16 サロゲートペア処理
		if (sizeof(wchar_t) == 2 && cp >= 0xD800 && cp <= 0xDBFF && p < pEnd) {
			uint32_t lo = static_cast<uint32_t>(*p);
			if (lo >= 0xDC00 && lo <= 0xDFFF) {
				cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
				++p;
			}
		}
		if (cp < 0x80) {
			result.push_back(static_cast<char>(cp));
		} else if (cp < 0x800) {
			result.push_back(static_cast<char>(0xC0 | (cp >> 6)));
			result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
		} else if (cp < 0x10000) {
			result.push_back(static_cast<char>(0xE0 | (cp >> 12)));
			result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
		} else {
			result.push_back(static_cast<char>(0xF0 | (cp >> 18)));
			result.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
		}
	}
	return result;
}

/// マルチバイト文字列をワイド文字列に変換する
inline int MultiByteToWideChar(UINT, DWORD, LPCSTR pszSrc, int nSrcLen, LPWSTR pszDst, int nDstLen)
{
	if (pszSrc == NULL) {
		return 0;
	}

	size_t srcLen = (nSrcLen < 0) ? strlen(pszSrc) : static_cast<size_t>(nSrcLen);
	std::wstring strWide = Utf8ToWstring(pszSrc, srcLen);
	int nRequired = (int)strWide.size() + ((nSrcLen < 0) ? 1 : 0);

	if ((pszDst == NULL) || (nDstLen <= 0)) {
		return nRequired;
	}

	int nCopy = min((int)strWide.size(), nDstLen - 1);
	wmemcpy(pszDst, strWide.c_str(), nCopy);
	if (nSrcLen < 0) {
		pszDst[nCopy] = L'\0';
		return nCopy + 1;
	}
	return nCopy;
}

/// ワイド文字列をマルチバイト文字列に変換する
inline int WideCharToMultiByte(UINT, DWORD, LPCWSTR pszSrc, int nSrcLen, LPSTR pszDst, int nDstLen, LPCSTR, BOOL *)
{
	if (pszSrc == NULL) {
		return 0;
	}

	size_t srcLen = (nSrcLen < 0) ? wcslen(pszSrc) : static_cast<size_t>(nSrcLen);
	std::string strUtf8 = WstringToUtf8(pszSrc, srcLen);
	int nRequired = (int)strUtf8.size() + ((nSrcLen < 0) ? 1 : 0);

	if ((pszDst == NULL) || (nDstLen <= 0)) {
		return nRequired;
	}

	int nCopy = min((int)strUtf8.size(), nDstLen - 1);
	memcpy(pszDst, strUtf8.data(), (size_t)nCopy);
	if (nSrcLen < 0) {
		pszDst[nCopy] = '\0';
		return nCopy + 1;
	}
	return nCopy;
}

/// _ftprintf マクロ（fwprintf エイリアス）
#define _ftprintf fwprintf

/// 配列要素数を返すマクロ
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))

#endif // !_WIN32
