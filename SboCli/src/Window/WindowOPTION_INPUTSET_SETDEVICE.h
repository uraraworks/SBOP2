/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowOPTION_INPUTSET_SETDEVICE.h							 */
/* ���e			:�I�v�V����-���͐ݒ�-���̓f�o�C�X�̐ݒ�E�B���h�E�N���X ��`�t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrKeyInput;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowOPTION_INPUTSET_SETDEVICE : public CWindowBase
{
public:
			CWindowOPTION_INPUTSET_SETDEVICE();				/* �R���X�g���N�^ */
	virtual ~CWindowOPTION_INPUTSET_SETDEVICE();			/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */


protected:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Z) */


protected:
	CMgrKeyInput		*m_MgrKeyInput;				/* �L�[���͊Ǘ� */
} CWindowOPTION_INPUTSET_SETDEVICE, *PCWindowOPTION_INPUTSET_SETDEVICE;

/* Copyright(C)URARA-works 2008 */
