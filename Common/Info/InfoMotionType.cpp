/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMotionType.cpp											 */
/* 内容			:モーション種別情報クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotionType.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwMotionTypeID",		/* モーション種別ID */
	"m_wGrpIDSub",			/* プレビュー用グラフィックIDサブ */
	"m_strName",			/* モーション種別名 */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoMotionType::CInfoMotionType								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

CInfoMotionType::CInfoMotionType()
{
	m_wGrpIDSub		 = 0;
	m_dwMotionTypeID = 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoMotionType::~CInfoMotionType								 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

CInfoMotionType::~CInfoMotionType()
{
}


/* ========================================================================= */
/* 関数名	:CInfoMotionType::GetElementNo									 */
/* 内容		:要素番号を取得													 */
/* 日付		:2005/05/01														 */
/* ========================================================================= */

int CInfoMotionType::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoMotionType::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

DWORD CInfoMotionType::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwMotionTypeID)	+
			sizeof (m_wGrpIDSub)		+
			(m_strName.GetStoreLength () + 1);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMotionType::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

DWORD CInfoMotionType::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwMotionTypeID);		break;		/* モーション種別ID */
	case 1:		dwRet = sizeof (m_wGrpIDSub);			break;		/* プレビュー用グラフィックIDサブ */
	case 2:		dwRet = (m_strName.GetStoreLength () + 1);	break;		/* モーション種別名 */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMotionType::GetName										 */
/* 内容		:要素名を取得													 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

LPCSTR CInfoMotionType::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoMotionType::GetWriteData									 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

PBYTE CInfoMotionType::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = (PBYTE)&m_dwMotionTypeID;	break;		/* モーション種別ID */
	case 1:		pSrc = (PBYTE)&m_wGrpIDSub;			break;		/* プレビュー用グラフィックIDサブ */
	case 2:		pSrc = (PBYTE)(LPCSTR)m_strName;	break;		/* モーション種別名 */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMotionType::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

DWORD CInfoMotionType::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwMotionTypeID;	dwSize = sizeof (m_dwMotionTypeID);	break;		/* モーション種別ID */
	case 1:		pDst = (PBYTE)&m_wGrpIDSub;			dwSize = sizeof (m_wGrpIDSub);		break;		/* プレビュー用グラフィックIDサブ */
	case 2:		/* モーション種別名 */
		m_strName = (LPCTSTR)AnsiToTString ((LPCSTR)pSrc, 932);
		dwSize = (DWORD)(strlen ((LPCSTR)pSrc) + 1);
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoMotionType::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

DWORD CInfoMotionType::GetSendDataSize(void)
{
	return GetDataSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoMotionType::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

PBYTE CInfoMotionType::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwMotionTypeID,	sizeof (m_dwMotionTypeID),	pDataTmp);	/* モーション種別ID */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDSub,		sizeof (m_wGrpIDSub),		pDataTmp);	/* プレビュー用グラフィックIDサブ */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);										/* モーション種別名 */

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoMotionType::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/05/26														 */
/* ========================================================================= */

PBYTE CInfoMotionType::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;

	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwMotionTypeID, pDataTmp, sizeof (m_dwMotionTypeID), pDataTmp);	/* モーション種別ID */
	CopyMemoryRenew (&m_wGrpIDSub,		pDataTmp, sizeof (m_wGrpIDSub),		 pDataTmp);	/* プレビュー用グラフィックIDサブ */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);									/* モーション種別名 */

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CInfoMotionType::Copy											 */
/* 内容		:コピー															 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

void CInfoMotionType::Copy(CInfoMotionType *pSrc)
{
	m_dwMotionTypeID	= pSrc->m_dwMotionTypeID;	/* モーション種別ID */
	m_wGrpIDSub			= pSrc->m_wGrpIDSub;		/* プレビュー用グラフィックIDサブ */
	m_strName			= pSrc->m_strName;			/* モーション種別名 */
}

/* Copyright(C)URARA-works 2008 */
