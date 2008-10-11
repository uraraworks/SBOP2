/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharBATTLE1Svr.cpp										 */
/* ���e			:�L�������(�ړ����čU��)�T�[�o�[�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharBATTLE1Svr.h"


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::CInfoCharBATTLE1Svr						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CInfoCharBATTLE1Svr::CInfoCharBATTLE1Svr()
{
	m_dwLastTiemAtack = 0;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::~CInfoCharBATTLE1Svr						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CInfoCharBATTLE1Svr::~CInfoCharBATTLE1Svr()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::SetMoveState								 */
/* ���e		:�ړ���Ԃ�ύX													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CInfoCharBATTLE1Svr::SetMoveState(int nMoveState)
{
	CInfoCharSvr::SetMoveState (nMoveState);

	switch (nMoveState) {
	case CHARMOVESTATE_BATTLE:
	case CHARMOVESTATE_BATTLEATACK:
		m_dwLastTiemAtack = timeGetTime ();
		break;
	default:
		m_dwLastTimeMove = timeGetTime ();
		m_dwTargetCharID = 0;
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::IsLogoutDelete							 */
/* ���e		:���O�A�E�g���ɍ폜���邩����									 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::IsLogoutDelete(void)
{
	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::TimerProc									 */
/* ���e		:���ԏ���														 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::TimerProc(DWORD dwTime)
{
	BOOL bRet;
	int nMoveState;

	bRet = FALSE;

	nMoveState = m_nMoveState;
	if (IsAtackTarget () == FALSE) {
		nMoveState = -1;
	}

	switch (nMoveState) {
	case CHARMOVESTATE_STAND:			/* ���� */
		bRet |= TimerProcSTAND (dwTime);
		break;
	case CHARMOVESTATE_MOVE:			/* �ړ��� */
		bRet |= TimerProcMOVE (dwTime);
		break;
	case CHARMOVESTATE_BATTLE:			/* �퓬�� */
		bRet |= TimerProcBATTLE (dwTime);
		break;
	default:
		bRet |= CInfoCharSvr::TimerProc (dwTime);
		break;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::ProcAtack									 */
/* ���e		:����(�U��������)												 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CInfoCharBATTLE1Svr::ProcAtack(void)
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::ProcHit									 */
/* ���e		:����(�U�����󂯂���)											 */
/* ���t		:2008/07/24														 */
/* �߂�l	:TRUE:�ȍ~�̏����𑱍s����										 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::ProcHit(CInfoCharSvr *pInfoChar)
{
	int anDirection[] = {1, 0, 3, 2, 6, 7, 4, 5};
	BOOL bRet;

	bRet = CInfoCharSvr::ProcHit (pInfoChar);

	if (IsAtackTarget ()) {
		/* ������ԂōU�����󂯂��ꍇ�͐퓬���[�h�Ɉڍs���� */
		switch (m_nMoveState) {
		case CHARMOVESTATE_STAND:
		case CHARMOVESTATE_MOVE:
			m_nReserveChgMoveState = CHARMOVESTATE_BATTLE;
			break;
		}
		m_nDirection = anDirection[pInfoChar->m_nDirection];
		SetTarget (pInfoChar);
		m_bChgPos = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharSvr::ProcSWOON										 */
