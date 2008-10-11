/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcITEM.cpp									 */
/* ���e			:�N���C�A���g���C���t���[��(�A�C�e���n��M����) �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

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
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcITEM										 */
/* ���e		:��M����(�A�C�e���n)											 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ITEM_RES_ITEMINFO:		RecvProcITEM_RES_ITEMINFO		(pData);	break;	/* �A�C�e����񉞓� */
	case SBOCOMMANDID_SUB_ITEM_ITEMINFO:			RecvProcITEM_ITEMINFO			(pData);	break;	/* �A�C�e�����ʒm */
	case SBOCOMMANDID_SUB_ITEM_DELETEITEMINFO:		RecvProcITEM_DELETEITEMINFO		(pData);	break;	/* �A�C�e�����폜 */
	case SBOCOMMANDID_SUB_ITEM_RES_ITEMTYPEINFO:	RecvProcITEM_RES_ITEMTYPEINFO	(pData);	break;	/* �A�C�e����ʏ�񉞓� */
	case SBOCOMMANDID_SUB_ITEM_ITEMTYPEINFO:		RecvProcITEM_ITEMTYPEINFO		(pData);	break;	/* �A�C�e����ʏ��ʒm */
	case SBOCOMMANDID_SUB_ITEM_DELETEITEMTYPEINFO:	RecvProcITEM_DELETEITEMTYPEINFO	(pData);	break;	/* �A�C�e����ʏ��폜 */
	case SBOCOMMANDID_SUB_ITEM_RES_ITEMWEAPONINFO:	RecvProcITEM_RES_ITEMWEAPONINFO	(pData);	break;	/* �����񉞓� */
	case SBOCOMMANDID_SUB_ITEM_ITEMWEAPONINFO:		RecvProcITEM_ITEMWEAPONINFO		(pData);	break;	/* ������ʒm */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcITEM_RES_ITEMINFO							 */
/* ���e		:��M����(�A�C�e����񉞓�)										 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_RES_ITEMINFO(PBYTE pData)
{
	PCInfoItem pInfoItem;
	CPacketITEM_RES_ITEMINFO Packet;

	Packet.Set (pData);

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (Packet.m_pInfo->m_dwItemID);
	if (pInfoItem == NULL) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetNew ();
		pInfoItem->Copy (Packet.m_pInfo);
		m_pLibInfoItem->Add (pInfoItem);

	} else {
		pInfoItem->Copy (Packet.m_pInfo);
	}
	m_pLibInfoItem->Sort ();
	RenewItemArea ();

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMINFO, pInfoItem->m_dwItemID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcITEM_ITEMINFO								 */
/* ���e		:��M����(�A�C�e�����ʒm)										 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_ITEMINFO(PBYTE pData)
{
	int i, nCount;
	PCInfoItem pInfoItem, pInfoItemTmp;
	CPacketITEM_ITEMINFO Packet;

	Packet.Set (pData);

	nCount = Packet.m_pLibInfo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItemTmp = (PCInfoItem)Packet.m_pLibInfo->GetPtr (i);
		pInfoItem	 = (PCInfoItem)m_pLibInfoItem->GetPtr (pInfoItemTmp->m_dwItemID);
		if (pInfoItem == NULL) {
			pInfoItem = (PCInfoItem)m_pLibInfoItem->GetNew ();
			pInfoItem->Copy (pInfoItemTmp);
			m_pLibInfoItem->Add (pInfoItem);
		}
		pInfoItem->Copy (pInfoItemTmp);
	}
	m_pLibInfoItem->Sort ();
	RenewItemArea ();

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcITEM_DELETEITEMINFO						 */
/* ���e		:��M����(�A�C�e�����폜)										 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_DELETEITEMINFO(PBYTE pData)
{
	CPacketITEM_DELETEITEMINFO Packet;

	Packet.Set (pData);

	m_pLibInfoItem->Delete (Packet.m_dwItemID);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMINFO, Packet.m_dwItemID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcITEM_RES_ITEMINFO							 */
