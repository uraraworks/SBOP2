/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoItemTypeBase.cpp										 */
/* 内容			:アイテム種別基底情報クラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotion.h"
#include "InfoItemTypeBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_byDelAverage",		/* 使ってなくなる確率 */
	"m_byTarget",			/* 使用対象 */
	"m_byArea",				/* 使用範囲 */
	"m_bPutOn",				/* 上に置けるか判定 */
	"m_dwTypeID",			/* 種別ID */
	"m_dwItemTypeID",		/* アイテム種別ID */
	"m_dwGrpID",			/* 地面にある時の画像ID */
	"m_dwIconGrpID",		/* バックパック内の画像ID */
	"m_strName",			/* アイテム名 */
	"m_dwDropSoundID",		/* 落ちたときの効果音ID */
	"m_wGrpIDMain",			/* 画像IDメイン */
	"m_wGrpIDSub",			/* 画像IDサブ */
	"m_dwUseEffectID",		/* 使った時に再生するエフェクトID */
	"m_dwUseSoundID",		/* 使った時に再生する効果音ID */
	"m_dwWeaponInfoID",		/* 武器情報ID */
	"m_dwValue",			/* 攻撃力 */
	"m_dwMoveWait",			/* 速度 */
	"m_dwMoveCount",		/* 飛距離 */
	"m_dwValue2",			/* 効果(最大) */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::CInfoItemTypeBase							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

