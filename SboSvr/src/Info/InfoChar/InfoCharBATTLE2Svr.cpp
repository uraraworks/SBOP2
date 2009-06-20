/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharBATTLE2Svr.cpp										 */
/* ���e			:�L�������(�ړ����čU��)�T�[�o�[�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/07/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharBATTLE2Svr.h"
#include "LibInfoCharSvr.h"


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE2Svr::CInfoCharBATTLE2Svr						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/07/17														 */
/* ========================================================================= */

CInfoCharBATTLE2Svr::CInfoCharBATTLE2Svr()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE2Svr::~CInfoCharBATTLE2Svr						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/07/17														 */
/* ========================================================================= */

CInfoCharBATTLE2Svr::~CInfoCharBATTLE2Svr()
{
}


/* ========================================================================= */
/* �֐���	:CInfoCharBATTLE2Svr::TimerProcSTAND							 */
/* ���e		:���ԏ���(����)													 */
/* ���t		:2008/07/24														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE2Svr::TimerProcSTAND(DWORD dwTime)
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
		if (m_dwTargetCharID == 0) {
			m_bChgTargetChar = TRUE;
		} else {
			m_nReserveChgMoveState = CHARMOVESTATE_BATTLE;
			m_bChgPos = TRUE;
		}
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


#if 0
	DWORD dwTmp;
	BOOL bRet, bResult;

	bRet = FALSE;

	dwTmp = dwTime - m_dwLastTimeMove;
	/* 1�^�[����100ms�ɂ��Ă��� */
	if (dwTmp < 100) {
		goto Exit;
	}

	m_dwLastTimeMove = dwTime;
//	if (genrand () % 100 < 100 - m_nMoveAverage) {
//		goto Exit;
//	}

	if (m_pInfoMap == NULL) {
		goto Exit;
	}
	bResult = IsEnableMove ();
	if (bResult == FALSE) {
		goto Exit;
	}

	if (m_dwTargetCharID == 0) {
		m_bChgTargetChar = TRUE;
	} else {
		m_nReserveChgMoveState = CHARMOVESTATE_BATTLE;
		m_bChgPos = TRUE;
	}

	bRet = TRUE;
Exit:
	return bRet;
#endif
}

/* Copyright(C)URARA-works 2009 */
