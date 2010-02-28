/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:DlgAdminItemTypeNewARMSBow.cpp								 */
/* ���e			:�A�C�e�����(������[�|�p�̐ݒ�])�ݒ�_�C�A���O�N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/08/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SboCli.h"
#include "InfoItemTypeBase.h"
#include "DlgAdminItemTypeNewARMSBow.h"

/* ========================================================================= */
/* �N���X�̐ݒ�																 */
/* ========================================================================= */

IMPLEMENT_DYNAMIC(CDlgAdminItemTypeNewARMSBow, CDlgAdminBase)

void CDlgAdminItemTypeNewARMSBow::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MOVEWAIT, m_dwMoveWait);
	DDX_Text(pDX, IDC_MOVECOUNT, m_dwMoveCount);
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNewARMSBow, CDlgAdminBase)
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMSBow::CDlgAdminItemTypeNewARMSBow		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/08/29														 */
/* ========================================================================= */

CDlgAdminItemTypeNewARMSBow::CDlgAdminItemTypeNewARMSBow(CWnd* pParent /*=NULL*/)
: CDlgAdminBase(CDlgAdminItemTypeNewARMSBow::IDD, pParent)
, m_dwMoveWait(0)
, m_dwMoveCount(0)
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMSBow::~CDlgAdminItemTypeNewARMSBow		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/08/29														 */
/* ========================================================================= */

CDlgAdminItemTypeNewARMSBow::~CDlgAdminItemTypeNewARMSBow()
{
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMSBow::Set								 */
/* ���e		:�ҏW���e��ݒ�													 */
/* ���t		:2009/08/29														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMSBow::Set(CInfoItemTypeBase *pSrc)
{
	m_dwMoveWait	= pSrc->m_dwMoveWait;			/* ���x */
	m_dwMoveCount	= pSrc->m_dwMoveCount;			/* �򋗗� */

	UpdateData(FALSE);
}


/* ========================================================================= */
/* �֐���	:CDlgAdminItemTypeNewARMSBow::Get								 */
/* ���e		:�ҏW���e���擾													 */
/* ���t		:2009/08/29														 */
/* ========================================================================= */

void CDlgAdminItemTypeNewARMSBow::Get(CInfoItemTypeBase *&pDst)
{
	UpdateData();

	pDst->m_dwMoveWait	= m_dwMoveWait;			/* ���x */
	pDst->m_dwMoveCount	= m_dwMoveCount;		/* �򋗗� */
}

/* Copyright(C)URARA-works 2009 */
