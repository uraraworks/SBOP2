/// @file PacketBase.cpp
/// @brief コマンド基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/08/01
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "PacketBase.h"

CPacketBase::CPacketBase()
{
	m_pPacket	= NULL;
	m_dwPacketSize	= 0;
	m_byCmdMain	= 0;
	m_byCmdSub	= 0;
}

CPacketBase::~CPacketBase()
{
	SAFE_DELETE_ARRAY(m_pPacket);
}

DWORD CPacketBase::GetSize(void)
{
	return m_dwPacketSize;
}

PBYTE CPacketBase::Get(void)
{
	return m_pPacket;
}

PBYTE CPacketBase::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	PACKETBASE stPacketBase;

	pRet = pDataTmp = pPacket;

	CopyMemoryRenew((PBYTE)&stPacketBase, pDataTmp, sizeof (stPacketBase), pDataTmp);

	m_byCmdMain	= stPacketBase.byCmdMain;	// コマンドID(メイン)
	m_byCmdSub	= stPacketBase.byCmdSub;	// コマンドID(サブ)

	pRet = pDataTmp;
	return pRet;
}

void CPacketBase::RenewPacket(PBYTE pPacket, DWORD dwSize)
{
	SAFE_DELETE_ARRAY(m_pPacket);

	m_pPacket	= pPacket;
	m_dwPacketSize	= dwSize;
}
