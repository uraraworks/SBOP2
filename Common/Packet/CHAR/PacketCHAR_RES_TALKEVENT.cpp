/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_RES_TALKEVENT.cpp								 */
/* 内容			:コマンド(会話イベント系:会話イベント情報応答) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/27													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoTalkEvent.h"
#include "Command.h"
#include "PacketCHAR_RES_TALKEVENT.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_TALKEVENT::CPacketCHAR_RES_TALKEVENT			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

CPacketCHAR_RES_TALKEVENT::CPacketCHAR_RES_TALKEVENT()
{
	m_dwParam	= 0;
	m_pInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_TALKEVENT::~CPacketCHAR_RES_TALKEVENT			 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

CPacketCHAR_RES_TALKEVENT::~CPacketCHAR_RES_TALKEVENT()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_TALKEVENT::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

void CPacketCHAR_RES_TALKEVENT::Make(
	CInfoTalkEvent *pInfo,		/* [in] 会話イベント情報 */
	DWORD dwParam)				/* [in] パラメータ */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwParam)		+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_RES_TALKEVENT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwParam,		sizeof (dwParam),	pDataTmp);	/* パラメータ */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,		pDataTmp);	/* 会話イベント情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_TALKEVENT::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

PBYTE CPacketCHAR_RES_TALKEVENT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwParam, pDataTmp, sizeof (m_dwParam), pDataTmp);	/* パラメータ */

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoTalkEvent;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
