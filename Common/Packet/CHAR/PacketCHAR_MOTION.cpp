/// @file PacketCHAR_MOTION.cpp
/// @brief コマンド(管理者系:モーション情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/11/23
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "LibInfoMotion.h"
#include "Command.h"
#include "PacketCHAR_MOTION.h"

CPacketCHAR_MOTION::CPacketCHAR_MOTION()
{
	m_dwMotionTypeID = 0;
	m_dwMotionListID = 0;
	m_pInfo	 = NULL;
}

CPacketCHAR_MOTION::~CPacketCHAR_MOTION()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketCHAR_MOTION::Make(
	DWORD dwMotionTypeID,	// [in] モーション種別ID
	DWORD dwMotionListID,	// [in] モーションリストID
	CLibInfoMotion *pSrc)	// [in] モーション情報
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pSrc->GetSendDataSize(dwMotionTypeID, dwMotionListID);
	pInfoDataTmp	= pSrc->GetSendData(dwMotionTypeID, dwMotionListID);

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwMotionTypeID) +
			 sizeof (dwMotionListID) +
			 dwSizeDataTmp;

	pData = ZeroNew(dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_MOTION;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwMotionTypeID, sizeof (dwMotionTypeID), pDataTmp);	// モーション種別ID
	CopyMemoryRenew(pDataTmp, &dwMotionListID, sizeof (dwMotionListID), pDataTmp);	// モーションリストID
	CopyMemoryRenew(pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	 pDataTmp);	// モーション情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketCHAR_MOTION::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwMotionTypeID, pDataTmp, sizeof (m_dwMotionTypeID), pDataTmp);	// モーション種別ID
	CopyMemoryRenew(&m_dwMotionListID, pDataTmp, sizeof (m_dwMotionListID), pDataTmp);	// モーションリストID

	SAFE_DELETE(m_pInfo);
	m_pInfo	= new CLibInfoMotion;
	m_pInfo->Create();
	pDataTmp	= m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
