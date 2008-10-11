/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_RES_EFFECTINFO.cpp							 */
/* 内容			:コマンド(エフェクト系:エフェクト情報応答) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoEffect.h"
#include "Command.h"
#include "PacketEFFECT_RES_EFFECTINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketEFFECT_RES_EFFECTINFO::CPacketEFFECT_RES_EFFECTINFO		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CPacketEFFECT_RES_EFFECTINFO::CPacketEFFECT_RES_EFFECTINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_RES_EFFECTINFO::~CPacketEFFECT_RES_EFFECTINFO	 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CPacketEFFECT_RES_EFFECTINFO::~CPacketEFFECT_RES_EFFECTINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_RES_EFFECTINFO::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CPacketEFFECT_RES_EFFECTINFO::Make(
	CInfoEffect *pInfo)		/* [in] エフェクト情報 */
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_RES_EFFECTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* エフェクト情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_RES_EFFECTINFO::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_RES_EFFECTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoEffect;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
