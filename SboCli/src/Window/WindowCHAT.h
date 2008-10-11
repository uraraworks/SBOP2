/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C�����F	WindowCHAT.h												 */
/* ���e�F		�`���b�g�E�B���h�E�N���X ��`�t�@�C��						 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2007/02/03													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowCHAT : public CWindowBase
{
public:
			CWindowCHAT();									/* �R���X�g���N�^ */
	virtual ~CWindowCHAT();									/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */
	void	Draw		(CImg32 *pDst);								/* �`�� */
	void	SetActive	(BOOL bActive);								/* �A�N�e�B�u���ݒ� */
	int		GetType		(void);										/* �`���b�g��ʂ��擾 */


private:
	void	MakeWindow	(void);										/* �E�B���h�E�쐬 */
	static LRESULT CALLBACK ChatWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* �`���b�g���͗��v���V�[�W�� */


public:
	CmyString	m_strChat;					/* ���͂��ꂽ������ */


private:
	int		m_nType;						/* �`���b�g��� */
	BOOL	m_bPushEnter,					/* Enter�L�[������� */
			m_bPushEsc;						/* ESC�L�[������� */
	HWND	m_hWndChat;						/* �`���b�g���͗� */

	CmyArray<int, int> m_aArrayType;		/* �`���b�g��� */

	WNDPROC	m_OrgWndProcChat;				/* �`���b�g���͗��̌��E�B���h�E�v���V�[�W�� */
} CWindowCHAT, *PCWindowCHAT;

/* Copyright(C)URARA-works 2007 */
