/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_CHARINFO2.cpp								 */
/* 内容			:コマンド(キャラ系:キャラID通知) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/02/13													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_CHARINFO2.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHARINFO2::CPacketCHAR_REQ_CHARINFO2			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/02/13														 */
/* ========================================================================= */

CPacketCHAR_REQ_CHARINFO2::CPacketCHAR_REQ_CHARINFO2()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHARINFO2::~CPacketCHAR_REQ_CHARINFO2			 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/02/13														 */
/* ========================================================================= */

CPacketCHAR_REQ_CHARINFO2::~CPacketCHAR_REQ_CHARINFO2()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHARINFO2::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/02/13														 */
/* ========================================================================= */

void CPacketCHAR_REQ_CHARINFO2::Make(
	PARRAYDWORD paSrc)		/* [in] キャラID */
{
	PBYTE pData, pDataTmp;
	DWORD i, dwSize, dwCount, dwTmp;
	PPACKETBASE pPacketBase;

	dwCount = paSrc->GetSize ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (DWORD)			+
			 (sizeof (DWORD) * dwCount);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO2;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCount, sizeof (dwCount), pDataTmp);	/* 個数 */
	for (i = 0; i < dwCount; i ++) {
		dwTmp = paSrc->GetAt (i);
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);	/* キャラID */
	}

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHARINFO2::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/02/13														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_CHARINFO2::Set(PBYTE pPacket)
{
	DWORD i, dwCount, dwTmp;
	PBYTE pRet, pDataTmp;

	m_adwCharID.RemoveAll ();

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&dwCount, pDataTmp, sizeof (dwCount), pDataTmp);	/* 個数 */
	for (i = 0; i < dwCount; i ++) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);	/* キャラID */
		m_adwCharID.Add (dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
