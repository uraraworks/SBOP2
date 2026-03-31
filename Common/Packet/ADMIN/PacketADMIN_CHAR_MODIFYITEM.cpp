/// @file PacketADMIN_CHAR_MODIFYITEM.cpp
/// @brief コマンド(管理者系:所持アイテムの変更) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/24
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHAR_MODIFYITEM.h"

CPacketADMIN_CHAR_MODIFYITEM::CPacketADMIN_CHAR_MODIFYITEM()
{
	m_dwCharID	= 0;
	m_dwItemID	= 0;
	m_nType	= CHARMODIFYITEMTYPE_NONE;
}

CPacketADMIN_CHAR_MODIFYITEM::~CPacketADMIN_CHAR_MODIFYITEM()
{
}

void CPacketADMIN_CHAR_MODIFYITEM::Make(
	DWORD dwCharID,	// [in] キャラID
	DWORD dwItemID,	// [in] アイテムID
	int nType)	// [in] 種別
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)	+
			 sizeof (dwItemID)	+
			 sizeof (nType);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_MODIFYITEM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &dwItemID,	sizeof (dwItemID),	pDataTmp);	// アイテムID
	CopyMemoryRenew(pDataTmp, &nType,	sizeof (nType),	pDataTmp);	// 種別

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_CHAR_MODIFYITEM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&m_dwItemID,	pDataTmp, sizeof (m_dwItemID),	pDataTmp);	// アイテムID
	CopyMemoryRenew(&m_nType,	pDataTmp, sizeof (m_nType),	pDataTmp);	// 種別

	pRet = pDataTmp;
	return pRet;
}
