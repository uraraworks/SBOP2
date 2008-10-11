/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminServerInfo.cpp										 */
/* ���e			:�T�[�o�[���_�C�A���O�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "PacketADMIN_SERVER_SAVEINFO.h"
#include "PacketADMIN_REQ_PLAYSOUND.h"
#include "PacketADMIN_RENEW_CLIENTVERSION.h"
#include "UraraSockTCPSBO.h"
#include "SBOVersion.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "DlgAdminServerInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminServerInfo::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminServerInfo)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminServerInfo, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminServerInfo)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PLAYSOUND, &CDlgAdminServerInfo::OnBnClickedPlaysound)
	ON_BN_CLICKED(IDC_RENEW_CLIENTVERSION, &CDlgAdminServerInfo::OnBnClickedRenewClientversion)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminServerInfo::CDlgAdminServerInfo						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/08														 */
/* ========================================================================= */

CDlgAdminServerInfo::CDlgAdminServerInfo(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminServerInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminServerInfo)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminServerInfo::~CDlgAdminServerInfo						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/08														 */
/* ========================================================================= */

CDlgAdminServerInfo::~CDlgAdminServerInfo()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminServerInfo::Init										 */
/* ���e		:������															 */
/* ���t		:2007/07/08														 */
/* ========================================================================= */

void CDlgAdminServerInfo::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* �E�B���h�E�쐬 */
	Create (CDlgAdminServerInfo::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminServerInfo::OnSave									 */
/* ���e		:�{�^���n���h��(�T�[�o�[���ۑ�)								 */
/* ���t		:2007/07/08														 */
/* ========================================================================= */

void CDlgAdminServerInfo::OnSave()
{
	CPacketADMIN_SERVER_SAVEINFO Packet;

	Packet.Make ();
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminServerInfo::OnBnClickedPlaysound						 */
/* ���e		:�{�^���n���h��(�����O�̂��m�点��)								 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

void CDlgAdminServerInfo::OnBnClickedPlaysound()
{
	CPacketADMIN_REQ_PLAYSOUND Packet;

	Packet.Make (SOUNDID_PINPONPANPON);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminServerInfo::OnBnClickedRenewClientversion				 */
/* ���e		:�{�^���n���h��(�N���C�A���g�o�[�W�����X�V)						 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CDlgAdminServerInfo::OnBnClickedRenewClientversion()
{
	CPacketADMIN_RENEW_CLIENTVERSION Packet;

	Packet.Make (VERTEXT);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2007 */