CInfoItemTypeBase::CInfoItemTypeBase()
{
	m_byDelAverage	= 0;
	m_byTarget		= 0;				/* 使用対象 */
	m_byArea		= 0;				/* 使用範囲 */
	m_bPutOn		= FALSE;
	m_wGrpIDMain	= 0;
	m_wGrpIDSub		= 0;
	m_dwTypeID		= 0;
	m_dwItemTypeID	= ITEMTYPEID_NONE;
	m_dwGrpID		= 0;
	m_dwIconGrpID	= 0;
	m_dwDropSoundID	= 0;
	m_dwUseEffectID	= 0;
	m_dwUseSoundID	= 0;

	m_dwWeaponInfoID	= 0;
	m_dwValue			= 0;	/* 攻撃力 */
	m_dwMoveWait		= 0;	/* 速度 */
	m_dwMoveCount		= 0;	/* 飛距離 */
	m_dwValue2			= 0;	/* 効果(最大) */

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::~CInfoItemTypeBase							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

CInfoItemTypeBase::~CInfoItemTypeBase()
{
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::GetElementNo								 */
/* 内容		:要素番号を取得													 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

int CInfoItemTypeBase::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoItemTypeBase::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

DWORD CInfoItemTypeBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_byDelAverage);		/* 使ってなくなる確率 */
	dwRet += sizeof (m_byTarget);			/* 使用対象 */
	dwRet += sizeof (m_byArea);				/* 使用範囲 */
	dwRet += sizeof (m_bPutOn);				/* 上に置けるか判定 */
	dwRet += sizeof (m_wGrpIDMain);			/* 画像IDメイン */
	dwRet += sizeof (m_wGrpIDSub);			/* 画像IDサブ */
	dwRet += sizeof (m_dwTypeID);			/* 種別ID */
	dwRet += sizeof (m_dwItemTypeID);		/* アイテム種別ID */
	dwRet += sizeof (m_dwGrpID);			/* 地面にある時の画像ID */
	dwRet += sizeof (m_dwIconGrpID);		/* バックパック内の画像ID */
	dwRet += sizeof (m_dwDropSoundID);		/* 落ちたときの効果音ID */
	dwRet += (m_strName.GetLength () + 1);	/* アイテム名 */
	dwRet += sizeof (m_dwUseEffectID);		/* 使った時に再生するエフェクトID */
	dwRet += sizeof (m_dwUseSoundID);		/* 使った時に再生する効果音ID */
	dwRet += sizeof (m_dwWeaponInfoID);		/* 武器情報ID */
	dwRet += sizeof (m_dwValue);			/* 攻撃力 */
	dwRet += sizeof (m_dwMoveWait);			/* 速度 */
	dwRet += sizeof (m_dwMoveCount);		/* 飛距離 */
	dwRet += sizeof (m_dwValue2);			/* 効果(最大) */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::GetDataSizeNo								 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

DWORD CInfoItemTypeBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_byDelAverage);		break;	/* 使ってなくなる確率 */
	case 1:		dwRet = sizeof (m_byTarget);			break;	/* 使用対象 */
	case 2:		dwRet = sizeof (m_byArea);				break;	/* 使用範囲 */
	case 3:		dwRet = sizeof (m_bPutOn);				break;	/* 上に置けるか判定 */
	case 4:		dwRet = sizeof (m_dwTypeID);			break;	/* 種別ID */
	case 5:		dwRet = sizeof (m_dwItemTypeID);		break;	/* アイテム種別ID */
	case 6:		dwRet = sizeof (m_dwGrpID);				break;	/* 地面にある時の画像ID */
	case 7:		dwRet = sizeof (m_dwIconGrpID);			break;	/* バックパック内の画像ID */
	case 8:		dwRet = (m_strName.GetLength () + 1);	break;	/* アイテム名 */
	case 9:		dwRet = sizeof (m_dwDropSoundID);		break;	/* 落ちたときの効果音ID */
	case 10:	dwRet = sizeof (m_wGrpIDMain);			break;	/* 画像IDメイン */
	case 11:	dwRet = sizeof (m_wGrpIDSub);			break;	/* 画像IDサブ */
	case 12:	dwRet = sizeof (m_dwUseEffectID);		break;	/* 使った時に再生するエフェクトID */
	case 13:	dwRet = sizeof (m_dwUseSoundID);		break;	/* 使った時に再生する効果音ID */
	case 14:	dwRet = sizeof (m_dwWeaponInfoID);		break;	/* 武器情報ID */
	case 15:	dwRet = sizeof (m_dwValue);				break;	/* 攻撃力 */
	case 16:	dwRet = sizeof (m_dwMoveWait);			break;	/* 速度 */
	case 17:	dwRet = sizeof (m_dwMoveCount);			break;	/* 飛距離 */
	case 18:	dwRet = sizeof (m_dwValue2);			break;	/* 効果(最大) */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::GetName										 */
/* 内容		:要素名を取得													 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

LPCSTR CInfoItemTypeBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::GetWriteData								 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

PBYTE CInfoItemTypeBase::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = (PBYTE)&m_byDelAverage;		break;	/* 使ってなくなる確率 */
	case 1:		pSrc = (PBYTE)&m_byTarget;			break;	/* 使用対象 */
	case 2:		pSrc = (PBYTE)&m_byArea;			break;	/* 使用範囲 */
	case 3:		pSrc = (PBYTE)&m_bPutOn;			break;	/* 上に置けるか判定 */
	case 4:		pSrc = (PBYTE)&m_dwTypeID;			break;	/* 種別ID */
	case 5:		pSrc = (PBYTE)&m_dwItemTypeID;		break;	/* アイテム種別ID */
	case 6:		pSrc = (PBYTE)&m_dwGrpID;			break;	/* 地面にある時の画像ID */
	case 7:		pSrc = (PBYTE)&m_dwIconGrpID;		break;	/* バックパック内の画像ID */
	case 8:		pSrc = (PBYTE)(LPCSTR)m_strName;	break;	/* アイテム名 */
	case 9:		pSrc = (PBYTE)&m_dwDropSoundID;		break;	/* 落ちたときの効果音ID */
	case 10:	pSrc = (PBYTE)&m_wGrpIDMain;		break;	/* 画像IDメイン */
	case 11:	pSrc = (PBYTE)&m_wGrpIDSub;			break;	/* 画像IDサブ */
	case 12:	pSrc = (PBYTE)&m_dwUseEffectID;		break;	/* 使った時に再生するエフェクトID */
	case 13:	pSrc = (PBYTE)&m_dwUseSoundID;		break;	/* 使った時に再生する効果音ID */
	case 14:	pSrc = (PBYTE)&m_dwWeaponInfoID;	break;	/* 武器情報ID */
	case 15:	pSrc = (PBYTE)&m_dwValue;			break;	/* 攻撃力 */
	case 16:	pSrc = (PBYTE)&m_dwMoveWait;		break;	/* 速度 */
	case 17:	pSrc = (PBYTE)&m_dwMoveCount;		break;	/* 飛距離 */
	case 18:	pSrc = (PBYTE)&m_dwValue2;			break;	/* 効果(最大) */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

DWORD CInfoItemTypeBase::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_byDelAverage;		dwSize = sizeof (m_byDelAverage);	break;	/* 使ってなくなる確率 */
	case 1:		pDst = (PBYTE)&m_byTarget;			dwSize = sizeof (m_byTarget);		break;	/* 使用対象 */
	case 2:		pDst = (PBYTE)&m_byArea;			dwSize = sizeof (m_byArea);			break;	/* 使用範囲 */
	case 3:		pDst = (PBYTE)&m_bPutOn;			dwSize = sizeof (m_bPutOn);			break;	/* 上に置けるか判定 */
	case 4:		pDst = (PBYTE)&m_dwTypeID;			dwSize = sizeof (m_dwTypeID);		break;	/* 種別ID */
	case 5:		pDst = (PBYTE)&m_dwItemTypeID;		dwSize = sizeof (m_dwItemTypeID);	break;	/* アイテム種別ID */
	case 6:		pDst = (PBYTE)&m_dwGrpID;			dwSize = sizeof (m_dwGrpID);		break;	/* 地面にある時の画像ID */
	case 7:		pDst = (PBYTE)&m_dwIconGrpID;		dwSize = sizeof (m_dwIconGrpID);	break;	/* バックパック内の画像ID */
	case 8:		/* アイテム名 */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 9:		pDst = (PBYTE)&m_dwDropSoundID;		dwSize = sizeof (m_dwDropSoundID);		break;	/* 落ちたときの効果音ID */
	case 10:	pDst = (PBYTE)&m_wGrpIDMain;		dwSize = sizeof (m_wGrpIDMain);			break;	/* 画像IDメイン */
	case 11:	pDst = (PBYTE)&m_wGrpIDSub;			dwSize = sizeof (m_wGrpIDSub);			break;	/* 画像IDサブ */
	case 12:	pDst = (PBYTE)&m_dwUseEffectID;		dwSize = sizeof (m_dwUseEffectID);		break;	/* 使った時に再生するエフェクトID */
	case 13:	pDst = (PBYTE)&m_dwUseSoundID;		dwSize = sizeof (m_dwUseSoundID);		break;	/* 使った時に再生する効果音ID */
	case 14:	pDst = (PBYTE)&m_dwWeaponInfoID;	dwSize = sizeof (m_dwWeaponInfoID);		break;	/* 武器情報ID */
	case 15:	pDst = (PBYTE)&m_dwValue;			dwSize = sizeof (m_dwValue);			break;	/* 攻撃力 */
	case 16:	pDst = (PBYTE)&m_dwMoveWait;		dwSize = sizeof (m_dwMoveWait);			break;	/* 速度 */
	case 17:	pDst = (PBYTE)&m_dwMoveCount;		dwSize = sizeof (m_dwMoveCount);		break;	/* 飛距離 */
	case 18:	pDst = (PBYTE)&m_dwValue2;			dwSize = sizeof (m_dwValue2);			break;	/* 効果(最大) */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

DWORD CInfoItemTypeBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwTypeID)				+
			sizeof (m_dwItemTypeID)			+
			sizeof (m_dwGrpID)				+
			sizeof (m_dwIconGrpID)			+
			sizeof (m_dwDropSoundID)		+
			sizeof (m_byDelAverage)			+	/* 使ってなくなる確率 */
			sizeof (m_byTarget)				+	/* 使用対象 */
			sizeof (m_byArea)				+	/* 使用範囲 */
			sizeof (m_bPutOn)				+
			sizeof (m_wGrpIDMain)			+
			sizeof (m_wGrpIDSub)			+
			sizeof (m_dwUseEffectID)		+	/* 使った時に再生するエフェクトID */
			sizeof (m_dwUseSoundID)			+	/* 使った時に再生する効果音ID */
			(m_strName.GetLength () + 1)	+
			sizeof (m_dwWeaponInfoID)		+	/* 武器情報ID */
			sizeof (m_dwValue)				+	/* 攻撃力 */
			sizeof (m_dwMoveWait)			+	/* 速度 */
			sizeof (m_dwMoveCount)			+	/* 飛距離 */
			sizeof (m_dwValue2);				/* 効果(最大) */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

