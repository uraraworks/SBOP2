/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:SBOGlobal.cpp												 */
/* 内容			:プロジェクト依存ではない共通処理 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/06													 */
/* ========================================================================= */

#include "StdAfx.h"

/* ========================================================================= */
/* 関数プロトタイプ宣言														 */
/* ========================================================================= */

PBYTE	ZeroNew			(DWORD dwSize);												/* 指定サイズのメモリを確保して0クリア */
void	CopyMemoryRenew	(PVOID pDst, CONST PVOID pSrc, DWORD dwSize, PBYTE &pPos);	/* メモリコピーしてポインタを進める */
void	strcpyRenew		(LPSTR pszDst, LPCSTR pszSrc, PBYTE &pPos);					/* 文字列コピーしてポインタを進める */
void	StoreRenew		(CmyString &strDst, LPCSTR pszSrc, PBYTE &pPos);			/* 文字列コピーしてポインタを進める */

/* ========================================================================= */
/* 関数名	:ZeroNew														 */
/* 内容		:指定サイズのメモリを確保して0クリア							 */
/* 日付		:2006/11/06														 */
/* ========================================================================= */

PBYTE ZeroNew(DWORD dwSize)
{
	PBYTE pRet;

	pRet = new BYTE[dwSize];
	ZeroMemory (pRet, dwSize);

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CopyMemoryRenew												 */
/* 内容		:メモリコピーしてポインタを進める								 */
/* 日付		:2006/06/08														 */
/* ========================================================================= */

void CopyMemoryRenew(PVOID pDst, CONST PVOID pSrc, DWORD dwSize, PBYTE &pPos)
{
	CopyMemory (pDst, pSrc, dwSize);
	pPos += dwSize;
}


/* ========================================================================= */
/* 関数名	:strcpyRenew													 */
/* 内容		:文字列コピーしてポインタを進める								 */
/* 日付		:2006/06/08														 */
/* ========================================================================= */

void strcpyRenew(LPSTR pszDst, LPCSTR pszSrc, PBYTE &pPos)
{
	if ((pszSrc == NULL) || (strlen (pszSrc) <= 0)) {
		pPos ++;
		return;
	}
	strcpy (pszDst, pszSrc);
	pPos += (strlen (pszSrc) + 1);
}


/* ========================================================================= */
/* 関数名	:StoreRenew														 */
/* 内容		:文字列コピーしてポインタを進める								 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void StoreRenew(CmyString &strDst, LPCSTR pszSrc, PBYTE &pPos)
{
	if ((pszSrc == NULL) || (strlen (pszSrc) <= 0)) {
		pPos ++;
		return;
	}
	strDst = pszSrc;
	pPos += (strDst.GetLength () + 1);
}


/* ========================================================================= */
/* 関数名	:GetModuleFilePath												 */
/* 内容		:モジュールパスを取得											 */
/* 日付		:2007/05/03														 */
/* ========================================================================= */

void GetModuleFilePath(
	LPSTR pszDst,		/* [ou] パス格納先バッファ */
	DWORD dwSize)		/* [in] パス格納先バッファサイズ */
{
	ZeroMemory (pszDst, dwSize);
	GetModuleFileName (NULL, pszDst, dwSize);	/* モジュール名を取得 */
	PathRemoveFileSpec (pszDst);				/* ファイル名部分を消す */
	PathAddBackslash (pszDst);					/* 「\」を追加 */
}


/* ========================================================================= */
/* 関数名	:AllCreateDirectory												 */
/* 内容		:指定されたパスのフォルダを作成									 */
/* 日付		:2008/03/09														 */
/* ========================================================================= */

BOOL AllCreateDirectory(
	LPCTSTR					lpszDir,			/* [in] ディレクトリ名					*/
	LPSECURITY_ATTRIBUTES	pSAtt,				/* [in] (基本：NULL)セキュリティ識別子	*/
	BOOL					bResult				/* [in] (基本：TRUE)同名パスが存在する時*/
												/*		TRUE FALSE どちらを返すか		*/
	)
{
	UINT	nStrLen;
	TCHAR	szTempDir[MAX_PATH] = _T("");
	LPTSTR	posRoot;
	TCHAR	ch = _T('\0');


	/*****************************************
		初期チェック
	*****************************************/

	if (lpszDir == NULL) {
		return FALSE;
	}
	nStrLen = _tcslen (lpszDir);
	if ((nStrLen == 0) || (nStrLen >= MAX_PATH)){
		return FALSE;
	}


	/*****************************************
		とりあえず作成
	*****************************************/

	if (CreateDirectory (lpszDir, pSAtt)) {
		return TRUE;
	}

	if (GetLastError () == ERROR_ALREADY_EXISTS) {
		return bResult;
	}


	/*****************************************
		 作成開始位置を検出
	*****************************************/

	_tcscpy (szTempDir, lpszDir);
	szTempDir[MAX_PATH -1] = _T('\0');

	/* ネットワークフォルダを指定されている？ */
	if (memcmp (szTempDir, _T("\\\\"), 2) == 0) {
		posRoot = &szTempDir[1];
		posRoot = _tcschr (posRoot + 1, _T('\\'));
		if (posRoot == NULL) {
			return FALSE;
		}
		posRoot = _tcschr (posRoot + 1, _T('\\'));
		if (posRoot == NULL) {
			return FALSE;
		}

	/* ローカルフォルダを指定されている */
	} else {
		posRoot = _tcschr (szTempDir, _T('\\'));
		if (posRoot == NULL) {
			return FALSE;
		}
	}


	/********************************************
		ディレクトリを先頭から順に作成していく
	********************************************/

	while (posRoot) {
		posRoot = _tcschr (posRoot + 1, _T('\\'));
		if (posRoot) {
			ch = *posRoot;
			/* 文字列を一旦切る（あとで元に戻す）。	*/
			*posRoot = NULL;
		}

		if  (!CreateDirectory(szTempDir, pSAtt)) {
			if (ERROR_ALREADY_EXISTS != GetLastError()) {
				return FALSE;
			}
		}
		if (posRoot) {
			/* 区切った位置を元に戻す */
			*posRoot = ch;
		}
	}

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:TrimViewString													 */
/* 内容		:表示できない文字を削除											 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void TrimViewString(CmyString &strDst, LPCSTR pszSrc)
{
	int i, nLen;
	BYTE byTmp;
	char szTmp[3];

	strDst.Empty ();
	if (pszSrc == NULL) {
		return;
	}
	nLen = strlen (pszSrc);
	if (nLen <= 0) {
		return;
	}
	ZeroMemory (szTmp, sizeof (szTmp));

	for (i = 0; i < nLen; i ++) {
		byTmp = (BYTE)pszSrc[i];
		if (IsDBCSLeadByte (byTmp)) {
			szTmp[0] = byTmp;
			szTmp[1] = pszSrc[i + 1];
			strDst += szTmp;
			i ++;
			continue;
		}
		if ((byTmp < 0x20) || ((byTmp >= 0x7F) && !((byTmp >= 0xA1) && (byTmp <= 0xDF)))) {
			continue;
		}
		szTmp[0] = byTmp;
		szTmp[1] = 0;
		strDst += szTmp;
	}
}


/* ========================================================================= */
/* 関数名	:IsInRect														 */
/* 内容		:指定矩形が完全に内側にあるか判定								 */
/* 日付		:2009/01/31														 */
/* ========================================================================= */

BOOL IsInRect(RECT *pSrc, RECT *pTarget)
{
	BOOL bRet;

	bRet = FALSE;

	if ((pSrc->left >= pTarget->left) && (pSrc->right <= pTarget->right) &&
		(pSrc->top >= pTarget->top) && (pSrc->bottom <= pTarget->bottom)) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:IsHitRect														 */
/* 内容		:矩形同士の当たり判定											 */
/* 日付		:2009/01/31														 */
/* ========================================================================= */

BOOL IsHitRect(RECT *pSrc1, RECT *pSrc2)
{
	BOOL bRet;

	bRet = FALSE;

	if ((pSrc1->left <= pSrc2->right) && (pSrc2->left <= pSrc1->right) &&
		(pSrc1->top <= pSrc2->bottom) && (pSrc2->top <= pSrc1->bottom)) {
		bRet = TRUE;
	}

	return bRet;
}

/* Copyright(C)URARA-works 2006 */
