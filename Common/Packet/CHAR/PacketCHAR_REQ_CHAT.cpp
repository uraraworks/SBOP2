/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_CHAT.cpp									 */
/* 内容			:コマンド(キャラ系:チャット要求) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/02/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_CHAT.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHAT::CPacketCHAR_REQ_CHAT						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/02/04														 */
/* ========================================================================= */

CPacketCHAR_REQ_CHAT::CPacketCHAR_REQ_CHAT()
{
	m_nType		= CHATTYPE_NORMAL;
	m_dwCharID	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHAT::~CPacketCHAR_REQ_CHAT					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/02/04														 */
/* ========================================================================= */

CPacketCHAR_REQ_CHAT::~CPacketCHAR_REQ_CHAT()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHAT::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/02/04														 */
/* ========================================================================= */

void CPacketCHAR_REQ_CHAT::Make(
	int nType,			/* [in] チャット種別 */
	DWORD dwCharID,		/* [in] キャラID */
	LPCSTR pszChat)		/* [in] 発言内容 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (nType)			+
			 sizeof (dwCharID)		+
			 (strlen (pszChat) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_CHAT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &nType,		sizeof (nType),		pDataTmp);	/* チャット種別 */
	strcpyRenew ((LPSTR)pDataTmp, pszChat, pDataTmp);						/* 発言内容 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHAT::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/02/04														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_CHAT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_nType,		pDataTmp, sizeof (m_nType),		pDataTmp);	/* チャット種別 */
	StoreRenew (m_strChat, (LPCSTR)pDataTmp, pDataTmp);							/* 発言内容 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
