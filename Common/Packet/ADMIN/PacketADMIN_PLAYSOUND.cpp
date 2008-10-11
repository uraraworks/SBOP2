/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_PLAYSOUND.cpp									 */
/* 内容			:コマンド(管理者系:効果音の再生) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/02													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_PLAYSOUND.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_PLAYSOUND::CPacketADMIN_PLAYSOUND					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

CPacketADMIN_PLAYSOUND::CPacketADMIN_PLAYSOUND()
{
	m_dwSoundID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_PLAYSOUND::~CPacketADMIN_PLAYSOUND				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

CPacketADMIN_PLAYSOUND::~CPacketADMIN_PLAYSOUND()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_PLAYSOUND::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

void CPacketADMIN_PLAYSOUND::Make(
	DWORD dwSoundID)		/* [in] 効果音ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwSoundID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_PLAYSOUND;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwSoundID, sizeof (dwSoundID), pDataTmp);	/* 効果音ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_PLAYSOUND::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

PBYTE CPacketADMIN_PLAYSOUND::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwSoundID, pDataTmp, sizeof (m_dwSoundID), pDataTmp);	/* 効果音ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
