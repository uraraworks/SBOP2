/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MainFrame.h												 */
/* ���e			:�T�[�o�[���C���t���[�� ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/04													 */
/* ========================================================================= */

#pragma once

class CPacketBase;
class CMgrData;
class CUraraSockTCPSBO;
class CTextOutput;
class CUpdateServerInfo;
class CLibInfoAccount;
class CLibInfoCharSvr;
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
class CInfoCharBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CMainFrame
{
public:
			CMainFrame();											/* �R���X�g���N�^ */
	virtual ~CMainFrame();											/* �f�X�g���N�^ */

	int		MainLoop	(HINSTANCE hInstance);								/* ���C�����[�v */

	/* ���M���� */
	void	SendToScreenChar	(CInfoCharBase *pInfoChar, CPacketBase *pPacket);	/* �w��L�����Ɠ�����ʂ̃L�����֑��M */
	void	SendToMapChar		(DWORD dwMapID, CPacketBase *pPacket);				/* �w��}�b�v�ɂ���L�����֑��M */
	void	SendToAdminChar		(CPacketBase *pPacket);								/* �Ǘ��Ҍ����������Ă���L�����֑��M */
	void	SendToClient		(DWORD dwSessionID, CPacketBase *pPacket);			/* �Z�b�V����ID���L���ȏꍇ�̂ݑ��M */


private:
	static	LRESULT CALLBACK WndProcEntry	(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	/* ���C���E�B���h�E�v���V�[�W��(�G���g���|�C���g) */
			LRESULT WndProc					(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);	/* ���C���E�B���h�E�v���V�[�W�� */
	BOOL	OnCreate			(HWND hWnd, LPCREATESTRUCT lpCreateStruct);			/* ���b�Z�[�W�n���h��(WM_CREATE) */
	void	OnClose				(HWND hWnd);										/* ���b�Z�[�W�n���h��(WM_CLOSE) */
	void	OnDestroy			(HWND hWnd);										/* ���b�Z�[�W�n���h��(WM_DESTROY) */
	void	OnPaint				(HWND hWnd);										/* ���b�Z�[�W�n���h��(WM_PAINT) */
	void	OnTimer				(HWND hWnd, UINT id);								/* ���b�Z�[�W�n���h��(WM_TIMER) */
	void	OnCommand			(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);	/* ���b�Z�[�W�n���h��(WM_COMMAND) */
	void	OnAddClient			(DWORD dwSessionID);								/* ���b�Z�[�W�n���h��(WM_URARASOCK_ADDCLIENT) */
	void	OnDecClient			(DWORD dwSessionID);								/* ���b�Z�[�W�n���h��(WM_URARASOCK_DECCLIENT) */
	void	OnRecv				(PBYTE pData, DWORD dwSessionID);					/* ���b�Z�[�W�n���h��(WM_URARASOCK_RECV) */
	void	OnDisconnect		(DWORD dwSessionID);								/* ���b�Z�[�W�n���h��(WM_DISCONNECT) */
	void	TimerProc			(void);												/* ���ԏ��� */
	void	TimerProcKeepalive	(void);												/* ���ԏ���(�����m�F�`�F�b�N) */
	void	MyTextOut			(HDC hDC, int x, int y, LPCSTR pStr);				/* ������`�� */
	void	UpdateServerInfo	(BOOL bSend = TRUE, BOOL bUpload = FALSE);			/* �T�[�o�[�����X�V */

	void	OnCommandACCOUNT_DELETEALL	(void);									/* ���j���[�n���h��(�S�A�J�E���g���폜) */
	void	OnCommandUPDATE_RENEW		(void);									/* ���j���[�n���h��(�A�b�v�f�[�g�t�@�C���X�V) */

	/* ��M����(MainFrameRecvProcVERSION.cpp) */
	void	RecvProcVERSION						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* �o�[�W�����n */
	void	RecvProcVERSION_REQ_VERSIONCHECK	(PBYTE pData, DWORD dwSessionID);					/* �o�[�W�����`�F�b�N�v�� */
	void	RecvProcVERSION_REQ_FILELISTCHECK	(PBYTE pData, DWORD dwSessionID);					/* �t�@�C�����X�g�`�F�b�N�v�� */
	void	RecvProcVERSION_REQ_FILELIST		(PBYTE pData, DWORD dwSessionID);					/* �t�@�C�����X�g�v�� */
	void	RecvProcVERSION_REQ_FILE			(PBYTE pData, DWORD dwSessionID);					/* �t�@�C���v�� */

	/* ��M����(MainFrameRecvProcCONNECT.cpp) */
	void	RecvProcCONNECT						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* �ڑ��n */
	void	RecvProcCONNECT_REQ_LOGIN			(PBYTE pData, DWORD dwSessionID);					/* ���O�C���v�� */
	void	RecvProcCONNECT_REQ_PLAY			(PBYTE pData, DWORD dwSessionID);					/* �Q�[���J�n�v�� */
	void	RecvProcCONNECT_KEEPALIVE			(PBYTE pData, DWORD dwSessionID);					/* �����m�F�ʒm */

	/* ��M����(MainFrameRecvProcACCOUNT.cpp) */
	void	RecvProcACCOUNT						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* �A�J�E���g�n */
	void	RecvProcACCOUNT_REQ_ACCOUNTINFO		(PBYTE pData, DWORD dwSessionID);					/* �A�J�E���g���v�� */
	void	RecvProcACCOUNT_REQ_MAKECHAR		(PBYTE pData, DWORD dwSessionID);					/* �L�����쐬�v�� */

	/* ��M����(MainFrameRecvProcCHAR.cpp) */
	void	RecvProcCHAR						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* �L�����n */
	void	RecvProcCHAR_REQ_CHARINFO			(PBYTE pData, DWORD dwSessionID);					/* �L�������v�� */
	void	RecvProcCHAR_MOVEPOS				(PBYTE pData, DWORD dwSessionID);					/* �ړ��ʒm */
	void	RecvProcCHAR_STATE					(PBYTE pData, DWORD dwSessionID);					/* ��Ԓʒm */
	void	RecvProcCHAR_REQ_CHAT				(PBYTE pData, DWORD dwSessionID);					/* �`���b�g�v�� */
	void	RecvProcCHAR_REQ_CHARINFO2			(PBYTE pData, DWORD dwSessionID);					/* �L�������v��(����) */
	void	RecvProcCHAR_REQ_PUTGET				(PBYTE pData, DWORD dwSessionID);					/* �A�C�e�����E���u���v�� */
	void	RecvProcCHAR_REQ_USEITEM			(PBYTE pData, DWORD dwSessionID);					/* �A�C�e�����g���v�� */
	void	RecvProcCHAR_REQ_DRAGITEM			(PBYTE pData, DWORD dwSessionID);					/* �A�C�e���ʒu�ύX�v�� */
	void	RecvProcCHAR_REQ_PUSH				(PBYTE pData, DWORD dwSessionID);					/* �����v�� */
	void	RecvProcCHAR_REQ_TAIL				(PBYTE pData, DWORD dwSessionID);					/* �t���čs���v�� */
	void	RecvProcCHAR_REQ_MODIFY_PARAM		(PBYTE pData, DWORD dwSessionID);					/* �p�����[�^�ύX�v�� */
	void	RecvProcCHAR_REQ_EQUIP				(PBYTE pData, DWORD dwSessionID);					/* �����v�� */
	void	RecvProcCHAR_PROC_FISHING			(PBYTE pData, DWORD dwSessionID);					/* �ނ�v�� */
	void	RecvProcCHAR_REQ_CHECKMAPEVENT		(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�C�x���g�`�F�b�N�v�� */
	void	RecvProcCHAR_STATE_CHARGE			(PBYTE pData, DWORD dwSessionID);					/* ���ߏ�Ԓʒm */
	void	RecvProcCHAR_REQ_RECOVERY			(PBYTE pData, DWORD dwSessionID);					/* �C��㕜���v�� */

	/* ��M����(MainFrameRecvProcMAP.cpp) */
	void	RecvProcMAP							(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* �}�b�v�n */
	void	RecvProcMAP_REQ_MAPINFO				(PBYTE pData, DWORD dwSessionID);					/* �}�b�v���v�� */

	/* ��M����(MainFrameRecvProcITEM.cpp) */
	void	RecvProcITEM						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* �A�C�e���n */
	void	RecvProcITEM_REQ_ITEMINFO			(PBYTE pData, DWORD dwSessionID);					/* �A�C�e�����v�� */
	void	RecvProcITEM_RENEWITEMINFO			(PBYTE pData, DWORD dwSessionID);					/* �A�C�e�����X�V */
	void	RecvProcITEM_REQ_ITEMTYPEINFO		(PBYTE pData, DWORD dwSessionID);					/* �A�C�e����ʏ��v�� */
	void	RecvProcITEM_RENEWITEMTYPEINFO		(PBYTE pData, DWORD dwSessionID);					/* �A�C�e����ʏ��X�V */
	void	RecvProcITEM_REQ_ITEMWEAPONINFO		(PBYTE pData, DWORD dwSessionID);					/* ������v�� */

	/* ��M����(MainFrameRecvProcADMIN.cpp) */
	void	RecvProcADMIN						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* �Ǘ��Ҍn */
	void	RecvProcADMIN_CHARINFO				(PBYTE pData, DWORD dwSessionID);					/* �L�������ʒm */
	void	RecvProcADMIN_DELETECHARINFO		(PBYTE pData, DWORD dwSessionID);					/* �L�������폜 */
	void	RecvProcADMIN_MAP_RENEWMAPOBJECT	(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�I�u�W�F�N�g�X�V */
	void	RecvProcADMIN_MAP_RENEWOBJECTDATA	(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^�X�V */
	void	RecvProcADMIN_MAP_DELETEOBJECTDATA	(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�I�u�W�F�N�g�z�u�f�[�^�폜 */
	void	RecvProcADMIN_RENEWMAPPARTS			(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�p�[�c�X�V */
	void	RecvProcADMIN_MAP_SETPARTS			(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�p�[�c�z�u */
	void	RecvProcADMIN_MAP_RENEWMAPSIZE		(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�T�C�Y�X�V */
	void	RecvProcADMIN_MAP_DELETEPARTS		(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�p�[�c�폜 */
	void	RecvProcADMIN_MAP_COPYPARTS			(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�p�[�c�R�s�[ */
	void	RecvProcADMIN_MAP_SETMAPNAME		(PBYTE pData, DWORD dwSessionID);					/* �}�b�v���ύX */
	void	RecvProcADMIN_RENEWMAPSHADOW		(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�e�X�V */
	void	RecvProcADMIN_MAP_SETMAPSHADOW		(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�e�z�u */
	void	RecvProcADMIN_MAP_DELETEMAPSHADOW	(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�e�폜 */
	void	RecvProcADMIN_MAP_RENEWEVENT		(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�C�x���g���X�V */
	void	RecvProcADMIN_MAP_DELETEEVENT		(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�C�x���g���폜 */
	void	RecvProcADMIN_MAP_ADD				(PBYTE pData, DWORD dwSessionID);					/* �}�b�v�ǉ� */
	void	RecvProcADMIN_REQ_ADMINILEVEL		(PBYTE pData, DWORD dwSessionID);					/* �Ǘ��Ҍ������x���v�� */
	void	RecvProcADMIN_RENEWADMINILEVEL		(PBYTE pData, DWORD dwSessionID);					/* �Ǘ��Ҍ������x���X�V */
	void	RecvProcADMIN_SERVER_SAVEINFO		(PBYTE pData, DWORD dwSessionID);					/* �T�[�o�[���ۑ� */
	void	RecvProcADMIN_ITEM_ADD				(PBYTE pData, DWORD dwSessionID);					/* �A�C�e�����ǉ� */
	void	RecvProcADMIN_ITEM_COPY				(PBYTE pData, DWORD dwSessionID);					/* �A�C�e�����R�s�[ */
	void	RecvProcADMIN_ITEM_DELETE			(PBYTE pData, DWORD dwSessionID);					/* �A�C�e�����폜 */
	void	RecvProcADMIN_ITEMTYPE_ADD			(PBYTE pData, DWORD dwSessionID);					/* �A�C�e����ʏ��ǉ� */
	void	RecvProcADMIN_ITEMTYPE_COPY			(PBYTE pData, DWORD dwSessionID);					/* �A�C�e����ʏ��R�s�[ */
	void	RecvProcADMIN_ITEMTYPE_DELETE		(PBYTE pData, DWORD dwSessionID);					/* �A�C�e����ʏ��폜 */
	void	RecvProcADMIN_ITEMWEAPON_ADD		(PBYTE pData, DWORD dwSessionID);					/* ������ǉ� */
	void	RecvProcADMIN_ITEMWEAPON_RENEW		(PBYTE pData, DWORD dwSessionID);					/* ������X�V */
	void	RecvProcADMIN_CHAR_ADDNPC			(PBYTE pData, DWORD dwSessionID);					/* NPC�̒ǉ� */
	void	RecvProcADMIN_CHAR_MODIFYITEM		(PBYTE pData, DWORD dwSessionID);					/* �����A�C�e���̕ύX */
	void	RecvProcADMIN_CHAR_RENEWMOTION		(PBYTE pData, DWORD dwSessionID);					/* �L�������[�V�������̍X�V */
	void	RecvProcADMIN_CHAR_ADDMOTIONTYPE	(PBYTE pData, DWORD dwSessionID);					/* �L�������[�V������ʏ��̒ǉ� */
	void	RecvProcADMIN_CHAR_RENEWMOTIONTYPE	(PBYTE pData, DWORD dwSessionID);					/* �L�������[�V������ʏ��̍X�V */
	void	RecvProcADMIN_CHAR_RENEWSTATUS		(PBYTE pData, DWORD dwSessionID);					/* �X�e�[�^�X���X�V */
	void	RecvProcADMIN_CHAR_REQ_STATUS		(PBYTE pData, DWORD dwSessionID);					/* �X�e�[�^�X���v�� */
	void	RecvProcADMIN_CHAR_REQ_ONLINE		(PBYTE pData, DWORD dwSessionID);					/* �I�����C�����L�����ꗗ�v�� */
	void	RecvProcADMIN_EFC_RENEWBALLOON		(PBYTE pData, DWORD dwSessionID);					/* ���o�����̍X�V */
	void	RecvProcADMIN_EFC_RENEWEFFECT		(PBYTE pData, DWORD dwSessionID);					/* �G�t�F�N�g���̍X�V */
	void	RecvProcADMIN_REQ_PLAYSOUND			(PBYTE pData, DWORD dwSessionID);					/* ���ʉ��̍Đ��v�� */
	void	RecvProcADMIN_CHAR_REQ_ACCOUNT		(PBYTE pData, DWORD dwSessionID);					/* �A�J�E���g���v�� */
	void	RecvProcADMIN_CHAR_RENEW_ACCOUNT	(PBYTE pData, DWORD dwSessionID);					/* �A�J�E���g���X�V */
	void	RecvProcADMIN_RENEW_CLIENTVERSION	(PBYTE pData, DWORD dwSessionID);					/* �N���C�A���g�o�[�W�����X�V */
	void	RecvProcADMIN_SYSTEM_REQ_INFO		(PBYTE pData, DWORD dwSessionID);					/* �V�X�e�����v�� */
	void	RecvProcADMIN_SYSTEM_RENEWINFO		(PBYTE pData, DWORD dwSessionID);					/* �V�X�e�����̍X�V */

	/* ��M����(MainFrameRecvProcMSGCMD.cpp) */
	void	RecvProcMSGCMD						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* ���b�Z�[�W�R�}���h�n */
	void	RecvProcMSGCMD_CHGFACE				(PBYTE pData, DWORD dwSessionID);					/* �\��ύX */
	void	RecvProcMSGCMD_CHGHAIR				(PBYTE pData, DWORD dwSessionID);					/* ���ύX */
	void	RecvProcMSGCMD_CHGCLOTH				(PBYTE pData, DWORD dwSessionID);					/* �����ύX */
	void	RecvProcMSGCMD_CHGACCE				(PBYTE pData, DWORD dwSessionID);					/* �A�N�Z�T���ύX */
	void	RecvProcMSGCMD_CHGCOLOR				(PBYTE pData, DWORD dwSessionID);					/* �F�ύX */
	void	RecvProcMSGCMD_CHGARMS				(PBYTE pData, DWORD dwSessionID);					/* �������ύX */
	void	RecvProcMSGCMD_CHGSHIELD			(PBYTE pData, DWORD dwSessionID);					/* ���ύX */
	void	RecvProcMSGCMD_MAKEITEM				(PBYTE pData, DWORD dwSessionID);					/* �A�C�e���쐬 */
	void	RecvProcMSGCMD_BALLOON				(PBYTE pData, DWORD dwSessionID);					/* ���o�� */
	void	RecvProcMSGCMD_DICE					(PBYTE pData, DWORD dwSessionID);					/* �T�C�R�� */
	void	RecvProcMSGCMD_RND					(PBYTE pData, DWORD dwSessionID);					/* �����_�� */
	void	RecvProcMSGCMD_EFFECT				(PBYTE pData, DWORD dwSessionID);					/* �G�t�F�N�g */
	void	RecvProcMSGCMD_WHERE				(PBYTE pData, DWORD dwSessionID);					/* �ł��W�܂��Ă���ꏊ */

	/* ��M����(MainFrameRecvProcEFFECT.cpp) */
	void	RecvProcEFFECT						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* �G�t�F�N�g�n */
	void	RecvProcEFFECT_REQ_BALLOONINFO		(PBYTE pData, DWORD dwSessionID);					/* ���o�����v�� */
	void	RecvProcEFFECT_DELETEBALLOONINFO	(PBYTE pData, DWORD dwSessionID);					/* ���o�����폜 */

	/* ��M����(MainFrameRecvProcBATTLE.cpp) */
	void	RecvProcBATTLE						(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID);	/* �퓬�n */
	void	RecvProcBATTLE_REQ_ATACK			(PBYTE pData, DWORD dwSessionID);					/* �U���v�� */


private:
	BYTE				m_byLastSendClock;				/* �Ō�ɂ��m�点�������� */
	DWORD				m_dwServerStartTime,			/* �T�[�o�[�N������ */
						m_dwLastKeepaliveCheck;			/* �Ō�ɐ����m�F�`�F�b�N�������� */
	HWND				m_hWnd;							/* �E�B���h�E�n���h�� */
	HFONT				m_hFont;						/* �T�[�o�[��Ԃ̕`��Ɏg���t�H���g */

	CMgrData			*m_pMgrData;					/* �f�[�^�Ǘ� */
	CUraraSockTCPSBO	*m_pSock;						/* �ʐM�}�l�[�W�� */
	CTextOutput			*m_pLog;						/* ���O�t�@�C�� */
	CUpdateServerInfo	*m_pUpdateServerInfo;			/* �T�[�o�[���A�b�v���[�h */

	CLibInfoAccount		*m_pLibInfoAccount;				/* �A�J�E���g��񃉃C�u���� */
	CLibInfoCharSvr		*m_pLibInfoChar;				/* �L������񃉃C�u���� */
	CLibInfoMapBase		*m_pLibInfoMap;					/* �}�b�v��񃉃C�u���� */
	CLibInfoMapObject	*m_pLibInfoMapObject;			/* �}�b�v�I�u�W�F�N�g��񃉃C�u���� */
	CLibInfoMapParts	*m_pLibInfoMapParts;			/* �}�b�v�p�[�c��񃉃C�u���� */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;			/* �}�b�v�e��񃉃C�u���� */
	CLibInfoItemType	*m_pLibInfoItemType;			/* �A�C�e����ʏ�񃉃C�u���� */
	CLibInfoItem		*m_pLibInfoItem;				/* �A�C�e����񃉃C�u���� */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;			/* �����񃉃C�u���� */
	CLibInfoEffect		*m_pLibInfoEffect;				/* �G�t�F�N�g��񃉃C�u���� */
	CLibInfoMotion		*m_pLibInfoMotion;				/* ���[�V������񃉃C�u���� */
	CLibInfoMotionType	*m_pLibInfoMotionType;			/* ���[�V������ʏ�񃉃C�u���� */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;			/* ���o����񃉃C�u���� */
	CLibInfoSystem		*m_pLibInfoSystem;				/* �V�X�e����񃉃C�u���� */
	CLibInfoSkill		*m_pLibInfoSkill;				/* �X�L����񃉃C�u���� */
};

/* Copyright(C)URARA-works 2006 */
