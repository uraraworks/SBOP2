/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoMapObjectData.cpp									 */
/* 内容			:マップオブジェクト配置データライブラリクラス 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapObjectData.h"
#include "LibInfoMapObjectData.h"


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::CLibInfoMapObjectData					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

CLibInfoMapObjectData::CLibInfoMapObjectData()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::~CLibInfoMapObjectData					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

CLibInfoMapObjectData::~CLibInfoMapObjectData()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::Create									 */
/* 内容		:作成															 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Create(void)
{
	m_paInfo = new ARRAYMAPOBJECTDATAINFO;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::Destroy									 */
/* 内容		:破棄															 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::GetNew									 */
/* 内容		:新規データを取得												 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObjectData::GetNew(void)
{
	PCInfoMapObjectData pInfo;

	pInfo = new CInfoMapObjectData;

	return pInfo;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::GetCount								 */
/* 内容		:データ数を取得													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

int CLibInfoMapObjectData::GetCount(void)
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
/* 関数名	:CLibInfoMapObjectData::Add										 */
/* 内容		:追加															 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Add(PCInfoBase pInfo)
{
	PCInfoMapObjectData pInfoMapObjectData;

	pInfoMapObjectData = (PCInfoMapObjectData)pInfo;
	if (pInfoMapObjectData->m_dwDataID == 0) {
		pInfoMapObjectData->m_dwDataID = GetNewID ();
	}
	m_paInfo->Add (pInfoMapObjectData);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::Delete									 */
/* 内容		:削除															 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoMapObjectData pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::Delete									 */
/* 内容		:削除															 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Delete(DWORD dwDataID)
{
	int i, nCount;
	PCInfoMapObjectData pInfoTmp;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDataID != dwDataID) {
			continue;
		}
		Delete (i);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::DeleteAll								 */
/* 内容		:全て削除														 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::DeleteAll(void)
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
/* 関数名	:CLibInfoMapObjectData::Merge									 */
/* 内容		:取り込み														 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Merge(CLibInfoMapObjectData *pSrc)
{
	int i, nCount;
	PCInfoMapObjectData pInfoTmp, pInfoSrc, pInfoDst;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapObjectData)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoMapObjectData)GetPtr (pInfoSrc->m_dwDataID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapObjectData)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		} else {
			pInfoDst = (PCInfoMapObjectData)GetNew ();
			pInfoDst->Copy (pInfoSrc);
			Renew (pInfoSrc->m_dwDataID, pInfoDst);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::Renew									 */
/* 内容		:更新															 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

CInfoMapObjectData *CLibInfoMapObjectData::Renew(DWORD dwDataID, CInfoMapObjectData *pSrc)
{
	int i, nCount;
	PCInfoMapObjectData pRet, pInfo, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDataID != dwDataID) {
			continue;
		}
		pInfo = (PCInfoMapObjectData)GetNew ();
		pInfo->Copy (pSrc);
		pInfo->m_dwDataID = pInfoTmp->m_dwDataID;

		SAFE_DELETE (pInfoTmp);
		m_paInfo->SetAt (i, pInfo);
		pRet = pInfo;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::GetPtr									 */
/* 内容		:配置データを取得												 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObjectData::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::GetPtr									 */
/* 内容		:配置データを取得												 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObjectData::GetPtr(DWORD dwDataID)
{
	int i, nCount;
	PCInfoMapObjectData pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDataID == dwDataID) {
			pRet = pInfoTmp;
			break;
		}
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectDataMOVE::GetDataSize							 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

DWORD CLibInfoMapObjectData::GetDataSize(void)
{
	int i, j, nCount, nCount2;
	DWORD dwRet;
	PCInfoMapObjectData pInfo;

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
/* 関数名	:CLibInfoMapObjectData::GetWriteData							 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

PBYTE CLibInfoMapObjectData::GetWriteData(PDWORD pdwSize)
{
	int i, j, nCount, nCount2;
	PBYTE pRet, pRetTmp, pTmp;
	DWORD dwSize, dwTmp;
	PCInfoMapObjectData pInfo;

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
/* 関数名	:CLibInfoMapObjectData::ReadElementData							 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

DWORD CLibInfoMapObjectData::ReadElementData(PBYTE pSrc)
{
	int i, j, nNo, nCount, nCount2;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapObjectData pInfo;
	CmyString strTmp;

	pDst	= NULL;
	dwSize	= 0;

	DeleteAll ();

	pSrcTmp = pSrc;
	CopyMemoryRenew (&nCount, pSrcTmp, sizeof (nCount), pSrcTmp);				/* データ数 */
	for (i = 0; i < nCount; i ++) {
		CopyMemoryRenew (&nCount2, pSrcTmp, sizeof (nCount2), pSrcTmp);			/* 要素数 */

		pInfo = (PCInfoMapObjectData)GetNew ();
		for (j = 0; j < nCount2; j ++) {
			StoreRenew (strTmp, (LPCSTR)pSrcTmp, pSrcTmp);						/* 要素名 */
			CopyMemoryRenew (&dwSizeTmp, pSrcTmp, sizeof (dwSizeTmp), pSrcTmp);	/* データサイズ */
			nNo = pInfo->GetElementNo ((LPCSTR)strTmp);
			if (nNo >= 0) {
				dwSizeTmp = pInfo->ReadElementData (pSrcTmp, nNo);
			}
			pSrcTmp += dwSizeTmp;
		}
		Add (pInfo);
	}

	return (DWORD)(pSrcTmp - pSrc);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

DWORD CLibInfoMapObjectData::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoMapObjectData pInfo;

	dwRet = dwSize = 0;

	/* データ数分のサイズ */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)GetPtr (i);

		dwSize += pInfo->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

PBYTE CLibInfoMapObjectData::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoMapObjectData pInfo;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* データ数を書き込み */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* 配置データを書き込み */
	nCount = (int)dwCount;
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)GetPtr (i);

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
/* 関数名	:CLibInfoMapObjectData::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/11/03														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CLibInfoMapObjectData::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CInfoMapObjectData InfoTmp, *pInfo;

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
		pInfo = (PCInfoMapObjectData)GetNew ();
		pDataTmp = pDataTmpBack;
		pDataTmp = pInfo->SetSendData (pDataTmp);
		Add (pInfo);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapObjectData::GetNewID								 */
/* 内容		:新しいIDを取得													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

DWORD CLibInfoMapObjectData::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapObjectData pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDataID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
