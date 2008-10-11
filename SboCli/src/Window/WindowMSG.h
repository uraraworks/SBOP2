/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C�����F	WindowMSG.h													 */
/* ���e�F		���b�Z�[�W�\���E�B���h�E�N���X ��`�t�@�C��					 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2006/11/03													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowMSG : public CWindowBase
{
public:
			CWindowMSG();									/* �R���X�g���N�^ */
	virtual ~CWindowMSG();									/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */

	void	Draw		(CImg32 *pDst);								/* �`�� */
	BOOL	TimerProc	(void);										/* ���ԏ��� */
	void	SetMsg		(LPCSTR pszMsg, DWORD dwTime, int nType=0);				/* ���b�Z�[�W�ݒ� */


private:
	int			m_nType;					/* �E�B���h�E��� */
	DWORD		m_dwDeleteTime;				/* �폜�܂ł̎��� */
	CmyString	m_strMsg;					/* ���b�Z�[�W */
} CWindowMSG, *PCWindowMSG;

/* Copyright(C)URARA-works 2006 */
