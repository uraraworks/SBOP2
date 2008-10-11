/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowEYECOLOR.h											 */
/* ���e			:�ڐF�I���E�B���h�E�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/08													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowEYECOLOR : public CWindowBase
{
public:
			CWindowEYECOLOR();								/* �R���X�g���N�^ */
	virtual ~CWindowEYECOLOR();								/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */
	BOOL	TimerProc	(void);										/* ���ԏ��� */
	void	SetType		(WORD wEyeColor);							/* �ڐF��ݒ� */
	WORD	GetType		(void);										/* �ڐF���擾 */


private:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Y) */


private:
	int		m_nPosX;								/* ���̑I���ʒu */
} CWindowEYECOLOR, *PCWindowEYECOLOR;

/* Copyright(C)URARA-works 2007 */
