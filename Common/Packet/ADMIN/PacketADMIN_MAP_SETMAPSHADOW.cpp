/// @file PacketADMIN_MAP_SETMAPSHADOW.cpp
/// @brief コマンド(管理者系:マップ影配置) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/07
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoMapShadow.h"
#include "Command.h"
#include "PacketADMIN_MAP_SETMAPSHADOW.h"

CPacketADMIN_MAP_SETMAPSHADOW::CPacketADMIN_MAP_SETMAPSHADOW()
{
	m_ptPos.x	= 0;
	m_ptPos.y	= 0;
	m_dwMapID	= 0;
	m_dwShadowID	= 0;
}

CPacketADMIN_MAP_SETMAPSHADOW::~CPacketADMIN_MAP_SETMAPSHADOW()
{
}

void CPacketADMIN_MAP_SETMAPSHADOW::Make(
	DWORD dwMapID,	// [in] マップID
	int x,	// [in] X座標
	int y,	// [in] Y座標
	DWORD dwShadowID)	// [in] マップ影ID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	POINT ptPos;
	PPACKETBASE pPacketBase;

	ptPos.x = x;
	ptPos.y = y;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (ptPos)	+
			 sizeof (dwMapID)	+
			 sizeof (dwShadowID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_SETMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwMapID,	sizeof (dwMapID),	pDataTmp);	// マップID
	CopyMemoryRenew(pDataTmp, &ptPos,	sizeof (ptPos),	pDataTmp);	// 座標
	CopyMemoryRenew(pDataTmp, &dwShadowID,	sizeof (dwShadowID),	pDataTmp);	// マップ影ID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_MAP_SETMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),	pDataTmp);	// マップID
	CopyMemoryRenew(&m_ptPos,	pDataTmp, sizeof (m_ptPos),	pDataTmp);	// 座標
	CopyMemoryRenew(&m_dwShadowID,	pDataTmp, sizeof (m_dwShadowID),	pDataTmp);	// マップ影ID

	pRet = pDataTmp;
	return pRet;
}
