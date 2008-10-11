/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharAddNPC.cpp										 */
/* ���e			:NPC�̒ǉ��_�C�A���O�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_CHAR_ADDNPC.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharAddNPC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharAddNPC::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharAddNPC)
	DDX_Control(pDX, IDC_FAMILYTYPE, m_ctlFamilyType);
	DDX_Text(pDX, IDC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_POSMAPID, m_nMapID);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharAddNPC, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharAddNPC)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAddNPC::CDlgAdminCharAddNPC						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

CDlgAdminCharAddNPC::CDlgAdminCharAddNPC(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharAddNPC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharAddNPC)
	m_strCharName = _T("");
	m_nMapID = 0;
	m_nPosX = 0;
	m_nPosY = 0;
	//}}AFX_DATA_INIT

	m_pInfoChar = NULL;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAddNPC::~CDlgAdminCharAddNPC						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

CDlgAdminCharAddNPC::~CDlgAdminCharAddNPC()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAddNPC::Init										 */
/* ���e		:������															 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

void CDlgAdminCharAddNPC::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminCharAddNPC::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAddNPC::Renew										 */
/* ���e		:�X�V															 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

void CDlgAdminCharAddNPC::Renew(void)
{
	if (m_pInfoChar == NULL) {
		goto Exit;
	}

Exit:
	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAddNPC::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

void CDlgAdminCharAddNPC::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:	/* ���N���b�N�ʒm */
		switch (m_pMgrData->GetAdminNotifyTypeL ()) {
		case ADMINNOTIFYTYPE_POS:				/* �}�b�v���W */
			UpdateData ();
			m_nMapID = m_pInfoChar->m_dwMapID;
			m_nPosX  = HIWORD (dwPara);
			m_nPosY  = LOWORD (dwPara);
			UpdateData (FALSE);
			break;
		}
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAddNPC::OnInitDialog								 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

BOOL CDlgAdminCharAddNPC::OnInitDialog()
{
	int i;
	LPCSTR pszFamilyName;

	CDlgAdminBase::OnInitDialog ();

	m_pInfoChar = m_pMgrData->GetPlayerChar ();

	for (i = FAMILYTYPE_NONE + 1; i < FAMILYTYPE_MAX; i ++) {
		pszFamilyName = m_pMgrData->GetFamilyTypeName (i);
		m_ctlFamilyType.AddString (pszFamilyName);
	}
	m_ctlFamilyType.SetCurSel (0);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharAddNPC::OnSend									 */
/* ���e		:�{�^���n���h��(���M)											 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

void CDlgAdminCharAddNPC::OnSend()
{
	int nResult;
	CInfoCharCli InfoCharTmp;
	CPacketADMIN_CHAR_ADDNPC Packet;

	UpdateData ();

	if (m_nMapID == 0) {
		MessageBox ("�}�b�vID�ƍ��W���m�F���Ă�������", "�m�F", MB_ICONINFORMATION | MB_OK);
		return;
	}

	nResult = MessageBox ("�w����W��NPC��ǉ����܂����H", "�m�F", MB_ICONQUESTION | MB_YESNO);
	if (nResult != IDYES) {
		return;
	}

	/* ���͓��e���L�������֔��f */
	InfoCharTmp.Create (m_pMgrData);
	InfoCharTmp.m_strCharName	= m_strCharName;
	InfoCharTmp.m_dwMapID		= m_nMapID;
	InfoCharTmp.m_nMapX			= m_nPosX * 2;
	InfoCharTmp.m_nMapY			= m_nPosY * 2;
	InfoCharTmp.m_nMoveType		= CHARMOVETYPE_STAND;
	InfoCharTmp.m_wFamilyID		= m_ctlFamilyType.GetCurSel () + 1;

	Packet.Make (&InfoCharTmp);
	m_pSock->Send (&Packet);

	/* ���M�����̂œ�x�����h�~�ɍ��W���N���A���Ă��� */
	m_nMapID = m_nPosX = m_nPosY = 0;
	UpdateData (FALSE);
}

/* Copyright(C)URARA-works 2007 */
