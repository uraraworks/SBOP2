/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_MAPOBJECTDATA.cpp								 */
/* 内容			:コマンド(マップ系:マップオブジェクト配置データ通知) 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapObjectData.h"
#include "Command.h"
#include "PacketMAP_MAPOBJECTDATA.h"

/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPOBJECTDATA::CPacketMAP_MAPOBJECTDATA				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

CPacketMAP_MAPOBJECTDATA::CPacketMAP_MAPOBJECTDATA()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPOBJECTDATA::~CPacketMAP_MAPOBJECTDATA			 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

CPacketMAP_MAPOBJECTDATA::~CPacketMAP_MAPOBJECTDATA()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPOBJECTDATA::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CPacketMAP_MAPOBJECTDATA::Make(
	DWORD dwMapID,						/* [in] マップID */
	CLibInfoMapObjectData *pLibInfo)	/* [in] マップオブジェクト配置データ */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pLibInfo->GetSendDataSize ();
	pInfoDataTmp	= pLibInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPOBJECTDATA;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID, sizeof (dwMapID), pDataTmp);	/* マップID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* マップオブジェクト配置データ */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPOBJECTDATA::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CPacketMAP_MAPOBJECTDATA::Make(
	DWORD dwMapID,					/* [in] マップID */
	CInfoMapObjectData *pInfo)		/* [in] マップオブジェクト配置データ */
{
	PCInfoMapObjectData pInfoTmp;
	CLibInfoMapObjectData LibInfoMapObjectData;

	LibInfoMapObjectData.Create ();

	pInfoTmp = (PCInfoMapObjectData)LibInfoMapObjectData.GetNew ();
	pInfoTmp->Copy (pInfo);
	LibInfoMapObjectData.Add (pInfoTmp);

	Make (dwMapID, &LibInfoMapObjectData);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_MAPOBJECTDATA::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

PBYTE CPacketMAP_MAPOBJECTDATA::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	/* マップID */

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoMapObjectData;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
