/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharAdmin.h										 */
/* 内容			:管理者権限の設定ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharAdmin : public CDlgAdminBase
{
public:
			CDlgAdminCharAdmin(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminCharAdmin();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	Renew		(void);										/* 更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */


protected:
	int				m_nAdminLevel;					/* 管理者レベル */
	DWORD			m_dwAccountID;					/* 編集中のアカウントID */



public:
	//{{AFX_DATA(CDlgAdminCharAdmin)
	enum { IDD = IDD_CHAR_ADMIN };
	CComboBox	m_ctAdminLevel;
	CString	m_strCharName;
	CString	m_strAccountID;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharAdmin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharAdmin)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharAdmin, *PCDlgAdminCharAdmin;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
