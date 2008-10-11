/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowFAMILYTYPE.h											 */
/* ���e			:�푰�I���E�B���h�E�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/07													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowFAMILYTYPE : public CWindowBase
{
public:
			CWindowFAMILYTYPE();							/* �R���X�g���N�^ */
	virtual ~CWindowFAMILYTYPE();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */
	BOOL	TimerProc	(void);										/* ���ԏ��� */
	WORD	GetType		(void);										/* �푰���擾 */


private:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Y) */


private:
	int		m_nPos;									/* �I���ʒu */
} CWindowFAMILYTYPE, *PCWindowFAMILYTYPE;

/* Copyright(C)URARA-works 2007 */
