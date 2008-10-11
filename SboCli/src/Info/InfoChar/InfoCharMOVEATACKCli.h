/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharMOVEATACKCli.h										 */
/* ���e			:�L�������(�ړ����čU��)�N���C�A���g�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/17													 */
/* ========================================================================= */

#pragma once

#include "InfoCharCli.h"

class CInfoMotion;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharMOVEATACKCli : public CInfoCharCli
{
public:
			CInfoCharMOVEATACKCli();					/* �R���X�g���N�^ */
	virtual ~CInfoCharMOVEATACKCli();					/* �f�X�g���N�^ */

	void	ChgMoveState	(int nMoveState);					/* �ړ���ԕύX */
	void	SetViewState	(int nViewState);					/* �\����Ԑݒ� */

	CInfoMotion *GetMotionInfo	(int *pnCount = NULL);			/* ���[�V���������擾 */
} CInfoCharMOVEATACKCli, *PCInfoCharMOVEATACKCli;

/* Copyright(C)URARA-works 2007 */
