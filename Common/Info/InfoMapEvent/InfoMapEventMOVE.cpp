/// @file InfoMapEventMOVE.cpp
/// @brief イベント情報(マップ内移動)クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/05
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoMapEventMOVE.h"

// ヘッダ情報
static LPCSTR s_aszName[] = {
	"ptDst",	// 移動先
	"m_nDirection",	// 移動後の向き
	NULL
};

CInfoMapEventMOVE::CInfoMapEventMOVE()
{
	m_nType	= MAPEVENTTYPE_MOVE;
	m_nDirection	= -1;
	ZeroMemory(&m_ptDst, sizeof(m_ptDst));

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}

CInfoMapEventMOVE::~CInfoMapEventMOVE()
{
}

void CInfoMapEventMOVE::RenewSize(int nDirection, int nSize, SIZE *pSize)
{
	CInfoMapEventBase::RenewSize(nDirection, nSize, pSize);

	switch (nDirection) {
	case 0:
		m_ptDst.y += nSize;
		break;
	case 2:
		m_ptDst.x += nSize;
		break;
	}

	// 座標の調整
	m_ptDst.x = max(0, m_ptDst.x);
	m_ptDst.x = min(m_ptDst.x, pSize->cx - 1);
	m_ptDst.y = max(0, m_ptDst.y);
	m_ptDst.y = min(m_ptDst.y, pSize->cy - 1);
}

int CInfoMapEventMOVE::GetElementNo(LPCSTR pszName)
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

DWORD CInfoMapEventMOVE::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize();
	dwRet += sizeof(m_ptDst);
	dwRet += sizeof(m_nDirection);

	return dwRet;
}

DWORD CInfoMapEventMOVE::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo(nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof(m_ptDst);	break;	// 移動先
		case 1:	dwRet = sizeof(m_nDirection);	break;	// 移動後の向き
		}
	}

	return dwRet;
}

LPCSTR CInfoMapEventMOVE::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName(nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}

PBYTE CInfoMapEventMOVE::GetWriteData(int nNo, PDWORD pdwSize)
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
		case 0:	pSrc = (PBYTE)&m_ptDst;	break;	// 移動先
		case 1:	pSrc = (PBYTE)&m_nDirection;	break;	// 移動後の向き
		}

		if (pSrc) {
			CopyMemory(pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}

DWORD CInfoMapEventMOVE::ReadElementData(
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
		case 0: pDst = (PBYTE)&m_ptDst;	dwSize = sizeof(m_ptDst);	break;	// 移動先
		case 1: pDst = (PBYTE)&m_nDirection;	dwSize = sizeof(m_nDirection);	break;	// 移動後の向き
		}

		if (pDst) {
			CopyMemory(pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}

DWORD CInfoMapEventMOVE::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize();
	dwRet += sizeof(m_ptDst);	// 移動先
	dwRet += sizeof(m_nDirection);	// 移動後の向き

	return dwRet;
}

PBYTE CInfoMapEventMOVE::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize();

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);

	pDataTmp = pData;

	CopyMemoryRenew(pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew(pDataTmp, &m_ptDst,	sizeof(m_ptDst),	pDataTmp);	// 移動先
	CopyMemoryRenew(pDataTmp, &m_nDirection,	sizeof(m_nDirection),	pDataTmp);	// 移動後の向き

	SAFE_DELETE_ARRAY(pDataBase);

	return pData;
}

PBYTE CInfoMapEventMOVE::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData(pSrc);

	CopyMemoryRenew(&m_ptDst,	pDataTmp, sizeof(m_ptDst),	pDataTmp);	// 移動先
	CopyMemoryRenew(&m_nDirection,	pDataTmp, sizeof(m_nDirection),	pDataTmp);	// 移動後の向き

	pRet = pDataTmp;
	return pRet;
}

void CInfoMapEventMOVE::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventMOVE pSrcTmp = (PCInfoMapEventMOVE)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy(pSrc);

	m_ptDst	= pSrcTmp->m_ptDst;	// 移動先
	m_nDirection	= pSrcTmp->m_nDirection;	// 移動後の向き
}

