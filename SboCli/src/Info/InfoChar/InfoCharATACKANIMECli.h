/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharATACKANIMECli.h									 */
/* ���e			:�L�������(�U���󂯂�ƃA�j���[�V����)�N���C�A���g�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/10													 */
/* ========================================================================= */

#pragma once

#include "InfoCharCli.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharATACKANIMECli : public CInfoCharCli
{
public:
			CInfoCharATACKANIMECli();					/* �R���X�g���N�^ */
	virtual ~CInfoCharATACKANIMECli();					/* �f�X�g���N�^ */

	void	ChgMoveState	(int nMoveState);					/* �ړ���ԕύX */


protected:
	BOOL RenewAnime			(DWORD dwTime, int nAdd = 1);		/* �A�j���[�V�����̍X�V */
} CInfoCharATACKANIMECli, *PCInfoCharATACKANIMECli;

/* Copyright(C)URARA-works 2007 */
