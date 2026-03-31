/// @file ParamUtil.cpp
/// @brief 区切り文字付きのパラメータ分解クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/02
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "ParamUtil.h"

CParamUtil::CParamUtil()
{
}

CParamUtil::~CParamUtil()
{
}

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

	nCount = strlen(pszText);
	pszStart = pszTmp = pszText;
	pszTextTmp = new char[nCount + 1];

	for (i = 0; i < nCount; i ++) {
		if ((i == nCount - 1) || (pszTmp[i + 1] == cSeparator)) {
			ZeroMemory(pszTextTmp, nCount + 1);
			CopyMemory(pszTextTmp, pszStart, &pszTmp[i] - pszStart + 1);
			pszStart = &pszTmp[i + 2];
			strParam = pszTextTmp;
			m_astrParam.push_back(strParam);
		}
	}
	SAFE_DELETE_ARRAY(pszTextTmp);
}

int CParamUtil::GetCount(void)
{
	return m_astrParam.size();
}

LPCSTR CParamUtil::GetParam(int nNo)
{
	int nCount;
	LPCSTR pszRet;

	pszRet = NULL;

	nCount = GetCount();
	if (nNo >= nCount) {
		goto Exit;
	}

	pszRet = m_astrParam[nNo];
Exit:
	return pszRet;
}

