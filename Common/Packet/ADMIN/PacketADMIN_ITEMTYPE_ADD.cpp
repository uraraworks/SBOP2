/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_ITEMTYPE_ADD.cpp								 */
/* 内容			:コマンド(管理者系:アイテム種別情報追加) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoItemType.h"
#include "Command.h"
#include "PacketADMIN_ITEMTYPE_ADD.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEMTYPE_ADD::CPacketADMIN_ITEMTYPE_ADD			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/10/01														 */
/* ========================================================================= */

CPacketADMIN_ITEMTYPE_ADD::CPacketADMIN_ITEMTYPE_ADD()
{
	m_pInfoItem = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEMTYPE_ADD::~CPacketADMIN_ITEMTYPE_ADD			 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/10/01														 */
/* ========================================================================= */

CPacketADMIN_ITEMTYPE_ADD::~CPacketADMIN_ITEMTYPE_ADD()
{
	SAFE_DELETE (m_pInfoItem);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEMTYPE_ADD::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/10/01														 */
/* ========================================================================= */

void CPacketADMIN_ITEMTYPE_ADD::Make(
	CInfoItemTypeBase *pInfo)		/* [in] アイテム種別情報 */
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_ADD;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* アイテム種別情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEMTYPE_ADD::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/10/01														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEMTYPE_ADD::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	CLibInfoItemType LibInfo;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);
	pDataTmp	= LibInfo.GetNewSendData (pDataTmp, m_pInfoItem);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
