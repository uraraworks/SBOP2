/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHARINFO.cpp									 */
/* 内容			:コマンド(管理者系:キャラ情報通知) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/03/18													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHARINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHARINFO::CPacketADMIN_CHARINFO					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

CPacketADMIN_CHARINFO::CPacketADMIN_CHARINFO()
{
	m_bChgScreenPos = FALSE;
	m_pInfoChar = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHARINFO::~CPacketADMIN_CHARINFO					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

CPacketADMIN_CHARINFO::~CPacketADMIN_CHARINFO()
{
	SAFE_DELETE (m_pInfoChar);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHARINFO::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CPacketADMIN_CHARINFO::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHARINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &bChgScreenPos,	sizeof (bChgScreenPos),	pDataTmp);	/* 画面位置変更 */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,			pDataTmp);	/* キャラ情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHARINFO::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHARINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_bChgScreenPos, pDataTmp, sizeof (m_bChgScreenPos), pDataTmp);	/* 画面位置変更 */

	SAFE_DELETE (m_pInfoChar);
	m_pInfoChar	= new CInfoCharBase;
	pDataTmp	= m_pInfoChar->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
