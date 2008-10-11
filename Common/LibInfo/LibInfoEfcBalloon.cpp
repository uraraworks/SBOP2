/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoEfcBalloon.cpp										 */
/* 内容			:噴出し情報ライブラリ基底クラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoEfcBalloon.h"
#include "LibInfoEfcBalloon.h"


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::CLibInfoEfcBalloon							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CLibInfoEfcBalloon::CLibInfoEfcBalloon()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::~CLibInfoEfcBalloon						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CLibInfoEfcBalloon::~CLibInfoEfcBalloon()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Create(void)
{
	m_paInfo = new ARRAYEFCBALLOON;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::Destroy									 */
/* 内容		:破棄															 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

PCInfoBase CLibInfoEfcBalloon::GetNew(void)
{
	PCInfoEfcBalloon pInfoEfcBalloon;

	pInfoEfcBalloon = new CInfoEfcBalloon;

	return pInfoEfcBalloon;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

PCInfoBase CLibInfoEfcBalloon::GetPtr(int nNo)
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
/* 関数名	:CLibInfoEfcBalloon::GetCount									 */
/* 内容		:データ数を取得													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

int CLibInfoEfcBalloon::GetCount(void)
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
/* 関数名	:CLibInfoEfcBalloon::Add										 */
/* 内容		:追加															 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Add(PCInfoBase pInfo)
{
	PCInfoEfcBalloon pInfoEfcBalloon;

	pInfoEfcBalloon = (PCInfoEfcBalloon)pInfo;
	if (pInfoEfcBalloon->m_dwEfcBalloonID == 0) {
		pInfoEfcBalloon->m_dwEfcBalloonID = GetNewID ();
	}

	m_paInfo->Add (pInfoEfcBalloon);
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::Delete										 */
/* 内容		:削除															 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoEfcBalloon pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::Delete										 */
/* 内容		:削除															 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Delete(
	DWORD dwEfcBalloonID)		/* [in] モーションID */
{
	int i, nCount, nNo;
	PCInfoEfcBalloon pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwEfcBalloonID != dwEfcBalloonID) {
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
/* 関数名	:CLibInfoEfcBalloon::DeleteAll									 */
/* 内容		:全て削除														 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::DeleteAll(void)
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
/* 関数名	:CLibInfoEfcBalloon::SetList									 */
/* 内容		:指定IDのリストを設定											 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::SetList(DWORD dwListID, CLibInfoEfcBalloon *pSrc)
{
	int i, nCount;
	PCInfoEfcBalloon pInfo, pInfoTmp;

	/* まずは対象となるリストIDの情報を削除 */
	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_paInfo->GetAt (i);
		if (dwListID != 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
			}
		}
		Delete (i);
	}

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoEfcBalloon)pSrc->GetPtr (i);
		if (dwListID != 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
			}
		}
		pInfoTmp = new CInfoEfcBalloon;
		pInfoTmp->Copy (pInfo);
		Add (pInfoTmp);
	}

	Sort ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::GetListID									 */
/* 内容		:種別ID一覧を取得												 */
/* 日付		:2007/12/29														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::GetListID(ARRAYDWORD &aDst)
{
	int i, j, nCount, nCountTmp;
	PCInfoEfcBalloon pInfo;

	aDst.RemoveAll ();

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);

		nCountTmp = aDst.GetSize ();
		for (j = 0; j < nCountTmp; j ++) {
			if (pInfo->m_dwListID == aDst[j]) {
				break;
			}
		}
		if (j < nCountTmp) {
			continue;
		}
		aDst.Add (pInfo->m_dwListID);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::GetName									 */
