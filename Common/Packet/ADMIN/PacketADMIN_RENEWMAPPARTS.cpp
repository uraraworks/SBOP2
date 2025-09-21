/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_RENEWMAPPARTS.cpp								 */
/* 内容			:コマンド(管理者系:マップパーツ更新) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/29													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketADMIN_RENEWMAPPARTS.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEWMAPPARTS::CPacketADMIN_RENEWMAPPARTS			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

CPacketADMIN_RENEWMAPPARTS::CPacketADMIN_RENEWMAPPARTS()
{
	m_pInfoMapParts = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEWMAPPARTS::~CPacketADMIN_RENEWMAPPARTS		 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

CPacketADMIN_RENEWMAPPARTS::~CPacketADMIN_RENEWMAPPARTS()
{
	SAFE_DELETE (m_pInfoMapParts);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEWMAPPARTS::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

void CPacketADMIN_RENEWMAPPARTS::Make(
	CInfoMapParts *pInfo)		/* [in] マップパーツ情報 */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEWMAPPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* マップパーツ情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_RENEWMAPPARTS::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

PBYTE CPacketADMIN_RENEWMAPPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfoMapParts);
	m_pInfoMapParts	= new CInfoMapParts;
	pDataTmp		= m_pInfoMapParts->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
