/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MainFrame.h												 */
/* ���e			:�N���C�A���g���C���t���[�� ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#pragma once

class CMgrData;
class CUraraSockTCPSBO;
class CMgrGrpData;
class CMgrDraw;
class CMgrLayer;
class CMgrWindow;
class CMgrKeyInput;
class CMgrSound;
class CLibInfoCharCli;
class CLibInfoMapBase;
class CLibInfoMapObject;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItemType;
class CLibInfoItem;
class CLibInfoItemWeapon;
class CLibInfoEffect;
class CLibInfoMotion;
class CLibInfoMotionType;
class CLibInfoEfcBalloon;
class CLibInfoSystem;
class CLibInfoSkill;
class CStateProcBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CMainFrame
{
public:
			CMainFrame();									/* �R���X�g���N�^ */
	virtual ~CMainFrame();									/* �f�X�g���N�^ */

	int		MainLoop		(HINSTANCE hInstance);							/* ���C�����[�v */
	void	DisConnectProc	(int nID);										/* �ؒf���� */
	void	ChgMoveState	(BOOL bChgBGM);									/* �v���C���[�ړ���ԕύX���� */
	void	RenewItemArea	(void);											/* ��ʓ��̃A�C�e�������X�V */
	void	SendChat		(int nType, LPCSTR pszMsg, DWORD *pdwDst=NULL);	/* �`���b�g���� */


