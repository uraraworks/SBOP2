/// @file PacketADMIN_PARA2.cpp
/// @brief コマンド(管理者系:汎用[パラメータ2個]) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/25
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_PARA2.h"

CPacketADMIN_PARA2::CPacketADMIN_PARA2()
{
	m_dwPara1	= 0;
	m_dwPara2	= 0;
}

CPacketADMIN_PARA2::~CPacketADMIN_PARA2()
{
}

void CPacketADMIN_PARA2::Make(
	BYTE byCmdSub,	// [in] コマンドIDサブ
	DWORD dwPara1/*=0*/,	// [in] パラメータ1
	DWORD dwPara2/*=0*/)	// [in] パラメータ2
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwPara1)	+
			 sizeof (dwPara2);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= byCmdSub;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwPara1, sizeof (dwPara1), pDataTmp);	// パラメータ1
	CopyMemoryRenew(pDataTmp, &dwPara2, sizeof (dwPara2), pDataTmp);	// パラメータ2

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_PARA2::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwPara1, pDataTmp, sizeof (m_dwPara1), pDataTmp);	// パラメータ1
	CopyMemoryRenew(&m_dwPara2, pDataTmp, sizeof (m_dwPara2), pDataTmp);	// パラメータ2

	pRet = pDataTmp;
	return pRet;
}
