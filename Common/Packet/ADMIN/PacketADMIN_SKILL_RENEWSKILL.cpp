/// @file PacketADMIN_SKILL_RENEWSKILL.cpp
/// @brief コマンド(管理者系:スキル情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/08
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "LibInfoSkill.h"
#include "Command.h"
#include "PacketADMIN_SKILL_RENEWSKILL.h"

CPacketADMIN_SKILL_RENEWSKILL::CPacketADMIN_SKILL_RENEWSKILL()
{
	m_pInfo = NULL;
}

CPacketADMIN_SKILL_RENEWSKILL::~CPacketADMIN_SKILL_RENEWSKILL()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketADMIN_SKILL_RENEWSKILL::Make(
	CInfoSkillBase *pInfo)	// [in] スキル情報
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize();
	pInfoDataTmp	= pInfo->GetSendData();

	dwSize = sizeof (PACKETBASE)	+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_SKILL_RENEWSKILL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// スキル情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_SKILL_RENEWSKILL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CLibInfoSkill LibInfo;
	CInfoSkillBase InfoTmp;

	pRet	 = pPacket;
	pDataTmp	 = CPacketBase::Set(pPacket);

	pDataTmpBack = pDataTmp;
	InfoTmp.SetSendData(pDataTmp);

	SAFE_DELETE(m_pInfo);
	m_pInfo	= (PCInfoSkillBase)LibInfo.GetNew(InfoTmp.m_nTypeMain, InfoTmp.m_nTypeSub);
	pDataTmp	= m_pInfo->SetSendData(pDataTmpBack);

	pRet = pDataTmp;
	return pRet;
}
