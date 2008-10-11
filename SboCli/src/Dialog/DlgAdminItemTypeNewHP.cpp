/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemTypeNewHP.cpp									 */
/* ���e			:�A�C�e�����(HP����)�ݒ�_�C�A���O�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/15													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoItemTypeBase.h"
#include "InfoMotion.h"
#include "MgrData.h"
#include "DlgAdminItemTypeNewHP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemTypeNewHP)
	DDX_Control(pDX, IDC_TARGET, m_ctlTarget);
	DDX_Control(pDX, IDC_AREA, m_ctlArea);
	DDX_Text(pDX, IDC_VALUE1, m_nValue1);
	DDX_Text(pDX, IDC_VALUE2, m_nValue2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNewHP, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemTypeNewHP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::CDlgAdminItemTypeNewHP					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

CDlgAdminItemTypeNewHP::CDlgAdminItemTypeNewHP(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemTypeNewHP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemTypeNewHP)
	m_nValue1 = 0;
	m_nValue2 = 0;
	//}}AFX_DATA_INIT

	m_byTarget = 0;		/* �g�p�Ώ� */
	m_byArea = 0;		/* �g�p�͈� */
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::~CDlgAdminItemTypeNewHP				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

CDlgAdminItemTypeNewHP::~CDlgAdminItemTypeNewHP()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::Set									 */
/* ���e		:�ҏW���e��ݒ�													 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::Set(CInfoItemTypeBase *pSrc)
{
	m_byTarget	= pSrc->m_byTarget;			/* �g�p�Ώ� */
	m_byArea	= pSrc->m_byArea;			/* �g�p�͈� */
	m_nValue1	= (int)pSrc->m_dwValue;	/* �ŏ� */
	m_nValue2	= (int)pSrc->m_dwValue2;	/* �ő� */
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::Get									 */
/* ���e		:�ҏW���e���擾													 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::Get(CInfoItemTypeBase *&pDst)
{
	pDst->m_byTarget	= m_byTarget;			/* �g�p�Ώ� */
	pDst->m_byArea		= m_byArea;				/* �g�p�͈� */
	pDst->m_dwValue		= m_nValue1;			/* �ŏ� */
	pDst->m_dwValue2	= m_nValue2;			/* �ő� */
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::OnInitDialog							 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

BOOL CDlgAdminItemTypeNewHP::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_ctlTarget.InsertString (0, "����");
	m_ctlTarget.InsertString (1, "�G");

	m_ctlArea.InsertString (0, "����");
	m_ctlArea.InsertString (1, "1���O");
	m_ctlArea.InsertString (2, "����");
	m_ctlArea.InsertString (3, "��ʓ�");

	SetTarget (m_byTarget);
	SetArea (m_byArea);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::OnOK									 */
/* ���e		:�{�^���n���h��(OK)												 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::OnOK()
{
	UpdateData ();

	m_byTarget	= GetTarget ();
	m_byArea	= GetArea ();

	CDlgAdminBase::EndDialog (IDOK);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::SetTarget								 */
/* ���e		:�g�p�Ώۂ�ݒ�													 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::SetTarget(BYTE byTarget)
{
	m_ctlTarget.SetCurSel (byTarget);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::GetTarget								 */
/* ���e		:�g�p�Ώۂ��擾													 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

BYTE CDlgAdminItemTypeNewHP::GetTarget(void)
{
	int nSelect;
	BYTE byRet;

	byRet	= 0;
	nSelect	= m_ctlTarget.GetCurSel ();
	if (nSelect >= 0) {
		byRet = (BYTE)nSelect;
	}

	return byRet;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::SetArea								 */
/* ���e		:�g�p�͈͂�ݒ�													 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewHP::SetArea(BYTE byTarget)
{
	m_ctlArea.SetCurSel (byTarget);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewHP::GetArea								 */
/* ���e		:�g�p�͈͂��擾													 */
/* ���t		:2008/08/15														 */
/* ========================================================================= */

BYTE CDlgAdminItemTypeNewHP::GetArea(void)
{
	int nSelect;
	BYTE byRet;

	byRet	= 0;
	nSelect	= m_ctlArea.GetCurSel ();
	if (nSelect >= 0) {
		byRet = (BYTE)nSelect;
	}

	return byRet;
}

/* Copyright(C)URARA-works 2008 */
