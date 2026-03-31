/// @file PacketCHAR_REQ_CHAT.cpp
/// @brief コマンド(キャラ系:チャット要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/02/04
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_CHAT.h"

CPacketCHAR_REQ_CHAT::CPacketCHAR_REQ_CHAT()
{
	m_nType	= CHATTYPE_NORMAL;
	m_dwCharID	= 0;
}

CPacketCHAR_REQ_CHAT::~CPacketCHAR_REQ_CHAT()
{
}

void CPacketCHAR_REQ_CHAT::Make(
	int nType,	// [in] チャット種別
	DWORD dwCharID,	// [in] キャラID
	LPCSTR pszChat)	// [in] 発言内容
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (nType)	+
			 sizeof (dwCharID)	+
			 (strlen(pszChat) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_CHAT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &nType,	sizeof (nType),	pDataTmp);	// チャット種別
	strcpyRenew((LPSTR)pDataTmp, pszChat, pDataTmp);	// 発言内容

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_REQ_CHAT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&m_nType,	pDataTmp, sizeof (m_nType),	pDataTmp);	// チャット種別
	StoreRenew(m_strChat, (LPCSTR)pDataTmp, pDataTmp);	// 発言内容

	pRet = pDataTmp;
	return pRet;
}
