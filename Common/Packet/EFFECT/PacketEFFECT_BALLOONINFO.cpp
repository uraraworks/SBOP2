/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_BALLOONINFO.cpp								 */
/* 内容			:コマンド(エフェクト系:噴出し情報応答) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoEfcBalloon.h"
#include "Command.h"
#include "PacketEFFECT_BALLOONINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketEFFECT_BALLOONINFO::CPacketEFFECT_BALLOONINFO			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_BALLOONINFO::CPacketEFFECT_BALLOONINFO()
{
	m_dwListID	= 0;
	m_pLibInfo	= NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_BALLOONINFO::~CPacketEFFECT_BALLOONINFO			 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_BALLOONINFO::~CPacketEFFECT_BALLOONINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_BALLOONINFO::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CPacketEFFECT_BALLOONINFO::Make(
	DWORD dwListID,						/* [in] 種別ID */
	CLibInfoEfcBalloon *pLibInfo)		/* [in] 噴出し情報 */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pLibInfo->GetSendDataSize (dwListID);
	pInfoDataTmp	= pLibInfo->GetSendData (dwListID);

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwListID) +
			 dwSizeDataTmp;

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_BALLOONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwListID,		sizeof (dwListID),	pDataTmp);	/* 種別ID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	 	pDataTmp);	/* 噴出し情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_BALLOONINFO::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_BALLOONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwListID, pDataTmp, sizeof (m_dwListID), pDataTmp);	/* 種別ID */

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoEfcBalloon;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
