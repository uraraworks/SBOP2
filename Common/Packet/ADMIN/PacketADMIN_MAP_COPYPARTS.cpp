/// @file PacketADMIN_MAP_COPYPARTS.cpp
/// @brief コマンド(管理者系:マップパーツコピー) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/14
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketADMIN_MAP_COPYPARTS.h"

CPacketADMIN_MAP_COPYPARTS::CPacketADMIN_MAP_COPYPARTS()
{
	m_ptViewPos.x	= 0;
	m_ptViewPos.y	= 0;
	m_dwSrcPartsID	= 0;
}

CPacketADMIN_MAP_COPYPARTS::~CPacketADMIN_MAP_COPYPARTS()
{
}

void CPacketADMIN_MAP_COPYPARTS::Make(
	DWORD dwSrcPartsID,	// [in] コピー元パーツID
	POINT ptViewPos)	// [in] 表示位置
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwSrcPartsID)	+
			 sizeof (ptViewPos);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_COPYPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwSrcPartsID,	sizeof (dwSrcPartsID),	pDataTmp);	// コピー元パーツID
	CopyMemoryRenew(pDataTmp, &ptViewPos,	sizeof (ptViewPos),	pDataTmp);	// 表示位置

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_MAP_COPYPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwSrcPartsID,	pDataTmp, sizeof (m_dwSrcPartsID),	pDataTmp);	// パーツID
	CopyMemoryRenew(&m_ptViewPos,	pDataTmp, sizeof (m_ptViewPos),	pDataTmp);	// 座標

	pRet = pDataTmp;
	return pRet;
}
