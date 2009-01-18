/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_MODIFYSKILL.cpp							 */
/* 内容			:コマンド(管理者系:所持スキルの変更) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/18													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHAR_MODIFYSKILL.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_MODIFYSKILL::CPacketADMIN_CHAR_MODIFYSKILL	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

CPacketADMIN_CHAR_MODIFYSKILL::CPacketADMIN_CHAR_MODIFYSKILL()
{
	m_dwCharID	= 0;
	m_dwSkillID	= 0;
	m_nType		= CHARMODIFYSKILLTYPE_NONE;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_MODIFYSKILL::~CPacketADMIN_CHAR_MODIFYSKILL	 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

CPacketADMIN_CHAR_MODIFYSKILL::~CPacketADMIN_CHAR_MODIFYSKILL()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_MODIFYSKILL::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_MODIFYSKILL::Make(
	DWORD dwCharID,			/* [in] キャラID */
	DWORD dwSkillID,		/* [in] スキルID */
	int nType)				/* [in] 種別 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwSkillID)		+
			 sizeof (nType);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_MODIFYSKILL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwSkillID,	sizeof (dwSkillID),	pDataTmp);	/* スキルID */
	CopyMemoryRenew (pDataTmp, &nType,		sizeof (nType),		pDataTmp);	/* 種別 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_MODIFYSKILL::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2009/01/18														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_MODIFYSKILL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwSkillID,	pDataTmp, sizeof (m_dwSkillID),	pDataTmp);	/* スキルID */
	CopyMemoryRenew (&m_nType,		pDataTmp, sizeof (m_nType),		pDataTmp);	/* 種別 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2009 */