/* 内容		:噴出し名を取得													 */
/* 日付		:2007/12/29														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::GetName(
	DWORD dwListID,			/* [in] 種別ID */
	CmyString &strDst)		/* [ou] 噴出し名 */
{
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	strDst.Empty ();

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (pInfo->m_dwListID != dwListID) {
			continue;
		}
		strDst = pInfo->m_strName;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::Sort										 */
/* 内容		:種別IDでソート													 */
/* 日付		:2008/01/01														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Sort(void)
{
	int i, nCount;
	DWORD dwListID;
	PCInfoEfcBalloon pInfo;
	ARRAYEFCBALLOON apInfo;

	apInfo.Copy (m_paInfo);
	m_paInfo->RemoveAll ();

	while (1) {
		nCount = apInfo.GetSize ();
		if (nCount <= 0) {
			break;
		}
		dwListID = -1;
		/* 最小の種別IDを探す */
		for (i = 0; i < nCount; i ++) {
			pInfo = apInfo[i];
			if (pInfo->m_dwListID > dwListID) {
				continue;
			}
			dwListID = pInfo->m_dwListID;
		}

		for (i = 0; i < nCount; i ++) {
			pInfo = apInfo[i];
			if (pInfo->m_dwListID != dwListID) {
				continue;
			}
			m_paInfo->Add (pInfo);
			apInfo.RemoveAt (i);
			i --;
			nCount = apInfo.GetSize ();
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::GetNextAnimeID								 */
/* 内容		:次のコマ番号を取得												 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

DWORD CLibInfoEfcBalloon::GetNextAnimeID(
	DWORD dwListID,			/* [in] 種別ID */
	DWORD dwAnimeID)		/* [in] コマ番号 */
{
	int i, nCount;
	DWORD dwRet;
	ARRAYEFCBALLOON apBalloon;
	PCInfoEfcBalloon pInfo;

	dwRet = -1;

	GetEfcBalloonInfo (dwListID, apBalloon);
	nCount = apBalloon.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = apBalloon[i];
		if (dwAnimeID + 1 != pInfo->m_dwAnimeID) {
			continue;
		}
		break;
	}
	/* 続きがある？ */
	if (i < nCount) {
		dwRet = dwAnimeID + 1;
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::GetPtr										 */
/* 内容		:噴出し情報を取得												 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

CInfoEfcBalloon *CLibInfoEfcBalloon::GetPtr(
	DWORD dwListID,			/* [in] 種別ID */
	DWORD dwAnimeID)		/* [in] コマ番号 */
{
	int i, nCount;
	PCInfoEfcBalloon pRet, pInfo;
	ARRAYEFCBALLOON apBalloon;

	pRet = NULL;
	if (dwListID == 0) {
		goto Exit;
	}
	GetEfcBalloonInfo (dwListID, apBalloon);

	nCount = apBalloon.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = apBalloon[i];
		if (dwAnimeID != pInfo->m_dwAnimeID) {
			continue;
		}
		pRet = pInfo;
		break;
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::GetEfcBalloonInfo							 */
/* 内容		:指定種別IDの情報を設定											 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::GetEfcBalloonInfo(DWORD dwListID, ARRAYEFCBALLOON &aDst)
{
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	if (dwListID == 0) {
		return;
	}
	aDst.RemoveAll ();

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoEfcBalloon)GetPtr (i);
		if (dwListID != 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
			}
		}
		aDst.Add (pInfo);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

DWORD CLibInfoEfcBalloon::GetSendDataSize(DWORD dwListID)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	dwRet = 0;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (dwListID > 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
			}
		}
		dwRet += pInfo->GetSendDataSize ();
	}

	dwRet += sizeof (DWORD);	/* 終端分 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

PBYTE CLibInfoEfcBalloon::GetSendData(DWORD dwListID)
{
	PBYTE pRet, pDataTmp, pPos;
	DWORD dwSize, dwSizeTmp;
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	pRet = NULL;

	dwSize = GetSendDataSize (dwListID);
	if (dwSize == 0) {
		goto Exit;
	}
	pRet = ZeroNew (dwSize);
	pPos = pRet;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (dwListID > 0) {
			if (pInfo->m_dwListID != dwListID) {
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
/* 関数名	:CLibInfoEfcBalloon::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

PBYTE CLibInfoEfcBalloon::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwListID;
	PCInfoEfcBalloon pInfoEfcBalloon;

	pRet = pSrc;

	pDataTmp = pSrc;
	while (1) {
		CopyMemory (&dwListID, pDataTmp, sizeof (dwListID));
		if (dwListID == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoEfcBalloon = new CInfoEfcBalloon;
		pDataTmp = pInfoEfcBalloon->SetSendData (pDataTmp);
		Add (pInfoEfcBalloon);
	}
	pRet = pDataTmp;

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEfcBalloon::GetNewID									 */
/* 内容		:新しいIDを取得													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

DWORD CLibInfoEfcBalloon::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoEfcBalloon pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwEfcBalloonID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
