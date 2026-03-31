/// @file PacketACCOUNT_REQ_MAKECHAR.cpp
/// @brief コマンド(アカウント系:キャラ作成要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/14
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Command.h"
#include "InfoCharBase.h"
#include "PacketACCOUNT_REQ_MAKECHAR.h"

CPacketACCOUNT_REQ_MAKECHAR::CPacketACCOUNT_REQ_MAKECHAR()
{
	m_pInfoCharBase = NULL;
}

CPacketACCOUNT_REQ_MAKECHAR::~CPacketACCOUNT_REQ_MAKECHAR()
{
	SAFE_DELETE(m_pInfoCharBase);
}

void CPacketACCOUNT_REQ_MAKECHAR::Make(
	DWORD dwAccountID,	// [in] アカウントID
	CInfoCharBase *pInfoCharBase)	// [in] キャラ情報
{
	PBYTE pData, pDataTmp, pCharData;
	DWORD dwSize, dwCharDataSize;
	PPACKETBASE pPacketBase;

	dwCharDataSize	= pInfoCharBase->GetSendDataSize();
	pCharData	= pInfoCharBase->GetSendData();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwAccountID)	+
			 dwCharDataSize;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_REQ_MAKECHAR;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	// アカウントID
	CopyMemoryRenew(pDataTmp, pCharData, dwCharDataSize, pDataTmp);	// キャラ情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pCharData);
}

PBYTE CPacketACCOUNT_REQ_MAKECHAR::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	SAFE_DELETE(m_pInfoCharBase);
	m_pInfoCharBase = new CInfoCharBase;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	// アカウントID
	pDataTmp = m_pInfoCharBase->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
