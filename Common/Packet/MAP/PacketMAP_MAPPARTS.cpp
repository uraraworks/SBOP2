/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_MAPPARTS.cpp										 */
/* 内容			:コマンド(マップ系:マップパーツ情報通知) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/30													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapParts.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketMAP_MAPPARTS.h"

/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPPARTS::CPacketMAP_MAPPARTS						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

CPacketMAP_MAPPARTS::CPacketMAP_MAPPARTS()
{
	m_pLibInfoMapParts = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPPARTS::~CPacketMAP_MAPPARTS						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

CPacketMAP_MAPPARTS::~CPacketMAP_MAPPARTS()
{
	SAFE_DELETE (m_pLibInfoMapParts);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPPARTS::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CPacketMAP_MAPPARTS::Make(
	CLibInfoMapParts *pLibInfo)		/* [in] マップパーツ情報 */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pLibInfo->GetSendDataSize ();
	pInfoDataTmp	= pLibInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* マップパーツ情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPPARTS::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CPacketMAP_MAPPARTS::Make(
	CInfoMapParts *pInfo)		/* [in] マップパーツ情報 */
{
	PCInfoMapParts pInfoTmp;
	CLibInfoMapParts LibInfoMapParts;

	LibInfoMapParts.Create ();

	pInfoTmp = (PCInfoMapParts)LibInfoMapParts.GetNew ();
	pInfoTmp->Copy (pInfo);
	LibInfoMapParts.Add (pInfoTmp);

	Make (&LibInfoMapParts);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPPARTS::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

PBYTE CPacketMAP_MAPPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfoMapParts);
	m_pLibInfoMapParts	= new CLibInfoMapParts;
	m_pLibInfoMapParts->Create ();
	pDataTmp			= m_pLibInfoMapParts->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
