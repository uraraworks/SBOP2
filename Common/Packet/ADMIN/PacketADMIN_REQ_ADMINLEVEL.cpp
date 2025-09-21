/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_REQ_ADMINLEVEL.cpp								 */
/* 内容			:コマンド(管理者系:管理者権限レベル要求) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_REQ_ADMINLEVEL.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_REQ_ADMINLEVEL::CPacketADMIN_REQ_ADMINLEVEL		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

CPacketADMIN_REQ_ADMINLEVEL::CPacketADMIN_REQ_ADMINLEVEL()
{
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_REQ_ADMINLEVEL::~CPacketADMIN_REQ_ADMINLEVEL		 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

CPacketADMIN_REQ_ADMINLEVEL::~CPacketADMIN_REQ_ADMINLEVEL()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_REQ_ADMINLEVEL::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

void CPacketADMIN_REQ_ADMINLEVEL::Make(
	DWORD dwAccountID)		/* [in] アカウントID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwAccountID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_REQ_ADMINLEVEL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	/* アカウントID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_REQ_ADMINLEVEL::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

PBYTE CPacketADMIN_REQ_ADMINLEVEL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* アカウントID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
