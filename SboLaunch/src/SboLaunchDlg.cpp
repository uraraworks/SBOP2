/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:SboLaunchDlg.cpp											 */
/* ���e			:���C���_�C�A���O �����t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/01/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SboLaunch.h"
#include "GetMD5File.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "InfoFileList.h"
#include "SboLaunchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CSboLaunchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSboLaunchDlg)
	DDX_Control(pDX, IDC_PROGRESS_FILE, m_ctlProgressFile);
	DDX_Control(pDX, IDC_PROGRESS_ALL, m_ctlProgressAll);
	DDX_Control(pDX, IDC_LOG, m_edLog);
	DDX_Text(pDX, IDC_FILE, m_strFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSboLaunchDlg, CDialog)
	//{{AFX_MSG_MAP(CSboLaunchDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSboLaunchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_HISTORY, &CSboLaunchDlg::OnBnClickedHistory)
	ON_BN_CLICKED(IDC_HOWTO, &CSboLaunchDlg::OnBnClickedHowto)
	ON_BN_CLICKED(IDC_RETRY, &CSboLaunchDlg::OnBnClickedRetry)
END_MESSAGE_MAP()

#define TIMERID_CONNECT		(100)			/* �T�[�o�[�֐ڑ�����^�C�}�[ */
#define TIMERID_CHECKFILE	(101)			/* �t�@�C�����X�g���`�F�b�N����^�C�}�[ */
#define TIMERID_GETFILE		(102)			/* �t�@�C�����擾����^�C�}�[ */


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::CSboLaunchDlg									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/01/27														 */
/* ========================================================================= */

CSboLaunchDlg::CSboLaunchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSboLaunchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSboLaunchDlg)
	m_strFile = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bConnect			= FALSE;
	m_nState			= GAMESTATE_NONE;
	m_pSock				= NULL;
	m_wServerPort		= 0;
	m_pInfoFileList		= NULL;
	m_pstCheckFlieList	= NULL;
	m_pstGetFileInfo	= NULL;
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::~CSboLaunchDlg									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/01/27														 */
/* ========================================================================= */

