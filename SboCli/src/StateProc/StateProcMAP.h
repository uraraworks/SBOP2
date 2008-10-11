/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:StateProcMAP.h												 */
/* ���e			:��ԏ����N���X(�}�b�v���) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#pragma once

#include "StateProcBase.h"

class CInfoCharCli;
class CInfoMapBase;
class CLibInfoCharCli;
class CLibInfoMapBase;
class CLibInfoItem;
class CDlgMsgLog;
class CAdminWindow;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CStateProcMAP : public CStateProcBase
{
public:
			CStateProcMAP();										/* �R���X�g���N�^ */
	virtual ~CStateProcMAP();										/* �f�X�g���N�^ */

	void Create				(CMgrData *pMgrData, CUraraSockTCPSBO *pSock);	/* �쐬 */
	void Init				(void);											/* ������ */
	void GetMsgLogRect		(RECT &rcDst);									/* ���b�Z�[�W���O�E�B���h�E�̋�`���擾 */
	BOOL TimerProc			(void);											/* ���ԏ��� */
	void KeyProc			(BYTE byCode, BOOL bDown);						/* �L�[���� */
	void OnWindowMsg		(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_WINDOWMSG) */
	void OnLButtonDown		(int x, int y);									/* ���b�Z�[�W�n���h��(WM_LBUTTONDOWN) */
	void OnRButtonDown		(int x, int y);									/* ���b�Z�[�W�n���h��(WM_RBUTTONDOWN) */
	void OnRButtonDblClk	(int x, int y);									/* ���b�Z�[�W�n���h��(WM_RBUTTONDBLCLK) */
	void OnMouseMove		(int x, int y);									/* ���b�Z�[�W�n���h��(WM_MOUSEMOVE) */
	void OnMainFrame		(DWORD dwCommand, DWORD dwParam);				/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */
	void OnAdminMsg			(int nType, DWORD dwPara);						/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */


