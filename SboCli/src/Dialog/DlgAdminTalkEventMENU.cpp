/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminTalkEventMENU.cpp									 */
/* ���e			:��b�C�x���g�ݒ�(���ڑI��)�_�C�A���O�N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventMENU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminTalkEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventMENU)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMENU, CDlgAdminTalkEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMENU)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::CDlgAdminTalkEventMENU					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMENU::CDlgAdminTalkEventMENU(CWnd* pParent /*=NULL*/)
	: CDlgAdminTalkEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventMENU)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventMENU::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::~CDlgAdminTalkEventMENU				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMENU::~CDlgAdminTalkEventMENU()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::Init									 */
/* ���e		:������															 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::Init(CMgrData *pMgrData)
{
	CDlgAdminTalkEventNONE::Init (pMgrData);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminTalkEventMENU::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventMENU::OnInitDialog()
{
	CDlgAdminTalkEventNONE::OnInitDialog();

	RegisterControl (IDC_LIST,		LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDC_ADD,		LH_CTRL_Y);
	RegisterControl (IDC_MODIFY,	LH_CTRL_Y);
	RegisterControl (IDC_DEL,		LH_CTRL_Y);

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
