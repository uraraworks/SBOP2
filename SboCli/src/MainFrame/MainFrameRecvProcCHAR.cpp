/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcCHAR.cpp									 */
/* ���e			:�N���C�A���g���C���t���[��(�L�����n��M����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/31													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "InfoAccount.h"
#include "InfoMapBase.h"
#include "LibInfoMotion.h"
#include "LibInfoMotionType.h"
#include "LibInfoCharCli.h"
#include "LibInfoItem.h"
#include "InfoCharCli.h"
#include "LayerMap.h"
#include "StateProcBase.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrWindow.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR										 */
/* ���e		:��M����(�L�����n)												 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_CHAR_RES_CHARINFO:		RecvProcCHAR_RES_CHARINFO		(pData);	break;	/* �L������񉞓� */
	case SBOCOMMANDID_SUB_CHAR_CHARINFO:			RecvProcCHAR_CHARINFO			(pData);	break;	/* �L�������ʒm */
	case SBOCOMMANDID_SUB_CHAR_MOVEPOS:				RecvProcCHAR_MOVEPOS			(pData);	break;	/* �ړ��ʒm */
	case SBOCOMMANDID_SUB_CHAR_STATE:				RecvProcCHAR_STATE				(pData);	break;	/* ��Ԓʒm */
	case SBOCOMMANDID_SUB_CHAR_PROCSTATE:			RecvProcCHAR_PROCSTATE			(pData);	break;	/* �s����Ԓʒm */
	case SBOCOMMANDID_SUB_CHAR_CHAT:				RecvProcCHAR_CHAT				(pData);	break;	/* �`���b�g�ʒm */
	case SBOCOMMANDID_SUB_CHAR_CHARID:				RecvProcCHAR_CHARID				(pData);	break;	/* �L����ID�ʒm */
	case SBOCOMMANDID_SUB_CHAR_RES_PUTGET:			RecvProcCHAR_RES_PUTGET			(pData);	break;	/* �A�C�e�����E���u������ */
	case SBOCOMMANDID_SUB_CHAR_SET_EFFECT:			RecvProcCHAR_SET_EFFECT			(pData);	break;	/* �G�t�F�N�g���w�� */
	case SBOCOMMANDID_SUB_CHAR_ITEMINFO:			RecvProcCHAR_ITEMINFO			(pData);	break;	/* �A�C�e�����ʒm */
	case SBOCOMMANDID_SUB_CHAR_RES_TAIL:			RecvProcCHAR_RES_TAIL			(pData);	break;	/* �t���čs������ */
	case SBOCOMMANDID_SUB_CHAR_MODIFY_PARAM:		RecvProcCHAR_MODIFY_PARAM		(pData);	break;	/* �p�����[�^�ύX�ʒm */
	case SBOCOMMANDID_SUB_CHAR_GRP:					RecvProcCHAR_GRP				(pData);	break;	/* �摜�ʒm */
	case SBOCOMMANDID_SUB_CHAR_MOTION:				RecvProcCHAR_MOTION				(pData);	break;	/* ���[�V�������ʒm */
	case SBOCOMMANDID_SUB_CHAR_MOTIONTYPE:			RecvProcCHAR_MOTIONTYPE			(pData);	break;	/* ���[�V������ʏ��ʒm */
	case SBOCOMMANDID_SUB_CHAR_SET_EFCBALLOON:		RecvProcCHAR_SET_EFCBALLOON		(pData);	break;	/* ���o�����w�� */
	case SBOCOMMANDID_SUB_CHAR_SET_MOTION:			RecvProcCHAR_SET_MOTION			(pData);	break;	/* ���[�V�������w�� */
	case SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT:	RecvProcCHAR_RES_CHECKMAPEVENT	(pData);	break;	/* �}�b�v�C�x���g�`�F�b�N���� */
	case SBOCOMMANDID_SUB_CHAR_STATUS:				RecvProcCHAR_STATUS				(pData);	break;	/* �X�e�[�^�X���ʒm */
	case SBOCOMMANDID_SUB_CHAR_TEXTEFFECT:			RecvProcCHAR_TEXTEFFECT			(pData);	break;	/* �����G�t�F�N�g�ʒm */
	case SBOCOMMANDID_SUB_CHAR_STATE_CHARGE:		RecvProcCHAR_STATE_CHARGE		(pData);	break;	/* ���ߏ�Ԓʒm */
	case SBOCOMMANDID_SUB_CHAR_RES_TALKEVENT:		RecvProcCHAR_RES_TALKEVENT		(pData);	break;	/* ��b�C�x���g��񉞓� */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_RES_CHARINFO							 */