private:
	static LRESULT CALLBACK WndProcEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	/* ���C���E�B���h�E�v���V�[�W��(�G���g���|�C���g) */
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);							/* ���C���E�B���h�E�v���V�[�W�� */
	BOOL	OnCreate		(HWND hWnd, LPCREATESTRUCT lpCreateStruct);							/* ���b�Z�[�W�n���h��(WM_CREATE) */
	void	OnInitEnd		(HWND hWnd);														/* ���b�Z�[�W�n���h��(WM_INITEND) */
	void	OnClose			(HWND hWnd);														/* ���b�Z�[�W�n���h��(WM_CLOSE) */
	void	OnDestroy		(HWND hWnd);														/* ���b�Z�[�W�n���h��(WM_DESTROY) */
	void	OnPaint			(HWND hWnd);														/* ���b�Z�[�W�n���h��(WM_PAINT) */
	void	OnTimer			(HWND hWnd, UINT id);												/* ���b�Z�[�W�n���h��(WM_TIMER) */
	void	OnCommand		(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);					/* ���b�Z�[�W�n���h��(WM_COMMAND) */
	void	OnActivate		(HWND hWnd, UINT state, HWND hwndActDeact, BOOL fMinimized);		/* ���b�Z�[�W�n���h��(WM_ACTIVATE) */
	void	OnKeyUp			(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);			/* ���b�Z�[�W�n���h��(WM_KEYDOWN) */
	void	OnLButtonDown	(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);		/* ���b�Z�[�W�n���h��(WM_LBUTTONDOWN) */
	void	OnRButtonDown	(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);		/* ���b�Z�[�W�n���h��(WM_RBUTTONDOWN) */
	void	OnRButtonDblClk	(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);		/* ���b�Z�[�W�n���h��(WM_RBUTTONDBLCLK) */
	void	OnMouseMove		(HWND hWnd, int x, int y, UINT keyFlags);							/* ���b�Z�[�W�n���h��(WM_MOUSEMOVE) */
	void	OnMgrDraw		(int nType, DWORD dwPara);											/* ���b�Z�[�W�n���h��(WM_MGRDRAW) */
	void	OnWindowMsg		(int nType, DWORD dwPara);											/* ���b�Z�[�W�n���h��(WM_WINDOWMSG) */
	void	OnMainFrame		(DWORD dwCommand, DWORD dwParam);									/* ���b�Z�[�W�n���h��(WM_MAINFRAME) */
	void	OnAdminMsg		(int nType, DWORD dwPara);											/* ���b�Z�[�W�n���h��(WM_ADMINMSG) */
	void	OnRecvCommand	(DWORD dwMain, DWORD dwSub);										/* ���b�Z�[�W�n���h��(WM_RECVCOMMAND) */
	void	OnConnect		(void);																/* ���b�Z�[�W�n���h��(WM_URARASOCK_CONNECT) */
	void	OnDisConnect	(void);																/* ���b�Z�[�W�n���h��(WM_URARASOCK_DISCONNECT) */
	void	OnRecv			(PBYTE pData);														/* ���b�Z�[�W�n���h��(WM_URARASOCK_RECV) */

	BOOL	TimerProc		(void);								/* ���ԏ��� */
	void	KeyProc			(void);								/* �L�[���͏��� */
	void	ChgGameState	(int nGameState);					/* �Q�[����Ԃ�؂�ւ��� */
	void	Connect			(void);								/* �ڑ� */
	void	FlashMainWindow	(void);								/* �E�B���h�E��_�ł����� */
	int		GetMsgCmdType	(LPCSTR pszText);					/* ���b�Z�[�W�R�}���h��ʂ��擾 */

	/* ��M����(MainFrameRecvProcVERSION.cpp) */
	void	RecvProcVERSION						(BYTE byCmdSub, PBYTE pData);	/* �o�[�W�����n */
	void	RecvProcVERSION_RES_VERSIONCHECK	(PBYTE pData);					/* �o�[�W�����`�F�b�N���� */
	void	RecvProcVERSION_RES_FILELISTCHECK	(PBYTE pData);					/* �t�@�C�����X�g�`�F�b�N���� */

	/* ��M����(MainFrameRecvProcCONNECT.cpp) */
	void	RecvProcCONNECT						(BYTE byCmdSub, PBYTE pData);	/* �ڑ��n */
	void	RecvProcCONNECT_RES_LOGIN			(PBYTE pData);					/* ���O�C������ */
	void	RecvProcCONNECT_RES_PLAY			(PBYTE pData);					/* �Q�[���J�n���� */

	/* ��M����(MainFrameRecvProcACCOUNT.cpp) */
	void	RecvProcACCOUNT						(BYTE byCmdSub, PBYTE pData);	/* �A�J�E���g�n */
	void	RecvProcACCOUNT_RES_ACCOUNTINFO		(PBYTE pData);					/* �A�J�E���g��񉞓� */
	void	RecvProcACCOUNT_RES_MAKECHAR		(PBYTE pData);					/* �L�����쐬���� */

	/* ��M����(MainFrameRecvProcCHAR.cpp) */
	void	RecvProcCHAR						(BYTE byCmdSub, PBYTE pData);	/* �L�����n */
	void	RecvProcCHAR_RES_CHARINFO			(PBYTE pData);					/* �L������񉞓� */
	void	RecvProcCHAR_CHARINFO				(PBYTE pData);					/* �L�������ʒm */
	void	RecvProcCHAR_MOVEPOS				(PBYTE pData);					/* �ړ��ʒm */
	void	RecvProcCHAR_STATE					(PBYTE pData);					/* ��Ԓʒm */
	void	RecvProcCHAR_PROCSTATE				(PBYTE pData);					/* �s����Ԓʒm */
	void	RecvProcCHAR_CHAT					(PBYTE pData);					/* �`���b�g�ʒm */
	void	RecvProcCHAR_CHARID					(PBYTE pData);					/* �L����ID�ʒm */
	void	RecvProcCHAR_RES_PUTGET				(PBYTE pData);					/* �A�C�e�����E���u������ */
	void	RecvProcCHAR_SET_EFFECT				(PBYTE pData);					/* �G�t�F�N�g���w�� */
	void	RecvProcCHAR_ITEMINFO				(PBYTE pData);					/* �A�C�e�����ʒm */
	void	RecvProcCHAR_RES_TAIL				(PBYTE pData);					/* �t���čs������ */
	void	RecvProcCHAR_MODIFY_PARAM			(PBYTE pData);					/* �p�����[�^�ύX�ʒm */
	void	RecvProcCHAR_GRP					(PBYTE pData);					/* �摜�ʒm */
	void	RecvProcCHAR_MOTION					(PBYTE pData);					/* ���[�V�������ʒm */
	void	RecvProcCHAR_MOTIONTYPE				(PBYTE pData);					/* ���[�V������ʏ��ʒm */
	void	RecvProcCHAR_SET_EFCBALLOON			(PBYTE pData);					/* ���o�����w�� */
	void	RecvProcCHAR_SET_MOTION				(PBYTE pData);					/* ���[�V�������w�� */
	void	RecvProcCHAR_RES_CHECKMAPEVENT		(PBYTE pData);					/* �}�b�v�C�x���g�`�F�b�N���� */
	void	RecvProcCHAR_STATUS					(PBYTE pData);					/* �X�e�[�^�X���ʒm */
	void	RecvProcCHAR_TEXTEFFECT				(PBYTE pData);					/* �����G�t�F�N�g�ʒm */
	void	RecvProcCHAR_STATE_CHARGE			(PBYTE pData);					/* ���ߏ�Ԓʒm */
	void	RecvProcCHAR_RES_TALKEVENT			(PBYTE pData);					/* ��b�C�x���g��񉞓� */
	void	RecvProcCHAR_SKILLINFO				(PBYTE pData);					/* �X�L�����ʒm */

	/* ��M����(MainFrameRecvProcMAP.cpp) */
	void	RecvProcMAP							(BYTE byCmdSub, PBYTE pData);	/* �}�b�v�n */
	void	RecvProcMAP_RES_MAPINFO				(PBYTE pData);					/* �}�b�v��񉞓� */
	void	RecvProcMAP_ONLINE					(PBYTE pData);					/* �I�����C�����ʒm */
	void	RecvProcMAP_SYSTEMMSG				(PBYTE pData);					/* �V�X�e�����b�Z�[�W�ʒm */
	void	RecvProcMAP_FORMATMSG				(PBYTE pData);					/* �t�H�[�}�b�g���b�Z�[�W�ʒm */
	void	RecvProcMAP_MAPOBJECT				(PBYTE pData);					/* �}�b�v�I�u�W�F�N�g���ʒm */
	void	RecvProcMAP_MAPOBJECTDATA			(PBYTE pData);					/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^�ʒm */
	void	RecvProcMAP_DELETEMAPOBJECTDATA		(PBYTE pData);					/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^�폜 */
	void	RecvProcMAP_MAPPARTS				(PBYTE pData);					/* �}�b�v�p�[�c���ʒm */
	void	RecvProcMAP_SETPARTS				(PBYTE pData);					/* �}�b�v�p�[�c�z�u */
	void	RecvProcMAP_RENEWMAPSIZE			(PBYTE pData);					/* �}�b�v�T�C�Y�X�V */
	void	RecvProcMAP_DELETEPARTS				(PBYTE pData);					/* �}�b�v�p�[�c�폜 */
	void	RecvProcMAP_MAPSHADOW				(PBYTE pData);					/* �}�b�v�e���ʒm */
	void	RecvProcMAP_SETMAPSHADOW			(PBYTE pData);					/* �}�b�v�e�z�u */
	void	RecvProcMAP_DELETEMAPSHADOW			(PBYTE pData);					/* �}�b�v�e�폜 */
	void	RecvProcMAP_MAPNAME					(PBYTE pData);					/* �}�b�v���ʒm */
	void	RecvProcMAP_MAPEVENT				(PBYTE pData);					/* �}�b�v�C�x���g���ʒm */
	void	RecvProcMAP_DELETEEVENT				(PBYTE pData);					/* �}�b�v�C�x���g���폜 */
	void	RecvProcMAP_FADEINOUT				(PBYTE pData);					/* �t�F�[�h�C��/�A�E�g�ʒm */

	/* ��M����(MainFrameRecvProcITEM.cpp) */
	void	RecvProcITEM						(BYTE byCmdSub, PBYTE pData);	/* �A�C�e���n */
	void	RecvProcITEM_RES_ITEMINFO			(PBYTE pData);					/* �A�C�e����񉞓� */
	void	RecvProcITEM_ITEMINFO				(PBYTE pData);					/* �A�C�e�����ʒm */
	void	RecvProcITEM_DELETEITEMINFO			(PBYTE pData);					/* �A�C�e�����폜 */
	void	RecvProcITEM_RES_ITEMTYPEINFO		(PBYTE pData);					/* �A�C�e����ʏ�񉞓� */
	void	RecvProcITEM_ITEMTYPEINFO			(PBYTE pData);					/* �A�C�e����ʏ��ʒm */
	void	RecvProcITEM_DELETEITEMTYPEINFO		(PBYTE pData);					/* �A�C�e����ʏ��폜 */
	void	RecvProcITEM_RES_ITEMWEAPONINFO		(PBYTE pData);					/* �����񉞓� */
	void	RecvProcITEM_ITEMWEAPONINFO			(PBYTE pData);					/* ������ʒm */

	/* ��M����(MainFrameRecvProcADMIN.cpp) */
	void	RecvProcADMIN						(BYTE byCmdSub, PBYTE pData);	/* �Ǘ��Ҍn */
	void	RecvProcADMIN_RES_ADMINLEVEL		(PBYTE pData);					/* �Ǘ��҃��x������ */
	void	RecvProcADMIN_RENEWADMINLEVEL		(PBYTE pData);					/* �Ǘ��҃��x���X�V */
	void	RecvProcADMIN_PLAYSOUND				(PBYTE pData);					/* ���ʉ��̍Đ� */
	void	RecvProcADMIN_CHAR_RES_ACCOUNT		(PBYTE pData);					/* �A�J�E���g��񉞓� */
	void	RecvProcADMIN_CHAR_RES_ONLINE		(PBYTE pData);					/* �I�����C�����L�����ꗗ���� */

	/* ��M����(MainFrameRecvProcEFFECT.cpp) */
	void	RecvProcEFFECT						(BYTE byCmdSub, PBYTE pData);	/* �G�t�F�N�g�n */
	void	RecvProcEFFECT_RES_EFFECTINFO		(PBYTE pData);					/* �G�t�F�N�g��񉞓� */
	void	RecvProcEFFECT_EFFECTINFO			(PBYTE pData);					/* �G�t�F�N�g���ʒm */
	void	RecvProcEFFECT_DELETEEFFECTINFO		(PBYTE pData);					/* �G�t�F�N�g���폜 */
	void	RecvProcEFFECT_BALLOONINFO			(PBYTE pData);					/* ���o�����ʒm */

	/* ��M����(MainFrameRecvProcSYSTEM.cpp) */
	void	RecvProcSYSTEM						(BYTE byCmdSub, PBYTE pData);	/* �V�X�e���n */
	void	RecvProcSYSTEM_INFO					(PBYTE pData);					/* �V�X�e�����ʒm */

	/* ��M����(MainFrameRecvProcSKILL.cpp) */
	void	RecvProcSKILL						(BYTE byCmdSub, PBYTE pData);	/* �X�L���n */
	void	RecvProcSKILL_SKILLINFO				(PBYTE pData);					/* �X�L�����ʒm */


