/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharAccountInfo.h									 */
/* 内容			:アカウント情報ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharAccountInfo : public CDlgAdminBase
{
public:
			CDlgAdminCharAccountInfo(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminCharAccountInfo();						/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	Renew		(void);											/* 更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */


protected:
	DWORD	m_dwAccountID;			/* 編集中のアカウントID */



public:
	//{{AFX_DATA(CDlgAdminCharAccountInfo)
	enum { IDD = IDD_CHAR_ACCOUNTINFO };
	CString	m_strCharName;
	CString	m_strAccountID;
	CString	m_strAccount;
	CString	m_strPassword;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharAccountInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharAccountInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminCharAccountInfo, *PCDlgAdminCharAccountInfo;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
