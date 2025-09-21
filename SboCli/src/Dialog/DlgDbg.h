/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:DlgDbg.h													 */
/* 内容			:デバッグ情報ダイアログクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/05													 */
/* ========================================================================= */

#pragma once

#include "resource.h"
#include "LayoutHelper.h"

class CMgrData;
class CUraraSockTCPSBO;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgDbg : public CDialog, CLayoutHelper
{
public:
			CDlgDbg(CWnd* pParent = NULL);				/* コンストラクタ */
	virtual ~CDlgDbg();									/* デストラクタ */

	BOOL Create	(HWND hWndParent, CMgrData *pMgrData);			/* 作成 */
	void Renew	(void);											/* 更新 */


protected:


protected:
	CMgrData			*m_pMgrData;			/* データ管理 */
	CUraraSockTCPSBO	*m_pSock;				/* 通信マネージャ */


public:
	void OnOK () {}
	void OnCancel () {}
	//{{AFX_DATA(CDlgDbg)
	enum { IDD = IDD_DBGDLG };
	CString	m_strOnline;
	CString	m_strCharCount;
	CString	m_strPing;
	CString	m_strPos;
	CString	m_strThrowghput;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgDbg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgDbg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
} CDlgDbg, *PCDlgDbg;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2009 */
