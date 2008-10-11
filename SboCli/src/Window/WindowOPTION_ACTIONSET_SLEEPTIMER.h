/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowOPTION_ACTIONSET_SLEEPTIMER.h						 */
/* ���e			:�I�v�V����-����ݒ�-���Ђ�˃^�C�}�[�̐ݒ�E�B���h�E�N���X ��`�t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/21													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowOPTION_ACTIONSET_SLEEPTIMER : public CWindowBase
{
public:
			CWindowOPTION_ACTIONSET_SLEEPTIMER();			/* �R���X�g���N�^ */
	virtual ~CWindowOPTION_ACTIONSET_SLEEPTIMER();			/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */


private:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Z) */


protected:
	int		m_nSleepTimer;		/* ���Ђ�˃^�C�}�[ */
} CWindowOPTION_ACTIONSET_SLEEPTIMER, *PCWindowOPTION_ACTIONSET_SLEEPTIMER;

/* Copyright(C)URARA-works 2008 */
