/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcCHAR.cpp									 */
/* ���e			:�T�[�o�[���C���t���[��(�L�����n��M����) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/31													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoAccount.h"
#include "LibInfoItem.h"
#include "LibInfoMapBase.h"
#include "LibInfoMapParts.h"
#include "LibInfoSkill.h"
#include "InfoAccount.h"
#include "InfoMapParts.h"
#include "InfoMapEventBase.h"
#include "LibInfoCharSvr.h"
#include "LibInfoTalkEvent.h"
#include "InfoCharSvr.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR										 */
/* ���e		:��M����(�L�����n)												 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO:		RecvProcCHAR_REQ_CHARINFO		(pData, dwSessionID);	break;	/* �L�������v�� */
	case SBOCOMMANDID_SUB_CHAR_MOVEPOS:				RecvProcCHAR_MOVEPOS			(pData, dwSessionID);	break;	/* �ړ��ʒm */
	case SBOCOMMANDID_SUB_CHAR_STATE:				RecvProcCHAR_STATE				(pData, dwSessionID);	break;	/* ��Ԓʒm */
	case SBOCOMMANDID_SUB_CHAR_REQ_CHAT:			RecvProcCHAR_REQ_CHAT			(pData, dwSessionID);	break;	/* �`���b�g�v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO2:		RecvProcCHAR_REQ_CHARINFO2		(pData, dwSessionID);	break;	/* �L�������v��(����) */
	case SBOCOMMANDID_SUB_CHAR_REQ_PUTGET:			RecvProcCHAR_REQ_PUTGET			(pData, dwSessionID);	break;	/* �A�C�e�����E���u���v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_USEITEM:			RecvProcCHAR_REQ_USEITEM		(pData, dwSessionID);	break;	/* �A�C�e�����g���v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_DRAGITEM:		RecvProcCHAR_REQ_DRAGITEM		(pData, dwSessionID);	break;	/* �A�C�e���ʒu�ύX�v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_PUSH:			RecvProcCHAR_REQ_PUSH			(pData, dwSessionID);	break;	/* �����v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_TAIL:			RecvProcCHAR_REQ_TAIL			(pData, dwSessionID);	break;	/* �t���čs���v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_MODIFY_PARAM:	RecvProcCHAR_REQ_MODIFY_PARAM	(pData, dwSessionID);	break;	/* �p�����[�^�ύX�v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_EQUIP:			RecvProcCHAR_REQ_EQUIP			(pData, dwSessionID);	break;	/* �����v�� */
	case SBOCOMMANDID_SUB_CHAR_PROC_FISHING:		RecvProcCHAR_PROC_FISHING		(pData, dwSessionID);	break;	/* �ނ�v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_CHECKMAPEVENT:	RecvProcCHAR_REQ_CHECKMAPEVENT	(pData, dwSessionID);	break;	/* �}�b�v�C�x���g�`�F�b�N�v�� */
	case SBOCOMMANDID_SUB_CHAR_STATE_CHARGE:		RecvProcCHAR_STATE_CHARGE		(pData, dwSessionID);	break;	/* ���ߏ�Ԓʒm */
	case SBOCOMMANDID_SUB_CHAR_REQ_RECOVERY:		RecvProcCHAR_REQ_RECOVERY		(pData, dwSessionID);	break;	/* �C��㕜���v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_TALKEVENT:		RecvProcCHAR_REQ_TALKEVENT		(pData, dwSessionID);	break;	/* ��b�C�x���g���v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_ADDSKILL:		RecvProcCHAR_REQ_ADDSKILL		(pData, dwSessionID);	break;	/* �X�L���ǉ��v�� */
	case SBOCOMMANDID_SUB_CHAR_REQ_USESKILL:		RecvProcCHAR_REQ_USESKILL		(pData, dwSessionID);	break;	/* �X�L�����g���v�� */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_CHARINFO							 */
