/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketACCOUNT_RES_ACCOUNTINFO.cpp							 */
/* 内容			:コマンド(アカウント系:アカウント情報応答) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoAccount.h"
#include "Command.h"
#include "PacketACCOUNT_RES_ACCOUNTINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_RES_ACCOUNTINFO::CPacketACCOUNT_RES_ACCOUNTINFO	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/07														 */
/* ========================================================================= */

CPacketACCOUNT_RES_ACCOUNTINFO::CPacketACCOUNT_RES_ACCOUNTINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_RES_ACCOUNTINFO::~CPacketACCOUNT_RES_ACCOUNTINFO */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/07														 */
/* ========================================================================= */

CPacketACCOUNT_RES_ACCOUNTINFO::~CPacketACCOUNT_RES_ACCOUNTINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_RES_ACCOUNTINFO::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/11/07														 */
/* ========================================================================= */

void CPacketACCOUNT_RES_ACCOUNTINFO::Make(
	CInfoAccount *pInfo)		/* [in] アカウント情報 */
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_RES_ACCOUNTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* アカウント情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_RES_ACCOUNTINFO::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/11/07														 */
/* ========================================================================= */

PBYTE CPacketACCOUNT_RES_ACCOUNTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoAccount;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
