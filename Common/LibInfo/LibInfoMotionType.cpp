/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoMotionType.cpp										 */
/* 内容			:モーション情報ライブラリ基底クラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMotionType.h"


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::CLibInfoMotionType							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

CLibInfoMotionType::CLibInfoMotionType()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::~CLibInfoMotionType						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

CLibInfoMotionType::~CLibInfoMotionType()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::Create										 */
/* 内容		:作成															 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::Create(void)
{
	m_paInfo = new ARRAYMOTIONTYPEINFO;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::Destroy									 */
/* 内容		:破棄															 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

PCInfoBase CLibInfoMotionType::GetNew(void)
{
	PCInfoMotionType pInfoMotion;

	pInfoMotion = new CInfoMotionType;

	return pInfoMotion;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

PCInfoBase CLibInfoMotionType::GetPtr(int nNo)
{
	PCInfoBase pRet;

	pRet = NULL;
	if (nNo < 0 || nNo >= GetCount ()) {
		goto Exit;
	}
	pRet = m_paInfo->GetAt (nNo);
Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2008/05/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoMotionType::GetPtr(DWORD dwMotionTypeID)
{
	int i, nCount;
	PCInfoMotionType pRet, pInfo;

	pRet = NULL;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
			continue;
		}
		pRet = pInfo;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::GetCount									 */
/* 内容		:データ数を取得													 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

int CLibInfoMotionType::GetCount(void)
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
/* 関数名	:CLibInfoMotionType::Add										 */
/* 内容		:追加															 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::Add(PCInfoBase pInfo)
{
	PCInfoMotionType pMotionTypeInfo;

	pMotionTypeInfo = (PCInfoMotionType)pInfo;
	if (pMotionTypeInfo->m_dwMotionTypeID == 0) {
		pMotionTypeInfo->m_dwMotionTypeID = GetNewID ();
	}

	m_paInfo->Add (pMotionTypeInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::Delete										 */
/* 内容		:削除															 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoMotionType pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::Delete										 */
/* 内容		:削除															 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::Delete(
	DWORD dwMotionTypeID)		/* [in] モーション種別ID */
{
	int i, nCount, nNo;
	PCInfoMotionType pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMotionTypeID != dwMotionTypeID) {
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
/* 関数名	:CLibInfoMotionType::DeleteAll									 */
/* 内容		:全て削除														 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::DeleteAll(void)
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
/* 関数名	:CLibInfoMotionType::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

DWORD CLibInfoMotionType::GetSendDataSize(DWORD dwMotionTypeID)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotionType pInfo;

	dwRet = 0;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
			}
		}
		dwRet += pInfo->GetSendDataSize ();
	}

	dwRet += sizeof (DWORD);	/* 終端分 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

PBYTE CLibInfoMotionType::GetSendData(DWORD dwMotionTypeID)
{
	PBYTE pRet, pDataTmp, pPos;
	DWORD dwSize, dwSizeTmp;
	int i, nCount;
	PCInfoMotionType pInfo;

	pRet = NULL;

	dwSize = GetSendDataSize (dwMotionTypeID);
	if (dwSize == 0) {
		goto Exit;
	}
	pRet = ZeroNew (dwSize);
	pPos = pRet;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
			}
		}
		dwSizeTmp	= pInfo->GetSendDataSize ();
		pDataTmp	= pInfo->GetSendData ();
		CopyMemoryRenew (pPos, pDataTmp, dwSizeTmp, pPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

PBYTE CLibInfoMotionType::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwMotionTypeID;
	PCInfoMotionType pInfoMotion;

	pRet = pSrc;

	pDataTmp = pSrc;
	while (1) {
		CopyMemory (&dwMotionTypeID, pDataTmp, sizeof (dwMotionTypeID));
		if (dwMotionTypeID == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoMotion = (PCInfoMotionType)GetPtr (dwMotionTypeID);
		if (pInfoMotion == NULL) {
			pInfoMotion = new CInfoMotionType;
			pDataTmp = pInfoMotion->SetSendData (pDataTmp);
			Add (pInfoMotion);
		} else {
			pDataTmp = pInfoMotion->SetSendData (pDataTmp);
		}
	}
	pRet = pDataTmp;

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotionType::GetNewID									 */
/* 内容		:新しいモーションIDを取得										 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

DWORD CLibInfoMotionType::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotionType pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMotionTypeID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
