/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketSKILL_SKILLINFO.cpp									 */
/* 内容			:コマンド(スキル系:スキル情報通知) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/06													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketSKILL_SKILLINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketSKILL_SKILLINFO::CPacketSKILL_SKILLINFO					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

CPacketSKILL_SKILLINFO::CPacketSKILL_SKILLINFO()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketSKILL_SKILLINFO::~CPacketSKILL_SKILLINFO				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

CPacketSKILL_SKILLINFO::~CPacketSKILL_SKILLINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketSKILL_SKILLINFO::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

void CPacketSKILL_SKILLINFO::Make(
	PCLibInfoSkill pInfo)		/* [in] スキル情報 */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_SKILL;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_SKILL_SKILLINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* スキル情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketSKILL_SKILLINFO::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/12/08														 */
/* ========================================================================= */

void CPacketSKILL_SKILLINFO::Make(
	CInfoSkillBase *pInfo)		/* [in] スキル情報 */
{
	PCInfoSkillBase pInfoTmp;
	CLibInfoSkill LibInfoSkill;

	LibInfoSkill.Create ();

	pInfoTmp = (PCInfoSkillBase)LibInfoSkill.GetNew (pInfo->m_nType);
	pInfoTmp->Copy (pInfo);
	LibInfoSkill.Add (pInfoTmp);

	Make (&LibInfoSkill);
}


/* ========================================================================= */
/* 関数名	:CPacketSKILL_SKILLINFO::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

PBYTE CPacketSKILL_SKILLINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoSkill;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
