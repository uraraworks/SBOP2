/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharMOVEATACKSvr.cpp									 */
/* ���e			:�L�������(�ړ����čU��)�T�[�o�[�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharMOVEATACKSvr.h"


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKSvr::CInfoCharMOVEATACKSvr					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

CInfoCharMOVEATACKSvr::CInfoCharMOVEATACKSvr()
{
	m_bDelete		= TRUE;
	m_nMoveCount	= 0;
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKSvr::~CInfoCharMOVEATACKSvr					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

CInfoCharMOVEATACKSvr::~CInfoCharMOVEATACKSvr()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKSvr::SetMoveState							 */
/* ���e		:�ړ���Ԃ�ύX													 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharMOVEATACKSvr::SetMoveState(int nMoveState)
{
	if (m_nMoveState == nMoveState) {
		return;
	}
	/* �퓬�� */
	if (nMoveState == CHARMOVESTATE_BATTLE) {
		nMoveState = CHARMOVESTATE_MOVE;
	}
	CInfoCharSvr::SetMoveState (nMoveState);

	m_dwLastTimeMove = timeGetTime ();
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKSvr::TimerProc								 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

BOOL CInfoCharMOVEATACKSvr::TimerProc(DWORD dwTime)
{
	int i, nCount;
	BOOL bRet, bResult;
	POINT ptFront;
	DWORD dwTmp;

	bRet = CInfoCharBase::TimerProc (dwTime);

	bResult = IsEnableAtack ();
	if (bResult == FALSE) {
		SetMoveState (CHARMOVESTATE_DELETE);
		goto Exit;
	}

	dwTmp = dwTime - m_dwLastTimeMove;
	if (dwTmp < m_dwMoveWait * 16) {
		goto Exit;
	}
//Todo:�ړ�����(�Ƃ肠�������i)
	m_dwLastTimeMove = dwTime;
	/* �E�o�\���`�F�b�N */
	bResult = FALSE;
	RenewBlockMapArea (m_nMapX, m_nMapY, m_nDirection, TRUE);

	nCount = m_aposBockMapArea.GetSize ();
	for (i = 0; i < nCount; i ++) {
		bResult |= !m_pInfoMap->IsMoveOut (m_aposBockMapArea[i].x, m_aposBockMapArea[i].y, m_nDirection);
	}
	bResult = !bResult;
	if (bResult) {
		/* �i�߂邩�`�F�b�N */
		bResult = FALSE;
		GetFrontPos (ptFront, m_nDirection, TRUE);
		RenewBlockMapArea (ptFront.x, ptFront.y, m_nDirection);
		nCount = m_aposBockMapArea.GetSize ();
		for (i = 0; i < nCount; i ++) {
			bResult |= !m_pInfoMap->IsMove (m_aposBockMapArea[i].x, m_aposBockMapArea[i].y, m_nDirection);
		}
		bResult = !bResult;
	}

	m_nMoveCount ++;
	if (m_nMoveCount >= (int)m_dwMoveCount) {
		bResult = FALSE;
	}

	/* �Ԃ���H */
	if (bResult == FALSE) {
		SetMoveState (CHARMOVESTATE_DELETE);

	} else {
		/* 2��ڈȍ~�H */
		if (m_nMoveCount > 1) {
			m_nMapX = ptFront.x;
			m_nMapY = ptFront.y;
		}
		m_bChgPos = TRUE;
		SetMoveState (CHARMOVESTATE_BATTLEATACK);
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKSvr::ProcAtack								 */
/* ���e		:����(�U��������)												 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharMOVEATACKSvr::ProcAtack(void)
{
	/* �U���ł����̂ŏI�� */
	SetMoveState (CHARMOVESTATE_DELETE);
}

/* Copyright(C)URARA-works 2007 */
