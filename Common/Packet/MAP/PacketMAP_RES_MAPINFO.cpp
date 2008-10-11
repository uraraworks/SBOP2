/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_RES_MAPINFO.cpp									 */
/* 内容			:コマンド(マップ系:マップ情報応答) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapBase.h"
#include "Command.h"
#include "PacketMAP_RES_MAPINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketMAP_RES_MAPINFO::CPacketMAP_RES_MAPINFO					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/01/04														 */
/* ========================================================================= */

CPacketMAP_RES_MAPINFO::CPacketMAP_RES_MAPINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_RES_MAPINFO::~CPacketMAP_RES_MAPINFO				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/01/04														 */
/* ========================================================================= */

CPacketMAP_RES_MAPINFO::~CPacketMAP_RES_MAPINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_RES_MAPINFO::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/01/04														 */
/* ========================================================================= */

void CPacketMAP_RES_MAPINFO::Make(
	CInfoMapBase *pInfo)		/* [in] マップ情報 */
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_RES_MAPINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* マップ情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_RES_MAPINFO::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/01/04														 */
/* ========================================================================= */

PBYTE CPacketMAP_RES_MAPINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoMapBase;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
