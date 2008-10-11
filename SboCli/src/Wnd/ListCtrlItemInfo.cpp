/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:ListCtrlItemInfo.cpp										 */
/* 内容			:アイテム一覧リストコントロールクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/24													 */
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
/* クラス設定																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CListCtrlItemInfo, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlItemInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CListCtrlItemInfo::CListCtrlItemInfo							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

CListCtrlItemInfo::CListCtrlItemInfo()
{
	m_pWndParent		= NULL;
	m_pMgrData			= NULL;
	m_pLibInfoItem		= NULL;
}


/* ========================================================================= */
/* 関数名	:CListCtrlItemInfo::~CListCtrlItemInfo							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

CListCtrlItemInfo::~CListCtrlItemInfo()
{
}


/* ========================================================================= */
/* 関数名	:CListCtrlItemInfo::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

BOOL CListCtrlItemInfo::Create(CWnd *pParent, CMgrData *pMgrData)
{
	m_pWndParent	= pParent;
	m_pMgrData		= pMgrData;
	m_pLibInfoItem	= m_pMgrData->GetLibInfoItem ();

	SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	InsertColumn (1, "アイテム名",	LVCFMT_LEFT, 120);
	InsertColumn (2, "種別",		LVCFMT_LEFT, 120);
	InsertColumn (3, "所持キャラ",	LVCFMT_LEFT, 120);
	InsertColumn (4, "配置座標",	LVCFMT_LEFT, 120);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CListCtrlItemInfo::Renew										 */
/* 内容		:更新															 */
/* 日付		:2007/09/24														 */
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
		SetItemText (i, 1, pInfoItem->m_strName);	/* アイテム名 */

		strTmp = m_pLibInfoItem->GetTypeName (pInfoItem->m_dwItemTypeID);
		SetItemText (i, 2, strTmp);					/* 種別 */

		strTmp.Empty ();
		pInfoChar = (PCInfoCharCli)pLibInfoChar->GetPtr (pInfoItem->m_dwCharID);
		if (pInfoChar) {
			strTmp.Format ("[%u]%s", pInfoChar->m_dwCharID, (LPCSTR)pInfoChar->m_strCharName);
		}
		SetItemText (i, 3, strTmp);					/* 所持キャラ */

		strTmp.Empty ();
		if (pInfoItem->m_dwMapID) {
			strTmp.Format ("MAPID:%u (%d, %d)", pInfoItem->m_dwMapID, pInfoItem->m_ptPos.x, pInfoItem->m_ptPos.y);
		}
		SetItemText (i, 4, strTmp);					/* 配置座標 */
	}
}

/* Copyright(C)URARA-works 2007 */
