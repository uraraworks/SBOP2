/// @file InfoMapEventINITSTATUS.cpp
/// @brief イベント情報(ステータス初期化)クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/05
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "InfoMapEventINITSTATUS.h"

// ヘッダ情報
static LPCSTR s_aszName[] = {
	"m_dwEffectID",	// エフェクトID
	NULL
};

CInfoMapEventINITSTATUS::CInfoMapEventINITSTATUS()
{
	m_nType = MAPEVENTTYPE_INITSTATUS;

	m_dwEffectID = 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}

CInfoMapEventINITSTATUS::~CInfoMapEventINITSTATUS()
{
}

int CInfoMapEventINITSTATUS::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = CInfoMapEventBase::GetElementNo(pszName);
	if (nRet >= 0) {
		goto Exit;
	}

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp(s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}
	if (nRet >= 0) {
		nRet += m_nElementCountBase;
	}

Exit:
	return nRet;
}

DWORD CInfoMapEventINITSTATUS::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize();
	dwRet += sizeof(m_dwEffectID);

	return dwRet;
}

DWORD CInfoMapEventINITSTATUS::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo(nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof(m_dwEffectID);	break;
		}
	}

	return dwRet;
}

LPCSTR CInfoMapEventINITSTATUS::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName(nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}

PBYTE CInfoMapEventINITSTATUS::GetWriteData(int nNo, PDWORD pdwSize)
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
	if (nNo < m_nElementCountBase) {
		pRet = CInfoMapEventBase::GetWriteData(nNo, pdwSize);
	} else {
		pRet = new BYTE[dwSize];

		switch (nNo - m_nElementCountBase) {
		case 0:	pSrc = (PBYTE)&m_dwEffectID;	break;
		}

		if (pSrc) {
			CopyMemory(pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}

DWORD CInfoMapEventINITSTATUS::ReadElementData(
	PBYTE pSrc,	// [in] データの読み込み元
	int nNo)	// [in] 要素番号
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	if (nNo < m_nElementCountBase) {
		dwSize = CInfoMapEventBase::ReadElementData(pSrc, nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0: pDst = (PBYTE)&m_dwEffectID;	dwSize = sizeof(m_dwEffectID);	break;
		}

		if (pDst) {
			CopyMemory(pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}

DWORD CInfoMapEventINITSTATUS::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize();
	dwRet += sizeof(m_dwEffectID);

	return dwRet;
}

PBYTE CInfoMapEventINITSTATUS::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize();

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);

	pDataTmp = pData;

	CopyMemoryRenew(pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew(pDataTmp, &m_dwEffectID, sizeof(m_dwEffectID), pDataTmp);	// エフェクトID

	SAFE_DELETE_ARRAY(pDataBase);

	return pData;
}

PBYTE CInfoMapEventINITSTATUS::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData(pSrc);

	CopyMemoryRenew(&m_dwEffectID, pDataTmp, sizeof(m_dwEffectID), pDataTmp);	// エフェクトID

	pRet = pDataTmp;
	return pRet;
}

void CInfoMapEventINITSTATUS::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventINITSTATUS pSrcTmp = (PCInfoMapEventINITSTATUS)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy(pSrc);

	m_dwEffectID = pSrcTmp->m_dwEffectID;
}

