/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcITEM.cpp									 */
/* 内容			:サーバーメインフレーム(アイテム系受信処理) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoItem.h"
#include "LibInfoCharSvr.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcITEM										 */
/* 内容		:受信処理(アイテム系)											 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ITEM_REQ_ITEMINFO:		RecvProcITEM_REQ_ITEMINFO		(pData, dwSessionID);	break;	/* アイテム情報要求 */
	case SBOCOMMANDID_SUB_ITEM_RENEWITEMINFO:		RecvProcITEM_RENEWITEMINFO		(pData, dwSessionID);	break;	/* アイテム情報更新 */
	case SBOCOMMANDID_SUB_ITEM_REQ_ITEMTYPEINFO:	RecvProcITEM_REQ_ITEMTYPEINFO	(pData, dwSessionID);	break;	/* アイテム種別情報要求 */
	case SBOCOMMANDID_SUB_ITEM_RENEWITEMTYPEINFO:	RecvProcITEM_RENEWITEMTYPEINFO	(pData, dwSessionID);	break;	/* アイテム種別情報更新 */
	case SBOCOMMANDID_SUB_ITEM_REQ_ITEMWEAPONINFO:	RecvProcITEM_REQ_ITEMWEAPONINFO	(pData, dwSessionID);	break;	/* 武器情報要求 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcITEM_REQ_ITEMINFO							 */
/* 内容		:受信処理(アイテム情報要求)										 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_REQ_ITEMINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketITEM_REQ_ITEMINFO Packet;
	CPacketITEM_ITEMINFO PacketITEM_ITEMINFO;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;
	PCPacketBase pPacket;
	PCInfoItem pInfoItem;

	pPacket = NULL;
	Packet.Set (pData);

	/* 全て？ */
	if (Packet.m_dwItemID == 0) {
		PacketITEM_ITEMINFO.Make (m_pLibInfoItem);
		pPacket = &PacketITEM_ITEMINFO;

	} else {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (Packet.m_dwItemID);
		if (pInfoItem == NULL) {
			return;
		}

		PacketITEM_RES_ITEMINFO.Make (pInfoItem);
		pPacket = &PacketITEM_RES_ITEMINFO;
	}

	m_pSock->SendTo (dwSessionID, pPacket);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcITEM_RENEWITEMINFO							 */
/* 内容		:受信処理(アイテム情報更新)										 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_RENEWITEMINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketITEM_RENEWITEMINFO Packet;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;
	PCInfoItem pInfoItem;

	Packet.Set (pData);

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (Packet.m_pInfo->m_dwItemID);
	if (pInfoItem == NULL) {
		return;
	}
	pInfoItem->Copy (Packet.m_pInfo);
	PacketITEM_RES_ITEMINFO.Make (pInfoItem);

	if (pInfoItem->m_dwMapID) {
		SendToMapChar (pInfoItem->m_dwMapID, &PacketITEM_RES_ITEMINFO);
	}
	SendToAdminChar (&PacketITEM_RES_ITEMINFO);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcITEM_REQ_ITEMTYPEINFO						 */
/* 内容		:受信処理(アイテム種別情報要求)									 */
/* 日付		:2007/10/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_REQ_ITEMTYPEINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketITEM_REQ_ITEMTYPEINFO Packet;
	CPacketITEM_ITEMTYPEINFO PacketITEM_ITEMTYPEINFO;
	CPacketITEM_RES_ITEMTYPEINFO PacketITEM_RES_ITEMTYPEINFO;
	PCPacketBase pPacket;
	PCInfoItemTypeBase pInfoItem;

	pPacket = NULL;
	Packet.Set (pData);

	/* 全て？ */
	if (Packet.m_dwTypeID == 0) {
		PacketITEM_ITEMTYPEINFO.Make (m_pLibInfoItemType);
		pPacket = &PacketITEM_ITEMTYPEINFO;

	} else {
		pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (Packet.m_dwTypeID);
		if (pInfoItem == NULL) {
			return;
		}

		PacketITEM_RES_ITEMTYPEINFO.Make (pInfoItem);
		pPacket = &PacketITEM_RES_ITEMTYPEINFO;
	}
	m_pSock->SendTo (dwSessionID, pPacket);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcITEM_RENEWITEMTYPEINFO						 */
/* 内容		:受信処理(アイテム種別情報更新)									 */
/* 日付		:2007/10/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_RENEWITEMTYPEINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketITEM_RENEWITEMTYPEINFO Packet;
	CPacketITEM_RES_ITEMTYPEINFO PacketITEM_RES_ITEMTYPEINFO;
	PCInfoItemTypeBase pInfoItemType;

	Packet.Set (pData);

	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (Packet.m_pInfo->m_dwTypeID);
	if (pInfoItemType == NULL) {
		return;
	}
	pInfoItemType->Copy (Packet.m_pInfo);
	PacketITEM_RES_ITEMTYPEINFO.Make (pInfoItemType);
	m_pSock->SendTo (0, &PacketITEM_RES_ITEMTYPEINFO);

//	m_pLibInfoChar->RenewGrpID (0);
	m_pLibInfoChar->RenewItemGrp (pInfoItemType->m_dwTypeID);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcITEM_REQ_ITEMWEAPONINFO					 */
/* 内容		:受信処理(武器情報要求)											 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_REQ_ITEMWEAPONINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketITEM_REQ_ITEMWEAPONINFO Packet;
	CPacketITEM_ITEMWEAPONINFO PacketITEM_ITEMWEAPONINFO;
	CPacketITEM_RES_ITEMWEAPONINFO PacketITEM_RES_ITEMWEAPONINFO;
	PCPacketBase pPacket;
	PCInfoItemWeapon pInfoItemWeapon;

	pPacket = NULL;
	Packet.Set (pData);

	/* 全て？ */
	if (Packet.m_dwWeaponInfoID == 0) {
		PacketITEM_ITEMWEAPONINFO.Make (m_pLibInfoItemWeapon);
		pPacket = &PacketITEM_ITEMWEAPONINFO;

	} else {
		pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (Packet.m_dwWeaponInfoID);
		if (pInfoItemWeapon == NULL) {
			return;
		}

		PacketITEM_RES_ITEMWEAPONINFO.Make (pInfoItemWeapon);
		pPacket = &PacketITEM_RES_ITEMWEAPONINFO;
	}

	m_pSock->SendTo (dwSessionID, pPacket);
}

/* Copyright(C)URARA-works 2007 */
