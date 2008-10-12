/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_GRP.cpp											 */
/* 内容			:コマンド(キャラ系:画像通知) 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketCHAR_GRP.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_GRP::CPacketCHAR_GRP								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

CPacketCHAR_GRP::CPacketCHAR_GRP()
{
	m_dwCharID					= 0;	/* キャラID */
	m_dwEquipItemIDCloth		= 0;	/* 装備アイテムID(服) */
	m_dwEquipItemIDAcce1		= 0;	/* 装備アイテムID(アクセサリ) */
	m_dwEquipItemIDArmsRight	= 0;	/* 装備アイテムID(持ち物) */
	m_dwEquipItemIDArmsLeft		= 0;	/* 装備アイテムID:左手 */
	m_wGrpIDNPC					= 0;	/* 画像ID(NPC) */
	m_wGrpIDCloth				= 0;	/* 画像ID(服) */
	m_wGrpIDEye					= 0;	/* 画像ID(目) */
	m_wGrpIDEyeColor			= 0;	/* 画像ID(目色) */
	m_wGrpIDHairType			= 0;	/* 画像ID(髪) */
	m_wGrpIDHairColor			= 0;	/* 画像ID(髪色) */
	m_wGrpIDSP					= 0;	/* 画像ID(特殊服) */
	m_wGrpIDAcce				= 0;	/* 画像ID(アクセサリ) */
	m_wGrpIDArmsMain			= 0;	/* 画像ID(持ち物:メイン) */
	m_wGrpIDArmsSub				= 0;	/* 画像ID(持ち物:サブ) */
	m_wGrpIDArmsLeftMain		= 0;	/* 画像ID(盾:メイン) */
	m_wGrpIDArmsLeftSub			= 0;	/* 画像ID(盾:サブ) */
	m_wGrpIDTmpMain				= 0;	/* 画像ID(一時服:メイン) */
	m_wGrpIDTmpSub				= 0;	/* 画像ID(一時服:サブ) */
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_GRP::~CPacketCHAR_GRP								 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

CPacketCHAR_GRP::~CPacketCHAR_GRP()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_GRP::Make											 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

