/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowNAMEINPUT.h											 */
/* ���e			:���O���̓E�B���h�E�N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/10													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrWindow;
class CMgrDraw;
class CWindowCHARNAME;
class CInfoCharCli;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowNAMEINPUT : public CWindowBase
{
public:
			CWindowNAMEINPUT();								/* �R���X�g���N�^ */
	virtual ~CWindowNAMEINPUT();							/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	OnWindowMsg	(int nType, DWORD dwPara);					/* ���b�Z�[�W�n���h��(WM_WINDOWMSG) */
	void	Draw		(CImg32 *pDst);								/* �`�� */
	BOOL	TimerProc	(void);										/* ���ԏ��� */


private:
	BOOL	OnUp		(void);								/* �L�[�n���h��(��) */
	BOOL	OnDown		(void);								/* �L�[�n���h��(��) */
	BOOL	OnLeft		(void);								/* �L�[�n���h��(��) */
	BOOL	OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL	OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL	OnZ			(BOOL bDown);						/* �L�[�n���h��(Y) */


public:
	CMgrWindow			*m_pMgrWindow;				/* �E�B���h�E�}�l�[�W�� */
	CMgrDraw			*m_pMgrDraw;				/* �`��}�l�[�W�� */
	CWindowCHARNAME		*m_pWindowCHARNAME;			/* �L���������̓E�B���h�E */
	CInfoCharCli		*m_pInfoCharCli;			/* �쐬���̃L������� */
} CWindowNAMEINPUT, *PCWindowNAMEINPUT;

/* Copyright(C)URARA-works 2007 */
