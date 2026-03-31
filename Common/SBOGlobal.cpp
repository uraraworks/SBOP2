#include "StdAfx.h"

// 関数プロトタイプ宣言

PBYTE	ZeroNew(DWORD dwSize);											// 指定サイズのメモリを確保して0クリア
void	CopyMemoryRenew(PVOID pDst, CONST PVOID pSrc, DWORD dwSize, PBYTE &pPos);	// メモリコピーしてポインタを進める
void	strcpyRenew(LPSTR pszDst, LPCSTR pszSrc, PBYTE &pPos);						// 文字列コピーしてポインタを進める
void	StoreRenew(CmyString &strDst, LPCSTR pszSrc, PBYTE &pPos);					// 文字列コピーしてポインタを進める

// ZeroNew

PBYTE ZeroNew(DWORD dwSize)
{
	PBYTE pRet;

	pRet = new BYTE[dwSize];
	ZeroMemory(pRet, dwSize);

	return pRet;
}

// CopyMemoryRenew

void CopyMemoryRenew(PVOID pDst, CONST PVOID pSrc, DWORD dwSize, PBYTE &pPos)
{
	CopyMemory(pDst, pSrc, dwSize);
	pPos += dwSize;
}

// strcpyRenew

void strcpyRenew(LPSTR pszDst, LPCSTR pszSrc, PBYTE &pPos)
{
	if ((pszSrc == NULL) || (strlen(pszSrc) <= 0)) {
		pPos ++;
		return;
	}
	strcpy(pszDst, pszSrc);
	pPos += (strlen(pszSrc) + 1);
}

void strcpyRenew(LPSTR pszDst, const CmyString &strSrc, PBYTE &pPos)
{
#ifdef _UNICODE
	// 旧パケット/保存形式との互換のため、この経路は CP932 を維持する
	CStringA strAnsi = TStringToAnsi(static_cast<LPCTSTR>(strSrc), SBO_LEGACY_CODEPAGE);
	LPCSTR pszSrc = strAnsi.GetString();
	int nLength = strAnsi.GetLength();
	if ((pszSrc == NULL) || (nLength <= 0)) {
		if (pszDst) {
			pszDst[0] = '\0';
		}
		pPos ++;
		return;
	}
	strcpy(pszDst, pszSrc);
	pPos += (nLength + 1);
#else
	strcpyRenew(pszDst, static_cast<LPCSTR>(strSrc), pPos);
#endif
}

// StoreRenew

void StoreRenew(CmyString &strDst, LPCSTR pszSrc, PBYTE &pPos)
{
	if ((pszSrc == NULL) || (strlen(pszSrc) <= 0)) {
		strDst.Empty();
		pPos ++;
		return;
	}
	// 旧パケット/保存形式との互換のため、この経路は CP932 を維持する
	CString strConverted = AnsiToTString(pszSrc, SBO_LEGACY_CODEPAGE);
	strDst = (LPCTSTR)strConverted;
	pPos += (strlen(pszSrc) + 1);
}

// GetModuleFilePath

void GetModuleFilePath(
	LPTSTR pszDst,		// [ou] パス格納先バッファ
	DWORD dwSize)		// [in] パス格納先バッファサイズ
{
	if ((pszDst == NULL) || (dwSize == 0)) {
		return;
	}
	ZeroMemory(pszDst, sizeof(TCHAR) * dwSize);
	GetModuleFileName(NULL, pszDst, dwSize);	// モジュール名を取得
	PathRemoveFileSpec(pszDst);					// ファイル名部分を消す
	PathAddBackslash(pszDst);					// 「\\」を追加
}

// AllCreateDirectory