/* ���e		:��M����(�L������񉞓�)										 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_RES_CHARINFO(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	PCInfoAccount pInfoAccount;
	PCLayerMap pLayerMap;
	CPacketCHAR_RES_CHARINFO Packet;

	pInfoChar = NULL;
	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_pInfo->m_dwCharID);
	if (pInfoChar == NULL) {
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetNew (Packet.m_pInfo->m_nMoveType);
		pInfoChar->Copy (Packet.m_pInfo);
		pInfoChar->SetViewState (INFOCHARCLI_VIEWSTATE_FADEIN);
		m_pLibInfoChar->Add (pInfoChar);
	} else {
		/* �ړ���ʂ��ς�����H */
		if (Packet.m_pInfo->m_nMoveType != pInfoChar->m_nMoveType) {
			pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetNew (Packet.m_pInfo->m_nMoveType);
			m_pLibInfoChar->SetPtr (Packet.m_pInfo->m_dwCharID, pInfoChar);
		}
	}
	if (pInfoChar->m_nMoveState == CHARMOVESTATE_DELETEREADY) {
		return;
	}

	pInfoChar->Copy (Packet.m_pInfo);
	pInfoChar->MakeCharGrp ();
	pInfoChar->m_bRedraw = TRUE;
	m_pLibInfoChar->RenewMotionInfo (pInfoChar);

	pInfoAccount = m_pMgrData->GetAccount ();
	if (pInfoAccount->m_dwCharID == Packet.m_pInfo->m_dwCharID) {
		m_pMgrData->SetPlayerChar (pInfoChar);
	}

	/* ���L�����H */
	if (m_pMgrData->GetCharID () == pInfoChar->m_dwCharID) {
		if (Packet.m_bChgScreenPos) {
			pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
			if (pLayerMap) {
				pLayerMap->SetCenterPos (pInfoChar->m_nMapX, pInfoChar->m_nMapY);
			}
			RenewItemArea ();
		}
	}

	if (pInfoChar) {
		PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARINFO, pInfoChar->m_dwCharID);
	}
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount ());
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_CHARINFO								 */
/* ���e		:��M����(�L�������ʒm)										 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_CHARINFO(PBYTE pData)
{
	int i, nCount;
	PCInfoCharCli pInfoChar, pInfoCharTmp;
	CPacketCHAR_CHARINFO Packet;

	pInfoChar = NULL;
	Packet.Set (pData);

	nCount = Packet.m_pLibInfo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharCli)Packet.m_pLibInfo->GetPtr (i);

		/* ���L�����H */
		if (m_pMgrData->GetCharID () == pInfoCharTmp->m_dwCharID) {
			continue;
		}
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (pInfoCharTmp->m_dwCharID);
		if (pInfoChar == NULL) {
			pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetNew (pInfoCharTmp->m_nMoveType);
			pInfoChar->Copy (pInfoCharTmp);
			m_pLibInfoChar->Add (pInfoChar);
		} else {
			if (pInfoChar->m_nMoveState == CHARMOVESTATE_DELETEREADY) {
				continue;
			}
			pInfoChar->Copy (pInfoCharTmp);
		}
		pInfoChar->MakeCharGrp ();
		m_pLibInfoChar->RenewMotionInfo (pInfoChar);
	}
	/* �d�Ȃ蒲�� */
	m_pLibInfoChar->SortY ();

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount ());
	if (nCount == 1) {
		if (pInfoChar) {
			PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARINFO, pInfoChar->m_dwCharID);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_MOVEPOS								 */
/* ���e		:��M����(�ړ��ʒm)												 */
/* ���t		:2007/01/14														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_MOVEPOS(PBYTE pData)
{
	int nState, nStateStand, nStateMove, nResult;
	BOOL bResult;
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	PCLayerMap pLayerMap;
	POINT ptPos;
	CPacketCHAR_MOVEPOS Packet;

	Packet.Set (pData);
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);

	/* ���L�����H */
	if (m_pMgrData->GetCharID () == Packet.m_dwCharID) {
		m_pStateProc->KeyProc (0, FALSE);
		if (Packet.m_bUpdate == FALSE) {
			m_pLibInfoChar->SortY ();
			return;
		}
		if (pLayerMap) {
			nResult = pLayerMap->IsScrollPos (Packet.m_pos.x, Packet.m_pos.y, Packet.m_nDirection);
			if (nResult >= 0) {
				pLayerMap->Scroll (nResult, TRUE);
			}
			/* ��ʊO�ɏo��H */
			bResult = pLayerMap->IsInScreen (Packet.m_pos.x, Packet.m_pos.y);
			if (bResult == FALSE) {
				pLayerMap->SetScrollMode (TRUE, 0);
				pLayerMap->SetCenterPos (Packet.m_pos.x, Packet.m_pos.y);
			}

			RenewItemArea ();
		}
	}
	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		CPacketCHAR_REQ_CHARINFO PacketCHAR_REQ_CHARINFO;

		/* ��ʓ��H */
		if (pLayerMap) {
			bResult = pLayerMap->IsInScreen (Packet.m_pos.x, Packet.m_pos.y);
			if (bResult) {
				PacketCHAR_REQ_CHARINFO.Make (Packet.m_dwCharID);
				m_pSock->Send (&PacketCHAR_REQ_CHARINFO);
			}
		}
		return;
	}
	nState = -1;

	nStateStand	= CHARMOVESTATE_STAND;
	nStateMove	= CHARMOVESTATE_MOVE;
	if (pInfoChar->IsStateBattle ()) {
		nStateStand	= CHARMOVESTATE_BATTLE;
		if (pInfoChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			nStateStand	= CHARMOVESTATE_BATTLE_DEFENSE;
		}
		nStateMove	= CHARMOVESTATE_BATTLEMOVE;
	}

	if (pInfoChar->m_nDirection != Packet.m_nDirection) {
		if (pInfoChar->m_nMoveState != CHARMOVESTATE_SIT) {
			nState = nStateStand;
		}
	}

	if (!((pInfoChar->m_nMapX == Packet.m_pos.x) && (pInfoChar->m_nMapY == Packet.m_pos.y))) {
		nState = nStateMove;
	}

	ptPos.x = Packet.m_pos.x;
	ptPos.y = Packet.m_pos.y;
	bResult = TRUE;
	if (pInfoCharPlayer) {
		bResult = pInfoCharPlayer->IsViewArea (pInfoCharPlayer->m_dwMapID, &ptPos);
	}
	if (bResult == FALSE) {
		nState = CHARMOVESTATE_DELETEREADY;

	} else {
		bResult = pInfoChar->IsStateMove ();
		if (bResult) {
			pInfoChar->AddMovePosQue (nState, Packet.m_nDirection, Packet.m_pos.x, Packet.m_pos.y);
			nState = -1;
		} else {
			pInfoChar->SetPos (Packet.m_pos.x, Packet.m_pos.y);
			pInfoChar->SetDirection (Packet.m_nDirection);
			pInfoChar->m_bRedraw = TRUE;
		}
	}

	if (nState != -1) {
		pInfoChar->ChgMoveState (nState);
	}
	m_bRenewCharInfo = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_STATE									 */
