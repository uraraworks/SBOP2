/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharMotionList.h									 */
/* 内容			:キャラモーションリストダイアログクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/28													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharMotionList : public CDlgAdminBase
{
public:
			CDlgAdminCharMotionList(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminCharMotionList();							/* デストラクタ */

	void	Init	(CMgrData *pMgrData, DWORD dwMotionTypeID);			/* 初期化 */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* メッセージハンドラ(WM_MAINFRAME) */


protected:
	DWORD	m_dwMotionTypeID;		/* 編集中のモーション種別ID */



public:
	void	OnOK();
	//{{AFX_DATA(CDlgAdminCharMotionList)
	enum { IDD = IDD_CHAR_MOTION_LIST };
	int m_nGrpIDSub;
	CListCtrl	m_List;
	CString	m_strMotionName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharMotionList)
	protected:
	virtual void PostNcDestroy ();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharMotionList)
	virtual BOOL OnInitDialog();
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharMotionList, *PCDlgAdminCharMotionList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
