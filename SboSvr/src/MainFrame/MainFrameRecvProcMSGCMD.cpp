/* Copyright(C)URARA-works 2007 */
/* ================================================================================= */
/* �t�@�C����	:MainFrameRecvProcMSGCMD.cpp										 */
/* ���e			:�T�[�o�[���C���t���[��(���b�Z�[�W�R�}���h�n��M����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)									 */
/* �쐬�J�n��	:2007/05/02															 */
/* ================================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "InfoCharSvr.h"
#include "LibInfoMapBase.h"
#include "LibInfoItem.h"
#include "LibInfoCharSvr.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD										 */
/* ���e		:��M����(���b�Z�[�W�R�}���h�n)									 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_MSGCMD_CHGFACE:		RecvProcMSGCMD_CHGFACE		(pData, dwSessionID);	break;	/* �\��ύX */
//	case SBOCOMMANDID_SUB_MSGCMD_CHGHAIR:		RecvProcMSGCMD_CHGHAIR		(pData, dwSessionID);	break;	/* ���ύX */
//	case SBOCOMMANDID_SUB_MSGCMD_CHGCLOTH:		RecvProcMSGCMD_CHGCLOTH		(pData, dwSessionID);	break;	/* �����ύX */
//	case SBOCOMMANDID_SUB_MSGCMD_CHGACCE:		RecvProcMSGCMD_CHGACCE		(pData, dwSessionID);	break;	/* �A�N�Z�T���ύX */
	case SBOCOMMANDID_SUB_MSGCMD_CHGCOLOR:		RecvProcMSGCMD_CHGCOLOR		(pData, dwSessionID);	break;	/* �F�ύX */
//	case SBOCOMMANDID_SUB_MSGCMD_CHGARMS:		RecvProcMSGCMD_CHGARMS		(pData, dwSessionID);	break;	/* �������ύX */
//	case SBOCOMMANDID_SUB_MSGCMD_CHGSHIELD:		RecvProcMSGCMD_CHGSHIELD	(pData, dwSessionID);	break;	/* ���ύX */
	case SBOCOMMANDID_SUB_MSGCMD_MAKEITEM:		RecvProcMSGCMD_MAKEITEM		(pData, dwSessionID);	break;	/* �A�C�e���쐬 */
	case SBOCOMMANDID_SUB_MSGCMD_BALLOON:		RecvProcMSGCMD_BALLOON		(pData, dwSessionID);	break;	/* ���o�� */
	case SBOCOMMANDID_SUB_MSGCMD_DICE:			RecvProcMSGCMD_DICE			(pData, dwSessionID);	break;	/* �T�C�R�� */
	case SBOCOMMANDID_SUB_MSGCMD_RND:			RecvProcMSGCMD_RND			(pData, dwSessionID);	break;	/* �����_�� */
	case SBOCOMMANDID_SUB_MSGCMD_EFFECT:		RecvProcMSGCMD_EFFECT		(pData, dwSessionID);	break;	/* �G�t�F�N�g */
	case SBOCOMMANDID_SUB_MSGCMD_WHERE:			RecvProcMSGCMD_WHERE		(pData, dwSessionID);	break;	/* �ł��W�܂��Ă���ꏊ */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_CHGFACE								 */
/* ���e		:��M����(�\��ύX)												 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_CHGFACE(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_CHGFACE Packet;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	if ((Packet.m_nType <= 0) || (Packet.m_nType > 18)) {
		return;
	}
	pInfoChar->m_wGrpIDEye = Packet.m_nType;

	PacketRES_CHARINFO.Make (pInfoChar);
	SendToScreenChar (pInfoChar, &PacketRES_CHARINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_CHGHAIR								 */
/* ���e		:��M����(���ύX)												 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_CHGHAIR(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_PARA1 Packet;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	pInfoChar->m_wGrpIDHairType = (WORD)Packet.m_dwPara;

	PacketRES_CHARINFO.Make (pInfoChar);
	SendToScreenChar (pInfoChar, &PacketRES_CHARINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_CHGCLOTH							 */
/* ���e		:��M����(�����ύX)												 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_CHGCLOTH(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_CHGCLOTH Packet;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	if ((Packet.m_nType <= 0) || (Packet.m_nType > 32)) {
		return;
	}
	pInfoChar->m_wGrpIDCloth = Packet.m_nType;

	PacketRES_CHARINFO.Make (pInfoChar);
	SendToScreenChar (pInfoChar, &PacketRES_CHARINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_CHGACCE								 */
