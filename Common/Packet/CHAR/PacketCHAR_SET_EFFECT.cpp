/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_SET_EFFECT.cpp									 */
/* 内容			:コマンド(キャラ系:エフェクトを指定) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/27													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_SET_EFFECT.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_SET_EFFECT::CPacketCHAR_SET_EFFECT					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/27														 */
/* ========================================================================= */

CPacketCHAR_SET_EFFECT::CPacketCHAR_SET_EFFECT()
{
	m_dwCharID		= 0;
	m_dwEffectID	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_SET_EFFECT::~CPacketCHAR_SET_EFFECT				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/27														 */
/* ========================================================================= */

CPacketCHAR_SET_EFFECT::~CPacketCHAR_SET_EFFECT()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_SET_EFFECT::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/07/27														 */
/* ========================================================================= */

void CPacketCHAR_SET_EFFECT::Make(
	DWORD dwCharID,			/* [in] キャラID */
	DWORD dwEffectID)		/* [in] エフェクトID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwEffectID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_SET_EFFECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),		pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwEffectID,	sizeof (dwEffectID),	pDataTmp);	/* エフェクトID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_SET_EFFECT::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/07/27														 */
/* ========================================================================= */

PBYTE CPacketCHAR_SET_EFFECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwEffectID,	pDataTmp, sizeof (m_dwEffectID),	pDataTmp);	/* エフェクトID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
