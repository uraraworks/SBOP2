/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharSCORECli.h											 */
/* ���e			:�L�������(���_)�N���C�A���g�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/09													 */
/* ========================================================================= */

#pragma once

#include "InfoCharCli.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharSCORECli : public CInfoCharCli
{
public:
			CInfoCharSCORECli();						/* �R���X�g���N�^ */
	virtual ~CInfoCharSCORECli();						/* �f�X�g���N�^ */

	void	ChgMoveState	(int nMoveState);					/* �ړ���ԕύX */
} CInfoCharSCORECli, *PCInfoCharSCORECli;

/* Copyright(C)URARA-works 2007 */
