/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:WindowLOGINMENU.h											 */
/* ���e			:���O�C�����j���[�E�B���h�E�N���X ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowLOGINMENU : public CWindowBase
{
public:
			CWindowLOGINMENU();								/* �R���X�g���N�^ */
	virtual ~CWindowLOGINMENU();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Update		(void);										/* �X�V */
	void	Draw		(CImg32 *pDst);								/* �`�� */
	BOOL	TimerProc	(void);										/* ���ԏ��� */
	void	SetEnable	(int nNo, BOOL bEnable);					/* ���j���[���ڂ̓��͏�Ԑݒ� */


private:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */


private:
	BOOL	m_abEnable[4];
} CWindowLOGINMENU, *PCWindowLOGINMENU;

/* Copyright(C)URARA-works 2006 */
