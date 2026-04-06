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

/// マルチバイト文字列をワイド文字列に変換する
inline int MultiByteToWideChar(UINT, DWORD, LPCSTR pszSrc, int nSrcLen, LPWSTR pszDst, int nDstLen)
{
	if (pszSrc == NULL) {
		return 0;
	}

	std::string strSrc;
	if (nSrcLen < 0) {
		strSrc = pszSrc;
	} else {
		strSrc.assign(pszSrc, pszSrc + nSrcLen);
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring strWide = converter.from_bytes(strSrc);
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

	std::wstring strSrc;
	if (nSrcLen < 0) {
		strSrc = pszSrc;
	} else {
		strSrc.assign(pszSrc, pszSrc + nSrcLen);
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string strUtf8 = converter.to_bytes(strSrc);
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
