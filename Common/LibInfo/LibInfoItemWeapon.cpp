/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoItemWeapon.cpp										 */
/* 内容			:アイテム武器情報ライブラリクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/10													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoItemWeapon.h"


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::CLibInfoItemWeapon							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

CLibInfoItemWeapon::CLibInfoItemWeapon()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::~CLibInfoItemWeapon						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

CLibInfoItemWeapon::~CLibInfoItemWeapon()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::Create										 */
/* 内容		:作成															 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::Create(void)
{
	m_paInfo = new ARRAYWEAPONINFO;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::Destroy									 */
/* 内容		:破棄															 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemWeapon::GetNew(void)
{
	PCInfoItemWeapon pTmp;

	pTmp = new CInfoItemWeapon;

	return (PCInfoBase)pTmp;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::GetCount									 */
/* 内容		:データ数を取得													 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

int CLibInfoItemWeapon::GetCount(void)
{
	int nRet;

	nRet = 0;

	if (m_paInfo == NULL) {
		goto Exit;
	}

	nRet = m_paInfo->GetSize ();
Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::Add										 */
/* 内容		:追加															 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::Add(PCInfoBase pInfo)
{
	PCInfoItemWeapon pItemInfo;

	pItemInfo = (PCInfoItemWeapon)pInfo;
	if (pItemInfo->m_dwWeaponInfoID == 0) {
		pItemInfo->m_dwWeaponInfoID = GetNewID ();
	}

	m_paInfo->Add (pItemInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::Delete										 */
/* 内容		:削除															 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoItemWeapon pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::Delete										 */
/* 内容		:削除															 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::Delete(
	DWORD dwWeaponInfoID)		/* [in] 武器情報ID */
{
	int i, nCount, nNo;
	PCInfoItemWeapon pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwWeaponInfoID != dwWeaponInfoID) {
			continue;
		}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete (nNo);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::DeleteAll									 */
/* 内容		:全て削除														 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::GetPtr										 */
/* 内容		:アイテム情報を取得												 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemWeapon::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::GetPtr										 */
/* 内容		:アイテム情報を取得												 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemWeapon::GetPtr(
	DWORD dwWeaponInfoID)		/* [in] 武器情報ID */
{
	int i, nCount;
	PCInfoItemWeapon pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwWeaponInfoID != dwWeaponInfoID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

DWORD CLibInfoItemWeapon::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoItemWeapon pItem;

	dwRet = dwSize = 0;

	/* データ数分のサイズ */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItemWeapon)GetPtr (i);

		dwSize += pItem->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

PBYTE CLibInfoItemWeapon::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoItemWeapon pItem;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* データ数を書き込み */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* キャラ情報を書き込み */
	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItemWeapon)GetPtr (i);

		dwSizeTmp	= pItem->GetSendDataSize ();
		pDataTmp	= pItem->GetSendData ();
		CopyMemory (&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY (pDataTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/08/10														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CLibInfoItemWeapon::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	CInfoItemWeapon *pItem;

	pRet		= pSrc;
	pDataTmp	= pSrc;

	DeleteAll ();

	/* データ数を読み込み */
	CopyMemoryRenew (&dwCount, pDataTmp, sizeof (dwCount), pDataTmp);
	nCount = (int)dwCount;

	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItemWeapon)GetNew ();

		pDataTmp = pItem->SetSendData (pDataTmp);
		Add (pItem);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::GetNewSendData								 */
/* 内容		:送信データから新規データを取得									 */
/* 日付		:2008/08/11														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CLibInfoItemWeapon::GetNewSendData(PBYTE pSrc, PCInfoItemWeapon &pDst)
{
	PBYTE pRet;
	CInfoItemWeapon InfoTmp;

	SAFE_DELETE (pDst);

	/* まずは基底クラスへ取り込み */
	InfoTmp.SetSendData (pSrc);
	pDst = (PCInfoItemWeapon)GetNew ();
	pRet = pDst->SetSendData (pSrc);

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemWeapon::GetNewID									 */
/* 内容		:新しいアイテム種別IDを取得										 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

DWORD CLibInfoItemWeapon::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoItemWeapon pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwWeaponInfoID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