/* ���e		:��M����(�L�������v��)										 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_CHARINFO(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharBase pInfoChar;
	CPacketCHAR_REQ_CHARINFO Packet;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	PacketRES_CHARINFO.Make (pInfoChar);
	m_pSock->SendTo (dwSessionID, &PacketRES_CHARINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_MOVEPOS								 */
/* ���e		:��M����(�ړ��ʒm)												 */
/* ���t		:2007/01/14														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_MOVEPOS(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	int i, nCount, nResult;
	PCInfoCharSvr pInfoChar, pInfoCharTmp, pInfoCharFront;
	CPacketCHAR_MOVEPOS Packet;
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	if (pInfoChar->m_dwMapID != Packet.m_dwMapID) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	bResult = pInfoChar->IsEnableMove ();
	if (bResult == FALSE) {
		if (pInfoChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			if (((pInfoChar->m_nMapX == Packet.m_pos.x) && (pInfoChar->m_nMapY == Packet.m_pos.y))) {
				pInfoChar->SetDirection (Packet.m_nDirection);
				pInfoChar->m_bChgPos = TRUE;
				/* �h�䒆�̌����ύX�͏������� */
				return;
			}
		}
		bResult = pInfoChar->IsStateBattle ();
		if (bResult == FALSE) {
			/* �ړ��ł��Ȃ���ԂȂ̂Ŗ��� */
			return;
		}
	}
	if (pInfoChar->m_dwHP == 0) {
		/* HP0�ŕ������Ƃ������͋C�₳���� */
		pInfoChar->SetMoveState (CHARMOVESTATE_SWOON);
		return;
	}

	/* �ړ������H */
	if (!((pInfoChar->m_nMapX == Packet.m_pos.x) && (pInfoChar->m_nMapY == Packet.m_pos.y))) {
		/* �t���ė��Ă���L�����ꗗ���쐬 */
		pInfoCharTmp = pInfoChar;
		while (1) {
			if (pInfoCharTmp->m_dwTailCharID == 0) {
				break;
			}
			pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (pInfoCharTmp->m_dwTailCharID);
			if (pInfoCharTmp == NULL) {
				break;
			}
			apInfoChar.Add (pInfoCharTmp);
		}

		/* �O�̃L�����̍��W�֐i�ނ悤�ɐݒ肵�Ă��� */
		nCount = apInfoChar.GetSize ();
		for (i = nCount - 1; i >= 0; i --) {
			pInfoCharTmp	= apInfoChar[i];
			pInfoCharFront	= (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (pInfoCharTmp->m_dwFrontCharID);
			if (pInfoCharFront == NULL) {
				continue;
			}
			nResult = pInfoCharTmp->GetDirection (pInfoCharFront->m_ptCharBack.x, pInfoCharFront->m_ptCharBack.y);
			if (nResult != -1) {
				pInfoCharTmp->SetDirection (nResult);
			}
			pInfoCharTmp->SetPos (pInfoCharFront->m_ptCharBack.x, pInfoCharFront->m_ptCharBack.y);
			pInfoCharTmp->m_bChgPos			= TRUE;
			pInfoCharTmp->m_bChgUpdatePos	= TRUE;
		}
	}

	pInfoChar->SetPos (Packet.m_pos.x, Packet.m_pos.y);
	pInfoChar->SetDirection (Packet.m_nDirection);
	pInfoChar->m_bChgPos = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_STATE									 */
/* ���e		:��M����(��Ԓʒm)												 */
/* ���t		:2007/04/20														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_STATE(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	int nState;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_STATE Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	nState = Packet.m_nState;

	switch (nState) {
	case CHARMOVESTATE_MOVE:			/* �ړ��� */
		bResult = pInfoChar->IsEnableMove ();
		if (bResult == FALSE) {
			nState = pInfoChar->m_nMoveState;
		}
		break;
	case CHARMOVESTATE_BATTLE:			/* �퓬�� */
	case CHARMOVESTATE_BATTLEATACK:		/* �퓬�U���� */
	case CHARMOVESTATE_BATTLE_DEFENSE:	/* �h�䒆 */
		bResult = pInfoChar->IsEnableBattle ();
		if (bResult == FALSE) {
			nState = pInfoChar->m_nMoveState;
		}
		break;
	}

	pInfoChar->SetMoveState (nState);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_CHAT								 */
