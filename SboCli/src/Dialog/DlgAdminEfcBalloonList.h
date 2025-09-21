/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminEfcBalloonList.h									 */
/* 内容			:噴出し一覧ダイアログクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CLibInfoEfcBalloon;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminEfcBalloonList : public CDlgAdminBase
{
public:
			CDlgAdminEfcBalloonList(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminEfcBalloonList();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	Renew		(void);											/* 一覧を更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */
	void	OnMainFrame	(DWORD dwCommand, DWORD dwParam);				/* メッセージハンドラ(WM_MAINFRAME) */


protected:


protected:
	CLibInfoEfcBalloon	*m_pLibInfo;		/* 噴出し情報 */



public:
	//{{AFX_DATA(CDlgAdminEfcBalloonList)
	enum { IDD = IDD_EFC_BALLOON_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminEfcBalloonList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminEfcBalloonList)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	afx_msg void OnRenew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminEfcBalloonList, *PCDlgAdminEfcBalloonList;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
