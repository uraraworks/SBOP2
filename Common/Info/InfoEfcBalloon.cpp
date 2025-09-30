/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoEfcBalloon.cpp											 */
/* 内容			:噴出し情報クラス 実装ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoEfcBalloon.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_byWait",				/* 待ち時間(×１０ミリ秒) */
	"m_bLoop",				/* ループ再生 */
	"m_dwEfcBalloonID",		/* 噴出しID */
	"m_dwListID",			/* 噴出し種別ID */
	"m_dwAnimeID",			/* コマ番号 */
	"m_dwSoundID",			/* 効果音ID */
	"m_dwGrpID",			/* 画像ID*/
	"m_strName",			/* 噴出し名 */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoEfcBalloon::CInfoEfcBalloon								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CInfoEfcBalloon::CInfoEfcBalloon()
{
	m_byWait			= 0;
	m_bLoop				= 0;
	m_dwEfcBalloonID	= 0;
	m_dwListID			= 0;
	m_dwAnimeID			= 0;
	m_dwSoundID			= 0;
	m_dwGrpID			= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoEfcBalloon::~CInfoEfcBalloon								 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CInfoEfcBalloon::~CInfoEfcBalloon()
{
}


/* ========================================================================= */
/* 関数名	:CInfoEfcBalloon::GetElementNo									 */
/* 内容		:要素番号を取得													 */
/* 日付		:2005/05/01														 */
/* ========================================================================= */

int CInfoEfcBalloon::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoEfcBalloon::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

DWORD CInfoEfcBalloon::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_byWait)				+	/* 待ち時間(×１０ミリ秒) */
			sizeof (m_bLoop)				+	/* ループ再生 */
			sizeof (m_dwEfcBalloonID)		+	/* 噴出しID */
			sizeof (m_dwListID)				+	/* 噴出し種別ID */
			sizeof (m_dwAnimeID)			+	/* コマ番号 */
			sizeof (m_dwSoundID)			+	/* 効果音ID */
			(m_strName.GetStoreLength () + 1)	+	/* 画像ID */
			sizeof (m_dwGrpID); 			 	/* 噴出し名 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoEfcBalloon::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

DWORD CInfoEfcBalloon::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_byWait);				break;	/* 待ち時間(×１０ミリ秒) */
	case 1:		dwRet = sizeof (m_bLoop);				break;	/* ループ再生 */
	case 2:		dwRet = sizeof (m_dwEfcBalloonID);		break;	/* 噴出しID */
	case 3:		dwRet = sizeof (m_dwListID);			break;	/* 噴出し種別ID */
	case 4:		dwRet = sizeof (m_dwAnimeID);			break;	/* コマ番号 */
	case 5:		dwRet = sizeof (m_dwSoundID);			break;	/* 効果音ID */
	case 6:		dwRet = sizeof (m_dwGrpID);				break;	/* 画像ID */
	case 7:		dwRet = (m_strName.GetStoreLength () + 1);	break;	/* 噴出し名 */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoEfcBalloon::GetName										 */
/* 内容		:要素名を取得													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

LPCSTR CInfoEfcBalloon::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoEfcBalloon::GetWriteData									 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

PBYTE CInfoEfcBalloon::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = &m_byWait;					break;		/* 待ち時間(×１０ミリ秒) */
	case 1:		pSrc = (PBYTE)&m_bLoop;				break;		/* ループ再生 */
	case 2:		pSrc = (PBYTE)&m_dwEfcBalloonID;	break;		/* 噴出しID */
	case 3:		pSrc = (PBYTE)&m_dwListID;			break;		/* 噴出し種別ID */
	case 4:		pSrc = (PBYTE)&m_dwAnimeID;			break;		/* コマ番号 */
	case 5:		pSrc = (PBYTE)&m_dwSoundID;			break;		/* 効果音ID */
	case 6:		pSrc = (PBYTE)&m_dwGrpID;			break;		/* 画像ID */
	case 7:		pSrc = (PBYTE)(LPCSTR)m_strName;	break;		/* 噴出し名 */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoEfcBalloon::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

