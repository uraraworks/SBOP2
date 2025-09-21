/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_REQ_EFFECTINFO.cpp							 */
/* 内容			:コマンド(エフェクト系:エフェクト情報要求) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketEFFECT_REQ_EFFECTINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketEFFECT_REQ_EFFECTINFO::CPacketEFFECT_REQ_EFFECTINFO		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CPacketEFFECT_REQ_EFFECTINFO::CPacketEFFECT_REQ_EFFECTINFO()
{
	m_dwEffectID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_REQ_EFFECTINFO::~CPacketEFFECT_REQ_EFFECTINFO	 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CPacketEFFECT_REQ_EFFECTINFO::~CPacketEFFECT_REQ_EFFECTINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_REQ_EFFECTINFO::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CPacketEFFECT_REQ_EFFECTINFO::Make(
	DWORD dwEffectID)		/* [in] エフェクトID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwEffectID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_REQ_EFFECTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwEffectID, sizeof (dwEffectID), pDataTmp);	/* エフェクトID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_REQ_EFFECTINFO::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_REQ_EFFECTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwEffectID, pDataTmp, sizeof (m_dwEffectID), pDataTmp);	/* エフェクトID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
