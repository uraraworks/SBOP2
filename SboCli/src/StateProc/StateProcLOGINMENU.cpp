/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:StateProcLOGINMENU.cpp										 */
/* ���e			:��ԏ����N���X(���O�C�����) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "PacketACCOUNT_REQ_MAKECHAR.h"
#include "PacketACCOUNT_RES_MAKECHAR.h"
#include "PacketCHAR_REQ_CHARINFO.h"
#include "PacketCONNECT_REQ_PLAY.h"
#include "InfoAccount.h"
#include "LibInfoCharCli.h"
#include "WindowSTATUS.h"
#include "WindowLOGINMENU.h"
#include "WindowCHARNAME.h"
#include "WindowSEX.h"
#include "WindowSTYLESELECT.h"
#include "WindowNAMEINPUT.h"
#include "WindowFAMILYTYPE.h"
#include "LayerCharSelect.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "StateProcLOGINMENU.h"

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

enum {
	STATE_MENU = 0,										/* ���j���[�I�� */
	STATE_SELECTPLAYCHAR,								/* �L�����I�� */
	STATE_SELECTDELETECHAR,								/* �폜�L�����I�� */
	STATE_BACK,											/* �߂� */
};


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::CStateProcLOGINMENU						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CStateProcLOGINMENU::CStateProcLOGINMENU()
{
	m_dwLastTimeKey		= 0;
	m_nCountKeyRepeate	= 0;
	m_nSelect			= 0;
	m_nState			= STATE_MENU;

	m_pInfoAccount			= NULL;
	m_pWindowLOGINMENU		= NULL;
	m_pWindowSTATUS			= NULL;
	m_pWindowSTYLESELECT	= NULL;
	m_pWindowNAMEINPUT		= NULL;

	m_pInfoCharCli			= new CInfoCharCli;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::~CStateProcLOGINMENU						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CStateProcLOGINMENU::~CStateProcLOGINMENU()
{
	SAFE_DELETE (m_pInfoCharCli);
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::Init										 */
/* ���e		:������															 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CStateProcLOGINMENU::Init(void)
{
	m_pInfoCharCli->Create (m_pMgrData);

	m_pMgrWindow->MakeWindowLOGINMENU ();
//	m_pMgrWindow->MakeWindowACCOUNTINFO ();
//	m_pMgrWindow->MakeWindowSTATUS ();

	m_pWindowLOGINMENU	= (PCWindowLOGINMENU)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGINMENU);
//	m_pWindowSTATUS		= (PCWindowSTATUS)m_pMgrWindow->GetWindow (WINDOWTYPE_STATUS);
	m_pMgrLayer->MakeLOGINMENU ();
	m_pMgrLayer->MakeCHARSELECT (m_pMgrData->GetAccountID ());

	PostMessage (m_hWndMain, WM_MAINFRAME, MAINFRAMEMSG_RENEWACCOUNTINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnWindowMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_WINDOWMSG)								 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnWindowMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case WINDOWTYPE_LOGINMENU:		OnWindowMsgLOGINMENU (dwPara);		break;	/* ���j���[ */
	case WINDOWTYPE_FAMILYTYPE:		OnWindowMsgFAMILYTYPE (dwPara);		break;	/* �푰�I�� */
	case WINDOWTYPE_STYLESELECT:	OnWindowMsgSTYLESELECT (dwPara);	break;	/* �e�p�I�� */
	case WINDOWTYPE_NAMEINPUT:		OnWindowMsgNAMEINPUT (dwPara);		break;	/* ���O���� */

	case WINDOWTYPE_CHARNAME:		/* �L���������� */
		m_pWindowNAMEINPUT->OnWindowMsg (nType, dwPara);
		break;
	case WINDOWTYPE_SEX:			/* ���� */
	case WINDOWTYPE_HAIRTYPE:		/* ���^�I�� */
	case WINDOWTYPE_HAIRCOLOR:		/* ���F�I�� */
	case WINDOWTYPE_EYECOLOR:		/* �ڐF�I�� */
		m_pWindowSTYLESELECT->OnWindowMsg (nType, dwPara);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnMainFrame								 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RES_MAKECHAR:			/* �L�����쐬���� */
		switch (dwParam) {
		case MAKECHARRES_NG_USE:		/* �g�p�ς� */
			m_pMgrWindow->MakeWindowMSG ("���̖��O�͎g�p�ς݂ł�", 3000, 4);
			break;
		case MAKECHARRES_NG_SPACE:		/* �󔒂��g�p����Ă��� */
			m_pMgrWindow->MakeWindowMSG ("�L�������ɋ󔒂͎g���܂���", 3000, 4);
			break;
		default:
			m_pMgrWindow->MakeWindowMSG ("�L�����N�^�[���쐬���܂���", 3000, 4);
			m_pMgrWindow->Delete (WINDOWTYPE_NAMEINPUT);
			m_pWindowLOGINMENU->SetPos (0);
			break;
		}
		m_pMgrWindow->Update ();
		break;
	case MAINFRAMEMSG_RENEWACCOUNTINFO:		/* �A�J�E���g���X�V */
		{
			int i, nCount;
			CPacketCHAR_REQ_CHARINFO Packet;

			m_pMgrWindow->Update ();
			m_pInfoAccount = m_pMgrData->GetAccount ();
			nCount = m_pInfoAccount->m_adwCharID.GetSize ();
			for (i = 0; i < nCount; i ++) {
				Packet.Make (m_pInfoAccount->m_adwCharID[i]);
				m_pSock->Send (&Packet);
			}
		}
		break;
	case MAINFRAMEMSG_RENEWCHARINFO:		/* �L�������X�V */
		m_pMgrWindow->Update ();
		break;
	default:
		CStateProcBase::OnMainFrame (dwCommand, dwParam);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnLeft									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2005/07/09														 */
/* ========================================================================= */

BOOL CStateProcLOGINMENU::OnLeft(BOOL bDown)
{
	BOOL bRet;
	DWORD dwTime, dwTimeTmp;
	PCLayerCharSelect pLayer;

	bRet	= FALSE;
	dwTime	= 250;

	if (bDown == FALSE) {
		m_dwLastTimeKey = 0;
		goto Exit;
	}

	if (m_nCountKeyRepeate > 1) {
		dwTime = 125;
	}
	dwTimeTmp = timeGetTime () - m_dwLastTimeKey;
	if (dwTimeTmp < dwTime) {
		goto Exit;
	}

	if (m_nSelect <= 0) {
		goto Exit;
	}

	m_nSelect --;
	m_dwLastTimeKey = timeGetTime ();
	pLayer = (PCLayerCharSelect)m_pMgrLayer->Get (LAYERTYPE_CHARSELECT);
	pLayer->SetSelect (m_nSelect);

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnRight									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2005/07/09														 */
/* ========================================================================= */

BOOL CStateProcLOGINMENU::OnRight(BOOL bDown)
{
	BOOL bRet;
	DWORD dwTime, dwTimeTmp;
	PCLayerCharSelect pLayer;

	bRet	= FALSE;
	dwTime	= 250;

	if (bDown == FALSE) {
		m_dwLastTimeKey = 0;
		goto Exit;
	}

	if (m_nCountKeyRepeate > 1) {
		dwTime = 125;
	}
	dwTimeTmp = timeGetTime () - m_dwLastTimeKey;
	if (dwTimeTmp < dwTime) {
		goto Exit;
	}

	if (m_nSelect >= m_pInfoAccount->GetCharCount () - 1) {
		goto Exit;
	}

	m_nSelect ++;
	m_dwLastTimeKey = timeGetTime ();
	pLayer = (PCLayerCharSelect)m_pMgrLayer->Get (LAYERTYPE_CHARSELECT);
	pLayer->SetSelect (m_nSelect);

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnX										 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2005/06/26														 */
/* ========================================================================= */

BOOL CStateProcLOGINMENU::OnX(BOOL bDown)
{
	PCLayerCharSelect pLayer;

	if (bDown) {
		return FALSE;
	}

	pLayer = (PCLayerCharSelect)m_pMgrLayer->Get (LAYERTYPE_CHARSELECT);
	m_pMgrData->SetCharID (pLayer->GetSelectCharID ());

	m_pMgrDraw->SetFadeState (FADESTATE_FADEOUT);

	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnZ										 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2005/06/26														 */
/* ========================================================================= */

BOOL CStateProcLOGINMENU::OnZ(BOOL bDown)
{
	PCLayerCharSelect pLayer;

	m_pWindowLOGINMENU->SetActive (TRUE);
	m_pWindowLOGINMENU->SetInput (TRUE);

	m_nSelect	= 0;
	m_nState	= STATE_MENU;

	pLayer = (PCLayerCharSelect)m_pMgrLayer->Get (LAYERTYPE_CHARSELECT);
	pLayer->SetSelect (-1);
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnMgrDrawEND_FADEOUT						 */
/* ���e		:�t�F�[�h�A�E�g����												 */
/* ���t		:2007/02/27														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnMgrDrawEND_FADEOUT(DWORD dwPara)
{
	PCLibInfoCharCli pLibInfoCharCli;
	CPacketCHAR_REQ_CHARINFO PacketCHAR_REQ_CHARINFO;
	CPacketCONNECT_REQ_PLAY PacketCONNECT_REQ_PLAY;

	switch (m_nState) {
	case STATE_SELECTPLAYCHAR:	/* �L�����I�� */
		pLibInfoCharCli = m_pMgrData->GetLibInfoChar ();
		pLibInfoCharCli->DeleteAll ();

		PacketCHAR_REQ_CHARINFO.Make (m_pMgrData->GetCharID ());
		m_pSock->Send (&PacketCHAR_REQ_CHARINFO);
		PacketCONNECT_REQ_PLAY.Make (m_pMgrData->GetAccountID (), m_pMgrData->GetCharID ());
		m_pSock->Send (&PacketCONNECT_REQ_PLAY);
		break;

	case STATE_BACK:			/* �߂� */
		m_pSock->Destroy ();
		PostMessage (m_hWndMain, WM_MAINFRAME, MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGIN);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnWindowMsgLOGINMENU						 */
/* ���e		:���j���[�E�B���h�E����̒ʒm									 */
/* ���t		:2007/04/24														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnWindowMsgLOGINMENU(DWORD dwPara)
{
	PCLayerCharSelect pLayer;

	switch (dwPara) {
	case 0:		/* �L�����I�� */
		m_nState = STATE_SELECTPLAYCHAR;
		m_pWindowLOGINMENU->SetActive (FALSE);
		m_pWindowLOGINMENU->SetInput (FALSE);
		pLayer = (PCLayerCharSelect)m_pMgrLayer->Get (LAYERTYPE_CHARSELECT);
		pLayer->SetSelect (0);
		break;

	case 1:		/* �V�K�쐬 */
		m_pMgrWindow->MakeWindowFAMILYTYPE ();
		break;

	case 2:		/* �L�����폜 */
		break;

	case 3:		/* �߂� */
		m_nState = STATE_BACK;
		m_pMgrDraw->SetFadeState (FADESTATE_FADEOUT);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnWindowMsgFAMILYTYPE						 */
/* ���e		:�푰�I���E�B���h�E����̒ʒm									 */
/* ���t		:2007/04/24														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnWindowMsgFAMILYTYPE(DWORD dwPara)
{
	PCWindowFAMILYTYPE pWindow;

	switch (dwPara) {
	case 0:
		m_pMgrWindow->MakeWindowSTYLESELECT ();
		m_pWindowSTYLESELECT = (PCWindowSTYLESELECT)m_pMgrWindow->GetWindow (WINDOWTYPE_STYLESELECT);

		pWindow = (PCWindowFAMILYTYPE)m_pMgrWindow->GetWindow (WINDOWTYPE_FAMILYTYPE);
		if (pWindow) {
			/* �푰�I����ʂ��痈�� */
			m_pInfoCharCli->m_wFamilyID = pWindow->GetType ();
			m_pMgrWindow->Delete (WINDOWTYPE_FAMILYTYPE);
			m_pWindowSTYLESELECT->SetFamilyType (m_pInfoCharCli->m_wFamilyID);

		} else {
			/* �e�p�I����ʂ���߂��Ă��� */
			m_pWindowSTYLESELECT->SetInfoChar (m_pInfoCharCli);
		}
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnWindowMsgSTYLESELECT					 */
/* ���e		:�e�p�I���E�B���h�E����̒ʒm									 */
/* ���t		:2007/04/24														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnWindowMsgSTYLESELECT(DWORD dwPara)
{
	switch (dwPara) {
	case 0:
		m_pInfoCharCli->Copy (m_pWindowSTYLESELECT->m_pInfoCharCli);
		m_pMgrWindow->Delete (WINDOWTYPE_STYLESELECT);
		m_pMgrWindow->MakeWindowNAMEINPUT ();
		m_pWindowNAMEINPUT = (PCWindowNAMEINPUT)m_pMgrWindow->GetWindow (WINDOWTYPE_NAMEINPUT);
		m_pWindowNAMEINPUT->m_pInfoCharCli->Copy (m_pInfoCharCli);
		m_pWindowNAMEINPUT->m_pInfoCharCli->MakeCharGrp ();
		break;

	case -1:
		/* �L�����Z�����ꂽ�̂őO�̉�ʂɖ߂� */
		m_pMgrWindow->Delete (WINDOWTYPE_STYLESELECT);
		PostMessage (m_hWndMain, WM_WINDOWMSG, WINDOWTYPE_LOGINMENU, 1);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGINMENU::OnWindowMsgNAMEINPUT						 */
/* ���e		:���O���̓E�B���h�E����̒ʒm									 */
/* ���t		:2007/04/24														 */
/* ========================================================================= */

void CStateProcLOGINMENU::OnWindowMsgNAMEINPUT(DWORD dwPara)
{
	CPacketACCOUNT_REQ_MAKECHAR Packet;

	switch (dwPara) {
	case 0:
		m_pInfoCharCli->Copy (m_pWindowNAMEINPUT->m_pInfoCharCli);
		Packet.Make (m_pMgrData->GetAccountID (), m_pInfoCharCli);
		m_pSock->Send (&Packet);
		break;

	case -1:
		/* �L�����Z�����ꂽ�̂őO�̉�ʂɖ߂� */
		m_pMgrWindow->Delete (WINDOWTYPE_NAMEINPUT);
		PostMessage (m_hWndMain, WM_WINDOWMSG, WINDOWTYPE_FAMILYTYPE, 0);
		break;
	}
}

/* Copyright(C)URARA-works 2006 */
