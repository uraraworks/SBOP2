/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharMOVEATACKSvr.h										 */
/* ���e			:�L�������(�ړ����čU��)�T�[�o�[�N���X ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/17													 */
/* ========================================================================= */

#pragma once

#include "InfoCharSvr.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharMOVEATACKSvr : public CInfoCharSvr
{
public:
			CInfoCharMOVEATACKSvr();					/* �R���X�g���N�^ */
	virtual ~CInfoCharMOVEATACKSvr();					/* �f�X�g���N�^ */

	void SetMoveState	(int nMoveState);						/* �ړ���Ԃ�ύX */
	BOOL IsLogoutDelete	(void);									/* ���O�A�E�g���ɍ폜���邩���� */
	BOOL TimerProc		(DWORD dwTime);							/* ���ԏ��� */
	void ProcAtack		(void);									/* ����(�U��������) */


public:
	int		m_nMoveCount;					/* �ړ��������� */
} CInfoCharMOVEATACKSvr, *PCInfoCharMOVEATACKSvr;

/* Copyright(C)URARA-works 2007 */
