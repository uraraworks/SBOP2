/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMSGCMD_CHGFACE.cpp									 */
/* 内容			:コマンド(メッセージコマンド系:表情変更) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/02													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMSGCMD_CHGFACE.h"

/* ========================================================================= */
/* 関数名	:CPacketMSGCMD_CHGFACE::CPacketMSGCMD_CHGFACE					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/02														 */
/* ========================================================================= */

CPacketMSGCMD_CHGFACE::CPacketMSGCMD_CHGFACE()
{
	m_dwCharID	= 0;
	m_nType		= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketMSGCMD_CHGFACE::~CPacketMSGCMD_CHGFACE					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/05/02														 */
/* ========================================================================= */

CPacketMSGCMD_CHGFACE::~CPacketMSGCMD_CHGFACE()
{
}


/* ========================================================================= */
/* 関数名	:CPacketMSGCMD_CHGFACE::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/05/02														 */
/* ========================================================================= */

void CPacketMSGCMD_CHGFACE::Make(
	DWORD dwCharID,		/* [in] キャラID */
	int nType)			/* [in] 種別 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (nType);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MSGCMD;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MSGCMD_CHGFACE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &nType,		sizeof (nType),		pDataTmp);	/* 種別 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketMSGCMD_CHGFACE::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/05/02														 */
/* ========================================================================= */

PBYTE CPacketMSGCMD_CHGFACE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_nType,		pDataTmp, sizeof (m_nType),		pDataTmp);	/* 種別 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
