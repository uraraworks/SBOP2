/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:SaveLoadInfoMapParts.h										 */
/* ���e			:�f�[�^�ۑ��E�ǂݍ��݃N���X(�}�b�v�p�[�c���) ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/30													 */
/* ========================================================================= */

#pragma once

#include "SaveLoadInfoBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CSaveLoadInfoMapParts : public CSaveLoadInfoBase
{
public:
			CSaveLoadInfoMapParts();						/* �R���X�g���N�^ */
	virtual ~CSaveLoadInfoMapParts();						/* �f�X�g���N�^ */


protected:
	void	SetHeaderInfo	(PCInfoBase pInfo);		/* �w�b�_����ݒ� */
} CSaveLoadInfoMapParts, *PCSaveLoadInfoMapParts;

/* Copyright(C)URARA-works 2007 */
