/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:ListCtrlEffectInfo.cpp										 */
/* ���e			:�G�t�F�N�g�ꗗ���X�g�R���g���[���N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoEffect.h"
#include "MgrData.h"
#include "ListCtrlEffectInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CListCtrlEffectInfo, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEffectInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CListCtrlEffectInfo::CListCtrlEffectInfo						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CListCtrlEffectInfo::CListCtrlEffectInfo()
{
	m_pWndParent		= NULL;
	m_pMgrData			= NULL;
	m_pLibInfoEffect	= NULL;
}


/* ========================================================================= */
/* �֐���	:CListCtrlEffectInfo::~CListCtrlEffectInfo						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CListCtrlEffectInfo::~CListCtrlEffectInfo()
{
}


/* ========================================================================= */
/* �֐���	:CListCtrlEffectInfo::Create									 */
/* ���e		:�쐬															 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

BOOL CListCtrlEffectInfo::Create(CWnd *pParent, CMgrData *pMgrData)
{
	m_pWndParent		= pParent;
	m_pMgrData			= pMgrData;
	m_pLibInfoEffect	= m_pMgrData->GetLibInfoEffect ();

	SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	InsertColumn (0, "ID",				LVCFMT_LEFT, 40);
	InsertColumn (1, "�G�t�F�N�g��",	LVCFMT_LEFT, 120);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CListCtrlEffectInfo::Renew										 */
/* ���e		:�X�V															 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CListCtrlEffectInfo::Renew(void)
{
	int i, nCount;
	PCInfoEffect pInfoEffect;
	CString strTmp;

	if (m_pMgrData == NULL) {
		return;
	}

	DeleteAllItems ();

	nCount = m_pLibInfoEffect->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)m_pLibInfoEffect->GetPtr (i);
		strTmp.Format ("%d", pInfoEffect->m_dwEffectID);
		InsertItem (i, strTmp);
		SetItemData (i, pInfoEffect->m_dwEffectID);				/* ID */
		SetItemText (i, 1, (LPCSTR)pInfoEffect->m_strName);		/* �G�t�F�N�g�� */
	}
}

/* Copyright(C)URARA-works 2008 */
