/// @file PacketMSGCMD_CHGCLOTH.cpp
/// @brief コマンド(メッセージコマンド系:服装変更) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/02
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketMSGCMD_CHGCLOTH.h"

CPacketMSGCMD_CHGCLOTH::CPacketMSGCMD_CHGCLOTH()
{
	m_dwCharID	= 0;
	m_nType	= 0;
}

CPacketMSGCMD_CHGCLOTH::~CPacketMSGCMD_CHGCLOTH()
{
}

void CPacketMSGCMD_CHGCLOTH::Make(
	DWORD dwCharID,	// [in] キャラID
	int nType)	// [in] 種別
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)	+
			 sizeof (nType);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MSGCMD;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MSGCMD_CHGCLOTH;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &nType,	sizeof (nType),	pDataTmp);	// 種別

	RenewPacket(pData, dwSize);
}

PBYTE CPacketMSGCMD_CHGCLOTH::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&m_nType,	pDataTmp, sizeof (m_nType),	pDataTmp);	// 種別

	pRet = pDataTmp;
	return pRet;
}
