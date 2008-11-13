/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LibInfoMapBase.cpp											 */
/* 内容			:マップ情報基底クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/15													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapBase.h"


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::CLibInfoMapBase								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

CLibInfoMapBase::CLibInfoMapBase()
{
	m_paInfo = NULL;
	m_pLibInfoMapParts = NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::~CLibInfoMapBase								 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

CLibInfoMapBase::~CLibInfoMapBase()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::Create										 */
/* 内容		:作成															 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::Create(CLibInfoMapParts *pLibInfoMapParts)
{
	m_paInfo = new ARRAYINFOMAPBASE;

	m_pLibInfoMapParts = pLibInfoMapParts;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapBase::GetNew(void)
{
	PCInfoMapBase pInfoMapBase;

	pInfoMapBase = new CInfoMapBase;
	pInfoMapBase->Create (m_pLibInfoMapParts);

	return pInfoMapBase;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::RenewMapEvent									 */
/* 内容		:マップイベント更新												 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

void CLibInfoMapBase::RenewMapEvent(void)
{
	int i, nCount;
	PCInfoMapBase pInfoMapBase;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapBase = (PCInfoMapBase)GetPtr (i);
		pInfoMapBase->RenewMapEvent ();
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::RenewHitTmp									 */
/* 内容		:マップパーツ以外での当たり判定を更新							 */
/* 日付		:2008/11/13														 */
/* ========================================================================= */

void CLibInfoMapBase::RenewHitTmp(void)
{
	int i, nCount;
	PCInfoMapBase pInfoMapBase;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapBase = (PCInfoMapBase)GetPtr (i);
		pInfoMapBase->RenewHitTmp ();
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::SetMapObject									 */
/* 内容		:マップオブジェクト情報を設定									 */
/* 日付		:2008/11/13														 */
/* ========================================================================= */

void CLibInfoMapBase::SetMapObject(CLibInfoMapObject *pLibInfo)
{
	int i, nCount;
	PCInfoMapBase pInfoMapBase;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapBase = (PCInfoMapBase)GetPtr (i);
		pInfoMapBase->m_pLibInfoMapObject = pLibInfo;
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

int CLibInfoMapBase::GetCount(void)
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
/* 関数名	:CLibInfoMapBase::Add											 */
/* 内容		:追加															 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::Add(PCInfoBase pInfo)
{
	PCInfoMapBase pMapInfo;

	pMapInfo = (PCInfoMapBase)pInfo;
	if (pMapInfo->m_dwMapID == 0) {
		pMapInfo->m_dwMapID = GetNewID ();
	}

	m_paInfo->Add (pMapInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::Delete										 */
/* 内容		:削除															 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoMapBase pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::Delete										 */
/* 内容		:削除															 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::Delete(
	DWORD dwMapID)		/* [in] マップID */
{
	int i, nCount, nNo;
	PCInfoMapBase pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapID != dwMapID) {
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
/* 関数名	:CLibInfoMapBase::DeleteAll										 */
/* 内容		:全て削除														 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::DeleteAll(void)
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
/* 関数名	:CLibInfoMapBase::DeleteParts									 */
/* 内容		:指定パーツを削除												 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

void CLibInfoMapBase::DeleteParts(DWORD dwPartsID)
{
	int i, nCount;
	PCInfoMapBase pInfoTmp;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		pInfoTmp->DeleteParts (dwPartsID);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::DeleteShadow									 */
/* 内容		:指定マップ影を削除												 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

void CLibInfoMapBase::DeleteShadow(DWORD dwShadowID)
{
	int i, nCount;
	PCInfoMapBase pInfoTmp;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		pInfoTmp->DeleteShadow (dwShadowID);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::GetPtr										 */
/* 内容		:マップ情報を取得												 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapBase::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::GetPtr										 */
/* 内容		:マップ情報を取得												 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapBase::GetPtr(
	DWORD dwMapID)		/* [in] マップID */
{
	int i, nCount;
	PCInfoMapBase pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapID != dwMapID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapBase::GetNewID										 */
/* 内容		:新しいマップIDを取得											 */
/* 日付		:2006/10/15														 */
/* ========================================================================= */

DWORD CLibInfoMapBase::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapBase pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2006 */
