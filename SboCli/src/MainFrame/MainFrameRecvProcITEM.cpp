/// @file MainFrameRecvProcITEM.cpp
/// @brief クライアントメインフレーム(アイテム系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/05
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoItem.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "LayerMap.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "MainFrame.h"


void CMainFrame::RecvProcITEM(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ITEM_RES_ITEMINFO: RecvProcITEM_RES_ITEMINFO(pData); break; // アイテム情報応答
	case SBOCOMMANDID_SUB_ITEM_ITEMINFO: RecvProcITEM_ITEMINFO(pData); break; // アイテム情報通知
	case SBOCOMMANDID_SUB_ITEM_DELETEITEMINFO: RecvProcITEM_DELETEITEMINFO(pData); break; // アイテム情報削除
	case SBOCOMMANDID_SUB_ITEM_RES_ITEMTYPEINFO: RecvProcITEM_RES_ITEMTYPEINFO(pData); break; // アイテム種別情報応答
	case SBOCOMMANDID_SUB_ITEM_ITEMTYPEINFO: RecvProcITEM_ITEMTYPEINFO(pData); break; // アイテム種別情報通知
	case SBOCOMMANDID_SUB_ITEM_DELETEITEMTYPEINFO: RecvProcITEM_DELETEITEMTYPEINFO(pData); break; // アイテム種別情報削除
	case SBOCOMMANDID_SUB_ITEM_RES_ITEMWEAPONINFO: RecvProcITEM_RES_ITEMWEAPONINFO(pData); break; // 武器情報応答
	case SBOCOMMANDID_SUB_ITEM_ITEMWEAPONINFO: RecvProcITEM_ITEMWEAPONINFO(pData); break; // 武器情報通知
	}
}


void CMainFrame::RecvProcITEM_RES_ITEMINFO(PBYTE pData)
{
	PCInfoItem pInfoItem;
	CPacketITEM_RES_ITEMINFO Packet;

	Packet.Set(pData);

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(Packet.m_pInfo->m_dwItemID);
	if (pInfoItem == NULL) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetNew();
		pInfoItem->Copy(Packet.m_pInfo);
		m_pLibInfoItem->Add(pInfoItem);

	} else {
		pInfoItem->Copy(Packet.m_pInfo);
	}
	m_pLibInfoItem->Sort();
	RenewItemArea();
	m_pMgrWindow->Update();

	PostMessage(m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMINFO, pInfoItem->m_dwItemID);
}


void CMainFrame::RecvProcITEM_ITEMINFO(PBYTE pData)
{
	int i, nCount;
	PCInfoItem pInfoItem, pInfoItemTmp;
	CPacketITEM_ITEMINFO Packet;

	Packet.Set(pData);

	nCount = Packet.m_pLibInfo->GetCount();
	for (i = 0; i < nCount; i++) {
		pInfoItemTmp = (PCInfoItem)Packet.m_pLibInfo->GetPtr(i);
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(pInfoItemTmp->m_dwItemID);
		if (pInfoItem == NULL) {
			pInfoItem = (PCInfoItem)m_pLibInfoItem->GetNew();
			pInfoItem->Copy(pInfoItemTmp);
			m_pLibInfoItem->Add(pInfoItem);
		}
		pInfoItem->Copy(pInfoItemTmp);
	}
	m_pLibInfoItem->Sort();
	RenewItemArea();

	PostMessage(m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMINFO, 0);
}


void CMainFrame::RecvProcITEM_DELETEITEMINFO(PBYTE pData)
{
	CPacketITEM_DELETEITEMINFO Packet;

	Packet.Set(pData);

	m_pLibInfoItem->Delete(Packet.m_dwItemID);

	PostMessage(m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMINFO, Packet.m_dwItemID);
}


