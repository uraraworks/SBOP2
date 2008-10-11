/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:SboLaunchDlg.h												 */
/* ���e			:���C���_�C�A���O ��`�t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/01/27													 */
/* ========================================================================= */

#pragma once

class CUraraSockTCPSBO;
class CInfoFileList;

/* ========================================================================= */
/* �\���̐錾																 */
/* ========================================================================= */

/* �t�@�C�����X�g�`�F�b�N�\���� */
typedef struct _CHECKFILELIST {
	int			nNo;			/* �`�F�b�N���̃t�@�C�����X�g�ԍ� */
	ARRAYINT	anGetFlieNo;	/* �擾����t�@�C�����X�g�ԍ� */
} CHECKFILELIST, *PCHECKFILELIST;

/* �t�@�C���擾���\���� */
typedef struct _GETFILEINFO {
	int			nNo;			/* �擾���̃t�@�C�����X�g�ԍ� */
	DWORD		dwRecvSize;		/* �擾�ς݃T�C�Y */
	PBYTE		pRecvDataTmp;	/* �擾���f�[�^ */
} GETFILEINFO, *PGETFILEINFO;


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

class CSboLaunchDlg : public CDialog
{
public:
			CSboLaunchDlg(CWnd* pParent = NULL);		/* �R���X�g���N�^ */
	virtual ~CSboLaunchDlg();							/* �f�X�g���N�^ */


protected:
	void	AddMsg					(LPCSTR pszMsg);	/* ���b�Z�[�W�ǉ� */
	void	ReadIniData				(void);				/* ini�t�@�C������ݒ��ǂݍ��� */
	void	ChgState				(int nState);		/* ��ԑJ�� */
	void	ChgStateNONE			(void);				/* ��ԑJ��(��~��) */
	void	ChgStateCONNECT			(void);				/* ��ԑJ��(�T�[�o�[�֐ڑ�) */
	void	ChgStateCONNECTMISS		(void);				/* ��ԑJ��(�T�[�o�[�֐ڑ����s) */
	void	ChgStateDISCONNECT		(void);				/* ��ԑJ��(�T�[�o�[�Ɛؒf) */
	void	ChgStateGETFILELIST		(void);				/* ��ԑJ��(�t�@�C�����X�g�擾) */
	void	ChgStateCHECKFILELIST	(void);				/* ��ԑJ��(�t�@�C�����X�g�`�F�b�N) */
	void	ChgStateGETFILE			(void);				/* ��ԑJ��(�t�@�C���擾) */
	void	ChgStateMAKEFILELIST	(void);				/* ��ԑJ��(�t�@�C�����X�g�o��) */
	void	ChgStateRUNUPDATE		(void);				/* ��ԑJ��(�A�b�v�f�[�^�N��) */
	void	ChgStateQUIT			(void);				/* ��ԑJ��(�I��) */
	void	ProcCHECKFILELIST		(void);				/* ����(�t�@�C�����X�g�`�F�b�N) */
	void	ProcGETFILE				(void);				/* ����(�t�@�C���擾) */
	void	OnRecv					(PBYTE pRecvData);	/* �f�[�^��M */
	void	RecvProcVERSION					(BYTE byCmdSub, PBYTE pData);	/* ��M����(�o�[�W�����n) */
	void	RecvProcVERSION_RES_FILELIST	(PBYTE pData);					/* ��M����(�t�@�C�����X�g����) */
	void	RecvProcVERSION_RES_FILE		(PBYTE pData);					/* ��M����(�t�@�C������) */
	void	DeleteAllFiles			(LPCSTR pszPath);						/* �w��p�X�ȉ��̃t�@�C����S�č폜 */
	void	DeleteAllFilesTmp		(HANDLE hFind, LPCSTR pszPath1, LPCSTR pszPath2, CmyStringArray &astrFolderPath);	/* �w��p�X�ȉ��̃t�@�C����S�č폜(����) */


protected:
	BOOL				m_bConnect;				/* �T�[�o�[�Ɛڑ����� */
	int					m_nState;				/* ��� */
	CUraraSockTCPSBO	*m_pSock;				/* �ʐM���C�u���� */
	CString				m_strServerAddr;		/* �T�[�o�[�A�h���X */
	WORD				m_wServerPort;			/* �T�[�o�[�ڑ��|�[�g */
	CInfoFileList		*m_pInfoFileList;		/* �t�@�C�����X�g��� */
	CHECKFILELIST		*m_pstCheckFlieList;	/* �t�@�C�����X�g�`�F�b�N�\���� */
	GETFILEINFO			*m_pstGetFileInfo;		/* �t�@�C���擾���\���� */



public:
	//{{AFX_DATA(CSboLaunchDlg)
	enum { IDD = IDD_SBOLAUNCH_DIALOG };
	CProgressCtrl	m_ctlProgressFile;
	CProgressCtrl	m_ctlProgressAll;
	CEdit	m_edLog;
	CString	m_strFile;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSboLaunchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

	//{{AFX_MSG(CSboLaunchDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedHistory();
	afx_msg void OnBnClickedHowto();
	afx_msg void OnBnClickedRetry();
};

//{{AFX_INSERT_LOCATION}}
/* Copyright(C)URARA-works 2008 */
