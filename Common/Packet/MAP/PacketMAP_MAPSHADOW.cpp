/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_MAPSHADOW.cpp									 */
/* 内容			:コマンド(マップ系:マップ影情報通知) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapShadow.h"
#include "InfoMapShadow.h"
#include "Command.h"
#include "PacketMAP_MAPSHADOW.h"

/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPSHADOW::CPacketMAP_MAPSHADOW						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

CPacketMAP_MAPSHADOW::CPacketMAP_MAPSHADOW()
{
	m_pLibInfoMapShadow = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPSHADOW::~CPacketMAP_MAPSHADOW					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

CPacketMAP_MAPSHADOW::~CPacketMAP_MAPSHADOW()
{
	SAFE_DELETE (m_pLibInfoMapShadow);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPSHADOW::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

void CPacketMAP_MAPSHADOW::Make(
	CLibInfoMapShadow *pLibInfo)		/* [in] マップ影情報 */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* マップ影情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPSHADOW::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

void CPacketMAP_MAPSHADOW::Make(
	CInfoMapShadow *pInfo)		/* [in] マップ影情報 */
{
	PCInfoMapShadow pInfoTmp;
	CLibInfoMapShadow LibInfoMapShadow;

	LibInfoMapShadow.Create ();

	pInfoTmp = (PCInfoMapShadow)LibInfoMapShadow.GetNew ();
	pInfoTmp->Copy (pInfo);
	LibInfoMapShadow.Add (pInfoTmp);

	Make (&LibInfoMapShadow);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPSHADOW::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

PBYTE CPacketMAP_MAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfoMapShadow);
	m_pLibInfoMapShadow	= new CLibInfoMapShadow;
	m_pLibInfoMapShadow->Create ();
	pDataTmp			= m_pLibInfoMapShadow->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