/* ���e		:��M����(��Ԓʒm)												 */
/* ���t		:2007/01/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_STATE(PBYTE pData)
{
	BOOL bChgBGM;
	int nState;
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_STATE Packet;

	Packet.Set (pData);

	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}
	nState = Packet.m_nState;
	if (nState == CHARMOVESTATE_DELETE) {
		nState = CHARMOVESTATE_DELETEREADY;
	}

	if (nState == CHARMOVESTATE_BATTLEATACK) {
		m_pLibInfoChar->RenewMotionInfo (pInfoChar);
	}
	pInfoChar->ChgMoveState (nState);
	pInfoChar->SetChgWait (FALSE);

	if (pInfoChar == pInfoCharPlayer) {
		bChgBGM = FALSE;
		pInfoMap = m_pMgrData->GetMap ();
		if (pInfoMap && (pInfoMap->m_bRecovery == FALSE)) {
			/* �C�₵���H */
			if (pInfoChar->m_nMoveState == CHARMOVESTATE_SWOON) {
				bChgBGM = TRUE;
				m_pMgrWindow->MakeWindowSWOON ();
			} else {
				m_pMgrWindow->Delete (WINDOWTYPE_SWOON);
			}
		}
		ChgMoveState (bChgBGM);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_PROCSTATE								 */
