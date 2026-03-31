/// @file PacketCHAR_PARA1.cpp
/// @brief コマンド(キャラ系:汎用[パラメータ1個]) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/01
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_PARA1.h"

CPacketCHAR_PARA1::CPacketCHAR_PARA1()
{
	m_dwCharID	= 0;
	m_dwPara	= 0;
}

CPacketCHAR_PARA1::~CPacketCHAR_PARA1()
{
}

void CPacketCHAR_PARA1::Make(
	BYTE byCmdSub,	// [in] コマンドIDサブ
	DWORD dwCharID,	// [in] キャラID
	DWORD dwPara)	// [in] パラメータ
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)	+
			 sizeof (dwPara);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= byCmdSub;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &dwPara,	sizeof (dwPara),	pDataTmp);	// パラメータ

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_PARA1::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&m_dwPara,	pDataTmp, sizeof (m_dwPara),	pDataTmp);	// パラメータ

	pRet = pDataTmp;
	return pRet;
}
