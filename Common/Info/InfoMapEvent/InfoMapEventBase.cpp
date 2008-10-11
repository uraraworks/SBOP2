/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoMapEventBase.cpp										 */
/* 内容			:イベント情報基底クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_nType",			/* イベント種別 */
	"m_dwMapEventID",	/* マップイベントID */
	"m_ptPos",			/* 座標 */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::CInfoMapEventBase							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

CInfoMapEventBase::CInfoMapEventBase()
{
	m_dwMapEventID	= 0;
	m_nType			= MAPEVENTTYPE_NONE;
	ZeroMemory (&m_ptPos, sizeof (m_ptPos));

	for (m_nElementCountBase = 0; s_aszName[m_nElementCountBase] != NULL; m_nElementCountBase ++) {}
	m_nElementCount = m_nElementCountBase;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::~CInfoMapEventBase							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

CInfoMapEventBase::~CInfoMapEventBase()
{
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::RenewSize									 */
/* 内容		:サイズ更新														 */
/* 日付		:2008/09/14														 */
/* ========================================================================= */

void CInfoMapEventBase::RenewSize(int nDirection, int nSize)
{
	switch (nDirection) {
	case 0:
		m_ptPos.y += nSize;
		break;
	case 2:
		m_ptPos.x += nSize;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::GetElementNo								 */
/* 内容		:要素番号を取得													 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

int CInfoMapEventBase::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoMapEventBase::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_nType);
	dwRet += sizeof (m_dwMapEventID);
	dwRet += sizeof (m_ptPos);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::GetDataSizeNo								 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_nType);			break;
	case 1:	dwRet = sizeof (m_dwMapEventID);	break;
	case 2:	dwRet = sizeof (m_ptPos);			break;
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::GetName										 */
/* 内容		:要素名を取得													 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

LPCSTR CInfoMapEventBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::GetWriteData								 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

PBYTE CInfoMapEventBase::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = (PBYTE)&m_nType;			break;
	case 1:	pSrc = (PBYTE)&m_dwMapEventID;	break;
	case 2:	pSrc = (PBYTE)&m_ptPos;			break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventBase::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = (PBYTE)&m_nType;			dwSize = sizeof (m_nType);			break;
	case 1: pDst = (PBYTE)&m_dwMapEventID;	dwSize = sizeof (m_dwMapEventID);	break;
	case 2:	pDst = (PBYTE)&m_ptPos;			dwSize = sizeof (m_ptPos);			break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

DWORD CInfoMapEventBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_nType)		+
			sizeof (m_dwMapEventID)	+
			sizeof (m_ptPos);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventBase::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_nType,		sizeof (m_nType), 			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwMapEventID,	sizeof (m_dwMapEventID), 	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptPos,		sizeof (m_ptPos),			pDataTmp);

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventBase::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_nType,			pDataTmp, sizeof (m_nType), 		pDataTmp);
	CopyMemoryRenew (&m_dwMapEventID,	pDataTmp, sizeof (m_dwMapEventID), 	pDataTmp);
	CopyMemoryRenew (&m_ptPos,			pDataTmp, sizeof (m_ptPos),			pDataTmp);

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventBase::Copy										 */
/* 内容		:コピー															 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

void CInfoMapEventBase::Copy(CInfoMapEventBase *pSrc)
{
	if (pSrc == NULL) {
		return;
	}
	m_nType			= pSrc->m_nType;
	m_dwMapEventID	= pSrc->m_dwMapEventID;
	m_ptPos			= pSrc->m_ptPos;
}

/* Copyright(C)URARA-works 2007 */