DWORD CInfoEfcBalloon::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = &m_byWait;					dwSize = sizeof (m_byWait);			break;	/* 待ち時間(×１０ミリ秒) */
	case 1:		pDst = (PBYTE)&m_bLoop;				dwSize = sizeof (m_bLoop);			break;	/* ループ再生 */
	case 2:		pDst = (PBYTE)&m_dwEfcBalloonID;	dwSize = sizeof (m_dwEfcBalloonID);	break;	/* 噴出しID */
	case 3:		pDst = (PBYTE)&m_dwListID;			dwSize = sizeof (m_dwListID);		break;	/* 噴出し種別ID */
	case 4:		pDst = (PBYTE)&m_dwAnimeID;			dwSize = sizeof (m_dwAnimeID);		break;	/* コマ番号 */
	case 5:		pDst = (PBYTE)&m_dwSoundID;			dwSize = sizeof (m_dwSoundID);		break;	/* 効果音ID */
	case 6:		pDst = (PBYTE)&m_dwGrpID;			dwSize = sizeof (m_dwGrpID);		break;	/* 画像ID */
	case 7: 																				  	/* 噴出し名 */
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
/* 関数名	:CInfoEfcBalloon::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

DWORD CInfoEfcBalloon::GetSendDataSize(void)
{
	return GetDataSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoEfcBalloon::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

PBYTE CInfoEfcBalloon::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwListID,			sizeof (m_dwListID),		pDataTmp);	/* 噴出し種別ID */
	CopyMemoryRenew (pDataTmp, &m_byWait,			sizeof (m_byWait),			pDataTmp);	/* 待ち時間(×１０ミリ秒) */
	CopyMemoryRenew (pDataTmp, &m_bLoop,			sizeof (m_bLoop),			pDataTmp);	/* ループ再生 */
	CopyMemoryRenew (pDataTmp, &m_dwEfcBalloonID,	sizeof (m_dwEfcBalloonID),	pDataTmp);	/* 噴出しID */
	CopyMemoryRenew (pDataTmp, &m_dwAnimeID,		sizeof (m_dwAnimeID),		pDataTmp);	/* コマ番号 */
	CopyMemoryRenew (pDataTmp, &m_dwSoundID,		sizeof (m_dwSoundID),		pDataTmp);	/* 効果音ID */
	CopyMemoryRenew (pDataTmp, &m_dwGrpID,			sizeof (m_dwGrpID),			pDataTmp);	/* 画像ID */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp); 								  	/* 噴出し名 */

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoEfcBalloon::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

PBYTE CInfoEfcBalloon::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;

	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwListID,		pDataTmp, sizeof (m_dwListID),			pDataTmp);	/* 噴出し種別ID */
	CopyMemoryRenew (&m_byWait,			pDataTmp, sizeof (m_byWait),			pDataTmp);	/* 待ち時間(×１０ミリ秒) */
	CopyMemoryRenew (&m_bLoop,			pDataTmp, sizeof (m_bLoop),				pDataTmp);	/* ループ再生 */
	CopyMemoryRenew (&m_dwEfcBalloonID,	pDataTmp, sizeof (m_dwEfcBalloonID),	pDataTmp);	/* 噴出しID */
	CopyMemoryRenew (&m_dwAnimeID,		pDataTmp, sizeof (m_dwAnimeID),			pDataTmp);	/* コマ番号 */
	CopyMemoryRenew (&m_dwSoundID,		pDataTmp, sizeof (m_dwSoundID),			pDataTmp);	/* 効果音ID */
	CopyMemoryRenew (&m_dwGrpID,		pDataTmp, sizeof (m_dwGrpID),			pDataTmp);	/* 画像ID */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp); 								  	/* 噴出し名 */

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CInfoEfcBalloon::Copy											 */
/* 内容		:コピー															 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CInfoEfcBalloon::Copy(CInfoEfcBalloon *pSrc)
{
	m_byWait			= pSrc->m_byWait;			/* 待ち時間(×１０ミリ秒) */
	m_bLoop				= pSrc->m_bLoop;			/* ループ再生 */
	m_dwEfcBalloonID	= pSrc->m_dwEfcBalloonID;	/* 噴出しID */
	m_dwListID			= pSrc->m_dwListID;			/* 噴出し種別ID */
	m_dwAnimeID			= pSrc->m_dwAnimeID;		/* コマ番号 */
	m_dwSoundID			= pSrc->m_dwSoundID;		/* 効果音ID */
	m_dwGrpID			= pSrc->m_dwGrpID;			/* 画像ID */
	m_strName			= pSrc->m_strName;			/* 噴出し名 */
}

/* Copyright(C)URARA-works 2007 */
