/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemTypeList.h										 */
/* 内容			:アイテム種別一覧ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/28													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoItemType;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminItemTypeList : public CDlgAdminBase
{
public:
			CDlgAdminItemTypeList(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminItemTypeList();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	Renew		(void);											/* 一覧を更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* メッセージハンドラ(WM_MAINFRAME) */


protected:
	int		GetIndex	(DWORD dwItemID);							/* アイテムIDからインデックスを取得 */


protected:
	CWnd				*m_pWndNotify;				/* 通知先ウィンドウ */
	CLibInfoItemType	*m_pLibInfoItemType;		/* 編集中のアイテム種別情報 */



public:
	//{{AFX_DATA(CDlgAdminItemTypeList)
	enum { IDD = IDD_ITEMTYPE_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemTypeList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemTypeList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	afx_msg void OnRenew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemTypeList, *PCDlgAdminItemTypeList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
