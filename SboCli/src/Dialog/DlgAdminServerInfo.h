/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminServerInfo.h										 */
/* 内容			:サーバー情報ダイアログクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/08													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminServerInfo : public CDlgAdminBase
{
public:
			CDlgAdminServerInfo(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminServerInfo();							/* デストラクタ */

	void	Init		(CMgrData *pMgrData);						/* 初期化 */



public:
	//{{AFX_DATA(CDlgAdminServerInfo)
	enum { IDD = IDD_SERVER_INFO };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminServerInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminServerInfo)
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPlaysound();
	afx_msg void OnBnClickedRenewClientversion();
} CDlgAdminServerInfo, *PCDlgAdminServerInfo;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