/* ���e		:��M����(�ړ��ʒm)												 */
/* ���t		:2007/01/14														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_CHAT(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_REQ_CHAT Packet;
	CPacketMAP_SYSTEMMSG PacketMAP_SYSTEMMSG;
	CmyString strTmp, strChar;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	TrimViewString (strChar, Packet.m_strChat);

	switch (Packet.m_nType) {
	case CHATTYPE_NORMAL:				/* �ʏ� */
		pInfoChar->SetSpeak (strChar);
		break;
//	case CHATTYPE_PARTY:				/* �p�[�e�B�`���b�g */
//	case CHATTYPE_MAP:					/* �}�b�v������ */
	case CHATTYPE_ADMIN:				/* ��ĕ��� */
		strTmp.Format ("%s:%s", (LPCSTR)pInfoChar->m_strCharName, (LPCSTR)strChar);
		PacketMAP_SYSTEMMSG.Make (strTmp);
		m_pSock->SendTo (0, &PacketMAP_SYSTEMMSG);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_CHARINFO2							 */
/* ���e		:��M����(�L�������v��(����))									 */
/* ���t		:2007/02/13														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_CHARINFO2(PBYTE pData, DWORD dwSessionID)
{
	int i, nCount;
	PCInfoCharBase pInfoChar;
	CPacketCHAR_REQ_CHARINFO2 Packet;
	CPacketCHAR_CHARINFO PacketCHAR_CHARINFO;
	CLibInfoCharSvr LibInfoCharTmp;

	Packet.Set (pData);

	LibInfoCharTmp.Create (m_pMgrData);

	nCount = Packet.m_adwCharID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn (Packet.m_adwCharID[i]);
		if (pInfoChar == NULL) {
			continue;
		}
		LibInfoCharTmp.CopyAdd (pInfoChar);
	}

	PacketCHAR_CHARINFO.Make (&LibInfoCharTmp);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_CHARINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_PUTGET							 */