/* ���e		:��M����(�s����Ԓʒm)											 */
/* ���t		:2008/06/11														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_PROCSTATE(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}
	pInfoChar->SetProcState (Packet.m_dwPara);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_CHAT									 */
/* ���e		:��M����(�`���b�g�ʒm)											 */
/* ���t		:2007/02/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_CHAT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_CHAT Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}
	pInfoChar->SetSpeak (Packet.m_strChat);
	m_pMgrSound->PlaySound (SOUNDID_CHAT);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARCHAT, Packet.m_dwCharID);
	FlashMainWindow ();
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_CHARID								 */
/* ���e		:��M����(�L����ID�ʒm)											 */
/* ���t		:2007/02/12														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_CHARID(PBYTE pData)
{
	int i, nCount;
	PCInfoCharCli pInfoChar;
	CPacketCHAR_CHARID Packet;
	CPacketCHAR_REQ_CHARINFO2 PacketCHAR_CHARINFO2;

	Packet.Set (pData);

	/* �m���Ă���L�������폜���� */
	nCount = Packet.m_adwCharID.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_adwCharID[i]);
		/* �m��Ȃ��L�����H */
		if (pInfoChar == NULL) {
			continue;
		}
		Packet.m_adwCharID.RemoveAt (i);
	}
	if (Packet.m_adwCharID.GetSize () <= 0) {
		return;
	}

	/* �c�����L�����̏���v�� */
	PacketCHAR_CHARINFO2.Make (&Packet.m_adwCharID);
	m_pSock->Send (&PacketCHAR_CHARINFO2);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_RES_PUTGET							 */
/* ���e		:��M����(�A�C�e�����E���u������)								 */
/* ���t		:2007/09/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_RES_PUTGET(PBYTE pData)
{
	PCInfoItem pInfoItem;
	CPacketCHAR_RES_PUTGET Packet;
	CmyString strTmp;

	Packet.Set (pData);

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (Packet.m_dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	switch (Packet.m_nResult) {
	case RESULTID_CHAR_RES_PUTGET_PUT:		/* �u���� */
		strTmp.Format ("%s��u���܂���", (LPCSTR)pInfoItem->m_strName);
		break;
	case RESULTID_CHAR_RES_PUTGET_GET:		/* �擾���� */
		strTmp.Format ("%s���E���܂���", (LPCSTR)pInfoItem->m_strName);
		break;
	case RESULTID_CHAR_RES_PUTGET_DELETE:	/* �폜���� */
		strTmp.Format ("%s�͏�������܂���", (LPCSTR)pInfoItem->m_strName);
		break;
	}

	if (strTmp.IsEmpty ()) {
		return;
	}
	m_pMgrData->AddSystemMsg (FALSE, strTmp, RGB (255, 255, 255));
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_SET_EFFECT							 */
/* ���e		:��M����(�G�t�F�N�g���w��)										 */
/* ���t		:2007/07/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_SET_EFFECT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_SET_EFFECT Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->SetEffectID (Packet.m_dwEffectID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_ITEMINFO								 */
/* ���e		:��M����(�A�C�e�����ʒm)										 */
/* ���t		:2007/08/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_ITEMINFO(PBYTE pData)
{
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_ITEMINFO Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->SetItem (&Packet.m_adwItemID);

	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();
	if (pInfoChar == pInfoCharPlayer) {
		PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARINFO, pInfoChar->m_dwCharID);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_RES_TAIL								 */
