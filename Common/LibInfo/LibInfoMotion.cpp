/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoMotion.cpp											 */
/* 内容			:モーション情報ライブラリ基底クラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotion.h"
#include "LibInfoMotion.h"


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::CLibInfoMotion									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

CLibInfoMotion::CLibInfoMotion()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::~CLibInfoMotion								 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

CLibInfoMotion::~CLibInfoMotion()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::Create											 */
/* 内容		:作成															 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::Create(void)
{
	m_paInfo = new ARRAYMOTIONINFO;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::GetNew											 */
/* 内容		:新規データを取得												 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

PCInfoBase CLibInfoMotion::GetNew(void)
{
	PCInfoMotion pInfoMotion;

	pInfoMotion = new CInfoMotion;

	return pInfoMotion;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::GetPtr											 */
/* 内容		:情報を取得														 */
/* 日付		:2007/11/14														 */
/* ========================================================================= */

PCInfoBase CLibInfoMotion::GetPtr(int nNo)
{
	PCInfoBase pRet;

	pRet = NULL;
	if (nNo < 0 || nNo >= GetCount ()) {
		goto Exit;
	}
	pRet = m_paInfo->at(nNo);
Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::Revice											 */
/* 内容		:データの補正													 */
/* 日付		:2008/07/02														 */
/* ========================================================================= */

void CLibInfoMotion::Revice(void)
{
	int i, j, nCount;
	PCInfoMotion pInfoTmp;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = (PCInfoMotion)GetPtr (i);

		if (pInfoTmp->m_anDrawList.size() > 0) {
			continue;
	}
		for (j = 0; j < 4; j ++) {
			pInfoTmp->m_anDrawList.push_back (j);
	}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

int CLibInfoMotion::GetCount(void)
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


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::Add											 */
/* 内容		:追加															 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::Add(PCInfoBase pInfo)
{
	PCInfoMotion pItemInfo;

	pItemInfo = (PCInfoMotion)pInfo;
	if (pItemInfo->m_dwMotionID == 0) {
		pItemInfo->m_dwMotionID = GetNewID ();
	}

	m_paInfo->Add (pItemInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::Add											 */
/* 内容		:追加															 */
/* 日付		:2008/07/15														 */
/* ========================================================================= */

void CLibInfoMotion::Add(PCInfoBase pInfo, int nNo)
{
	PCInfoMotion pItemInfo;

	pItemInfo = (PCInfoMotion)pInfo;
	if (pItemInfo->m_dwMotionID == 0) {
		pItemInfo->m_dwMotionID = GetNewID ();
	}

	m_paInfo->insert (m_paInfo->begin () + nNo, pItemInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::AddType										 */
/* 内容		:指定種別で一覧追加												 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

void CLibInfoMotion::AddType(DWORD dwMotionTypeID)
{
	int i;
	PCInfoMotion pInfoTmp;

	for (i = CHARMOTIONLISTID_NONE + 1; i < CHARMOTIONLISTID_MAX; i ++) {
		pInfoTmp = (PCInfoMotion)GetNew ();
		pInfoTmp->m_dwMotionListID	= i;
		pInfoTmp->m_dwMotionTypeID	= dwMotionTypeID;
		Add (pInfoTmp);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::Delete											 */
/* 内容		:削除															 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoMotion pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE (pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase (m_paInfo->begin () + nNo);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::Delete											 */
/* 内容		:削除															 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::Delete(
	DWORD dwMotionID)		/* [in] モーションID */
{
	int i, nCount, nNo;
	PCInfoMotion pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwMotionID != dwMotionID) {
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
/* 関数名	:CLibInfoMotion::DeleteAll										 */
/* 内容		:全て削除														 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->size();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::SetList										 */
/* 内容		:指定IDのリストを設定											 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

void CLibInfoMotion::SetList(DWORD dwMotionTypeID, DWORD dwMotionListID, CLibInfoMotion *pSrc)
{
	int i, nCount;
	PCInfoMotion pInfo, pInfoTmp;

	/* まずは対象となるリストIDの情報を削除 */
	nCount = m_paInfo->size();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_paInfo->at(i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
		}
	}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
		}
	}
		Delete (i);
	}

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMotion)pSrc->GetPtr (i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
		}
	}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
		}
	}
		pInfoTmp = new CInfoMotion;
		pInfoTmp->Copy (pInfo);
		Add (pInfoTmp);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::IsDirection									 */
