/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_RES_CHARINFO.cpp								 */
/* 内容			:コマンド(キャラ系:キャラ情報応答) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketCHAR_RES_CHARINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_CHARINFO::CPacketCHAR_RES_CHARINFO				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

CPacketCHAR_RES_CHARINFO::CPacketCHAR_RES_CHARINFO()
{
	m_bChgScreenPos = FALSE;
	m_pInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_CHARINFO::~CPacketCHAR_RES_CHARINFO			 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

CPacketCHAR_RES_CHARINFO::~CPacketCHAR_RES_CHARINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_CHARINFO::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

void CPacketCHAR_RES_CHARINFO::Make(
	CInfoCharBase *pInfo,		/* [in] キャラ情報 */
	BOOL bChgScreenPos)			/* [in] 画面位置変更 */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (bChgScreenPos)	+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_RES_CHARINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &bChgScreenPos,	sizeof (bChgScreenPos),	pDataTmp);	/* 画面位置変更 */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,			pDataTmp);	/* キャラ情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_CHARINFO::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_RES_CHARINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_bChgScreenPos, pDataTmp, sizeof (m_bChgScreenPos), pDataTmp);	/* 画面位置変更 */

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoCharBase;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