/* ���e		:��M����(�A�C�e����ʏ�񉞓�)									 */
/* ���t		:2007/10/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_RES_ITEMTYPEINFO(PBYTE pData)
{
	PCInfoItemTypeBase pInfoItem;
	CPacketITEM_RES_ITEMTYPEINFO Packet;

	Packet.Set (pData);

	pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (Packet.m_pInfo->m_dwTypeID);
	if (pInfoItem == NULL) {
		pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetNew (Packet.m_pInfo->m_dwItemTypeID);
		pInfoItem->Copy (Packet.m_pInfo);
		m_pLibInfoItemType->Add (pInfoItem);

	} else {
		pInfoItem->Copy (Packet.m_pInfo);
	}

	m_pLibInfoChar->RenewMotionInfo ((DWORD)0);
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMTYPEINFO, pInfoItem->m_dwTypeID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcITEM_ITEMTYPEINFO							 */
/* ���e		:��M����(�A�C�e����ʏ��ʒm)									 */
/* ���t		:2007/10/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_ITEMTYPEINFO(PBYTE pData)
{
	int i, nCount;
	PCInfoItemTypeBase pInfoItem, pInfoItemTmp;
	CPacketITEM_ITEMTYPEINFO Packet;

	Packet.Set (pData);

	nCount = Packet.m_pLibInfo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItemTmp = (PCInfoItemTypeBase)Packet.m_pLibInfo->GetPtr (i);
		pInfoItem	 = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (pInfoItemTmp->m_dwTypeID);
		if (pInfoItem == NULL) {
			pInfoItem = (PCInfoItemTypeBase)m_pLibInfoItemType->GetNew (pInfoItemTmp->m_dwItemTypeID);
			pInfoItem->Copy (pInfoItemTmp);
			m_pLibInfoItemType->Add (pInfoItem);
		}
		pInfoItem->Copy (pInfoItemTmp);
	}

	m_pLibInfoChar->RenewMotionInfo ((DWORD)0);
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMTYPEINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcITEM_DELETEITEMTYPEINFO					 */
/* ���e		:��M����(�A�C�e����ʏ��폜)									 */
/* ���t		:2007/10/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_DELETEITEMTYPEINFO(PBYTE pData)
{
	CPacketITEM_DELETEITEMTYPEINFO Packet;

	Packet.Set (pData);

	m_pLibInfoItemType->Delete (Packet.m_dwTypeID);

	m_pLibInfoChar->RenewMotionInfo ((DWORD)0);
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMTYPEINFO, Packet.m_dwTypeID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcITEM_RES_ITEMWEAPONINFO					 */
/* ���e		:��M����(�����񉞓�)											 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_RES_ITEMWEAPONINFO(PBYTE pData)
{
	PCInfoItemWeapon pInfoItemWeapon;
	CPacketITEM_RES_ITEMWEAPONINFO Packet;

	Packet.Set (pData);

	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (Packet.m_pInfo->m_dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetNew ();
		pInfoItemWeapon->Copy (Packet.m_pInfo);
		m_pLibInfoItemWeapon->Add (pInfoItemWeapon);

	} else {
		pInfoItemWeapon->Copy (Packet.m_pInfo);
	}

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMWEAPONINFO, pInfoItemWeapon->m_dwWeaponInfoID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcITEM_ITEMWEAPONINFO						 */
/* ���e		:��M����(������ʒm)											 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcITEM_ITEMWEAPONINFO(PBYTE pData)
{
	int i, nCount;
	PCInfoItemWeapon pInfoItemWeapon, pInfoItemWeaponTmp;
	CPacketITEM_ITEMWEAPONINFO Packet;

	Packet.Set (pData);

	nCount = Packet.m_pLibInfo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItemWeaponTmp = (PCInfoItemWeapon)Packet.m_pLibInfo->GetPtr (i);
		pInfoItemWeapon	 = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (pInfoItemWeaponTmp->m_dwWeaponInfoID);
		if (pInfoItemWeapon == NULL) {
			pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetNew ();
			pInfoItemWeapon->Copy (pInfoItemWeaponTmp);
			m_pLibInfoItemWeapon->Add (pInfoItemWeapon);
		}
		pInfoItemWeapon->Copy (pInfoItemWeaponTmp);
	}

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWITEMWEAPONINFO, 0);
}

/* Copyright(C)URARA-works 2007 */