/* 内容		:向き毎に設定があるモーションか判定								 */
/* 日付		:2008/01/03														 */
/* ========================================================================= */

BOOL CLibInfoMotion::IsDirection(DWORD dwMotionListID)
{
	BOOL bRet;

	bRet = TRUE;
	switch (dwMotionListID) {
	case CHARMOTIONLISTID_STAND:			/* 立ち */
	case CHARMOTIONLISTID_STAND_SIMPLE:		/* 立ち(上下左右1コマずつ) */
	case CHARMOTIONLISTID_WALK:				/* 歩き(方向無し) */
	case CHARMOTIONLISTID_DESTROY:			/* 破壊 */
	case CHARMOTIONLISTID_SIMPLE:			/* 16コマ連続 */
		bRet = FALSE;
		break;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::GetWaitTime									 */
/* 内容		:全コマの表示時間を取得											 */
/* 日付		:2008/07/24														 */
/* ========================================================================= */

DWORD CLibInfoMotion::GetWaitTime(DWORD dwMotionTypeID, DWORD dwMotionListID)
{
	int i, nCount;
	DWORD dwRet;
	ARRAYMOTIONINFO apMotionInfo;

	dwRet = 0;
	GetMotionInfo (dwMotionTypeID, dwMotionListID, apMotionInfo);

	nCount = apMotionInfo.size();
	for (i = 0; i < nCount; i ++) {
		dwRet += apMotionInfo[i]->m_byWait;
	}
	dwRet *= 10;

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::GetMotionInfo									 */
/* 内容		:指定IDのモーション情報一覧を取得								 */
/* 日付		:2007/11/24														 */
/* ========================================================================= */

void CLibInfoMotion::GetMotionInfo(DWORD dwMotionTypeID, DWORD dwMotionListID, ARRAYMOTIONINFO &aDst)
{
	int i, nCount;
	PCInfoMotion pInfo;

	if (dwMotionListID == 0) {
		return;
	}
	aDst.clear();

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMotion)GetPtr (i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
		}
	}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
		}
	}
		aDst.push_back (pInfo);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

DWORD CLibInfoMotion::GetSendDataSize(DWORD dwMotionTypeID, DWORD dwMotionListID)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotion pInfo;

	dwRet = 0;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
		}
	}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
		}
	}
		dwRet += pInfo->GetSendDataSize ();
	}

	dwRet += sizeof (DWORD);	/* 終端分 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

PBYTE CLibInfoMotion::GetSendData(DWORD dwMotionTypeID, DWORD dwMotionListID)
{
	PBYTE pRet, pDataTmp, pPos;
	DWORD dwSize, dwSizeTmp;
	int i, nCount;
	PCInfoMotion pInfo;

	pRet = NULL;

	dwSize = GetSendDataSize (dwMotionTypeID, dwMotionListID);
	if (dwSize == 0) {
		goto Exit;
	}
	pRet = ZeroNew (dwSize);
	pPos = pRet;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
		}
	}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
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
/* 関数名	:CLibInfoMotion::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

PBYTE CLibInfoMotion::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwMotionID;
	PCInfoMotion pInfoMotion;

	pRet = pSrc;

	pDataTmp = pSrc;
	while (1) {
		CopyMemory (&dwMotionID, pDataTmp, sizeof (dwMotionID));
		if (dwMotionID == 0) {
			pDataTmp += sizeof (DWORD);
			break;
	}
		pInfoMotion = new CInfoMotion;
		pDataTmp = pInfoMotion->SetSendData (pDataTmp);
		Add (pInfoMotion);
	}
	pRet = pDataTmp;

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::GetSendDataSizeType							 */
/* 内容		:指定種別IDの送信データサイズを取得								 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

DWORD CLibInfoMotion::GetSendDataSizeType(DWORD dwMotionTypeID)
{
	return 0;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::GetSendDataType								 */
/* 内容		:指定種別IDの送信データを取得									 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

PBYTE CLibInfoMotion::GetSendDataType(DWORD dwMotionTypeID)
{
	return NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::SetSendDataType								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

PBYTE CLibInfoMotion::SetSendDataType(PBYTE pSrc)
{
	return NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMotion::GetNewID										 */
/* 内容		:新しいモーションIDを取得										 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

DWORD CLibInfoMotion::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotion pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwMotionID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
