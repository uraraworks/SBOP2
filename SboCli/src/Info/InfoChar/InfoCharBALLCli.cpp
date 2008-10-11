/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharBALLCli.cpp										 */
/* ���e			:�L�������(�{�[��)�N���C�A���g�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotion.h"
#include "InfoCharBALLCli.h"


/* ========================================================================= */
/* �֐���	:CInfoCharBALLCli::CInfoCharBALLCli								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

CInfoCharBALLCli::CInfoCharBALLCli()
{
	m_dwMoveWait		= 6;
	m_bMotionDirection	= FALSE;

	m_adwMotionID[CHARMOTIONID_STAND]	= CHARMOTIONLISTID_STAND;	/* ���[�V����ID(����) */
	m_adwMotionID[CHARMOTIONID_WALK]	= CHARMOTIONLISTID_WALK;	/* ���[�V����ID(����) */
}


/* ========================================================================= */
/* �֐���	:CInfoCharBALLCli::~CInfoCharBALLCli							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

CInfoCharBALLCli::~CInfoCharBALLCli()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharBALLCli::ChgMoveState									 */
/* ���e		:�ړ���ԕύX													 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

void CInfoCharBALLCli::ChgMoveState(int nMoveState)
{
	CInfoCharCli::ChgMoveState (nMoveState);

	switch (nMoveState) {
	case CHARMOVESTATE_MOVE:		/* �ړ��� */
		m_nAnime = 0;
		m_dwLastTimeAnime = timeGetTime ();
		break;
	}
}

/* Copyright(C)URARA-works 2007 */
