/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:ListCtrlItemInfo.cpp										 */
/* ���e			:�A�C�e���ꗗ���X�g�R���g���[���N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoItem.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "ListCtrlItemInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CListCtrlItemInfo, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlItemInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CListCtrlItemInfo::CListCtrlItemInfo							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

CListCtrlItemInfo::CListCtrlItemInfo()
{
	m_pWndParent		= NULL;
	m_pMgrData			= NULL;
	m_pLibInfoItem		= NULL;
}


/* ========================================================================= */
/* �֐���	:CListCtrlItemInfo::~CListCtrlItemInfo							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

CListCtrlItemInfo::~CListCtrlItemInfo()
{
}


/* ========================================================================= */
/* �֐���	:CListCtrlItemInfo::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

BOOL CListCtrlItemInfo::Create(CWnd *pParent, CMgrData *pMgrData)
{
	m_pWndParent	= pParent;
	m_pMgrData		= pMgrData;
	m_pLibInfoItem	= m_pMgrData->GetLibInfoItem ();

	SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	InsertColumn (1, "�A�C�e����",	LVCFMT_LEFT, 120);
	InsertColumn (2, "���",		LVCFMT_LEFT, 120);
	InsertColumn (3, "�����L����",	LVCFMT_LEFT, 120);
	InsertColumn (4, "�z�u���W",	LVCFMT_LEFT, 120);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CListCtrlItemInfo::Renew										 */
/* ���e		:�X�V															 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

void CListCtrlItemInfo::Renew(void)
{
	int i, nCount;
	PCInfoItem pInfoItem;
	PCInfoCharCli pInfoChar;
	PCLibInfoCharCli pLibInfoChar;
	CString strTmp;

	if (m_pMgrData == NULL) {
		return;
	}

	pLibInfoChar = m_pMgrData->GetLibInfoChar ();

	DeleteAllItems ();

	nCount = m_pLibInfoItem->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (i);
		strTmp.Format ("%d", pInfoItem->m_dwItemID);
		InsertItem (i, strTmp);
		SetItemData (i, pInfoItem->m_dwItemID);		/* ID */
		SetItemText (i, 1, pInfoItem->m_strName);	/* �A�C�e���� */

		strTmp = m_pLibInfoItem->GetTypeName (pInfoItem->m_dwItemTypeID);
		SetItemText (i, 2, strTmp);					/* ��� */

		strTmp.Empty ();
		pInfoChar = (PCInfoCharCli)pLibInfoChar->GetPtr (pInfoItem->m_dwCharID);
		if (pInfoChar) {
			strTmp.Format ("[%u]%s", pInfoChar->m_dwCharID, (LPCSTR)pInfoChar->m_strCharName);
		}
		SetItemText (i, 3, strTmp);					/* �����L���� */

		strTmp.Empty ();
		if (pInfoItem->m_dwMapID) {
			strTmp.Format ("MAPID:%u (%d, %d)", pInfoItem->m_dwMapID, pInfoItem->m_ptPos.x, pInfoItem->m_ptPos.y);
		}
		SetItemText (i, 4, strTmp);					/* �z�u���W */
	}
}

/* Copyright(C)URARA-works 2007 */
