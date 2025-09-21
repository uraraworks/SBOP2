/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_DELETEMAPSHADOW.cpp								 */
/* 内容			:コマンド(マップ系:マップ影削除) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_DELETEMAPSHADOW.h"


/* ========================================================================= */
/* 関数名	:CPacketMAP_DELETEMAPSHADOW::CPacketMAP_DELETEMAPSHADOW			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/08														 */
/* ========================================================================= */

CPacketMAP_DELETEMAPSHADOW::CPacketMAP_DELETEMAPSHADOW()
{
	m_dwShadowID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_DELETEMAPSHADOW::~CPacketMAP_DELETEMAPSHADOW		 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/08														 */
/* ========================================================================= */

CPacketMAP_DELETEMAPSHADOW::~CPacketMAP_DELETEMAPSHADOW()
{
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_DELETEMAPSHADOW::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/06/08														 */
/* ========================================================================= */

void CPacketMAP_DELETEMAPSHADOW::Make(
	DWORD dwShadowID)		/* [in] マップ影ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwShadowID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_DELETEMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwShadowID, sizeof (dwShadowID), pDataTmp);	/* マップ影ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_DELETEMAPSHADOW::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/06/08														 */
/* ========================================================================= */

PBYTE CPacketMAP_DELETEMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwShadowID,	pDataTmp, sizeof (m_dwShadowID), pDataTmp);	/* マップ影ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
