/// @file PacketMAP_SETPARTS.cpp
/// @brief コマンド(マップ系:マップパーツ配置) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketMAP_SETPARTS.h"

CPacketMAP_SETPARTS::CPacketMAP_SETPARTS()
{
	m_bPile	= FALSE;
	m_ptPos.x	= 0;
	m_ptPos.y	= 0;
	m_dwMapID	= 0;
	m_dwPartsID	= 0;
}

CPacketMAP_SETPARTS::~CPacketMAP_SETPARTS()
{
}

void CPacketMAP_SETPARTS::Make(
	DWORD dwMapID,	// [in] マップID
	int x,	// [in] X座標
	int y,	// [in] Y座標
	DWORD dwPartsID,	// [in] パーツID
	BOOL bPile)	// [in] 重ね合わせ用
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
			 sizeof (dwPartsID)	+
			 sizeof (bPile);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_SETPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwMapID,	sizeof (dwMapID),	pDataTmp);	// マップID
	CopyMemoryRenew(pDataTmp, &ptPos,	sizeof (ptPos),	pDataTmp);	// 座標
	CopyMemoryRenew(pDataTmp, &dwPartsID,	sizeof (dwPartsID),	pDataTmp);	// パーツID
	CopyMemoryRenew(pDataTmp, &bPile,	sizeof (bPile),	pDataTmp);	// 重ね合わせ用

	RenewPacket(pData, dwSize);
}

PBYTE CPacketMAP_SETPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),	pDataTmp);	// マップID
	CopyMemoryRenew(&m_ptPos,	pDataTmp, sizeof (m_ptPos),	pDataTmp);	// 座標
	CopyMemoryRenew(&m_dwPartsID,	pDataTmp, sizeof (m_dwPartsID),	pDataTmp);	// パーツID
	CopyMemoryRenew(&m_bPile,	pDataTmp, sizeof (m_bPile),	pDataTmp);	// 重ね合わせ用

	pRet = pDataTmp;
	return pRet;
}
