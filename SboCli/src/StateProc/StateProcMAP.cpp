/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:StateProcMAP.cpp											 */
/* ���e			:��ԏ����N���X(�}�b�v���) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapBase.h"
#include "LibInfoItem.h"
#include "ParamUtil.h"
#include "InfoMapBase.h"
#include "InfoTalkEvent.h"
#include "PacketMAP_REQ_MAPINFO.h"
#include "PacketCHAR_MOVEPOS.h"
#include "PacketCHAR_STATE.h"
#include "PacketCHAR_REQ_CHAT.h"
#include "PacketCHAR_REQ_PUTGET.h"
#include "PacketCHAR_REQ_PUSH.h"
#include "PacketCHAR_REQ_TAIL.h"
#include "PacketCHAR_REQ_MODIFY_PARAM.h"
#include "PacketCHAR_REQ_EQUIP.h"
#include "PacketCHAR_REQ_USEITEM.h"
#include "PacketCHAR_REQ_DRAGITEM.h"
#include "PacketCHAR_PARA1.h"
#include "PacketCONNECT_KEEPALIVE.h"
#include "PacketADMIN_MAP_SETPARTS.h"
#include "PacketADMIN_MAP_SETMAPSHADOW.h"
#include "PacketMSGCMD_CHGFACE.h"
#include "PacketMSGCMD_CHGCLOTH.h"
#include "PacketMSGCMD_CHGACCE.h"
#include "PacketMSGCMD_CHGCOLOR.h"
#include "PacketMSGCMD_CHGARMS.h"
#include "PacketMSGCMD_MAKEITEM.h"
#include "PacketMSGCMD_PARA1.h"
#include "Command.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "LayerMap.h"
#include "LayerSystemMsg.h"
#include "WindowCHAT.h"
#include "WindowSYSTEMMENU.h"
#include "WindowSETCOLOR.h"
#include "WindowSETBGMVOLUME.h"
#include "WindowSETSOUNDVOLUME.h"
#include "WindowSETDRAWMODE.h"
#include "WindowITEMMENU.h"
#include "WindowITEMMENU_SELECT.h"
#include "WindowOPTION.h"
#include "WindowOPTION_VIEWSET.h"
#include "WindowOPTION_INPUTSET.h"
#include "WindowOPTION_TASKBAR.h"
#include "WindowOPTION_INPUTSET_SETDEVICE.h"
#include "WindowOPTION_ACTIONSET.h"
#include "WindowOPTION_ACTIONSET_SLEEPTIMER.h"
#include "WindowCOMMANDMENU.h"
#include "DlgMsgLog.h"
#include "MainFrame.h"
#include "AdminWindow.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrLayer.h"
#include "MgrKeyInput.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "StateProcMAP.h"

/* ========================================================================= */
/* �֐���	:CStateProcMAP::CStateProcMAP									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CStateProcMAP::CStateProcMAP()
{
	m_bChgScrollMode		= FALSE;
	m_bSendCheckMapEvent	= FALSE;
	m_nScrollMode			= 0;
	m_dwLastTimeMove		= 0;
	m_dwLastTimeKeepAlive	= 0;
	m_dwLastBalloonID		= 0;
	m_dwLastKeyInput		= 0;
	m_dwLastTimeGauge		= 0;
	m_dwStartChargeTime		= 0;

	m_pAdminWindow		= NULL;
	m_pPlayerChar		= NULL;
	m_pMap				= NULL;
	m_pLibInfoChar		= NULL;
	m_pLibInfoMap		= NULL;
	m_pLibInfoItem		= NULL;
	m_pDlgMsgLog		= NULL;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::~CStateProcMAP									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CStateProcMAP::~CStateProcMAP()
{
	if (m_pAdminWindow) {
		m_pMgrData->SetAdminWindow (NULL);
		m_pAdminWindow->Destroy ();
		m_pAdminWindow = NULL;
	}
	m_pMgrSound->StopBGM ();
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CStateProcMAP::Create(CMgrData *pMgrData, CUraraSockTCPSBO *pSock)
{
	CStateProcBase::Create (pMgrData, pSock);

	m_pLibInfoChar		= pMgrData->GetLibInfoChar ();
	m_pLibInfoMap		= pMgrData->GetLibInfoMap ();
	m_pLibInfoItem		= pMgrData->GetLibInfoItem ();

	m_pDlgMsgLog = new CDlgMsgLog;
	m_pDlgMsgLog->Create (pMgrData->GetMainWindow (), m_pMgrData);
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::Init											 */
/* ���e		:������															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CStateProcMAP::Init(void)
{
	char szFileName[MAX_PATH];
	CRect rc;

	ZeroMemory (szFileName, sizeof (szFileName));
	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	rc.left		= GetPrivateProfileInt ("Pos", "LogLeft",	-1, szFileName);
	rc.top		= GetPrivateProfileInt ("Pos", "LogTop",	-1, szFileName);
	rc.right	= GetPrivateProfileInt ("Pos", "LogRight",	-1, szFileName);
	rc.bottom	= GetPrivateProfileInt ("Pos", "LogBottom",	-1, szFileName);
	if (!((rc.left == -1) && (rc.top == -1))) {
		m_pDlgMsgLog->SetWindowPos (NULL, rc.left, rc.top, rc.Width (), rc.Height (), SWP_NOZORDER | SWP_NOACTIVATE);
	}

	m_pPlayerChar	= m_pMgrData->GetPlayerChar ();
	m_pMap			= m_pMgrData->GetMap ();

	m_pMgrLayer->MakeMAP ();
	m_pMgrLayer->MakeSYSTEMMSG ();

	if (m_pMgrData->GetAdminLevel () > ADMINLEVEL_NONE) {
		m_pAdminWindow = new CAdminWindow;
		m_pAdminWindow->Create (m_pMgrData->GetMainWindow (), m_pMgrData);
		m_hWndAdmin = m_pMgrData->GetAdminWindow ();
	}

	m_pMgrDraw->SetFadeState (FADESTATE_FADEIN);

	if (m_pMap) {
		m_pMgrSound->PlayBGM (m_pMap->m_dwBGMID);
	} else {
		m_pMgrSound->StopBGM ();
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::GetMsgLogRect									 */
/* ���e		:���b�Z�[�W���O�E�B���h�E�̋�`���擾							 */
/* ���t		:2008/06/09														 */
/* ========================================================================= */

