/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMapObjectData.cpp										 */
/* 内容			:マップオブジェクト配置データクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapObjectData.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwDataID",		/* 配置データID */
	"m_dwObjectID",		/* オブジェクトID */
	"m_ptPos",			/* 座標 */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::CInfoMapObjectData							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

CInfoMapObjectData::CInfoMapObjectData()
{
	m_dwDataID		= 0;
	m_dwObjectID	= 0;
	ZeroMemory (&m_ptPos,  sizeof (m_ptPos));

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::~CInfoMapObjectData						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

CInfoMapObjectData::~CInfoMapObjectData()
{
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::RenewSize									 */
/* 内容		:サイズ更新														 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CInfoMapObjectData::RenewSize(int nDirection, int nSize, SIZE *pSize)
{
	switch (nDirection) {
	case 0:
		m_ptPos. y += nSize;
		break;
	case 2:
		m_ptPos. x += nSize;
		break;
	}

	/* 座標の調整 */
	m_ptPos.x = max (0, m_ptPos.x);
	m_ptPos.x = min (m_ptPos.x, pSize->cx - 1);
	m_ptPos.y = max (0, m_ptPos.y);
	m_ptPos.y = min (m_ptPos.y, pSize->cy - 1);
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::GetElementNo								 */
/* 内容		:要素番号を取得													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

int CInfoMapObjectData::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoMapObjectData::GetDataSize								 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

DWORD CInfoMapObjectData::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwDataID);		/* 配置データID */
	dwRet += sizeof (m_dwObjectID);		/* オブジェクトID */
	dwRet += sizeof (m_ptPos);			/* 座標 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::GetDataSizeNo								 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

DWORD CInfoMapObjectData::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_dwDataID);		break;	/* 配置データID */
	case 1:	dwRet = sizeof (m_dwObjectID);		break;	/* オブジェクトID */
	case 2:	dwRet = sizeof (m_ptPos);			break;	/* 座標 */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::GetName									 */
/* 内容		:要素名を取得													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

LPCSTR CInfoMapObjectData::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::GetWriteData								 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

PBYTE CInfoMapObjectData::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = (PBYTE)&m_dwDataID;		break;	/* 配置データID */
	case 1:	pSrc = (PBYTE)&m_dwObjectID;	break;	/* オブジェクトID */
	case 2:	pSrc = (PBYTE)&m_ptPos;			break;	/* 座標 */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::ReadElementData							 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

DWORD CInfoMapObjectData::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:	pDst = (PBYTE)&m_dwDataID;		dwSize = sizeof (m_dwDataID);	break;	/* 配置データID */
	case 1:	pDst = (PBYTE)&m_dwObjectID;	dwSize = sizeof (m_dwObjectID);	break;	/* オブジェクトID */
	case 2:	pDst = (PBYTE)&m_ptPos;			dwSize = sizeof (m_ptPos);		break;	/* 座標 */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

DWORD CInfoMapObjectData::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwDataID)		+	/* 配置データID */
			sizeof (m_dwObjectID)	+	/* オブジェクトID */
			sizeof (m_ptPos);			/* 座標 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

PBYTE CInfoMapObjectData::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwDataID,		sizeof (m_dwDataID),	pDataTmp);	/* 配置データID */
	CopyMemoryRenew (pDataTmp, &m_dwObjectID,	sizeof (m_dwObjectID),	pDataTmp);	/* オブジェクトID */
	CopyMemoryRenew (pDataTmp, &m_ptPos,		sizeof (m_ptPos),		pDataTmp);	/* 座標 */

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

PBYTE CInfoMapObjectData::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwDataID,	pDataTmp, sizeof (m_dwDataID),		pDataTmp);	/* 配置データID */
	CopyMemoryRenew (&m_dwObjectID,	pDataTmp, sizeof (m_dwObjectID),	pDataTmp);	/* オブジェクトID */
	CopyMemoryRenew (&m_ptPos,		pDataTmp, sizeof (m_ptPos),			pDataTmp);	/* 座標 */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObjectData::Copy										 */
/* 内容		:コピー															 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CInfoMapObjectData::Copy(CInfoMapObjectData *pSrc)
{
	if (pSrc == NULL) {
		return;
	}
	m_dwDataID		= pSrc->m_dwDataID;			/* 配置データID */
	m_dwObjectID	= pSrc->m_dwObjectID;		/* オブジェクトID */
	m_ptPos			= pSrc->m_ptPos;			/* 座標 */
}

/* Copyright(C)URARA-works 2008 */