BOOL AllCreateDirectory(
	LPCTSTR					lpszDir,		// [in] ディレクトリ名
	LPSECURITY_ATTRIBUTES	pSAtt,			// [in] (基本：NULL)セキュリティ識別子
	BOOL					bResult			// [in] (基本：TRUE)同名パスが存在する時 TRUE FALSE どちらを返すか
	)
{
	UINT	nStrLen;
	TCHAR	szTempDir[MAX_PATH] = _T("");
	LPTSTR	posRoot;
	TCHAR	ch = _T('\0');

	// 初期チェック

	if (lpszDir == NULL) {
		return FALSE;
	}
	nStrLen = _tcslen(lpszDir);
	if ((nStrLen == 0) || (nStrLen >= MAX_PATH)){
		return FALSE;
	}

	// とりあえず作成

	if (CreateDirectory(lpszDir, pSAtt)) {
		return TRUE;
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return bResult;
	}

	// 作成開始位置を検出

	_tcscpy(szTempDir, lpszDir);
	szTempDir[MAX_PATH -1] = _T('\0');

	// ネットワークフォルダを指定されている？
	if (_tcsncmp(szTempDir, _T("\\\\"), 2) == 0) {
		posRoot = &szTempDir[1];
		posRoot = _tcschr(posRoot + 1, _T('\\'));
		if (posRoot == NULL) {
			return FALSE;
		}
		posRoot = _tcschr(posRoot + 1, _T('\\'));
		if (posRoot == NULL) {
			return FALSE;
		}

	// ローカルフォルダを指定されている
	} else {
		posRoot = _tcschr(szTempDir, _T('\\'));
		if (posRoot == NULL) {
			return FALSE;
		}
	}

	// ディレクトリを先頭から順に作成していく

	while (posRoot) {
		posRoot = _tcschr(posRoot + 1, _T('\\'));
		if (posRoot) {
			ch = *posRoot;
			// 文字列を一旦切る（あとで元に戻す）。
			*posRoot = _T('\0');
		}

		if (!CreateDirectory(szTempDir, pSAtt)) {
			if (ERROR_ALREADY_EXISTS != GetLastError()) {
				return FALSE;
			}
		}
		if (posRoot) {
			// 区切った位置を元に戻す
			*posRoot = ch;
		}
	}

	return TRUE;
}

// TrimViewString

void TrimViewString(CmyString &strDst, LPCSTR pszSrc)
{
	if (pszSrc == NULL) {
		strDst.Empty();
		return;
	}

	CmyString strTmp(pszSrc);
	TrimViewString(strDst, (LPCTSTR)strTmp);
}

void TrimViewString(CmyString &strDst, LPCTSTR pszSrc)
{
	if (pszSrc == NULL) {
		strDst.Empty();
		return;
	}

	CString strFiltered;
	int nLen = _tcslen(pszSrc);
	for (int i = 0; i < nLen; ++i) {
		TCHAR ch = pszSrc[i];
		if (ch < _T(' ') && ch != _T('\t')) {
			continue;
		}
		if (ch == 0x7F) {
			continue;
		}
		strFiltered.AppendChar(ch);
	}
	strDst = (LPCTSTR)strFiltered;
}

// IsInRect

BOOL IsInRect(RECT *pSrc, RECT *pTarget)
{
	if ((pSrc == NULL) || (pTarget == NULL)) {
		return FALSE;
	}

	return ((pSrc->left   >= pTarget->left)   &&
			(pSrc->top    >= pTarget->top)    &&
			(pSrc->right  <= pTarget->right)  &&
			(pSrc->bottom <= pTarget->bottom));
}

// IsHitRect

BOOL IsHitRect(RECT *pSrc1, RECT *pSrc2)
{
	if ((pSrc1 == NULL) || (pSrc2 == NULL)) {
		return FALSE;
	}

	if ((pSrc1->right  <= pSrc2->left) ||
		(pSrc1->left   >= pSrc2->right) ||
		(pSrc1->bottom <= pSrc2->top) ||
		(pSrc1->top    >= pSrc2->bottom)) {
		return FALSE;
	}

	return TRUE;
}
