/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoCharSvrMapEvent.cpp									 */
/* ���e			:�L������񃉃C�u�����N���X �����t�@�C��					 */
/* 				:�}�b�v�C�x���g�֘A����										 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Packet.h"
#include "Command.h"
#include "LibInfoMapBase.h"
#include "InfoMapEvent.h"
#include "InfoCharSvr.h"
#include "MainFrame.h"
#include "MgrData.h"
#include "LibInfoCharSvr.h"


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::CheckMapEvent									 */
/* ���e		:�}�b�v�C�x���g�`�F�b�N											 */
/* ���t		:2008/06/28														 */
/* �߂�l	:TRUE:������Ԃ�	�`�F�b�N�݂̂̏ꍇ�� TRUE:�}�b�v�C�x���g���� */
/* ========================================================================= */

BOOL CLibInfoCharSvr::CheckMapEvent(
	CInfoCharSvr *pInfoChar,	/* [in] �ΏۃL���� */
	BOOL bCheck/*FALSE*/)		/* [in] TRUE:�`�F�b�N�̂� */
{
	BOOL bRet, bResult, bBreak;
	int x, y;
	RECT rcChar, rcMap, rcMapEvent;
	POINT *pptPos1, *pptPos2;
	PCInfoMapBase pInfoMap;
	PCInfoMapEventBase pInfoMapEventBase;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	bRet = TRUE;
	pInfoMapEventBase = NULL;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}
	pInfoMapEventBase = NULL;
	/* ���W����`�Ŏ擾 */
	pInfoChar->GetPosRect (rcChar);
	pInfoChar->GetMapPosRect (rcMap);

	bBreak = FALSE;
	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			pInfoMapEventBase = pInfoMap->GetEvent (x, y);
			if (pInfoMapEventBase == NULL) {
				continue;
			}
			bResult = TRUE;
			pptPos1 = &pInfoMapEventBase->m_ptPos;
			pptPos2 = &pInfoMapEventBase->m_ptPos2;
			switch (pInfoMapEventBase->m_nHitType) {
			case MAPEVENTHITTYPE_MAPPOS:		/* �}�b�v���W�c�������ꂩ */
				SetRect (&rcMapEvent, pptPos1->x * 2, pptPos1->y * 2, pptPos1->x * 2 + 1, pptPos1->y * 2 + 1);
				bResult = IsInRect (&rcChar, &rcMapEvent);
				break;
			case MAPEVENTHITTYPE_CHARPOS:		/* �L�������W */
				SetRect (&rcMapEvent, pptPos1->x * 2, pptPos1->y * 2, pptPos1->x * 2 + 1, pptPos1->y * 2 + 1);
				bResult = IsHitRect (&rcChar, &rcMapEvent);
				break;
			case MAPEVENTHITTYPE_AREA:			/* �͈� */
				SetRect (&rcMapEvent, pptPos1->x * 2, pptPos1->y * 2, pptPos2->x * 2 + 1, pptPos2->y * 2 + 1);
				bResult = IsInRect (&rcChar, &rcMapEvent);
				break;
			case MAPEVENTHITTYPE_MAPPOS2:		/* �}�b�v���W���S��v */
				bResult = FALSE;
				if ((rcChar.left == pptPos1->x * 2) & (rcChar.top == pptPos1->y * 2 + 1)) {
					bResult = TRUE;
				}
				break;
			}
			if (bResult == FALSE) {
				pInfoMapEventBase = NULL;
				continue;
			}
			bBreak = TRUE;
			break;
		}
		if (bBreak) {
			break;
		}
	}
	if (pInfoMapEventBase == NULL) {
		goto Exit;
	}
	/* �����w�肠��H */
	if (pInfoMapEventBase->m_nHitDirection >= 0) {
		if (pInfoChar->m_nDirection != pInfoMapEventBase->m_nHitDirection) {
			pInfoMapEventBase = NULL;
			goto Exit;
		}
	}
	/* �`�F�b�N�̂݁H */
	if (bCheck) {
		goto Exit;
	}

	if (pInfoMapEventBase->m_dwSoundID != 0) {
		CPacketADMIN_PLAYSOUND PacketPLAYSOUND;

		PacketPLAYSOUND.Make (pInfoMapEventBase->m_dwSoundID);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketPLAYSOUND);
	}

	switch (pInfoMapEventBase->m_nType) {
	case MAPEVENTTYPE_MOVE:			bRet = MapEventProcMOVE			(pInfoChar, pInfoMapEventBase);	break;	/* �}�b�v���ړ� */
	case MAPEVENTTYPE_MAPMOVE:		bRet = MapEventProcMAPMOVE		(pInfoChar, pInfoMapEventBase);	break;	/* �}�b�v�Ԉړ� */
	case MAPEVENTTYPE_INITSTATUS:	bRet = MapEventProcINITSTATUS	(pInfoChar, pInfoMapEventBase);	break;	/* �X�e�[�^�X������ */
	case MAPEVENTTYPE_GRPIDTMP:		bRet = MapEventProcGRPIDTMP		(pInfoChar, pInfoMapEventBase);	break;	/* �ꎞ�摜�ݒ� */
	case MAPEVENTTYPE_LIGHT:		bRet = MapEventProcLIGHT		(pInfoChar, pInfoMapEventBase);	break;	/* ���� */
	}

