/// @file PacketSKILL_SKILLINFO.cpp
/// @brief コマンド(スキル系:スキル情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/06
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketSKILL_SKILLINFO.h"

CPacketSKILL_SKILLINFO::CPacketSKILL_SKILLINFO()
{
	m_pLibInfo = NULL;
}

CPacketSKILL_SKILLINFO::~CPacketSKILL_SKILLINFO()
{
	SAFE_DELETE(m_pLibInfo);
}

void CPacketSKILL_SKILLINFO::Make(
	PCLibInfoSkill pInfo)	// [in] スキル情報
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_SKILL;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_SKILL_SKILLINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// スキル情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

void CPacketSKILL_SKILLINFO::Make(
	CInfoSkillBase *pInfo)	// [in] スキル情報
{
	PCInfoSkillBase pInfoTmp;
	CLibInfoSkill LibInfoSkill;

	LibInfoSkill.Create();

	pInfoTmp = (PCInfoSkillBase)LibInfoSkill.GetNew(pInfo->m_nTypeMain, pInfo->m_nTypeSub);
	pInfoTmp->Copy(pInfo);
	LibInfoSkill.Add(pInfoTmp);

	Make(&LibInfoSkill);
}

PBYTE CPacketSKILL_SKILLINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pLibInfo);
	m_pLibInfo	= new CLibInfoSkill;
	m_pLibInfo->Create();
	pDataTmp	= m_pLibInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
