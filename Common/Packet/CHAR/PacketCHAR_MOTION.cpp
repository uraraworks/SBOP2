/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_MOTION.cpp										 */
/* 内容			:コマンド(管理者系:モーション情報通知) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/11/23													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMotion.h"
#include "Command.h"
#include "PacketCHAR_MOTION.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_MOTION::CPacketCHAR_MOTION							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

CPacketCHAR_MOTION::CPacketCHAR_MOTION()
{
	m_dwMotionTypeID = 0;
	m_dwMotionListID = 0;
	m_pInfo			 = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_MOTION::~CPacketCHAR_MOTION						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

CPacketCHAR_MOTION::~CPacketCHAR_MOTION()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_MOTION::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

void CPacketCHAR_MOTION::Make(
	DWORD dwMotionTypeID,		/* [in] モーション種別ID */
	DWORD dwMotionListID,		/* [in] モーションリストID */
	CLibInfoMotion *pSrc)		/* [in] モーション情報 */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pSrc->GetSendDataSize (dwMotionTypeID, dwMotionListID);
	pInfoDataTmp	= pSrc->GetSendData (dwMotionTypeID, dwMotionListID);

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwMotionTypeID) +
			 sizeof (dwMotionListID) +
			 dwSizeDataTmp;

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_MOTION;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMotionTypeID, sizeof (dwMotionTypeID), pDataTmp);	/* モーション種別ID */
	CopyMemoryRenew (pDataTmp, &dwMotionListID, sizeof (dwMotionListID), pDataTmp);	/* モーションリストID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,			 pDataTmp);	/* モーション情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_MOTION::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/11/23														 */
/* ========================================================================= */

PBYTE CPacketCHAR_MOTION::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMotionTypeID, pDataTmp, sizeof (m_dwMotionTypeID), pDataTmp);	/* モーション種別ID */
	CopyMemoryRenew (&m_dwMotionListID, pDataTmp, sizeof (m_dwMotionListID), pDataTmp);	/* モーションリストID */

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CLibInfoMotion;
	m_pInfo->Create ();
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
