/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C�����F	WindowTEXTMSG.h												 */
/* ���e�F		�e�L�X�g���b�Z�[�W�\���E�B���h�E�N���X ��`�t�@�C��			 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2008/11/22													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowTEXTMSG : public CWindowBase
{
public:
			CWindowTEXTMSG();						/* �R���X�g���N�^ */
	virtual ~CWindowTEXTMSG();						/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);				/* �쐬 */

	void	Draw		(CImg32 *pDst);						/* �`�� */
	BOOL	TimerProc	(void);								/* ���ԏ��� */
	void	SetMsg		(LPCSTR pszMsg);					/* ���b�Z�[�W�ݒ� */


protected:
	BOOL OnX	(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL OnZ	(BOOL bDown);						/* �L�[�n���h��(Z) */


protected:
	BOOL		m_bInputWait;				/* ���͑҂� */
	POINT		m_ptDraw;					/* �`��ʒu */
	int			m_nType,					/* �E�B���h�E��� */
				m_nProcPos;					/* �������̕����ʒu */
	DWORD		m_dwLastProc;				/* �O��̏������� */
	CmyString	m_strMsg;					/* ���b�Z�[�W */
	CImg32		*m_pDibText;				/* �����`��e���|���� */
} CWindowTEXTMSG, *PCWindowTEXTMSG;

/* Copyright(C)URARA-works 2008 */
