/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:InfoDisable.cpp											 */
/* 内容			:拒否情報クラス 実装ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoDisable.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwDisableID",		/* 拒否情報ID */
	"m_strMacAddress",		/* MACアドレス */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoDisable::CInfoDisable										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

CInfoDisable::CInfoDisable()
{
	m_dwDisableID = 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::~CInfoDisable									 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

CInfoDisable::~CInfoDisable()
{
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::GetElementNo										 */
/* 内容		:要素番号を取得													 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

int CInfoDisable::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::GetDataSize										 */
/* 内容		:データサイズを取得												 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

DWORD CInfoDisable::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwDisableID);				/* 拒否情報ID */
	dwRet += (m_strMacAddress.GetLength () + 1);	/* MACアドレス */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

DWORD CInfoDisable::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwDisableID);				break;	/* 拒否情報ID */
	case 1:		dwRet = (m_strMacAddress.GetLength () + 1);	break;	/* MACアドレス */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::GetName											 */
/* 内容		:要素名を取得													 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

LPCSTR CInfoDisable::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::GetWriteData										 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

PBYTE CInfoDisable::GetWriteData(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_dwDisableID;			break;	/* 拒否情報ID */
	case 1:		pSrc = (PBYTE)(LPCSTR)m_strMacAddress;	break;	/* MACアドレス */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::ReadElementData									 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

DWORD CInfoDisable::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwDisableID;	dwSize = sizeof (m_dwDisableID);	break;	/* 拒否情報ID */
	case 1:		/* MACアドレス */
		m_strMacAddress = (LPCSTR)pSrc;
		dwSize = m_strMacAddress.GetLength () + 1;
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::GetSendDataSize									 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

DWORD CInfoDisable::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwDisableID)	+
			(m_strMacAddress.GetLength () + 1);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::GetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

PBYTE CInfoDisable::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwDisableID, sizeof (m_dwDisableID), pDataTmp);
	strcpyRenew ((LPSTR)pDataTmp, m_strMacAddress, pDataTmp);

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::SetSendData										 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

PBYTE CInfoDisable::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwDisableID, pDataTmp, sizeof (m_dwDisableID), pDataTmp);
	StoreRenew (m_strMacAddress, (LPCSTR)pDataTmp, pDataTmp);

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoDisable::Copy												 */
/* 内容		:コピー															 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

void CInfoDisable::Copy(CInfoDisable *pSrc)
{
	m_dwDisableID		= pSrc->m_dwDisableID;
	m_strMacAddress		= pSrc->m_strMacAddress;
}

/* Copyright(C)URARA-works 2009 */
