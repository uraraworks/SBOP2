/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:SaveLoadInfoEffect.h										 */
/* ���e			:�f�[�^�ۑ��E�ǂݍ��݃N���X(�G�t�F�N�g���) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CSaveLoadInfoEffect : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoEffect();					/* �R���X�g���N�^ */
	virtual ~CSaveLoadInfoEffect();					/* �f�X�g���N�^ */


protected:
	void	SetHeaderInfo	(PCInfoBase pInfo);		/* �w�b�_����ݒ� */
} CSaveLoadInfoEffect, *PCSaveLoadInfoEffect;

/* Copyright(C)URARA-works 2007 */
