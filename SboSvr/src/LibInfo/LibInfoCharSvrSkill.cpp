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
#include "InfoCharSvr.h"
#include "InfoMapParts.h"
#include "LibInfoMapBase.h"
#include "LibInfoSkill.h"
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
/* �֐���	:CLibInfoCharSvr::UseSkillFISHING								 */
/* ���e		:�X�L���g�p(�ނ�)												 */
/* ���t		:2009/01/01														 */
/* �߂�l	:TRUE:�g�p����													 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::UseSkillFISHING(CInfoCharSvr *pInfoChar, CInfoSkillBase *pInfoSkill)
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
