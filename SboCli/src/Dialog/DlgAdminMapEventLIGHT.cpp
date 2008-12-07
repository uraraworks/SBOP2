/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventLIGHT.cpp									 */
/* ���e			:�}�b�v�C�x���g(����)�_�C�A���O�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "InfoMapEventLIGHT.h"
#include "DlgAdminMapEventLIGHT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapEventLIGHT::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminMapEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventLIGHT)
	DDX_Text(pDX, IDC_TIME, m_dwTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventLIGHT, CDlgAdminMapEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminMapEventLIGHT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventLIGHT::CDlgAdminMapEventLIGHT					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminMapEventLIGHT::CDlgAdminMapEventLIGHT(CWnd* pParent /*=NULL*/)
	: CDlgAdminMapEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventLIGHT)
	m_dwTime = 0;
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventLIGHT::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventLIGHT::~CDlgAdminMapEventLIGHT				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminMapEventLIGHT::~CDlgAdminMapEventLIGHT()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventLIGHT::Set									 */
/* ���e		:�ݒ�															 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminMapEventLIGHT::Set(CInfoMapEventBase *pSrc)
{
	int nSelect;
	PCInfoMapEventLIGHT pSrcTmp = (PCInfoMapEventLIGHT)pSrc;

	m_dwTime = pSrcTmp->m_dwTime;

	nSelect = (pSrcTmp->m_bLightOn) ? IDC_RADIO_ON : IDC_RADIO_OFF;
	CheckRadioButton (IDC_RADIO_OFF, IDC_RADIO_ON, nSelect);

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventLIGHT::Get									 */
/* ���e		:�擾															 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminMapEventLIGHT::Get(CInfoMapEventBase *pDst)
{
	int nSelect;

	PCInfoMapEventLIGHT pDstTmp = (PCInfoMapEventLIGHT)pDst;

	UpdateData ();

	nSelect = GetCheckedRadioButton (IDC_RADIO_OFF, IDC_RADIO_ON);
	pDstTmp->m_bLightOn	= (nSelect == IDC_RADIO_ON) ? TRUE : FALSE;
	pDstTmp->m_dwTime	= m_dwTime;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventLIGHT::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventLIGHT::OnInitDialog()
{
	CDlgAdminMapEventNONE::OnInitDialog();

	CheckRadioButton (IDC_RADIO_OFF, IDC_RADIO_ON, IDC_RADIO_OFF);

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
