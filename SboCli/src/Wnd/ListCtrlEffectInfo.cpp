/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:ListCtrlEffectInfo.cpp										 */
/* 内容			:エフェクト一覧リストコントロールクラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
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
/* クラス設定																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CListCtrlEffectInfo, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEffectInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CListCtrlEffectInfo::CListCtrlEffectInfo						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

CListCtrlEffectInfo::CListCtrlEffectInfo()
{
	m_pWndParent		= NULL;
	m_pMgrData			= NULL;
	m_pLibInfoEffect	= NULL;
}


/* ========================================================================= */
/* 関数名	:CListCtrlEffectInfo::~CListCtrlEffectInfo						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

CListCtrlEffectInfo::~CListCtrlEffectInfo()
{
}


/* ========================================================================= */
/* 関数名	:CListCtrlEffectInfo::Create									 */
/* 内容		:作成															 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

BOOL CListCtrlEffectInfo::Create(CWnd *pParent, CMgrData *pMgrData)
{
	m_pWndParent		= pParent;
	m_pMgrData			= pMgrData;
	m_pLibInfoEffect	= m_pMgrData->GetLibInfoEffect ();

	SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	InsertColumn (0, _T("ID"),				LVCFMT_LEFT, 40);
	InsertColumn (1, _T("エフェクト名"),	LVCFMT_LEFT, 120);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CListCtrlEffectInfo::Renew										 */
/* 内容		:更新															 */
/* 日付		:2008/08/11														 */
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
		strTmp.Format(_T("%d"), pInfoEffect->m_dwEffectID);
		InsertItem (i, strTmp);
		SetItemData (i, pInfoEffect->m_dwEffectID);				/* ID */
		SetItemText (i, 1, (LPCTSTR)pInfoEffect->m_strName);		/* エフェクト名 */
	}
}

/* Copyright(C)URARA-works 2008 */
