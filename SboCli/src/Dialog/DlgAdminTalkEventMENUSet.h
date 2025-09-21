/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventMENUSet.h									 */
/* 内容			:会話イベント選択項目の設定ダイアログクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/28													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "afxwin.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventMENUSet : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventMENUSet(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminTalkEventMENUSet();						/* デストラクタ */

	void	Init(CMgrData *pMgrData, int nPage);						/* 初期化 */


protected:


public:
	int		m_nPage,		/* 選択中のページ番号 */
			m_nPageCount;	/* ページ数 */



public:
	void OnOK();
	//{{AFX_DATA(CDlgAdminTalkEventMENUSet)
	enum { IDD = IDD_TALKEVENT_MENU_SET };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventMENUSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventMENUSet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CComboBox m_Combo;
} CDlgAdminTalkEventMENUSet, *PCDlgAdminTalkEventMENUSet;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
