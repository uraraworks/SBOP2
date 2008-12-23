/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventMENU.h									 */
/* 内容			:会話イベント設定(項目選択)ダイアログクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventMENU : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventMENU(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminTalkEventMENU();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */


protected:


protected:



public:
	//{{AFX_DATA(CDlgAdminTalkEventMENU)
	enum { IDD = IDD_TALKEVENT_MENU };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventMENU)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventMENU)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTalkEventMENU, *PCDlgAdminTalkEventMENU;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