void CPacketCHAR_GRP::Make(
	CInfoCharBase *pInfo)		/* [in] キャラ情報 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)						+
			 sizeof (pInfo->m_dwCharID)					+
			 sizeof (pInfo->m_dwEquipItemIDCloth)		+
			 sizeof (pInfo->m_dwEquipItemIDAcce1)		+
			 sizeof (pInfo->m_dwEquipItemIDArmsRight)	+
			 sizeof (pInfo->m_dwEquipItemIDArmsLeft)	+
			 sizeof (pInfo->m_wGrpIDNPC)				+
			 sizeof (pInfo->m_wGrpIDCloth)				+
			 sizeof (pInfo->m_wGrpIDEye)				+
			 sizeof (pInfo->m_wGrpIDEyeColor)			+
			 sizeof (pInfo->m_wGrpIDHairType)			+
			 sizeof (pInfo->m_wGrpIDHairColor)			+
			 sizeof (pInfo->m_wGrpIDSP)					+
			 sizeof (pInfo->m_wGrpIDAcce)				+
			 sizeof (pInfo->m_wGrpIDArmsMain)			+
			 sizeof (pInfo->m_wGrpIDArmsSub)			+
			 sizeof (pInfo->m_wGrpIDArmsLeftMain)		+
			 sizeof (pInfo->m_wGrpIDArmsLeftSub)		+
			 sizeof (pInfo->m_wGrpIDTmpMain)			+
			 sizeof (pInfo->m_wGrpIDTmpSub);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_GRP;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwCharID,					sizeof (pInfo->m_dwCharID),					pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwEquipItemIDCloth,		sizeof (pInfo->m_dwEquipItemIDCloth),		pDataTmp);	/* 装備アイテムID(服) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwEquipItemIDAcce1,		sizeof (pInfo->m_dwEquipItemIDAcce1),		pDataTmp);	/* 装備アイテムID(アクセサリ) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_dwEquipItemIDArmsRight,	sizeof (pInfo->m_dwEquipItemIDArmsRight),	pDataTmp);	/* 装備アイテムID(持ち物) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDNPC,					sizeof (pInfo->m_wGrpIDNPC),				pDataTmp);	/* 画像ID(NPC) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDCloth,				sizeof (pInfo->m_wGrpIDCloth),				pDataTmp);	/* 画像ID(服) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDEye,					sizeof (pInfo->m_wGrpIDEye),				pDataTmp);	/* 画像ID(目) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDEyeColor,			sizeof (pInfo->m_wGrpIDEyeColor),			pDataTmp);	/* 画像ID(目色) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDHairType,			sizeof (pInfo->m_wGrpIDHairType),			pDataTmp);	/* 画像ID(髪) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDHairColor,			sizeof (pInfo->m_wGrpIDHairColor),			pDataTmp);	/* 画像ID(髪色) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDSP,					sizeof (pInfo->m_wGrpIDSP),					pDataTmp);	/* 画像ID(特殊服) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDAcce,				sizeof (pInfo->m_wGrpIDAcce),				pDataTmp);	/* 画像ID(アクセサリ) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDArmsMain,			sizeof (pInfo->m_wGrpIDArmsMain),			pDataTmp);	/* 画像ID(持ち物:メイン) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDArmsSub,				sizeof (pInfo->m_wGrpIDArmsSub),			pDataTmp);	/* 画像ID(持ち物:サブ) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDArmsLeftMain,		sizeof (pInfo->m_wGrpIDArmsLeftMain),		pDataTmp);	/* 画像ID(盾:メイン) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDArmsLeftSub,			sizeof (pInfo->m_wGrpIDArmsLeftSub),		pDataTmp);	/* 画像ID(盾:サブ) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDTmpMain,				sizeof (pInfo->m_wGrpIDTmpMain),			pDataTmp);	/* 画像ID(一時服:メイン) */
	CopyMemoryRenew (pDataTmp, &pInfo->m_wGrpIDTmpSub,				sizeof (pInfo->m_wGrpIDTmpSub),				pDataTmp);	/* 画像ID(一時服:サブ) */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_GRP::Set											 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

PBYTE CPacketCHAR_GRP::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,					pDataTmp, sizeof (m_dwCharID),					pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwEquipItemIDCloth,			pDataTmp, sizeof (m_dwEquipItemIDCloth),		pDataTmp);	/* 装備アイテムID(服) */
	CopyMemoryRenew (&m_dwEquipItemIDAcce1,			pDataTmp, sizeof (m_dwEquipItemIDAcce1),		pDataTmp);	/* 装備アイテムID(アクセサリ) */
	CopyMemoryRenew (&m_dwEquipItemIDArmsRight,		pDataTmp, sizeof (m_dwEquipItemIDArmsRight),	pDataTmp);	/* 装備アイテムID(持ち物) */
	CopyMemoryRenew (&m_wGrpIDNPC,					pDataTmp, sizeof (m_wGrpIDNPC),					pDataTmp);	/* 画像ID(NPC) */
	CopyMemoryRenew (&m_wGrpIDCloth,				pDataTmp, sizeof (m_wGrpIDCloth),				pDataTmp);	/* 画像ID(服) */
	CopyMemoryRenew (&m_wGrpIDEye,					pDataTmp, sizeof (m_wGrpIDEye),					pDataTmp);	/* 画像ID(目) */
	CopyMemoryRenew (&m_wGrpIDEyeColor,				pDataTmp, sizeof (m_wGrpIDEyeColor),			pDataTmp);	/* 画像ID(目色) */
	CopyMemoryRenew (&m_wGrpIDHairType,				pDataTmp, sizeof (m_wGrpIDHairType),			pDataTmp);	/* 画像ID(髪) */
	CopyMemoryRenew (&m_wGrpIDHairColor,			pDataTmp, sizeof (m_wGrpIDHairColor),			pDataTmp);	/* 画像ID(髪色) */
	CopyMemoryRenew (&m_wGrpIDSP,					pDataTmp, sizeof (m_wGrpIDSP),					pDataTmp);	/* 画像ID(特殊服) */
	CopyMemoryRenew (&m_wGrpIDAcce,					pDataTmp, sizeof (m_wGrpIDAcce),				pDataTmp);	/* 画像ID(アクセサリ) */
	CopyMemoryRenew (&m_wGrpIDArmsMain,				pDataTmp, sizeof (m_wGrpIDArmsMain),			pDataTmp);	/* 画像ID(持ち物:メイン) */
	CopyMemoryRenew (&m_wGrpIDArmsSub,				pDataTmp, sizeof (m_wGrpIDArmsSub),				pDataTmp);	/* 画像ID(持ち物:サブ) */
	CopyMemoryRenew (&m_wGrpIDArmsLeftMain,			pDataTmp, sizeof (m_wGrpIDArmsLeftMain),		pDataTmp);	/* 画像ID(盾:メイン) */
	CopyMemoryRenew (&m_wGrpIDArmsLeftSub,			pDataTmp, sizeof (m_wGrpIDArmsLeftSub),			pDataTmp);	/* 画像ID(盾:サブ) */
	CopyMemoryRenew (&m_wGrpIDTmpMain,				pDataTmp, sizeof (m_wGrpIDTmpMain),				pDataTmp);	/* 画像ID(一時服:メイン) */
	CopyMemoryRenew (&m_wGrpIDTmpSub,				pDataTmp, sizeof (m_wGrpIDTmpSub),				pDataTmp);	/* 画像ID(一時服:サブ) */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_GRP::SetParam										 */
