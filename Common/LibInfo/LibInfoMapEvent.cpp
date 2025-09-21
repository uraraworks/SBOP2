/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoMapEvent.cpp										 */
/* 内容			:マップイベント情報ライブラリクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEvent.h"
#include "LibInfoMapEvent.h"


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::CLibInfoMapEvent								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

CLibInfoMapEvent::CLibInfoMapEvent()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::~CLibInfoMapEvent							 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

CLibInfoMapEvent::~CLibInfoMapEvent()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::Create										 */
/* 内容		:作成															 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::Create(void)
{
	m_paInfo = new ARRAYMAPEVENTBASEINFO;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapEvent::GetNew(int nType)
{
	PCInfoMapEventBase pInfo;

	pInfo = NULL;

	switch (nType) {
	case MAPEVENTTYPE_MOVE:				/* マップ内移動 */
		pInfo = new CInfoMapEventMOVE;
		break;
	case MAPEVENTTYPE_MAPMOVE:			/* マップ間移動 */
		pInfo = new CInfoMapEventMAPMOVE;
		break;
	case MAPEVENTTYPE_INITSTATUS:		/* ステータス初期化 */
		pInfo = new CInfoMapEventINITSTATUS;
		break;
	case MAPEVENTTYPE_GRPIDTMP:			/* 一時画像設定 */
		pInfo = new CInfoMapEventGRPIDTMP;
		break;
	case MAPEVENTTYPE_LIGHT:			/* 灯り */
		pInfo = new CInfoMapEventLIGHT;
		break;
	default:
		pInfo = new CInfoMapEventBase;
		break;
	}
	pInfo->m_nType = nType;

	return pInfo;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

int CLibInfoMapEvent::GetCount(void)
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
/* 関数名	:CLibInfoMapEvent::Add											 */
/* 内容		:追加															 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::Add(PCInfoBase pInfo)
{
	PCInfoMapEventBase pInfoMapEvent;

	pInfoMapEvent = (PCInfoMapEventBase)pInfo;
	if (pInfoMapEvent->m_dwMapEventID == 0) {
		pInfoMapEvent->m_dwMapEventID = GetNewID ();
	}
	m_paInfo->Add (pInfoMapEvent);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::Delete										 */
/* 内容		:削除															 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoMapEventBase pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::Delete										 */
/* 内容		:削除															 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::Delete(DWORD dwMapEventID)
{
	int i, nCount;
	PCInfoMapEventBase pInfoTmp;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapEventID != dwMapEventID) {
			continue;
		}
		Delete (i);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::DeleteAll									 */
/* 内容		:全て削除														 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::DeleteAll(void)
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
/* 関数名	:CLibInfoMapEvent::Merge										 */
/* 内容		:取り込み														 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

void CLibInfoMapEvent::Merge(CLibInfoMapEvent *pSrc)
{
	int i, nCount;
	PCInfoMapEventBase pInfoTmp, pInfoSrc, pInfoDst;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapEventBase)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoMapEventBase)GetPtr (pInfoSrc->m_dwMapEventID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapEventBase)GetNew (pInfoSrc->m_nType);
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		} else {
			pInfoDst = (PCInfoMapEventBase)GetNew (pInfoSrc->m_nType);
			pInfoDst->Copy (pInfoSrc);
			Renew (pInfoSrc->m_dwMapEventID, pInfoDst);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::Renew										 */
/* 内容		:更新															 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

CInfoMapEventBase *CLibInfoMapEvent::Renew(DWORD dwMapEventID, CInfoMapEventBase *pSrc)
{
	int i, nCount;
	PCInfoMapEventBase pRet, pInfo, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapEventID != dwMapEventID) {
			continue;
		}
		pInfo = (PCInfoMapEventBase)GetNew (pSrc->m_nType);
		pInfo->Copy (pSrc);
		pInfo->m_dwMapEventID = pInfoTmp->m_dwMapEventID;

		SAFE_DELETE (pInfoTmp);
		m_paInfo->SetAt (i, pInfo);
		pRet = pInfo;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::GetPtr										 */
