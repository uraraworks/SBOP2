/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoTalkEventMENU.cpp										 */
/* 内容			:会話イベント情報(項目選択)クラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoTalkEventMENU.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"nCount",			/* 項目数 */
	"nPage",			/* ジャンプ先ページ番号 */
	"strName",			/* 項目名 */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::CInfoTalkEventMENU							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

CInfoTalkEventMENU::CInfoTalkEventMENU()
{
	m_nEventType = TALKEVENTTYPE_MENU;

	m_nElementCountBase = m_nElementCount;
	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::~CInfoTalkEventMENU						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

CInfoTalkEventMENU::~CInfoTalkEventMENU()
{
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::GetElementCount							 */
/* 内容		:要素数を取得													 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

int CInfoTalkEventMENU::GetElementCount(void)
{
	return m_nElementCountBase + m_nElementCount;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::GetElementNo								 */
/* 内容		:要素番号を取得													 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

int CInfoTalkEventMENU::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = CInfoTalkEventBase::GetElementNo (pszName);
	if (nRet >= 0) {
		goto Exit;
	}

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = m_nElementCountBase + i;
			break;
		}
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::GetDataSize								 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

DWORD CInfoTalkEventMENU::GetDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PSTTALKEVENTMENUINFO pInfo;

	nCount = m_aMenuInfo.GetSize ();

	dwRet = CInfoTalkEventBase::GetDataSize();
	dwRet += sizeof (int);				/* 項目数 */
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aMenuInfo[i];
		dwRet += sizeof (int);						/* ジャンプ先ページ番号 */
		dwRet += (pInfo->strName.GetLength () + 1);	/* 項目名 */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::GetDataSizeNo								 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

DWORD CInfoTalkEventMENU::GetDataSizeNo(int nNo)
{
	int i, nCount;
	DWORD dwRet;
	PSTTALKEVENTMENUINFO pInfo;

	dwRet = 0;
	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetDataSizeNo (nNo);
	}

	nCount = m_aMenuInfo.GetSize ();

	switch (nNo - m_nElementCountBase) {
	case 0:			/* 項目数 */
		dwRet = sizeof (int);
		break;
	case 1:			/* ジャンプ先ページ番号 */
		dwRet = sizeof (int) * nCount;
		break;
	case 2:			/* 項目名 */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_aMenuInfo[i];
			dwRet += (pInfo->strName.GetLength () + 1);
		}
		break;
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::GetName									 */
/* 内容		:要素名を取得													 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

LPCSTR CInfoTalkEventMENU::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetName (nNo);
	}
	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::GetWriteData								 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

PBYTE CInfoTalkEventMENU::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	DWORD dwSize;
	PBYTE pRet, pSrc, pDataTmp;
	PSTTALKEVENTMENUINFO pInfo;

	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetWriteData (nNo, pdwSize);
	}

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet	 = ZeroNew (dwSize);
	nCount	 = m_aMenuInfo.GetSize ();
	pDataTmp = pRet;

	nNo -= m_nElementCountBase;
	switch (nNo) {
	case 0:			/* 項目数 */
		pSrc = (PBYTE)&nCount;
		break;
	case 1:			/* ジャンプ先ページ番号 */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_aMenuInfo[i];
			CopyMemoryRenew (pDataTmp, &pInfo->nPage, sizeof (pInfo->nPage), pDataTmp);
		}
		break;
	case 2:			/* 項目名 */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_aMenuInfo[i];
			strcpyRenew ((LPSTR)pDataTmp, pInfo->strName, pDataTmp);
		}
		break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::ReadElementData							 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

DWORD CInfoTalkEventMENU::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	int i, nCount;
	DWORD dwSize;
	PBYTE pDst, pDataTmp;
	PSTTALKEVENTMENUINFO pInfo;

	pDst	= NULL;
	dwSize	= 0;

	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::ReadElementData (pSrc, nNo);
	}

	pDataTmp = pSrc;
	nCount	 = m_aMenuInfo.GetSize ();

	nNo -= m_nElementCountBase;
	switch (nNo) {
	case 0:			/* 項目数 */
		DeleteAllMenuInfo ();
		nCount = 0;
		CopyMemoryRenew (&nCount, pDataTmp, sizeof (nCount), pDataTmp);
		for (i = 0; i < nCount; i ++) {
			AddMenuInfo (0, NULL);
		}
		dwSize = (pDataTmp - pSrc);
		break;
	case 1:			/* ジャンプ先ページ番号 */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_aMenuInfo[i];
			CopyMemoryRenew (&pInfo->nPage, pDataTmp, sizeof (pInfo->nPage), pDataTmp);
		}
		dwSize = (pDataTmp - pSrc);
		break;
	case 2:			/* 項目名 */
		for (i = 0; i < nCount; i ++) {
			pInfo = m_aMenuInfo[i];
			StoreRenew (pInfo->strName, (LPCSTR)pDataTmp, pDataTmp);
		}
		dwSize = (pDataTmp - pSrc);
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

