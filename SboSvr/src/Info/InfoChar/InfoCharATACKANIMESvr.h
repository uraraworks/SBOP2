/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharATACKANIMESvr.h									 */
/* ���e			:�L�������(�U���󂯂�ƃA�j���[�V����)�T�[�o�[�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/17													 */
/* ========================================================================= */

#pragma once

#include "InfoCharSvr.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharATACKANIMESvr : public CInfoCharSvr
{
public:
			CInfoCharATACKANIMESvr();					/* �R���X�g���N�^ */
	virtual ~CInfoCharATACKANIMESvr();					/* �f�X�g���N�^ */

	BOOL ProcHit	(CInfoCharSvr *pInfoChar);					/* ����(�U�����󂯂���) */
	BOOL TimerProc	(DWORD dwTime);								/* ���ԏ��� */


public:
} CInfoCharATACKANIMESvr, *PCInfoCharATACKANIMESvr;

/* Copyright(C)URARA-works 2007 */
