/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_DELETEMAPSHADOW.cpp						 */
/* 内容			:コマンド(管理者系:マップ影削除) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_MAP_DELETEMAPSHADOW.h"


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_DELETEMAPSHADOW::CPacketADMIN_MAP_DELETEMAPSHADOW	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/07														 */
/* ========================================================================= */

CPacketADMIN_MAP_DELETEMAPSHADOW::CPacketADMIN_MAP_DELETEMAPSHADOW()
{
	m_dwShadowID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_DELETEMAPSHADOW::~CPacketADMIN_MAP_DELETEMAPSHADOW */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/07														 */
/* ========================================================================= */

CPacketADMIN_MAP_DELETEMAPSHADOW::~CPacketADMIN_MAP_DELETEMAPSHADOW()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_DELETEMAPSHADOW::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/06/07														 */
/* ========================================================================= */

void CPacketADMIN_MAP_DELETEMAPSHADOW::Make(
	DWORD dwShadowID)		/* [in] 影ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwShadowID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_DELETEMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwShadowID, sizeof (dwShadowID), pDataTmp);	/* 影ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_DELETEMAPSHADOW::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/06/07														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_DELETEMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwShadowID,	pDataTmp, sizeof (m_dwShadowID), pDataTmp);	/* 影ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
