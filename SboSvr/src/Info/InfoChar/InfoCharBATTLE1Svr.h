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

	void SetMoveState	(int nMoveState);						/* �ړ���Ԃ�ύX */
	BOOL IsLogoutDelete	(void);									/* ���O�A�E�g���ɍ폜���邩���� */
	BOOL TimerProc		(DWORD dwTime);							/* ���ԏ��� */
	void ProcAtack		(void);									/* ����(�U��������) */
	BOOL ProcHit		(CInfoCharSvr *pInfoChar);				/* ����(�U�����󂯂���) */
	BOOL ProcSWOON		(DWORD dwPara);							/* �s������(�C��) */


protected:
	BOOL TimerProcSTAND	(DWORD dwTime);							/* ���ԏ���(����) */
	BOOL TimerProcMOVE	(DWORD dwTime);							/* ���ԏ���(�ړ���) */
	BOOL TimerProcBATTLE(DWORD dwTime);							/* ���ԏ���(�퓬��) */


public:
	DWORD	m_dwLastTiemAtack;				/* �Ō�ɍU���������� */
} CInfoCharBATTLE1Svr, *PCInfoCharBATTLE1Svr;

/* Copyright(C)URARA-works 2008 */
