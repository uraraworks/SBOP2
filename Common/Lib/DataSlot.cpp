/// @file DataSlot.cpp
/// @brief データスロットクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/07
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "DataSlot.h"

CDataSlot::CDataSlot()
{
	m_dwNewID = 1;
}

CDataSlot::~CDataSlot()
{
	Destroy();
}

void CDataSlot::Destroy(void)
{
	int i, nCount;

	m_CritData.Enter();

	// 確保されているデータを全て消去

	// データスロット
	nCount = m_DataSlot.size();
	for (i = 0; i < nCount; i ++) {
		PDATASLOTINFO pInfo;

		pInfo = &m_DataSlot[i];
		SAFE_DELETE_ARRAY(pInfo->pData);
	}
	m_DataSlot.clear();

	m_CritData.Leave();
}

DWORD CDataSlot::Add(
	DWORD dwSize,	// [in] データサイズ
	LPBYTE pSrc)	// [in] 追加するデータ
{
	int nNo;
	DWORD dwRet;
	LPBYTE pData;
	DATASLOTINFO Info;

	m_CritData.Enter();

	pData = new BYTE[dwSize];
	CopyMemory(pData, pSrc, dwSize);

	dwRet = (DWORD)pData;
	Info.dwID	= m_dwNewID;	// データID
	Info.dwSize	= dwSize;	// データサイズ
	Info.pData	= pData;	// データバッファ
	m_DataSlot.push_back(Info);

	while (1) {
		m_dwNewID ++;
		if (m_dwNewID == 0) {
			continue;
		}
		nNo = GetID(m_dwNewID);
		if (nNo == -1) {
			break;
		}
	}

	m_CritData.Leave();

	return Info.dwID;
}

DWORD CDataSlot::AddDelete(
	DWORD dwSize,	// [in] データサイズ
	LPBYTE *pSrc)	// [in] 追加するデータ
{
	DWORD dwRet;

	dwRet = Add(dwSize, *pSrc);
	SAFE_DELETE_ARRAY(*pSrc);

	return dwRet;
}

DWORD CDataSlot::GetSize(
	DWORD dwID)	// [in] データID
{
	int nID;
	DWORD dwRet;
	PDATASLOTINFO pInfo;

	dwRet = 0;
	m_CritData.Enter();

	nID = GetID(dwID);
	// データが見つからなかった？
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	dwRet = pInfo->dwSize;

Exit:
	m_CritData.Leave();

	return dwRet;
}

BOOL CDataSlot::Copy(
	DWORD dwID,	// [in] データID
	LPBYTE pDst,	// [out] 格納先のポインタ
	BOOL bDelete)	// [in] 取得後削除するかどうか
{
	int nID;
	BOOL bRet;
	PDATASLOTINFO pInfo;

	bRet = FALSE;

	m_CritData.Enter();

	nID = GetID(dwID);
	// データが見つからなかった？
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	CopyMemory(pDst, pInfo->pData, pInfo->dwSize);

	// 削除指定？
	if (bDelete) {
		SAFE_DELETE_ARRAY(pInfo->pData);
		m_DataSlot.erase(m_DataSlot.begin() + nID);
	}

	bRet = TRUE;

Exit:
	m_CritData.Leave();

	return bRet;
}

LPBYTE CDataSlot::GetPtr(
	DWORD dwID)	// [in] データID
{
	int nID;
	LPBYTE pRet;
	PDATASLOTINFO pInfo;

	pRet = NULL;

	m_CritData.Enter();

	if (dwID == 0) {
		goto Exit;
	}

	nID = GetID(dwID);
	// データが見つからなかった？
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	pRet = pInfo->pData;

Exit:
	m_CritData.Leave();

	return pRet;
}

void CDataSlot::Delete(
	DWORD dwID)	// [in] データID
{
	int nID;
	PDATASLOTINFO pInfo;

	m_CritData.Enter();

	nID = GetID(dwID);
	// データが見つからなかった？
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	SAFE_DELETE_ARRAY(pInfo->pData);
	m_DataSlot.erase(m_DataSlot.begin() + nID);

Exit:
	m_CritData.Leave();
}

void CDataSlot::Lock(void)
{
	m_CritDataLock.Enter();
}

void CDataSlot::UnLock(void)
{
	m_CritDataLock.Leave();
}

int CDataSlot::GetID(
	DWORD dwID)	// [in] データID
{
	int i, nCount, nRet;
	PDATASLOTINFO pInfo;

	nRet = -1;
	nCount = m_DataSlot.size();

	for (i = 0; i < nCount; i ++) {
		pInfo = &m_DataSlot[i];
		if (pInfo->dwID == dwID) {
			nRet = i;
			break;
		}
	}

	return nRet;
}

