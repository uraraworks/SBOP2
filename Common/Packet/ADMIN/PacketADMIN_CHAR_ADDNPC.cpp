/// @file PacketADMIN_CHAR_ADDNPC.cpp
/// @brief コマンド(管理者系:NPCの追加) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/01
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHAR_ADDNPC.h"

CPacketADMIN_CHAR_ADDNPC::CPacketADMIN_CHAR_ADDNPC()
{
	m_pInfoChar = NULL;
}

CPacketADMIN_CHAR_ADDNPC::~CPacketADMIN_CHAR_ADDNPC()
{
	SAFE_DELETE(m_pInfoChar);
}

void CPacketADMIN_CHAR_ADDNPC::Make(
	CInfoCharBase *pInfo)	// [in] キャラ情報
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize();
	pInfoDataTmp	= pInfo->GetSendData();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_ADDNPC;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// キャラ情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_CHAR_ADDNPC::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfoChar);
	m_pInfoChar	= new CInfoCharBase;
	pDataTmp	= m_pInfoChar->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
