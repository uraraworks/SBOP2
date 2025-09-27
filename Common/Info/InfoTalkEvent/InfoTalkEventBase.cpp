/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoTalkEventBase.cpp										 */
/* 内容			:会話イベント情報基底クラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/18													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoTalkEventBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_nEventType",					/* 会話イベント種別 */
	"m_nPage",						/* 所属ページ番号 */
	"m_dwData",						/* バイナリデータ */
	"m_strText",					/* 文字列データ */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::CInfoTalkEventBase							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/18														 */
/* ========================================================================= */

CInfoTalkEventBase::CInfoTalkEventBase()
{
	m_nEventType		= TALKEVENTTYPE_NONE;
	m_nPage				= 0;
	m_dwData			= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::~CInfoTalkEventBase						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/18														 */
/* ========================================================================= */

CInfoTalkEventBase::~CInfoTalkEventBase()
{
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::GetElementNo								 */
/* 内容		:要素番号を取得													 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

int CInfoTalkEventBase::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoTalkEventBase::GetDataSize								 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/12/18														 */
/* ========================================================================= */

DWORD CInfoTalkEventBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_nEventType);					/* 会話イベント種別 */
	dwRet += sizeof (m_nPage);						/* 所属ページ番号 */
	dwRet += sizeof (m_dwData);						/* バイナリデータ */
	dwRet += (m_strText.GetUtf8Length () + 1);			/* 文字列データ */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::GetDataSizeNo								 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/12/18														 */
/* ========================================================================= */

DWORD CInfoTalkEventBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_nEventType);			break;	/* 会話イベント種別 */
	case 1:	dwRet = sizeof (m_nPage);				break;	/* 所属ページ番号 */
	case 2:	dwRet = sizeof (m_dwData);				break;	/* バイナリデータ */
	case 3:	dwRet = m_strText.GetUtf8Length () + 1;		break;	/* 文字列データ */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::GetName									 */
/* 内容		:要素名を取得													 */
/* 日付		:2005/07/05														 */
/* ========================================================================= */

LPCSTR CInfoTalkEventBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::GetWriteData								 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/12/18														 */
/* ========================================================================= */

PBYTE CInfoTalkEventBase::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = (PBYTE)&m_nEventType;			break;		/* 会話イベント種別 */
	case 1:	pSrc = (PBYTE)&m_nPage;					break;		/* 所属ページ番号 */
	case 2:	pSrc = (PBYTE)&m_dwData;				break;		/* バイナリデータ */
	case 3:	pSrc = (PBYTE)(LPCSTR)m_strText;		break;		/* 文字列データ */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::ReadElementData							 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/12/18														 */
/* ========================================================================= */

DWORD CInfoTalkEventBase::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = (PBYTE)&m_nEventType;		dwSize = sizeof (m_nEventType);			break;		/* 会話イベント種別 */
	case 1: pDst = (PBYTE)&m_nPage;				dwSize = sizeof (m_nPage);				break;		/* 所属ページ番号 */
	case 2: pDst = (PBYTE)&m_dwData;			dwSize = sizeof (m_dwData);				break;		/* バイナリデータ */
	case 3:				/* 文字列データ */
		m_strText = (LPCSTR)pSrc;
		dwSize = (DWORD)(strlen ((LPCSTR)pSrc) + 1);
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/12/18														 */
/* ========================================================================= */

DWORD CInfoTalkEventBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_nEventType);					/* 会話イベント種別 */
	dwRet += sizeof (m_nPage);						/* 所属ページ番号 */
	dwRet += sizeof (m_dwData);						/* バイナリデータ */
	dwRet += (m_strText.GetUtf8Length () + 1);			/* 文字列データ */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/12/18														 */
/* ========================================================================= */

PBYTE CInfoTalkEventBase::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_nEventType,			sizeof (m_nEventType),			pDataTmp);		/* 会話イベント種別 */
	CopyMemoryRenew (pDataTmp, &m_nPage,				sizeof (m_nPage),				pDataTmp);		/* 所属ページ番号 */
	CopyMemoryRenew (pDataTmp, &m_dwData,				sizeof (m_dwData),				pDataTmp);		/* バイナリデータ */
	strcpyRenew ((LPSTR)pDataTmp, m_strText, pDataTmp);					/* 文字列データ */

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/12/18														 */
/* ========================================================================= */

PBYTE CInfoTalkEventBase::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_nEventType,			pDataTmp, sizeof (m_nEventType),		pDataTmp);		/* 会話イベント種別 */
	CopyMemoryRenew (&m_nPage,				pDataTmp, sizeof (m_nPage),				pDataTmp);		/* 所属ページ番号 */
	CopyMemoryRenew (&m_dwData,				pDataTmp, sizeof (m_dwData),			pDataTmp);		/* バイナリデータ */
	StoreRenew (m_strText, (LPCSTR)pDataTmp, pDataTmp);					/* 文字列データ */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoTalkEventBase::Copy										 */
/* 内容		:コピー															 */
/* 日付		:2008/12/18														 */
/* ========================================================================= */

void CInfoTalkEventBase::Copy(CInfoTalkEventBase *pSrc)
{
	if (pSrc == NULL) {
		return;
	}

	m_nEventType		= pSrc->m_nEventType;			/* 会話イベント種別 */
	m_nPage				= pSrc->m_nPage;				/* 所属ページ番号 */
	m_dwData			= pSrc->m_dwData;				/* バイナリデータ */
	m_strText			= pSrc->m_strText;				/* 文字列データ */
}

/* Copyright(C)URARA-works 2008 */