PBYTE CInfoItemTypeBase::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwItemTypeID,		sizeof (m_dwItemTypeID),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwTypeID,			sizeof (m_dwTypeID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwGrpID,			sizeof (m_dwGrpID),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwIconGrpID,		sizeof (m_dwIconGrpID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwDropSoundID,	sizeof (m_dwDropSoundID),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byDelAverage,		sizeof (m_byDelAverage),	pDataTmp);				/* 使ってなくなる確率 */
	CopyMemoryRenew (pDataTmp, &m_byTarget,			sizeof (m_byTarget),		pDataTmp);				/* 使用対象 */
	CopyMemoryRenew (pDataTmp, &m_byArea,			sizeof (m_byArea),			pDataTmp);				/* 使用範囲 */
	CopyMemoryRenew (pDataTmp, &m_bPutOn,			sizeof (m_bPutOn),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDMain,		sizeof (m_wGrpIDMain),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDSub,		sizeof (m_wGrpIDSub),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwUseEffectID,	sizeof (m_dwUseEffectID),	pDataTmp);				/* 使った時に再生するエフェクトID */
	CopyMemoryRenew (pDataTmp, &m_dwUseSoundID,		sizeof (m_dwUseSoundID),	pDataTmp);				/* 使った時に再生する効果音ID */
	CopyMemoryRenew (pDataTmp, &m_dwValue,			sizeof (m_dwValue),			pDataTmp);				/* 攻撃力 */
	CopyMemoryRenew (pDataTmp, &m_dwMoveWait,		sizeof (m_dwMoveWait),		pDataTmp);				/* 速度 */
	CopyMemoryRenew (pDataTmp, &m_dwMoveCount,		sizeof (m_dwMoveCount),		pDataTmp);				/* 飛距離 */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwWeaponInfoID,		sizeof (m_dwWeaponInfoID),		pDataTmp);		/* 武器情報ID */
	CopyMemoryRenew (pDataTmp, &m_dwValue2,				sizeof (m_dwValue2),			pDataTmp);		/* 効果(最大) */

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