/* ���e		:��M����(�A�N�Z�T���ύX)										 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_CHGACCE(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_CHGACCE Packet;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	if ((Packet.m_nType < 0) || (Packet.m_nType > 56)) {
		return;
	}
	pInfoChar->m_wGrpIDAcce = Packet.m_nType;

	PacketRES_CHARINFO.Make (pInfoChar);
	SendToScreenChar (pInfoChar, &PacketRES_CHARINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_CHGCOLOR							 */
/* ���e		:��M����(�F�ύX)												 */
/* ���t		:2007/05/09														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_CHGCOLOR(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	COLORREF cl;
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_CHGCOLOR Packet;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	if ((Packet.m_nType < 0) || (Packet.m_nType > 6)) {
		return;
	}
	cl = RGB (255, 255, 255);
	switch (Packet.m_nType) {
	case 0:	cl = RGB (255, 255, 255);	break;	/* �� */
	case 1:	cl = RGB (255, 200, 200);	break;	/* �� */
	case 2:	cl = RGB (255, 255, 200);	break;	/* �� */
	case 3:	cl = RGB (200, 255, 200);	break;	/* �� */
	case 4:	cl = RGB (200, 255, 255);	break;	/* �� */
	case 5:	cl = RGB (200, 200, 255);	break;	/* �� */
	case 6:	cl = RGB (255, 200, 255);	break;	/* �� */
	}
	pInfoChar->m_clName		= cl;
	pInfoChar->m_clSpeak	= cl;

	PacketRES_CHARINFO.Make (pInfoChar);
	SendToScreenChar (pInfoChar, &PacketRES_CHARINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_CHGARMS								 */
/* ���e		:��M����(�������ύX)											 */
/* ���t		:2007/07/21														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_CHGARMS(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_CHGARMS Packet;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	pInfoChar->m_wGrpIDArmsMain = 0;
	pInfoChar->m_wGrpIDArmsSub = Packet.m_nType;
#if 0
	if ((Packet.m_nType >= 1) && (Packet.m_nType <= 35)) {
		pInfoChar->m_wGrpIDArmsMain = 0;
		pInfoChar->m_wGrpIDArmsSub = Packet.m_nType;
	} else if ((Packet.m_nType >= 100) && (Packet.m_nType <= 103)) {
		pInfoChar->m_wGrpIDArmsMain = 1;
		pInfoChar->m_wGrpIDArmsSub = Packet.m_nType - 100 + 1;
	} else {
		pInfoChar->m_wGrpIDArmsMain = 0;
		pInfoChar->m_wGrpIDArmsSub = 0;
	}
#endif

	PacketRES_CHARINFO.Make (pInfoChar);
	SendToScreenChar (pInfoChar, &PacketRES_CHARINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_CHGSHIELD							 */
/* ���e		:��M����(���ύX)												 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_CHGSHIELD(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_PARA1 Packet;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	pInfoChar->m_wGrpIDArmsLeftMain	= ITEMTYPEID_SHIELD;
	pInfoChar->m_wGrpIDArmsLeftSub	= (WORD)Packet.m_dwPara;

	PacketRES_CHARINFO.Make (pInfoChar);
	SendToScreenChar (pInfoChar, &PacketRES_CHARINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_MAKEITEM							 */
/* ���e		:��M����(�A�C�e���쐬)											 */
/* ���t		:2007/10/20														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_MAKEITEM(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	DWORD dwItemID;
	POINT ptPos;
	PCInfoItem pInfoItem;
	PCInfoMapBase pInfoMap;
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_MAKEITEM Packet;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
//Todo:�����`�F�b�N
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	pInfoChar->GetFrontPos (ptPos);
	bResult = pInfoMap->IsMove (ptPos.x / 2, ptPos.y / 2, pInfoChar->m_nDirection);
	if (bResult == FALSE) {
		return;
	}

	dwItemID = m_pLibInfoItem->MakeItem (pInfoChar->m_dwMapID, &ptPos, Packet.m_dwItemTypeID);
	if (dwItemID == 0) {
		return;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}
	PacketITEM_RES_ITEMINFO.Make (pInfoItem);
	SendToMapChar (pInfoChar->m_dwMapID, &PacketITEM_RES_ITEMINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_BALLOON								 */
/* ���e		:��M����(���o��)												 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_BALLOON(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_PARA1 Packet;
	CPacketCHAR_SET_EFCBALLOON PacketSET_EFCBALLOON;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}

	PacketSET_EFCBALLOON.Make (Packet.m_dwCharID, Packet.m_dwPara);
	SendToScreenChar (pInfoChar, &PacketSET_EFCBALLOON);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_DICE								 */
/* ���e		:��M����(�T�C�R��)												 */
/* ���t		:2008/06/16														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_DICE(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_PARA1 Packet;
	CmyString strTmp;
	CPacketMAP_SYSTEMMSG PacketSYSTEMMSG;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	strTmp.Format ("%s�̃T�C�R���I[%d]���o�܂���", (LPCSTR)pInfoChar->m_strCharName, (genrand () % 6) + 1);
	PacketSYSTEMMSG.Make (strTmp, RGB (255, 255, 255), FALSE);
	SendToScreenChar (pInfoChar, &PacketSYSTEMMSG);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_RND									 */
/* ���e		:��M����(�����_��)												 */
/* ���t		:2008/06/19														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_RND(PBYTE pData, DWORD dwSessionID)
{
	int nTmp;
	PCInfoCharBase pInfoChar;
	CPacketMSGCMD_PARA1 Packet;
	CmyString strTmp;
	CPacketMAP_SYSTEMMSG PacketSYSTEMMSG;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	nTmp = Packet.m_dwPara;
	if (nTmp != 0) {
		nTmp = min (nTmp, 1000);
		nTmp = max (nTmp, 2);
	} else {
		nTmp = 1000;
	}
	strTmp.Format ("%s�� 1�`%d�I�I[%d]���o�܂���",
			(LPCSTR)pInfoChar->m_strCharName,
			nTmp,
			(genrand () % nTmp) + 1);
	PacketSYSTEMMSG.Make (strTmp, RGB (255, 255, 255), FALSE);
	SendToScreenChar (pInfoChar, &PacketSYSTEMMSG);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_EFFECT								 */
/* ���e		:��M����(�G�t�F�N�g)											 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_EFFECT(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketMSGCMD_PARA1 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	pInfoChar->m_nReserveChgEfect = (int)Packet.m_dwPara;
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcMSGCMD_WHERE								 */
/* ���e		:��M����(�ł��W�܂��Ă���ꏊ)									 */
/* ���t		:2008/10/18														 */
/* ========================================================================= */

void CMainFrame::RecvProcMSGCMD_WHERE(PBYTE pData, DWORD dwSessionID)
{
	int nCount;
	DWORD dwTmp;
	PCInfoCharSvr pInfoChar;
	CPacketMSGCMD_PARA1 Packet;
	CPacketMAP_SYSTEMMSG PacketMAP_SYSTEMMSG;
	CmyString strTmp, strTmp2;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	strTmp.Format ("SYSTEM:���ݍł��l���W�܂��Ă���ꏊ��");
	PacketMAP_SYSTEMMSG.Make (strTmp, RGB (255, 255, 255), TRUE, SYSTEMMSGTYPE_NOLOG);
	m_pSock->SendTo (dwSessionID, &PacketMAP_SYSTEMMSG);
	dwTmp  = m_pLibInfoChar->GetPlaceName (strTmp2);
	nCount = m_pLibInfoChar->GetCountOnline (dwTmp);

	if (strTmp2.IsEmpty ()) {
		strTmp.Format ("SYSTEM:�I�����C���� %d �Ń}�b�v�ԍ�[%d]�̂悤�ł�", nCount, dwTmp);
	} else {
		strTmp.Format ("SYSTEM:�I�����C���� %d ��[%s]�̂悤�ł�", nCount, (LPCSTR)strTmp2);
	}
	PacketMAP_SYSTEMMSG.Make (strTmp, RGB (255, 255, 255), FALSE, SYSTEMMSGTYPE_NOLOG);
	m_pSock->SendTo (dwSessionID, &PacketMAP_SYSTEMMSG);
}

/* Copyright(C)URARA-works 2007 */