void CMainFrame::RecvProcITEM_RES_ITEMTYPEINFO(PBYTE pData)
{
	PCInfoItemTypeBase pInfoItem;
	CPacketITEM_RES_ITEMTYPEINFO Packet;

	Packet.Set(pData);

	pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr(Packet.m_pInfo->m_dwTypeID);
	if (pInfoItem == NULL) {
		pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetNew(Packet.m_pInfo->m_dwItemTypeID);
		pInfoItem->Copy(Packet.m_pInfo);
		m_pLibInfoItemType->Add(pInfoItem);

	} else {
		pInfoItem->Copy(Packet.m_pInfo);
	}

	m_pLibInfoChar->RenewMotionInfo((DWORD)0);
	PostMessage(m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMTYPEINFO, pInfoItem->m_dwTypeID);
}


void CMainFrame::RecvProcITEM_ITEMTYPEINFO(PBYTE pData)
{
	int i, nCount;
	PCInfoItemTypeBase pInfoItem, pInfoItemTmp;
	CPacketITEM_ITEMTYPEINFO Packet;

	Packet.Set(pData);

	nCount = Packet.m_pLibInfo->GetCount();
	for (i = 0; i < nCount; i++) {
		pInfoItemTmp = (PCInfoItemTypeBase)Packet.m_pLibInfo->GetPtr(i);
		pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr(pInfoItemTmp->m_dwTypeID);
		if (pInfoItem == NULL) {
			pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetNew(pInfoItemTmp->m_dwItemTypeID);
			pInfoItem->Copy(pInfoItemTmp);
			m_pLibInfoItemType->Add(pInfoItem);
		}
		pInfoItem->Copy(pInfoItemTmp);
	}

	m_pLibInfoChar->RenewMotionInfo((DWORD)0);
	PostMessage(m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMTYPEINFO, 0);
}


void CMainFrame::RecvProcITEM_DELETEITEMTYPEINFO(PBYTE pData)
{
	CPacketITEM_DELETEITEMTYPEINFO Packet;

	Packet.Set(pData);

	m_pLibInfoItemType->Delete(Packet.m_dwTypeID);

	m_pLibInfoChar->RenewMotionInfo((DWORD)0);
	PostMessage(m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMTYPEINFO, Packet.m_dwTypeID);
}


void CMainFrame::RecvProcITEM_RES_ITEMWEAPONINFO(PBYTE pData)
{
	PCInfoItemWeapon pInfoItemWeapon;
	CPacketITEM_RES_ITEMWEAPONINFO Packet;

	Packet.Set(pData);

	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr(Packet.m_pInfo->m_dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetNew();
		pInfoItemWeapon->Copy(Packet.m_pInfo);
		m_pLibInfoItemWeapon->Add(pInfoItemWeapon);

	} else {
		pInfoItemWeapon->Copy(Packet.m_pInfo);
	}

	PostMessage(m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMWEAPONINFO, pInfoItemWeapon->m_dwWeaponInfoID);
}


void CMainFrame::RecvProcITEM_ITEMWEAPONINFO(PBYTE pData)
{
	int i, nCount;
	PCInfoItemWeapon pInfoItemWeapon, pInfoItemWeaponTmp;
	CPacketITEM_ITEMWEAPONINFO Packet;

	Packet.Set(pData);

	nCount = Packet.m_pLibInfo->GetCount();
	for (i = 0; i < nCount; i++) {
		pInfoItemWeaponTmp = (PCInfoItemWeapon)Packet.m_pLibInfo->GetPtr(i);
		pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr(pInfoItemWeaponTmp->m_dwWeaponInfoID);
		if (pInfoItemWeapon == NULL) {
			pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetNew();
			pInfoItemWeapon->Copy(pInfoItemWeaponTmp);
			m_pLibInfoItemWeapon->Add(pInfoItemWeapon);
		}
		pInfoItemWeapon->Copy(pInfoItemWeaponTmp);
	}

	PostMessage(m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMWEAPONINFO, 0);
}
