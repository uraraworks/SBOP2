/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C�����F	WindowSEX.h													 */
/* ���e�F		���ʃE�B���h�E�N���X ��`�t�@�C��							 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2006/11/14													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowSEX : public CWindowBase
{
public:
			CWindowSEX();									/* �R���X�g���N�^ */
	virtual ~CWindowSEX();									/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */
	BOOL	TimerProc	(void);										/* ���ԏ��� */
	void	SetType		(int nSex);									/* ���ʂ�ݒ� */
	int		GetType		(void);										/* ���ʂ��擾 */


private:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Y) */


private:
	int		m_nPos;									/* �I���ʒu */
} CWindowSEX, *PCWindowSEX;

/* Copyright(C)URARA-works 2006 */
