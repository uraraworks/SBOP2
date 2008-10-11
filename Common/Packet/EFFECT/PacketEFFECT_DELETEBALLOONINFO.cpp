/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_DELETEBALLOONINFO.cpp							 */
/* 内容			:コマンド(噴出し系:噴出し情報削除) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketEFFECT_DELETEBALLOONINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketEFFECT_DELETEBALLOONINFO::CPacketEFFECT_DELETEBALLOONINFO */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_DELETEBALLOONINFO::CPacketEFFECT_DELETEBALLOONINFO()
{
	m_dwBalloonID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_DELETEBALLOONINFO::~CPacketEFFECT_DELETEBALLOONINFO */
/* 内容		:デストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_DELETEBALLOONINFO::~CPacketEFFECT_DELETEBALLOONINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_DELETEBALLOONINFO::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CPacketEFFECT_DELETEBALLOONINFO::Make(
	DWORD dwBalloonID)		/* [in] 噴出しID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwBalloonID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_DELETEBALLOONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwBalloonID, sizeof (dwBalloonID), pDataTmp);	/* 噴出しID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_DELETEBALLOONINFO::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_DELETEBALLOONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwBalloonID, pDataTmp, sizeof (m_dwBalloonID), pDataTmp);	/* 噴出しID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