void CStateProcMAP::GetMsgLogRect(RECT &rcDst)
{
	if (m_pDlgMsgLog->IsWindowVisible ()) {
		m_pDlgMsgLog->GetWindowRect (&rcDst);
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/09/16														 */
/* ========================================================================= */

BOOL CStateProcMAP::TimerProc(void)
{
	BOOL bRet, bResult;
	DWORD dwTime;
	PCLayerMap pLayerMap;
	PCMgrKeyInput pMgrKeyInput;

	bRet = FALSE;
	m_pPlayerChar = m_pMgrData->GetPlayerChar ();

	if (m_dwLastTimeMove != 0) {
		dwTime = timeGetTime () - m_dwLastTimeMove;
		if (dwTime > 2 * 1000) {
			pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
			if (pLayerMap->m_nSystemIconMode != 0) {
				pLayerMap->SetSystemIconMode (0);
				bRet = TRUE;
			}
		}
	}

	dwTime = timeGetTime () - m_dwLastTimeKeepAlive;
	if (dwTime > 20 * 1000) {
		CPacketCONNECT_KEEPALIVE Packet;

		/* �����m�F�ʒm�𑗐M */
		Packet.Make ();
		m_pSock->Send (&Packet);
		m_dwLastTimeKeepAlive = timeGetTime ();
	}

	if (m_pPlayerChar) {
		pMgrKeyInput = m_pMgrData->GetMgrKeyInput ();
		bResult = pMgrKeyInput->IsInput ('Z');
		if (m_pMgrWindow->IsKeyInput ()) {
			bResult = FALSE;
		}
		if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			if (bResult == FALSE) {
				/* �h����� */
				DefenseOff ();
			}
		} else {
			if (bResult) {
				OnZ (TRUE);
			}
		}
		if (m_pPlayerChar->m_bWaitCheckMapEvent && (m_bSendCheckMapEvent == FALSE)) {
			bResult = m_pPlayerChar->IsMove ();
			if (bResult == FALSE) {
				CPacketCHAR_PARA1 PacketPara1;

				/* �}�b�v�C�x���g�`�F�b�N */
				PacketPara1.Make (SBOCOMMANDID_SUB_CHAR_REQ_CHECKMAPEVENT, m_pPlayerChar->m_dwCharID, 0);
				m_pSock->Send (&PacketPara1);
				m_bSendCheckMapEvent = TRUE;
			}
		}
	}

	/* ���Ђ�˃^�C�}�[���� */
	TimerProcSleepTimer ();
	/* ���ߍU������ */
	TimerProcChargeAtack ();
	/* �Q�[�W�� */
	TimerProcGauge ();

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::KeyProc											 */
/* ���e		:�L�[����														 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

void CStateProcMAP::KeyProc(
	BYTE byCode,		/* [in] �C�x���g */
	BOOL bDown)			/* [in] ������� */
{
	if ((byCode == 0) && (bDown == FALSE)) {
		/* ���Ђ�˃^�C�}�[�����ׂ̈Ɏ��ԍX�V */
		m_dwLastKeyInput = timeGetTime ();
		return;
	}

	CStateProcBase::KeyProc (byCode, bDown);
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_WINDOWMSG)								 */
/* ���t		:2007/02/03														 */
/* ========================================================================= */

void CStateProcMAP::OnWindowMsg(int nType, DWORD dwPara)
{
	BOOL bClose;

	bClose = FALSE;
	switch (nType) {
	case WINDOWTYPE_CHAT:							bClose = OnWindowMsgCHAT						(dwPara);	break;	/* �`���b�g���� */
	case WINDOWTYPE_SYSTEMMENU:						bClose = OnWindowMsgSYSTEMMENU					(dwPara);	break;	/* �V�X�e�����j���[ */
	case WINDOWTYPE_SETCOLOR:						bClose = OnWindowMsgSETCOLOR					(dwPara);	break;	/* ���O�Ɣ����F�̐ݒ� */
	case WINDOWTYPE_SETDRAWMODE:					bClose = OnWindowMsgSETDRAWMODE					(dwPara);	break;	/* �\�����e�̐ݒ� */
	case WINDOWTYPE_ITEMMENU:						bClose = OnWindowMsgITEMMENU					(dwPara);	break;	/* �A�C�e�����j���[ */
	case WINDOWTYPE_ITEMMENU_SELECT:				bClose = OnWindowMsgITEMMENU_SELECT				(dwPara);	break;	/* �A�C�e���ǂ����邩���j���[ */
	case WINDOWTYPE_OPTION:							bClose = OnWindowMsgOPTION						(dwPara);	break;	/* �I�v�V���� */
	case WINDOWTYPE_OPTION_VIEWSET:					bClose = OnWindowMsgOPTION_VIEWSET				(dwPara);	break;	/* �I�v�V����-�\���ݒ� */
	case WINDOWTYPE_OPTION_INPUTSET:				bClose = OnWindowMsgOPTION_INPUTSET				(dwPara);	break;	/* �I�v�V����-���͐ݒ� */
	case WINDOWTYPE_OPTION_TASKBAR:					bClose = OnWindowMsgOPTION_TASKBAR				(dwPara);	break;	/* �I�v�V����(�������Ƀ^�X�N�o�[�`�J�`�J) */
	case WINDOWTYPE_OPTION_INPUTSET_SETDEVICE:		bClose = OnWindowMsgOPTION_INPUTSET_SETDEVICE	(dwPara);	break;	/* �I�v�V����-���͐ݒ�-���̓f�o�C�X�̐ݒ� */
	case WINDOWTYPE_OPTION_ACTIONSET:				bClose = OnWindowMsgOPTION_ACTIONSET			(dwPara);	break;	/* �I�v�V����-����ݒ� */
	case WINDOWTYPE_OPTION_ACTIONSET_SLEEPTIMER:	bClose = OnWindowMsgOPTION_ACTIONSET_SLEEPTIMER	(dwPara);	break;	/* �I�v�V����-����ݒ�-���Ђ�˃^�C�}�[�̐ݒ� */
	case WINDOWTYPE_COMMANDMENU:					bClose = OnWindowMsgCOMMANDMENU					(dwPara);	break;	/* �R�}���h���j���[ */
	case WINDOWTYPE_SWOON:							bClose = OnWindowMsgSWOON						(dwPara);	break;	/* �C�⃁�j���[ */
	}
	if (bClose) {
		m_pMgrWindow->Delete (nType);
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnLButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/03/17														 */
/* ========================================================================= */

void CStateProcMAP::OnLButtonDown(int x, int y)
{
	int nMapX, nMapY, nType, xx, yy;
	DWORD dwNotifyData;
	PCLayerMap pLayerMap;
	SIZE size;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pLayerMap->GetViewMapPos (nMapX, nMapY);
	xx = (pLayerMap->m_nViewX % 2) ? x + 16 : x;
	yy = (pLayerMap->m_nViewY % 2) ? y + 16 : y;
	m_hWndAdmin = m_pMgrData->GetAdminWindow ();

	nType = m_pMgrData->GetAdminNotifyTypeL ();
	switch (nType) {
	case ADMINNOTIFYTYPE_CHARID:			/* �L����ID */
	case ADMINNOTIFYTYPE_ACCOUNTID:			/* �A�J�E���gID */
		{
			int i, nCount;
			PCInfoCharCli pInfoChar;

			x = (x / SCROLLSIZE) + (pLayerMap->m_nViewX);
			y = (y / SCROLLSIZE) + (pLayerMap->m_nViewY);
			dwNotifyData = 0;
			y --;
			size.cx = size.cy = 2;

			nCount = m_pLibInfoChar->GetCount ();
			for (i = 0; i < nCount; i ++) {
				pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (i);
				if (pInfoChar->IsHitCharPos (x, y, &size)) {
					dwNotifyData = pInfoChar->m_dwCharID;
					if (nType == ADMINNOTIFYTYPE_ACCOUNTID) {
						dwNotifyData = pInfoChar->m_dwAccountID;
					}
					break;
				}
			}
			PostMessage (m_hWndAdmin, WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, dwNotifyData);
		}
		break;

	case ADMINNOTIFYTYPE_MAPEDIT:			/* �}�b�v�ҏW(�ʒm�͖���) */
		{
			BOOL bPile;
			CPacketADMIN_MAP_SETPARTS Packet;

			x = (xx / 32) + nMapX;
			y = (yy / 32) + nMapY;

			bPile = m_pMgrData->GetEditMapPile ();
			if (bPile) {
				m_pMap->SetPartsPile (x, y, m_pMgrData->GetSelectMapPartsID ());
			} else {
				m_pMap->SetParts (x, y, m_pMgrData->GetSelectMapPartsID ());
			}
			Packet.Make (m_pMap->m_dwMapID, x, y, m_pMgrData->GetSelectMapPartsID (), bPile);
			m_pSock->Send (&Packet);
		}
		break;

	case ADMINNOTIFYTYPE_MAPSHADOWEDIT:		/* �}�b�v�e�ҏW(�ʒm�͖���) */
		{
			CPacketADMIN_MAP_SETMAPSHADOW Packet;

			x = (xx / 32) + nMapX;
			y = (yy / 32) + nMapY;
			m_pMap->SetShadow (x, y, m_pMgrData->GetSelectMapShadowID ());

			Packet.Make (m_pMap->m_dwMapID, x, y, m_pMgrData->GetSelectMapShadowID ());
			m_pSock->Send (&Packet);
		}
		break;

	case ADMINNOTIFYTYPE_ITEMID:			/* �A�C�e��ID */
		{
			POINT ptTmp;
			PCInfoItem pInfoItem;

			x = (x / SCROLLSIZE) + (pLayerMap->m_nViewX);
			y = (y / SCROLLSIZE) + (pLayerMap->m_nViewY);
			dwNotifyData = 0;
			ptTmp.x = x;
			ptTmp.y = y;

			pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pMap->m_dwMapID, &ptTmp);
			if (pInfoItem == NULL) {
				break;
			}
			dwNotifyData = pInfoItem->m_dwItemID;
			PostMessage (m_hWndAdmin, WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, dwNotifyData);
		}
		break;

	case ADMINNOTIFYTYPE_POS:				/* �}�b�v���W */
		x = (xx / 32) + nMapX;
		y = (yy / 32) + nMapY;
		dwNotifyData = MAKELPARAM (y, x);
		PostMessage (m_hWndAdmin, WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, dwNotifyData);
		break;

	case ADMINNOTIFYTYPE_CHARPOS:			/* �L�������W */
		x = (x / SCROLLSIZE) + (pLayerMap->m_nViewX);
		y = (y / SCROLLSIZE) + (pLayerMap->m_nViewY);
		dwNotifyData = MAKELPARAM (y, x);
		PostMessage (m_hWndAdmin, WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, dwNotifyData);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnRButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDOWN)								 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CStateProcMAP::OnRButtonDown(int x, int y)
{
	BOOL bPile;
	int nMapX, nMapY, xx, yy;
	WORD wTmp;
	DWORD dwNotifyData;
	PCLayerMap pLayerMap;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pLayerMap->GetViewMapPos (nMapX, nMapY);
	xx = (pLayerMap->m_nViewX % 2) ? x + 16 : x;
	yy = (pLayerMap->m_nViewY % 2) ? y + 16 : y;

	switch (m_pMgrData->GetAdminNotifyTypeR ()) {
	case ADMINNOTIFYTYPE_MAPEDIT:			/* �}�b�v�ҏW(�ʒm�͖���) */
		x = (xx / 32) + nMapX;
		y = (yy / 32) + nMapY;
		bPile = m_pMgrData->GetEditMapPile ();
		if (bPile) {
			wTmp = m_pMap->GetPartsPile (x, y);
		} else {
			wTmp = m_pMap->GetParts (x, y);
		}
		m_pMgrData->SetSelectMapPartsID (wTmp);
		break;

	case ADMINNOTIFYTYPE_MAPSHADOWEDIT:		/* �}�b�v�e�ҏW(�ʒm�͖���) */
		x = (xx / 32) + nMapX;
		y = (yy / 32) + nMapY;
		wTmp = m_pMap->GetShadow (x, y);
		m_pMgrData->SetSelectMapShadowID (wTmp);
		break;

	case ADMINNOTIFYTYPE_POS:				/* �}�b�v���W */
		x = (xx / 32) + nMapX;
		y = (yy / 32) + nMapY;
		dwNotifyData = MAKELPARAM (y, x);
		PostMessage (m_hWndAdmin, WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDOWN, dwNotifyData);
		break;

	case ADMINNOTIFYTYPE_CHARPOS:			/* �L�������W */
		x = (x / SCROLLSIZE) + (pLayerMap->m_nViewX);
		y = (y / SCROLLSIZE) + (pLayerMap->m_nViewY);
		dwNotifyData = MAKELPARAM (y, x);
		PostMessage (m_hWndAdmin, WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDOWN, dwNotifyData);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnRButtonDblClk									 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDBLCLK)							 */
/* ���t		:2007/09/16														 */
/* ========================================================================= */

void CStateProcMAP::OnRButtonDblClk(int x, int y)
{
	int nMapX, nMapY, xx, yy;
	DWORD dwNotifyData;
	PCLayerMap pLayerMap;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pLayerMap->GetViewMapPos (nMapX, nMapY);
	xx = (pLayerMap->m_nViewX % 2) ? x + 16 : x;
	yy = (pLayerMap->m_nViewY % 2) ? y + 16 : y;

	switch (m_pMgrData->GetAdminNotifyTypeRR ()) {
	case ADMINNOTIFYTYPE_POS:				/* �}�b�v���W */
		x = (xx / 32) + nMapX;
		y = (yy / 32) + nMapY;
		dwNotifyData = MAKELPARAM (y, x);
		PostMessage (m_hWndAdmin, WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDBLCLK, dwNotifyData);
		break;

	case ADMINNOTIFYTYPE_CHARPOS:			/* �L�������W */
		x = (x / SCROLLSIZE) + (pLayerMap->m_nViewX);
		y = (y / SCROLLSIZE) + (pLayerMap->m_nViewY);
		dwNotifyData = MAKELPARAM (y, x);
		PostMessage (m_hWndAdmin, WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDBLCLK, dwNotifyData);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnMouseMove										 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEMOVE)								 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CStateProcMAP::OnMouseMove(int x, int y)
{
	int nMapX, nMapY, xx, yy;
	PCLayerMap pLayerMap;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pLayerMap->GetViewMapPos (nMapX, nMapY);
	xx = (pLayerMap->m_nViewX % 2) ? x + 16 : x;
	yy = (pLayerMap->m_nViewY % 2) ? y + 16 : y;

	switch (m_pMgrData->GetAdminNotifyTypeL ()) {
	case ADMINNOTIFYTYPE_MAPEDIT:	/* �}�b�v�ҏW(�ʒm�͖���) */
		{
			BOOL bPile;
			WORD wMapParts;
			DWORD dwMapPartsID;
			CPacketADMIN_MAP_SETPARTS Packet;

			if ((GetAsyncKeyState (VK_LBUTTON) & 0x8000) == 0) {
				break;
			}
			x = (xx / 32) + nMapX;
			y = (yy / 32) + nMapY;
			wMapParts		= m_pMap->GetParts (x, y);
			dwMapPartsID	= m_pMgrData->GetSelectMapPartsID ();
			if (wMapParts == dwMapPartsID) {
				break;
			}
			bPile = m_pMgrData->GetEditMapPile ();
			if (bPile) {
				m_pMap->SetPartsPile (x, y, dwMapPartsID);
			} else {
				m_pMap->SetParts (x, y, dwMapPartsID);
			}

			Packet.Make (m_pMap->m_dwMapID, x, y, m_pMgrData->GetSelectMapPartsID (), bPile);
			m_pSock->Send (&Packet);
		}
		break;

	case ADMINNOTIFYTYPE_MAPSHADOWEDIT:		/* �}�b�v�e�ҏW(�ʒm�͖���) */
		{
			WORD wMapShadow;
			DWORD dwMapShadowID;
			CPacketADMIN_MAP_SETMAPSHADOW Packet;

			if ((GetAsyncKeyState (VK_LBUTTON) & 0x8000) == 0) {
				break;
			}
			x = (xx / 32) + nMapX;
			y = (yy / 32) + nMapY;
			wMapShadow		= m_pMap->GetShadow (x, y);
			dwMapShadowID	= m_pMgrData->GetSelectMapShadowID ();
			if (wMapShadow == dwMapShadowID) {
				break;
			}
			m_pMap->SetShadow (x, y, dwMapShadowID);

			Packet.Make (m_pMap->m_dwMapID, x, y, m_pMgrData->GetSelectMapShadowID ());
			m_pSock->Send (&Packet);
		}
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnMainFrame										 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2007/01/07														 */
/* ========================================================================= */

void CStateProcMAP::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	if (m_pAdminWindow) {
		m_pAdminWindow->PostMessage (WM_MAINFRAME, dwCommand, dwParam);
	}
	m_pPlayerChar = m_pMgrData->GetPlayerChar ();

	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWMAPINFO:		/* �}�b�v���X�V */
		{
			PCLayerMap pLayerMap;
			CRect rcTmp;

			m_pMap = m_pMgrData->GetMap ();
			pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
			pLayerMap->SetCenterPos (m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY);

			rcTmp.left	 = pLayerMap->m_nViewX - 2;
			rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * 2) + 2;
			rcTmp.top	 = pLayerMap->m_nViewY - 2;
			rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * 2) + 2;
			m_pLibInfoItem->SetArea (m_pMgrData->GetPlayerChar ()->m_dwMapID, &rcTmp);
		}
		break;

	case MAINFRAMEMSG_RENEWCHARINFO:	/* �L�������X�V */
		m_pMgrWindow->Update ();
		break;

	case MAINFRAMEMSG_RENEWCHARCHAT:	/* �������e�X�V */
		{
			PCInfoCharCli pInfoChar;
			CmyString strTmp;

			pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (dwParam);
			if (pInfoChar == NULL) {
				break;
			}
			if (pInfoChar == m_pPlayerChar) {
				KeyProc (0, FALSE);
			}
			strTmp.Format ("%s�F%s", (LPCSTR)pInfoChar->m_strCharName, (LPCSTR)pInfoChar->m_strSpeak);
			m_pDlgMsgLog->Add (strTmp, pInfoChar->m_clSpeak);
		}
		break;

	case MAINFRAMEMSG_RENEWCHARCOUNT:	/* �L�������X�V */
		m_pDlgMsgLog->SetCharCount (dwParam);
		break;

	case MAINFRAMEMSG_RENEWONLINECOUNT:	/* �I�����C�����X�V */
		m_pDlgMsgLog->SetOnlineCount (m_pMgrData->GetOnlineCount ());
		break;

	case MAINFRAMEMSG_RENEWSYSTEMMSG:	/* �V�X�e�����b�Z�[�W�X�V */
		{
			int i, nCount;
			PCLayerSystemMsg pLayerSystemMsg;
			PSTSYSTEMMSG pSystemMsg;

			nCount = m_pMgrData->GetSystemMsgCount ();
			if (nCount <= 0) {
				break;
			}
			pLayerSystemMsg = (PCLayerSystemMsg)m_pMgrLayer->Get (LAYERTYPE_SYSTEMMSG);
			for (i = 0; i < nCount; i ++) {
				pSystemMsg = m_pMgrData->GetSystemMsg (i);

				if (pSystemMsg->bAddLog) {
					m_pDlgMsgLog->Add (pSystemMsg->strMsg, pSystemMsg->clMsg);
				}
				pLayerSystemMsg->AddMsg (pSystemMsg->strMsg, pSystemMsg->clMsg);
			}
			m_pMgrData->DeleteAllSystemMsg ();
		}
		break;

	case MAINFRAMEMSG_RENEWADMINLEVEL:	/* �Ǘ��Ҍ����ύX */
		if (m_pAdminWindow) {
			m_pMgrData->SetAdminWindow (NULL);
			SAFE_DELETE (m_pAdminWindow);
		}
		if (m_pMgrData->GetAdminLevel () > ADMINLEVEL_NONE) {
			m_pAdminWindow = new CAdminWindow;
			m_pAdminWindow->Create (m_pMgrData->GetMainWindow (), m_pMgrData);
			m_hWndAdmin = m_pMgrData->GetAdminWindow ();
		}
		break;

	case MAINFRAMEMSG_RENEWITEMINFO:	/* �A�C�e�����X�V */
		OnMainFrameRENEWITEMINFO (dwParam);
		break;

	case MAINFRAMEMSG_RENEWTALKEVENT:	/* ��b�C�x���g���X�V */
		OnMainFrameRENEWTALKEVENT (dwParam);
		break;

	case MAINFRAMEMSG_DAMAGE:			/* �_���[�W���󂯂� */
		{
			int nTmp;

			if (m_pPlayerChar == NULL) {
				break;
			}
			m_dwLastTimeGauge = timeGetTime ();

			nTmp = 3;
			if (m_pPlayerChar->m_wAtackGauge < 3) {
				nTmp = m_pPlayerChar->m_wAtackGauge;
			}
			m_pPlayerChar->m_wAtackGauge -= nTmp;
			nTmp = 3;
			if (m_pPlayerChar->m_wDefenseGauge < 3) {
				nTmp = m_pPlayerChar->m_wDefenseGauge;
			}
			m_pPlayerChar->m_wDefenseGauge -= nTmp;
		}
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnAdminMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CStateProcMAP::OnAdminMsg(int nCode, DWORD dwPara)
{
	if (m_pAdminWindow) {
		m_pAdminWindow->PostMessage (WM_ADMINMSG, nCode, dwPara);
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::TimerProcSleepTimer								 */
/* ���e		:���ԏ���(���Ђ�˃^�C�}�[)										 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

void CStateProcMAP::TimerProcSleepTimer(void)
{
	BOOL bSleepTime;
	int nState, nSleepTimer, anSleepTimer[] = {0, 3, 5, 10, 30, 60};
	CPacketCHAR_STATE Packet;

	if (m_pPlayerChar == NULL) {
		return;
	}

	nState		= -1;
	nSleepTimer	= m_pMgrData->GetSleepTimer ();
	nSleepTimer	= anSleepTimer[nSleepTimer];

	if (!((nSleepTimer > 0) && (m_dwLastKeyInput != 0))) {
		return;
	}

	bSleepTime = FALSE;
	if (timeGetTime () - m_dwLastKeyInput > (DWORD)(nSleepTimer * 1000 * 60)) {
		bSleepTime = TRUE;
	}

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_STAND:				/* ���� */
	case CHARMOVESTATE_SIT:					/* ���蒆 */
	case CHARMOVESTATE_SLEEPTIMER:			/* ���Ђ�˃^�C�}�[�� */
		if (m_pPlayerChar->IsEnableMove () == FALSE) {
			m_pPlayerChar->SetSleepTimer (FALSE);
			break;
		}
		if (bSleepTime) {
			/* ����H */
			if (m_pPlayerChar->m_bModeSleepTimer == FALSE) {
				nState = CHARMOVESTATE_SLEEPTIMER;
			}
			m_pPlayerChar->SetSleepTimer (TRUE);
		}
		break;
	}

	/* ���Ђ�˃^�C�}�[���H */
	if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SLEEPTIMER) {
		if (bSleepTime == FALSE) {
			if (m_pPlayerChar->m_bModeSleepTimer) {
				nState = CHARMOVESTATE_SIT;
			}
			m_pPlayerChar->SetSleepTimer (FALSE);
		}
	}

	if (nState != -1) {
		m_pPlayerChar->ChgMoveState (nState);
		Packet.Make (m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nMoveState);
		m_pSock->Send (&Packet);
		m_pPlayerChar->SetChgWait (TRUE);
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::TimerProcChargeAtack							 */
/* ���e		:���ԏ���(���ߍU��)												 */
/* ���t		:2008/08/13														 */
/* ========================================================================= */

void CStateProcMAP::TimerProcChargeAtack(void)
{
	BOOL bResult, bCancel;
	DWORD dwTime;
	CMainFrame *pMainFrame;
	PCMgrKeyInput pMgrKeyInput;
	CPacketCHAR_PARA1 Packet;

	bCancel = FALSE;
	pMgrKeyInput = m_pMgrData->GetMgrKeyInput ();
	pMainFrame	 = m_pMgrData->GetMainFrame ();

	if (m_pPlayerChar == NULL) {
		bCancel = TRUE;
		goto Exit;
	}
	if (m_dwStartChargeTime == 0) {
		return;
	}
	if (m_pMgrWindow->IsKeyInput ()) {
		bCancel = TRUE;
		goto Exit;
	}
	if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SWOON) {
		bCancel = TRUE;
		goto Exit;
	}
	if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
		bCancel = TRUE;
		goto Exit;
	}
	bResult = pMgrKeyInput->IsInput ('X');
	if (bResult == FALSE) {
		bCancel = TRUE;
		goto Exit;
	}
	if (m_pPlayerChar->m_bChargeAtack) {
		return;
	}

	dwTime = timeGetTime () - m_dwStartChargeTime;
	if (dwTime < 2000) {
		return;
	}

	/* ���ߏ��ON��ʒm */
	Packet.Make (SBOCOMMANDID_SUB_CHAR_STATE_CHARGE, m_pPlayerChar->m_dwCharID, 1);
	m_pSock->Send (&Packet);
	m_pPlayerChar->m_bChargeAtack = TRUE;
	pMainFrame->ChgMoveState (FALSE);

Exit:
	if (bCancel) {
		if (m_pPlayerChar) {
			m_pPlayerChar->m_bChargeAtack = FALSE;
			/* ���ߏ��OFF��ʒm */
			Packet.Make (SBOCOMMANDID_SUB_CHAR_STATE_CHARGE, m_pPlayerChar->m_dwCharID, 0);
			m_pSock->Send (&Packet);
		}
		m_dwStartChargeTime = 0;
		pMainFrame->ChgMoveState (FALSE);
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::TimerProcGauge									 */
/* ���e		:���ԏ���(�Q�[�W��)											 */
/* ���t		:2008/08/23														 */
/* ========================================================================= */

void CStateProcMAP::TimerProcGauge(void)
{
	BOOL bResult;
	int nTmp;
	float fAverage;
	DWORD dwTime;

	if (m_pPlayerChar == NULL) {
		return;
	}
	if (m_dwLastTimeGauge == 0) {
		return;
	}

	bResult = m_pPlayerChar->IsStateBattle ();
	if (bResult == FALSE) {
		return;
	}

	dwTime = timeGetTime () - m_dwLastTimeGauge;
	if (dwTime < 750) {
		return;
	}
	dwTime -= 750;
	fAverage = (float)dwTime * 100.0f / 5000.0f;

	/* �A�^�b�N�Q�[�W�̏��� */
	{
		nTmp = (int)(MAX_ATACKGAUGE * fAverage * 0.02f);
		if (nTmp > 0) {
			m_pPlayerChar->m_wAtackGauge += (WORD)nTmp;
			if (m_pPlayerChar->m_wAtackGauge >= MAX_ATACKGAUGE) {
				m_pPlayerChar->m_wAtackGauge = MAX_ATACKGAUGE;
			}
		}
	}

	/* �K�[�h�Q�[�W�̏��� */
	{
		nTmp = (int)(MAX_DEFENSEGAUGE * fAverage * 0.02f);
		if (nTmp > 0) {
			m_pPlayerChar->m_wDefenseGauge += (WORD)nTmp;
			if (m_pPlayerChar->m_wDefenseGauge >= MAX_DEFENSEGAUGE) {
				m_pPlayerChar->m_wDefenseGauge = MAX_DEFENSEGAUGE;
			}
		}
	}

	if ((m_pPlayerChar->m_wAtackGauge >= MAX_ATACKGAUGE) &&
		(m_pPlayerChar->m_wDefenseGauge >= MAX_DEFENSEGAUGE)) {
		m_dwLastTimeGauge = 0;

	} else {
		/* �A�^�b�N�Q�[�W�ƃK�[�h�Q�[�W�̍ő傪�����Ԃ͓��������ŁB */
		if (nTmp > 0) {
			m_dwLastTimeGauge = timeGetTime () - 750;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::IsKeyInputEnable								 */
/* ���e		:�L�[���͂������邩����										 */
/* ���t		:2008/06/29														 */
/* �߂�l	:TRUE:����														 */
/* ========================================================================= */

BOOL CStateProcMAP::IsKeyInputEnable(void)
{
	BOOL bRet;
	PCLayerMap pLayerMap;
	PCInfoCharCli pInfoChar;

	bRet = FALSE;
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		goto Exit;
	}
	if (m_pMgrDraw->IsFade ()) {
		goto Exit;
	}
	pInfoChar = m_pMgrData->GetPlayerChar ();
	if (pInfoChar == NULL) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnUp											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnUp(BOOL bDown)
{
	BOOL bRet, bResult;
	int x, y, nDirection;
	PCMgrKeyInput pMgrKeyInput;

	pMgrKeyInput = m_pMgrData->GetMgrKeyInput ();
	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	x = 0;
	y = -1;
	nDirection = 0;
	bResult = pMgrKeyInput->IsInput (VK_RIGHT);
	if (bResult) {
		nDirection = 4;
		x = 1;
	}
	bResult = pMgrKeyInput->IsInput (VK_LEFT);
	if (bResult) {
		nDirection = 7;
		x = -1;
	}

	bResult = MoveProc (m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY, x, y, nDirection);
	if (bResult == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnDown											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnDown(BOOL bDown)
{
	BOOL bRet, bResult;
	int x, y, nDirection;
	PCMgrKeyInput pMgrKeyInput;

	pMgrKeyInput = m_pMgrData->GetMgrKeyInput ();
	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	x = 0;
	y = 1;
	nDirection = 1;
	bResult = pMgrKeyInput->IsInput (VK_RIGHT);
	if (bResult) {
		nDirection = 5;
		x = 1;
	}
	bResult = pMgrKeyInput->IsInput (VK_LEFT);
	if (bResult) {
		nDirection = 6;
		x = -1;
	}

	bResult = MoveProc (m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY, x, y, nDirection);
	if (bResult == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnLeft											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnLeft(BOOL bDown)
{
	BOOL bRet, bResult;
	int x, y, nDirection;
	PCMgrKeyInput pMgrKeyInput;

	pMgrKeyInput = m_pMgrData->GetMgrKeyInput ();
	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	x = -1;
	y = 0;
	nDirection = 2;
	bResult = pMgrKeyInput->IsInput (VK_UP);
	if (bResult) {
		nDirection = 7;
		y = -1;
	}
	bResult = pMgrKeyInput->IsInput (VK_DOWN);
	if (bResult) {
		nDirection = 6;
		y = 1;
	}

	bResult = MoveProc (m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY, x, y, nDirection);
	if (bResult == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnRight											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnRight(BOOL bDown)
{
	BOOL bRet, bResult;
	int x, y, nDirection;
	PCMgrKeyInput pMgrKeyInput;

	pMgrKeyInput = m_pMgrData->GetMgrKeyInput ();
	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	x = 1;
	y = 0;
	nDirection = 3;
	bResult = pMgrKeyInput->IsInput (VK_UP);
	if (bResult) {
		nDirection = 4;
		y = -1;
	}
	bResult = pMgrKeyInput->IsInput (VK_DOWN);
	if (bResult) {
		nDirection = 5;
		y = 1;
	}

	bResult = MoveProc (m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY, x, y, nDirection);
	if (bResult == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnX												 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnX(BOOL bDown)
{
	int i, nCount, anDirection[] = {0, 1, 2, 3, 0, 1, 1, 0};
	BOOL bRet, bResult;
	DWORD dwFrontCharID;
	POINT ptPos, ptFrontPos;
	PCInfoItem pInfoItem;
	CMainFrame *pMainFrame;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CPacketCHAR_REQ_PUTGET PacketCHAR_REQ_PUTGET;
	CPacketCHAR_MOVEPOS PacketCHAR_MOVEPOS;
	CPacketCHAR_REQ_TAIL PacketCHAR_REQ_TAIL;
	CPacketCHAR_PARA1 PacketCHAR_PARA1;
	CmyArray<POINT, POINT> aptPos;

	bRet = FALSE;

	pMainFrame = m_pMgrData->GetMainFrame ();
	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	bResult = m_pPlayerChar->IsChgWait ();
	/* ��ԕύX�҂��H */
	if (bResult) {
		goto Exit;
	}
	if (bDown) {
		if ((m_dwStartChargeTime == 0) && (m_pPlayerChar->m_bChargeAtack == FALSE)) {
			if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE) {
//Todo:�b��
				if (m_pPlayerChar->m_wAtackGauge > 10) {
					m_pPlayerChar->m_wAtackGauge -= 10;
					m_dwLastTimeGauge = timeGetTime ();

					PacketCHAR_MOVEPOS.Make (
							m_pPlayerChar->m_dwMapID,
							m_pPlayerChar->m_dwCharID,
							anDirection[m_pPlayerChar->m_nDirection],
							m_pPlayerChar->m_nMapX,
							m_pPlayerChar->m_nMapY,
							FALSE);
					m_pSock->Send (&PacketCHAR_MOVEPOS);

					m_pPlayerChar->SetChgWait (TRUE);
					PacketCHAR_STATE.Make (m_pPlayerChar->m_dwCharID, CHARMOVESTATE_BATTLEATACK);
					m_pSock->Send (&PacketCHAR_STATE);
					/* ���ߊJ�n */
					m_dwStartChargeTime = timeGetTime ();
				}
			}
		}
		goto Exit;
	}
	if (m_dwStartChargeTime) {
		if (m_pPlayerChar->m_bChargeAtack) {
			/* ���ߍU�� */
			m_pPlayerChar->SetChgWait (TRUE);
			PacketCHAR_STATE.Make (m_pPlayerChar->m_dwCharID, CHARMOVESTATE_BATTLEATACK);
			m_pSock->Send (&PacketCHAR_STATE);
			pMainFrame->ChgMoveState (FALSE);
		}
		m_dwStartChargeTime = 0;
		goto Exit;
	}

	bResult = m_pPlayerChar->IsEnableMove ();
	if (bResult == FALSE) {
		if ((m_pPlayerChar->m_dwFrontCharID) || (m_pPlayerChar->m_dwTailCharID)) {
			PacketCHAR_REQ_TAIL.Make (m_pPlayerChar->m_dwCharID, 0, FALSE);
			m_pSock->Send (&PacketCHAR_REQ_TAIL);
		}
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	dwFrontCharID = m_pLibInfoChar->GetFrontCharID (m_pPlayerChar->m_dwCharID);

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_STAND:			/* ���� */
		bResult = OnXChar (dwFrontCharID);
		if (bResult) {
			break;
		}

		m_pPlayerChar->GetFrontPos (ptFrontPos);
		m_pPlayerChar->RenewBlockMapArea (ptFrontPos.x, ptFrontPos.y, m_pPlayerChar->m_nDirection);
		nCount = m_pPlayerChar->m_aposBockMapArea.GetSize ();
		for (i = 0; i < nCount; i ++) {
			/* �i���\���`�F�b�N */
			bResult |= !m_pMap->IsMove (m_pPlayerChar->m_aposBockMapArea[i].x, m_pPlayerChar->m_aposBockMapArea[i].y, m_pPlayerChar->m_nDirection);
		}
		bResult = !bResult;
		if (bResult) {
			m_pPlayerChar->GetFrontPos (aptPos);
			nCount = aptPos.GetSize ();
			for (i = 0; i < nCount; i ++) {
				ptPos = aptPos[i];
				pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pPlayerChar->m_dwMapID, &ptPos, FALSE);
				if (pInfoItem) {
					break;
				}
			}

			if (i < nCount) {
				/* �A�C�e���E���v�� */
				PacketCHAR_REQ_PUTGET.Make (m_pPlayerChar->m_dwCharID, 0);
				m_pSock->Send (&PacketCHAR_REQ_PUTGET);
				break;
			}
		}
		if ((m_pPlayerChar->m_dwFrontCharID) || (m_pPlayerChar->m_dwTailCharID)) {
			PacketCHAR_REQ_TAIL.Make (m_pPlayerChar->m_dwCharID, 0, FALSE);
			m_pSock->Send (&PacketCHAR_REQ_TAIL);
		}
		break;

	default:
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnZ												 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnZ(BOOL bDown)
{
	BOOL bRet, bResult, bStateBattle;
	DWORD dwCharID;
	CMainFrame *pMainFrame;
	CPacketCHAR_REQ_TAIL PacketCHAR_REQ_TAIL;
	CPacketCHAR_STATE Packet;
	CPacketCHAR_PARA1 PacketCHAR_PARA1;

	bRet = FALSE;

	pMainFrame	  = m_pMgrData->GetMainFrame ();
	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();
	bStateBattle = m_pPlayerChar->IsStateBattle ();
	if (bDown) {
		if (bStateBattle == FALSE) {
			goto Exit;
		}
		bResult = m_pPlayerChar->IsChgWait ();
		if (bResult) {
			goto Exit;
		}
		if (m_pPlayerChar->m_nMoveState != CHARMOVESTATE_BATTLE) {
			goto Exit;
		}
		Packet.Make (m_pPlayerChar->m_dwCharID, CHARMOVESTATE_BATTLE_DEFENSE);
		m_pSock->Send (&Packet);
		m_pPlayerChar->SetChgWait (TRUE);
		bRet = TRUE;

		m_pPlayerChar->m_bChargeAtack = FALSE;
		/* ���ߏ��OFF��ʒm */
		PacketCHAR_PARA1.Make (SBOCOMMANDID_SUB_CHAR_STATE_CHARGE, m_pPlayerChar->m_dwCharID, 0);
		m_pSock->Send (&PacketCHAR_PARA1);
		m_dwStartChargeTime = 0;
		pMainFrame->ChgMoveState (FALSE);
		goto Exit;
	}

	if (bStateBattle == FALSE) {
		dwCharID = m_pLibInfoChar->GetFrontCharID (m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nDirection);
		PacketCHAR_REQ_TAIL.Make (m_pPlayerChar->m_dwCharID, dwCharID, TRUE);
		m_pSock->Send (&PacketCHAR_REQ_TAIL);
	} else {
		/* �h����� */
		DefenseOff ();
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnS												 */
/* ���e		:�L�[�n���h��(S)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnS(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnR												 */
/* ���e		:�L�[�n���h��(R)												 */
/* ���t		:2007/04/20														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnR(BOOL bDown)
{
	BOOL bRet, bResult;
	CPacketCHAR_STATE Packet;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();
	bResult = m_pPlayerChar->IsChgWait ();
	/* ��ԕύX�҂��H */
	if (bResult) {
		goto Exit;
	}

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_STAND:		/* ���� */
		if (m_pPlayerChar->m_dwFrontCharID != 0) {
			/* ���Ă������͍���Ȃ� */
			goto Exit;
		}
		m_pPlayerChar->ChgMoveState (CHARMOVESTATE_SIT);
		break;
	case CHARMOVESTATE_SIT:			/* ���蒆 */
		m_pPlayerChar->ChgMoveState (CHARMOVESTATE_STAND);
		break;
	default:
		goto Exit;
	}

	Packet.Make (m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nMoveState);
	m_pSock->Send (&Packet);
	m_pPlayerChar->SetChgWait (TRUE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnB												 */
/* ���e		:�L�[�n���h��(B)												 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnB(BOOL bDown)
{
	BOOL bRet;
	PCWindowITEMMENU pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SWOON) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	pWnd = (PCWindowITEMMENU)m_pMgrWindow->GetWindow (WINDOWTYPE_ITEMMENU);
	if (pWnd) {
		goto Exit;
	}

	m_pMgrWindow->MakeWindowITEMMENU ();
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnN												 */
/* ���e		:�L�[�n���h��(N)												 */
/* ���t		:2008/07/14														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnN(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	if (m_pMgrData->GetDrawMode () != 0) {
		m_pMgrData->SetDrawMode (0);
		AddSystemMsg (FALSE, "���O��\���ɕύX����܂����@������[ N ]�L�[", RGB (255, 255, 255));
	} else {
		m_pMgrData->SetDrawMode (1);
		AddSystemMsg (FALSE, "���O��\�����܂�", RGB (255, 255, 255));
	}
	m_pMgrData->SaveIniData ();

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnV												 */
/* ���e		:�L�[�n���h��(V)												 */
/* ���t		:2008/11/18														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnV(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	if (m_pMgrData->GetOptionViewItemName ()) {
		m_pMgrData->SetOptionViewItemName (FALSE);
		AddSystemMsg (FALSE, "�A�C�e�������\���ɂ��܂����@������[ V ]�L�[", RGB (255, 255, 255));
	} else {
		m_pMgrData->SetOptionViewItemName (TRUE);
		AddSystemMsg (FALSE, "�A�C�e������\�����܂�", RGB (255, 255, 255));
	}
	m_pMgrData->SaveIniData ();

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnAt											 */
/* ���e		:�L�[�n���h��(@)												 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnAt(BOOL bDown)
{
	BOOL bRet;
	CPacketMSGCMD_PARA1 Packet;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	Packet.Make (SBOCOMMANDID_SUB_MSGCMD_BALLOON, m_pPlayerChar->m_dwCharID, m_dwLastBalloonID);
	m_pSock->Send (&Packet);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnEnter											 */
/* ���e		:�L�[�n���h��(Enter)											 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnEnter(BOOL bDown)
{
	BOOL bRet;
	PCWindowCHAT pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	pWnd = (PCWindowCHAT)m_pMgrWindow->GetWindow (WINDOWTYPE_CHAT);
	if (pWnd) {
		goto Exit;
	}

	m_pMgrWindow->MakeWindowCHAT ();
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnEscape										 */
/* ���e		:�L�[�n���h��(Escape)											 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnEscape(BOOL bDown)
{
	BOOL bRet;
	PCWindowSYSTEMMENU pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	pWnd = (PCWindowSYSTEMMENU)m_pMgrWindow->GetWindow (WINDOWTYPE_SYSTEMMENU);
	if (pWnd) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	m_pMgrWindow->MakeWindowSYSTEMMENU ();
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnTab											 */
/* ���e		:�L�[�n���h��(Tab)												 */
/* ���t		:2007/07/08														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnTab(BOOL bDown)
{
	BOOL bRet, bResult;
	CMainFrame *pMainFrame;
	CPacketCHAR_STATE Packet;
	CPacketCHAR_PARA1 PacketCHAR_PARA1;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();
	bResult = m_pPlayerChar->IsChgWait ();
	/* ��ԕύX�҂��H */
	if (bResult) {
		goto Exit;
	}
	bResult = m_pPlayerChar->IsEnableMove ();
	if (bResult == FALSE) {
		goto Exit;
	}
	/* ���蒆�H */
	if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SIT) {
		goto Exit;
	}

	/* �N���ɕt���čs���Ă���H */
	if (m_pPlayerChar->m_dwFrontCharID) {
		AddSystemMsg (FALSE, "�t���čs���Ă��鎞�̓��[�h�ύX�ł��܂���", RGB (255, 255, 255));
		goto Exit;
	}

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_STAND:			/* ���� */
		bResult = m_pMap->IsEnableBattle ();
		if (bResult == FALSE) {
			AddSystemMsg (FALSE, "���̃}�b�v�ł͐퓬�ł��܂���", RGB (255, 255, 255));
			goto Exit;
		}
		m_pPlayerChar->SetMoveState (CHARMOVESTATE_BATTLE);
		m_dwLastTimeGauge = timeGetTime ();
		break;
	case CHARMOVESTATE_BATTLE:			/* �퓬�� */
		m_pPlayerChar->SetMoveState (CHARMOVESTATE_STAND);

		if (m_dwStartChargeTime || m_pPlayerChar->m_bChargeAtack) {
			m_dwStartChargeTime = 0;
			m_pPlayerChar->m_bChargeAtack = FALSE;
			/* ���ߏ��OFF��ʒm */
			PacketCHAR_PARA1.Make (SBOCOMMANDID_SUB_CHAR_STATE_CHARGE, m_pPlayerChar->m_dwCharID, 0);
			m_pSock->Send (&PacketCHAR_PARA1);
		}
		break;
	}
	pMainFrame = m_pMgrData->GetMainFrame ();
	pMainFrame->ChgMoveState (TRUE);

	Packet.Make (m_pPlayerChar->m_dwCharID, m_pPlayerChar->m_nMoveState);
	m_pSock->Send (&Packet);
	m_pPlayerChar->SetChgWait (TRUE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnShift											 */
/* ���e		:�L�[�n���h��(Shift)											 */
/* ���t		:2007/07/28														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnShift(BOOL bDown)
{
	BOOL bRet, bResult;
	PCLayerMap pLayerMap;
	PCMgrKeyInput pMgrKeyInput;

	bRet = FALSE;
	pMgrKeyInput = m_pMgrData->GetMgrKeyInput ();

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
		pLayerMap->SetSystemIconMode (1);
		m_dwLastTimeMove = timeGetTime ();

		bResult = pMgrKeyInput->IsInput (VK_CONTROL);
		if (bResult == FALSE) {
			goto Exit;
		}
		pLayerMap->SetCenterPos (m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY);
		AddSystemMsg (FALSE, "�����ʒu�Ɏ��_��߂��܂���", RGB (255, 255, 255));
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnCtrl											 */
/* ���e		:�L�[�n���h��(Ctrl)												 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnCtrl(BOOL bDown)
{
	BOOL bRet;
	RECT rcTmp;
	CmyString strTmp;
	PCLayerMap pLayerMap;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		if (m_bChgScrollMode == FALSE) {
			m_bChgScrollMode = TRUE;
		}
		goto Exit;
	}

	if (m_bChgScrollMode == FALSE) {
		goto Exit;
	}

	m_bChgScrollMode = FALSE;
	m_nScrollMode ++;
	if (m_nScrollMode > 1) {
		m_nScrollMode = 0;
	}

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);

	switch (m_nScrollMode) {
	case 0:
		strTmp = "�X�N���[�����܂�";
		pLayerMap->SetCenterPos (m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY);
		pLayerMap->SetScrollMode (TRUE, 0);
		break;
	case 1:
		strTmp = "��ʌŒ肵�܂�";
		pLayerMap->SetScrollMode (FALSE, 2);
		break;
	}
	if (strTmp.IsEmpty () == FALSE) {
		AddSystemMsg (FALSE, strTmp, RGB (255, 255, 255));
	}

	if (pLayerMap) {
		rcTmp.left	 = pLayerMap->m_nViewX - 2;
		rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * 2) + 2;
		rcTmp.top	 = pLayerMap->m_nViewY - 2;
		rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * 2) + 2;
		m_pLibInfoItem->SetArea (m_pPlayerChar->m_dwMapID, &rcTmp);
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnSpace											 */
/* ���e		:�L�[�n���h��(Space)											 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnSpace(BOOL bDown)
{
	BOOL bRet;
	PCWindowCOMMANDMENU pWnd;

	bRet = FALSE;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	if (bDown) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	switch (m_pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_SWOON:	/* �C�⒆ */
		pWnd = (PCWindowCOMMANDMENU)m_pMgrWindow->GetWindow (WINDOWTYPE_SWOON);
		if (pWnd) {
			goto Exit;
		}
		m_pMgrWindow->MakeWindowSWOON ();
		break;
	default:
		pWnd = (PCWindowCOMMANDMENU)m_pMgrWindow->GetWindow (WINDOWTYPE_COMMANDMENU);
		if (pWnd) {
			goto Exit;
		}
		m_pMgrWindow->MakeWindowCOMMANDMENU ();
		m_pMgrSound->PlaySound (SOUNDID_OPEN_WINDOW);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnMgrDrawSTART_FADEIN							 */
/* ���e		:�t�F�[�h�C���J�n												 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

void CStateProcMAP::OnMgrDrawSTART_FADEIN(DWORD dwPara)
{
	PCLayerMap pLayerMap;
	CMainFrame *pMainFrame;
	RECT rcTmp;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		return;
	}
	if (m_pMgrLayer == NULL) {
		return;
	}
	pMainFrame = m_pMgrData->GetMainFrame ();
	pMainFrame->ChgMoveState (FALSE);

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap) {
		pLayerMap->RenewMapName (NULL);
		pLayerMap->SetCenterPos (m_pPlayerChar->m_nMapX, m_pPlayerChar->m_nMapY);
		rcTmp.left	 = pLayerMap->m_nViewX - 2;
		rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * 2) + 2;
		rcTmp.top	 = pLayerMap->m_nViewY - 2;
		rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * 2) + 2;
		m_pLibInfoItem->SetArea (m_pPlayerChar->m_dwMapID, &rcTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnMgrDrawEND_FADEIN								 */
/* ���e		:�t�F�[�h�C������												 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CStateProcMAP::OnMgrDrawEND_FADEIN(DWORD dwPara)
{
	PCLayerMap pLayerMap;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	if (m_pMap == NULL) {
		return;
	}
	pLayerMap->RenewMapName ((LPCSTR)m_pMap->m_strMapName);
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::MoveProc										 */
/* ���e		:�ړ�����														 */
/* ���t		:2006/11/01														 */
/* �߂�l	:TRUE:�������� FALSE:������										 */
/* ========================================================================= */

BOOL CStateProcMAP::MoveProc(
	int x,				/* [in] ���݈ʒu(���R) */
	int y,				/* [in] ���݈ʒu(�^�e) */
	int xx,				/* [in] ����(���R) */
	int yy,				/* [in] ����(�^�e) */
	int nDirection)		/* [in] ���� */
{
	int i, nCount, nResult, nDirectionBack, nDirectionView, nState, nTmp, xBack, yBack,
		anPosChangeX[] = {0, 0, -1, 1, 1, 1, -1, -1}, anPosChangeY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	BOOL bRet, bResult;
	DWORD dwCharID;
	RECT rcTmp;
	PCInfoMapBase pMap;
	PCLayerMap pLayerMap;
	PCMgrKeyInput pMgrKeyInput;
	CPacketCHAR_MOVEPOS Packet;
	CPacketCHAR_REQ_PUSH PacketREQ_PUSH;
	CPacketCHAR_STATE PacketSTATE;

	bRet = FALSE;
	m_bChgScrollMode = FALSE;

	xBack = m_pPlayerChar->m_nMapX;
	yBack = m_pPlayerChar->m_nMapY;
	nDirectionView = nDirection;

	pLayerMap		= (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	pMgrKeyInput	= m_pMgrData->GetMgrKeyInput ();

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	pMap = m_pMgrData->GetMap ();
	if (pMap == NULL) {
		goto Exit;
	}
	/* �ړ������ɃX�N���[�������H */
	bResult = pMgrKeyInput->IsInput (VK_CONTROL);
	if (bResult) {
		bResult = pLayerMap->IsScrollArea (x, y, nDirection);
		if (bResult) {
			pLayerMap->SetScrollMode (TRUE, 1);
			pLayerMap->Scroll (nDirection);
			if (m_nScrollMode == 1) {
				pLayerMap->SetScrollMode (FALSE, 2);
			}
		}
		bRet = TRUE;
		goto Exit;
	}
	bResult = m_pPlayerChar->IsMove ();
	/* �ړ����H */
	if (bResult) {
		goto Exit;
	}
	bResult = m_pPlayerChar->IsChgWait ();
	/* ��ԕύX�҂��H */
	if (bResult) {
		goto Exit;
	}
	/* �N���ɕt���čs���Ă���H */
	if (m_pPlayerChar->m_dwFrontCharID) {
		goto Exit;
	}
	bResult = m_pPlayerChar->IsEnableMove ();
	if (bResult == FALSE) {
		if (m_pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			/* �h�䒆�͌����̕ύX���� */
			m_pPlayerChar->ChgDirection (nDirection);
			goto ExitSend;
		}
		goto Exit;
	}

	nDirectionBack = m_pPlayerChar->m_nDirection;
	if ((nDirectionBack < 4) && (nDirection >= 4)) {
		bResult = TRUE;
		switch (nDirectionBack) {
		case 0:
			if (yy > 0) {
				bResult = FALSE;
			}
			break;
		case 1:
			if (yy < 0) {
				bResult = FALSE;
			}
			break;
		case 2:
			if (xx > 0) {
				bResult = FALSE;
			}
			break;
		case 3:
			if (xx < 0) {
				bResult = FALSE;
			}
			break;
		}
		if (bResult) {
			/* �΂߂Ɉړ����鎞�͍Ō��4�������ێ����� */
			nDirectionView = nDirectionBack;
		}
	}
	if (m_pPlayerChar->IsStateBattle ()) {
		bResult = pMgrKeyInput->IsInput ('X');
		if (bResult) {
			nDirectionView = nDirectionBack;
		}
	}
	m_pPlayerChar->ChgDirection (nDirectionView);

	/* �ړ������Ɍ����ύX�����H */
	bResult = pMgrKeyInput->IsInput (VK_SHIFT);
	if (bResult) {
		goto ExitSend;
	}
	/* �����Ă���H */
	if ((m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SIT) ||
		(m_pPlayerChar->m_nMoveState == CHARMOVESTATE_SLEEPTIMER)) {
		x = m_pPlayerChar->m_nMapX;
		y = m_pPlayerChar->m_nMapY;
		goto ExitSend;
	}

	nTmp = nDirection;
	bResult = m_pMgrData->GetMoveNoBlock ();
	if (bResult == FALSE) {
		/* �����蔻�薳����ԂłȂ��Ȃ�`�F�b�N */
		bResult = m_pLibInfoChar->IsMove (m_pPlayerChar, nDirection);
	}
	if (bResult) {
		if (nTmp != nDirection) {
			xx = anPosChangeX[nDirection];
			yy = anPosChangeY[nDirection];
			m_pPlayerChar->ChgDirection (nDirection);
		}

	} else {
		m_pPlayerChar->RenewBlockMapArea (0, 0, -1);
		if (nDirection != nDirectionBack) {
			goto ExitSend;
		} else {
			goto Exit;
		}
	}
	/* 1���O�ɉ�����L����������H */
	dwCharID = m_pLibInfoChar->GetFrontCharIDPush (m_pPlayerChar->m_dwCharID, nDirection);
	if (dwCharID) {
		PacketREQ_PUSH.Make (dwCharID, nDirection);
		m_pSock->Send (&PacketREQ_PUSH);
		m_pPlayerChar->m_dwMoveWaitOnce = BATTLEMOVEWAIT;
		pLayerMap->m_dwMoveWaitOnce = BATTLEMOVEWAIT;
	}

	/* �Ԃ���H */
	bResult = m_pLibInfoChar->IsBlockChar (m_pPlayerChar, nDirection);
	if (bResult) {
		bRet = TRUE;
		goto Exit;
	}

	nState = CHARMOVESTATE_MOVE;
	if (m_pPlayerChar->IsStateBattle ()) {
		nState = CHARMOVESTATE_BATTLEMOVE;
	}

	m_pPlayerChar->ChgDirection (nDirectionView);
	m_pPlayerChar->SetPos (x + xx, y + yy);
	m_pPlayerChar->ChgMoveState (nState);

	if (nDirection <= 1) {
		/* �d�Ȃ蒲�� */
		m_pLibInfoChar->SortY ();
	}

	nResult = pLayerMap->IsScrollPos (x + xx, y + yy, nDirection);
	if (nResult >= 0) {
		pLayerMap->Scroll (nResult);
	}
	/* ��ʊO�ɏo��H */
	bResult = pLayerMap->IsInScreen (x + xx, y + yy);
	if (bResult == FALSE) {
		m_nScrollMode = 0;
		pLayerMap->SetScrollMode (TRUE, m_nScrollMode);
		pLayerMap->SetCenterPos (x + xx, y + yy);
	}

	bResult = m_pLibInfoChar->DeleteOutScreen (m_pPlayerChar);
	if (bResult) {
		PostMessage (m_pMgrData->GetMainWindow (), WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount ());
	}
	pLayerMap->SetSystemIconMode (1);
	m_dwLastTimeMove = timeGetTime ();

	x += xx;
	y += yy;
ExitSend:
	m_pPlayerChar->m_bRedraw = TRUE;
	/* �T�[�o�ֈړ��ʒm */
	Packet.Make (m_pPlayerChar->m_dwMapID, m_pPlayerChar->m_dwCharID, nDirectionView, x, y, FALSE);
	m_pSock->Send (&Packet);

	if ((xBack != x) || (yBack != y)) {
		m_dwLastTimeGauge = timeGetTime ();
		m_pPlayerChar->RenewBlockMapArea (x, y, nDirectionView);
		nCount = m_pPlayerChar->m_aposBockMapArea.GetSize ();
		for (i = 0; i < nCount; i ++) {
			bResult = bResult = pMap->IsMapEvent (m_pPlayerChar->m_aposBockMapArea[i].x, m_pPlayerChar->m_aposBockMapArea[i].y);
			if (bResult) {
				/* �}�b�v�C�x���g�`�F�b�N�\�� */
				m_pPlayerChar->m_bWaitCheckMapEvent = TRUE;
				m_bSendCheckMapEvent = FALSE;
			}
		}
	}

	bRet = TRUE;
Exit:
	if (bRet && pLayerMap) {
		rcTmp.left	 = pLayerMap->m_nViewX - 2;
		rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * 2) + 2;
		rcTmp.top	 = pLayerMap->m_nViewY - 2;
		rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * 2) + 2;
		m_pLibInfoItem->SetArea (m_pPlayerChar->m_dwMapID, &rcTmp);
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgCHAT									 */
/* ���e		:�`���b�g����													 */
/* ���t		:2007/06/20														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgCHAT(DWORD dwPara)
{
	int nType;
	LPCSTR pszMsg;
	PCWindowCHAT pWndChat;
	CMainFrame *pMainFrame;

	m_pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (m_pPlayerChar == NULL) {
		goto Exit;
	}
	if (IsKeyInputEnable () == FALSE) {
		goto Exit;
	}
	pWndChat = (PCWindowCHAT)m_pMgrWindow->GetWindow (WINDOWTYPE_CHAT);
	if (pWndChat == NULL) {
		goto Exit;
	}
	m_dwLastKeyInput = timeGetTime ();

	pszMsg = pWndChat->m_strChat;
	nType  = pWndChat->GetType ();

	pMainFrame = m_pMgrData->GetMainFrame ();
	pMainFrame->SendChat (nType, pszMsg, &m_dwLastBalloonID);

Exit:
	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgSYSTEMMENU							 */
/* ���e		:�V�X�e�����j���[												 */
/* ���t		:2007/06/20														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgSYSTEMMENU(DWORD dwPara)
{
	BOOL bRet;
	PCWindowSYSTEMMENU pWnd;

	bRet = TRUE;
	pWnd = (PCWindowSYSTEMMENU)m_pMgrWindow->GetWindow (WINDOWTYPE_SYSTEMMENU);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		/* ���O�Ɣ����F�̐ݒ� */
		m_pMgrWindow->MakeWindowSETCOLOR ();
		break;
	case 1:		/* �I�v�V���� */
		m_pMgrWindow->MakeWindowOPTION ();
		break;
	default:
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgSETCOLOR								 */
/* ���e		:���O�Ɣ����F�̐ݒ�												 */
/* ���t		:2007/06/24														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgSETCOLOR(DWORD dwPara)
{
	BOOL bRet;
	PCWindowSETCOLOR pWnd;
	CPacketMSGCMD_CHGCOLOR Packet;

	bRet = TRUE;
	pWnd = (PCWindowSETCOLOR)m_pMgrWindow->GetWindow (WINDOWTYPE_SETCOLOR);
	if (pWnd == NULL) {
		goto Exit;
	}

	Packet.Make (m_pPlayerChar->m_dwCharID, dwPara);
	m_pSock->Send (&Packet);

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgSETDRAWMODE							 */
/* ���e		:�\�����e�̐ݒ�													 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgSETDRAWMODE(DWORD dwPara)
{
	BOOL bRet;
	PCWindowSETDRAWMODE pWnd;

	bRet = TRUE;
	pWnd = (PCWindowSETDRAWMODE)m_pMgrWindow->GetWindow (WINDOWTYPE_SETDRAWMODE);
	if (pWnd == NULL) {
		goto Exit;
	}

	m_pMgrData->SetDrawMode (dwPara);
	m_pMgrData->SaveIniData ();

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgITEMMENU								 */
/* ���e		:�A�C�e�����j���[												 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgITEMMENU(DWORD dwPara)
{
	BOOL bRet;
	int nPos;
	DWORD dwDragItemID;
	PCWindowITEMMENU pWnd;
	CPacketCHAR_REQ_DRAGITEM Packet;

	bRet = TRUE;
	pWnd = (PCWindowITEMMENU)m_pMgrWindow->GetWindow (WINDOWTYPE_ITEMMENU);
	if (pWnd == NULL) {
		goto Exit;
	}

	dwDragItemID = pWnd->GetDragItemID ();
	if (dwDragItemID != 0) {
		Packet.Make (m_pPlayerChar->m_dwCharID, dwDragItemID, pWnd->GetDragPos ());
		m_pSock->Send (&Packet);
		pWnd->DragOff ();

	} else {
		if (dwPara == 0) {
			goto Exit;
		}
		nPos = pWnd->GetPos ();
		m_pMgrWindow->MakeWindowITEMMENU_SELECT (nPos, dwPara);
	}

	bRet = FALSE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgITEMMENU_SELECT						 */
/* ���e		:�A�C�e���ǂ����邩���j���[										 */
/* ���t		:2007/08/14														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgITEMMENU_SELECT(DWORD dwPara)
{
	BOOL bRet, bResult, bEnableMove;
	DWORD dwItemID;
	int i, nCount, nPosX[] = {0, 0, -1, 1}, nPosY[] = {-1, 1, 0, 0};
	POINT ptFrontPos;
	PCWindowITEMMENU pWndITEMMENU;
	PCWindowITEMMENU_SELECT pWndITEMMENU_SELECT;
	PCPacketBase pPacket;
	CPacketCHAR_REQ_PUTGET PacketCHAR_REQ_PUTGET;
	CPacketCHAR_REQ_EQUIP PacketCHAR_REQ_EQUIP;
	CPacketCHAR_REQ_USEITEM PacketCHAR_REQ_USEITEM;

	bRet	= FALSE;
	pPacket	= NULL;

	pWndITEMMENU = (PCWindowITEMMENU)m_pMgrWindow->GetWindow (WINDOWTYPE_ITEMMENU);
	if (pWndITEMMENU == NULL) {
		goto Exit;
	}
	pWndITEMMENU_SELECT = (PCWindowITEMMENU_SELECT)m_pMgrWindow->GetWindow (WINDOWTYPE_ITEMMENU_SELECT);
	if (pWndITEMMENU_SELECT == NULL) {
		goto Exit;
	}
	dwItemID = pWndITEMMENU->GetSelectItemID ();
	if (dwItemID == 0) {
		goto Exit;
	}
	bEnableMove = m_pPlayerChar->IsEnableMove ();

	switch (dwPara) {
	case ITEMMENU_SELECT_COMMAND_PUT:			/* �n�ʂɒu�� */
		bResult = FALSE;
		m_pPlayerChar->GetFrontPos (ptFrontPos);
		m_pPlayerChar->RenewBlockMapArea (ptFrontPos.x, ptFrontPos.y, m_pPlayerChar->m_nDirection);
		nCount = m_pPlayerChar->m_aposBockMapArea.GetSize ();
		for (i = 0; i < nCount; i ++) {
			/* �E�o�\���`�F�b�N */
			bResult |= !m_pMap->IsMoveOut (m_pPlayerChar->m_aposBockMapArea[i].x, m_pPlayerChar->m_aposBockMapArea[i].y, m_pPlayerChar->m_nDirection);
		}
		bResult = !bResult;
		if (bResult) {
			bResult = FALSE;
			m_pPlayerChar->RenewBlockMapArea (ptFrontPos.x, ptFrontPos.y, m_pPlayerChar->m_nDirection);
			nCount = m_pPlayerChar->m_aposBockMapArea.GetSize ();
			for (i = 0; i < nCount; i ++) {
				/* �i���\���`�F�b�N */
				bResult |= !m_pMap->IsMove (m_pPlayerChar->m_aposBockMapArea[i].x, m_pPlayerChar->m_aposBockMapArea[i].y, m_pPlayerChar->m_nDirection);
			}
			bResult = !bResult;
		}
		if (bResult == FALSE) {
			break;
		}
		PacketCHAR_REQ_PUTGET.Make (m_pPlayerChar->m_dwCharID, dwItemID);
		pPacket = &PacketCHAR_REQ_PUTGET;
		break;
	case ITEMMENU_SELECT_COMMAND_EQUIP:			/* �������� */
		if (bEnableMove == FALSE) {
			AddSystemMsg (FALSE, "�s�����Ȃ̂ő����ύX�ł��܂���", RGB (255, 255, 255));
			break;
		}
		PacketCHAR_REQ_EQUIP.Make (m_pPlayerChar->m_dwCharID, dwItemID, -1);
		pPacket = &PacketCHAR_REQ_EQUIP;
		m_dwLastTimeGauge = timeGetTime ();
		break;
	case ITEMMENU_SELECT_COMMAND_EQUIP_UNSET:	/* �������O�� */
		if (bEnableMove == FALSE) {
			AddSystemMsg (FALSE, "�s�����Ȃ̂ő����ύX�ł��܂���", RGB (255, 255, 255));
			break;
		}
		PacketCHAR_REQ_EQUIP.Make (m_pPlayerChar->m_dwCharID, dwItemID, pWndITEMMENU_SELECT->GetType ());
		pPacket = &PacketCHAR_REQ_EQUIP;
		break;
	case ITEMMENU_SELECT_COMMAND_USE:			/* �g�� */
		PacketCHAR_REQ_USEITEM.Make (m_pPlayerChar->m_dwCharID, dwItemID);
		pPacket = &PacketCHAR_REQ_USEITEM;
		break;
	default:
		goto Exit;
	}

	if (pPacket) {
		m_pSock->Send (pPacket);
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgOPTION								 */
/* ���e		:�I�v�V����														 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgOPTION(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION pWnd;

	bRet = TRUE;
	pWnd = (PCWindowOPTION)m_pMgrWindow->GetWindow (WINDOWTYPE_OPTION);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		/* �\���ݒ� */
		m_pMgrWindow->MakeWindowOPTION_VIEWSET ();
		break;
	case 1:		/* ���ʐݒ� */
		m_pMgrWindow->MakeWindowOPTION_VOLUMESET ();
		break;
	case 2:		/* ���͐ݒ� */
		m_pMgrWindow->MakeWindowOPTION_INPUTSET ();
		break;
	case 3:		/* ����ݒ� */
		m_pMgrWindow->MakeWindowOPTION_ACTIONSET ();
		break;
	default:
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgOPTION_VIEWSET						 */
/* ���e		:�I�v�V����-�\���ݒ�											 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgOPTION_VIEWSET(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION_VIEWSET pWnd;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_VIEWSET)m_pMgrWindow->GetWindow (WINDOWTYPE_OPTION_VIEWSET);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		/* �������Ƀ^�X�N�o�[�`�J�`�J */
		if (m_pMgrData->GetOptionTaskbar ()) {
			m_pMgrData->SetOptionTaskbar (FALSE);
			AddSystemMsg (FALSE, "�������ł��ω����܂���", RGB (255, 255, 255));
		} else {
			m_pMgrData->SetOptionTaskbar (TRUE);
			AddSystemMsg (FALSE, "�������Ƀ^�X�N�o�[��_�ł����܂�", RGB (255, 255, 255));
		}
		m_pMgrData->SaveIniData ();
		break;
	case 1:		/* ���O��\������ */
		if (m_pMgrData->GetDrawMode () != 0) {
			m_pMgrData->SetDrawMode (0);
			AddSystemMsg (FALSE, "���O��\�����܂���", RGB (255, 255, 255));
		} else {
			m_pMgrData->SetDrawMode (1);
			AddSystemMsg (FALSE, "���O��\�����܂�", RGB (255, 255, 255));
		}
		m_pMgrData->SaveIniData ();
		break;
	case 2:		/* ������\������ */
		if (m_pMgrData->GetOptionViewChat () != 0) {
			m_pMgrData->SetOptionViewChat (0);
			AddSystemMsg (FALSE, "������\�����܂���", RGB (255, 255, 255));
		} else {
			m_pMgrData->SetOptionViewChat (1);
			AddSystemMsg (FALSE, "������\�����܂�", RGB (255, 255, 255));
		}
		m_pMgrData->SaveIniData ();
		break;
	case 3:		/* �A�C�e����\������ */
		if (m_pMgrData->GetOptionViewItem ()) {
			m_pMgrData->SetOptionViewItem (FALSE);
			AddSystemMsg (FALSE, "�A�C�e����\�����܂���", RGB (255, 255, 255));
		} else {
			m_pMgrData->SetOptionViewItem (TRUE);
			AddSystemMsg (FALSE, "�A�C�e����\�����܂�", RGB (255, 255, 255));
		}
		m_pMgrData->SaveIniData ();
		break;
	case 4:		/* �A�C�e������\������ */
		if (m_pMgrData->GetOptionViewItemName ()) {
			m_pMgrData->SetOptionViewItemName (FALSE);
			AddSystemMsg (FALSE, "�A�C�e������\�����܂���", RGB (255, 255, 255));
		} else {
			m_pMgrData->SetOptionViewItemName (TRUE);
			AddSystemMsg (FALSE, "�A�C�e������\�����܂�", RGB (255, 255, 255));
		}
		m_pMgrData->SaveIniData ();
		break;
	case 5:		/* �w���v�A�C�R����\������ */
		if (m_pMgrData->GetOptionViewHelpIcon () != 0) {
			m_pMgrData->SetOptionViewHelpIcon (0);
			AddSystemMsg (FALSE, "�w���v�A�C�R����\�����܂���", RGB (255, 255, 255));
		} else {
			m_pMgrData->SetOptionViewHelpIcon (1);
			AddSystemMsg (FALSE, "�w���v�A�C�R����\�����܂�", RGB (255, 255, 255));
		}
		m_pMgrData->SaveIniData ();
		break;
	case 6:		/* �퓬���b�Z�[�W�����O�Ɏc�� */
		if (m_pMgrData->GetOptionBattleMsgLog () == TRUE) {
			m_pMgrData->SetOptionBattleMsgLog (FALSE);
			AddSystemMsg (FALSE, "�퓬���b�Z�[�W�����O�Ɏc���܂���", RGB (255, 255, 255));
		} else {
			m_pMgrData->SetOptionBattleMsgLog (TRUE);
			AddSystemMsg (FALSE, "�퓬���b�Z�[�W�����O�Ɏc���܂�", RGB (255, 255, 255));
		}
		m_pMgrData->SaveIniData ();
		break;
	case 7:		/* 60�t���[���ŕ\������ */
		if (m_pMgrData->GetOption60Frame () == TRUE) {
			m_pMgrData->SetOption60Frame (FALSE);
			AddSystemMsg (FALSE, "�b��30�t���[���ŕ\�����܂�", RGB (255, 255, 255));
		} else {
			m_pMgrData->SetOption60Frame (TRUE);
			AddSystemMsg (FALSE, "�b��60�t���[���ŕ\�����܂�", RGB (255, 255, 255));
		}
		m_pMgrData->SaveIniData ();
		PostMessage (m_pMgrData->GetMainWindow (), WM_MAINFRAME, MAINFRAMEMSG_RENEWVIEWSET, 0);
		break;
	default:
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgOPTION_INPUTSET						 */
/* ���e		:�I�v�V����-���͐ݒ�											 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgOPTION_INPUTSET(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION_INPUTSET pWnd;
	PCMgrKeyInput pMgrKeyInput;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_INPUTSET)m_pMgrWindow->GetWindow (WINDOWTYPE_OPTION_INPUTSET);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		/* �g�p����W���C�p�b�h�̐ݒ� */
		pMgrKeyInput = m_pMgrData->GetMgrKeyInput ();
		pMgrKeyInput->Enum ();
		m_pMgrWindow->MakeWindowOPTION_INPUTSET_SETDEVICE ();
		break;
	default:
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgOPTION_TASKBAR						 */
/* ���e		:�I�v�V����(�������Ƀ^�X�N�o�[�`�J�`�J)							 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgOPTION_TASKBAR(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION_TASKBAR pWnd;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_TASKBAR)m_pMgrWindow->GetWindow (WINDOWTYPE_OPTION_TASKBAR);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		/* ON */
		m_pMgrData->SetOptionTaskbar (TRUE);
		AddSystemMsg (FALSE, "�������Ƀ^�X�N�o�[��_�ł����܂�", RGB (255, 255, 255));
		m_pMgrData->SaveIniData ();
		break;
	case 1:		/* OFF */
		m_pMgrData->SetOptionTaskbar (FALSE);
		AddSystemMsg (FALSE, "�������ł��ω����܂���", RGB (255, 255, 255));
		m_pMgrData->SaveIniData ();
		break;
	default:
		goto Exit;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgOPTION_INPUTSET_SETDEVICE			 */
/* ���e		:�I�v�V����-���͐ݒ�-���̓f�o�C�X�̐ݒ�							 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgOPTION_INPUTSET_SETDEVICE(DWORD dwPara)
{
	BOOL bRet;
	int nNo;
	GUID stGuid;
	PCWindowOPTION_INPUTSET_SETDEVICE pWnd;
	PCMgrKeyInput pMgrKeyInput;
	CmyString strName, strTmp;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_INPUTSET_SETDEVICE)m_pMgrWindow->GetWindow (WINDOWTYPE_OPTION_INPUTSET_SETDEVICE);
	if (pWnd == NULL) {
		goto Exit;
	}
	pMgrKeyInput = m_pMgrData->GetMgrKeyInput ();
	nNo = (int)dwPara;
	nNo --;
	pMgrKeyInput->SetDevice (nNo, m_pMgrData->GetMainWindow ());

	ZeroMemory (&stGuid, sizeof (stGuid));
	if (nNo >= 0) {
		pMgrKeyInput->GetGUID (nNo, stGuid);
		pMgrKeyInput->GetDeviceName (nNo, strName);

		strTmp.Format ("[%s]���g�p���܂�", (LPCSTR)strName);
	} else {
		strTmp.Format ("�W���C�p�b�h���g�p���܂���");
	}
	m_pMgrData->SetInputGuid (stGuid);
	m_pMgrData->SaveIniData ();
	AddSystemMsg (FALSE, (LPCSTR)strTmp, RGB (255, 255, 255));

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgOPTION_ACTIONSET						 */
/* ���e		:�I�v�V����-����ݒ�											 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgOPTION_ACTIONSET(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION_ACTIONSET pWnd;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_ACTIONSET)m_pMgrWindow->GetWindow (WINDOWTYPE_OPTION_ACTIONSET);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:		/* ���Ђ�˃^�C�}�[�̐ݒ� */
		m_pMgrWindow->MakeWindowOPTION_ACTIONSET_SLEEPTIMER ();
		break;
	default:
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgOPTION_ACTIONSET_SLEEPTIMER			 */
/* ���e		:�I�v�V����-����ݒ�-���Ђ�˃^�C�}�[�̐ݒ�						 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgOPTION_ACTIONSET_SLEEPTIMER(DWORD dwPara)
{
	BOOL bRet;
	PCWindowOPTION_ACTIONSET_SLEEPTIMER pWnd;

	bRet = TRUE;
	pWnd = (PCWindowOPTION_ACTIONSET_SLEEPTIMER)m_pMgrWindow->GetWindow (WINDOWTYPE_OPTION_ACTIONSET_SLEEPTIMER);
	if (pWnd == NULL) {
		goto Exit;
	}

	m_pMgrData->SetSleepTimer (dwPara);
	m_pMgrData->SaveIniData ();

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgCOMMANDMENU							 */
/* ���e		:�R�}���h���j���[												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgCOMMANDMENU(DWORD dwPara)
{
	BOOL bRet;
	PCWindowCOMMANDMENU pWnd;

	bRet = TRUE;
	pWnd = (PCWindowCOMMANDMENU)m_pMgrWindow->GetWindow (WINDOWTYPE_COMMANDMENU);
	if (pWnd == NULL) {
		goto Exit;
	}

	switch (dwPara) {
	case 0:	/* �L�����N�^�[(C) */
		m_pMgrWindow->MakeWindowCHAR_STATUS ();
		break;
//	case 1:	/* �X�L��(S) */
	case 2:	/* �o�b�O(B) */
		m_pMgrWindow->MakeWindowITEMMENU ();
		break;
//	case 3:	/* ����(I) */
	case 4:	/* �V�X�e��(ESC) */
		m_pMgrWindow->MakeWindowSYSTEMMENU ();
		break;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnWindowMsgSWOON								 */
/* ���e		:�C�⃁�j���[													 */
/* ���t		:2008/12/02														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnWindowMsgSWOON(DWORD dwPara)
{
	CPacketCHAR_PARA1 PacketCHAR_PARA1;

	switch (dwPara) {
	case 0:		/* ���̏�ŏ�����҂� */
		break;
	case 1:		/* �L�^�����ꏊ�ŕ������� */
		PacketCHAR_PARA1.Make (SBOCOMMANDID_SUB_CHAR_REQ_RECOVERY, m_pPlayerChar->m_dwCharID, 0);
		m_pSock->Send (&PacketCHAR_PARA1);
		break;
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnMainFrameRENEWITEMINFO						 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)[�A�C�e�����X�V]				 */
/* ���t		:2007/09/08														 */
/* ========================================================================= */

void CStateProcMAP::OnMainFrameRENEWITEMINFO(DWORD dwItemID)
{
	BOOL bResult, bDelete;
	HWND hWndAdmin;
	PCInfoItem pInfoItem;
	PCInfoCharBase pInfoChar;

	if (dwItemID == 0) {
		return;
	}
	if (m_pPlayerChar == NULL) {
		return;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}
	hWndAdmin = m_pMgrData->GetAdminWindow ();
	if (hWndAdmin == NULL) {
		bDelete = FALSE;
		pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtr (pInfoItem->m_dwCharID);
		if (pInfoChar == NULL) {
			if (pInfoItem->m_dwMapID == 0) {
				bDelete = TRUE;
			}
		}
		if (bDelete) {
			m_pLibInfoItem->Delete (dwItemID);
			return;
		}
	}
	if (pInfoItem->m_dwDropSoundID == 0) {
		return;
	}
	bResult = m_pPlayerChar->IsViewArea (pInfoItem->m_dwMapID, &pInfoItem->m_ptPos);
	if (bResult == FALSE) {
		return;
	}

	/* �ߋ����ɂ���̂Ō��ʉ��Đ� */
	m_pMgrSound->PlaySound (pInfoItem->m_dwDropSoundID);
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnMainFrameRENEWTALKEVENT						 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)[��b�C�x���g���X�V]			 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CStateProcMAP::OnMainFrameRENEWTALKEVENT(DWORD dwParam)
{
	LPCSTR pszName;
	PCInfoTalkEvent pInfo;
	PCInfoCharBase pInfoChar;

	if (dwParam != 0) {
		return;
	}
	pInfo = m_pMgrData->GetInfoTalkEvent ();
	if (pInfo->GetPageCount () <= 0) {
		return;
	}
	pszName	  = NULL;
	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtr (pInfo->m_dwTalkEventID);
	if (pInfoChar) {
		pszName = (LPCSTR)pInfoChar->m_strCharName;
	}

	m_pMgrWindow->MakeWindowTEXTMSG (NULL, pszName, pInfo);
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::OnXChar											 */
/* ���e		:�L������X�L�[�����������̏���									 */
/* �߂�l	:TRUE:��������													 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

BOOL CStateProcMAP::OnXChar(DWORD dwCharID)
{
	int nLen;
	BOOL bRet, bResult;
	PCInfoCharBase pInfoChar;
	CPacketCHAR_REQ_MODIFY_PARAM Packet;
	CPacketCHAR_REQ_PUSH PacketREQ_PUSH;
	CmyString strTmp;

	bRet = FALSE;

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtr (dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}
	bResult = pInfoChar->IsNPC ();
	if (bResult == FALSE) {
		goto Exit;
	}

	switch (pInfoChar->m_nMoveType) {
	case CHARMOVETYPE_SCORE:			/* ���_ */
		Packet.Make (dwCharID, PARAMID_CHAR_REQ_MODIFY_ANIME, 0);
		m_pSock->Send (&Packet);
		break;
	case CHARMOVETYPE_STYLECOPY_PUT:	/* �e�p�R�s�[(��荞��) */
		Packet.Make (dwCharID, PARAMID_CHAR_REQ_MODIFY_STYLECOPY_PUT, m_pPlayerChar->m_dwCharID);
		m_pSock->Send (&Packet);
		break;
	case CHARMOVETYPE_STYLECOPY_GET:	/* �e�p�R�s�[(���f) */
		Packet.Make (m_pPlayerChar->m_dwCharID, PARAMID_CHAR_REQ_MODIFY_STYLECOPY_GET, dwCharID);
		m_pSock->Send (&Packet);
		break;
	case CHARMOVETYPE_BALL:				/* �{�[�� */
		PacketREQ_PUSH.Make (dwCharID, m_pPlayerChar->m_nDirection, 2);
		m_pSock->Send (&PacketREQ_PUSH);
		break;
	default:
		TrimViewString (strTmp, (LPCSTR)pInfoChar->m_strTalk);
		nLen = strTmp.GetLength ();
		if (nLen == 1) {
			if (strcmp ((LPCSTR)strTmp, "@") == 0) {
				CPacketCHAR_PARA1 Packet;

				Packet.Make (SBOCOMMANDID_SUB_CHAR_REQ_TALKEVENT, dwCharID, 0);
				m_pSock->Send (&Packet);
				m_pPlayerChar->m_bWaitCheckMapEvent = TRUE;
				break;
			}
		}
		if (nLen > 0) {
			m_pMgrWindow->MakeWindowTEXTMSG (NULL, (LPCSTR)pInfoChar->m_strCharName, (LPCSTR)strTmp);
			break;
		}
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::AddSystemMsg									 */
/* ���e		:�V�X�e�����b�Z�[�W��ǉ�										 */
/* ���t		:2007/09/16														 */
/* ========================================================================= */

void CStateProcMAP::AddSystemMsg(
	BOOL bAddLog,		/* [in] TRUE:���O�ɒǉ� */
	LPCSTR pszMsg,		/* [in] ���b�Z�[�W */
	COLORREF cl)		/* [in] �\���F */
{
	m_pMgrData->AddSystemMsg (bAddLog, pszMsg, cl);
	PostMessage (m_pMgrData->GetMainWindow (), WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
}


/* ========================================================================= */
/* �֐���	:CStateProcMAP::DefenseOff										 */
/* ���e		:�h�����														 */
/* ���t		:2008/07/07														 */
/* ========================================================================= */

void CStateProcMAP::DefenseOff(void)
{
	BOOL bResult;
	CPacketCHAR_STATE Packet;

	if (m_pPlayerChar == NULL) {
		return;
	}
	if (m_pPlayerChar->m_nMoveState != CHARMOVESTATE_BATTLE_DEFENSE) {
		return;
	}
	bResult = m_pPlayerChar->IsChgWait ();
	if (bResult) {
		return;
	}
	Packet.Make (m_pPlayerChar->m_dwCharID, CHARMOVESTATE_BATTLE);
	m_pSock->Send (&Packet);
	m_pPlayerChar->SetChgWait (TRUE);
}

/* Copyright(C)URARA-works 2006 */
