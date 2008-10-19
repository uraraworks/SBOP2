/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemTypeNewLIGHT.cpp								 */
/* ���e			:�A�C�e�����(����)�ݒ�_�C�A���O�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/19													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoItemTypeBase.h"
#include "MgrData.h"
#include "DlgAdminItemTypeNewLIGHT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminItemTypeNewLIGHT::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemTypeNewLIGHT)
	DDX_Text(pDX, IDC_VALUE1, m_nValue1);
	DDX_Text(pDX, IDC_VALUE2, m_nValue2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNewLIGHT, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemTypeNewLIGHT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewLIGHT::CDlgAdminItemTypeNewLIGHT			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/10/19														 */
/* ========================================================================= */

CDlgAdminItemTypeNewLIGHT::CDlgAdminItemTypeNewLIGHT(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemTypeNewLIGHT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemTypeNewLIGHT)
	m_nValue1 = 0;
	m_nValue2 = 0;
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewLIGHT::~CDlgAdminItemTypeNewLIGHT			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/10/19														 */
/* ========================================================================= */

CDlgAdminItemTypeNewLIGHT::~CDlgAdminItemTypeNewLIGHT()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewLIGHT::Set									 */
/* ���e		:�ҏW���e��ݒ�													 */
/* ���t		:2008/10/19														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewLIGHT::Set(CInfoItemTypeBase *pSrc)
{
	m_nValue1	= (int)pSrc->m_dwValue;		/* ���背�x�� */
	m_nValue2	= (int)pSrc->m_dwValue2;	/* �������� */
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewLIGHT::Get									 */
/* ���e		:�ҏW���e���擾													 */
/* ���t		:2008/10/19														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewLIGHT::Get(CInfoItemTypeBase *&pDst)
{
	pDst->m_dwValue		= m_nValue1;		/* ���背�x�� */
	pDst->m_dwValue2	= m_nValue2;		/* �������� */
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewLIGHT::PostNcDestroy						 */
/* ���e		:�I������														 */
/* ���t		:2008/10/19														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewLIGHT::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}

/* Copyright(C)URARA-works 2008 */
