/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_EFC_RENEWEFFECT.cpp							 */
/* 内容			:コマンド(管理者系:エフェクト情報の更新) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/06													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoEffect.h"
#include "Command.h"
#include "PacketADMIN_EFC_RENEWEFFECT.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_EFC_RENEWEFFECT::CPacketADMIN_EFC_RENEWEFFECT		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

CPacketADMIN_EFC_RENEWEFFECT::CPacketADMIN_EFC_RENEWEFFECT()
{
	m_dwEffectID	= 0;
	m_pInfo			= NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_EFC_RENEWEFFECT::~CPacketADMIN_EFC_RENEWEFFECT	 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

CPacketADMIN_EFC_RENEWEFFECT::~CPacketADMIN_EFC_RENEWEFFECT()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_EFC_RENEWEFFECT::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CPacketADMIN_EFC_RENEWEFFECT::Make(
	DWORD dwEffectID,		/* [in] エフェクトID */
	CInfoEffect *pSrc)		/* [in] エフェクト情報 */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pSrc->GetSendDataSize ();
	pInfoDataTmp	= pSrc->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwEffectID) +
			 dwSizeDataTmp;

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_EFC_RENEWEFFECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwEffectID,		sizeof (dwEffectID),	pDataTmp);	/* エフェクトID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	 		pDataTmp);	/* エフェクト情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_EFC_RENEWEFFECT::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

PBYTE CPacketADMIN_EFC_RENEWEFFECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwEffectID, pDataTmp, sizeof (m_dwEffectID), pDataTmp);	/* エフェクトID */

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoEffect;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
