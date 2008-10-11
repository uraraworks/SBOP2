/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharSCORECli.cpp										 */
/* ���e			:�L�������(���_)�N���C�A���g�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/09													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharSCORECli.h"


/* ========================================================================= */
/* �֐���	:CInfoCharSCORECli::CInfoCharSCORECli							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

CInfoCharSCORECli::CInfoCharSCORECli()
{
	m_bMotionDirection = FALSE;
	m_adwMotionID[CHARMOTIONID_STAND]	= CHARMOTIONLISTID_SIMPLE;	/* ���[�V����ID(����) */
	m_adwMotionID[CHARMOTIONID_WALK]	= CHARMOTIONLISTID_SIMPLE;	/* ���[�V����ID(����) */
}


/* ========================================================================= */
/* �֐���	:CInfoCharSCORECli::~CInfoCharSCORECli							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

CInfoCharSCORECli::~CInfoCharSCORECli()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharSCORECli::ChgMoveState								 */
/* ���e		:�ړ���ԕύX													 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

void CInfoCharSCORECli::ChgMoveState(int nMoveState)
{
	int nAnimeBack;

	nAnimeBack = m_nAnime;
	CInfoCharCli::ChgMoveState (nMoveState);
	m_nAnime = nAnimeBack;
}

/* Copyright(C)URARA-works 2007 */
