/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoCharBALLCli.h											 */
/* ���e			:�L�������(�{�[��)�N���C�A���g�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/02													 */
/* ========================================================================= */

#pragma once

#include "InfoCharCli.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CInfoCharBALLCli : public CInfoCharCli
{
public:
			CInfoCharBALLCli();							/* �R���X�g���N�^ */
	virtual ~CInfoCharBALLCli();						/* �f�X�g���N�^ */

	void		ChgMoveState	(int nMoveState);				/* �ړ���ԕύX */
} CInfoCharBALLCli, *PCInfoCharBALLCli;

/* Copyright(C)URARA-works 2007 */
