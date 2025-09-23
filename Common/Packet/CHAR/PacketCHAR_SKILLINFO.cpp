/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_SKILLINFO.cpp									 */
/* 内容			:コマンド(キャラ系:スキル情報通知) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_SKILLINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_SKILLINFO::CPacketCHAR_SKILLINFO					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

CPacketCHAR_SKILLINFO::CPacketCHAR_SKILLINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_SKILLINFO::~CPacketCHAR_SKILLINFO					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

CPacketCHAR_SKILLINFO::~CPacketCHAR_SKILLINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_SKILLINFO::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

void CPacketCHAR_SKILLINFO::Make(
	DWORD dwCharID,			/* [in] キャラID */
	PARRAYDWORD paSrc)		/* [in] スキルID */
{
	PBYTE pData, pDataTmp;
	DWORD i, dwSize, dwCount, dwTmp;
	PPACKETBASE pPacketBase;

	dwCount = paSrc->size();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (DWORD)			+
			 (sizeof (DWORD) * dwCount);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_SKILLINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwCount,	sizeof (dwCount),	pDataTmp);	/* 個数 */
	for (i = 0; i < dwCount; i ++) {
		dwTmp = paSrc->at(i);
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);		/* スキルID */
	}

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_SKILLINFO::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_SKILLINFO::Set(PBYTE pPacket)
{
	DWORD i, dwCount, dwTmp;
	PBYTE pRet, pDataTmp;

	m_adwSkillID.clear();

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (&dwCount,		pDataTmp, sizeof (dwCount),		pDataTmp);	/* 個数 */
	for (i = 0; i < dwCount; i ++) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);			/* スキルID */
		m_adwSkillID.push_back (dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
