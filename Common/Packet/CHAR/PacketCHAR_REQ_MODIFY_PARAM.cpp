/// @file PacketCHAR_REQ_MODIFY_PARAM.cpp
/// @brief コマンド(キャラ系:パラメータ変更要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/09
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_MODIFY_PARAM.h"

CPacketCHAR_REQ_MODIFY_PARAM::CPacketCHAR_REQ_MODIFY_PARAM()
{
	m_dwCharID	= 0;
	m_nType	= PARAMID_CHAR_REQ_MODIFY_NONE;
	m_nParam	= 0;
}

CPacketCHAR_REQ_MODIFY_PARAM::~CPacketCHAR_REQ_MODIFY_PARAM()
{
}

void CPacketCHAR_REQ_MODIFY_PARAM::Make(
	DWORD dwCharID,	// [in] キャラID
	int nType,	// [in] 種別
	int nParam)	// [in] パラメータ
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)	+
			 sizeof (nType)	+
			 sizeof (nParam);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_MODIFY_PARAM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &nType,	sizeof (nType),	pDataTmp);	// 種別
	CopyMemoryRenew(pDataTmp, &nParam,	sizeof (nParam),	pDataTmp);	// パラメータ

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_REQ_MODIFY_PARAM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&m_nType,	pDataTmp, sizeof (m_nType),	pDataTmp);	// 種別
	CopyMemoryRenew(&m_nParam,	pDataTmp, sizeof (m_nParam),	pDataTmp);	// パラメータ

	pRet = pDataTmp;
	return pRet;
}
