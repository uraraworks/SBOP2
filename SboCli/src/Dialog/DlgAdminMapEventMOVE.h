/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventMOVE.h										 */
/* 内容			:マップイベント(マップ内移動)ダイアログクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/24													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminMapEventNONE.h"
#include "afxwin.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventMOVE : public CDlgAdminMapEventNONE
{
public:
			CDlgAdminMapEventMOVE(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminMapEventMOVE();							/* デストラクタ */

	void	Set		(CInfoMapEventBase *pSrc);							/* 設定 */
	void	Get		(CInfoMapEventBase *pDst);							/* 取得 */



public:
	//{{AFX_DATA(CDlgAdminMapEventMOVE)
	enum { IDD = IDD_MAP_EVENT_MOVE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventMOVE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventMOVE)
	virtual BOOL OnInitDialog();
	virtual afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nPosX;
	int m_nPosY;
	CComboBox m_ctlDirection;
	int m_nDirection;
} CDlgAdminMapEventMOVE, *PCDlgAdminMapEventMOVE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
