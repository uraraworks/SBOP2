/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminBase.h												 */
/* 内容			:管理者ウィンドウ基底ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/03/18													 */
/* ========================================================================= */

#pragma once

#include "LayoutHelper.h"

class CMgrData;
class CUraraSockTCPSBO;
class CInfoItemTypeBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminBase : public CDialog, public CLayoutHelper
{
public:
			CDlgAdminBase(int nResourceID, CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminBase();										/* デストラクタ */

	virtual void	Init			(CMgrData *pMgrData);					/* 初期化 */
	virtual void	Renew			(void);									/* 更新 */
	virtual void	OnAdminMsg		(int nType, DWORD dwPara);				/* メッセージハンドラ(WM_ADMINMSG) */
	virtual void	OnMainFrame		(DWORD dwCommand, DWORD dwParam);		/* メッセージハンドラ(WM_MAINFRAME) */
	virtual void	Set				(CInfoItemTypeBase *pSrc)  {}			/* 編集内容を設定 */
	virtual void	Get				(CInfoItemTypeBase *&pDst) {}			/* 編集内容を取得 */


protected:
	void	SelectCmb		(CComboBox *pCmb, DWORD dwID);	/* 項目を選択 */


protected:
	CMgrData			*m_pMgrData;			/* データマネージャ */
	CUraraSockTCPSBO	*m_pSock;				/* 通信マネージャ */
	CWnd				*m_pWndParent;			/* 親ウィンドウ */



public:
	//{{AFX_DATA(CDlgAdminBase)
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy ();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminBase)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminBase, *PCDlgAdminBase;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