PBYTE CInfoItemTypeBase::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwItemTypeID,	pDataTmp, sizeof (m_dwItemTypeID),	pDataTmp);
	CopyMemoryRenew (&m_dwTypeID,		pDataTmp, sizeof (m_dwTypeID),		pDataTmp);
	CopyMemoryRenew (&m_dwGrpID,		pDataTmp, sizeof (m_dwGrpID),		pDataTmp);
	CopyMemoryRenew (&m_dwIconGrpID,	pDataTmp, sizeof (m_dwIconGrpID),	pDataTmp);
	CopyMemoryRenew (&m_dwDropSoundID,	pDataTmp, sizeof (m_dwDropSoundID),	pDataTmp);
	CopyMemoryRenew (&m_byDelAverage,	pDataTmp, sizeof (m_byDelAverage),	pDataTmp);			/* 使ってなくなる確率 */
	CopyMemoryRenew (&m_byTarget,		pDataTmp, sizeof (m_byTarget),		pDataTmp);			/* 使用対象 */
	CopyMemoryRenew (&m_byArea,			pDataTmp, sizeof (m_byArea),		pDataTmp);			/* 使用範囲 */
	CopyMemoryRenew (&m_bPutOn,			pDataTmp, sizeof (m_bPutOn),		pDataTmp);
	CopyMemoryRenew (&m_wGrpIDMain,		pDataTmp, sizeof (m_wGrpIDMain),	pDataTmp);
	CopyMemoryRenew (&m_wGrpIDSub,		pDataTmp, sizeof (m_wGrpIDSub),		pDataTmp);
	CopyMemoryRenew (&m_dwUseEffectID,	pDataTmp, sizeof (m_dwUseEffectID),	pDataTmp);			/* 使った時に再生するエフェクトID */
	CopyMemoryRenew (&m_dwUseSoundID,	pDataTmp, sizeof (m_dwUseSoundID),	pDataTmp);			/* 使った時に再生する効果音ID */
	CopyMemoryRenew (&m_dwValue,		pDataTmp, sizeof (m_dwValue),		pDataTmp);			/* 攻撃力 */
	CopyMemoryRenew (&m_dwMoveWait,		pDataTmp, sizeof (m_dwMoveWait),	pDataTmp);			/* 速度 */
	CopyMemoryRenew (&m_dwMoveCount,	pDataTmp, sizeof (m_dwMoveCount),	pDataTmp);			/* 飛距離 */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);
	CopyMemoryRenew (&m_dwWeaponInfoID,	pDataTmp, sizeof (m_dwWeaponInfoID),	pDataTmp);		/* 武器情報ID */
	CopyMemoryRenew (&m_dwValue2,		pDataTmp, sizeof (m_dwValue2),			pDataTmp);		/* 効果(最大) */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::Copy										 */
