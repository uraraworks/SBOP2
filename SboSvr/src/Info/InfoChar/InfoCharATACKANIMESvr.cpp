/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharATACKANIMESvr.cpp									 */
/* ���e			:�L�������(�U���󂯂�ƃA�j���[�V����)�T�[�o�[�N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharATACKANIMESvr.h"


/* ========================================================================= */
/* �֐���	:CInfoCharATACKANIMESvr::CInfoCharATACKANIMESvr					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

CInfoCharATACKANIMESvr::CInfoCharATACKANIMESvr()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharATACKANIMESvr::~CInfoCharATACKANIMESvr				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

CInfoCharATACKANIMESvr::~CInfoCharATACKANIMESvr()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharATACKANIMESvr::ProcHit								 */
/* ���e		:����(�U�����󂯂���)											 */
/* ���t		:2007/09/17														 */
/* �߂�l	:TRUE:�ȍ~�̏����𑱍s����										 */
/* ========================================================================= */

BOOL CInfoCharATACKANIMESvr::ProcHit(CInfoCharSvr *pInfoChar)
{
//Todo:�b��
	if (m_wGrpIDNPC != 10) {
//		m_nReserveChgEfect = 1;
//		goto Exit;
	}

	SetMoveState (CHARMOVESTATE_ANIME);

//Exit:
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

BOOL CInfoCharATACKANIMESvr::TimerProc(DWORD dwTime)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_bChgMoveState) {
		goto Exit;
	}
	if (m_nMoveState == CHARMOVESTATE_ANIME) {
		m_nMoveState = CHARMOVESTATE_STAND;
	}

Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2007 */