/* ���e		:��M����(�A�C�e�����E���u���v��)								 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_PUTGET(PBYTE pData, DWORD dwSessionID)
{
	int nResult, i, nCount, nMapEventType;
	BOOL bResult;
	POINT ptPos, ptMapPos;
	PCInfoCharSvr pInfoChar;
	PCInfoItem pInfoItem;
	PCInfoMapBase pInfoMap;
	CPacketCHAR_REQ_PUTGET Packet;
	CPacketCHAR_RES_PUTGET PacketCHAR_RES_PUTGET;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;
	CPacketCHAR_ITEMINFO PacketCHAR_ITEMINFO;
	CmyArray<POINT, POINT> aptPos;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}

	pInfoChar->GetFrontPos (ptPos);

	/* �A�C�e�����E���H */
	if (Packet.m_dwItemID == 0) {
		pInfoChar->GetFrontPos (aptPos);
		nCount = aptPos.GetSize ();
		for (i = 0; i < nCount; i ++) {
			ptPos = aptPos[i];
			pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (pInfoChar->m_dwMapID, &ptPos, FALSE);
			if (pInfoItem) {
				break;
			}
		}
		if (i < nCount) {
			bResult = pInfoChar->IsItemAdd ();
			if (bResult) {
				/* �A�C�e����ǉ� */
				m_pLibInfoItem->AddItem (pInfoChar->m_dwCharID, pInfoItem->m_dwItemID, &pInfoChar->m_adwItemID);

				PacketITEM_RES_ITEMINFO.Make (pInfoItem);
				m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketITEM_RES_ITEMINFO);

				PacketCHAR_ITEMINFO.Make (pInfoChar->m_dwCharID, &pInfoChar->m_adwItemID);
				m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketCHAR_ITEMINFO);

				PacketCHAR_RES_PUTGET.Make (pInfoItem->m_dwItemID, RESULTID_CHAR_RES_PUTGET_GET);
				m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketCHAR_RES_PUTGET);

				PacketITEM_RES_ITEMINFO.Make (pInfoItem);
				SendToMapChar (pInfoChar->m_dwMapID, &PacketITEM_RES_ITEMINFO);
			} else {
				/* ���ĂȂ� */
			}
		}

	} else {
		bResult = pInfoChar->HaveItem (Packet.m_dwItemID);
		if (bResult == FALSE) {
			return;
		}
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (pInfoChar->m_dwMapID, &ptPos);
		if (pInfoItem) {
			if (pInfoItem->m_bPutOn == FALSE) {
				/* ���łɒu����Ă���ꏊ�ɂ͒u���Ȃ� */
				return;
			}
		}
		bResult = pInfoMap->IsMove (ptPos.x / 2, ptPos.y / 2, pInfoChar->m_nDirection);
		if (bResult == FALSE) {
			/* �ړ��ł��Ȃ��Ƃ���ɂ͒u���Ȃ� */
			return;
		}
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (Packet.m_dwItemID);
		if (pInfoItem == NULL) {
			return;
		}
		pInfoChar->DeleteItem (pInfoItem->m_dwItemID);
		m_pLibInfoItem->DeleteItem (pInfoItem->m_dwItemID, pInfoChar);

		PacketCHAR_ITEMINFO.Make (pInfoChar->m_dwCharID, &pInfoChar->m_adwItemID);
		m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketCHAR_ITEMINFO);

		PacketITEM_RES_ITEMINFO.Make (pInfoItem);
		SendToMapChar (pInfoChar->m_dwMapID, &PacketITEM_RES_ITEMINFO);

		nResult = RESULTID_CHAR_RES_PUTGET_PUT;

		/* �S�~������ */
		pInfoChar->GetFrontMapPos (ptMapPos);
		nMapEventType = pInfoMap->GetMapEventType (ptMapPos.x, ptMapPos.y);
		if (nMapEventType == MAPEVENTTYPE_TRASHBOX) {
			nResult = RESULTID_CHAR_RES_PUTGET_DELETE;
			PacketCHAR_RES_PUTGET.Make (pInfoItem->m_dwItemID, nResult);
			m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketCHAR_RES_PUTGET);

			CPacketITEM_DELETEITEMINFO PacketITEM_DELETEITEMINFO;
			PacketITEM_DELETEITEMINFO.Make (Packet.m_dwItemID);
			if (pInfoItem->m_dwMapID) {
				SendToMapChar (pInfoItem->m_dwMapID, &PacketITEM_DELETEITEMINFO);
			}
			SendToAdminChar (&PacketITEM_DELETEITEMINFO);

			m_pLibInfoItem->Delete (Packet.m_dwItemID);
		}

		if (nResult == RESULTID_CHAR_RES_PUTGET_PUT) {
			PacketCHAR_RES_PUTGET.Make (pInfoItem->m_dwItemID, nResult);
			m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketCHAR_RES_PUTGET);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_USEITEM							 */
