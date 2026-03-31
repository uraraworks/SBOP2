/// @file InfoAnime.cpp
/// @brief アニメーション情報クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/29
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include "InfoAnime.h"

// ヘッダ情報
static LPCSTR s_aszName[] = {
	"byWait",	// 待ち時間(×１０ミリ秒)
	"byLevel",	// 透明度
	"wGrpIDBase",	// グラフィックID
	"wGrpIDPile",	// 重ね合わせ時の背景ID
	NULL
};

CInfoAnime::CInfoAnime()
{
	m_byWait	= 0;
	m_byLevel	= 0;
	m_wGrpIDBase	= 0;
	m_wGrpIDPile	= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}

CInfoAnime::~CInfoAnime()
{
}

int CInfoAnime::GetElementCount(void)
{
	return m_nElementCount;
}

int CInfoAnime::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp(s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}

DWORD CInfoAnime::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof(m_byWait)	+
			sizeof(m_byLevel)	+
			sizeof(m_wGrpIDBase)	+
			sizeof(m_wGrpIDPile);

	return dwRet;
}

DWORD CInfoAnime::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof(m_byWait);	break;
	case 1:	dwRet = sizeof(m_byLevel);	break;
	case 2:	dwRet = sizeof(m_wGrpIDBase);	break;
	case 3:	dwRet = sizeof(m_wGrpIDPile);	break;
	}

	return dwRet;
}

LPCSTR CInfoAnime::GetName(int nNo)
{
	return s_aszName[nNo];
}

PBYTE CInfoAnime::GetWriteData(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet	= NULL;
	pSrc	= NULL;
	dwSize	= GetDataSizeNo(nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:	pSrc = &m_byWait;	break;
	case 1:	pSrc = &m_byLevel;	break;
	case 2:	pSrc = (PBYTE)&m_wGrpIDBase;	break;
	case 3:	pSrc = (PBYTE)&m_wGrpIDPile;	break;
	}

	if (pSrc) {
		CopyMemory(pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}

DWORD CInfoAnime::ReadElementData(
	PBYTE pSrc,	// [in] データの読み込み元
	int nNo)	// [in] 要素番号
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = &m_byWait;	dwSize = sizeof(BYTE);	break;
	case 1:	pDst = &m_byLevel;	dwSize = sizeof(BYTE);	break;
	case 2:	pDst = (PBYTE)&m_wGrpIDBase;	dwSize = sizeof(WORD);	break;
	case 3:	pDst = (PBYTE)&m_wGrpIDPile;	dwSize = sizeof(WORD);	break;
	}

	if (pDst) {
		CopyMemory(pDst, pSrc, dwSize);
	}

	return dwSize;
}

DWORD CInfoAnime::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof(m_byWait)	+
			sizeof(m_byLevel)	+
			sizeof(m_wGrpIDBase)	+
			sizeof(m_wGrpIDPile);

	return dwRet;
}

PBYTE CInfoAnime::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);

	pDataTmp = pData;

	CopyMemoryRenew(pDataTmp, &m_byWait,	sizeof(m_byWait),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_byLevel,	sizeof(m_byLevel),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_wGrpIDBase,	sizeof(m_wGrpIDBase),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_wGrpIDPile,	sizeof(m_wGrpIDPile),	pDataTmp);

	return pData;
}

void CInfoAnime::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;

	pDataTmp = pSrc;

	CopyMemoryRenew(&m_byWait,	pDataTmp, sizeof(m_byWait),	pDataTmp);
	CopyMemoryRenew(&m_byLevel,	pDataTmp, sizeof(m_byLevel),	pDataTmp);
	CopyMemoryRenew(&m_wGrpIDBase,	pDataTmp, sizeof(m_wGrpIDBase),	pDataTmp);
	CopyMemoryRenew(&m_wGrpIDPile,	pDataTmp, sizeof(m_wGrpIDPile),	pDataTmp);
}

