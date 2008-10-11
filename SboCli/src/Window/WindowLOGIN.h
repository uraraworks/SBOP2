/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:WindowLOGIN.h												 */
/* ���e			:���O�C�����E�B���h�E�N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/03													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CWindowLOGIN : public CWindowBase
{
public:
			CWindowLOGIN();									/* �R���X�g���N�^ */
	virtual ~CWindowLOGIN();								/* �f�X�g���N�^ */

	void	Create		(CMgrData *pMgrData);						/* �쐬 */

	void	Draw		(CImg32 *pDst);								/* �`�� */
	void	Enable		(BOOL bEnable);								/* ���͋��E�֎~ */
	void	SetShow		(BOOL bShow);								/* �\�����邩�ݒ� */
	void	Save		(void);										/* �A�J�E���g�ƃp�X���[�h��ۑ� */
	void	SetCheck	(BOOL bCheck);								/* �p�X���[�h��ۑ�����`�F�b�N�̐ݒ� */
	BOOL	GetCheck	(void);										/* �p�X���[�h��ۑ�����`�F�b�N�̎擾 */
	LPCSTR	GetAccount	(void)	{ return m_strAccount;	}			/* ���͂��ꂽ�A�J�E���g���擾 */
	LPCSTR	GetPassword	(void)	{ return m_strPassword;	}			/* ���͂��ꂽ�p�X���[�h���擾 */


private:
	void	MakeWindow	(void);										/* �E�B���h�E�쐬 */
	static LRESULT CALLBACK AccountWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* �A�J�E���g���͗��v���V�[�W�� */
	static LRESULT CALLBACK PasswordWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* �p�X���[�h���͗��v���V�[�W�� */
	static LRESULT CALLBACK SavePasswordWndProc	(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* �p�X���[�h���L�^����`�F�b�N�v���V�[�W�� */
	static LRESULT CALLBACK ConnectWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* �ڑ��{�^���v���V�[�W�� */
	void	OnConnect	(void);										/* �ڑ��{�^���n���h�� */


public:
	CmyString	m_strAccount,				/* ���͂��ꂽ�A�J�E���g */
				m_strPassword;				/* ���͂��ꂽ�p�X���[�h */


private:
	HWND	m_hWndAccount,					/* �A�J�E���g���͗� */
			m_hWndPassword,					/* �p�X���[�h���͗� */
			m_hWndSavePassword,				/* �p�X���[�h���L�^����`�F�b�N */
			m_hWndConnect;					/* �ڑ��{�^�� */

	WNDPROC	m_OrgWndProcAccount,			/* �A�J�E���g���͗��̌��E�B���h�E�v���V�[�W�� */
			m_OrgWndProcPassword,			/* �p�X���[�h���͗��̌��E�B���h�E�v���V�[�W�� */
			m_OrgWndProcSavePassword,		/* �p�X���[�h���L�^����`�F�b�N�̌��E�B���h�E�v���V�[�W�� */
			m_OrgWndProcConnect;			/* �ڑ��{�^���̌��E�B���h�E�v���V�[�W�� */
} CWindowLOGIN, *PCWindowLOGIN;

/* Copyright(C)URARA-works 2006 */