/* ���e		:��M����(�A�C�e�����g���v��)									 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_USEITEM(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_REQ_USEITEM Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	m_pLibInfoChar->UseItem (pInfoChar, Packet.m_dwItemID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_DRAGITEM							 */
/* ���e		:��M����(�A�C�e���ʒu�ύX�v��)									 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_DRAGITEM(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_REQ_DRAGITEM Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	m_pLibInfoChar->DragItem (pInfoChar, Packet.m_dwItemID, Packet.m_ptNewPos);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_PUSH								 */
/* ���e		:��M����(�����v��)												 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_PUSH(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_REQ_PUSH Packet;

	Packet.Set (pData);

	if (Packet.m_nPushCount == 1) {
		m_pLibInfoChar->Move (Packet.m_dwCharID, Packet.m_nDirection, TRUE);
	} else {
		pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
		if (pInfoChar == NULL) {
			return;
		}
		pInfoChar->m_nDirection = Packet.m_nDirection;
		pInfoChar->m_nMoveCount = Packet.m_nPushCount;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_TAIL								 */
/* ���e		:��M����(�t���čs���v��)										 */
/* ���t		:2007/09/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_TAIL(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	int nResult;
	PCInfoCharSvr pInfoChar, pInfoCharTarget;
	CPacketCHAR_REQ_TAIL Packet;

	nResult = RES_TAIL_RESULT_NONE;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}
	pInfoCharTarget = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwTargetCharID);
	m_pLibInfoChar->Tail (pInfoChar, pInfoCharTarget, Packet.m_bTail);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_TAIL								 */
/* ���e		:��M����(�p�����[�^�ύX�v��)									 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_MODIFY_PARAM(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar, pInfoCharTmp;
	CPacketCHAR_REQ_MODIFY_PARAM Packet;
	CPacketCHAR_MODIFY_PARAM PacketMODIFY_PARAM;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	switch (Packet.m_nType) {
	case PARAMID_CHAR_REQ_MODIFY_ANIME:			/* �A�j���[�V�����ԍ��̕ύX */
		pInfoChar->m_nAnime ++;
		if (pInfoChar->m_nAnime >= 10) {
			pInfoChar->m_nAnime = 0;
		}
		PacketMODIFY_PARAM.Make (pInfoChar->m_dwCharID, PARAMID_CHAR_MODIFY_ANIME, pInfoChar->m_nAnime);
		SendToScreenChar (pInfoChar, &PacketMODIFY_PARAM);
		break;

	case PARAMID_CHAR_REQ_MODIFY_STYLECOPY_PUT:	/* �e�p�̃R�s�[(��荞��) */
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn ((DWORD)Packet.m_nParam);
		if (pInfoCharTmp == NULL) {
			break;
		}
		pInfoChar->m_wFamilyID			= pInfoCharTmp->m_wFamilyID;		/* �푰ID */
		pInfoChar->m_wGrpIDNPC			= pInfoCharTmp->m_wGrpIDNPC;		/* �摜ID(NPC) */
		pInfoChar->m_wGrpIDCloth		= pInfoCharTmp->m_wGrpIDCloth;		/* �摜ID(��) */
		pInfoChar->m_wGrpIDEye			= pInfoCharTmp->m_wGrpIDEye;		/* �摜ID(��) */
		pInfoChar->m_wGrpIDEyeColor		= pInfoCharTmp->m_wGrpIDEyeColor;	/* �摜ID(�ڐF) */
		pInfoChar->m_wGrpIDHairType		= pInfoCharTmp->m_wGrpIDHairType;	/* �摜ID(��) */
		pInfoChar->m_wGrpIDHairColor	= pInfoCharTmp->m_wGrpIDHairColor;	/* �摜ID(���F) */
		pInfoChar->m_wGrpIDSP			= pInfoCharTmp->m_wGrpIDSP;			/* �摜ID(���ꕞ) */
		pInfoChar->m_wGrpIDAcce			= pInfoCharTmp->m_wGrpIDAcce;		/* �摜ID(�A�N�Z�T��) */
		pInfoChar->m_wGrpIDArmsMain		= pInfoCharTmp->m_wGrpIDArmsMain;	/* �摜ID(������:���C��) */
		pInfoChar->m_wGrpIDArmsSub		= pInfoCharTmp->m_wGrpIDArmsSub;	/* �摜ID(������:�T�u) */
		pInfoChar->m_clName				= pInfoCharTmp->m_clName;			/* ���O�̐F */
		pInfoChar->m_nSex				= pInfoCharTmp->m_nSex;				/* ���� */
		pInfoChar->m_dwMotionTypeID		= pInfoCharTmp->m_dwMotionTypeID;	/* ���[�V�������ID */
		PacketRES_CHARINFO.Make (pInfoChar);
		SendToScreenChar (pInfoChar, &PacketRES_CHARINFO);
		break;
	case PARAMID_CHAR_REQ_MODIFY_STYLECOPY_GET:	/* �e�p�̃R�s�[(���f) */
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn ((DWORD)Packet.m_nParam);
		if (pInfoCharTmp == NULL) {
			break;
		}
		pInfoChar->m_wGrpIDCloth	= pInfoCharTmp->m_wGrpIDCloth;		/* �摜ID(��) */
		pInfoChar->m_wGrpIDHairType	= pInfoCharTmp->m_wGrpIDHairType;	/* �摜ID(��) */
		pInfoChar->m_wGrpIDSP		= pInfoCharTmp->m_wGrpIDSP;			/* �摜ID(���ꕞ) */
		pInfoChar->m_nSex			= pInfoCharTmp->m_nSex;				/* ���� */
		pInfoChar->m_dwMotionTypeID	= pInfoCharTmp->m_dwMotionTypeID;	/* ���[�V�������ID */
		PacketRES_CHARINFO.Make (pInfoChar);
		SendToScreenChar (pInfoChar, &PacketRES_CHARINFO);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_EQUIP								 */
