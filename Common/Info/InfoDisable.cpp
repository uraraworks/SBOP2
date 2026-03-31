/// @file InfoDisable.cpp
/// @brief 拒否情報クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/08
/// @copyright Copyright(C)URARA-works 2009

#include "stdafx.h"
#include "InfoDisable.h"

// ヘッダ情報
static LPCSTR s_aszName[] = {
	"m_dwDisableID",	// 拒否情報ID
	"m_strMacAddress",	// MACアドレス
	NULL
};

CInfoDisable::CInfoDisable()
{
	m_dwDisableID = 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}

CInfoDisable::~CInfoDisable()
{
}

int CInfoDisable::GetElementNo(LPCSTR pszName)
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

DWORD CInfoDisable::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof(m_dwDisableID);	// 拒否情報ID
	dwRet += (m_strMacAddress.GetLegacyStoreLength() + 1);	// MACアドレス

	return dwRet;
}

DWORD CInfoDisable::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof(m_dwDisableID);	break;	// 拒否情報ID
	case 1:	dwRet = (m_strMacAddress.GetLegacyStoreLength() + 1);	break;	// MACアドレス
	}

	return dwRet;
}

LPCSTR CInfoDisable::GetName(int nNo)
{
	return s_aszName[nNo];
}

PBYTE CInfoDisable::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = (PBYTE)&m_dwDisableID;	break;	// 拒否情報ID
	case 1:	pSrc = (PBYTE)(LPCSTR)m_strMacAddress;	break;	// MACアドレス
	}

	if (pSrc) {
		CopyMemory(pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}

DWORD CInfoDisable::ReadElementData(
	PBYTE pSrc,	// [in] データの読み込み元
	int nNo)	// [in] 要素番号
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:	pDst = (PBYTE)&m_dwDisableID;	dwSize = sizeof(m_dwDisableID);	break;	// 拒否情報ID
	case 1:	// MACアドレス
		m_strMacAddress = (LPCTSTR)LegacyAnsiToTString((LPCSTR)pSrc);
		dwSize = (DWORD)(strlen((LPCSTR)pSrc) + 1);
		break;
	}

	if (pDst) {
		CopyMemory(pDst, pSrc, dwSize);
	}

	return dwSize;
}

DWORD CInfoDisable::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof(m_dwDisableID)	+
			(m_strMacAddress.GetLegacyStoreLength() + 1);

	return dwRet;
}

PBYTE CInfoDisable::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);

	pDataTmp = pData;

	CopyMemoryRenew(pDataTmp, &m_dwDisableID, sizeof(m_dwDisableID), pDataTmp);
	strcpyRenew((LPSTR)pDataTmp, m_strMacAddress, pDataTmp);

	return pData;
}

PBYTE CInfoDisable::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew(&m_dwDisableID, pDataTmp, sizeof(m_dwDisableID), pDataTmp);
	StoreRenew(m_strMacAddress, (LPCSTR)pDataTmp, pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

void CInfoDisable::Copy(CInfoDisable *pSrc)
{
	m_dwDisableID	= pSrc->m_dwDisableID;
	m_strMacAddress	= pSrc->m_strMacAddress;
}

