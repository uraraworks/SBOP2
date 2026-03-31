/// @file LibInfoItemWeapon.cpp
/// @brief アイテム武器情報ライブラリクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/10
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "LibInfoItemWeapon.h"

CLibInfoItemWeapon::CLibInfoItemWeapon()
{
	m_dwNewIDTmp	= 0;
	m_paInfo	= NULL;
}

CLibInfoItemWeapon::~CLibInfoItemWeapon()
{
	Destroy();
}

void CLibInfoItemWeapon::Create(void)
{
	m_paInfo = new ARRAYWEAPONINFO;
}

void CLibInfoItemWeapon::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

PCInfoBase CLibInfoItemWeapon::GetNew(void)
{
	PCInfoItemWeapon pTmp;

	pTmp = new CInfoItemWeapon;

	return (PCInfoBase)pTmp;
}

int CLibInfoItemWeapon::GetCount(void)
{
	int nRet;

	nRet = 0;

	if (m_paInfo == NULL) {
		goto Exit;
	}

	nRet = m_paInfo->size();
Exit:
	return nRet;
}

void CLibInfoItemWeapon::Add(PCInfoBase pInfo)
{
	PCInfoItemWeapon pItemInfo;

	pItemInfo = (PCInfoItemWeapon)pInfo;
	if (pItemInfo->m_dwWeaponInfoID == 0) {
		pItemInfo->m_dwWeaponInfoID = GetNewID();
	}

	m_paInfo->Add(pItemInfo);
}

void CLibInfoItemWeapon::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoItemWeapon pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoItemWeapon::Delete(
	DWORD dwWeaponInfoID)	// [in] 武器情報ID
{
	int i, nCount, nNo;
	PCInfoItemWeapon pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwWeaponInfoID != dwWeaponInfoID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoItemWeapon::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->size();
	for (i = nCount - 1; i >= 0; i --) {
		Delete(i);
	}
}

PCInfoBase CLibInfoItemWeapon::GetPtr(int nNo)
{
	return m_paInfo->at(nNo);
}

PCInfoBase CLibInfoItemWeapon::GetPtr(
	DWORD dwWeaponInfoID)	// [in] 武器情報ID
{
	int i, nCount;
	PCInfoItemWeapon pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwWeaponInfoID != dwWeaponInfoID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

DWORD CLibInfoItemWeapon::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoItemWeapon pItem;

	dwRet = dwSize = 0;

	// データ数分のサイズ
	dwSize += sizeof(DWORD);

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItemWeapon)GetPtr(i);

		dwSize += pItem->GetSendDataSize();
	}

	dwRet = dwSize;
	return dwRet;
}

PBYTE CLibInfoItemWeapon::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoItemWeapon pItem;

	pRet = NULL;

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);
	dwOffset	= 0;

	// データ数を書き込み
	dwCount = (DWORD)GetCount();
	CopyMemory(pData, &dwCount, sizeof(dwCount));
	dwOffset += sizeof(dwCount);

	// キャラ情報を書き込み
	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItemWeapon)GetPtr(i);

		dwSizeTmp	= pItem->GetSendDataSize();
		pDataTmp	= pItem->GetSendData();
		CopyMemory(&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY(pDataTmp);
	}

	pRet = pData;
	return pRet;
}

PBYTE CLibInfoItemWeapon::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	CInfoItemWeapon *pItem;

	pRet	= pSrc;
	pDataTmp	= pSrc;

	DeleteAll();

	// データ数を読み込み
	CopyMemoryRenew(&dwCount, pDataTmp, sizeof(dwCount), pDataTmp);
	nCount = (int)dwCount;

	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItemWeapon)GetNew();

		pDataTmp = pItem->SetSendData(pDataTmp);
		Add(pItem);
	}

	pRet = pDataTmp;
	return pRet;
}

PBYTE CLibInfoItemWeapon::GetNewSendData(PBYTE pSrc, PCInfoItemWeapon &pDst)
{
	PBYTE pRet;
	CInfoItemWeapon InfoTmp;

	SAFE_DELETE(pDst);

	// まずは基底クラスへ取り込み
	InfoTmp.SetSendData(pSrc);
	pDst = (PCInfoItemWeapon)GetNew();
	pRet = pDst->SetSendData(pSrc);

	return pRet;
}

DWORD CLibInfoItemWeapon::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoItemWeapon pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwWeaponInfoID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

