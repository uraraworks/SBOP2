/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowCHAR_STATUS.h										 */
/* ���e			:�L����-�X�e�[�^�X�E�B���h�E�N���X ��`�t�@�C��				 */
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

typedef class CWindowCHAR_STATUS : public CWindowBase
{
public:
			CWindowCHAR_STATUS();							/* �R���X�g���N�^ */
	virtual ~CWindowCHAR_STATUS();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */


private:
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Z) */
	BOOL	OnJ			(BOOL bDown);						/* �L�[�n���h��(J) */
} CWindowCHAR_STATUS, *PCWindowCHAR_STATUS;

/* Copyright(C)URARA-works 2008 */
