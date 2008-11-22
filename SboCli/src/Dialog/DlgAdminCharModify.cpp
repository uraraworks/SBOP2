/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharModify.cpp										 */
/* ���e			:�I���L�������ҏW�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/03/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_CHARINFO.h"
#include "LibInfoMotionType.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharModifyTypePUTNPC.h"
#include "DlgAdminTalkSet.h"
#include "DlgAdminCharModify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminCharModify::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModify)
	DDX_Control(pDX, IDC_MOVETYPE, m_ctlMoveType);
	DDX_Control(pDX, IDC_MOTIONTYPE, m_ctlMotionType);
	DDX_Control(pDX, IDC_SEX, m_ctlSex);
	DDX_Text(pDX, IDC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_STATIC_CHARID, m_strCharID);
	DDX_Check(pDX, IDC_BLOCK, m_bBlock);
	DDX_Check(pDX, IDC_PUSH, m_bPush);
	DDX_Text(pDX, IDC_POSMAPID, m_dwMapID);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharModify, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModify)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_TALK, OnTalk)
	ON_BN_CLICKED(IDC_SET_MOVETYPE, OnSetMoveType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::CDlgAdminCharModify						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/03/17														 */
/* ========================================================================= */

CDlgAdminCharModify::CDlgAdminCharModify(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharModify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModify)
	m_strCharName = _T("");
	m_strCharID = _T("");
	m_bBlock = FALSE;
	m_bPush = FALSE;
	m_dwMapID = 0;
	m_nPosX = 0;
	m_nPosY = 0;
	//}}AFX_DATA_INIT

	m_pInfoChar = NULL;
	Create (CDlgAdminCharModify::IDD, pParent);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::~CDlgAdminCharModify						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/03/17														 */
/* ========================================================================= */

CDlgAdminCharModify::~CDlgAdminCharModify()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::Init										 */
/* ���e		:������															 */
/* ���t		:2007/03/17														 */
/* ========================================================================= */

void CDlgAdminCharModify::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::Renew										 */
/* ���e		:�X�V															 */
/* ���t		:2007/03/17														 */
/* ========================================================================= */

void CDlgAdminCharModify::Renew(void)
{
	int i, nCount, nNo;
	PCLibInfoMotionType pLibInfoMotionType;
	PCInfoMotionType pInfoMotionType;

	pLibInfoMotionType = m_pMgrData->GetLibInfoMotionType ();
	m_strCharID.	Empty ();
	m_strCharName.	Empty ();
	m_bBlock	= FALSE;
	m_bPush		= FALSE;
	m_dwMapID	= 0;
	m_nPosX		= 0;
	m_nPosY		= 0;
	m_ctlMoveType.SetCurSel (0);
	m_ctlMotionType.SetCurSel (0);
	m_ctlSex.SetCurSel (0);

	if (m_pInfoChar == NULL) {
		goto Exit;
	}

	m_strCharID.Format ("%u", m_pInfoChar->m_dwCharID);
	m_strCharName	= m_pInfoChar->m_strCharName;
	m_bBlock		= m_pInfoChar->m_bBlock;
	m_bPush			= m_pInfoChar->m_bPush;
	for (i = 0; i < CHARMOVETYPE_MAX; i ++) {
		if (m_ctlMoveType.GetItemData (i) == m_pInfoChar->m_nMoveType) {
			m_ctlMoveType.SetCurSel (i);
			break;
		}
	}
	if (i >= CHARMOVETYPE_MAX) {
		m_ctlMoveType.SetCurSel (0);
	}

	m_dwMapID	= m_pInfoChar->m_dwMapID;
	m_nPosX		= m_pInfoChar->m_nMapX;
	m_nPosY		= m_pInfoChar->m_nMapY;

	m_ctlMotionType.ResetContent ();
	nCount = pLibInfoMotionType->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMotionType = (PCInfoMotionType)pLibInfoMotionType->GetPtr (i);
		m_ctlMotionType.AddString ((LPCSTR)pInfoMotionType->m_strName);
		m_ctlMotionType.SetItemData (i, pInfoMotionType->m_dwMotionTypeID);
	}

	nNo = 0;
	nCount = m_ctlMotionType.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_pInfoChar->m_dwMotionTypeID == m_ctlMotionType.GetItemData (i)) {
			nNo = i;
			break;
		}
	}
	m_ctlMotionType.SetCurSel (nNo);

	m_ctlSex.ResetContent ();
	m_ctlSex.AddString ("�j��");
	m_ctlSex.SetItemData (0, SEX_MALE);
	m_ctlSex.AddString ("����");
	m_ctlSex.SetItemData (1, SEX_FEMALE);
	nNo = 0;
	if (m_pInfoChar->m_nSex == SEX_FEMALE) {
		nNo = 1;
	}
	m_ctlSex.SetCurSel (nNo);

