/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowCHAR_STATUS2.h										 */
/* ���e			:�L����-�X�e�[�^�X2�E�B���h�E�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/27													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowCHAR_STATUS2 : public CWindowBase
{
public:
			CWindowCHAR_STATUS2();							/* �R���X�g���N�^ */
	virtual ~CWindowCHAR_STATUS2();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */
} CWindowCHAR_STATUS2, *PCWindowCHAR_STATUS2;

/* Copyright(C)URARA-works 2008 */
