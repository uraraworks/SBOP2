/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:DlgAdminAccountAdd.h										 */
/* 内容			:アカウント追加ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/17													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminAccountAdd : public CDlgAdminBase
{
public:
			CDlgAdminAccountAdd(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgAdminAccountAdd();								/* デストラクタ */

	void	Init	(CMgrData *pMgrData);								/* 初期化 */



public:
	//{{AFX_DATA(CDlgAdminAccountAdd)
	enum { IDD = IDD_ACCOUNT_ADD };
	CString	m_strAccount;
	CString	m_strPassword;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminAccountAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminAccountAdd)
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminAccountAdd, *PCDlgAdminAccountAdd;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
