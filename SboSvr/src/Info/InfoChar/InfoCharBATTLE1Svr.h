/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharBATTLE1Svr.h										 */
/* ���e			:�L�������(�퓬1)�T�[�o�[�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#pragma once

#include "InfoCharSvr.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharBATTLE1Svr : public CInfoCharSvr
{
public:
			CInfoCharBATTLE1Svr();						/* �R���X�g���N�^ */
	virtual ~CInfoCharBATTLE1Svr();						/* �f�X�g���N�^ */

	virtual void SetTarget		(CInfoCharBase *pCharTarget);	/* �^�[�Q�b�g�L������ݒ� */
	virtual void SetMoveState	(int nMoveState);				/* �ړ���Ԃ�ύX */
	virtual BOOL TimerProc		(DWORD dwTime);					/* ���ԏ��� */
	virtual void ProcAtack		(void);							/* ����(�U��������) */
	virtual BOOL ProcHit		(CInfoCharSvr *pInfoChar);		/* ����(�U�����󂯂���) */
	virtual BOOL ProcSWOON		(DWORD dwPara);					/* �s������(�C��) */


protected:
	virtual BOOL TimerProcSTAND	(DWORD dwTime);				/* ���ԏ���(����) */
	virtual BOOL TimerProcMOVE	(DWORD dwTime);				/* ���ԏ���(�ړ���) */
	virtual BOOL TimerProcBATTLE(DWORD dwTime);				/* ���ԏ���(�퓬��) */
	virtual BOOL IsMoveDirection(int nDirection);			/* �w������ɐi�߂邩�`�F�b�N */


public:
	DWORD			m_dwLastTiemAtack;		/* �Ō�ɍU���������� */
	DWORD			m_dwLastTimeBattleMove;	/* �퓬���[�h���Ɉړ��������� */
	CInfoCharSvr	*m_pInfoCharTarget;		/* �^�[�Q�b�g���̃L������� */
} CInfoCharBATTLE1Svr, *PCInfoCharBATTLE1Svr;

/* Copyright(C)URARA-works 2008 */
