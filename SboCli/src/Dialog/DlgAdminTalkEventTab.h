/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventTab.h										 */
/* 内容			:会話イベントタブダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "afxcmn.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventTab : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventTab(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminTalkEventTab();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */


protected:


protected:



public:
//	void OnOK(){}
//	void OnCancel(){}
	//{{AFX_DATA(CDlgAdminTalkEventTab)
	enum { IDD = IDD_TALKEVENT_TAB };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventTab)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	afx_msg void OnBnClickedAdd();
} CDlgAdminTalkEventTab, *PCDlgAdminTalkEventTab;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