/* 内容		:マップイベント情報を取得										 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapEvent::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::GetPtr										 */
/* 内容		:マップイベント情報を取得										 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapEvent::GetPtr(DWORD dwMapEventID)
{
	int i, nCount;
	PCInfoMapEventBase pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapEventID == dwMapEventID) {
			pRet = pInfoTmp;
			break;
		}
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

DWORD CLibInfoMapEvent::GetDataSize(void)
{
	int i, j, nCount, nCount2;
	DWORD dwRet;
	PCInfoMapEventBase pInfo;

	dwRet = 0;

	dwRet += sizeof (int);									/* データ数 */
	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		dwRet += sizeof (int);								/* 要素数 */
		nCount2 = pInfo->GetElementCount ();
		for (j = 0; j < nCount2; j ++) {
			dwRet += (strlen (pInfo->GetName (j)) + 1);		/* 要素名 */
			dwRet += sizeof (DWORD);						/* データサイズ */
			dwRet += pInfo->GetDataSizeNo (j);				/* データ */
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::GetWriteData									 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/06/22														 */
/* ========================================================================= */

PBYTE CLibInfoMapEvent::GetWriteData(PDWORD pdwSize)
{
	int i, j, nCount, nCount2;
	PBYTE pRet, pRetTmp, pTmp;
	DWORD dwSize, dwTmp;
	PCInfoMapEventBase pInfo;

	pRet		= NULL;
	dwSize		= GetDataSize ();
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}

	pRet = ZeroNew (dwSize);
	pRetTmp = pRet;

	nCount = m_paInfo->GetSize ();
	CopyMemoryRenew (pRetTmp, &nCount, sizeof (nCount), pRetTmp);		/* データ数 */
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		nCount2 = pInfo->GetElementCount ();
		CopyMemoryRenew (pRetTmp, &nCount2, sizeof (nCount2), pRetTmp);	/* 要素数 */

		for (j = 0; j < nCount2; j ++) {
			strcpyRenew ((LPSTR)pRetTmp, pInfo->GetName (j), pRetTmp); 	/* 要素名 */
			dwTmp = pInfo->GetDataSizeNo (j);
			CopyMemoryRenew (pRetTmp, &dwTmp, sizeof (dwTmp), pRetTmp);	/* データサイズ */
			pTmp = pInfo->GetWriteData (j, &dwTmp);
			CopyMemoryRenew (pRetTmp, pTmp, dwTmp, pRetTmp);			/* データ */
			SAFE_DELETE_ARRAY (pTmp);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

DWORD CLibInfoMapEvent::ReadElementData(PBYTE pSrc)
{
	int i, j, nNo, nCount, nCount2, nType;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapEventBase pInfo;
	CmyString strTmp;

	pDst	= NULL;
	dwSize	= 0;

	DeleteAll ();

	pSrcTmp = pSrc;
	CopyMemoryRenew (&nCount, pSrcTmp, sizeof (nCount), pSrcTmp);				/* データ数 */
	for (i = 0; i < nCount; i ++) {
		CopyMemoryRenew (&nCount2, pSrcTmp, sizeof (nCount2), pSrcTmp);			/* 要素数 */

		for (j = 0; j < nCount2; j ++) {
			StoreRenew (strTmp, (LPCSTR)pSrcTmp, pSrcTmp);						/* 要素名 */
			CopyMemoryRenew (&dwSizeTmp, pSrcTmp, sizeof (dwSizeTmp), pSrcTmp);	/* データサイズ */
			if (j == 0) {
				/* 最初はイベント種別 */
				CopyMemoryRenew (&nType, pSrcTmp, sizeof (nType), pSrcTmp);		/* イベント種別 */
				pInfo = (PCInfoMapEventBase)GetNew (nType);
			} else {
				nNo = pInfo->GetElementNo ((LPCSTR)strTmp);
				if (nNo >= 0) {
					dwSizeTmp = pInfo->ReadElementData (pSrcTmp, nNo);
				}
				pSrcTmp += dwSizeTmp;
			}
		}
		Add (pInfo);
	}

	return (DWORD)(pSrcTmp - pSrc);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

DWORD CLibInfoMapEvent::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoMapEventBase pInfo;

	dwRet = dwSize = 0;

	/* データ数分のサイズ */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapEventBase)GetPtr (i);

		dwSize += pInfo->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PBYTE CLibInfoMapEvent::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoMapEventBase pInfo;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* データ数を書き込み */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* イベント情報を書き込み */
	nCount = (int)dwCount;
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapEventBase)GetPtr (i);

		dwSizeTmp	= pInfo->GetSendDataSize ();
		pDataTmp	= pInfo->GetSendData ();
		CopyMemory (&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY (pDataTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/05/05														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CLibInfoMapEvent::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CInfoMapEventBase InfoTmp, *pInfo;

	pRet		= pSrc;
	pDataTmp	= pSrc;

	DeleteAll ();

	/* データ数を読み込み */
	CopyMemory (&dwCount, pDataTmp, sizeof (dwCount));
	nCount		= (int)dwCount;
	pDataTmp	+= sizeof (dwCount);

	for (i = 0; i < nCount; i ++) {
		pDataTmpBack = pDataTmp;
		InfoTmp.SetSendData (pDataTmp);
		pInfo = (PCInfoMapEventBase)GetNew (InfoTmp.m_nType);
		pDataTmp = pDataTmpBack;
		pDataTmp = pInfo->SetSendData (pDataTmp);
		Add (pInfo);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapEvent::GetNewID										 */
/* 内容		:新しいIDを取得													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

DWORD CLibInfoMapEvent::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapEventBase pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapEventID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