/* ���e		:��M����(�t���čs������)										 */
/* ���t		:2007/09/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_RES_TAIL(PBYTE pData)
{
	PCInfoCharCli pInfoChar, pInfoCharPlayer, pInfoCharTarget, pInfoCharPara;
	CPacketCHAR_RES_TAIL Packet;
	CmyString strTmp, strNamePara;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}
	pInfoCharTarget = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwTargetCharID);
	pInfoCharPara	= (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwPara);
	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();
	if (pInfoChar == pInfoCharPlayer) {
		switch (Packet.m_nResult) {
		case RES_TAIL_RESULT_OK:			/* ���Ă��� */
			if (pInfoCharTarget) {
				strTmp.Format ("%s�ɂ��Ă����܂�", (LPCSTR)pInfoCharTarget->m_strCharName);
			}
			pInfoCharPlayer->m_dwFrontCharID = Packet.m_dwTargetCharID;
			break;
		case RES_TAIL_RESULT_PARGE:			/* ���Ă������� */
			strTmp.Format ("���Ă����̂���߂܂���");
			pInfoCharPlayer->m_dwFrontCharID = Packet.m_dwTargetCharID;
			break;
		case RES_TAIL_RESULT_TAILPARGE:		/* ��Ă������� */
			pInfoCharPlayer->m_dwTailCharID = 0;
			strTmp.Format ("��Ă����̂���߂܂���");
			break;
		case RES_TAIL_RESULT_NG_TAIL:		/* ���ɂ��Ă����Ă��� */
			if (pInfoCharPara) {
				strNamePara = (LPCSTR)pInfoCharPara->m_strCharName;
			}
			if (pInfoCharTarget) {
				strTmp.Format ("%s�ɂ�%s�����Ă��Ă��܂�", (LPCSTR)pInfoCharTarget->m_strCharName, (LPCSTR)strNamePara);
			}
			break;
		case RES_TAIL_RESULT_NG_STATE:		/* ���Ă����Ȃ���� */
			strTmp.Format ("%s�͎�荞�ݒ��̂悤�ł�", (LPCSTR)pInfoCharTarget->m_strCharName);
			break;
		case RES_TAIL_RESULT_NG_SWOON:		/* ���Ă����Ȃ� */
			strTmp.Format ("�C�⒆�Ȃ̂ł��Ă������Ƃ͂ł��܂���");
			break;
		case RES_TAIL_RESULT_NG_PLACE:		/* ���Ă����Ȃ��ꏊ */
			strTmp.Format ("%s������ꏊ�ł͂��Ă������Ƃ��ł��܂���", (LPCSTR)pInfoCharTarget->m_strCharName);
			break;
		}
		if (strTmp.IsEmpty () == FALSE) {
			m_pMgrData->AddSystemMsg (FALSE, strTmp, RGB (255, 255, 255));
			PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
		}

	} else if (pInfoCharTarget == pInfoCharPlayer) {
		switch (Packet.m_nResult) {
		case RES_TAIL_RESULT_OK:
			strTmp.Format ("%s�����Ă��܂�", (LPCSTR)pInfoChar->m_strCharName);
			m_pMgrData->AddSystemMsg (FALSE, strTmp, RGB (255, 255, 255));
			PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
			pInfoCharPlayer->m_dwTailCharID = Packet.m_dwCharID;
			break;
		case RES_TAIL_RESULT_PARGE:
			strTmp.Format ("%s�͂��Ă����̂���߂܂���", (LPCSTR)pInfoChar->m_strCharName);
			m_pMgrData->AddSystemMsg (FALSE, strTmp, RGB (255, 255, 255));
			PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
			pInfoCharPlayer->m_dwTailCharID = Packet.m_dwCharID;
			break;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_MODIFY_PARAM							 */
/* ���e		:��M����(�p�����[�^�ύX�ʒm)									 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_MODIFY_PARAM(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_MODIFY_PARAM Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}

	switch (Packet.m_nType) {
	case PARAMID_CHAR_MODIFY_ANIME:		/* �A�j���[�V�����ԍ��̕ύX */
		pInfoChar->m_nAnime = Packet.m_nParam;
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_GRP									 */
/* ���e		:��M����(�摜�ʒm)												 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_GRP(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_GRP Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}
	Packet.SetParam (pInfoChar);
	pInfoChar->MakeCharGrp ();
	pInfoChar->m_bRedraw = TRUE;
	m_pLibInfoChar->RenewMotionInfo (pInfoChar);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_MOTION								 */
/* ���e		:��M����(���[�V�������ʒm)									 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_MOTION(PBYTE pData)
{
	CPacketCHAR_MOTION Packet;

	Packet.Set (pData);
	m_pLibInfoMotion->SetList (Packet.m_dwMotionTypeID, Packet.m_dwMotionListID, Packet.m_pInfo);
	m_pLibInfoChar->RenewMotionInfo ((DWORD)0);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWMOTION, Packet.m_dwMotionListID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_MOTIONTYPE							 */
/* ���e		:��M����(���[�V������ʏ��ʒm)								 */
/* ���t		:2008/06/13														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_MOTIONTYPE(PBYTE pData)
{
	PBYTE pDataTmp;
	CPacketCHAR_MOTIONTYPE Packet;

	Packet.Set (pData);
	pDataTmp = Packet.m_pInfo->GetSendData (Packet.m_dwMotionTypeID);
	m_pLibInfoMotionType->SetSendData (pDataTmp);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWMOTIONTYPE, Packet.m_dwMotionTypeID);
	SAFE_DELETE (pDataTmp);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_SET_EFCBALLOON						 */
/* ���e		:��M����(���o�����w��)											 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_SET_EFCBALLOON(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_SET_EFCBALLOON Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->SetBalloonID (Packet.m_dwBalloonID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_SET_MOTION							 */
/* ���e		:��M����(���[�V�������w��)										 */
/* ���t		:2008/01/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_SET_MOTION(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}

	if (Packet.m_dwPara == -1) {
		m_pLibInfoChar->RenewMotionInfo (pInfoChar);
	} else {
		m_pLibInfoChar->SetMotionInfo (pInfoChar, CHARMOTIONID_INTERRUUPT, Packet.m_dwPara);
		pInfoChar->InitMotionInfo (CHARMOTIONID_INTERRUUPT);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_RES_CHECKMAPEVENT						 */
/* ���e		:��M����(�}�b�v�C�x���g�`�F�b�N����)							 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_RES_CHECKMAPEVENT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->m_bWaitCheckMapEvent = FALSE;
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_STATUS								 */
/* ���e		:��M����(�X�e�[�^�X���ʒm)									 */
/* ���t		:2008/06/29														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_STATUS(PBYTE pData)
{
	DWORD dwHPBack;
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_STATUS Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}
	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();

	dwHPBack = pInfoChar->m_dwHP;
	Packet.SetParam (pInfoChar);
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSTATUS, Packet.m_dwCharID);
	if (pInfoChar == pInfoCharPlayer) {
		/* HP���������H */
		if (dwHPBack > pInfoChar->m_dwHP) {
			PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_DAMAGE, Packet.m_dwCharID);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_TEXTEFFECT							 */
/* ���e		:��M����(�����G�t�F�N�g�ʒm)									 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_TEXTEFFECT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_TEXTEFFECT Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	/* �m��Ȃ��L�����H */
	if (pInfoChar == NULL) {
		return;
	}

	if (Packet.m_dwData == -1) {
		pInfoChar->AddTextEffect ((LPCSTR)Packet.m_strData, Packet.m_nColor, Packet.m_nSize);
	} else {
		pInfoChar->AddTextEffect (Packet.m_dwData, Packet.m_nColor, Packet.m_nSize);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_STATE_CHARGE							 */
/* ���e		:��M����(���ߏ�Ԓʒm)											 */
/* ���t		:2008/08/13														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_STATE_CHARGE(PBYTE pData)
{
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoCharPlayer = m_pMgrData->GetPlayerChar ();

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->m_bChargeAtack = (Packet.m_dwPara == 0) ? FALSE : TRUE;
	if (pInfoChar == pInfoCharPlayer) {
		ChgMoveState (FALSE);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_RES_TALKEVENT							 */
/* ���e		:��M����(��b�C�x���g��񉞓�)									 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_RES_TALKEVENT(PBYTE pData)
{
	PCInfoCharCli pInfoCharPlayer;
	CPacketCHAR_RES_TALKEVENT Packet;

	Packet.Set (pData);

	if (Packet.m_dwParam == 0) {
		pInfoCharPlayer = m_pMgrData->GetPlayerChar ();
		if (pInfoCharPlayer) {
			pInfoCharPlayer->m_bWaitCheckMapEvent = FALSE;
		}
	}

	m_pMgrData->SetInfoTalkEvent (Packet.m_pInfo);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWTALKEVENT, Packet.m_dwParam);
	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWTALKEVENT, Packet.m_dwParam);
}

/* Copyright(C)URARA-works 2006 */
