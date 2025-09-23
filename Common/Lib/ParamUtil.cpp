/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:ParamUtil.cpp												 */
/* 内容			:区切り文字付きのパラメータ分解クラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include "ParamUtil.h"


/* ========================================================================= */
/* 関数名	:CParamUtil::CParamUtil											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/02														 */
/* ========================================================================= */

CParamUtil::CParamUtil()
{
}


/* ========================================================================= */
/* 関数名	:CParamUtil::~CParamUtil										 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/05/02														 */
/* ========================================================================= */

CParamUtil::~CParamUtil()
{
}


/* ========================================================================= */
/* 関数名	:CParamUtil::Set												 */
/* 内容		:パラメータ設定													 */
/* 日付		:2007/05/02														 */
/* ========================================================================= */

void CParamUtil::Set(LPCSTR pszText, char cSeparator)
{
	int i, nCount;
	LPSTR pszTextTmp;
	LPCSTR pszTmp, pszStart;
	CmyString strParam;

	m_astrParam.clear();
	if (pszText == NULL) {
		return;
	}

	nCount = strlen (pszText);
	pszStart = pszTmp = pszText;
	pszTextTmp = new char[nCount + 1];

	for (i = 0; i < nCount; i ++) {
		if ((i == nCount - 1) || (pszTmp[i + 1] == cSeparator)) {
			ZeroMemory (pszTextTmp, nCount + 1);
			CopyMemory (pszTextTmp, pszStart, &pszTmp[i] - pszStart + 1);
			pszStart = &pszTmp[i + 2];
			strParam = pszTextTmp;
			m_astrParam.push_back (strParam);
		}
	}
	SAFE_DELETE_ARRAY (pszTextTmp);
}


/* ========================================================================= */
/* 関数名	:CParamUtil::GetCount											 */
/* 内容		:パラメータ数取得												 */
/* 日付		:2007/05/02														 */
/* ========================================================================= */

int CParamUtil::GetCount(void)
{
	return m_astrParam.size();
}


/* ========================================================================= */
/* 関数名	:CParamUtil::GetParam											 */
/* 内容		:パラメータ取得													 */
/* 日付		:2007/05/02														 */
/* ========================================================================= */

LPCSTR CParamUtil::GetParam(int nNo)
{
	int nCount;
	LPCSTR pszRet;

	pszRet = NULL;

	nCount = GetCount ();
	if (nNo >= nCount) {
		goto Exit;
	}

	pszRet = m_astrParam[nNo];
Exit:
	return pszRet;
}

/* Copyright(C)URARA-works 2007 */