DWORD CInfoTalkEventMENU::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoTalkEventBase::GetSendDataSize ();
	dwRet += GetDataSizeNo (m_nElementCountBase + 0);	/* 項目数 */
	dwRet += GetDataSizeNo (m_nElementCountBase + 1);	/* ジャンプ先ページ番号 */
	dwRet += GetDataSizeNo (m_nElementCountBase + 2);	/* 項目名 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

PBYTE CInfoTalkEventMENU::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;
	PSTTALKEVENTMENUINFO pInfo;

	dwSize	 = GetSendDataSize ();
	pData	 = ZeroNew (dwSize);
	pDataTmp = pData;

	pDataBase  = CInfoTalkEventBase::GetSendData ();
	dwSizeBase = CInfoTalkEventBase::GetSendDataSize ();
	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);
	SAFE_DELETE_ARRAY (pDataBase);

	nCount = m_aMenuInfo.GetSize ();
	CopyMemoryRenew (pDataTmp, &nCount, sizeof (nCount), pDataTmp);		/* 項目数 */

	for (i = 0; i < nCount; i ++) {
		pInfo = m_aMenuInfo[i];
		/* ジャンプ先ページ番号 */
		CopyMemoryRenew (pDataTmp, &pInfo->nPage, sizeof (pInfo->nPage), pDataTmp);
		/* 項目名 */
		strcpyRenew ((LPSTR)pDataTmp, pInfo->strName, pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

PBYTE CInfoTalkEventMENU::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pRet, pDataTmp;
	PSTTALKEVENTMENUINFO pInfo;

	pRet = pSrc;
	DeleteAllMenuInfo ();

	pDataTmp = CInfoTalkEventBase::SetSendData (pSrc);
	CopyMemoryRenew (&nCount, pDataTmp, sizeof (nCount), pDataTmp);		/* 項目数 */
	for (i = 0; i < nCount; i ++) {
		AddMenuInfo (0, NULL);
		pInfo = m_aMenuInfo[i];
		/* ジャンプ先ページ番号 */
		CopyMemoryRenew (&pInfo->nPage, pDataTmp, sizeof (pInfo->nPage), pDataTmp);
		/* 項目名 */
		StoreRenew (pInfo->strName, (LPCSTR)pDataTmp, pDataTmp);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::Copy										 */
/* 内容		:コピー															 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CInfoTalkEventMENU::Copy(CInfoTalkEventBase *pSrc)
{
	int i, nCount;
	PSTTALKEVENTMENUINFO pInfo;
	PCInfoTalkEventMENU pSrcTmp;

	if (pSrc == NULL) {
		return;
	}
	pSrcTmp = (PCInfoTalkEventMENU)pSrc;
	DeleteAllMenuInfo ();

	CInfoTalkEventBase::Copy (pSrcTmp);

	nCount = pSrcTmp->GetMenuInfoCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = pSrcTmp->GetPtr (i);
		AddMenuInfo (pInfo->nPage, (LPCSTR)pInfo->strName);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::DeleteMenuInfo								 */
/* 内容		:項目情報を削除													 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CInfoTalkEventMENU::DeleteMenuInfo(int nNo)
{
	PSTTALKEVENTMENUINFO pInfo;

	if (nNo >= m_aMenuInfo.GetSize ()) {
		return;
	}

	pInfo = m_aMenuInfo[nNo];
	SAFE_DELETE (pInfo);
	m_aMenuInfo.RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::DeleteAllMenuInfo							 */
/* 内容		:項目情報を全て削除												 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CInfoTalkEventMENU::DeleteAllMenuInfo(void)
{
	int i, nCount;

	nCount = GetMenuInfoCount ();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteMenuInfo (i);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::AddMenuInfo								 */
/* 内容		:項目情報を追加													 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CInfoTalkEventMENU::AddMenuInfo(int nPage, LPCSTR pszName)
{
	PSTTALKEVENTMENUINFO pInfo;

	pInfo = new STTALKEVENTMENUINFO;
	pInfo->nPage	= nPage;		/* ジャンプ先ページ番号 */
	pInfo->strName	= pszName;		/* 項目名 */
	m_aMenuInfo.Add (pInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::GetMenuInfoCount							 */
/* 内容		:項目数を取得													 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

int CInfoTalkEventMENU::GetMenuInfoCount(void)
{
	return m_aMenuInfo.GetSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventMENU::GetPtr										 */
/* 内容		:項目情報を取得													 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

PSTTALKEVENTMENUINFO CInfoTalkEventMENU::GetPtr(int nNo)
{
	if (nNo >= m_aMenuInfo.GetSize ()) {
		return NULL;
	}

	return m_aMenuInfo[nNo];
}

/* Copyright(C)URARA-works 2008 */
