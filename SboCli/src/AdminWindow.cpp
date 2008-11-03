/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:AdminWindow.cpp											 */
/* ���e			:�Ǘ��҃E�B���h�E���C���t���[���N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/01/25													 */
/* ========================================================================= */

#include "stdafx.h"
#include "afxpriv.h"
#include "resource.h"
#include "Command.h"
#include "PacketADMIN_PARA2.h"
#include "UraraSockTCPSBO.h"
#include "DlgAdminCharModify.h"
#include "DlgAdminCharModifyStatus.h"
#include "DlgAdminCharModifyGrp.h"
#include "DlgAdminCharModifyItem.h"
#include "DlgAdminCharAdmin.h"
#include "DlgAdminCharAddNPC.h"
#include "DlgAdminCharMotionTypeList.h"
#include "DlgAdminCharAccountInfo.h"
#include "DlgAdminMapInfo.h"
#include "DlgAdminMapEvent.h"
#include "DlgAdminMapParts.h"
#include "DlgAdminMapShadow.h"
#include "DlgAdminMapObject.h"
#include "DlgAdminMapObjectData.h"
#include "DlgAdminItemTypeList.h"
#include "DlgAdminItemList.h"
#include "DlgAdminItemWeaponList.h"
#include "DlgAdminEfcBalloonList.h"
#include "DlgAdminEfcEffectList.h"
#include "DlgAdminServerInfo.h"
#include "DlgAdminSystemSetInitCharStatus.h"
#include "MgrData.h"
#include "WndMap.h"
#include "AdminWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

