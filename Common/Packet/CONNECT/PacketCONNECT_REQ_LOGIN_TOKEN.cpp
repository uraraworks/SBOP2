/// @file PacketCONNECT_REQ_LOGIN_TOKEN.cpp
/// @brief コマンド(接続系:端末トークンログイン要求) 実装ファイル

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_REQ_LOGIN_TOKEN.h"

CPacketCONNECT_REQ_LOGIN_TOKEN::CPacketCONNECT_REQ_LOGIN_TOKEN()
{
	ZeroMemory(m_byMacAddr, sizeof (m_byMacAddr));
}

CPacketCONNECT_REQ_LOGIN_TOKEN::~CPacketCONNECT_REQ_LOGIN_TOKEN()
{
}

void CPacketCONNECT_REQ_LOGIN_TOKEN::Make(
	LPCSTR pszDeviceToken,	// [in] 端末トークン
	PBYTE pbyMacAddr)	// [in] MACアドレス
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 (strlen(pszDeviceToken) + 1)	+
			 6;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_REQ_LOGIN_TOKEN;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strcpyRenew((LPSTR)pDataTmp, pszDeviceToken, pDataTmp);	// 端末トークン
	CopyMemoryRenew(pDataTmp, pbyMacAddr, 6, pDataTmp);	// MACアドレス

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCONNECT_REQ_LOGIN_TOKEN::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	StoreRenew(m_strDeviceToken,	(LPCSTR)pDataTmp, pDataTmp);	// 端末トークン
	CopyMemoryRenew(m_byMacAddr, pDataTmp, 6, pDataTmp);	// MACアドレス

	pRet = pDataTmp;
	return pRet;
}
