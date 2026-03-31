/// @file InfoMotionType.cpp
/// @brief モーション種別情報クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/05/26
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "InfoMotionType.h"

// ヘッダ情報
static LPCSTR s_aszName[] = {
	"m_dwMotionTypeID",	// モーション種別ID
	"m_wGrpIDSub",	// プレビュー用グラフィックIDサブ
	"m_strName",	// モーション種別名
	NULL
};

CInfoMotionType::CInfoMotionType()
{
	m_wGrpIDSub	 = 0;
	m_dwMotionTypeID = 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}

CInfoMotionType::~CInfoMotionType()
{
}

int CInfoMotionType::GetElementNo(LPCSTR pszName)
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

DWORD CInfoMotionType::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof(m_dwMotionTypeID)	+
			sizeof(m_wGrpIDSub)	+
			(m_strName.GetLegacyStoreLength() + 1);

	return dwRet;
}

DWORD CInfoMotionType::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof(m_dwMotionTypeID);	break;	// モーション種別ID
	case 1:	dwRet = sizeof(m_wGrpIDSub);	break;	// プレビュー用グラフィックIDサブ
	case 2:	dwRet = (m_strName.GetLegacyStoreLength() + 1);	break;	// モーション種別名
	}

	return dwRet;
}

LPCSTR CInfoMotionType::GetName(int nNo)
{
	return s_aszName[nNo];
}

PBYTE CInfoMotionType::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = (PBYTE)&m_dwMotionTypeID;	break;	// モーション種別ID
	case 1:	pSrc = (PBYTE)&m_wGrpIDSub;	break;	// プレビュー用グラフィックIDサブ
	case 2:	pSrc = (PBYTE)(LPCSTR)m_strName;	break;	// モーション種別名
	}

	if (pSrc) {
		CopyMemory(pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}

DWORD CInfoMotionType::ReadElementData(
	PBYTE pSrc,	// [in] データの読み込み元
	int nNo)	// [in] 要素番号
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:	pDst = (PBYTE)&m_dwMotionTypeID;	dwSize = sizeof(m_dwMotionTypeID);	break;	// モーション種別ID
	case 1:	pDst = (PBYTE)&m_wGrpIDSub;	dwSize = sizeof(m_wGrpIDSub);	break;	// プレビュー用グラフィックIDサブ
	case 2:	// モーション種別名
		m_strName = (LPCTSTR)LegacyAnsiToTString((LPCSTR)pSrc);
		dwSize = (DWORD)(strlen((LPCSTR)pSrc) + 1);
		break;
	}

	if (pDst) {
		CopyMemory(pDst, pSrc, dwSize);
	}

	return dwSize;
}

DWORD CInfoMotionType::GetSendDataSize(void)
{
	return GetDataSize();
}

PBYTE CInfoMotionType::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);

	pDataTmp = pData;

	CopyMemoryRenew(pDataTmp, &m_dwMotionTypeID,	sizeof(m_dwMotionTypeID),	pDataTmp);	// モーション種別ID
	CopyMemoryRenew(pDataTmp, &m_wGrpIDSub,	sizeof(m_wGrpIDSub),	pDataTmp);	// プレビュー用グラフィックIDサブ
	strcpyRenew((LPSTR)pDataTmp, m_strName, pDataTmp);	// モーション種別名

	return pData;
}

PBYTE CInfoMotionType::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;

	pDataTmp = pSrc;

	CopyMemoryRenew(&m_dwMotionTypeID, pDataTmp, sizeof(m_dwMotionTypeID), pDataTmp);	// モーション種別ID
	CopyMemoryRenew(&m_wGrpIDSub,	pDataTmp, sizeof(m_wGrpIDSub),	 pDataTmp);	// プレビュー用グラフィックIDサブ
	StoreRenew(m_strName, (LPCSTR)pDataTmp, pDataTmp);	// モーション種別名

	return pDataTmp;
}

void CInfoMotionType::Copy(CInfoMotionType *pSrc)
{
	m_dwMotionTypeID	= pSrc->m_dwMotionTypeID;	// モーション種別ID
	m_wGrpIDSub	= pSrc->m_wGrpIDSub;	// プレビュー用グラフィックIDサブ
	m_strName	= pSrc->m_strName;	// モーション種別名
}

