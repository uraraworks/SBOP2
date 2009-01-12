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
	m_bHitQuit			= TRUE;
	m_bDelete			= TRUE;
	m_dwLastAtackTime	= 0;
	m_dwQuitTime		= 0;
	m_dwHitEffectID		= 0;
	m_dwValue1			= 0;
	m_dwValue2			= 0;
	m_nMoveCount		= 0;
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
	DWORD dwTime;

	if (m_nMoveState == nMoveState) {
		return;
	}
	dwTime = timeGetTime ();;
	if (nMoveState == CHARMOVESTATE_BATTLEATACK) {
		m_dwLastAtackTime = dwTime;
	}
	/* �퓬�� */
	if (nMoveState == CHARMOVESTATE_BATTLE) {
		nMoveState = CHARMOVESTATE_MOVE;
	}
	CInfoCharSvr::SetMoveState (nMoveState);

	m_dwLastTimeMove = dwTime;
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKSvr::TimerProc								 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/09/17														 */
/* ========================================================================= */

BOOL CInfoCharMOVEATACKSvr::TimerProc(DWORD dwTime)
{
	int i, nCount, nState;
	BOOL bRet, bResult, bDelete;
	POINT ptFront;
	DWORD dwTmp;

	bRet = CInfoCharBase::TimerProc (dwTime);
	bDelete = FALSE;

	bResult = IsEnableAtack ();
	if (bResult == FALSE) {
		bDelete = TRUE;
	}
	if (m_dwQuitTime != 0) {
		if (dwTime >= m_dwQuitTime) {
			bDelete = TRUE;
		}
	}
	if (bDelete) {
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
		nState = CHARMOVESTATE_DELETE;
		if (m_dwQuitTime != 0) {
			if (dwTime - m_dwLastAtackTime < 1000) {
				goto Exit;
			}
			nState = CHARMOVESTATE_BATTLEATACK;
		}
		SetMoveState (nState);

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
	if (m_bHitQuit) {
		/* �U���ł����̂ŏI�� */
		SetMoveState (CHARMOVESTATE_DELETE);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKSvr::GetHitEffectID							 */
/* ���e		:�q�b�g���ɑ���ɕ\������G�t�F�N�gID���擾						 */
/* ���t		:2009/01/12														 */
/* ========================================================================= */

DWORD CInfoCharMOVEATACKSvr::GetHitEffectID(void)
{
	return m_dwHitEffectID;
}


/* ========================================================================= */
/* �֐���	:CInfoCharMOVEATACKSvr::GetDamage								 */
/* ���e		:�_���[�W�l���擾												 */
/* ���t		:2009/01/12														 */
/* ========================================================================= */

DWORD CInfoCharMOVEATACKSvr::GetDamage(void)
{
	DWORD dwRet;

	if (m_dwValue2 - m_dwValue1 == 0) {
		return 0;
	}
	dwRet = m_dwValue1 + (genrand () % (m_dwValue2 - m_dwValue1));

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