CSboLaunchDlg::~CSboLaunchDlg()
{
	SAFE_DELETE (m_pSock);
	SAFE_DELETE (m_pInfoFileList);
	SAFE_DELETE (m_pstCheckFlieList);
	SAFE_DELETE (m_pstGetFileInfo);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::OnInitDialog									 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/01/27														 */
/* ========================================================================= */

BOOL CSboLaunchDlg::OnInitDialog()
{
	CDialog::OnInitDialog ();

	SetIcon (m_hIcon, TRUE);

	m_pSock = new CUraraSockTCPSBO;
	ReadIniData ();

	GetDlgItem (IDC_RETRY)->EnableWindow (FALSE);
	GetDlgItem (IDOK)->EnableWindow (FALSE);

	SetTimer (TIMERID_CONNECT, 500, NULL);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::OnEraseBkgnd									 */
/* ���e		:���b�Z�[�W�n���h��(WM_ERASEBKGND)								 */
/* ���t		:2008/01/27														 */
/* ========================================================================= */

BOOL CSboLaunchDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc, rcMsg;
	CBrush Brush;
	CWnd *pWnd;
	HICON hIcon;

	CDialog::OnEraseBkgnd(pDC);

	GetClientRect (&rc);
	rc.bottom = 50;

	pWnd = GetDlgItem (IDC_MSG);
	if (pWnd == NULL) {
		goto Exit;
	}

	/* ���b�Z�[�W����h��Ԃ� */
	pWnd->GetClientRect (&rcMsg);
	pDC->FillSolidRect (rc, RGB (255, 255,255));

	/* �Z�b�g�A�b�v�A�C�R����\�� */
	hIcon = (HICON)LoadImage (AfxGetInstanceHandle (), MAKEINTRESOURCE (IDR_MAINFRAME), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	pDC->DrawIcon (rc.right - 48, 8, hIcon);
	DestroyIcon (hIcon);

	/* ���b�Z�[�W�����̐������� */
	rc.top = 50;
	rc.bottom = rc.top + 2;
	Brush.CreateSysColorBrush (COLOR_3DHILIGHT);
	pDC->FillRect (rc, &Brush);
	Brush.DeleteObject ();

	rc.bottom -= 1;
	Brush.CreateSysColorBrush (COLOR_3DSHADOW);
	pDC->FillRect (rc, &Brush);
	Brush.DeleteObject ();

Exit:
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::OnTimer											 */
/* ���e		:���b�Z�[�W�n���h��(WM_TIMER)									 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

void CSboLaunchDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent) {
	case TIMERID_CONNECT:		/* �T�[�o�[�֐ڑ�����^�C�}�[ */
		ChgState (GAMESTATE_CONNECT);
		KillTimer (TIMERID_CONNECT);
		break;
	case TIMERID_CHECKFILE:		/* �t�@�C�����X�g���`�F�b�N����^�C�}�[ */
		ProcCHECKFILELIST ();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::OnCtlColor										 */
/* ���e		:���b�Z�[�W�n���h��(WM_CTLCOLOR)								 */
/* ���t		:2008/01/27														 */
/* ========================================================================= */

HBRUSH CSboLaunchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor) {
	case CTLCOLOR_STATIC:		/* �X�^�e�B�b�N�R���g���[�� */
		/* ���b�Z�[�W���H*/
		if (pWnd->m_hWnd == GetDlgItem (IDC_MSG)->m_hWnd) {
			pDC->SetBkMode (TRANSPARENT);
			return (HBRUSH)GetStockObject (WHITE_BRUSH);
		}
		break;
	}
	
	return hbr;
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::OnBnClickedRetry								 */
/* ���e		:�{�^���n���h��(�ă`�F�b�N)										 */
/* ���t		:2008/05/28														 */
/* ========================================================================= */

void CSboLaunchDlg::OnBnClickedRetry()
{
	ChgState (GAMESTATE_CONNECT);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::OnBnClickedOk									 */
/* ���e		:�{�^���n���h��(�Q�[���J�n)										 */
/* ���t		:2008/05/25														 */
/* ========================================================================= */

void CSboLaunchDlg::OnBnClickedOk()
{
	BOOL bResult;
	char szPath[MAX_PATH], szTmp[MAX_PATH];
	STARTUPINFO stStartupInfo;
	PROCESS_INFORMATION stProcInfo;
	CmyString strFileName;

	GetModuleFilePath (szPath, sizeof (szPath));
	strFileName.Format ("%sSboCli.exe", szPath);

	ZeroMemory (&stStartupInfo, sizeof (stStartupInfo));
	ZeroMemory (&stProcInfo, sizeof (stProcInfo));
	stStartupInfo.cb = sizeof (stStartupInfo);

	strcpy (szTmp, strFileName);
	bResult = CreateProcess (NULL, szTmp, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, szPath,  &stStartupInfo, &stProcInfo);
	if (bResult == FALSE) {
		MessageBox ("�N���C�A���g�̋N���Ɏ��s���܂���", "�G���[", MB_OK);
		goto Exit;
	}

Exit:
	if (stProcInfo.hProcess) {
		CloseHandle (stProcInfo.hProcess);
	}
	if (stProcInfo.hThread) {
		CloseHandle (stProcInfo.hThread);
	}

	OnOK();
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::OnBnClickedOk									 */
/* ���e		:�{�^���n���h��(�X�V����)										 */
/* ���t		:2008/05/25														 */
/* ========================================================================= */

void CSboLaunchDlg::OnBnClickedHistory()
{
	char szPath[MAX_PATH];
	CmyString strFileName;

	GetModuleFilePath (szPath, sizeof (szPath));
	strFileName.Format ("%s�X�V����.txt", szPath);
	ShellExecute (NULL, "open", strFileName, NULL, NULL, SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::OnBnClickedOk									 */
/* ���e		:�{�^���n���h��(�V�ѕ�)											 */
/* ���t		:2008/05/25														 */
/* ========================================================================= */

void CSboLaunchDlg::OnBnClickedHowto()
{
	ShellExecute (NULL, "open", "http://uraran.jp/sbo/key.shtml", NULL, NULL, SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::DefWindowProc									 */
/* ���e		:���b�Z�[�W�n���h��												 */
/* ���t		:2008/02/25														 */
/* ========================================================================= */

LRESULT CSboLaunchDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int nState;

	if ((message >= URARASOCK_MSGBASE) && (message < URARASOCK_MSGBASE + WM_URARASOCK_MAX)) {
		switch (message - URARASOCK_MSGBASE) {
		case WM_URARASOCK_CONNECT:				/* �T�[�o�[�ɐڑ����� */
			AddMsg ("�T�[�o�[�ɐڑ����܂���\r\n");
			m_bConnect = TRUE;
			ChgState (GAMESTATE_GETFILELIST);
			break;
		case WM_URARASOCK_DISCONNECT:			/* �T�[�o�[�Ɛؒf���� */
			nState = GAMESTATE_DISCONNECT;
			if (m_bConnect == FALSE) {
				nState = GAMESTATE_CONNECTMISS;
			}
			ChgState (nState);
			break;
		case WM_URARASOCK_RECV:					/* �f�[�^��M */
			OnRecv ((PBYTE)wParam);
			break;
		}
		return 0;
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::AddMsg											 */
/* ���e		:���b�Z�[�W�ǉ�													 */
/* ���t		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::AddMsg(LPCSTR pszMsg)
{
	m_edLog.SetSel (-1);
	m_edLog.ReplaceSel (pszMsg);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ReadIniData										 */
/* ���e		:ini�t�@�C������ݒ��ǂݍ���									 */
/* ���t		:2008/01/27														 */
/* ========================================================================= */

void CSboLaunchDlg::ReadIniData(void)
{
	char szFileName[MAX_PATH], szTmp[128];

	GetModuleFilePath (szFileName, sizeof (szFileName));
	strcat (szFileName, "SboCli.ini");

	/* �T�[�o�[�A�h���X */
	GetPrivateProfileString ("Setting", "ServerAddr", "127.0.0.1", szTmp, sizeof (szTmp) - 1, szFileName);
	m_strServerAddr = szTmp;
	/* �҂������|�[�g */
	m_wServerPort = GetPrivateProfileInt ("Setting", "ServerPort", 2006, szFileName);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgState										 */
/* ���e		:��ԑJ��														 */
/* ���t		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgState(int nState)
{
	m_nState = nState;
	m_ctlProgressFile.SetRange (0, 0);
	m_ctlProgressAll. SetRange (0, 0);

	switch (nState) {
	case GAMESTATE_NONE:				/* ��~�� */
		ChgStateNONE ();
		break;
	case GAMESTATE_CONNECT:				/* �T�[�o�[�֐ڑ� */
		ChgStateCONNECT ();
		break;
	case GAMESTATE_CONNECTMISS:			/* �T�[�o�[�֐ڑ����s */
		ChgStateCONNECTMISS ();
		break;
	case GAMESTATE_DISCONNECT:			/* �T�[�o�[�Ɛؒf */
		ChgStateDISCONNECT ();
		break;
	case GAMESTATE_GETFILELIST:			/* �t�@�C�����X�g�擾 */
		ChgStateGETFILELIST ();
		break;
	case GAMESTATE_CHECKFILELIST:		/* �t�@�C�����X�g�`�F�b�N */
		ChgStateCHECKFILELIST ();
		break;
	case GAMESTATE_GETFILE:				/* �t�@�C���擾 */
		ChgStateGETFILE ();
		break;
	case GAMESTATE_MAKEFILELIST:		/* �t�@�C�����X�g�o�� */
		ChgStateMAKEFILELIST ();
		break;
	case GAMESTATE_RUNUPDATE:			/* �A�b�v�f�[�^�N�� */
		ChgStateRUNUPDATE ();
		break;
	case GAMESTATE_QUIT:				/* �I�� */
		ChgStateQUIT ();
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgStateNONE									 */
/* ���e		:��ԑJ��(��~��)												 */
/* ���t		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateNONE(void)
{
	SAFE_DELETE (m_pInfoFileList);
	SAFE_DELETE (m_pstCheckFlieList);
	if (m_pstGetFileInfo) {
		SAFE_DELETE_ARRAY (m_pstGetFileInfo->pRecvDataTmp);
	}
	SAFE_DELETE (m_pstGetFileInfo);
	m_pSock->Destroy ();
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgStateCONNECT									 */
/* ���e		:��ԑJ��(�T�[�o�[�֐ڑ�)										 */
/* ���t		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateCONNECT(void)
{
	BOOL bResult;

	m_bConnect = FALSE;
	GetDlgItem (IDC_RETRY)->EnableWindow (FALSE);

	AddMsg ("�T�[�o�[�֐ڑ����܂�...\r\n");
	bResult = m_pSock->Connect (m_hWnd, URARASOCK_MSGBASE, URARASOCK_PRECHECK, m_wServerPort, m_strServerAddr);
	if (bResult == FALSE) {
		ChgState (GAMESTATE_CONNECTMISS);
	}
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgStateCONNECTMISS								 */
/* ���e		:��ԑJ��(�T�[�o�[�֐ڑ����s)									 */
/* ���t		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateCONNECTMISS(void)
{
	AddMsg ("�T�[�o�[�ɐڑ��ł��܂���ł���\r\n");
	ChgState (GAMESTATE_NONE);
	GetDlgItem (IDC_RETRY)->EnableWindow (TRUE);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgStateDISCONNECT								 */
/* ���e		:��ԑJ��(�T�[�o�[�Ɛؒf)										 */
/* ���t		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateDISCONNECT(void)
{
	AddMsg ("�T�[�o�[�Ɛؒf���܂���\r\n");
	ChgState (GAMESTATE_NONE);
	GetDlgItem (IDC_RETRY)->EnableWindow (TRUE);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgStateGETFILELIST								 */
/* ���e		:��ԑJ��(�t�@�C�����X�g�擾)									 */
/* ���t		:2008/02/25														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateGETFILELIST(void)
{
	CPacketVERSION_REQ_FILELIST Packet;

	AddMsg ("�t�@�C�����X�g���擾���Ă��܂�...\r\n");
	Packet.Make ();
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgStateCHECKFILELIST							 */
/* ���e		:��ԑJ��(�t�@�C�����X�g�`�F�b�N)								 */
/* ���t		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateCHECKFILELIST(void)
{
	int nCount;

	AddMsg ("�X�V����t�@�C�����`�F�b�N���Ă��܂�\r\n");

	SAFE_DELETE (m_pstCheckFlieList);
	m_pstCheckFlieList = new CHECKFILELIST;
	m_pstCheckFlieList->nNo = 0;

	SetTimer (TIMERID_CHECKFILE, 10, NULL);

	nCount = m_pInfoFileList->GetCount ();
	m_ctlProgressFile.SetRange (0, nCount);
	m_ctlProgressAll. SetRange (0, nCount);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgStateGETFILE									 */
/* ���e		:��ԑJ��(�t�@�C���擾)											 */
/* ���t		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateGETFILE(void)
{
	int nCount;

	AddMsg ("�t�@�C�����_�E�����[�h���Ă��܂�...\r\n");

	m_pstGetFileInfo = new GETFILEINFO;
	m_pstGetFileInfo->nNo			= 0;
	m_pstGetFileInfo->dwRecvSize	= 0;
	m_pstGetFileInfo->pRecvDataTmp	= NULL;

	nCount = m_pInfoFileList->GetCount ();
	m_ctlProgressAll.SetRange (0, nCount);

	ProcGETFILE ();
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgStateMAKEFILELIST							 */
/* ���e		:��ԑJ��(�t�@�C�����X�g�o��)									 */
/* ���t		:2008/03/15														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateMAKEFILELIST(void)
{
	int i, nCount;
	HANDLE hFile;
	DWORD dwBytes;
	char szPath[MAX_PATH];
	PFILELISTINFO pInfo;
	CmyString strFileName, strTmp;

	GetModuleFilePath (szPath, sizeof (szPath));
	strFileName.Format ("%sUpdate\\FileList.txt", szPath);

	/* �t�@�C�����J�� */
	hFile = CreateFile (strFileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		strTmp.Format ("�t�@�C�����X�g�̏o�͂Ɏ��s���܂���[%d]\r\n", GetLastError ());
		AddMsg ((LPCSTR)strTmp);
		return;
	}

	nCount = m_pstCheckFlieList->anGetFlieNo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_pInfoFileList->GetPtr (m_pstCheckFlieList->anGetFlieNo[i]);
		strTmp = pInfo->strFileName;
		strTmp += "\r\n";

		dwBytes = 0;
		WriteFile (hFile, (PBYTE)(LPCSTR)strTmp, strTmp.GetLength (), &dwBytes, NULL);
	}

	CloseHandle (hFile);
	ChgState (GAMESTATE_RUNUPDATE);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgStateRUNUPDATE								 */
/* ���e		:��ԑJ��(�A�b�v�f�[�^�N��)										 */
/* ���t		:2008/03/15														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateRUNUPDATE(void)
{
	BOOL bResult;
	char szPath[MAX_PATH], szTmp[MAX_PATH];
	STARTUPINFO stStartupInfo;
	PROCESS_INFORMATION stProcInfo;
	CmyString strFileName, strTmp;

	AddMsg ("�t�@�C�����X�V���Ă��܂�\r\n");

	GetModuleFilePath (szPath, sizeof (szPath));
	strcat (szPath, "Update\\");
	strFileName.Format ("%sSboCopy.exe", szPath);

	ZeroMemory (&stStartupInfo, sizeof (stStartupInfo));
	ZeroMemory (&stProcInfo, sizeof (stProcInfo));
	stStartupInfo.cb = sizeof (stStartupInfo);

	strcpy (szTmp, strFileName);
	bResult = CreateProcess (NULL, szTmp, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, szPath,  &stStartupInfo, &stProcInfo);
	if (bResult == FALSE) {
		strTmp.Format ("�A�b�v�f�[�^�̋N���Ɏ��s���܂���[%d]\r\n", GetLastError ());
		AddMsg ((LPCSTR)strTmp);
		goto Exit;
	}

Exit:
	if (stProcInfo.hProcess) {
		CloseHandle (stProcInfo.hProcess);
	}
	if (stProcInfo.hThread) {
		CloseHandle (stProcInfo.hThread);
	}
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ChgStateQUIT									 */
/* ���e		:��ԑJ��(�I��)													 */
/* ���t		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ChgStateQUIT(void)
{
	char szPath[MAX_PATH];

	AddMsg ("�t�@�C���̃`�F�b�N���I�����܂���\r\n");
	m_pSock->Destroy ();

	GetModuleFilePath (szPath, sizeof (szPath));
	strcat (szPath, "Update\\");
	DeleteAllFiles (szPath);

	GetModuleFilePath (szPath, sizeof (szPath));
	strcat (szPath, "SboCopy.exe");
	DeleteFile (szPath);

	GetDlgItem (IDC_RETRY)->EnableWindow (TRUE);
	GetDlgItem (IDOK)->EnableWindow (TRUE);

	ChgState (GAMESTATE_NONE);
	GetDlgItem (IDC_RETRY)->EnableWindow (FALSE);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ProcCHECKFILELIST								 */
/* ���e		:����(�t�@�C�����X�g�`�F�b�N)									 */
/* ���t		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ProcCHECKFILELIST(void)
{
	BOOL bGetFile;
	int nCount, nState;
	char szPath[MAX_PATH], szHash[33];
	PFILELISTINFO pInfo;
	CGetMD5File GetMD5;
	CString strFileName, strTmp;

	nCount = m_pInfoFileList->GetCount ();
	/* �I���H */
	if (m_pstCheckFlieList->nNo >= nCount) {
		KillTimer (TIMERID_CHECKFILE);
		nState = GAMESTATE_GETFILE;
		/* �A�b�v�f�[�^�͖��񂠂�̂�2�ȏ�̏ꍇ�̂݃_�E�����[�h�ΏۂƂ��� */
		if (m_pstCheckFlieList->anGetFlieNo.GetSize () <= 1) {
			nState = GAMESTATE_QUIT;
		}
		ChgState (nState);
		return;
	}

	bGetFile = FALSE;
	ZeroMemory (szHash, sizeof (szHash));
	GetModuleFilePath (szPath, sizeof (szPath));
	pInfo = m_pInfoFileList->GetPtr (m_pstCheckFlieList->nNo);

	strFileName.Format ("%s%s", szPath, (LPCSTR)pInfo->strFileName);
	GetMD5.Init ();
	GetMD5.Update (strFileName);
	GetMD5.GetStr (szHash);
	if (strlen (szHash) <= 0) {
		bGetFile = TRUE;
	}
	if (pInfo->strMD5 != szHash) {
		bGetFile = TRUE;
	}

	if (bGetFile) {
		m_pstCheckFlieList->anGetFlieNo.Add (m_pstCheckFlieList->nNo);
	}
	m_pstCheckFlieList->nNo ++;
	m_ctlProgressFile.SetPos (m_pstCheckFlieList->nNo);
	m_ctlProgressAll. SetPos (m_pstCheckFlieList->nNo);
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::ProcGETFILE										 */
/* ���e		:����(�t�@�C���擾)												 */
/* ���t		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::ProcGETFILE(void)
{
	DWORD dwRecvSize, dwBytes;
	HANDLE hFile;
	PFILELISTINFO pInfo;
	LPSTR pszFileName;
	char szFileName[MAX_PATH], szPath[MAX_PATH];
	CPacketVERSION_REQ_FILE Packet;
	CmyString strFileName, strTmp;

	if (m_pstCheckFlieList == NULL) {
		return;
	}
	if (m_pstGetFileInfo == NULL) {
		return;
	}

	pInfo = m_pInfoFileList->GetPtr (m_pstCheckFlieList->anGetFlieNo[m_pstGetFileInfo->nNo]);

	/* ��M�����H */
	if (m_pstGetFileInfo->dwRecvSize == pInfo->dwFileSize) {
		m_ctlProgressAll.SetPos (m_ctlProgressAll.GetPos () + 1);

		GetModuleFilePath (szFileName, sizeof (szFileName));
		strFileName.Format ("%sUpdate\\%s", szFileName, (LPCSTR)pInfo->strFileName);
		strcpy (szPath, strFileName);
		pszFileName = PathFindFileName (szPath);
		if (pszFileName) {
			*pszFileName = 0;
		}
		AllCreateDirectory (szPath);
		/* �t�@�C�����J�� */
		hFile = CreateFile (strFileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE) {
			strTmp.Format ("�t�@�C���̏o�͂Ɏ��s���܂���[%d]\r\n", GetLastError ());
			AddMsg ((LPCSTR)strTmp);
			return;
		}
		dwBytes = 0;
		WriteFile (hFile, m_pstGetFileInfo->pRecvDataTmp, m_pstGetFileInfo->dwRecvSize, &dwBytes, NULL);
		CloseHandle (hFile);

		SAFE_DELETE_ARRAY (m_pstGetFileInfo->pRecvDataTmp);
		m_pstGetFileInfo->nNo ++;
		m_pstGetFileInfo->dwRecvSize	= 0;
		m_pstGetFileInfo->pRecvDataTmp	= NULL;

		/* �S�ă_�E�����[�h�����H */
		if (m_pstGetFileInfo->nNo >= m_pstCheckFlieList->anGetFlieNo.GetSize ()) {
			m_strFile.Empty ();
			UpdateData (FALSE);
			AddMsg ("�t�@�C���̃_�E�����[�h���������܂���\r\n");
			ChgState (GAMESTATE_MAKEFILELIST);
			return;
		}
		pInfo = m_pInfoFileList->GetPtr (m_pstCheckFlieList->anGetFlieNo[m_pstGetFileInfo->nNo]);
	}

	dwRecvSize = 1024 * 8 * 2;
	if (m_pstGetFileInfo->dwRecvSize + dwRecvSize > pInfo->dwFileSize) {
		dwRecvSize = pInfo->dwFileSize - m_pstGetFileInfo->dwRecvSize;
	}
	Packet.Make (m_pstGetFileInfo->dwRecvSize, dwRecvSize, pInfo->strFileName);
	m_pSock->Send (&Packet);

	/* ����H */
	if (m_pstGetFileInfo->dwRecvSize == 0) {
		strTmp.Format ("%s\r\n", (LPCSTR)pInfo->strFileName);
		AddMsg ((LPCSTR)strTmp);

		m_ctlProgressFile.SetRange32 (0, pInfo->dwFileSize);
		m_ctlProgressFile.SetPos (0);
		m_pstGetFileInfo->pRecvDataTmp = ZeroNew (pInfo->dwFileSize);
		m_strFile.Format ("0/%d", pInfo->dwFileSize);
		UpdateData (FALSE);
	}
}


/* ========================================================================= */
/* �֐���	:CSboLaunchDlg::OnRecv											 */
/* ���e		:�f�[�^��M														 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

void CSboLaunchDlg::OnRecv(PBYTE pRecvData)
{
	CPacketBase Packet;

	Packet.Set (pRecvData);

	switch (Packet.m_byCmdMain) {
	case SBOCOMMANDID_MAIN_VERSION:	RecvProcVERSION	(Packet.m_byCmdSub, pRecvData);		break;		/* �o�[�W�����n */
	}

	SAFE_DELETE_ARRAY (pRecvData);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION									 */
/* ���e		:��M����(�o�[�W�����n)											 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

void CSboLaunchDlg::RecvProcVERSION(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_VERSION_RES_FILELIST:	RecvProcVERSION_RES_FILELIST	(pData);	break;	/* �t�@�C�����X�g���� */
	case SBOCOMMANDID_SUB_VERSION_RES_FILE:		RecvProcVERSION_RES_FILE		(pData);	break;	/* �t�@�C������ */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION_RES_FILELIST						 */
/* ���e		:��M����(�t�@�C�����X�g����)									 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

void CSboLaunchDlg::RecvProcVERSION_RES_FILELIST(PBYTE pData)
{
	CPacketVERSION_RES_FILELIST Packet;

	Packet.Set (pData);

	AddMsg ("�t�@�C�����X�g���擾���܂���\r\n");

	SAFE_DELETE (m_pInfoFileList);
	m_pInfoFileList = new CInfoFileList;
	m_pInfoFileList->Copy (Packet.m_pInfo);

	ChgState (GAMESTATE_CHECKFILELIST);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION_RES_FILE							 */
/* ���e		:��M����(�t�@�C������)											 */
/* ���t		:2008/03/09														 */
/* ========================================================================= */

void CSboLaunchDlg::RecvProcVERSION_RES_FILE(PBYTE pData)
{
	PFILELISTINFO pInfo;
	CPacketVERSION_RES_FILE Packet;

	if (m_nState != GAMESTATE_GETFILE) {
		return;
	}

	Packet.Set (pData);

	pInfo = m_pInfoFileList->GetPtr (m_pstCheckFlieList->anGetFlieNo[m_pstGetFileInfo->nNo]);
	CopyMemory (&m_pstGetFileInfo->pRecvDataTmp[m_pstGetFileInfo->dwRecvSize], Packet.m_pFileData, Packet.m_dwSize);
	m_pstGetFileInfo->dwRecvSize += Packet.m_dwSize;

	m_strFile.Format ("%d/%d", m_pstGetFileInfo->dwRecvSize, pInfo->dwFileSize);
	UpdateData (FALSE);

	m_ctlProgressFile.SetPos (m_pstGetFileInfo->dwRecvSize);
	ProcGETFILE ();
}


/* ========================================================================= */
/* �֐���	:CMainFrame::DeleteAllFiles										 */
/* ���e		:�w��p�X�ȉ��̃t�@�C����S�č폜								 */
/* ���t		:2008/03/15														 */
/* ========================================================================= */

void CSboLaunchDlg::DeleteAllFiles(LPCSTR pszPath)
{
	int i, nCount;
	WIN32_FIND_DATA stFindData;
	HANDLE hFind;
	CmyString strTmp;
	CmyStringArray astrFolderPath;

	hFind = INVALID_HANDLE_VALUE;
	strTmp.Format ("%s*.*", pszPath);

	/* �ŏ��̌��� */
	hFind = FindFirstFile (strTmp, &stFindData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	DeleteAllFilesTmp (hFind, pszPath, "", astrFolderPath);
	FindClose (hFind);
	nCount = astrFolderPath.GetSize ();
	for (i = 0; i < nCount; i ++) {
		RemoveDirectory (astrFolderPath[i]);
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::DeleteAllFilesTmp									 */
/* ���e		:�w��p�X�ȉ��̃t�@�C����S�č폜(����)							 */
/* ���t		:2008/03/15														 */
/* ========================================================================= */

void CSboLaunchDlg::DeleteAllFilesTmp(
	HANDLE hFind,						/* [in] �����n���h�� */
	LPCSTR pszPath1,					/* [in] ��΃p�X */
	LPCSTR pszPath2,					/* [in] ���΃p�X */
	CmyStringArray &astrFolderPath)		/* [in/ou] �폜����t�H���_ */
{
	BOOL bResult;
	char szFullPath[MAX_PATH];
	HANDLE hFindNew;
	WIN32_FIND_DATA stFindData;
	CmyString strTmp, strPathTmp1, strPathTmp2;

	strcpy (szFullPath, pszPath1);
	PathAddBackslash (szFullPath);
	strcat (szFullPath, pszPath2);
	PathRemoveBackslash (szFullPath);

	bResult = TRUE;
	while (bResult) {
		bResult = FindNextFile (hFind, &stFindData);
		if (bResult == FALSE) {
			strTmp = szFullPath;
			astrFolderPath.Add (strTmp);
			break;
		}

		/* �t�H���_�H */
		if (stFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (strcmp (".", stFindData.cFileName) &&
				strcmp ("..", stFindData.cFileName)) {
				/* ���΃p�X���X�V */
				strPathTmp2  = pszPath2;
				strPathTmp2 += stFindData.cFileName;
				strPathTmp2 += "\\";

				/* ��΃p�X���X�V */
				strPathTmp1 = pszPath1;
				strPathTmp1 += pszPath2;
				strPathTmp1 += stFindData.cFileName;
				strPathTmp1 += "\\*.*";

				/* ���̃t�H���_������ */
				hFindNew = FindFirstFile (strPathTmp1, &stFindData);
				if (hFindNew == INVALID_HANDLE_VALUE) {
					goto Exit;
				}
				DeleteAllFilesTmp (hFindNew, pszPath1, strPathTmp2, astrFolderPath);
				FindClose (hFindNew);
			}

		} else {
			PathAddBackslash (szFullPath);
			strcat (szFullPath, stFindData.cFileName);
			DeleteFile (szFullPath);
			/* ���̎g�p�Ɍ����ăt�@�C�������폜���Ă��� */
			PathRemoveFileSpec (szFullPath);
		}
	}

Exit:
	return;
}

/* Copyright(C)URARA-works 2008 */
