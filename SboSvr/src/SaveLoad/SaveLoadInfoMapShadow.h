/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:SaveLoadInfoMapShadow.h									 */
/* ���e			:�f�[�^�ۑ��E�ǂݍ��݃N���X(�}�b�v�e���) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CSaveLoadInfoMapShadow : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMapShadow();						/* �R���X�g���N�^ */
	virtual ~CSaveLoadInfoMapShadow();						/* �f�X�g���N�^ */


protected:
	void	SetHeaderInfo	(PCInfoBase pInfo);		/* �w�b�_����ݒ� */
} CSaveLoadInfoMapShadow, *PCSaveLoadInfoMapShadow;

/* Copyright(C)URARA-works 2007 */
