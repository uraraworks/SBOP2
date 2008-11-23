/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkSet.h											 */
/* 内容			:会話データ設定ダイアログクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/22													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTalkSet : public CDlgAdminBase
{
public:
			CDlgAdminTalkSet(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminTalkSet();						/* デストラクタ */


public:
	void OnOK();
	//{{AFX_DATA(CDlgAdminTalkSet)
	enum { IDD = IDD_TALK_SET };
	CString	m_strTalk;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkSet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTalkSet, *PCDlgAdminTalkSet;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
