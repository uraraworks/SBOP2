/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketCONNECT_RES_LOGIN.cpp								 */
/* 内容			:コマンド(接続系:ログイン応答) 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_RES_LOGIN.h"

/* ========================================================================= */
/* 関数名	:CPacketCONNECT_RES_LOGIN::CPacketCONNECT_RES_LOGIN				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CPacketCONNECT_RES_LOGIN::CPacketCONNECT_RES_LOGIN()
{
	m_nResult		= LOGINRES_NG_PASSWORD;
	m_dwAccountID	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_RES_LOGIN::~CPacketCONNECT_RES_LOGIN			 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CPacketCONNECT_RES_LOGIN::~CPacketCONNECT_RES_LOGIN()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_RES_LOGIN::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CPacketCONNECT_RES_LOGIN::Make(
	int nResult,			/* [in] 結果 */
	DWORD dwAccountID)		/* [in] アカウントID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (nResult)		+
			 sizeof (dwAccountID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_RES_LOGIN;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nResult,		sizeof (nResult),		pDataTmp);	/* 結果 */
	CopyMemoryRenew (pDataTmp, &dwAccountID,	sizeof (dwAccountID),	pDataTmp);	/* アカウントID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_RES_LOGIN::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

PBYTE CPacketCONNECT_RES_LOGIN::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nResult,		pDataTmp, sizeof (m_nResult),		pDataTmp);	/* 結果 */
	CopyMemoryRenew (&m_dwAccountID,	pDataTmp, sizeof (m_dwAccountID),	pDataTmp);	/* アカウントID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
