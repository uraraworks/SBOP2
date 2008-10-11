/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:StateProcBase.h											 */
/* ���e			:��ԏ������N���X ��`�t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#pragma once

class CUraraSockTCPSBO;
class CMgrData;
class CMgrSound;
class CMgrDraw;
class CMgrLayer;
class CMgrWindow;
class CMgrKeyInput;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CStateProcBase
{
public:
			CStateProcBase();							/* �R���X�g���N�^ */
	virtual ~CStateProcBase();							/* �f�X�g���N�^ */

	virtual void Create			(CMgrData *pMgrData, CUraraSockTCPSBO *pSock);	/* �쐬 */

	virtual void Init				(void);								/* ������ */
	virtual BOOL TimerProc			(void);								/* ���ԏ��� */
	virtual void KeyProc			(BYTE byCode, BOOL bDown);			/* �L�[���� */
	virtual void OnLButtonDown		(int x, int y);						/* ���b�Z�[�W�n���h��(WM_LBUTTONDOWN) */
	virtual void OnRButtonDown		(int x, int y);						/* ���b�Z�[�W�n���h��(WM_RBUTTONDOWN) */
	virtual void OnRButtonDblClk	(int x, int y);						/* ���b�Z�[�W�n���h��(WM_RBUTTONDBLCLK) */
	virtual void OnMouseMove		(int x, int y);						/* ���b�Z�[�W�n���h��(WM_MOUSEMOVE) */
	virtual void OnKeyDown			(UINT vk);							/* ���b�Z�[�W�n���h��(WM_KEYDOWN) */
	virtual void OnWindowMsg		(int nType, DWORD dwPara);			/* ���b�Z�[�W�n���h��(WM_WINDOWMSG) */
	virtual void OnMainFrame		(DWORD dwCommand, DWORD dwParam);	/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */
	virtual void OnAdminMsg			(int nType, DWORD dwPara);			/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
			void OnMgrDraw			(int nCode, DWORD dwPara);			/* ���b�Z�[�W�n���h��(WM_MGRDRAW) */


protected:
	virtual BOOL OnUp		(BOOL bDown);							/* �L�[�n���h��(��) */
	virtual BOOL OnDown		(BOOL bDown);							/* �L�[�n���h��(��) */
	virtual BOOL OnLeft		(BOOL bDown);							/* �L�[�n���h��(��) */
	virtual BOOL OnRight	(BOOL bDown);							/* �L�[�n���h��(��) */
	virtual BOOL OnX		(BOOL bDown);							/* �L�[�n���h��(X) */
	virtual BOOL OnZ		(BOOL bDown);							/* �L�[�n���h��(Z) */
	virtual BOOL OnS		(BOOL bDown);							/* �L�[�n���h��(S) */
	virtual BOOL OnA		(BOOL bDown);							/* �L�[�n���h��(A) */
	virtual BOOL OnB		(BOOL bDown);							/* �L�[�n���h��(B) */
	virtual BOOL OnR		(BOOL bDown);							/* �L�[�n���h��(R) */
	virtual BOOL OnI		(BOOL bDown);							/* �L�[�n���h��(I) */
	virtual BOOL OnN		(BOOL bDown);							/* �L�[�n���h��(N) */
	virtual BOOL OnAt		(BOOL bDown);							/* �L�[�n���h��(@) */
	virtual BOOL OnEnter	(BOOL bDown);							/* �L�[�n���h��(Enter) */
	virtual BOOL OnEscape	(BOOL bDown);							/* �L�[�n���h��(Escape) */
	virtual BOOL OnTab		(BOOL bDown);							/* �L�[�n���h��(Tab) */
	virtual BOOL OnShift	(BOOL bDown);							/* �L�[�n���h��(Shift) */
	virtual BOOL OnCtrl		(BOOL bDown);							/* �L�[�n���h��(Ctrl) */
	virtual BOOL OnSpace	(BOOL bDown);							/* �L�[�n���h��(Space) */
	virtual void OnMgrDrawSTART_FADEIN	(DWORD dwPara) {}			/* �t�F�[�h�C���J�n */
	virtual void OnMgrDrawSTART_FADEOUT	(DWORD dwPara) {}			/* �t�F�[�h�A�E�g�J�n */
	virtual void OnMgrDrawEND_FADEIN	(DWORD dwPara) {}			/* �t�F�[�h�C������ */
	virtual void OnMgrDrawEND_FADEOUT	(DWORD dwPara) {}			/* �t�F�[�h�A�E�g���� */


protected:
	HWND			m_hWndMain,							/* ���C���t���[���̃E�B���h�E�n���h�� */
					m_hWndAdmin;						/* �Ǘ��҃E�B���h�E�̃E�B���h�E�n���h�� */

	CUraraSockTCPSBO	*m_pSock;						/* �ʐM�}�l�[�W�� */
	CMgrData			*m_pMgrData;					/* �f�[�^�Ǘ� */
	CMgrSound			*m_pMgrSound;					/* �T�E���h�Ǘ� */
	CMgrDraw			*m_pMgrDraw;					/* �`��Ǘ� */
	CMgrLayer			*m_pMgrLayer;					/* ���C���[�Ǘ� */
	CMgrWindow			*m_pMgrWindow;					/* �E�B���h�E�Ǘ� */
	CMgrKeyInput		*m_pMgrKeyInput;				/* �L�[���͊Ǘ� */
} CStateProcBase, *PCStateProcBase;

/* Copyright(C)URARA-works 2006 */
