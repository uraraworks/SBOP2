/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoMotion.cpp												 */
/* 内容			:モーション情報クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotion.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwMotionID",			/* モーションID */
	"m_dwMotionTypeID",		/* モーション種別ID */
	"m_dwMotionListID",		/* モーションリストID */
	"m_byWait",				/* 待ち時間(×１０ミリ秒) */
	"m_byLevel1",			/* 透明度1 */
	"m_byLevel2",			/* 透明度2 */
	"m_byLevel3",			/* 透明度3 */
	"m_bPile",				/* 重ね画像を先に描画 */
	"m_bRedrawHand",		/* 手を手前に描画 */
	"m_wGrpIDMainBase",		/* グラフィックIDメイン(下地) */
	"m_wGrpIDMainPile1",	/* グラフィックIDメイン(重ね合わせ1) */
	"m_wGrpIDMainPile2",	/* グラフィックIDメイン(重ね合わせ2) */
	"m_wGrpIDMainPile3",	/* グラフィックIDメイン(重ね合わせ3) */
	"m_wGrpIDSubBase",		/* グラフィックIDサブ(下地) */
	"m_wGrpIDSubPile1",		/* グラフィックIDサブ(重ね合わせ1) */
	"m_wGrpIDSubPile2",		/* グラフィックIDサブ(重ね合わせ2) */
	"m_wGrpIDSubPile3",		/* グラフィックIDサブ(重ね合わせ3) */
	"m_dwSoundID",			/* 効果音ID */
	"m_bLoop",				/* ループ再生 */
	"m_dwProcID",			/* 行動ID */
	"m_ptDrawPos0",			/* 描画位置(下地) */
	"m_ptDrawPos1",			/* 描画位置(重ね合わせ1) */
	"m_ptDrawPos2",			/* 描画位置(重ね合わせ2) */
	"m_ptDrawPos3",			/* 描画位置(重ね合わせ3) */
	"nDrawListCount",		/* 描画順リスト数 */
	"m_anDrawList",			/* 描画順リスト */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoMotion::CInfoMotion										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

