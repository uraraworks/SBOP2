/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoItem.cpp												 */
/* 内容			:アイテム情報クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoItem.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_bPutOn",			/* 上に置けるか判定 */
	"m_dwItemID",		/* アイテムID */
	"m_dwItemTypeID",	/* アイテム種別ID */
	"m_dwGrpID",		/* 画像ID */
	"m_dwIconGrpID",	/* バックパック内の画像ID */
	"m_dwMapID",		/* 落ちているマップID */
	"m_dwCharID",		/* 所有者キャラID */
	"m_nPosZ",			/* 落ちている高さ(0が地面) */
	"m_ptPos",			/* 落ちている座標 */
	"m_ptBackPack",		/* バックパック内の位置 */
	"m_strName",		/* アイテム名 */
	"m_dwDropSoundID",	/* 落ちたときの効果音ID */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoItem::CInfoItem											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

CInfoItem::CInfoItem()
{
	m_bPutOn		= FALSE;
	m_dwItemID		= 0;
	m_dwItemTypeID	= 0;
	m_dwGrpID		= 0;
	m_dwIconGrpID	= 0;
	m_dwMapID		= 0;
	m_dwCharID		= 0;
	m_dwDropSoundID	= 0;
	m_nPosZ			= 0;
	m_ptPos.x = m_ptPos.y = 0;
	m_ptBackPack.x = m_ptBackPack.y = 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoItem::~CInfoItem											 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

CInfoItem::~CInfoItem()
{
}


/* ========================================================================= */
/* 関数名	:CInfoItem::GetElementNo										 */
/* 内容		:要素番号を取得													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

int CInfoItem::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoItem::GetDataSize											 */
/* 内容		:データサイズを取得												 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

DWORD CInfoItem::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_bPutOn);				/* 上に置けるか判定 */
	dwRet += sizeof (m_dwItemID);			/* アイテムID */
	dwRet += sizeof (m_dwItemTypeID);		/* アイテム種別ID */
	dwRet += sizeof (m_dwGrpID);			/* 画像ID */
	dwRet += sizeof (m_dwIconGrpID);		/* バックパック内の画像ID */
	dwRet += sizeof (m_dwMapID);			/* 落ちているマップID */
	dwRet += sizeof (m_dwCharID);			/* 所有者キャラID */
	dwRet += sizeof (m_dwDropSoundID);		/* 落ちたときの効果音ID */
	dwRet += sizeof (m_nPosZ);				/* 落ちている高さ(0が地面) */
	dwRet += sizeof (m_ptPos);				/* 落ちている座標 */
	dwRet += sizeof (m_ptBackPack);			/* バックパック内の位置 */
	dwRet += (m_strName.GetLength () + 1);	/* アイテム名 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItem::GetDataSizeNo										 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

DWORD CInfoItem::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_bPutOn);				break;	/* 上に置けるか判定 */
	case 1:		dwRet = sizeof (m_dwItemID);			break;	/* アイテムID */
	case 2:		dwRet = sizeof (m_dwItemTypeID);		break;	/* アイテム種別ID */
	case 3:		dwRet = sizeof (m_dwGrpID);				break;	/* 画像ID */
	case 4:		dwRet = sizeof (m_dwIconGrpID);			break;	/* バックパック内の画像ID */
	case 5:		dwRet = sizeof (m_dwMapID);				break;	/* 落ちているマップID */
	case 6:		dwRet = sizeof (m_dwCharID);			break;	/* 所有者キャラID */
	case 7:		dwRet = sizeof (m_nPosZ);				break;	/* 落ちている高さ(0が地面) */
	case 8:		dwRet = sizeof (m_ptPos);				break;	/* 落ちている座標 */
	case 9:		dwRet = sizeof (m_ptBackPack);			break;	/* バックパック内の位置 */
	case 10:	dwRet = (m_strName.GetLength () + 1);	break;	/* アイテム名 */
	case 11:	dwRet = sizeof (m_dwDropSoundID);		break;	/* 落ちたときの効果音ID */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItem::GetName												 */
/* 内容		:要素名を取得													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

LPCSTR CInfoItem::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoItem::GetWriteData										 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

PBYTE CInfoItem::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = (PBYTE)&m_bPutOn;			break;	/* 上に置けるか判定 */
	case 1:		pSrc = (PBYTE)&m_dwItemID;			break;	/* アイテムID */
	case 2:		pSrc = (PBYTE)&m_dwItemTypeID;		break;	/* アイテム種別ID */
	case 3:		pSrc = (PBYTE)&m_dwGrpID;			break;	/* 画像ID */
	case 4:		pSrc = (PBYTE)&m_dwIconGrpID;		break;	/* バックパック内の画像ID */
	case 5:		pSrc = (PBYTE)&m_dwMapID;			break;	/* 落ちているマップID */
	case 6:		pSrc = (PBYTE)&m_dwCharID;			break;	/* 所有者キャラID */
	case 7:		pSrc = (PBYTE)&m_nPosZ;				break;	/* 落ちている高さ(0が地面) */
	case 8:		pSrc = (PBYTE)&m_ptPos;				break;	/* 落ちている座標 */
	case 9:		pSrc = (PBYTE)&m_ptBackPack;		break;	/* バックパック内の位置 */
	case 10:	pSrc = (PBYTE)(LPCSTR)m_strName;	break;	/* アイテム名 */
	case 11:	pSrc = (PBYTE)&m_dwDropSoundID;		break;	/* 落ちたときの効果音ID */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItem::ReadElementData										 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

DWORD CInfoItem::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_bPutOn;			dwSize = sizeof (m_bPutOn);			break;	/* 上に置けるか判定 */
	case 1:		pDst = (PBYTE)&m_dwItemID;			dwSize = sizeof (m_dwItemID);		break;	/* アイテムID */
	case 2:		pDst = (PBYTE)&m_dwItemTypeID;		dwSize = sizeof (m_dwItemTypeID);	break;	/* アイテム種別ID */
	case 3:		pDst = (PBYTE)&m_dwGrpID;			dwSize = sizeof (m_dwGrpID);		break;	/* 画像ID */
	case 4:		pDst = (PBYTE)&m_dwIconGrpID;		dwSize = sizeof (m_dwIconGrpID);	break;	/* バックパック内の画像ID */
	case 5:		pDst = (PBYTE)&m_dwMapID;			dwSize = sizeof (m_dwMapID);		break;	/* 落ちているマップID */
	case 6:		pDst = (PBYTE)&m_dwCharID;			dwSize = sizeof (m_dwCharID);		break;	/* 所有者キャラID */
	case 7:		pDst = (PBYTE)&m_nPosZ;				dwSize = sizeof (m_nPosZ);			break;	/* 落ちている高さ(0が地面) */
	case 8:		pDst = (PBYTE)&m_ptPos;				dwSize = sizeof (m_ptPos);			break;	/* 落ちている座標 */
	case 9:		pDst = (PBYTE)&m_ptBackPack;		dwSize = sizeof (m_ptBackPack);		break;	/* バックパック内の位置 */
	case 10:	/* アイテム名 */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 11:	pDst = (PBYTE)&m_dwDropSoundID;		dwSize = sizeof (m_dwDropSoundID);	break;	/* 落ちたときの効果音ID */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoItem::GetSendDataSize										 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

DWORD CInfoItem::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwItemID)			+
			sizeof (m_dwItemTypeID)		+
			sizeof (m_dwGrpID)			+
			sizeof (m_dwIconGrpID)		+
			sizeof (m_dwMapID)			+
			sizeof (m_dwCharID)			+
			sizeof (m_dwDropSoundID)	+
			sizeof (m_bPutOn)			+
			sizeof (m_nPosZ)			+
			sizeof (m_ptPos)			+
			sizeof (m_ptBackPack)		+
			(m_strName.GetLength () + 1);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItem::GetSendData											 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PBYTE CInfoItem::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwItemID,			sizeof (m_dwItemID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwItemTypeID,		sizeof (m_dwItemTypeID),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwGrpID,			sizeof (m_dwGrpID),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwIconGrpID,		sizeof (m_dwIconGrpID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwMapID,			sizeof (m_dwMapID),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwCharID,			sizeof (m_dwCharID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwDropSoundID,	sizeof (m_dwDropSoundID),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_bPutOn,			sizeof (m_bPutOn),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_nPosZ,			sizeof (m_nPosZ),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptPos,			sizeof (m_ptPos),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptBackPack,		sizeof (m_ptBackPack),		pDataTmp);
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoItem::SetSendData											 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PBYTE CInfoItem::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwItemID,		pDataTmp, sizeof (m_dwItemID),		pDataTmp);
	CopyMemoryRenew (&m_dwItemTypeID,	pDataTmp, sizeof (m_dwItemTypeID),	pDataTmp);
	CopyMemoryRenew (&m_dwGrpID,		pDataTmp, sizeof (m_dwGrpID),		pDataTmp);
	CopyMemoryRenew (&m_dwIconGrpID,	pDataTmp, sizeof (m_dwIconGrpID),	pDataTmp);
	CopyMemoryRenew (&m_dwMapID,		pDataTmp, sizeof (m_dwMapID),		pDataTmp);
	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),		pDataTmp);
	CopyMemoryRenew (&m_dwDropSoundID,	pDataTmp, sizeof (m_dwDropSoundID),	pDataTmp);
	CopyMemoryRenew (&m_bPutOn,			pDataTmp, sizeof (m_bPutOn),		pDataTmp);
	CopyMemoryRenew (&m_nPosZ,			pDataTmp, sizeof (m_nPosZ),			pDataTmp);
	CopyMemoryRenew (&m_ptPos,			pDataTmp, sizeof (m_ptPos),			pDataTmp);
	CopyMemoryRenew (&m_ptBackPack,		pDataTmp, sizeof (m_ptBackPack),	pDataTmp);
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItem::Copy												 */
/* 内容		:コピー															 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

void CInfoItem::Copy(CInfoItem *pSrc)
{
	m_bPutOn		= pSrc->m_bPutOn;
	m_dwItemID		= pSrc->m_dwItemID;
	m_dwItemTypeID	= pSrc->m_dwItemTypeID;
	m_dwGrpID		= pSrc->m_dwGrpID;
	m_dwIconGrpID	= pSrc->m_dwIconGrpID;
	m_dwMapID		= pSrc->m_dwMapID;
	m_dwCharID		= pSrc->m_dwCharID;
	m_dwDropSoundID	= pSrc->m_dwDropSoundID;
	m_nPosZ			= pSrc->m_nPosZ;
	m_ptPos			= pSrc->m_ptPos;
	m_ptBackPack	= pSrc->m_ptBackPack;
	m_strName		= pSrc->m_strName;
}

/* Copyright(C)URARA-works 2007 */