private:
	HWND				m_hWnd;					/* �E�B���h�E�n���h�� */
	BOOL				m_bWindowActive,		/* �E�B���h�E�̃A�N�e�B�u��� */
						m_bRenewCharInfo;		/* �L�������X�V */
	int					m_nGameState,			/* �Q�[����� */
						m_nDrawCount,			/* �b�ԕ\���� */
						m_nFPS;					/* 1�t���[���̎��� */
	DWORD				m_dwLastTimeCheck,		/* �O��̃`�F�b�N���� */
						m_dwDrawTime;			/* �`��Ɏg�������� */
	HRESULT				m_hCom;
	SYSTEMTIME			m_stSystemTime;			/* �O��̃`�F�b�N���� */

	CMgrData			*m_pMgrData;			/* �f�[�^�}�l�[�W�� */
	CUraraSockTCPSBO	*m_pSock;				/* �ʐM�}�l�[�W�� */
	CMgrGrpData			*m_pMgrGrpData;			/* �O���t�B�b�N�f�[�^�}�l�[�W�� */
	CMgrDraw			*m_pMgrDraw;			/* �`��}�l�[�W�� */
	CMgrLayer			*m_pMgrLayer;			/* ���C���[�}�l�[�W�� */
	CMgrWindow			*m_pMgrWindow;			/* �E�B���h�E�}�l�[�W�� */
	CMgrKeyInput		*m_pMgrKeyInput;		/* �L�[���̓}�l�[�W�� */
	CMgrSound			*m_pMgrSound;			/* �T�E���h�Ǘ� */
	CLibInfoCharCli		*m_pLibInfoChar;		/* �L������񃉃C�u���� */
	CLibInfoMapBase		*m_pLibInfoMap;			/* �}�b�v��񃉃C�u���� */
	CLibInfoMapObject	*m_pLibInfoMapObject;	/* �}�b�v�I�u�W�F�N�g��񃉃C�u���� */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* �}�b�v�p�[�c��񃉃C�u���� */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;	/* �}�b�v�e��񃉃C�u���� */
	CLibInfoItemType	*m_pLibInfoItemType;	/* �A�C�e����ʏ�񃉃C�u���� */
	CLibInfoItem		*m_pLibInfoItem;		/* �A�C�e����񃉃C�u���� */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	/* �����񃉃C�u���� */
	CLibInfoEffect		*m_pLibInfoEffect;		/* �G�t�F�N�g��񃉃C�u���� */
	CLibInfoMotion		*m_pLibInfoMotion;		/* ���[�V������񃉃C�u���� */
	CLibInfoMotionType	*m_pLibInfoMotionType;	/* ���[�V������ʏ�񃉃C�u���� */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;	/* ���o����񃉃C�u���� */
	CLibInfoSystem		*m_pLibInfoSystem;		/* �V�X�e����񃉃C�u���� */
	CLibInfoSkill		*m_pLibInfoSkill;		/* �X�L����񃉃C�u���� */
	CStateProcBase		*m_pStateProc;			/* ��ԏ��� */
};

/* Copyright(C)URARA-works 2005 */
