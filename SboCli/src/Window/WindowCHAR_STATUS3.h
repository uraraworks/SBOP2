/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowCHAR_STATUS3.h										 */
/* ���e			:�L����-�X�e�[�^�X3�E�B���h�E�N���X ��`�t�@�C��			 */
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

typedef class CWindowCHAR_STATUS3 : public CWindowBase
{
public:
			CWindowCHAR_STATUS3();							/* �R���X�g���N�^ */
	virtual ~CWindowCHAR_STATUS3();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */
} CWindowCHAR_STATUS3, *PCWindowCHAR_STATUS3;

/* Copyright(C)URARA-works 2008 */