/* ���e		:��M����(�����v��)												 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_EQUIP(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_REQ_EQUIP Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID (dwSessionID);
	if (bResult == FALSE) {
		PostMessage (m_hWnd, WM_DISCONNECT, 0, dwSessionID);
		return;
	}

	/* ��������H */
	if (Packet.m_nUnsetType == -1) {
		m_pLibInfoChar->Equip (pInfoChar, Packet.m_dwItemID);

	/* �������� */
	} else {
		m_pLibInfoChar->UnEquip (pInfoChar, Packet.m_dwItemID);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_PROC_FISHING							 */
/* ���e		:��M����(�ނ�v��)												 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_PROC_FISHING(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult, bRet;
	int nProcState;
	POINT ptPos;
	PCInfoCharSvr pInfoChar;
	PCInfoMapBase pInfoMap;
	CPacketCHAR_PARA1 Packet;

	bRet = FALSE;

	Packet.Set (pData);
	nProcState = CHARPROCSTATEID_NORMAL;

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}
	bResult = pInfoChar->IsEnableBattle ();
	if (bResult == FALSE) {
		goto Exit;
	}
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}
	pInfoChar->GetFrontMapPos (ptPos);
	bResult = pInfoMap->IsFlg (ptPos.x, ptPos.y, BIT_PARTSHIT_FISHING);
	if (bResult == FALSE) {
		goto Exit;
	}
	nProcState = CHARPROCSTATEID_FISHING;
	pInfoChar->AddProcInfo (CHARPROCID_FISHING, 2000, Packet.m_dwPara);

	bRet = TRUE;
Exit:
	if (pInfoChar && (bRet == FALSE)) {
		bResult = pInfoChar->IsEnableBattle ();
		if (bResult) {
			pInfoChar->SetMoveState (CHARMOVESTATE_BATTLE);
		}
	}
	pInfoChar->SetProcState (nProcState);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_CHECKMAPEVENT						 */
/* ���e		:��M����(�}�b�v�C�x���g�`�F�b�N�v��)							 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_CHECKMAPEVENT(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}
	if (pInfoChar->IsEnableMove () == FALSE) {
		goto Exit;
	}
	pInfoChar->m_bWaitCheckMapEvent = TRUE;

Exit:
	if (pInfoChar && (pInfoChar->m_bWaitCheckMapEvent == FALSE)) {
		Packet.Make (SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT, pInfoChar->m_dwCharID, 0);
		SendToClient (pInfoChar->m_dwSessionID, &Packet);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_STATE_CHARGE							 */
/* ���e		:��M����(���ߏ�Ԓʒm)											 */
/* ���t		:2008/08/13														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_STATE_CHARGE(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->m_bChargeAtack = (Packet.m_dwPara == 0) ? FALSE : TRUE;
	/* ����̃L�����ɂ��ʒm */
	Packet.Make (SBOCOMMANDID_SUB_CHAR_STATE_CHARGE, Packet.m_dwCharID, Packet.m_dwPara);
	SendToScreenChar (pInfoChar, &Packet);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_RECOVERY							 */
/* ���e		:��M����(�C��㕜���v��)										 */
/* ���t		:2008/12/02														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_RECOVERY(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_PARA1 Packet;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	if (pInfoChar->m_nMoveState != CHARMOVESTATE_SWOON) {
		return;
	}
	pInfoChar->AddProcInfo (CHARPROCID_SWOON, 2000, 1);

	/* ����̃L�����ɒʒm */
	PacketCHAR_STATE.Make (pInfoChar->m_dwCharID, CHARMOVESTATE_DELETE);
	SendToScreenChar (pInfoChar, &PacketCHAR_STATE);

	pInfoChar->m_bStateFadeInOut = TRUE;
	PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_FADEINOUT, pInfoChar->m_dwMapID, 1);
	SendToClient (pInfoChar->m_dwSessionID, &PacketMAP_PARA1);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_TALKEVENT							 */
