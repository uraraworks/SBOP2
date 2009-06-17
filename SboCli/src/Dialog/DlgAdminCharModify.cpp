/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminCharModify.cpp										 */
/* ���e			:�I���L�������ҏW�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/03/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "command.h"
#include "InfoTalkEvent.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_CHARINFO.h"
#include "PacketADMIN_CHAR_RENEW_TALKEVENT.h"
#include "PacketCHAR_PARA1.h"
#include "PacketADMIN_PARA2.h"
#include "PacketCHAR_REQ_CHARINFO.h"
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

#define COLORCOUNT	(14)		/* �F��� */

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
	DDX_Control(pDX, IDC_COLOR, m_ctlColor);
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
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
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

	m_dwCharID			= 0;
	m_pInfoChar			= NULL;
	m_pInfoTalkEvent	= NULL;
	Create (CDlgAdminCharModify::IDD, pParent);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::~CDlgAdminCharModify						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/03/17														 */
/* ========================================================================= */

CDlgAdminCharModify::~CDlgAdminCharModify()
{
	SAFE_DELETE (m_pInfoTalkEvent);
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
	m_ctlColor.SetCurSel (0);

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

	m_ctlColor.ResetContent ();
	m_ctlColor.AddString ("��");
	m_ctlColor.AddString ("��");
	m_ctlColor.AddString ("��");
	m_ctlColor.AddString ("��");
	m_ctlColor.AddString ("��");
	m_ctlColor.AddString ("��");
	m_ctlColor.AddString ("��");
	m_ctlColor.AddString ("(�Z)��");
	m_ctlColor.AddString ("(�Z)��");
	m_ctlColor.AddString ("(�Z)��");
	m_ctlColor.AddString ("(�Z)��");
	m_ctlColor.AddString ("(�Z)����");
	m_ctlColor.AddString ("(�Z)��");
	m_ctlColor.AddString ("(�Z)��");
	for (i = 0; i < COLORCOUNT; i ++) {
		if (m_pInfoChar->m_clName == GetColor (i)) {
			break;
		}
	}
	if (i >= COLORCOUNT) {
		i = 6;
	}
	m_ctlColor.SetCurSel (i);

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
	DWORD dwCharID;
	PCInfoCharCli pInfoCharTmp;
	PCLibInfoCharCli pLibInfoChar;
	CPacketCHAR_PARA1 Packet;

	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:	/* ���N���b�N�ʒm */
		dwCharID = dwPara;
		if (dwPara == (DWORD)-1) {
			dwCharID = m_dwCharID;
		}
		pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
		pInfoCharTmp	= (PCInfoCharCli)pLibInfoChar->GetPtr (dwCharID);
		if (pInfoCharTmp == NULL) {
			if (dwPara == (DWORD)-1) {
				CPacketCHAR_REQ_CHARINFO PacketCHAR_REQ_CHARINFO;

				PacketCHAR_REQ_CHARINFO.Make (m_dwCharID);
				m_pSock->Send (&PacketCHAR_REQ_CHARINFO);
				break;
			}
			dwCharID = 0;
		}
		m_pInfoChar = pInfoCharTmp;
		m_dwCharID = dwCharID;
		Renew ();
		if (m_dwCharID != 0) {
			Packet.Make (SBOCOMMANDID_SUB_CHAR_REQ_TALKEVENT, m_dwCharID, m_dwCharID);
			m_pSock->Send (&Packet);
		}
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:	/* �E�N���b�N�ʒm */
		m_dwMapID	= m_pMgrData->GetMapID ();
		m_nPosX		= HIWORD (dwPara);
		m_nPosY		= LOWORD (dwPara);
		UpdateData (FALSE);
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDBLCLK:	/* �E�_�u���N���b�N�ʒm */
		m_nPosX = HIWORD (dwPara);
		m_nPosY = LOWORD (dwPara);
		UpdateData (FALSE);
		Send (TRUE);
		break;
	case ADMINMSG_RENEWTALKEVENT:			/* ��b�C�x���g���X�V */
		if (m_pInfoChar == NULL) {
			break;
		}
		if (dwPara != m_pInfoChar->m_dwCharID) {
			break;
		}
		m_pInfoTalkEvent->Copy (m_pMgrData->GetInfoTalkEvent ());
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
	m_ctlMoveType.AddString ("�퓬2");
	m_ctlMoveType.SetItemData (9, CHARMOVETYPE_BATTLE2);
	m_ctlMoveType.SetCurSel (0);

	m_pInfoTalkEvent = new CInfoTalkEvent;

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
/* �֐���	:CDlgAdminCharModify::OnDelete									 */
/* ���e		:�{�^���n���h��(�폜)											 */
/* ���t		:2008/11/28														 */
/* ========================================================================= */

void CDlgAdminCharModify::OnDelete()
{
	int nResult;
	CString strTmp;
	CPacketADMIN_PARA2 Packet;

	if (m_pInfoChar == NULL) {
		return;
	}
	if (m_pInfoChar->IsNPC () == FALSE) {
		return;
	}

	strTmp.Format ("[%s]���폜���܂����H", (LPCSTR)m_pInfoChar->m_strCharName);
	nResult = MessageBox (strTmp, "�m�F", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (SBOCOMMANDID_SUB_ADMIN_DELETECHARINFO, m_pInfoChar->m_dwCharID);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::OnTalk									 */
/* ���e		:�{�^���n���h��(��b�f�[�^�̕ҏW)								 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CDlgAdminCharModify::OnTalk()
{
	int nResult;
	CDlgAdminTalkSet Dlg(this);

	if (m_pInfoChar == NULL) {
		return;
	}

	Dlg.Init (m_pMgrData);
	Dlg.m_pInfo->Copy (m_pInfoTalkEvent);
	Dlg.m_strTalk = m_pInfoChar->m_strTalk;
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
	m_pInfoChar->m_strTalk = Dlg.m_strTalk;
	m_pInfoTalkEvent->Copy (Dlg.m_pInfo);
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
	PCLibInfoCharCli pLibInfoChar;
	CPacketADMIN_CHARINFO Packet;
	CPacketADMIN_CHAR_RENEW_TALKEVENT PacketADMIN_CHAR_RENEW_TALKEVENT;

	pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
	m_pInfoChar		= (PCInfoCharCli)pLibInfoChar->GetPtr (m_dwCharID);
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
	InfoCharTmp.m_clName			= GetColor (m_ctlColor.GetCurSel ());
	InfoCharTmp.m_clSpeak			= InfoCharTmp.m_clName;

	Packet.Make (&InfoCharTmp, bChgScreenPos);
	m_pSock->Send (&Packet);

	m_pInfoTalkEvent->m_dwTalkEventID = m_dwCharID;
	PacketADMIN_CHAR_RENEW_TALKEVENT.Make (m_pInfoTalkEvent, m_dwCharID);
	m_pSock->Send (&PacketADMIN_CHAR_RENEW_TALKEVENT);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminCharModify::GetColor									 */
/* ���e		:�ԍ�����F���擾												 */
/* ���t		:2008/11/23														 */
/* ========================================================================= */

COLORREF CDlgAdminCharModify::GetColor(int nNo)
{
	COLORREF clRet, acl[] = {
		RGB (255, 200, 200),	/* �� */
		RGB (255, 255, 200),	/* �� */
		RGB (200, 255, 200),	/* �� */
		RGB (200, 255, 255),	/* �� */
		RGB (200, 200, 255),	/* �� */
		RGB (255, 200, 255),	/* �� */
		RGB (255, 255, 255),	/* �� */
		RGB (255, 150, 150),	/* �ԁi�s���N�j */
		RGB (255, 200, 100),	/* �� */
		RGB (150, 150, 255),	/* �� */
		RGB (200, 100, 255),	/* �� */
		RGB (200, 255, 150),	/* ���� */
		RGB (150, 255, 150),	/* �� */
		RGB (100, 200, 255),	/* �� */
	};

	if (nNo >= COLORCOUNT) {
		clRet = RGB (255, 255, 255);
	} else {
		clRet = acl[nNo];
	}

	return clRet;
}

/* Copyright(C)URARA-works 2007 */
