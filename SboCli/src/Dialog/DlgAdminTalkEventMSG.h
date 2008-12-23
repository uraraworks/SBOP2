/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventMSG.h										 */
/* 内容			:会話イベント設定(メッセージ表示)ダイアログクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventMSG : public CDlgAdminBase
{
public:
			CDlgAdminTalkEventMSG(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminTalkEventMSG();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */


protected:


protected:



public:
	//{{AFX_DATA(CDlgAdminTalkEventMSG)
	enum { IDD = IDD_TALKEVENT_MSG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventMSG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventMSG)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTalkEventMSG, *PCDlgAdminTalkEventMSG;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
