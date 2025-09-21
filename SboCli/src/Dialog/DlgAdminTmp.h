/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTmp.h												 */
/* 内容			:ダイアログクラス 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTmp : public CDlgAdminBase
{
public:
			CDlgAdminTmp(CWnd* pParent = NULL);					/* コンストラクタ */
	virtual ~CDlgAdminTmp();									/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */


protected:


protected:



public:
	//{{AFX_DATA(CDlgAdminTmp)
	enum { IDD = IDD_ };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTmp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTmp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminTmp, *PCDlgAdminTmp;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
