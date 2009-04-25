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

	void  SetMoveState	(int nMoveState);						/* �ړ���Ԃ�ύX */
	BOOL  TimerProc		(DWORD dwTime);							/* ���ԏ��� */
	void  ProcAtack		(void);									/* ����(�U��������) */
	DWORD GetHitEffectID(void);									/* �q�b�g���ɑ���ɕ\������G�t�F�N�gID���擾 */
	DWORD GetDamage		(void);									/* �_���[�W�l���擾 */


public:
	BOOL	m_bHitQuit,						/* �q�b�g����Ə��� */
			m_bDistanceDelete;				/* �˒������܂ōs���Ə����� */
	DWORD	m_dwLastAtackTime,				/* �Ō�ɍU���������� */
			m_dwQuitTime,					/* �I������ */
			m_dwHitEffectID,				/* �q�b�g���ɑ���ɕ\������G�t�F�N�gID */
			m_dwValue1,						/* ����1 */
			m_dwValue2;						/* ����2 */
	int		m_nMoveCount;					/* �ړ��������� */
} CInfoCharMOVEATACKSvr, *PCInfoCharMOVEATACKSvr;

/* Copyright(C)URARA-works 2007 */
