/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:DlgDbg.cpp													 */
/* ���e			:�f�o�b�O���_�C�A���O�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "MgrData.h"
#include "InfoCharCli.h"
#include "MgrWindow.h"
#include "MainFrame.h"
#include "DlgDbg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

void CDlgDbg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDbg)
	DDX_Text(pDX, IDC_ONLINE, m_strOnline);
	DDX_Text(pDX, IDC_CHARCOUNT, m_strCharCount);
	DDX_Text(pDX, IDC_PING, m_strPing);
	DDX_Text(pDX, IDC_POS, m_strPos);
	DDX_Text(pDX, IDC_THROWGHPUT, m_strThrowghput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgDbg, CDialog)
	//{{AFX_MSG_MAP(CDlgDbg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgDbg::CDlgDbg												 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/04/05														 */
/* ========================================================================= */

CDlgDbg::CDlgDbg(CWnd* pParent /*=NULL*/)
: CDialog(CDlgDbg::IDD, pParent)
, m_pSock(NULL)
, m_strOnline(_T(""))
, m_strCharCount(_T(""))
, m_strPing(_T(""))
, m_strPos(_T(""))
, m_strThrowghput(_T(""))
{
	//{{AFX_DATA_INIT(CDlgDbg)
	//}}AFX_DATA_INIT

	m_pMgrData 	= NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgDbg::~CDlgDbg												 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/04/05														 */
/* ========================================================================= */

CDlgDbg::~CDlgDbg()
{
}


/* ========================================================================= */
/* �֐���	:CDlgDbg::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2009/04/05														 */
/* ========================================================================= */

BOOL CDlgDbg::Create(HWND hWndParent, CMgrData *pMgrData)
{
	CWnd *pWnd;

	m_pMgrData	= pMgrData;
	m_pSock		= m_pMgrData->GetUraraSockTCP ();
	pWnd		= CWnd::FromHandle (hWndParent);

	CDialog::Create (IDD, pWnd);

	ShowWindow (SW_SHOW);
	pWnd->SetFocus ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgDbg::Renew													 */
/* ���e		:�X�V															 */
/* ���t		:2009/04/05														 */
/* ========================================================================= */

void CDlgDbg::Renew(void)
{
	SetTimer(100, 100, NULL);
}


/* ========================================================================= */
/* �֐���	:CDlgDbg::OnInitDialog											 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2009/04/05														 */
/* ========================================================================= */

BOOL CDlgDbg::OnInitDialog()
{
	char szFileName[MAX_PATH];
	CRect rc;
	POINT pt;

	CDialog::OnInitDialog ();

	::GetWindowRect (m_pMgrData->GetMainWindow (), rc);
	SetWindowPos (NULL,
		rc.right + GetSystemMetrics (SM_CXFIXEDFRAME) * 2, rc.top,
		0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	Initialize (m_hWnd);
	SetTimer(100, 1000, NULL);

	ZeroMemory (szFileName, sizeof (szFileName));
	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	m_pMgrData->SetDebugWindow (m_hWnd);
	pt.x = GetPrivateProfileInt ("Pos", "DebugX", -1, szFileName);
	pt.y = GetPrivateProfileInt ("Pos", "DebugY", -1, szFileName);
	if (!((pt.x == -1) && (pt.y == -1))) {
		SetWindowPos (NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgDbg::OnSize												 */
/* ���e		:���b�Z�[�W�n���h��(WM_SIZE)									 */
/* ���t		:2009/04/05														 */
/* ========================================================================= */

void CDlgDbg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize (nType, cx, cy);

	DoLayout (cx, cy);
}


/* ========================================================================= */
/* �֐���	:CDlgDbg::OnTimer												 */
/* ���e		:���b�Z�[�W�n���h��(WM_TIMER)									 */
/* ���t		:2009/04/05														 */
/* ========================================================================= */

void CDlgDbg::OnTimer(UINT nIDEvent)
{
	PCInfoCharCli pChar;

	m_strOnline.Format("�I�����C���F%d", m_pMgrData->GetOnlineCount ());
	m_strCharCount.Format("�L�������F%d", m_pMgrData->GetCharCount ());
	m_strPing.Format("Ping�F%dms �`�掞��:%4dms", m_pMgrData->GetPing (), m_pMgrData->GetDrawTime ());
	m_strPos = "���W�F";
	pChar = m_pMgrData->GetPlayerChar ();
	if (pChar) {
		m_strPos.Format("���W�FMAP:%d X:%3d Y:%3d", pChar->m_dwMapID, pChar->m_nMapX, pChar->m_nMapY);
	}
	m_strThrowghput.Format("���M�F%5dBps ��M�F%5dBps", m_pSock->GetThrowghPutSend (0), m_pSock->GetThrowghPutRecv (0));

	UpdateData(FALSE);
	SetTimer(100, 1000, NULL);
}


/* ========================================================================= */
/* �֐���	:CDlgDbg::PostNcDestroy											 */
/* ���e		:�㏈��															 */
/* ���t		:2009/04/05														 */
/* ========================================================================= */

void CDlgDbg::PostNcDestroy()
{
	delete this;
}

/* Copyright(C)URARA-works 2009 */
