/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_RENEWMAPSHADOW.cpp								 */
/* 内容			:コマンド(管理者系:マップ影更新) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/06													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapShadow.h"
#include "Command.h"
#include "PacketADMIN_RENEWMAPSHADOW.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEWMAPSHADOW::CPacketADMIN_RENEWMAPSHADOW		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

CPacketADMIN_RENEWMAPSHADOW::CPacketADMIN_RENEWMAPSHADOW()
{
	m_pInfoMapShadow = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEWMAPSHADOW::~CPacketADMIN_RENEWMAPSHADOW		 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

CPacketADMIN_RENEWMAPSHADOW::~CPacketADMIN_RENEWMAPSHADOW()
{
	SAFE_DELETE (m_pInfoMapShadow);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEWMAPSHADOW::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CPacketADMIN_RENEWMAPSHADOW::Make(
	CInfoMapShadow *pInfo)		/* [in] マップ影情報 */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEWMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* マップ影情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEWMAPSHADOW::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

PBYTE CPacketADMIN_RENEWMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfoMapShadow);
	m_pInfoMapShadow	= new CInfoMapShadow;
	pDataTmp			= m_pInfoMapShadow->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
