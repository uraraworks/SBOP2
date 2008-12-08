/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_SKILL_RENEWSKILL.cpp							 */
/* 内容			:コマンド(管理者系:スキル情報通知) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoSkill.h"
#include "Command.h"
#include "PacketADMIN_SKILL_RENEWSKILL.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_SKILL_RENEWSKILL::CPacketADMIN_SKILL_RENEWSKILL	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/08														 */
/* ========================================================================= */

CPacketADMIN_SKILL_RENEWSKILL::CPacketADMIN_SKILL_RENEWSKILL()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_SKILL_RENEWSKILL::~CPacketADMIN_SKILL_RENEWSKILL	 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/08														 */
/* ========================================================================= */

CPacketADMIN_SKILL_RENEWSKILL::~CPacketADMIN_SKILL_RENEWSKILL()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_SKILL_RENEWSKILL::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/12/08														 */
/* ========================================================================= */

void CPacketADMIN_SKILL_RENEWSKILL::Make(
	CInfoSkillBase *pInfo)		/* [in] スキル情報 */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_SKILL_RENEWSKILL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* スキル情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_SKILL_RENEWSKILL::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/12/08														 */
/* ========================================================================= */

PBYTE CPacketADMIN_SKILL_RENEWSKILL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CLibInfoSkill LibInfo;
	CInfoSkillBase InfoTmp;

	pRet		 = pPacket;
	pDataTmp	 = CPacketBase::Set (pPacket);

	pDataTmpBack = pDataTmp;
	InfoTmp.SetSendData (pDataTmp);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= (PCInfoSkillBase)LibInfo.GetNew (InfoTmp.m_nType);
	pDataTmp	= m_pInfo->SetSendData (pDataTmpBack);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
