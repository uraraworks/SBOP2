/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrWindow.h												 */
/* ���e			:�E�B���h�E�}�l�[�W���N���X ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/03													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;
class CMgrKeyInput;
class CInfoTalkEvent;

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

/* �E�B���h�EID */
enum {
	WINDOWTYPE_NONE = 0,
	WINDOWTYPE_LOGIN,						/* ���O�C�� */
	WINDOWTYPE_MSG,							/* ���b�Z�[�W�\�� */
	WINDOWTYPE_LOGINMENU,					/* ���j���[ */
	WINDOWTYPE_CHARNAME,					/* �L���������� */
	WINDOWTYPE_SEX,							/* ���� */
	WINDOWTYPE_MAKECHAR,					/* �L�����쐬 */
	WINDOWTYPE_ACCOUNTINFO,					/* �A�J�E���g��� */
	WINDOWTYPE_STATUS,						/* ���� */
	WINDOWTYPE_CHAT,						/* �`���b�g���� */
	WINDOWTYPE_FAMILYTYPE,					/* �푰�I�� */
	WINDOWTYPE_HAIRTYPE,					/* ���^�I�� */
	WINDOWTYPE_HAIRCOLOR,					/* ���F�I�� */
	WINDOWTYPE_EYECOLOR,					/* �ڐF�I�� */
	WINDOWTYPE_STYLESELECT,					/* �e�p�I�� */
	WINDOWTYPE_NAMEINPUT,					/* ���O���� */
	WINDOWTYPE_SYSTEMMENU,					/* �V�X�e�����j���[ */
	WINDOWTYPE_SETCOLOR,					/* ���O�Ɣ����F�̐ݒ� */
	WINDOWTYPE_SETBGMVOLUME,				/* BGM���ʂ̐ݒ� */
	WINDOWTYPE_SETSOUNDVOLUME,				/* ���ʉ����ʂ̐ݒ� */
	WINDOWTYPE_SETDRAWMODE,					/* �\�����e�̐ݒ� */
	WINDOWTYPE_ITEMMENU,					/* �A�C�e�����j���[ */
	WINDOWTYPE_ITEMMENU_SELECT,				/* �A�C�e���ǂ����邩���j���[ */
	WINDOWTYPE_OPTION,						/* �I�v�V�������j���[ */
	WINDOWTYPE_OPTION_VIEWSET,				/* �I�v�V����-�\���ݒ胁�j���[ */
	WINDOWTYPE_OPTION_VOLUMESET,			/* �I�v�V����-���ʐݒ胁�j���[ */
	WINDOWTYPE_OPTION_INPUTSET,				/* �I�v�V����-���͐ݒ胁�j���[ */
	WINDOWTYPE_OPTION_TASKBAR,				/* �I�v�V�������j���[(�������Ƀ`�J�`�J) */
	WINDOWTYPE_OPTION_INPUTSET_SETDEVICE,	/* �I�v�V����-���͐ݒ�-���̓f�o�C�X�̐ݒ� */
	WINDOWTYPE_OPTION_ACTIONSET,			/* �I�v�V����-����ݒ胁�j���[ */
	WINDOWTYPE_OPTION_ACTIONSET_SLEEPTIMER,	/* �I�v�V����-����ݒ�-���Ђ�˃^�C�}�[�̐ݒ� */
	WINDOWTYPE_COMMANDMENU,					/* �R�}���h���j���[ */
	WINDOWTYPE_CHAR_STATUS,					/* �L����-�X�e�[�^�X */
	WINDOWTYPE_CHAR_STATUS2,				/* �L����-�X�e�[�^�X2 */
	WINDOWTYPE_CHAR_STATUS3,				/* �L����-�X�e�[�^�X3 */
	WINDOWTYPE_TEXTMSG,						/* �e�L�X�g���b�Z�[�W */
	WINDOWTYPE_SWOON,						/* �C�⃁�j���[ */
	WINDOWTYPE_SKILLMENU,					/* �X�L�����j���[ */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CMgrWindow
{
public:
			CMgrWindow();								/* �R���X�g���N�^ */
	virtual ~CMgrWindow();								/* �f�X�g���N�^ */

	void		Create		(CMgrData *pMgrData);				/* �쐬 */
	void		Destroy		(void);								/* �j�� */

	void		Update		(void);								/* �S�ẴE�B���h�E���X�V */
	void		Draw		(CImg32 *pDst);						/* �`�� */
	BOOL		TimerProc	(void);								/* ���ԏ��� */
	void		KeyProc		(BYTE byEvent, BOOL bDown);			/* �L�[���� */
	void		DeleteAll	(void);								/* �S�ẴE�B���h�E��j�� */
	void		Delete		(int nID);							/* �w��ID�̃E�B���h�E���폜 */
	CWindowBase	*GetWindow	(int nID);							/* �w��ID�̃E�B���h�E���擾 */
	BOOL		IsKeyInput	(void);								/* �L�[�����̕K�v�����邩���� */

	/* �E�B���h�E�쐬 */
	void MakeWindowLOGIN			(void);						/* ���O�C����� */
	void MakeWindowMSG				(LPCSTR pszMsg, DWORD dwTime = 0, int nType = 0);	/* ���b�Z�[�W�\�� */
	void MakeWindowLOGINMENU		(void);						/* ���j���[ */
	void MakeWindowCHARNAME			(void);						/* �L���������� */
	void MakeWindowSEX				(void);						/* ���� */
	void MakeWindowACCOUNTINFO		(void);						/* �A�J�E���g��� */
	void MakeWindowSTATUS			(BOOL bShow = TRUE);		/* ���� */
	void MakeWindowCHAT				(void);						/* �`���b�g���� */
	void MakeWindowFAMILYTYPE		(void);						/* �푰�I�� */
	void MakeWindowHAIRTYPE			(void);						/* ���^�I�� */
	void MakeWindowHAIRCOLOR		(void);						/* ���F�I�� */
	void MakeWindowEYECOLOR			(void);						/* �ڐF�I�� */
	void MakeWindowSTYLESELECT		(void);						/* �e�p�I�� */
	void MakeWindowNAMEINPUT		(void);						/* ���O���� */
	void MakeWindowSYSTEMMENU		(void);						/* �V�X�e�����j���[ */
	void MakeWindowSETCOLOR			(void);						/* ���O�Ɣ����F�̐ݒ� */
	void MakeWindowSETBGMVOLUME		(void);						/* BGM���ʂ̐ݒ� */
	void MakeWindowSETSOUNDVOLUME	(void);						/* ���ʉ����ʂ̐ݒ� */
	void MakeWindowSETDRAWMODE		(void);						/* �\�����e�̐ݒ� */
	void MakeWindowITEMMENU			(void);						/* �A�C�e�����j���[ */
	void MakeWindowITEMMENU_SELECT	(int nPos, DWORD dwItemID);	/* �A�C�e���ǂ����邩���j���[ */
	void MakeWindowOPTION			(void);						/* �I�v�V���� */
	void MakeWindowOPTION_VIEWSET	(void);						/* �I�v�V����-�\���ݒ� */
	void MakeWindowOPTION_VOLUMESET	(void);						/* �I�v�V����-���ʐݒ� */
	void MakeWindowOPTION_INPUTSET	(void);						/* �I�v�V����-���͐ݒ� */
	void MakeWindowOPTION_TASKBAR	(void);						/* �I�v�V����(�������Ƀ^�X�N�o�[�`�J�`�J) */
	void MakeWindowOPTION_INPUTSET_SETDEVICE	(void);			/* �I�v�V����-���͐ݒ�-���̓f�o�C�X�̐ݒ� */
	void MakeWindowOPTION_ACTIONSET	(void);						/* �I�v�V����-����ݒ� */
	void MakeWindowOPTION_ACTIONSET_SLEEPTIMER	(void);			/* �I�v�V����-����ݒ�-���Ђ�˃^�C�}�[�̐ݒ� */
	void MakeWindowCOMMANDMENU		(void);						/* �R�}���h���j���[ */
	void MakeWindowCHAR_STATUS		(void);						/* �L����-�X�e�[�^�X */
	void MakeWindowTEXTMSG			(LPCSTR pszTitle, LPCSTR pszName, LPCSTR pszMsg);			/* �e�L�X�g���b�Z�[�W */
	void MakeWindowTEXTMSG			(LPCSTR pszTitle, LPCSTR pszName, CInfoTalkEvent *pInfo);	/* �e�L�X�g���b�Z�[�W */
	void MakeWindowSWOON			(void);						/* �C�⃁�j���[ */
	void MakeWindowSKILLMENU		(int nType = 0);			/* �X�L�����j���[ */


private:
	void SetActive				(void);								/* �A�N�e�B�u�ȃE�B���h�E�̐ݒ� */


private:
	BOOL				m_bDraw,						/* �`��t���O */
						m_bKeyInput;					/* �L�[�����̕K�v�����邩 */
	PARRAYWINDOWBASE	m_paWindow;						/* �E�B���h�E��� */
	CMgrData			*m_pMgrData;					/* �f�[�^�Ǘ� */
	CMgrGrpData			*m_pMgrGrpData;					/* �O���t�B�b�N�f�[�^�Ǘ� */
	CMgrKeyInput		*m_pMgrKeyInput;				/* �L�[���͊Ǘ� */
} CMgrWindow, *PCMgrWindow;

/* Copyright(C)URARA-works 2006 */
