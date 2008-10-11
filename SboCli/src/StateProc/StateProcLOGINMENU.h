/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:StateProcLOGINMENU.h										 */
/* ���e			:��ԏ����N���X(���j���[���) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"

class CInfoAccount;
class CWindowLOGINMENU;
class CWindowSTATUS;
class CWindowSTYLESELECT;
class CWindowNAMEINPUT;
class CInfoCharCli;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CStateProcLOGINMENU : public CStateProcBase
{
public:
			CStateProcLOGINMENU();						/* �R���X�g���N�^ */
	virtual ~CStateProcLOGINMENU();						/* �f�X�g���N�^ */

	void Init			(void);									/* ������ */
	void OnWindowMsg	(int nType, DWORD dwPara);				/* ���b�Z�[�W�n���h��(WM_WINDOWMSG) */
	void OnMainFrame	(DWORD dwCommand, DWORD dwParam);		/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */


protected:
	BOOL	OnLeft							(BOOL bDown);			/* �L�[�n���h��(��) */
	BOOL	OnRight							(BOOL bDown);			/* �L�[�n���h��(��) */
	BOOL	OnX								(BOOL bDown);			/* �L�[�n���h��(X) */
	BOOL	OnZ								(BOOL bDown);			/* �L�[�n���h��(Z) */
	void	OnMgrDrawEND_FADEOUT			(DWORD dwPara);			/* �t�F�[�h�A�E�g���� */
	void	OnWindowMsgLOGINMENU			(DWORD dwPara);			/* ���j���[�E�B���h�E����̒ʒm */
	void	OnWindowMsgFAMILYTYPE			(DWORD dwPara);			/* �푰�I���E�B���h�E����̒ʒm */
	void	OnWindowMsgSTYLESELECT			(DWORD dwPara);			/* �e�p�I���E�B���h�E����̒ʒm */
	void	OnWindowMsgNAMEINPUT			(DWORD dwPara);			/* ���O���̓E�B���h�E����̒ʒm */


protected:
	DWORD	m_dwLastTimeKey;					/* �O��̃L�[�������� */
	int		m_nCountKeyRepeate,					/* �L�[���s�[�g�� */
			m_nSelect,							/* �I���ʒu */
			m_nState;							/* ��� */

	CInfoAccount		*m_pInfoAccount;		/* �A�J�E���g��� */
	CWindowLOGINMENU	*m_pWindowLOGINMENU;	/* ���O�C�����j���[�E�B���h�E */
	CWindowSTATUS		*m_pWindowSTATUS;		/* �X�e�[�^�X�E�B���h�E */
	CWindowSTYLESELECT	*m_pWindowSTYLESELECT;	/* �e�p�I���E�B���h�E */
	CWindowNAMEINPUT	*m_pWindowNAMEINPUT;	/* ���O���� */

	CInfoCharCli		*m_pInfoCharCli;		/* �쐬���̃L������� */
} CStateProcLOGINMENU, *PCStateProcLOGINMENU;

/* Copyright(C)URARA-works 2006 */