CInfoMotion::CInfoMotion()
{
	m_dwMotionID		= 0;
	m_dwMotionTypeID	= 0;
	m_dwMotionListID	= 0;
	m_byWait			= 0;
	m_byLevel1			= 0;
	m_byLevel2			= 0;
	m_byLevel3			= 0;
	m_bPile				= FALSE;
	m_bRedrawHand		= FALSE;
	m_bLoop				= FALSE;
	m_wGrpIDMainBase	= 0;
	m_wGrpIDMainPile1	= 0;
	m_wGrpIDMainPile2	= 0;
	m_wGrpIDMainPile3	= 0;
	m_wGrpIDSubBase		= 0;
	m_wGrpIDSubPile1	= 0;
	m_wGrpIDSubPile2	= 0;
	m_wGrpIDSubPile3	= 0;
	m_dwSoundID			= 0;
	m_dwProcID			= CHARMOTIONPROCID_NONE;
	ZeroMemory (&m_ptDrawPosPile0, sizeof (m_ptDrawPosPile0));
	ZeroMemory (&m_ptDrawPosPile1, sizeof (m_ptDrawPosPile1));
	ZeroMemory (&m_ptDrawPosPile2, sizeof (m_ptDrawPosPile2));
	ZeroMemory (&m_ptDrawPosPile3, sizeof (m_ptDrawPosPile3));

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoMotion::~CInfoMotion										 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

CInfoMotion::~CInfoMotion()
{
}


/* ========================================================================= */
/* 関数名	:CInfoMapParts::GetElementCount									 */
/* 内容		:要素数を取得													 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

int CInfoMotion::GetElementCount(void)
{
	return m_nElementCount;
}


/* ========================================================================= */
/* 関数名	:CInfoMotion::GetElementNo										 */
/* 内容		:要素番号を取得													 */
/* 日付		:2005/05/01														 */
/* ========================================================================= */

int CInfoMotion::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoMotion::GetDataSize										 */
/* 内容		:データサイズを取得												 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

DWORD CInfoMotion::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwMotionID)		+
			sizeof (m_dwMotionTypeID)	+
			sizeof (m_dwMotionListID)	+
			sizeof (m_byWait)			+
			sizeof (m_byLevel1)			+
			sizeof (m_byLevel2)			+
			sizeof (m_byLevel3)			+
			sizeof (m_bPile)			+
			sizeof (m_bRedrawHand)		+
			sizeof (m_bLoop)			+
			sizeof (m_wGrpIDMainBase)	+
			sizeof (m_wGrpIDMainPile1)	+
			sizeof (m_wGrpIDMainPile2)	+
			sizeof (m_wGrpIDMainPile3)	+
			sizeof (m_wGrpIDSubBase)	+
			sizeof (m_wGrpIDSubPile1)	+
			sizeof (m_wGrpIDSubPile2)	+
			sizeof (m_wGrpIDSubPile3)	+
			sizeof (m_dwSoundID)		+
			sizeof (m_dwProcID)			+
			sizeof (m_ptDrawPosPile0)	+
			sizeof (m_ptDrawPosPile1)	+
			sizeof (m_ptDrawPosPile3)	+
			sizeof (m_ptDrawPosPile2)	+
			sizeof (int)				+
			(sizeof (int) * m_anDrawList.GetSize ());

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMotion::GetDataSizeNo										 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

DWORD CInfoMotion::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwMotionID);		break;
	case 1:		dwRet = sizeof (m_dwMotionTypeID);	break;
	case 2:		dwRet = sizeof (m_dwMotionListID);	break;
	case 3:		dwRet = sizeof (m_byWait);			break;
	case 4:		dwRet = sizeof (m_byLevel1);		break;
	case 5:		dwRet = sizeof (m_byLevel2);		break;
	case 6:		dwRet = sizeof (m_byLevel3);		break;
	case 7:		dwRet = sizeof (m_bPile);			break;
	case 8:		dwRet = sizeof (m_bRedrawHand);		break;
	case 9:		dwRet = sizeof (m_wGrpIDMainBase);	break;
	case 10:	dwRet = sizeof (m_wGrpIDMainPile1);	break;
	case 11:	dwRet = sizeof (m_wGrpIDMainPile2);	break;
	case 12:	dwRet = sizeof (m_wGrpIDMainPile3);	break;
	case 13:	dwRet = sizeof (m_wGrpIDSubBase);	break;
	case 14:	dwRet = sizeof (m_wGrpIDSubPile1);	break;
	case 15:	dwRet = sizeof (m_wGrpIDSubPile2);	break;
	case 16:	dwRet = sizeof (m_wGrpIDSubPile3);	break;
	case 17:	dwRet = sizeof (m_dwSoundID);		break;
	case 18:	dwRet = sizeof (m_bLoop);			break;
	case 19:	dwRet = sizeof (m_dwProcID);		break;
	case 20:	dwRet = sizeof (m_ptDrawPosPile0);	break;
	case 21:	dwRet = sizeof (m_ptDrawPosPile1);	break;
	case 22:	dwRet = sizeof (m_ptDrawPosPile2);	break;
	case 23:	dwRet = sizeof (m_ptDrawPosPile3);	break;
	case 24:	dwRet = sizeof (int);				break;
	case 25:	dwRet = sizeof (int) * m_anDrawList.GetSize ();	break;
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMotion::GetName											 */
/* 内容		:要素名を取得													 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

LPCSTR CInfoMotion::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoMotion::GetWriteData										 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

PBYTE CInfoMotion::GetWriteData(int nNo, PDWORD pdwSize)
{
	int nTmp, i, nCount;
	PBYTE pRet, pSrc, pTmp;
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
	case 0:		pSrc = (PBYTE)&m_dwMotionID;		break;
	case 1:		pSrc = (PBYTE)&m_dwMotionTypeID;	break;
	case 2:		pSrc = (PBYTE)&m_dwMotionListID;	break;
	case 3:		pSrc = &m_byWait;					break;
	case 4:		pSrc = &m_byLevel1;					break;
	case 5:		pSrc = &m_byLevel2;					break;
	case 6:		pSrc = &m_byLevel3;					break;
	case 7:		pSrc = (PBYTE)&m_bPile;				break;
	case 8:		pSrc = (PBYTE)&m_bRedrawHand;		break;
	case 9:		pSrc = (PBYTE)&m_wGrpIDMainBase;	break;
	case 10:	pSrc = (PBYTE)&m_wGrpIDMainPile1;	break;
	case 11:	pSrc = (PBYTE)&m_wGrpIDMainPile2;	break;
	case 12:	pSrc = (PBYTE)&m_wGrpIDMainPile3;	break;
	case 13:	pSrc = (PBYTE)&m_wGrpIDSubBase;		break;
	case 14:	pSrc = (PBYTE)&m_wGrpIDSubPile1;	break;
	case 15:	pSrc = (PBYTE)&m_wGrpIDSubPile2;	break;
	case 16:	pSrc = (PBYTE)&m_wGrpIDSubPile3;	break;
	case 17:	pSrc = (PBYTE)&m_dwSoundID;			break;
	case 18:	pSrc = (PBYTE)&m_bLoop;				break;
	case 19:	pSrc = (PBYTE)&m_dwProcID;			break;
	case 20:	pSrc = (PBYTE)&m_ptDrawPosPile0;	break;
	case 21:	pSrc = (PBYTE)&m_ptDrawPosPile1;	break;
	case 22:	pSrc = (PBYTE)&m_ptDrawPosPile2;	break;
	case 23:	pSrc = (PBYTE)&m_ptDrawPosPile3;	break;
	case 24:
		nTmp = m_anDrawList.GetSize ();
		pSrc = (PBYTE)&nTmp;
		break;
	case 25:
		pTmp = pRet;
		nCount = m_anDrawList.GetSize ();
		for (i = 0; i < nCount; i ++) {
			nTmp = m_anDrawList[i];
			CopyMemoryRenew (pTmp, &nTmp, sizeof (nTmp), pTmp);
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
/* 関数名	:CInfoMotion::ReadElementData									 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

DWORD CInfoMotion::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	int i, nCount, nTmp;
	PBYTE pDst, pTmp;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwMotionID;		dwSize = sizeof (m_dwMotionID);			break;
	case 1:		pDst = (PBYTE)&m_dwMotionTypeID;	dwSize = sizeof (m_dwMotionTypeID);		break;
	case 2:		pDst = (PBYTE)&m_dwMotionListID;	dwSize = sizeof (m_dwMotionListID);		break;
	case 3:		pDst = &m_byWait;					dwSize = sizeof (m_byWait);				break;
	case 4:		pDst = &m_byLevel1;					dwSize = sizeof (m_byLevel1);			break;
	case 5:		pDst = &m_byLevel2;					dwSize = sizeof (m_byLevel2);			break;
	case 6:		pDst = &m_byLevel3;					dwSize = sizeof (m_byLevel3);			break;
	case 7:		pDst = (PBYTE)&m_bPile;				dwSize = sizeof (m_bPile);				break;
	case 8:		pDst = (PBYTE)&m_bRedrawHand;		dwSize = sizeof (m_bRedrawHand);		break;
	case 9:		pDst = (PBYTE)&m_wGrpIDMainBase;	dwSize = sizeof (m_wGrpIDMainBase);		break;
	case 10:	pDst = (PBYTE)&m_wGrpIDMainPile1;	dwSize = sizeof (m_wGrpIDMainPile1);	break;
	case 11:	pDst = (PBYTE)&m_wGrpIDMainPile2;	dwSize = sizeof (m_wGrpIDMainPile2);	break;
	case 12:	pDst = (PBYTE)&m_wGrpIDMainPile3;	dwSize = sizeof (m_wGrpIDMainPile3);	break;
	case 13:	pDst = (PBYTE)&m_wGrpIDSubBase;		dwSize = sizeof (m_wGrpIDSubBase);		break;
	case 14:	pDst = (PBYTE)&m_wGrpIDSubPile1;	dwSize = sizeof (m_wGrpIDSubPile1);		break;
	case 15:	pDst = (PBYTE)&m_wGrpIDSubPile2;	dwSize = sizeof (m_wGrpIDSubPile2);		break;
	case 16:	pDst = (PBYTE)&m_wGrpIDSubPile3;	dwSize = sizeof (m_wGrpIDSubPile3);		break;
	case 17:	pDst = (PBYTE)&m_dwSoundID;			dwSize = sizeof (m_dwSoundID);			break;
	case 18:	pDst = (PBYTE)&m_bLoop;				dwSize = sizeof (m_bLoop);				break;
	case 19:	pDst = (PBYTE)&m_dwProcID;			dwSize = sizeof (m_dwProcID);			break;
	case 20:	pDst = (PBYTE)&m_ptDrawPosPile0;	dwSize = sizeof (m_ptDrawPosPile0);		break;
	case 21:	pDst = (PBYTE)&m_ptDrawPosPile1;	dwSize = sizeof (m_ptDrawPosPile1);		break;
	case 22:	pDst = (PBYTE)&m_ptDrawPosPile2;	dwSize = sizeof (m_ptDrawPosPile2);		break;
	case 23:	pDst = (PBYTE)&m_ptDrawPosPile3;	dwSize = sizeof (m_ptDrawPosPile3);		break;
	case 24:
		m_anDrawList.RemoveAll ();
		CopyMemory (&nCount, pSrc, sizeof (nCount));
		dwSize = sizeof (nCount);
		for (i = 0; i < nCount; i ++) {
			m_anDrawList.Add (0);
		}
		break;
	case 25:
		pTmp = pSrc;
		nCount = m_anDrawList.GetSize ();
		for (i = 0; i < nCount; i ++) {
			CopyMemoryRenew (&nTmp, pTmp, sizeof (nTmp), pTmp);
			m_anDrawList[i] = nTmp;
		}
		dwSize = sizeof (int) * nCount;
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
#if 0
		switch (nNo) {
		case 14:
			if (m_wGrpIDMainPile1==16) m_wGrpIDSubPile1 += 2;
			break;
		case 15:
			if (m_wGrpIDMainPile2==16) m_wGrpIDSubPile2 += 2;
			break;
		case 16:
			if (m_wGrpIDMainPile3==16) m_wGrpIDSubPile3 += 2;
			break;
		}
#endif
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoMotion::GetSendDataSize									 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

DWORD CInfoMotion::GetSendDataSize(void)
{
	return GetDataSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoMotion::GetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

PBYTE CInfoMotion::GetSendData(void)
{
	int i, nCount, nTmp;
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwMotionID,		sizeof (m_dwMotionID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwMotionTypeID,	sizeof (m_dwMotionTypeID),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwMotionListID,	sizeof (m_dwMotionListID),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byWait,			sizeof (m_byWait),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byLevel1,			sizeof (m_byLevel1),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byLevel2,			sizeof (m_byLevel2),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byLevel3,			sizeof (m_byLevel3),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_bPile,			sizeof (m_bPile),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_bRedrawHand,		sizeof (m_bRedrawHand),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_bLoop,			sizeof (m_bLoop),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDMainBase,	sizeof (m_wGrpIDMainBase),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDMainPile1,	sizeof (m_wGrpIDMainPile1),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDMainPile2,	sizeof (m_wGrpIDMainPile2),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDMainPile3,	sizeof (m_wGrpIDMainPile3),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDSubBase,	sizeof (m_wGrpIDSubBase),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDSubPile1,	sizeof (m_wGrpIDSubPile1),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDSubPile2,	sizeof (m_wGrpIDSubPile2),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDSubPile3,	sizeof (m_wGrpIDSubPile3),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwSoundID,		sizeof (m_dwSoundID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwProcID,			sizeof (m_dwProcID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptDrawPosPile0,	sizeof (m_ptDrawPosPile0),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptDrawPosPile1,	sizeof (m_ptDrawPosPile1),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptDrawPosPile2,	sizeof (m_ptDrawPosPile2),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptDrawPosPile3,	sizeof (m_ptDrawPosPile3),	pDataTmp);

	nCount = m_anDrawList.GetSize ();
	CopyMemoryRenew (pDataTmp, &nCount, sizeof (nCount), pDataTmp);
	for (i = 0; i < nCount; i ++) {
		nTmp = m_anDrawList[i];
		CopyMemoryRenew (pDataTmp, &nTmp, sizeof (nTmp), pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoMotion::SetSendData										 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/10/29														 */
/* ========================================================================= */

PBYTE CInfoMotion::SetSendData(PBYTE pSrc)
{
	int i, nCount, nTmp;
	PBYTE pDataTmp;

	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwMotionID,			pDataTmp, sizeof (m_dwMotionID),		pDataTmp);
	CopyMemoryRenew (&m_dwMotionTypeID,		pDataTmp, sizeof (m_dwMotionTypeID),	pDataTmp);
	CopyMemoryRenew (&m_dwMotionListID,		pDataTmp, sizeof (m_dwMotionListID),	pDataTmp);
	CopyMemoryRenew (&m_byWait,				pDataTmp, sizeof (m_byWait),			pDataTmp);
	CopyMemoryRenew (&m_byLevel1,			pDataTmp, sizeof (m_byLevel1),			pDataTmp);
	CopyMemoryRenew (&m_byLevel2,			pDataTmp, sizeof (m_byLevel2),			pDataTmp);
	CopyMemoryRenew (&m_byLevel3,			pDataTmp, sizeof (m_byLevel3),			pDataTmp);
	CopyMemoryRenew (&m_bPile,				pDataTmp, sizeof (m_bPile),				pDataTmp);
	CopyMemoryRenew (&m_bRedrawHand,		pDataTmp, sizeof (m_bRedrawHand),		pDataTmp);
	CopyMemoryRenew (&m_bLoop,				pDataTmp, sizeof (m_bLoop),				pDataTmp);
	CopyMemoryRenew (&m_wGrpIDMainBase,		pDataTmp, sizeof (m_wGrpIDMainBase),	pDataTmp);
	CopyMemoryRenew (&m_wGrpIDMainPile1,	pDataTmp, sizeof (m_wGrpIDMainPile1),	pDataTmp);
	CopyMemoryRenew (&m_wGrpIDMainPile2,	pDataTmp, sizeof (m_wGrpIDMainPile2),	pDataTmp);
	CopyMemoryRenew (&m_wGrpIDMainPile3,	pDataTmp, sizeof (m_wGrpIDMainPile3),	pDataTmp);
	CopyMemoryRenew (&m_wGrpIDSubBase,		pDataTmp, sizeof (m_wGrpIDSubBase),		pDataTmp);
	CopyMemoryRenew (&m_wGrpIDSubPile1,		pDataTmp, sizeof (m_wGrpIDSubPile1),	pDataTmp);
	CopyMemoryRenew (&m_wGrpIDSubPile2,		pDataTmp, sizeof (m_wGrpIDSubPile2),	pDataTmp);
	CopyMemoryRenew (&m_wGrpIDSubPile3,		pDataTmp, sizeof (m_wGrpIDSubPile3),	pDataTmp);
	CopyMemoryRenew (&m_dwSoundID,			pDataTmp, sizeof (m_dwSoundID),			pDataTmp);
	CopyMemoryRenew (&m_dwProcID,			pDataTmp, sizeof (m_dwProcID),			pDataTmp);
	CopyMemoryRenew (&m_ptDrawPosPile0,		pDataTmp, sizeof (m_ptDrawPosPile0),	pDataTmp);
	CopyMemoryRenew (&m_ptDrawPosPile1,		pDataTmp, sizeof (m_ptDrawPosPile1),	pDataTmp);
	CopyMemoryRenew (&m_ptDrawPosPile2,		pDataTmp, sizeof (m_ptDrawPosPile2),	pDataTmp);
	CopyMemoryRenew (&m_ptDrawPosPile3,		pDataTmp, sizeof (m_ptDrawPosPile3),	pDataTmp);

	m_anDrawList.RemoveAll ();
	CopyMemoryRenew (&nCount, pDataTmp, sizeof (nCount), pDataTmp);
	for (i = 0; i < nCount; i ++) {
		CopyMemoryRenew (&nTmp, pDataTmp, sizeof (nTmp), pDataTmp);
		m_anDrawList.Add (nTmp);
	}

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CInfoMotion::Copy												 */
/* 内容		:コピー															 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

void CInfoMotion::Copy(CInfoMotion *pSrc)
{
	m_byWait			= pSrc->m_byWait;
	m_byLevel1			= pSrc->m_byLevel1;
	m_byLevel2			= pSrc->m_byLevel2;
	m_byLevel3			= pSrc->m_byLevel3;
	m_bPile				= pSrc->m_bPile;
	m_bRedrawHand		= pSrc->m_bRedrawHand;
	m_bLoop				= pSrc->m_bLoop;
	m_wGrpIDMainBase	= pSrc->m_wGrpIDMainBase;
	m_wGrpIDMainPile1	= pSrc->m_wGrpIDMainPile1;
	m_wGrpIDMainPile2	= pSrc->m_wGrpIDMainPile2;
	m_wGrpIDMainPile3	= pSrc->m_wGrpIDMainPile3;
	m_wGrpIDSubBase		= pSrc->m_wGrpIDSubBase;
	m_wGrpIDSubPile1	= pSrc->m_wGrpIDSubPile1;
	m_wGrpIDSubPile2	= pSrc->m_wGrpIDSubPile2;
	m_wGrpIDSubPile3	= pSrc->m_wGrpIDSubPile3;
	m_dwMotionID		= pSrc->m_dwMotionID;
	m_dwMotionTypeID	= pSrc->m_dwMotionTypeID;
	m_dwMotionListID	= pSrc->m_dwMotionListID;
	m_dwSoundID			= pSrc->m_dwSoundID;
	m_dwProcID			= pSrc->m_dwProcID;
	m_ptDrawPosPile0	= pSrc->m_ptDrawPosPile0;
	m_ptDrawPosPile1	= pSrc->m_ptDrawPosPile1;
	m_ptDrawPosPile2	= pSrc->m_ptDrawPosPile2;
	m_ptDrawPosPile3	= pSrc->m_ptDrawPosPile3;
	m_anDrawList.Copy (&pSrc->m_anDrawList);
}

/* Copyright(C)URARA-works 2007 */