/* 内容		:コピー															 */
/* 日付		:2007/09/26														 */
/* ========================================================================= */

void CInfoItemTypeBase::Copy(CInfoItemTypeBase *pSrc)
{
	if (pSrc == NULL) {
		return;
	}
	m_byDelAverage		= pSrc->m_byDelAverage;			/* 使ってなくなる確率 */
	m_byTarget			= pSrc->m_byTarget;				/* 使用対象 */
	m_byArea			= pSrc->m_byArea;				/* 使用範囲 */
	m_bPutOn			= pSrc->m_bPutOn;
	m_wGrpIDMain		= pSrc->m_wGrpIDMain;
	m_wGrpIDSub			= pSrc->m_wGrpIDSub;
	m_dwTypeID			= pSrc->m_dwTypeID;
	m_dwItemTypeID		= pSrc->m_dwItemTypeID;
	m_dwGrpID			= pSrc->m_dwGrpID;
	m_dwIconGrpID		= pSrc->m_dwIconGrpID;
	m_dwDropSoundID		= pSrc->m_dwDropSoundID;
	m_dwUseEffectID		= pSrc->m_dwUseEffectID;		/* 使った時に再生するエフェクトID */
	m_dwUseSoundID		= pSrc->m_dwUseSoundID;			/* 使った時に再生する効果音ID */
	m_strName			= pSrc->m_strName;
	m_dwWeaponInfoID	= pSrc->m_dwWeaponInfoID;		/* 武器情報ID */
	m_dwValue			= pSrc->m_dwValue;				/* 攻撃力 */
	m_dwMoveWait		= pSrc->m_dwMoveWait;			/* 速度 */
	m_dwMoveCount		= pSrc->m_dwMoveCount;			/* 飛距離 */
	m_dwValue2			= pSrc->m_dwValue2;				/* 効果(最大) */
}


/* ========================================================================= */
/* 関数名	:CInfoItemTypeBase::SetGrpID									 */
/* 内容		:画像IDを設定													 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

void CInfoItemTypeBase::SetGrpID(WORD *pwDst1, WORD *pwDst2)
{
	if (pwDst1) {
		*pwDst1 = 0;
	}
	if (pwDst2) {
		*pwDst2 = 0;
	}

	switch (m_dwItemTypeID) {
	case ITEMTYPEID_NONE:			/* 効果無し */
		break;
	case ITEMTYPEID_CLOTH:			/* 服 */
		switch (m_wGrpIDMain) {
		case ITEMGRPIDMAIN_CLOTH_NONE:		/* 未設定 */
			if (pwDst1) {
				*pwDst1 = m_wGrpIDSub;
			}
			break;
		case ITEMGRPIDMAIN_CLOTH_SP:		/* 特殊服 */
			if (pwDst2) {
				*pwDst2 = m_wGrpIDSub;
			}
			break;
		}
		break;
	case ITEMTYPEID_ACCE:			/* アクセサリ */
		*pwDst1 = m_wGrpIDSub;
		break;
	case ITEMTYPEID_ARMS:			/* 持ち物 */
	case ITEMTYPEID_SHIELD:			/* 盾 */
	case ITEMTYPEID_HP:				/* HP増減 */
		*pwDst1 = m_wGrpIDMain;
		*pwDst2 = m_wGrpIDSub;
		break;
	}
}

/* Copyright(C)URARA-works 2007 */
