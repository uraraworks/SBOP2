/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_RENEW_CLIENTVERSION.cpp						 */
/* 内容			:コマンド(管理者系:クライアントバージョン更新) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_RENEW_CLIENTVERSION.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEW_CLIENTVERSION::CPacketADMIN_RENEW_CLIENTVERSION */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_RENEW_CLIENTVERSION::CPacketADMIN_RENEW_CLIENTVERSION()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEW_CLIENTVERSION::~CPacketADMIN_RENEW_CLIENTVERSION */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_RENEW_CLIENTVERSION::~CPacketADMIN_RENEW_CLIENTVERSION()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEW_CLIENTVERSION::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CPacketADMIN_RENEW_CLIENTVERSION::Make(
	LPCSTR pszClientVersion)		/* [in] クライアントバージョン */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			(strlen (pszClientVersion) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEW_CLIENTVERSION;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, (PBYTE)pszClientVersion, strlen (pszClientVersion) + 1, pDataTmp);	/* クライアントバージョン */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEW_CLIENTVERSION::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

PBYTE CPacketADMIN_RENEW_CLIENTVERSION::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	StoreRenew (m_strClientVersion, (LPCSTR)pDataTmp, pDataTmp);	/* クライアントバージョン */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