Exit:
	/* �`�F�b�N�̂݁H */
	if (bCheck) {
		if (pInfoMapEventBase == NULL) {
			bRet = FALSE;
		}
	}
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::MapEventProcMOVE								 */
/* ���e		:�}�b�v�C�x���g����(�}�b�v���ړ�)								 */
/* ���t		:2008/11/16														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::MapEventProcMOVE(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase)
{
	BOOL bRet;
	int i, nCount, nDirection;
	PCInfoMapEventMOVE pInfoMapEvent;
	PCInfoCharSvr pInfoCharTmp;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	bRet = FALSE;
	pInfoMapEvent = (PCInfoMapEventMOVE)pInfoMapEventBase;

	nDirection = pInfoMapEvent->m_nDirection;

	GetTailCharInfo (pInfoChar, apInfoChar);
	apInfoChar.InsertAt (0, pInfoChar);
	nCount = apInfoChar.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = apInfoChar[i];

		/* ����̃L�����ɒʒm */
		PacketCHAR_STATE.Make (pInfoCharTmp->m_dwCharID, CHARMOVESTATE_DELETE);
		m_pMainFrame->SendToScreenChar (pInfoCharTmp, &PacketCHAR_STATE);

		pInfoCharTmp->SetPos (pInfoMapEvent->m_ptDst.x * 2, pInfoMapEvent->m_ptDst.y * 2 + 1, TRUE);
		if (nDirection >= 0) {
			pInfoCharTmp->SetDirection (nDirection);
		}
		pInfoCharTmp->AddProcInfo (CHARPROCID_MAPMOVEIN, 2000, 0);
		pInfoCharTmp->AddProcInfo (CHARPROCID_INVINCIBLE, 5000, 0);
		pInfoCharTmp->m_bStatusInvincible = TRUE;
		pInfoCharTmp->m_bStateFadeInOut = TRUE;

		PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_FADEINOUT, pInfoCharTmp->m_dwMapID, 1);
		m_pMainFrame->SendToClient (pInfoCharTmp->m_dwSessionID, &PacketMAP_PARA1);
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::MapEventProcMAPMOVE							 */
/* ���e		:�}�b�v�C�x���g����(�}�b�v�Ԉړ�)								 */
/* ���t		:2008/11/16														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::MapEventProcMAPMOVE(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase)
{
	BOOL bRet;
	int i, nCount, nDirection;
	PCInfoCharSvr pInfoCharTmp;
	PCInfoMapEventMAPMOVE pInfoMapEvent;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	bRet = FALSE;
	pInfoMapEvent = (PCInfoMapEventMAPMOVE)pInfoMapEventBase;

	nDirection = pInfoMapEvent->m_nDirection;

	GetTailCharInfo (pInfoChar, apInfoChar);
	apInfoChar.InsertAt (0, pInfoChar);

	nCount = apInfoChar.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = apInfoChar[i];

		/* ����̃L�����ɒʒm */
		PacketCHAR_STATE.Make (pInfoCharTmp->m_dwCharID, CHARMOVESTATE_DELETE);
		m_pMainFrame->SendToScreenChar (pInfoCharTmp, &PacketCHAR_STATE);

		pInfoCharTmp->m_dwMapID = pInfoMapEvent->m_dwMapID;
		pInfoCharTmp->SetPos (pInfoMapEvent->m_ptDst.x * 2, pInfoMapEvent->m_ptDst.y * 2 + 1, TRUE);
		if (nDirection >= 0) {
			pInfoCharTmp->SetDirection (nDirection);
		}
		pInfoCharTmp->AddProcInfo (CHARPROCID_MAPMOVEOUT, 2000, 0);
		pInfoCharTmp->AddProcInfo (CHARPROCID_INVINCIBLE, 5000, 0);
		pInfoCharTmp->m_bStatusInvincible = TRUE;
		pInfoCharTmp->m_bStateFadeInOut = TRUE;

		PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_FADEINOUT, pInfoCharTmp->m_dwMapID, 1);
		m_pMainFrame->SendToClient (pInfoCharTmp->m_dwSessionID, &PacketMAP_PARA1);
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::MapEventProcINITSTATUS						 */
/* ���e		:�}�b�v�C�x���g����(�X�e�[�^�X������)							 */
/* ���t		:2008/11/16														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::MapEventProcINITSTATUS(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase)
{
	BOOL bRet;
	PCInfoMapEventINITSTATUS pInfoMapEvent;
	CmyArray<PCInfoCharSvr, PCInfoCharSvr> apInfoChar;

	bRet = TRUE;
	pInfoMapEvent = (PCInfoMapEventINITSTATUS)pInfoMapEventBase;

	if (pInfoMapEvent->m_dwEffectID != 0) {
		CPacketCHAR_SET_EFFECT PacketCHAR_SET_EFFECT;

		PacketCHAR_SET_EFFECT.Make (pInfoChar->m_dwCharID, pInfoMapEvent->m_dwEffectID);
		m_pMainFrame->SendToScreenChar (pInfoChar, &PacketCHAR_SET_EFFECT);
	}

	SetInitStatus (pInfoChar);
	pInfoChar->m_bChgStatus = TRUE;

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::MapEventProcGRPIDTMP							 */
/* ���e		:�}�b�v�C�x���g����(�ꎞ�摜�ݒ�)								 */
/* ���t		:2008/11/16														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::MapEventProcGRPIDTMP(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase)
{
	BOOL bRet;
	PCInfoMapEventGRPIDTMP pInfoMapEvent;

	bRet = TRUE;
	pInfoMapEvent = (PCInfoMapEventGRPIDTMP)pInfoMapEventBase;

	/* �ݒ�H */
	if (pInfoMapEvent->m_nSetType == GRPIDTMPTYPE_ON) {
		pInfoChar->m_wGrpIDTmpMain	= (WORD)pInfoMapEvent->m_dwIDMain;	/* �摜ID(�ꎞ��:���C��) */
		pInfoChar->m_wGrpIDTmpSub	= (WORD)pInfoMapEvent->m_dwIDSub;	/* �摜ID(�ꎞ��:�T�u) */
	/* ���� */
	} else {
		pInfoChar->m_wGrpIDTmpMain	= 0;		/* �摜ID(�ꎞ��:���C��) */
		pInfoChar->m_wGrpIDTmpSub	= 0;		/* �摜ID(�ꎞ��:�T�u) */
	}
	pInfoChar->m_bChgGrp = TRUE;

	/* �t���čs���̂ƕt���ė��Ă���̂������������2��Ă� */
	Tail (pInfoChar, NULL, FALSE);
	Tail (pInfoChar, NULL, FALSE);

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::MapEventProcLIGHT								 */
/* ���e		:�}�b�v�C�x���g����(����)										 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::MapEventProcLIGHT(CInfoCharSvr *pInfoChar, CInfoMapEventBase *pInfoMapEventBase)
{
	BOOL bRet;
	PCInfoMapEventLIGHT pInfoMapEvent;

	bRet = TRUE;
	pInfoMapEvent = (PCInfoMapEventLIGHT)pInfoMapEventBase;

	pInfoChar->m_nLightLevel = 0;
	pInfoChar->m_dwLightTime = 0;
	/* �ݒ�H */
	if (pInfoMapEvent->m_bLightOn) {
		pInfoChar->m_nLightLevel = 1;
		if (pInfoMapEvent->m_dwTime != 0) {
			pInfoChar->m_dwLightTime = timeGetTime () + pInfoMapEvent->m_dwTime;
		}
	}
	pInfoChar->m_bChgStatus = TRUE;

	return bRet;
}

/* Copyright(C)URARA-works 2008 */
