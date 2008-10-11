/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharATACKANIMECli.cpp									 */
/* ���e			:�L�������(�U���󂯂�ƃA�j���[�V����)�N���C�A���g�N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/10													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrSound.h"
#include "InfoCharATACKANIMECli.h"


/* ========================================================================= */
/* �֐���	:CInfoCharATACKANIMECli::CInfoCharATACKANIMECli					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/10														 */
/* ========================================================================= */

CInfoCharATACKANIMECli::CInfoCharATACKANIMECli()
{
	m_bMotionDirection = FALSE;
	m_nAnime = 0;

	m_adwMotionID[CHARMOTIONID_STAND]	= CHARMOTIONLISTID_STAND;			/* ���[�V����ID(����) */
	m_adwMotionID[CHARMOTIONID_WALK]	= CHARMOTIONLISTID_STAND_SIMPLE;	/* ���[�V����ID(����) */
	m_adwMotionID[CHARMOTIONID_ANIME]	= CHARMOTIONLISTID_DESTROY;			/* ���[�V����ID(�A�j���[�V����) */
}


/* ========================================================================= */
/* �֐���	:CInfoCharATACKANIMECli::~CInfoCharATACKANIMECli				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/10														 */
/* ========================================================================= */

CInfoCharATACKANIMECli::~CInfoCharATACKANIMECli()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharATACKANIMECli::ChgMoveState							 */
/* ���e		:�ړ���ԕύX													 */
/* ���t		:2007/09/10														 */
/* ========================================================================= */

void CInfoCharATACKANIMECli::ChgMoveState(int nMoveState)
{
	CInfoCharCli::ChgMoveState (nMoveState);

	m_nAnime = 0;
}


/* ========================================================================= */
/* �֐���	:CInfoCharATACKANIMECli::RenewAnime								 */
/* ���e		:�A�j���[�V�����̍X�V											 */
/* ���t		:2007/09/11														 */
/* ========================================================================= */

BOOL CInfoCharATACKANIMECli::RenewAnime(
	DWORD dwTime,		/* [in] ���� */
	int nAdd/*=1*/)		/* [in] ���̃p�^�[���ւ̑����� */
{
	BOOL bRet;
	int nCount;
	DWORD dwTmp;

	bRet	= FALSE;
	nCount	= 0;

	if (m_nMoveState != CHARMOVESTATE_ANIME) {
		bRet = TRUE;
		goto Exit;
	}

	bRet = CInfoCharCli::RenewAnime (dwTime);
	GetMotionInfo (&nCount);

	if (m_nAnime >= nCount - 1) {
		dwTmp = dwTime - m_dwLastTimeAnime;
		if (dwTmp > 5000) {
//Todo:�Ƃ肠����
			ChgMoveState (CHARMOVESTATE_STAND);
		}
		goto Exit;
	}

	if (bRet && m_nAnime == 1) {
//Todo:�Ƃ肠����
		switch (m_wGrpIDNPC) {
		case 8:		m_pMgrSound->PlaySound (SOUNDID_CRASH20_D);	break;
		case 9:		m_pMgrSound->PlaySound (SOUNDID_KUSA);		break;
		case 10:	m_pMgrSound->PlaySound (SOUNDID_KAI);		break;
		}
	}

Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2007 */
