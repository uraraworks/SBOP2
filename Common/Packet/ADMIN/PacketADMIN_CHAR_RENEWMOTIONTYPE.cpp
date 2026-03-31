/// @file PacketADMIN_CHAR_RENEWMOTIONTYPE.cpp
/// @brief コマンド(管理者系:キャラモーション種別情報の更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/05/31
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "LibInfoMotionType.h"
#include "Command.h"
#include "PacketADMIN_CHAR_RENEWMOTIONTYPE.h"

CPacketADMIN_CHAR_RENEWMOTIONTYPE::CPacketADMIN_CHAR_RENEWMOTIONTYPE()
{
	m_dwMotionTypeID = 0;
	m_pInfo	 = NULL;
}

CPacketADMIN_CHAR_RENEWMOTIONTYPE::~CPacketADMIN_CHAR_RENEWMOTIONTYPE()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketADMIN_CHAR_RENEWMOTIONTYPE::Make(
	DWORD dwMotionTypeID,	// [in] モーション種別ID
	CLibInfoMotionType *pSrc)	// [in] モーション種別情報
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pSrc->GetSendDataSize(dwMotionTypeID);
	pInfoDataTmp	= pSrc->GetSendData(dwMotionTypeID);

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwMotionTypeID) +
			 dwSizeDataTmp;

	pData = ZeroNew(dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWMOTIONTYPE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwMotionTypeID, sizeof (dwMotionTypeID), pDataTmp);	// モーション種別ID
	CopyMemoryRenew(pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	 pDataTmp);	// モーション種別情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_CHAR_RENEWMOTIONTYPE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwMotionTypeID, pDataTmp, sizeof (m_dwMotionTypeID), pDataTmp);	// モーション種別ID

	SAFE_DELETE(m_pInfo);
	m_pInfo	= new CLibInfoMotionType;
	m_pInfo->Create();
	pDataTmp	= m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
