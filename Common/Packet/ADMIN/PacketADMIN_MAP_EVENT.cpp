/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_EVENT.cpp									 */
/* 内容			:コマンド(管理者系:マップイベント情報通知) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/25													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapEvent.h"
#include "Command.h"
#include "PacketADMIN_MAP_EVENT.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_EVENT::CPacketADMIN_MAP_EVENT					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

CPacketADMIN_MAP_EVENT::CPacketADMIN_MAP_EVENT()
{
	m_dwMapID	= 0;
	m_pInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_EVENT::~CPacketADMIN_MAP_EVENT				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

CPacketADMIN_MAP_EVENT::~CPacketADMIN_MAP_EVENT()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_EVENT::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

void CPacketADMIN_MAP_EVENT::Make(
	DWORD dwMapID,					/* [in] マップID */
	CInfoMapEventBase *pInfo)		/* [in] イベント情報 */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_RENEWEVENT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,		sizeof (dwMapID),	pDataTmp);	/* マップID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,		pDataTmp);	/* イベント情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_EVENT::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_EVENT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CLibInfoMapEvent LibInfo;
	CInfoMapEventBase InfoTmp;

	pRet		 = pPacket;
	pDataTmp	 = CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	/* マップID */
	pDataTmpBack = pDataTmp;
	InfoTmp.SetSendData (pDataTmp);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= (PCInfoMapEventBase)LibInfo.GetNew (InfoTmp.m_nType);
	pDataTmp	= m_pInfo->SetSendData (pDataTmpBack);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
