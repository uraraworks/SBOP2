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
#include "LibInfoCharSvr.h"


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::CInfoCharBATTLE1Svr						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CInfoCharBATTLE1Svr::CInfoCharBATTLE1Svr()
{
	m_bDelete				= TRUE;
	m_dwLastTiemAtack		= 0;
	m_dwLastTimeBattleMove	= 0;
	m_pInfoCharTarget		= NULL;
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
/* �֐���	:CInfoCharBATTLE1Svr::SetTarget									 */
/* ���e		:�^�[�Q�b�g�L������ݒ�											 */
/* ���t		:2008/08/09														 */
/* ========================================================================= */

void CInfoCharBATTLE1Svr::SetTarget(CInfoCharBase *pCharTarget)
{
	CInfoCharSvr::SetTarget(pCharTarget);
	m_pInfoCharTarget = (CInfoCharSvr *)pCharTarget;
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
		m_dwLastTiemAtack = 0;
		m_dwTargetCharID = 0;
		break;
	}
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
	int nDirection;
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
	bResult = IsMoveDirection (nDirection);

	/* �i�߂�H */
	if (bResult) {
		m_nDirection = nDirection;
		GetFrontPos (ptFront, m_nDirection, TRUE);
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
	SIZE sizeDistance;

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
		dwTmp = timeGetTime () - m_dwLastTimeBattleMove;
		if ((m_dwLastTimeBattleMove != 0) && (dwTmp > 5000)) {
			/* 5�b�ȏ�ړ����Ă��Ȃ������炠����߂� */
			SetMoveState (CHARMOVESTATE_DELETE);
			goto Exit;
		}
		nMoveAverage = m_nMoveAverageBattle;
		if (nMoveAverage == 0) {
			nMoveAverage = m_nMoveAverage;
		}
		if (genrand () % 100 < 100 - nMoveAverage) {
			m_dwLastTimeMove = dwTime;
			goto Exit;
		}
	}

	GetTargetPos (&m_ptTargetPos, ptPos, 1);
	m_pLibInfoCharSvr->GetDistance (sizeDistance, this, m_pInfoCharTarget);
	xx = sizeDistance.cx;
	yy = sizeDistance.cy;
	if (xx + yy == 1) {
		ptPos.x = -1;
	}
	if (m_sizeSearchDistance.cx > 0) {
		if ((m_sizeSearchDistance.cx < xx) || (m_sizeSearchDistance.cy < yy)) {
			/* ���G�͈͊O�Ȃ̂ł�����߂� */
			SetMoveState (CHARMOVESTATE_STAND);
			goto Exit;
		}
	}

	if (xx + yy == 1) {
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
				SetMoveState (CHARMOVESTATE_DELETE);
				goto Exit;
			}
		}
		if (dwTmp >= 1000) {
			m_bAtack = TRUE;
		}
		m_dwLastTimeMove = m_dwLastTimeBattleMove = dwTime;

	} else {
		nDirection = GetDirection (ptPos.x, ptPos.y);
		bResult = IsMoveDirection (nDirection);
		if ((xx <= 1) && (yy <= 1)) {
			/* 4�����ɕϊ� */
			nDirection = GetDrawDirection (nDirection);
			if (bResult == FALSE) {
				SetMoveState (CHARMOVESTATE_STAND);
				goto Exit;
			}
		}

		/* �i�߂�H */
		if (bResult) {
			m_dwLastTimeBattleMove = dwTime;
			m_bChgMoveCount = TRUE;
			m_nDirection = nDirection;
			m_dwLastTiemAtack = 0;
		}
	}

Exit:
	return TRUE;
}

/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE1Svr::IsMoveDirection							 */
/* ���e		:�w������ɐi�߂邩�`�F�b�N										 */
/* ���t		:2009/06/14														 */
/* �߂�l	:TRUE:�i�߂�													 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::IsMoveDirection(int nDirection)
{
	int x, y;
	BOOL bResult, bRet;
	RECT rcMap;

	bRet = FALSE;

	bResult = IsEnableMove ();
	if (bResult == FALSE) {
		goto Exit;
	}

	/* �E�o�\���`�F�b�N */
	bResult = FALSE;
	GetFrontMapPosRect (rcMap, nDirection);
	if (rcMap.top < 0) {
		/* �͈͊O�ɏo�� */
		goto Exit;
	}

	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			/* �ʂ�Ȃ��ꍇ��TRUE�ɂȂ�悤���]���Ă��� */
			bResult |= !m_pInfoMap->IsMoveOut (x, y, nDirection);
		}
	}
	if (bResult) {
		goto Exit;
	}

	/* �i�߂邩�`�F�b�N */
	bResult = FALSE;
	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			/* �ʂ�Ȃ��ꍇ��TRUE�ɂȂ�悤���]���Ă��� */
			bResult |= !m_pInfoMap->IsMove (x, y, nDirection);
		}
	}
	if (bResult) {
		goto Exit;
	}

	bResult = m_pLibInfoCharSvr->IsBlockChar (this, nDirection, FALSE, TRUE);
	if (bResult) {
		goto Exit;
	}
	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2008 */
