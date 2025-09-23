/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoFileList.cpp											 */
/* 内容			:ファイルリスト情報クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/03/02													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoFileList.h"

/* ========================================================================= */
/* 関数名	:CInfoFileList::CInfoFileList									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/03/02														 */
/* ========================================================================= */

CInfoFileList::CInfoFileList()
{
}


/* ========================================================================= */
/* 関数名	:CInfoFileList::~CInfoFileList									 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/03/02														 */
/* ========================================================================= */

CInfoFileList::~CInfoFileList()
{
	DeleteAll ();
}


/* ========================================================================= */
/* 関数名	:CInfoFileList::Add												 */
/* 内容		:追加															 */
/* 日付		:2008/03/02														 */
/* ========================================================================= */

void CInfoFileList::Add(LPCSTR pszHash, LPCSTR pszFileName, DWORD dwFileSize)
{
	PFILELISTINFO pInfo;

	pInfo = GetNew ();
	pInfo->strMD5		= pszHash;
	pInfo->strFileName	= pszFileName;
	pInfo->dwFileSize	= dwFileSize;
	m_aFileListInfo.push_back (pInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoFileList::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2008/03/02														 */
/* ========================================================================= */

int CInfoFileList::GetCount(void)
{
	return m_aFileListInfo.size();
}


/* ========================================================================= */
/* 関数名	:CInfoFileList::GetPtr											 */
/* 内容		:ファイルリスト情報を取得										 */
/* 日付		:2008/03/02														 */
/* ========================================================================= */

PFILELISTINFO CInfoFileList::GetPtr(int nNo)
{
	PFILELISTINFO pRet;

	pRet = NULL;
	if ((nNo < 0) || (nNo >= static_cast<int>(m_aFileListInfo.size()))) {
		goto Exit;
	}

	pRet = m_aFileListInfo[nNo];
Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoFileList::DeleteAll										 */
/* 内容		:全て削除														 */
/* 日付		:2008/03/02														 */
/* ========================================================================= */

void CInfoFileList::DeleteAll(void)
{
	int i, nCount;
	PFILELISTINFO pInfo;

	nCount = m_aFileListInfo.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aFileListInfo[i];
		SAFE_DELETE (pInfo);
	}
	m_aFileListInfo.clear();
}


/* ========================================================================= */
/* 関数名	:CInfoFileList::GetNew											 */
/* 内容		:新規データ取得													 */
/* 日付		:2008/03/02														 */
/* ========================================================================= */

PFILELISTINFO CInfoFileList::GetNew(void)
{
	PFILELISTINFO pRet;

	pRet = new FILELISTINFO;
	pRet->dwFileSize = 0;

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoFileList::Copy											 */
/* 内容		:コピー															 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

void CInfoFileList::Copy(CInfoFileList *pSrc)
{
	int i, nCount;
	PFILELISTINFO pInfo;

	DeleteAll ();

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = pSrc->GetPtr (i);
		Add (pInfo->strMD5, pInfo->strFileName, pInfo->dwFileSize);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoFileList::GetSendDataSize									 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

DWORD CInfoFileList::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PFILELISTINFO pInfo;

	dwRet = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = GetPtr (i);
		dwRet += sizeof (pInfo->dwFileSize);				/* ファイルサイズ */
		dwRet += (pInfo->strMD5.GetLength () + 1);			/* MD5ハッシュ */
		dwRet += (pInfo->strFileName.GetLength () + 1);		/* ファイル名 */
	}
	dwRet += sizeof (DWORD);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoFileList::GetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

PBYTE CInfoFileList::GetSendData(void)
{
	PBYTE pRet, pData, pDataTmp;
	int i, nCount;
	DWORD dwSize, dwTmp;
	PFILELISTINFO pInfo;

	pRet = NULL;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;
	nCount   = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = GetPtr (i);
		CopyMemoryRenew (pDataTmp, &pInfo->dwFileSize, sizeof (pInfo->dwFileSize), pDataTmp);	/* ファイルサイズ */
		strcpyRenew ((LPSTR)pDataTmp, pInfo->strMD5,		pDataTmp);	/* MD5ハッシュ */
		strcpyRenew ((LPSTR)pDataTmp, pInfo->strFileName,	pDataTmp);	/* ファイル名 */
	}
	dwTmp = 0;
	CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);		/* 終端 */

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoFileList::SetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

PBYTE CInfoFileList::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwSize;
	CmyString strHash, strFileName;

	pRet = pSrc;
	DeleteAll ();

	pDataTmp = pSrc;
	while (1) {
		CopyMemoryRenew (&dwSize, pDataTmp, sizeof (dwSize), pDataTmp);		/* ファイルサイズ */
		if (dwSize == 0) {
			break;
		}
		StoreRenew (strHash,	 (LPCSTR)pDataTmp, pDataTmp);	/* MD5ハッシュ */
		StoreRenew (strFileName, (LPCSTR)pDataTmp, pDataTmp);	/* ファイル名 */
		Add (strHash, strFileName, dwSize);
	}

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
