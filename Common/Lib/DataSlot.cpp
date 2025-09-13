/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DataSlot.cpp												 */
/* 内容			:データスロットクラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "DataSlot.h"

/* ========================================================================= */
/* 関数名	:CDataSlot::CDataSlot											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

CDataSlot::CDataSlot()
{
	m_dwNewID = 1;
}


/* ========================================================================= */
/* 関数名	:CDataSlot::~CDataSlot											 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

CDataSlot::~CDataSlot()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CDataSlot::Destroy												 */
/* 内容		:破棄															 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CDataSlot::Destroy(void)
{
	int i, nCount;

	m_CritData.Enter ();

	/* 確保されているデータを全て消去 */

	/* データスロット */
	nCount = m_DataSlot.GetSize ();
	for (i = 0; i < nCount; i ++) {
		PDATASLOTINFO pInfo;

		pInfo = &m_DataSlot[i];
		SAFE_DELETE_ARRAY (pInfo->pData);
	}
	m_DataSlot.RemoveAll ();

	m_CritData.Leave ();
}


/* ========================================================================= */
/* 関数名	:CDataSlot::Add													 */
/* 内容		:データ追加														 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

DWORD CDataSlot::Add(
	DWORD dwSize,		/* [in] データサイズ */
	LPBYTE pSrc)		/* [in] 追加するデータ */
{
	int nNo;
	DWORD dwRet;
	LPBYTE pData;
	DATASLOTINFO Info;

	m_CritData.Enter ();

	pData = new BYTE[dwSize];
	CopyMemory (pData, pSrc, dwSize);

	dwRet = (DWORD)pData;
	Info.dwID	= m_dwNewID;		/* データID */
	Info.dwSize	= dwSize;			/* データサイズ */
	Info.pData	= pData;			/* データバッファ */
	m_DataSlot.Add (Info);

	while (1) {
		m_dwNewID ++;
		if (m_dwNewID == 0) {
			continue;
		}
		nNo = GetID (m_dwNewID);
		if (nNo == -1) {
			break;
		}
	}

	m_CritData.Leave ();

	return Info.dwID;
}


/* ========================================================================= */
/* 関数名	:CDataSlot::AddDelete											 */
/* 内容		:データ追加してバッファを削除									 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

DWORD CDataSlot::AddDelete(
	DWORD dwSize,		/* [in] データサイズ */
	LPBYTE *pSrc)		/* [in] 追加するデータ */
{
	DWORD dwRet;

	dwRet = Add (dwSize, *pSrc);
	SAFE_DELETE_ARRAY (*pSrc);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CDataSlot::GetSize												 */
/* 内容		:データサイズ取得												 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

DWORD CDataSlot::GetSize(
	DWORD dwID)		/* [in] データID */
{
	int nID;
	DWORD dwRet;
	PDATASLOTINFO pInfo;

	dwRet = 0;
	m_CritData.Enter ();

	nID = GetID (dwID);
	/* データが見つからなかった？ */
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	dwRet = pInfo->dwSize;

Exit:
	m_CritData.Leave ();

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CDataSlot::Copy												 */
/* 内容		:データ取得														 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

BOOL CDataSlot::Copy(
	DWORD dwID,			/* [in] データID */
	LPBYTE pDst,		/* [out] 格納先のポインタ */
	BOOL bDelete)		/* [in] 取得後削除するかどうか */
{
	int nID;
	BOOL bRet;
	PDATASLOTINFO pInfo;

	bRet = FALSE;

	m_CritData.Enter ();

	nID = GetID (dwID);
	/* データが見つからなかった？ */
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	CopyMemory (pDst, pInfo->pData, pInfo->dwSize);

	/* 削除指定？ */
	if (bDelete) {
		SAFE_DELETE_ARRAY (pInfo->pData);
		m_DataSlot.RemoveAt (nID);
	}

	bRet = TRUE;

Exit:
	m_CritData.Leave ();

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CDataSlot::GetPtr												 */
/* 内容		:データアドレス取得												 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

LPBYTE CDataSlot::GetPtr(
	DWORD dwID)					/* [in] データID */
{
	int nID;
	LPBYTE pRet;
	PDATASLOTINFO pInfo;

	pRet = NULL;

	m_CritData.Enter ();

	if (dwID == 0) {
		goto Exit;
	}

	nID = GetID (dwID);
	/* データが見つからなかった？ */
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	pRet = pInfo->pData;

Exit:
	m_CritData.Leave ();

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CDataSlot::Delete												 */
/* 内容		:データ削除														 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CDataSlot::Delete(
	DWORD dwID)		/* [in] データID */
{
	int nID;
	PDATASLOTINFO pInfo;

	m_CritData.Enter ();

	nID = GetID (dwID);
	/* データが見つからなかった？ */
	if (nID < 0) {
		goto Exit;
	}

	pInfo = &m_DataSlot[nID];
	SAFE_DELETE_ARRAY (pInfo->pData);
	m_DataSlot.RemoveAt (nID);

Exit:
	m_CritData.Leave ();
}


/* ========================================================================= */
/* 関数名	:CDataSlot::Lock												 */
/* 内容		:データのロック													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CDataSlot::Lock(void)
{
	m_CritDataLock.Enter ();
}


/* ========================================================================= */
/* 関数名	:CDataSlot::Lock												 */
/* 内容		:データのロック解除												 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CDataSlot::UnLock(void)
{
	m_CritDataLock.Leave ();
}


/* ========================================================================= */
/* 関数名	:CDataSlot::Lock												 */
/* 内容		:テーブルID取得													 */
/* 日付		:2008/06/07														 */
/* 戻り値	:テーブルID(-1:見つからなかった)								 */
/* ========================================================================= */

int CDataSlot::GetID(
	DWORD dwID)		/* [in] データID */
{
	int i, nCount, nRet;
	PDATASLOTINFO pInfo;

	nRet = -1;
	nCount = m_DataSlot.GetSize ();

	for (i = 0; i < nCount; i ++) {
		pInfo = &m_DataSlot[i];
		if (pInfo->dwID == dwID) {
			nRet = i;
			break;
		}
	}

	return nRet;
}

/* Copyright(C)URARA-works 2008 */
