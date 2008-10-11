/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowSYSTEMMENU.h											 */
/* ���e			:�V�X�e�����j���[�E�B���h�E�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/19													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowSYSTEMMENU : public CWindowBase
{
public:
			CWindowSYSTEMMENU();							/* �R���X�g���N�^ */
	virtual ~CWindowSYSTEMMENU();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */


private:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Z) */
	BOOL	OnEscape	(BOOL bDown);						/* �L�[�n���h��(Escape) */
} CWindowSYSTEMMENU, *PCWindowSYSTEMMENU;

/* Copyright(C)URARA-works 2007 */
