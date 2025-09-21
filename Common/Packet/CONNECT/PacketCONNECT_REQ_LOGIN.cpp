/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketCONNECT_REQ_LOGIN.cpp								 */
/* 内容			:コマンド(接続系:ログイン要求) 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_REQ_LOGIN.h"

/* ========================================================================= */
/* 関数名	:CPacketCONNECT_REQ_LOGIN::CPacketCONNECT_REQ_LOGIN				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CPacketCONNECT_REQ_LOGIN::CPacketCONNECT_REQ_LOGIN()
{
	ZeroMemory (m_byMacAddr, sizeof (m_byMacAddr));
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_REQ_LOGIN::~CPacketCONNECT_REQ_LOGIN			 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CPacketCONNECT_REQ_LOGIN::~CPacketCONNECT_REQ_LOGIN()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_REQ_LOGIN::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CPacketCONNECT_REQ_LOGIN::Make(
	LPCSTR pszAccount,		/* [in] アカウント */
	LPCSTR pszPassword,		/* [in] パスワード */
	PBYTE pbyMacAddr)		/* [in] MACアドレス */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)		+
			 (strlen (pszAccount) + 1)	+
			 (strlen (pszPassword) + 1)	+
			 6;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_REQ_LOGIN;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strcpyRenew ((LPSTR)pDataTmp, pszAccount, pDataTmp);		/* アカウント */
	strcpyRenew ((LPSTR)pDataTmp, pszPassword, pDataTmp);		/* パスワード */
	CopyMemoryRenew (pDataTmp, pbyMacAddr, 6, pDataTmp);		/* MACアドレス*/

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_REQ_LOGIN::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

PBYTE CPacketCONNECT_REQ_LOGIN::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	StoreRenew (m_strAccount,	(LPCSTR)pDataTmp, pDataTmp);		/* アカウント */
	StoreRenew (m_strPassword,	(LPCSTR)pDataTmp, pDataTmp);		/* パスワード */
	CopyMemoryRenew (m_byMacAddr, pDataTmp, 6, pDataTmp);			/* MACアドレス*/

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