protected:
	void	TimerProcSleepTimer			(void);											/* ���ԏ���(���Ђ�˃^�C�}�[) */
	void	TimerProcChargeAtack		(void);											/* ���ԏ���(���ߍU��) */
	void	TimerProcGauge				(void);											/* ���ԏ���(�Q�[�W��) */
	BOOL	IsKeyInputEnable			(void);											/* �L�[���͂������邩���� */
	BOOL	OnUp						(BOOL bDown);									/* �L�[�n���h��(��) */
	BOOL	OnDown						(BOOL bDown);									/* �L�[�n���h��(��) */
	BOOL	OnLeft						(BOOL bDown);									/* �L�[�n���h��(��) */
	BOOL	OnRight						(BOOL bDown);									/* �L�[�n���h��(��) */
	BOOL	OnX							(BOOL bDown);									/* �L�[�n���h��(X) */
	BOOL	OnZ							(BOOL bDown);									/* �L�[�n���h��(Z) */
	BOOL	OnS							(BOOL bDown);									/* �L�[�n���h��(S) */
	BOOL	OnR							(BOOL bDown);									/* �L�[�n���h��(R) */
	BOOL	OnB							(BOOL bDown);									/* �L�[�n���h��(B) */
	BOOL	OnN							(BOOL bDown);									/* �L�[�n���h��(N) */
	BOOL	OnAt						(BOOL bDown);									/* �L�[�n���h��(@) */
	BOOL	OnEnter						(BOOL bDown);									/* �L�[�n���h��(Enter) */
	BOOL	OnEscape					(BOOL bDown);									/* �L�[�n���h��(Escape) */
	BOOL	OnTab						(BOOL bDown);									/* �L�[�n���h��(Tab) */
	BOOL	OnShift						(BOOL bDown);									/* �L�[�n���h��(Shift) */
	BOOL	OnCtrl						(BOOL bDown);									/* �L�[�n���h��(Ctrl) */
	BOOL	OnSpace						(BOOL bDown);									/* �L�[�n���h��(Space) */
	void	OnMgrDrawSTART_FADEIN		(DWORD dwPara);									/* �t�F�[�h�C���J�n */
	BOOL	MoveProc					(int x, int y, int xx, int yy, int nDirection);	/* �ړ����� */
	BOOL	OnWindowMsgCHAT				(DWORD dwPara);									/* �`���b�g���� */
	BOOL	OnWindowMsgSYSTEMMENU		(DWORD dwPara);									/* �V�X�e�����j���[ */
	BOOL	OnWindowMsgSETCOLOR			(DWORD dwPara);									/* ���O�Ɣ����F�̐ݒ� */
	BOOL	OnWindowMsgSETDRAWMODE		(DWORD dwPara);									/* �\�����e�̐ݒ� */
	BOOL	OnWindowMsgITEMMENU			(DWORD dwPara);									/* �A�C�e�����j���[ */
	BOOL	OnWindowMsgITEMMENU_SELECT	(DWORD dwPara);									/* �A�C�e���ǂ����邩���j���[ */
	BOOL	OnWindowMsgOPTION			(DWORD dwPara);									/* �I�v�V���� */
	BOOL	OnWindowMsgOPTION_VIEWSET	(DWORD dwPara);									/* �I�v�V����-�\���ݒ� */
	BOOL	OnWindowMsgOPTION_INPUTSET	(DWORD dwPara);									/* �I�v�V����-���͐ݒ� */
	BOOL	OnWindowMsgOPTION_TASKBAR	(DWORD dwPara);									/* �I�v�V����(�������Ƀ^�X�N�o�[�`�J�`�J) */
	BOOL	OnWindowMsgOPTION_INPUTSET_SETDEVICE	(DWORD dwPara);						/* �I�v�V����-���͐ݒ�-���̓f�o�C�X�̐ݒ� */
	BOOL	OnWindowMsgOPTION_ACTIONSET	(DWORD dwPara);									/* �I�v�V����-����ݒ� */
	BOOL	OnWindowMsgOPTION_ACTIONSET_SLEEPTIMER	(DWORD dwPara);						/* �I�v�V����-����ݒ�-���Ђ�˃^�C�}�[�̐ݒ� */
	BOOL	OnWindowMsgCOMMANDMENU		(DWORD dwPara);									/* �R�}���h���j���[ */
	void	OnMainFrameRENEWITEMINFO	(DWORD dwItemID);								/* ���b�Z�[�W�n���h��(WM_MAINFRAME)[�A�C�e�����X�V] */
	BOOL	OnXChar						(DWORD dwCharID);								/* �L������X�L�[�����������̏��� */
	void	AddSystemMsg				(BOOL bAddLog, LPCSTR pszMsg, COLORREF cl);		/* �V�X�e�����b�Z�[�W��ǉ� */
	void	DefenseOff					(void);											/* �h����� */


protected:
	BOOL			m_bChgScrollMode,					/* �X�N���[�����[�h�̕ύX�ɓ���邩�`�F�b�N */
					m_bSendCheckMapEvent;				/* �}�b�v�C�x���g�`�F�b�N�𑗐M���������� */
	int				m_nScrollMode;						/* �X�N���[�����[�h */
	DWORD			m_dwLastTimeMove,					/* �Ō�Ɉړ��������� */
					m_dwLastTimeKeepAlive,				/* �Ō�ɐ����m�F�ʒm�𑗐M�������� */
					m_dwLastTimeGauge,					/* �Q�[�W�񕜎��� */
					m_dwLastBalloonID,					/* �Ō�Ɏg�������o��ID */
					m_dwLastKeyInput,					/* �Ō�ɃL�[���͂������� */
					m_dwStartChargeTime;				/* ���ߊJ�n���� */

	CAdminWindow	*m_pAdminWindow;					/* �Ǘ��҃E�B���h�E */
	CInfoCharCli	*m_pPlayerChar;						/* ���쒆�̃L������� */
	CInfoMapBase	*m_pMap;							/* �\�����̃}�b�v��� */

	CLibInfoCharCli	*m_pLibInfoChar;					/* �L������񃉃C�u���� */
	CLibInfoMapBase	*m_pLibInfoMap;						/* �}�b�v��񃉃C�u���� */
	CLibInfoItem	*m_pLibInfoItem;					/* �A�C�e����񃉃C�u���� */
	CDlgMsgLog		*m_pDlgMsgLog;						/* ���b�Z�[�W���O�E�B���h�E */
} CStateProcMAP, *PCStateProcMAP;

/* Copyright(C)URARA-works 2006 */
