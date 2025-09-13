/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminSelectItem.h										 */
/* 内容			:アイテム選択ダイアログクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/24													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "ListCtrlItemInfo.h"

class CLibInfoItem;
class CInfoItem;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminSelectItem : public CDlgAdminBase
{
public:
			CDlgAdminSelectItem(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminSelectItem();							/* デストラクタ */

	DWORD	GetSelectItemID	(void);			/* 選択されたアイテムIDを取得 */


protected:
	DWORD	m_dwSelectItemID;				/* 選択されたアイテムID */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminSelectItem)
	enum { IDD = IDD_SELECT_ITEM };
	CListCtrlItemInfo	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminSelectItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminSelectItem)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminSelectItem, *PCDlgAdminSelectItem;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