IMPLEMENT_DYNAMIC(CAdminWindow, CWnd)
BEGIN_MESSAGE_MAP(CAdminWindow, CWnd)
	//{{AFX_MSG_MAP(CAdminWindow)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_MAP_INFO, OnMapInfo)
	ON_COMMAND(IDM_MAP_EVENT, OnMapEvent)
	ON_COMMAND(IDM_MAP_PARTS, OnMapParts)
	ON_COMMAND(IDM_MAP_SHADOW, OnMapShadow)
	ON_COMMAND(IDM_MAP_ADD, OnMapAdd)
	ON_COMMAND(IDM_MAP_OBJECT, OnMapObject)
	ON_COMMAND(IDM_MAP_OBJECTDATA, OnMapObjectData)
	ON_COMMAND(IDM_MAP_WINDOW, OnMapWindow)
	ON_COMMAND(IDM_CHAR_MODIFY, OnCharModify)
	ON_COMMAND(IDM_CHAR_MODIFY_STATUS, OnCharModifyStatus)
	ON_COMMAND(IDM_CHAR_MODIFY_GRP, OnCharModifyGrp)
	ON_COMMAND(IDM_CHAR_MODIFY_ITEM, OnCharModifyItem)
	ON_COMMAND(IDM_CHAR_ADMIN, OnCharAdmin)
	ON_COMMAND(IDM_CHAR_ADDNPC, OnCharAddNPC)
	ON_COMMAND(IDM_CHAR_MOTION, OnCharMotion)
	ON_COMMAND(IDM_CHAR_ACCOUNTINFO, OnCharAccountInfo)
	ON_COMMAND(IDM_ITEMTYPE_LIST, OnItemTypeList)
	ON_COMMAND(IDM_ITEM_LIST, OnItemList)
	ON_COMMAND(IDM_ITEM_WEAPONLIST, OnItemWeaponList)
	ON_COMMAND(IDM_EFC_BALLOON_LIST, OnEfcBalloonList)
	ON_COMMAND(IDM_EFC_EFFECT_LIST, OnEfcEffectList)
	ON_COMMAND(IDM_SERVER_INFO, OnServerInfo)
	ON_COMMAND(IDM_DEBUG_MOVENOBLOCK, OnDebugMoveNoBlock)
	ON_COMMAND(IDM_DEBUG_GRID_OFF, OnDebugGridOff)
	ON_COMMAND(IDM_DEBUG_GRID, OnDebugGrid)
	ON_COMMAND(IDM_DEBUG_GRID_HALF, OnDebugGridHalf)
	ON_COMMAND(IDM_SYSTEM_SET_INITCHARSTATUS, OnSystemSetInitCharStatus)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	ON_MESSAGE(WM_MAINFRAME, OnMainFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CAdminWindow::CAdminWindow										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/01/25														 */
/* ========================================================================= */

CAdminWindow::CAdminWindow()
{
	m_hWnd			= NULL;
	m_hWndParent	= NULL;
	m_nScrID		= SCRIDADMIN_TOP;
	m_pDlgBase		= NULL;
	m_pMgrData		= NULL;
	m_pWndParent	= NULL;
	m_pWndMap		= NULL;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::~CAdminWindow									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/01/25														 */
/* ========================================================================= */

CAdminWindow::~CAdminWindow()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2006/01/25														 */
/* ========================================================================= */

BOOL CAdminWindow::Create(HWND hWndParent, CMgrData *pMgrData)
{
	BOOL bRet;

	m_pMgrData		= pMgrData;
	m_hWndParent	= hWndParent;

	bRet = CmyThread::Create ();
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::Destroy											 */
/* ���e		:�j��															 */
/* ���t		:2006/01/25														 */
/* ========================================================================= */

void CAdminWindow::Destroy(void)
{
	if (m_hThread) {
		if (m_hWnd) {
			PostMessage (WM_CLOSE);
		}
		WaitForSingleObject (m_hThread, INFINITE);
	}
	CmyThread::Destroy ();

	/* �ϐ��������� */
	m_pWndParent	= NULL;
	m_hWnd			= NULL;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::Show												 */
/* ���e		:�\��															 */
/* ���t		:2006/03/12														 */
/* ========================================================================= */

void CAdminWindow::Show(void)
{
	if (m_hWnd == NULL) {
		return;
	}

	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::Hide												 */
/* ���e		:��\��															 */
/* ���t		:2006/03/12														 */
/* ========================================================================= */

void CAdminWindow::Hide(void)
{
	if (m_hWnd == NULL) {
		return;
	}

	ShowWindow (SW_HIDE);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::ChgScreen										 */
/* ���e		:��ʐ؂�ւ�													 */
/* ���t		:2006/01/26														 */
/* ========================================================================= */

void CAdminWindow::ChgScreen(int nScrID)
{
	int nTypeL, nTypeR, nTypeRR;
	CRect rc;

	if (m_pDlgBase) {
		m_pDlgBase->DestroyWindow ();
		m_pDlgBase = NULL;
	}
	GetClientRect (rc);

	Initialize (m_hWnd);
	nTypeL  = ADMINNOTIFYTYPE_NONE;
	nTypeR  = ADMINNOTIFYTYPE_NONE;
	nTypeRR = ADMINNOTIFYTYPE_NONE;

	switch (nScrID) {
	case SCRIDADMIN_CHAR_MODIFY:				/* �I���L�������ҏW */
		nTypeL  = ADMINNOTIFYTYPE_CHARID;
		nTypeR  = ADMINNOTIFYTYPE_CHARPOS;
		nTypeRR = ADMINNOTIFYTYPE_CHARPOS;
		m_pDlgBase = new CDlgAdminCharModify(this);
		break;
	case SCRIDADMIN_CHAR_MODIFY_STATUS:			/* �I���L�������ҏW[�X�e�[�^�X] */
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharModifyStatus(this);
		break;
	case SCRIDADMIN_CHAR_MODIFY_GRP:			/* �I���L�������ҏW[�摜�̐ݒ�] */
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharModifyGrp(this);
		break;
	case SCRIDADMIN_CHAR_MODIFY_ITEM:			/* �I���L�������ҏW[�����A�C�e���̐ݒ�] */
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharModifyItem(this);
		break;
	case SCRIDADMIN_CHAR_ADMIN:					/* �Ǘ��Ҍ����̐ݒ� */
		nTypeL = ADMINNOTIFYTYPE_ACCOUNTID;
		m_pDlgBase = new CDlgAdminCharAdmin(this);
		break;
	case SCRIDADMIN_CHAR_ADDNPC:				/* NPC�̒ǉ� */
		nTypeL = ADMINNOTIFYTYPE_POS;
		m_pDlgBase = new CDlgAdminCharAddNPC(this);
		break;
	case SCRIDADMIN_CHAR_MOTION:				/* ���[�V�����̕ҏW */
		m_pDlgBase = new CDlgAdminCharMotionTypeList(this);
		break;
	case SCRIDADMIN_CHAR_ACCOUNTINFO:			/* �A�J�E���g���̕ҏW */
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharAccountInfo(this);
		break;
	case SCRIDADMIN_MAP_INFO:					/* �}�b�v���̕ҏW */
		m_pDlgBase = new CDlgAdminMapInfo(this);
		break;
	case SCRIDADMIN_MAP_EVENT:					/* �}�b�v�C�x���g���̕ҏW */
		nTypeL  = ADMINNOTIFYTYPE_POS;
		nTypeR  = ADMINNOTIFYTYPE_POS;
		m_pDlgBase = new CDlgAdminMapEvent(this);
		break;
	case SCRIDADMIN_MAP_PARTS:					/* �p�[�c�̕ҏW */
		nTypeL = ADMINNOTIFYTYPE_MAPEDIT;
		nTypeR = ADMINNOTIFYTYPE_MAPEDIT;
		m_pDlgBase = new CDlgAdminMapParts(this);
		break;
	case SCRIDADMIN_MAP_SHADOW:					/* �e�̕ҏW */
		nTypeL = ADMINNOTIFYTYPE_MAPSHADOWEDIT;
		nTypeR = ADMINNOTIFYTYPE_MAPSHADOWEDIT;
		m_pDlgBase = new CDlgAdminMapShadow(this);
		break;
	case SCRIDADMIN_MAP_OBJECT:					/* �}�b�v�I�u�W�F�N�g�̕ҏW */
		m_pDlgBase = new CDlgAdminMapObject(this);
		break;
	case SCRIDADMIN_MAP_OBJECTDATA:				/* �}�b�v�I�u�W�F�N�g�̔z�u */
		nTypeL = ADMINNOTIFYTYPE_POS;
		m_pDlgBase = new CDlgAdminMapObjectData(this);
		break;
	case SCRIDADMIN_ITEMTYPE_LIST:				/* �A�C�e����ʈꗗ */
		m_pDlgBase = new CDlgAdminItemTypeList(this);
		break;
	case SCRIDADMIN_ITEM_LIST:					/* �A�C�e���ꗗ */
		nTypeL = ADMINNOTIFYTYPE_ITEMID;
		m_pDlgBase = new CDlgAdminItemList(this);
		break;
	case SCRIDADMIN_ITEM_WEAPONLIST:			/* ������ꗗ */
		m_pDlgBase = new CDlgAdminItemWeaponList(this);
		break;
	case SCRIDADMIN_EFC_BALLOON_LIST:			/* ���o���ꗗ */
		m_pDlgBase = new CDlgAdminEfcBalloonList(this);
		break;
	case SCRIDADMIN_EFC_EFFECT_LIST:			/* �G�t�F�N�g�ꗗ */
		m_pDlgBase = new CDlgAdminEfcEffectList(this);
		break;
	case SCRIDADMIN_SERVER_INFO:				/* �T�[�o�[��� */
		m_pDlgBase = new CDlgAdminServerInfo(this);
		break;
	case SCRIDADMIN_SYSTEM_SET_INITCHARSTATUS:	/* �L�����X�e�[�^�X�����l�̐ݒ� */
		m_pDlgBase = new CDlgAdminSystemSetInitCharStatus(this);
		break;
	}
	m_pMgrData->SetAdminNotifyTypeL (nTypeL);
	m_pMgrData->SetAdminNotifyTypeR (nTypeR);
	m_pMgrData->SetAdminNotifyTypeRR (nTypeR);

	if (m_pDlgBase) {
		m_pDlgBase->Init (m_pMgrData);
		m_pDlgBase->SetWindowPos (NULL, 0, 0, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);
		RegisterControl (m_pDlgBase->m_hWnd, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	}
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::PreCreateWindow									 */
/* ���e		:�E�B���h�E�쐬�O����											 */
/* ���t		:2006/01/25														 */
/* ========================================================================= */

BOOL CAdminWindow::PreCreateWindow(CREATESTRUCT& cs)
{
	WNDCLASS wc;

	if (!CWnd::PreCreateWindow (cs)) {
		return FALSE;
	}

	GetClassInfo (AfxGetInstanceHandle (), cs.lpszClass, &wc);
	wc.style &= ~(CS_HREDRAW | CS_VREDRAW);

	AfxRegisterClass (&wc);
	cs.dwExStyle	&= ~WS_EX_CLIENTEDGE;
	cs.style		= WS_OVERLAPPEDWINDOW & ~WS_SYSMENU;
	cs.lpszName		= "�Ǘ��҃E�B���h�E";
	cs.lpszClass	= AfxRegisterWndClass (wc.style, wc.hCursor, wc.hbrBackground, wc.hIcon);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnCreate											 */
/* ���e		:���b�Z�[�W�n���h��(WM_CREATE)									 */
/* ���t		:2006/01/25														 */
/* ========================================================================= */

int CAdminWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	char szFileName[MAX_PATH];
	POINT pt;

	if (CWnd::OnCreate (lpCreateStruct) == -1) {
		return -1;
	}

	m_hWnd = GetSafeHwnd ();
	/* ���j���[�̐ݒ� */
	m_Menu.LoadMenu ("IDR_MENU1");
	SetMenu (&m_Menu);

	ChgScreen (SCRIDADMIN_CHAR_MODIFY);

	ZeroMemory (szFileName, sizeof (szFileName));
	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	pt.x = GetPrivateProfileInt ("Pos", "AdminX", -1, szFileName);
	pt.y = GetPrivateProfileInt ("Pos", "AdminY", -1, szFileName);
	if (!((pt.x == -1) && (pt.y == -1))) {
		SetWindowPos (NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}

	ShowWindow (SW_SHOW);
	m_pMgrData->SetAdminWindow (m_hWnd);

	return 0;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnDestroy										 */
/* ���e		:���b�Z�[�W�n���h��(WM_DESTROY)									 */
/* ���t		:2006/02/11														 */
/* ========================================================================= */

void CAdminWindow::OnDestroy()
{
	CWnd::OnDestroy ();
	PostQuitMessage (0);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::PostNcDestroy									 */
/* ���e		:�I������														 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CAdminWindow::PostNcDestroy()
{
//	delete this;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnGetMinMaxInfo									 */
/* ���e		:���b�Z�[�W�n���h��(WM_GETMINMAXINFO)							 */
/* ���t		:2006/01/25														 */
/* ========================================================================= */

void CAdminWindow::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 400;
	lpMMI->ptMinTrackSize.y = 300;
	
	CWnd::OnGetMinMaxInfo (lpMMI);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnSize											 */
/* ���e		:���b�Z�[�W�n���h��(WM_SIZE)									 */
/* ���t		:2006/01/25														 */
/* ========================================================================= */

void CAdminWindow::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize (nType, cx, cy);

	DoLayout (cx, cy);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnAdminMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

LRESULT CAdminWindow::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	if (m_pDlgBase) {
		m_pDlgBase->OnAdminMsg (wParam, lParam);
	}
	return -1;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnMainFrame										 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

LRESULT CAdminWindow::OnMainFrame(WPARAM wParam, LPARAM lParam)
{
	if (m_pDlgBase) {
		m_pDlgBase->OnMainFrame (wParam, lParam);
	}
	return -1;
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnMapInfo										 */
/* ���e		:���j���[�n���h��(�}�b�v���̕ҏW)								 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CAdminWindow::OnMapInfo()
{
	ChgScreen (SCRIDADMIN_MAP_INFO);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnMapEvent										 */
/* ���e		:���j���[�n���h��(�}�b�v�C�x���g���̕ҏW)						 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

void CAdminWindow::OnMapEvent()
{
	ChgScreen (SCRIDADMIN_MAP_EVENT);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnMapParts										 */
/* ���e		:���j���[�n���h��(�p�[�c�̕ҏW)									 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

void CAdminWindow::OnMapParts()
{
	ChgScreen (SCRIDADMIN_MAP_PARTS);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnMapShadow										 */
/* ���e		:���j���[�n���h��(�e�̕ҏW)										 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CAdminWindow::OnMapShadow()
{
	ChgScreen (SCRIDADMIN_MAP_SHADOW);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnMapAdd											 */
/* ���e		:���j���[�n���h��(�}�b�v�̒ǉ�)									 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

void CAdminWindow::OnMapAdd()
{
	int nResult;
	CPacketADMIN_PARA2 Packet;

	nResult = MessageBox ("�}�b�v��ǉ����܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}
	Packet.Make (SBOCOMMANDID_SUB_ADMIN_MAP_ADD, 0, 0);
	m_pMgrData->GetUraraSockTCP ()->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnMapObject										 */
/* ���e		:���j���[�n���h��(�}�b�v�I�u�W�F�N�g�̕ҏW)						 */
/* ���t		:2008/09/07														 */
/* ========================================================================= */

void CAdminWindow::OnMapObject()
{
	ChgScreen (SCRIDADMIN_MAP_OBJECT);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnMapObjectData									 */
/* ���e		:���j���[�n���h��(�}�b�v�I�u�W�F�N�g�̔z�u)						 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CAdminWindow::OnMapObjectData()
{
	ChgScreen (SCRIDADMIN_MAP_OBJECTDATA);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnMapWindow										 */
/* ���e		:���j���[�n���h��(�}�b�v�E�B���h�E)								 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CAdminWindow::OnMapWindow()
{
	if (m_pWndMap && IsWindow (m_pWndMap->GetSafeHwnd ())) {
		return;
	}

	m_pWndMap = new CWndMap;
	m_pWndMap->Create (this, m_pMgrData, 0);
	m_pWndMap->ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnCharModify										 */
/* ���e		:���j���[�n���h��(�I���L�����̕ҏW)								 */
/* ���t		:2007/03/17														 */
/* ========================================================================= */

void CAdminWindow::OnCharModify()
{
	ChgScreen (SCRIDADMIN_CHAR_MODIFY);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnCharModifyStatus								 */
/* ���e		:���j���[�n���h��(�I���L�����̕ҏW[�X�e�[�^�X])					 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CAdminWindow::OnCharModifyStatus()
{
	ChgScreen (SCRIDADMIN_CHAR_MODIFY_STATUS);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnCharModifyGrp									 */
/* ���e		:���j���[�n���h��(�I���L�����̕ҏW[�摜�̐ݒ�])					 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

void CAdminWindow::OnCharModifyGrp()
{
	ChgScreen (SCRIDADMIN_CHAR_MODIFY_GRP);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnCharModifyItem									 */
/* ���e		:���j���[�n���h��(�I���L�����̕ҏW[�����A�C�e���̐ݒ�])			 */
/* ���t		:2007/09/23														 */
/* ========================================================================= */

void CAdminWindow::OnCharModifyItem()
{
	ChgScreen (SCRIDADMIN_CHAR_MODIFY_ITEM);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnCharAdmin										 */
/* ���e		:���j���[�n���h��(�Ǘ��Ҍ����̐ݒ�)								 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

void CAdminWindow::OnCharAdmin()
{
	ChgScreen (SCRIDADMIN_CHAR_ADMIN);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnCharAddNPC										 */
/* ���e		:���j���[�n���h��(NPC�̒ǉ�)									 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

void CAdminWindow::OnCharAddNPC()
{
	ChgScreen (SCRIDADMIN_CHAR_ADDNPC);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnCharMotion										 */
/* ���e		:���j���[�n���h��(���[�V�����̕ҏW)								 */
/* ���t		:2007/10/28														 */
/* ========================================================================= */

void CAdminWindow::OnCharMotion()
{
	ChgScreen (SCRIDADMIN_CHAR_MOTION);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnCharAccountInfo								 */
/* ���e		:���j���[�n���h��(�A�J�E���g���̕ҏW)							 */
/* ���t		:2008/06/06														 */
/* ========================================================================= */

void CAdminWindow::OnCharAccountInfo()
{
	ChgScreen (SCRIDADMIN_CHAR_ACCOUNTINFO);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnItemTypeList									 */
/* ���e		:���j���[�n���h��(�A�C�e����ʈꗗ)								 */
/* ���t		:2007/09/30														 */
/* ========================================================================= */

void CAdminWindow::OnItemTypeList()
{
	ChgScreen (SCRIDADMIN_ITEMTYPE_LIST);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnItemList										 */
/* ���e		:���j���[�n���h��(�A�C�e���ꗗ)									 */
/* ���t		:2007/08/14														 */
/* ========================================================================= */

void CAdminWindow::OnItemList()
{
	ChgScreen (SCRIDADMIN_ITEM_LIST);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnItemWeaponList									 */
/* ���e		:���j���[�n���h��(������ꗗ)									 */
/* ���t		:2007/08/14														 */
/* ========================================================================= */

void CAdminWindow::OnItemWeaponList()
{
	ChgScreen (SCRIDADMIN_ITEM_WEAPONLIST);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnEfcBalloonList									 */
/* ���e		:���j���[�n���h��(���o���ꗗ)									 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CAdminWindow::OnEfcBalloonList()
{
	ChgScreen (SCRIDADMIN_EFC_BALLOON_LIST);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnEfcEffectList									 */
/* ���e		:���j���[�n���h��(�G�t�F�N�g�ꗗ)								 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CAdminWindow::OnEfcEffectList()
{
	ChgScreen (SCRIDADMIN_EFC_EFFECT_LIST);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnServerInfo										 */
/* ���e		:���j���[�n���h��(�T�[�o�[���)									 */
/* ���t		:2007/07/08														 */
/* ========================================================================= */

void CAdminWindow::OnServerInfo()
{
	ChgScreen (SCRIDADMIN_SERVER_INFO);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnDebugMoveNoBlock								 */
/* ���e		:���j���[�n���h��(�ړ������蔻�薳��)							 */
/* ���t		:2008/07/20														 */
/* ========================================================================= */

void CAdminWindow::OnDebugMoveNoBlock()
{
	BOOL bResult;

	bResult = m_pMgrData->GetMoveNoBlock ();
	bResult = (bResult) ? FALSE : TRUE;

	if (bResult) {
		GetMenu ()->CheckMenuItem (IDM_DEBUG_MOVENOBLOCK, MF_BYCOMMAND | MF_CHECKED);
	} else {
		GetMenu ()->CheckMenuItem (IDM_DEBUG_MOVENOBLOCK, MF_BYCOMMAND | MF_UNCHECKED);
	}
	m_pMgrData->SetMoveNoBlock (bResult);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnDebugGridOff									 */
/* ���e		:���j���[�n���h��(�O���b�h�\��-�\�����Ȃ�)						 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CAdminWindow::OnDebugGridOff()
{
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_OFF,	MF_BYCOMMAND | MF_CHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID,		MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_HALF,	MF_BYCOMMAND | MF_UNCHECKED);
	m_pMgrData->SetViewGrid (0);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnDebugGrid										 */
/* ���e		:���j���[�n���h��(�O���b�h�\��-1�L�����T�C�Y)					 */
/* ���t		:2008/05/02														 */
/* ========================================================================= */

void CAdminWindow::OnDebugGrid()
{
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_OFF,	MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID,		MF_BYCOMMAND | MF_CHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_HALF,	MF_BYCOMMAND | MF_UNCHECKED);
	m_pMgrData->SetViewGrid (1);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnDebugGridHalf									 */
/* ���e		:���j���[�n���h��(�O���b�h�\��-���L�����T�C�Y)					 */
/* ���t		:2008/05/02														 */
/* ========================================================================= */

void CAdminWindow::OnDebugGridHalf()
{
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_OFF,	MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID,		MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_HALF,	MF_BYCOMMAND | MF_CHECKED);
	m_pMgrData->SetViewGrid (2);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::OnSystemSetInitCharStatus						 */
/* ���e		:���j���[�n���h��(�L�����X�e�[�^�X�����l�̐ݒ�)					 */
/* ���t		:2008/09/22														 */
/* ========================================================================= */

void CAdminWindow::OnSystemSetInitCharStatus()
{
	ChgScreen (SCRIDADMIN_SYSTEM_SET_INITCHARSTATUS);
}


/* ========================================================================= */
/* �֐���	:CAdminWindow::ThreadMain										 */
/* ���e		:���C������														 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CAdminWindow::ThreadMain(void)
{
	MSG msg;
	BOOL bResult;
	CRect rc;

	m_pWndParent = CWnd::FromHandle (m_hWndParent);
	m_pWndParent->GetWindowRect (rc);
	rc.left		= rc.right;
	rc.right	= rc.left + 400;
	rc.bottom	= rc.top + 300;

	/* �E�B���h�E���쐬 */
	bResult = CWnd::Create (
					NULL,
					NULL,
					AFX_WS_DEFAULT_VIEW,
					rc,
					m_pWndParent,
					0);
	if (bResult == FALSE) {
		goto Exit;
	}

	m_pWndParent->SetFocus ();
	ShowWindow (SW_SHOW);

	while (1) {
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		} else {
			if (m_pWndMap && IsWindow (m_pWndMap->GetSafeHwnd ())) {
				/* �c�[���o�[���L���ɂȂ�悤�ɃA�C�h�����b�Z�[�W��ʒm */
				m_pWndMap->SendMessageToDescendants (WM_IDLEUPDATECMDUI);
			}
			MsgWaitForMultipleObjects (0, NULL, FALSE, 1, QS_ALLINPUT);
		}
	}

Exit:
	return;
}

/* Copyright(C)URARA-works 2006 */
