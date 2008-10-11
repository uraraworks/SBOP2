/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharMOVEATACKCli.cpp									 */
/* ���e			:�L�������(�ړ����čU��)�N���C�A���g�N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharMOVEATACKCli.h"


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKCli::CInfoCharMOVEATACKCli					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

CInfoCharMOVEATACKCli::CInfoCharMOVEATACKCli()
{
	m_bMotionDirection = FALSE;

	m_adwMotionID[CHARMOTIONID_STAND]		= CHARMOTIONLISTID_STAND_SIMPLE;	/* ���[�V����ID(����) */
	m_adwMotionID[CHARMOTIONID_WALK]		= CHARMOTIONLISTID_STAND_SIMPLE;	/* ���[�V����ID(����) */
	m_adwMotionID[CHARMOTIONID_BATTLESTAND]	= CHARMOTIONLISTID_STAND_SIMPLE;	/* �퓬���� */
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKCli::~CInfoCharMOVEATACKCli					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

CInfoCharMOVEATACKCli::~CInfoCharMOVEATACKCli()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKCli::ChgMoveState							 */
/* ���e		:�ړ���ԕύX													 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

void CInfoCharMOVEATACKCli::ChgMoveState(int nMoveState)
{
	BOOL bResult;

	if (nMoveState < 0) {
		return;
	}
	/* �퓬�� */
	if (nMoveState == CHARMOVESTATE_BATTLE) {
		nMoveState = CHARMOVESTATE_MOVE;
	}

	bResult = IsStateMove ();
	if (bResult) {
		switch (nMoveState) {
		case CHARMOVESTATE_STAND:				/* ���� */
		case CHARMOVESTATE_BATTLEATACK:			/* �퓬�U���� */
			AddMovePosQue (nMoveState, -1, -1, -1);
			return;
		case CHARMOVESTATE_MOVE:				/* �ړ��� */
			CInfoCharBase::SetMoveState (nMoveState);
			return;
		}
	}
	switch (nMoveState) {
	case CHARMOVESTATE_STAND:				/* ���� */
	case CHARMOVESTATE_MOVE:				/* �ړ��� */
		m_dwLastTimeMove = 0;
		break;
	case CHARMOVESTATE_DELETEREADY:			/* �������� */
	case CHARMOVESTATE_DELETE:				/* ���� */
		break;
	case CHARMOVESTATE_BATTLEMOVE:			/* �퓬�ړ��� */
	case CHARMOVESTATE_BATTLEATACK:			/* �퓬�U���� */
		nMoveState = CHARMOVESTATE_MOVE;
		break;
	default:
		if (m_nMoveState == CHARMOVESTATE_MOVE) {
			return;
		}
		nMoveState = CHARMOVESTATE_MOVE;
		m_ptMove.x = m_ptMove.y = 0;
		m_dwLastTimeMove = 0;
		return;
	}

	CInfoCharCli::ChgMoveState (nMoveState);
	m_nAnime = m_nDirection;
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKCli::SetViewState							 */
/* ���e		:�\����Ԑݒ�													 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharMOVEATACKCli::SetViewState(int nViewState)
{
	CInfoCharCli::SetViewState (nViewState);

	if (m_nViewState == INFOCHARCLI_VIEWSTATE_FADEOUT) {
		m_nFadeLevel = 50;
		return;
	}
	/* �t�F�[�h�C���E�t�F�[�h�A�E�g�͖��� */
	m_dwLastTimeViewState = 0;
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKCli::GetMotionInfo							 */
/* ���e		:���[�V���������擾											 */
/* ���t		:2007/12/23														 */
/* ========================================================================= */

CInfoMotion *CInfoCharMOVEATACKCli::GetMotionInfo(int *pnCount)
{
	m_nAnime = m_nDirection;
	return CInfoCharCli::GetMotionInfo (pnCount);
}

/* Copyright(C)URARA-works 2007 */