/* 内容		:指定キャラ情報へパラメータを反映								 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

void CPacketCHAR_GRP::SetParam(CInfoCharBase *pInfo)
{
	if (pInfo == NULL) {
		return;
	}

	pInfo->m_dwEquipItemIDCloth		= m_dwEquipItemIDCloth;		/* 装備アイテムID(服) */
	pInfo->m_dwEquipItemIDAcce1		= m_dwEquipItemIDAcce1;		/* 装備アイテムID(アクセサリ) */
	pInfo->m_dwEquipItemIDArmsRight	= m_dwEquipItemIDArmsRight;	/* 装備アイテムID(持ち物) */
	pInfo->m_dwEquipItemIDArmsLeft	= m_dwEquipItemIDArmsLeft;	/* 装備アイテムID:左手 */
	pInfo->m_wGrpIDNPC				= m_wGrpIDNPC;				/* 画像ID(NPC) */
	pInfo->m_wGrpIDCloth			= m_wGrpIDCloth;			/* 画像ID(服) */
	pInfo->m_wGrpIDEye				= m_wGrpIDEye;				/* 画像ID(目) */
	pInfo->m_wGrpIDEyeColor			= m_wGrpIDEyeColor;			/* 画像ID(目色) */
	pInfo->m_wGrpIDHairType			= m_wGrpIDHairType;			/* 画像ID(髪) */
	pInfo->m_wGrpIDHairColor		= m_wGrpIDHairColor;		/* 画像ID(髪色) */
	pInfo->m_wGrpIDSP				= m_wGrpIDSP;				/* 画像ID(特殊服) */
	pInfo->m_wGrpIDAcce				= m_wGrpIDAcce;				/* 画像ID(アクセサリ) */
	pInfo->m_wGrpIDArmsMain			= m_wGrpIDArmsMain;			/* 画像ID(持ち物:メイン) */
	pInfo->m_wGrpIDArmsSub			= m_wGrpIDArmsSub;			/* 画像ID(持ち物:サブ) */
	pInfo->m_wGrpIDArmsLeftMain		= m_wGrpIDArmsLeftMain;		/* 画像ID(盾:メイン) */
	pInfo->m_wGrpIDArmsLeftSub		= m_wGrpIDArmsLeftSub;		/* 画像ID(盾:サブ) */
	pInfo->m_wGrpIDTmpMain			= m_wGrpIDTmpMain;			/* 画像ID(一時服:メイン) */
	pInfo->m_wGrpIDTmpSub			= m_wGrpIDTmpSub;			/* 画像ID(一時服:サブ) */
}

/* Copyright(C)URARA-works 2007 */
