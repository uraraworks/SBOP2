/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventSet.h										 */
/* 内容			:会話イベント設定ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"
#include "afxcmn.h"

class CInfoTalkEvent;
class CDlgAdminTalkEventTab;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventSet : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventSet(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminTalkEventSet();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */


protected:
	void	AddPage(void);			/* ページの追加 */


protected:
	CInfoTalkEvent			*m_pInfo;		/* 編集中の会話イベント */
	CDlgAdminTalkEventTab	*m_pDlgTab;		/* タブダイアログ */


public:
	//{{AFX_DATA(CDlgAdminTalkEventSet)
	enum { IDD = IDD_TALKEVENT_SET };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventSet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
} CDlgAdminTalkEventSet, *PCDlgAdminTalkEventSet;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
