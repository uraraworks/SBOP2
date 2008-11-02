/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoMapObject.cpp										 */
/* 内容			:マップパーツ情報ライブラリクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapObject.h"


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::CLibInfoMapObject							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

CLibInfoMapObject::CLibInfoMapObject()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::~CLibInfoMapObject							 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

CLibInfoMapObject::~CLibInfoMapObject()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::Create										 */
/* 内容		:作成															 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Create(void)
{
	m_paInfo = new ARRAYMAPOBJECT;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObject::GetNew(void)
{
	return new CInfoMapObject;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::GetCount									 */
/* 内容		:データ数を取得													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

int CLibInfoMapObject::GetCount(void)
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
/* 関数名	:CLibInfoMapObject::Add											 */
/* 内容		:追加															 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Add(PCInfoBase pInfo)
{
	PCInfoMapObject pInfoMapObject;

	pInfoMapObject = (PCInfoMapObject)pInfo;
	if (pInfoMapObject->m_dwObjectID == 0) {
		pInfoMapObject->m_dwObjectID = GetNewID ();
	}

	m_paInfo->Add (pInfoMapObject);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::Delete										 */
/* 内容		:削除															 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoMapObject pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::Delete										 */
/* 内容		:削除															 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Delete(
	DWORD dwObjectID)		/* [in] マップオブジェクトID */
{
	int i, nCount, nNo;
	PCInfoMapObject pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwObjectID != dwObjectID) {
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
/* 関数名	:CLibInfoMapObject::DeleteAll									 */
/* 内容		:全て削除														 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::DeleteAll(void)
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
/* 関数名	:CLibInfoMapObject::Merge										 */
/* 内容		:取り込み														 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Merge(CLibInfoMapObject *pSrc)
{
	int i, nCount;
	PCInfoMapObject pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapObject)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoMapObject)GetPtr (pInfoSrc->m_dwObjectID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapObject)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		}
		pInfoTmp->Copy (pInfoSrc);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObject::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObject::GetPtr(
	DWORD dwObjectID)		/* [in] マップオブジェクトID */
{
	int i, nCount;
	PCInfoMapObject pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwObjectID != dwObjectID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

DWORD CLibInfoMapObject::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoMapObject pInfoMapObject;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapObject = (PCInfoMapObject)GetPtr (i);
		dwSize += pInfoMapObject->GetSendDataSize ();
	}
	/* 終端用 */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

PBYTE CLibInfoMapObject::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapObject pInfoMapObject;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapObject = (PCInfoMapObject)GetPtr (i);

		dwSizeTmp	= pInfoMapObject->GetSendDataSize ();
		pDataTmp	= pInfoMapObject->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

PBYTE CLibInfoMapObject::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoMapObject pInfoMapObject, pInfoMapObjectTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoMapObjectTmp = (PCInfoMapObject)GetNew ();
		pDataTmp = pInfoMapObjectTmp->SetSendData (pDataTmp);

		pInfoMapObject = (PCInfoMapObject)GetPtr (pInfoMapObjectTmp->m_dwObjectID);
		if (pInfoMapObject) {
			pInfoMapObject->Copy (pInfoMapObjectTmp);
			SAFE_DELETE (pInfoMapObjectTmp);
		} else {
			Add (pInfoMapObjectTmp);
		}
	}

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObject::GetNewID									 */
/* 内容		:新しいIDを取得													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

DWORD CLibInfoMapObject::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapObject pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwObjectID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
