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

class CInfoTalkEvent;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventTab : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventTab(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminTalkEventTab();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData, CInfoTalkEvent *pInfo);	/* 初期化 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */
	void	SetPage		(int nPage);									/* 表示するページを設定 */


protected:
	void	Renew(void);					/* 更新 */


protected:
	int				m_nPage;		/* 表示中のページ番号 */
	CInfoTalkEvent	*m_pInfo;		/* 編集中の会話イベント */



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
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDel();
} CDlgAdminTalkEventTab, *PCDlgAdminTalkEventTab;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
