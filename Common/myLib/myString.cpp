/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:myString.cpp												 */
/* 内容			:文字列クラス 定義ファイル									 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/05/25													 */
/* ========================================================================= */

#include "StdAfx.h"
#include <tchar.h>
#include "myString.h"

/* ========================================================================= */
/* 関数名	:CmyString::CmyString											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/05/25														 */
/* ========================================================================= */

CmyString::CmyString()
{
	m_nLength	= 0;
	m_pszString = NULL;
	Empty ();
}


/* ========================================================================= */
/* 関数名	:CmyString::CmyString											 */
/* 内容		:コピーコンストラクタ											 */
/* 日付		:2007/02/11														 */
/* ========================================================================= */

CmyString::CmyString(CmyString &strSrc)
{
	m_pszString = NULL;
	Renew ((LPCSTR)strSrc);
}
CmyString::CmyString(LPCTSTR szSrc)
{
	m_pszString = NULL;
	Renew (szSrc);
}


/* ========================================================================= */
/* 関数名	:CmyString::~CmyString											 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/05/25														 */
/* ========================================================================= */

CmyString::~CmyString()
{
	if (m_pszString) {
		delete [] m_pszString;
		m_pszString = NULL;
	}
}


/* ========================================================================= */
/* 関数名	:CmyString::Empty												 */
/* 内容		:文字列を空にする												 */
/* 日付		:2006/05/29														 */
/* ========================================================================= */

void CmyString::Empty(void)
{
	Renew ("");
}


/* ========================================================================= */
/* 関数名	:CmyString::IsEmpty												 */
/* 内容		:文字列が空か判定												 */
/* 日付		:2006/05/29														 */
/* ========================================================================= */

BOOL CmyString::IsEmpty(void)
{
	BOOL bRet;

	bRet = TRUE;

	if (m_pszString == NULL) {
		goto Exit;
	}
	if (m_nLength <= 0) {
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CmyString::GetLength											 */
/* 内容		:文字列長を取得													 */
/* 日付		:2006/05/30														 */
/* ========================================================================= */

int CmyString::GetLength(void)
{
	return m_nLength;
}


/* ========================================================================= */
/* 関数名	:CmyString::Format												 */
/* 内容		:書式文字列で初期化												 */
/* 日付		:2006/06/02														 */
/* メモ：	バッファは1024バイトなのでそれ以上にはならないように注意		 */
/* ========================================================================= */

void CmyString::Format(LPCSTR lpFormat, ...)
{
	LPSTR pszTmp;

	pszTmp = new char[1024];

	/* 文字列を書式化 */
	va_list argList;
	va_start (argList, lpFormat);
	wvsprintf (pszTmp, lpFormat, argList);
	va_end (argList);

	Renew (pszTmp);
	delete [] pszTmp;
}


/* ========================================================================= */
/* 関数名	:CmyString::operator =											 */
/* 内容		:文字列を初期化													 */
/* 日付		:2006/05/25														 */
/* ========================================================================= */

void CmyString::operator =(CmyString &strSrc)
{
	Renew ((LPCSTR)strSrc);
}


/* ========================================================================= */
/* 関数名	:CmyString::operator =											 */
/* 内容		:文字列を初期化													 */
/* 日付		:2006/05/25														 */
/* ========================================================================= */

void CmyString::operator =(LPCSTR pszSrc)
{
	Renew (pszSrc);
}


/* ========================================================================= */
/* 関数名	:CmyString::operator +=											 */
/* 内容		:文字列を追加													 */
/* 日付		:2006/05/25														 */
/* ========================================================================= */

void CmyString::operator +=(LPCSTR pszSrc)
{
	int nLen;
	LPSTR pszTmp;

	nLen	= 0;
	pszTmp	= NULL;

	if (m_pszString)	{	nLen += (strlen (m_pszString) + 1);	}
	if (pszSrc)			{	nLen += (strlen (pszSrc) + 1);		}

	if (nLen == 0) {
		goto Exit;
	}

	pszTmp = new char[nLen];
	ZeroMemory (pszTmp, nLen);

	if (m_pszString)	{	strcpy (pszTmp, m_pszString);	}
	if (pszSrc)			{	strcat (pszTmp, pszSrc);		}

	Renew (pszTmp);

Exit:
	if (pszTmp) {
		delete [] pszTmp;
	}
}


/* ========================================================================= */
/* 関数名	:CmyString::operator ==											 */
/* 内容		:文字列を比較													 */
/* 日付		:2006/06/01														 */
/* ========================================================================= */

BOOL CmyString::operator ==(LPCSTR pszSrc)
{
	int nResult;
	BOOL bRet;

	bRet = FALSE;

	if (m_pszString == NULL) {
		goto Exit;
	}

	nResult = strcmp (m_pszString, pszSrc);
	if (nResult == 0) {
		bRet = TRUE;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CmyString::operator !=											 */
/* 内容		:文字列を比較													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

BOOL CmyString::operator !=(LPCSTR pszSrc)
{
	int nResult;
	BOOL bRet;

	bRet = FALSE;

	if (m_pszString == NULL) {
		goto Exit;
	}

	nResult = strcmp (m_pszString, pszSrc);
	if (nResult != 0) {
		bRet = TRUE;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CmyString::operator LPCTSTR()									 */
/* 内容		:文字列を初期化													 */
/* 日付		:2006/05/25														 */
/* ========================================================================= */

CmyString::operator LPCTSTR()
{
	return m_pszString;
}


/* ========================================================================= */
/* 関数名	:CmyString::CompareNoCase										 */
/* 内容		:文字列を比較(大文字小文字区別無し)								 */
/* 日付		:2007/02/14														 */
/* ========================================================================= */

int CmyString::CompareNoCase(LPCSTR pszSrc)
{
	int nRet;

	nRet = -1;

	if (m_pszString == NULL) {
		goto Exit;
	}

	nRet = _tcsicmp (m_pszString, pszSrc);
Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CmyString::Renew												 */
/* 内容		:文字列を更新													 */
/* 日付		:2006/05/25														 */
/* ========================================================================= */

void CmyString::Renew(LPCSTR pszSrc)
{
	if (m_pszString == pszSrc) {
		return;
	}
	if (m_pszString) {
		delete [] m_pszString;
		m_pszString = NULL;
	}

	if (pszSrc == NULL) {
		return;
	}

	m_nLength = strlen (pszSrc);
	m_pszString = new char[m_nLength + 1];
	strcpy (m_pszString, pszSrc);
}

/* Copyright(C)URARA-works 2006 */
