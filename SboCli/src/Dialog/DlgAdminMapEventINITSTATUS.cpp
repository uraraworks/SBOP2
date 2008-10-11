/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminMapEventINITSTATUS.cpp								 */
/* ���e			:�}�b�v�C�x���g(�X�e�[�^�X������)�_�C�A���O�N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventINITSTATUS.h"
#include "LibInfoEffect.h"
#include "MgrData.h"
#include "DlgAdminMapEventINITSTATUS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

void CDlgAdminMapEventINITSTATUS::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminMapEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventINITSTATUS)
	DDX_Control(pDX, IDC_EFFECTID, m_ctlEffectID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventINITSTATUS, CDlgAdminMapEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminMapEventINITSTATUS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventINITSTATUS::CDlgAdminMapEventINITSTATUS		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

CDlgAdminMapEventINITSTATUS::CDlgAdminMapEventINITSTATUS(CWnd* pParent /*=NULL*/)
	: CDlgAdminMapEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventINITSTATUS)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventINITSTATUS::IDD;

	m_dwEffectID = 0;
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventINITSTATUS::~CDlgAdminMapEventINITSTATUS		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

CDlgAdminMapEventINITSTATUS::~CDlgAdminMapEventINITSTATUS()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventINITSTATUS::Set								 */
/* ���e		:�ݒ�															 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

void CDlgAdminMapEventINITSTATUS::Set(CInfoMapEventBase *pSrc)
{
	int i, nNo, nCount;
	PCInfoMapEventINITSTATUS pSrcTmp = (PCInfoMapEventINITSTATUS)pSrc;

	m_dwEffectID = pSrcTmp->m_dwEffectID;

	/* �G�t�F�N�gID */
	nNo = 0;
	nCount = m_ctlEffectID.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_dwEffectID == m_ctlEffectID.GetItemData (i)) {
			nNo = i;
			break;
		}
	}
	m_ctlEffectID.SetCurSel (nNo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventINITSTATUS::Get								 */
/* ���e		:�擾															 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

void CDlgAdminMapEventINITSTATUS::Get(CInfoMapEventBase *pDst)
{
	int nNo;
	PCInfoMapEventINITSTATUS pDstTmp = (PCInfoMapEventINITSTATUS)pDst;

	nNo = m_ctlEffectID.GetCurSel ();
	pDstTmp->m_dwEffectID = m_ctlEffectID.GetItemData (nNo);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminMapEventINITSTATUS::OnInitDialog						 */
/* ���e		:���b�Z�[�W�n���h��(WM_INITDIALOG)								 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventINITSTATUS::OnInitDialog()
{
	int i, nCount;
	PCInfoEffect pInfoEffect;
	PCLibInfoEffect pLibInfoEffect;

	CDlgAdminMapEventNONE::OnInitDialog();

	/* �G�t�F�N�gID */
	m_ctlEffectID.InsertString (0, "����");
	pLibInfoEffect = m_pMgrData->GetLibInfoEffect ();
	nCount = pLibInfoEffect->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)pLibInfoEffect->GetPtr (i);
		m_ctlEffectID.InsertString (i + 1, (LPCTSTR)pInfoEffect->m_strName);
		m_ctlEffectID.SetItemData (i + 1, pInfoEffect->m_dwEffectID);
	}

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
