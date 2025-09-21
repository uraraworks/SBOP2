/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_MODIFY_PARAM.cpp								 */
/* 内容			:コマンド(キャラ系:パラメータ変更通知) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/09													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_MODIFY_PARAM.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_MODIFY_PARAM::CPacketCHAR_MODIFY_PARAM				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

CPacketCHAR_MODIFY_PARAM::CPacketCHAR_MODIFY_PARAM()
{
	m_dwCharID	= 0;
	m_nType		= PARAMID_CHAR_MODIFY_NONE;
	m_nParam	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_MODIFY_PARAM::~CPacketCHAR_MODIFY_PARAM			 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

CPacketCHAR_MODIFY_PARAM::~CPacketCHAR_MODIFY_PARAM()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_MODIFY_PARAM::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

void CPacketCHAR_MODIFY_PARAM::Make(
	DWORD dwCharID,		/* [in] キャラID */
	int nType,			/* [in] 種別 */
	int nParam)			/* [in] パラメータ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (nType)			+
			 sizeof (nParam);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_MODIFY_PARAM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &nType,		sizeof (nType),		pDataTmp);	/* 種別 */
	CopyMemoryRenew (pDataTmp, &nParam,		sizeof (nParam),	pDataTmp);	/* パラメータ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_MODIFY_PARAM::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

PBYTE CPacketCHAR_MODIFY_PARAM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_nType,		pDataTmp, sizeof (m_nType),		pDataTmp);	/* 種別 */
	CopyMemoryRenew (&m_nParam,		pDataTmp, sizeof (m_nParam),	pDataTmp);	/* パラメータ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
