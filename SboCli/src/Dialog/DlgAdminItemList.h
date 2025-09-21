/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemList.h											 */
/* 内容			:アイテム一覧ダイアログクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/14													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoItemType;
class CLibInfoItem;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminItemList : public CDlgAdminBase
{
public:
			CDlgAdminItemList(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminItemList();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	Renew		(void);										/* 一覧を更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);			/* メッセージハンドラ(WM_MAINFRAME) */


protected:
	int		GetIndex	(DWORD dwItemID);							/* アイテムIDからインデックスを取得 */


protected:
	CWnd				*m_pWndNotify;				/* 通知先ウィンドウ */
	CLibInfoItemType	*m_pLibInfoItemType;		/* アイテム種別情報 */
	CLibInfoItem		*m_pLibInfoItem;			/* 編集中のアイテム情報 */



public:
	//{{AFX_DATA(CDlgAdminItemList)
	enum { IDD = IDD_ITEM_LIST };
	CListCtrl	m_List;
	BOOL m_bDrop;
	BOOL m_bMap;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminItemList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminItemList)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	afx_msg void OnRenew();
	afx_msg void OnBnClickedDrop();
	afx_msg void OnBnClickedMap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminItemList, *PCDlgAdminItemList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
