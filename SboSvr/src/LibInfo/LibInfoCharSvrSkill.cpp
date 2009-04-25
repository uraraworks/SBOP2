/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoCharSvrSkill.cpp									 */
/* ���e			:�L������񃉃C�u�����N���X �����t�@�C��					 */
/* 				:�X�L���֘A����												 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Packet.h"
#include "Command.h"
#include "UraraSockTCPSBO.h"
#include "InfoCharSvr.h"
#include "InfoMapParts.h"
#include "LibInfoMapBase.h"
#include "LibInfoSkill.h"
#include "InfoCharMOVEATACKSvr.h"
#include "InfoSkillMOVEATACK.h"
#include "InfoSkillHEAL.h"
#include "MainFrame.h"
#include "MgrData.h"
#include "LibInfoCharSvr.h"

/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::UseSkill										 */
/* ���e		:�X�L���g�p														 */
/* ���t		:2008/12/31														 */
/* �߂�l	:TRUE:�g�p����													 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::UseSkill(CInfoCharSvr *pInfoChar, DWORD dwSkillID)
{
	BOOL bRet, bResult;
	PCInfoSkillBase pInfoSkill;
	CPacketCHAR_STATUS PacketCHAR_STATUS;

	bRet = FALSE;
	if (pInfoChar == NULL) {
		goto Exit;
	}
	bResult = pInfoChar->HaveSkill (dwSkillID);
	if (bResult == FALSE) {
		goto Exit;
	}
	pInfoSkill = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (dwSkillID);
	if (pInfoSkill == NULL) {
		goto Exit;
	}
	if (pInfoChar->m_dwSP < pInfoSkill->m_dwSP) {
		SendSystemMsg (pInfoChar->m_dwSessionID, "SP������܂���", SYSTEMMSGTYPE_NOLOG);
		goto Exit;
	}

	switch (pInfoSkill->m_nTypeMain) {
	case SKILLTYPEMAIN_NONE:		/* �\�� */
		switch (pInfoSkill->m_nTypeSub) {
		case SKILLTYPESUB_NONE_HEAL:	/* �� */		bRet = UseSkillNONE_HEAL (pInfoChar, pInfoSkill);	break;
		}
		break;
	case SKILLTYPEMAIN_BATTLE:		/* �퓬 */
		switch (pInfoSkill->m_nTypeSub) {
		case SKILLTYPESUB_BATTLE_MOVEATACK:	/* �ړ����čU�� */	bRet = UseSkillBATTLE_MOVEATACK (pInfoChar, pInfoSkill);	break;
		}
		break;
	case SKILLTYPEMAIN_LIFE:		/* ���� */
		switch (pInfoSkill->m_nTypeSub) {
		case SKILLTYPESUB_LIFE_FISHING:	/* �ނ� */	bRet = UseSkillFISHING (pInfoChar, pInfoSkill);	break;
		}
		break;
	}
	if (bRet) {
		pInfoChar->m_dwSP -= pInfoSkill->m_dwSP;

		PacketCHAR_STATUS.Make (pInfoChar);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketCHAR_STATUS);
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::UseSkillNONE_HEAL								 */
/* ���e		:�X�L���g�p(�\��:��)											 */
/* ���t		:2009/01/24														 */
/* �߂�l	:TRUE:�g�p����													 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::UseSkillNONE_HEAL(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkillBase)
{
	int i, nCount, nTmp, nPoint, nDirection;
	BOOL bRet;
	PCInfoMapBase pInfoMap;
	PCInfoCharSvr pInfoCharTmp;
	PCInfoSkillHEAL pInfoSkill = (PCInfoSkillHEAL)pInfoSkillBase;
	CPacketMAP_FORMATMSG PacketMsg;
	RECT rcTarget;
	ARRAYINFOCHARSVR apInfoCharTarget;
	CmyString strMsg;
	CPacketCHAR_STATUS PacketCHAR_STATUS;
	CPacketADMIN_PLAYSOUND PacketPLAYSOUND;
	CPacketCHAR_TEXTEFFECT PacketCHAR_TEXTEFFECT;
	CPacketMAP_SYSTEMMSG PacketSYSTEMMSG;

	bRet = FALSE;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}

	PacketMsg.Make (
			FORMATMSGID_USESKILL,
			pInfoChar->m_dwCharID,
			pInfoSkillBase->m_dwSkillID,
			RGB (255, 255, 255),
			FALSE,
			FORMATMSGTYPE_NOLOG);
	m_pMainFrame->SendToScreenChar (pInfoChar, &PacketMsg);

	nTmp = (int)pInfoSkill->m_dwDistance;
	nDirection = pInfoChar->GetDrawDirection ();
	pInfoChar->GetPosRect (rcTarget);
	/* �͈� */
	switch (pInfoSkill->m_dwArea) {
	case SKILLHEAL_AREA_NONE:			/* �����ʒu */
		break;
	case SKILLHEAL_AREA_FRONT:			/* �O�� */
		switch (nDirection) {
		case 0:
			rcTarget.top -= nTmp;
			break;
		case 1:
			rcTarget.bottom += nTmp;
			break;
		case 2:
			rcTarget.left -= nTmp;
			break;
		case 3:
			rcTarget.right += nTmp;
			break;
		}
		break;
	case SKILLHEAL_AREA_AREA:			/* ���� */
		rcTarget.top	-= nTmp;
		rcTarget.bottom	+= nTmp;
		rcTarget.left	-= nTmp;
		rcTarget.right	+= nTmp;
		break;
	}
	/* �ΏۃL���������擾 */
	GetAreaCharInfo (pInfoChar->m_dwMapID, &rcTarget, apInfoCharTarget);
	nCount = apInfoCharTarget.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = apInfoCharTarget[i];
		if (pInfoChar->IsNPC () != pInfoCharTmp->IsNPC()) {
			continue;
		}

		/* ���� */
		nPoint = genrand () % (pInfoSkill->m_dwValue2 - pInfoSkill->m_dwValue1 + 1);
		nPoint += pInfoSkill->m_dwValue1;

		switch (pInfoSkill->m_dwHealType) {
		case SKILLHEAL_HEALTYPE_HP:			/* HP */
			nPoint = min ((DWORD)nPoint, pInfoCharTmp->m_dwMaxHP - pInfoCharTmp->m_dwHP);
			strMsg.Format ("%s ��HP�� %d �񕜂��܂���", (LPCSTR)pInfoCharTmp->m_strCharName, nPoint);
			pInfoCharTmp->m_dwHP += nPoint;
			break;
		case SKILLHEAL_HEALTYPE_SP:			/* SP */
			nPoint = min ((DWORD)nPoint, pInfoCharTmp->m_dwMaxSP - pInfoCharTmp->m_dwSP);
			strMsg.Format ("%s ��SP�� %d �񕜂��܂���", (LPCSTR)pInfoCharTmp->m_strCharName, nPoint);
			pInfoCharTmp->m_dwSP += nPoint;
			break;
		}
		if (nPoint <= 0) {
			continue;
		}
		PacketSYSTEMMSG.Make ((LPCSTR)strMsg, RGB (255, 255, 255), FALSE, SYSTEMMSGTYPE_NOLOG);
		m_pMainFrame->SendToScreenChar (pInfoCharTmp, &PacketSYSTEMMSG);

		/* �����G�t�F�N�g��ʒm */
		PacketCHAR_TEXTEFFECT.Make (pInfoCharTmp->m_dwCharID, nPoint, 3);
		m_pMainFrame->SendToScreenChar (pInfoCharTmp, &PacketCHAR_TEXTEFFECT);

		if (pInfoCharTmp->m_dwSessionID) {
			PacketCHAR_STATUS.Make (pInfoCharTmp);
			m_pSock->SendTo (pInfoCharTmp->m_dwSessionID, &PacketCHAR_STATUS);
		}
		/* �G�t�F�N�g�̐ݒ� */
		if (pInfoSkill->m_dwHitEffectID != 0) {
			pInfoCharTmp->m_nReserveChgEfect = (int)pInfoSkill->m_dwHitEffectID;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::UseSkillBATTLE_MOVEATACK						 */
/* ���e		:�X�L���g�p(�퓬:�ړ����čU��)									 */
/* ���t		:2009/01/11														 */
/* �߂�l	:TRUE:�g�p����													 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::UseSkillBATTLE_MOVEATACK(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkillBase)
{
	int i, nCount, nDirectionTmp, nDirection;
	BOOL bRet, bResult;
	POINT ptFront;
	PCInfoMapBase pInfoMap;
	PCInfoSkillMOVEATACK pInfoSkill = (PCInfoSkillMOVEATACK)pInfoSkillBase;
	CInfoCharMOVEATACKSvr InfoCharTmp, *pInfoCharTmp;
	CPacketMAP_FORMATMSG PacketMsg;
	ARRAYINT anDirection;

	bRet = FALSE;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}

	switch (pInfoSkill->m_dwPutType) {
	case SKILLMOVEATACKPUTTYPE_FRONT:		/* �O�� */
		nDirection = pInfoChar->GetDrawDirection ();
		anDirection.Add (nDirection);
		break;
	case SKILLMOVEATACKPUTTYPE_CROSS:		/* �㉺���E */
		anDirection.Add (0);
		anDirection.Add (1);
		anDirection.Add (2);
		anDirection.Add (3);
		break;
	}

	nCount = anDirection.GetSize ();
	for (i = 0; i < nCount; i ++) {
		nDirectionTmp = anDirection[i];
		/* �i�߂邩�`�F�b�N */
		bResult = IsMove (pInfoChar, nDirectionTmp);
		if (bResult == FALSE) {
			continue;
		}

		nDirection = pInfoChar->GetDrawDirection ();
		pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
		InfoCharTmp.m_dwParentCharID	= pInfoChar->m_dwCharID;
		InfoCharTmp.m_dwMapID			= pInfoChar->m_dwMapID;
		InfoCharTmp.m_nDirection		= nDirectionTmp;
		InfoCharTmp.m_bChargeAtack		= pInfoChar->m_bChargeAtack;
		InfoCharTmp.m_nMapX				= ptFront.x;
		InfoCharTmp.m_nMapY				= ptFront.y;
		InfoCharTmp.m_dwHP				= 1;
		InfoCharTmp.m_nMoveType			= CHARMOVETYPE_MOVEATACK;
		InfoCharTmp.m_wGrpIDNPC			= (WORD)-1;
		InfoCharTmp.m_wGrpIDCloth		= (WORD)pInfoSkill->m_adwEffectID[nDirectionTmp];

		pInfoCharTmp = (PCInfoCharMOVEATACKSvr)AddNPC (&InfoCharTmp);
		pInfoCharTmp->SetMap (pInfoMap);
		pInfoCharTmp->m_bParentInfo		= FALSE;
		pInfoCharTmp->m_bHitQuit		= pInfoSkill->m_bHitQuit;
		pInfoCharTmp->m_bDistanceDelete	= pInfoSkill->m_bDistanceDelete;
		pInfoCharTmp->m_nAtackTarget	= pInfoSkill->m_dwTartgetType;
		pInfoCharTmp->m_dwMoveWait		= pInfoSkill->m_dwWaitTime;
		pInfoCharTmp->m_dwMoveCount		= pInfoSkill->m_dwDistance;
		pInfoCharTmp->m_dwHitEffectID	= pInfoSkill->m_dwHitEffectID;
		pInfoCharTmp->m_dwValue1		= pInfoSkill->m_dwValue1;
		pInfoCharTmp->m_dwValue2		= pInfoSkill->m_dwValue2;
		pInfoCharTmp->m_dwQuitTime		= timeGetTime () + pInfoSkill->m_dwAliveTime;
	}
	PacketMsg.Make (
			FORMATMSGID_USESKILL,
			pInfoChar->m_dwCharID,
			pInfoSkillBase->m_dwSkillID,
			RGB (255, 255, 255),
			FALSE,
			FORMATMSGTYPE_NOLOG);
	m_pMainFrame->SendToScreenChar (pInfoChar, &PacketMsg);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoCharSvr::UseSkillFISHING								 */
/* ���e		:�X�L���g�p(�ނ�)												 */
/* ���t		:2009/01/01														 */
/* �߂�l	:TRUE:�g�p����													 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::UseSkillFISHING(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkillBase)
{
	BOOL bRet, bResult;
	DWORD dwMotionType;
	POINT ptPos;
	PCInfoMapBase pInfoMap;

	bRet = FALSE;

	/* �ނ�֘A�̕��킩�`�F�b�N */
	dwMotionType = m_pLibInfoItem->GetMotionIDAtack (pInfoChar->m_dwEquipItemIDArmsRight);
	if ((dwMotionType & INFOITEMARMS_MOTION_FISHING) == 0) {
		goto Exit;
	}
	/* �}�b�v�����`�F�b�N */
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}
	/* 1���O�̃p�[�c�������`�F�b�N */
	pInfoChar->GetFrontMapPos (ptPos);
	bResult = pInfoMap->IsFlg (ptPos.x, ptPos.y, BIT_PARTSHIT_FISHING);
	if (bResult == FALSE) {
		goto Exit;
	}

	/* �ނ胂�[�V�����ɕύX */
	pInfoChar->SetMotion (CHARMOTIONLISTID_FISHING_UP);

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2009 */
