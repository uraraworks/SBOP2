/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowOPTION_VIEWSET.h										 */
/* ���e			:�I�v�V����-�\���ݒ�E�B���h�E�N���X ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/27													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowOPTION_VIEWSET : public CWindowBase
{
public:
			CWindowOPTION_VIEWSET();						/* �R���X�g���N�^ */
	virtual ~CWindowOPTION_VIEWSET();						/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */
	BOOL	TimerProc	(void);										/* ���ԏ��� */


protected:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Z) */
	BOOL	GetCheck	(int nNo);							/* �`�F�b�N��Ԃ��擾 */


protected:
	ARRAYINT	m_anCheck;			/* �`�F�b�N�{�b�N�X�`��p */
	ARRAYDWORD	m_adwCheckTime;		/* �`�F�b�N�{�b�N�X�`��p */
} CWindowOPTION_VIEWSET, *PCWindowOPTION_VIEWSET;

/* Copyright(C)URARA-works 2008 */
