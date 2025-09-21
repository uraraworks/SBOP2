/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_RENEWMAPOBJECT.cpp							 */
/* 内容			:コマンド(管理者系:マップオブジェクト更新) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapObject.h"
#include "Command.h"
#include "PacketADMIN_MAP_RENEWMAPOBJECT.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_RENEWMAPOBJECT::CPacketADMIN_MAP_RENEWMAPOBJECT */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

CPacketADMIN_MAP_RENEWMAPOBJECT::CPacketADMIN_MAP_RENEWMAPOBJECT()
{
	m_pInfoMapObject = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_RENEWMAPOBJECT::~CPacketADMIN_MAP_RENEWMAPOBJECT */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

CPacketADMIN_MAP_RENEWMAPOBJECT::~CPacketADMIN_MAP_RENEWMAPOBJECT()
{
	SAFE_DELETE (m_pInfoMapObject);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_RENEWMAPOBJECT::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CPacketADMIN_MAP_RENEWMAPOBJECT::Make(
	CInfoMapObject *pInfo)		/* [in] マップオブジェクト情報 */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_RENEWMAPOBJECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* マップオブジェクト情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_RENEWMAPOBJECT::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_RENEWMAPOBJECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfoMapObject);
	m_pInfoMapObject	= new CInfoMapObject;
	pDataTmp			= m_pInfoMapObject->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
