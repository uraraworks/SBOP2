/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharDisable.h										 */
/* 内容			:拒否一覧ダイアログクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/06													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoDisable;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharDisable : public CDlgAdminBase
{
public:
			CDlgAdminCharDisable(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgAdminCharDisable();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */
	void	Renew		(void);										/* 一覧を更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);					/* メッセージハンドラ(WM_ADMINMSG) */


protected:


protected:
	CWnd				*m_pWndNotify;				/* 通知先ウィンドウ */
	CLibInfoDisable		*m_pLibInfoDisable;			/* 編集中の拒否情報 */



public:
	//{{AFX_DATA(CDlgAdminCharDisable)
	enum { IDD = IDD_CHAR_DISABLE };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharDisable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharDisable)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRenew();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
} CDlgAdminCharDisable, *PCDlgAdminCharDisable;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
