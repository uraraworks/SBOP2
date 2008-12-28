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
class CInfoTalkEvent;

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
	void	SetTitle	(LPCSTR pszTitle);					/* ��������ݒ� */
	void	SetName		(LPCSTR pszName);					/* ���O��ݒ� */
	void	SetMsg		(LPCSTR pszMsg);					/* ���b�Z�[�W�ݒ� */
	void	SetTalkEvent(CInfoTalkEvent *pInfo);			/* ��b�C�x���g�ݒ� */


protected:
	BOOL OnUp			(void);								/* �L�[�n���h��(��) */
	BOOL OnDown			(void);								/* �L�[�n���h��(��) */
	BOOL OnLeft			(void);								/* �L�[�n���h��(��) */
	BOOL OnRight		(void);								/* �L�[�n���h��(��) */
	BOOL OnX			(BOOL bDown);						/* �L�[�n���h��(X) */
	BOOL OnZ			(BOOL bDown);						/* �L�[�n���h��(Z) */
	void DrawChar		(LPCSTR pszText);					/* �P�����\�� */
	void RenewTitle		(void);								/* �����Ɩ��O�摜���X�V */
	void InitText		(void);								/* ���b�Z�[�W�摜�������� */
	void MsgProc		(void);								/* ���b�Z�[�W��͏��� */
	void GetLineText	(LPCSTR pszSrc, CmyString &strDst);	/* 1�s�擾 */
	void TrimSpace		(CmyString &strSrc);				/* �O��̋󔒂����� */
	void GetBlock		(LPCSTR pszSrc, CmyString &strDst);	/* �傩����1�u���b�N���擾 */


protected:
	BOOL		m_bInputWait;				/* ���͑҂� */
	POINT		m_ptDraw;					/* �`��ʒu */
	int			m_nState,					/* �\����� */
				m_nType,					/* �E�B���h�E��� */
				m_nProcPos,					/* �������̕����ʒu */
				m_nProcPosTmp,				/* �������̕����ʒu */
				m_nProcEventPage,			/* �������C�x���g�y�[�W */
				m_nProcEventNo,				/* �������C�x���g�ԍ� */
				m_nSpaceHeight;				/* �E�B���h�E��̗]���̍��� */
	DWORD		m_dwLastProc;				/* �O��̏������� */
	CmyString	m_strTitle,					/* ������ */
				m_strName,					/* ���O */
				m_strMsg,					/* ���b�Z�[�W */
				m_strMsgTmp;				/* ���b�Z�[�W */
	CmyStringArray	m_astrMenu;				/* ���j���[���� */
	CImg32		*m_pDibTitle,				/* �������Ɩ��O���� */
				*m_pDibText;				/* �����`��e���|���� */
	CInfoTalkEvent	*m_pInfoTalkEvent;		/* �������̉�b�C�x���g */
} CWindowTEXTMSG, *PCWindowTEXTMSG;

/* Copyright(C)URARA-works 2008 */
