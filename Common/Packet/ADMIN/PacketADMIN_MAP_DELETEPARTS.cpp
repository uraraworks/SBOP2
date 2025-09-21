/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_DELETEPARTS.cpp							 */
/* 内容			:コマンド(管理者系:マップパーツ削除) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_MAP_DELETEPARTS.h"


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_DELETEPARTS::CPacketADMIN_MAP_DELETEPARTS		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

CPacketADMIN_MAP_DELETEPARTS::CPacketADMIN_MAP_DELETEPARTS()
{
	m_dwPartsID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_DELETEPARTS::~CPacketADMIN_MAP_DELETEPARTS	 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

CPacketADMIN_MAP_DELETEPARTS::~CPacketADMIN_MAP_DELETEPARTS()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_DELETEPARTS::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

void CPacketADMIN_MAP_DELETEPARTS::Make(
	DWORD dwPartsID)		/* [in] パーツID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwPartsID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_DELETEPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwPartsID, sizeof (dwPartsID), pDataTmp);	/* パーツID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_DELETEPARTS::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_DELETEPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwPartsID,	pDataTmp, sizeof (m_dwPartsID), pDataTmp);	/* パーツID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