/* ���e		:��M����(��b�C�x���g���v��)									 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_TALKEVENT(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CInfoTalkEvent InfoTalkEventTmp, *pInfoTalkEvent;
	CPacketCHAR_PARA1 Packet;
	CPacketCHAR_RES_TALKEVENT PacketCHAR_RES_TALKEVENT;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	pInfoTalkEvent = (PCInfoTalkEvent)m_pLibInfoTalkEvent->GetPtr (Packet.m_dwCharID);
	if (pInfoTalkEvent == NULL) {
		pInfoTalkEvent = &InfoTalkEventTmp;
		pInfoTalkEvent->m_dwTalkEventID = Packet.m_dwCharID;
	}
	PacketCHAR_RES_TALKEVENT.Make (pInfoTalkEvent, Packet.m_dwPara);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_RES_TALKEVENT);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_ADDSKILL							 */
/* ���e		:��M����(�X�L���ǉ��v��)										 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_ADDSKILL(PBYTE pData, DWORD dwSessionID)
{
	int i, nCount;
	PARRAYDWORD paSkill;
	PCInfoCharSvr pInfoChar;
	PCInfoSkillBase pInfoSkill;
	CPacketCHAR_PARA1 Packet;
	CPacketCHAR_SKILLINFO PacketCHAR_SKILLINFO;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	pInfoSkill = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (Packet.m_dwPara);
	if (pInfoSkill == NULL) {
		return;
	}

	paSkill = pInfoChar->GetSkill ();
	nCount = paSkill->GetSize ();
	for (i = 0; i < nCount; i ++) {
		if (paSkill->GetAt (i) == Packet.m_dwPara) {
			break;
		}
	}
	/* ���łɎ����Ă���H */
	if (i < nCount) {
		return;
	}

//Todo:
	/* �����̔���ȂǓ���� */

	paSkill->Add (Packet.m_dwPara);
	PacketCHAR_SKILLINFO.Make (Packet.m_dwCharID, paSkill);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_SKILLINFO);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCHAR_REQ_USESKILL							 */
/* ���e		:��M����(�X�L�����g���v��)										 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCHAR_REQ_USESKILL(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	m_pLibInfoChar->UseSkill (pInfoChar, Packet.m_dwPara);
}

/* Copyright(C)URARA-works 2006 */
