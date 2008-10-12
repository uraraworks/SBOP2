/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventGRPIDTMP.cpp								 */
/* ���e			:�}�b�v�C�x���g(�ꎞ�摜�ݒ�)�_�C�A���O�N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "InfoMapEventGRPIDTMP.h"
#include "DlgAdminMapEventGRPIDTMP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapEventGRPIDTMP::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminMapEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventGRPIDTMP)
	DDX_Control(pDX, IDC_IDMAIN, m_ctlIDMain);
	DDX_Text(pDX, IDC_IDSUB, m_dwIDSub);
	DDX_CBIndex(pDX, IDC_IDMAIN, m_nSelect);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventGRPIDTMP, CDlgAdminMapEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminMapEventGRPIDTMP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventGRPIDTMP::CDlgAdminMapEventGRPIDTMP			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

CDlgAdminMapEventGRPIDTMP::CDlgAdminMapEventGRPIDTMP(CWnd* pParent /*=NULL*/)
	: CDlgAdminMapEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventGRPIDTMP)
	m_nSelect = 0;
	m_dwIDSub = 0;
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventGRPIDTMP::IDD;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventGRPIDTMP::~CDlgAdminMapEventGRPIDTMP			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

CDlgAdminMapEventGRPIDTMP::~CDlgAdminMapEventGRPIDTMP()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventGRPIDTMP::Set									 */
/* ���e		:�ݒ�															 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

void CDlgAdminMapEventGRPIDTMP::Set(CInfoMapEventBase *pSrc)
{
	int i, nNo, nCount;
	PCInfoMapEventGRPIDTMP pSrcTmp = (PCInfoMapEventGRPIDTMP)pSrc;

	m_dwIDSub = pSrcTmp->m_dwIDSub;

	/* �摜ID���C�� */
	nNo = 0;
	nCount = m_ctlIDMain.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (pSrcTmp->m_dwIDMain == m_ctlIDMain.GetItemData (i)) {
			nNo = i;
			break;
		}
	}
	CheckRadioButton (IDC_RADIO_OFF, IDC_RADIO_ON, pSrcTmp->m_nSetType);
	m_ctlIDMain.SetCurSel (nNo);

	UpdateData (FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventGRPIDTMP::Get									 */
/* ���e		:�擾															 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

void CDlgAdminMapEventGRPIDTMP::Get(CInfoMapEventBase *pDst)
{
	int nSelect;

	PCInfoMapEventGRPIDTMP pDstTmp = (PCInfoMapEventGRPIDTMP)pDst;

	UpdateData ();

	nSelect = GetCheckedRadioButton (IDC_RADIO_OFF, IDC_RADIO_ON);
	pDstTmp->m_nSetType	= (nSelect == IDC_RADIO_ON) ? GRPIDTMPTYPE_ON : GRPIDTMPTYPE_OFF;
	pDstTmp->m_dwIDMain	= m_ctlIDMain.GetItemData (m_nSelect);
	pDstTmp->m_dwIDSub	= m_dwIDSub;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventGRPIDTMP::OnInitDialog						 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventGRPIDTMP::OnInitDialog()
{
	CDlgAdminMapEventNONE::OnInitDialog();

	/* �摜ID���C�� */
	m_ctlIDMain.InsertString (0, "��(2x2)");
	m_ctlIDMain.SetItemData  (0, GRPIDMAIN_2X2_CLOTH);
	m_ctlIDMain.InsertString (1, "���ꕞ(2x2)");
	m_ctlIDMain.SetItemData  (1, GRPIDMAIN_2X2_SPCLOTH);

	m_ctlIDMain.SetCurSel (0);
	CheckRadioButton (IDC_RADIO_OFF, IDC_RADIO_ON, IDC_RADIO_OFF);

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