Exit:
	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::OnAdminMsg								 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharModify::OnAdminMsg(int nType, DWORD dwPara)
{
	PCLibInfoCharCli pLibInfoChar;

	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:	/* ���N���b�N�ʒm */
		pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
		m_pInfoChar		= (PCInfoCharCli)pLibInfoChar->GetPtr (dwPara);
		Renew ();
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:	/* �E�N���b�N�ʒm */
		m_nPosX = HIWORD (dwPara);
		m_nPosY = LOWORD (dwPara);
		UpdateData (FALSE);
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDBLCLK:	/* �E�_�u���N���b�N�ʒm */
		m_nPosX = HIWORD (dwPara);
		m_nPosY = LOWORD (dwPara);
		UpdateData (FALSE);
		Send (TRUE);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::OnInitDialog								 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2007/03/17														 */
/* ========================================================================= */

BOOL CDlgAdminCharModify::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog ();

	m_ctlMoveType.AddString ("�v���C���[�L����");
	m_ctlMoveType.SetItemData (0, CHARMOVETYPE_PC);
	m_ctlMoveType.AddString ("�ړ����Ȃ�");
	m_ctlMoveType.SetItemData (1, CHARMOVETYPE_STAND);
	m_ctlMoveType.AddString ("�{�[��");
	m_ctlMoveType.SetItemData (2, CHARMOVETYPE_BALL);
	m_ctlMoveType.AddString ("���_");
	m_ctlMoveType.SetItemData (3, CHARMOVETYPE_SCORE);
	m_ctlMoveType.AddString ("�e�p�R�s�[(��荞��)");
	m_ctlMoveType.SetItemData (4, CHARMOVETYPE_STYLECOPY_PUT);
	m_ctlMoveType.AddString ("�U���󂯂�ƃA�j���[�V����");
	m_ctlMoveType.SetItemData (5, CHARMOVETYPE_ATACKANIME);
	m_ctlMoveType.AddString ("�e�p�R�s�[(���f)");
	m_ctlMoveType.SetItemData (6, CHARMOVETYPE_STYLECOPY_GET);
	m_ctlMoveType.AddString ("NPC����");
	m_ctlMoveType.SetItemData (7, CHARMOVETYPE_PUTNPC);
	m_ctlMoveType.AddString ("�퓬1");
	m_ctlMoveType.SetItemData (8, CHARMOVETYPE_BATTLE1);
	m_ctlMoveType.SetCurSel (0);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::OnSend									 */
/* ���e		:�{�^���n���h��(���M)											 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharModify::OnSend()
{
	Send (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::OnTalk									 */
/* ���e		:�{�^���n���h��(��b�f�[�^�̕ҏW)								 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CDlgAdminCharModify::OnTalk()
{
	int nResult;
	CDlgAdminTalkSet Dlg;

	if (m_pInfoChar == NULL) {
		return;
	}

	Dlg.m_strTalk = m_pInfoChar->m_strTalk;
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
	m_pInfoChar->m_strTalk = Dlg.m_strTalk;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::OnSetMoveType								 */
/* ���e		:�{�^���n���h��(�ړ���ʂ̐ݒ�)									 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModify::OnSetMoveType()
{
	int nMoveType, nResult;
	PCDlgAdminCharModifyTypeBase pDlg;

	pDlg = NULL;

	nMoveType = m_ctlMoveType.GetItemData (m_ctlMoveType.GetCurSel ());
	switch (nMoveType) {
	case CHARMOVETYPE_PUTNPC:	/* NPC���� */
		pDlg = new CDlgAdminCharModifyTypePUTNPC(this);
		break;
	}

	if (pDlg) {
		pDlg->Set (m_pInfoChar);
		nResult = pDlg->DoModal ();
		if (nResult == IDOK) {
			pDlg->Get (m_pInfoChar);
		}
	}

	SAFE_DELETE (pDlg);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::Send										 */
/* ���e		:���M����														 */
/* ���t		:2007/09/16														 */
/* ========================================================================= */

void CDlgAdminCharModify::Send(BOOL bChgScreenPos)
{
	CInfoCharCli InfoCharTmp;
	CPacketADMIN_CHARINFO Packet;

	if (m_pInfoChar == NULL) {
		return;
	}

	UpdateData ();

	InfoCharTmp.Create (m_pMgrData);
	InfoCharTmp.Copy (m_pInfoChar);
	InfoCharTmp.m_strCharName		= m_strCharName;
	InfoCharTmp.m_bBlock			= m_bBlock;
	InfoCharTmp.m_bPush				= m_bPush;
	InfoCharTmp.m_nMoveType			= m_ctlMoveType.GetItemData (m_ctlMoveType.GetCurSel ());
	InfoCharTmp.m_dwMapID			= m_dwMapID;
	InfoCharTmp.m_nMapX				= m_nPosX;
	InfoCharTmp.m_nMapY				= m_nPosY;
	InfoCharTmp.m_dwMotionTypeID	= m_ctlMotionType.GetItemData (m_ctlMotionType.GetCurSel ());
	InfoCharTmp.m_nSex				= m_ctlSex.GetItemData (m_ctlSex.GetCurSel ());

	Packet.Make (&InfoCharTmp, bChgScreenPos);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2007 */
