/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharList.h											 */
/* 内容			:キャラ一覧ダイアログクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/01													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoCharCli;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminCharList : public CDlgAdminBase
{
public:
			CDlgAdminCharList(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgAdminCharList();								/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	Renew		(void);											/* 一覧を更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */


protected:


protected:
	CWnd				*m_pWndNotify;				/* 通知先ウィンドウ */
	CLibInfoCharCli		*m_pLibInfoChar;			/* 編集中のキャラ情報 */



public:
	//{{AFX_DATA(CDlgAdminCharList)
	enum { IDD = IDD_CHAR_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminCharList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminCharList)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRenew();
} CDlgAdminCharList, *PCDlgAdminCharList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