/* ���e		:�s������(�C��)													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::ProcSWOON(DWORD dwPara)
{
	if (m_nDropItemAverage != 0) {
		m_bDropItem = TRUE;
	}

//Todo:�b��
	m_nReserveChgEfect = 5;
	m_nReserveChgMoveState = CHARMOVESTATE_DELETE;

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::TimerProcSTAND							 */
/* ���e		:���ԏ���(����)													 */
/* ���t		:2008/07/24														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::TimerProcSTAND(DWORD dwTime)
{
	int i, nCount, nDirection;
	DWORD dwTmp;
	BOOL bRet, bResult;
	POINT ptFront;

	bRet = FALSE;

	dwTmp = dwTime - m_dwLastTimeMove;
	/* 1�^�[����100ms�ɂ��Ă��� */
	if (dwTmp < 100) {
		goto Exit;
	}

	m_dwLastTimeMove = dwTime;
	if (genrand () % 100 < 100 - m_nMoveAverage) {
		goto Exit;
	}

	if (m_pInfoMap == NULL) {
		goto Exit;
	}
	bResult = IsEnableMove ();
	if (bResult == FALSE) {
		goto Exit;
	}

	nDirection = genrand () % 4;
	/* �E�o�\���`�F�b�N */
	bResult = FALSE;
	RenewBlockMapArea (m_nMapX, m_nMapY, nDirection, TRUE);

	/* �i�߂邩�`�F�b�N */
	nCount = m_aposBockMapArea.GetSize ();
	for (i = 0; i < nCount; i ++) {
		bResult |= !m_pInfoMap->IsMoveOut (m_aposBockMapArea[i].x, m_aposBockMapArea[i].y, nDirection);
	}
	bResult = !bResult;
	if (bResult) {
		bResult = FALSE;
		GetFrontPos (ptFront, nDirection, TRUE);
		RenewBlockMapArea (ptFront.x, ptFront.y, nDirection);
		nCount = m_aposBockMapArea.GetSize ();
		for (i = 0; i < nCount; i ++) {
			bResult |= !m_pInfoMap->IsMove (m_aposBockMapArea[i].x, m_aposBockMapArea[i].y, nDirection);
		}
		bResult = !bResult;
	}

	/* �i�߂�H */
	if (bResult) {
		m_nDirection = nDirection;
		SetPos (ptFront.x, ptFront.y);

		m_bChgPos = TRUE;
		SetMoveState (CHARMOVESTATE_MOVE);
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::TimerProcMOVE								 */
/* ���e		:���ԏ���(�ړ���)												 */
/* ���t		:2008/07/24														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::TimerProcMOVE(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTmp, dwMoveWait;

	bRet = FALSE;
	if (m_nMoveState != CHARMOVESTATE_MOVE) {
		goto Exit;
	}

	dwMoveWait = GetMoveWait ();
	dwTmp = dwTime - m_dwLastTimeMove;
	/* 1�����̎��Ԃ��o�߂��Ă���H */
	if (dwTmp < dwMoveWait * 16) {
		goto Exit;
	}
	m_dwLastTimeMove = dwTime;
	SetMoveState (CHARMOVESTATE_STAND);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::TimerProcBATTLE							 */
/* ���e		:���ԏ���(�퓬��)												 */
/* ���t		:2008/07/24														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::TimerProcBATTLE(DWORD dwTime)
{
	int nDirection, xx, yy, nMoveAverage;
	BOOL bResult;
	DWORD dwTmp, dwMoveWait;
	POINT ptPos;

	if (m_dwTargetCharID == 0) {
		SetMoveState (CHARMOVESTATE_STAND);
		goto Exit;
	}

	bResult = IsEnableMove ();
	if (bResult == FALSE) {
		goto Exit;
	}
	m_bRenewTargetPos = TRUE;
	dwTmp = dwTime - m_dwLastTimeMove;
	/* 1�^�[����100ms�ɂ��Ă��� */
	if (dwTmp < 100) {
		goto Exit;
	}
	dwMoveWait = GetMoveWait ();
	dwTmp = dwTime - m_dwLastTimeMove;
	/* 1�����̎��Ԃ��o�߂��Ă���H */
	if (dwTmp < dwMoveWait * 16) {
		goto Exit;
	}

	if (m_bChgMoveCount) {
		goto Exit;
	}

	/* �O��͈ړ��H */
	if (m_dwLastTiemAtack == 0) {
		dwTmp = timeGetTime () - m_dwLastTimeMove;
		if ((m_dwLastTimeMove != 0) && (dwTmp > 5000)) {
			/* 5�b�ȏ�ړ����Ă��Ȃ������炠����߂� */
			SetMoveState (CHARMOVESTATE_STAND);
			goto Exit;
		}
		m_dwLastTimeMove = dwTime;
		nMoveAverage = m_nMoveAverageBattle;
		if (nMoveAverage == 0) {
			nMoveAverage = m_nMoveAverage;
		}
		if (genrand () % 100 < 100 - nMoveAverage) {
			goto Exit;
		}
	}

	GetTargetPos (&m_ptTargetPos, ptPos, 1);
	xx = abs (m_ptTargetPos.x - m_nMapX);
	yy = abs (m_ptTargetPos.y - m_nMapY);
	if ((xx <= 2) && (yy <= 2)) {
		nDirection = GetDirection (m_ptTargetPos.x, m_ptTargetPos.y);
		if (nDirection >= 4) {
			if ((xx <= 1) && (yy <= 1)) {
				ptPos.x = -1;
			}
		} else {
			ptPos.x = -1;
		}
	}
	if (ptPos.x < 0) {
		nDirection = GetDirection (m_ptTargetPos.x, m_ptTargetPos.y);
		/* 4�����ɕϊ� */
		nDirection = GetDrawDirection (nDirection);
		if (m_nDirection != nDirection) {
			m_nDirection = nDirection;
			m_bChgPosRenew = TRUE;
		}
		dwTmp = timeGetTime () - m_dwLastTiemAtack;
		if (m_dwLastTiemAtack != 0) {
			if (dwTmp > 5000) {
				SetMoveState (CHARMOVESTATE_STAND);
				goto Exit;
			}
		}
		if (dwTmp >= 1000) {
			m_bAtack = TRUE;
		}
		m_dwLastTimeMove = dwTime;

	} else {
		nDirection = GetDirection (ptPos.x, ptPos.y);
		if ((xx <= 1) && (yy <= 1)) {
			/* 4�����ɕϊ� */
			nDirection = GetDrawDirection (nDirection);
		}

		/* �i�߂�H */
		m_bChgMoveCount = TRUE;
		m_nDirection = nDirection;
		m_dwLastTiemAtack = 0;
	}

Exit:
	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
