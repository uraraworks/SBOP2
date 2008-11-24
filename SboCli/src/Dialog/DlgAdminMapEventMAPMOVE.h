/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventMAPMOVE.h									 */
/* 内容			:マップイベント(マップ間移動)ダイアログクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/27													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminMapEventNONE.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventMAPMOVE : public CDlgAdminMapEventNONE
{
public:
			CDlgAdminMapEventMAPMOVE(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminMapEventMAPMOVE();						/* デストラクタ */

	void	Set		(CInfoMapEventBase *pSrc);							/* 設定 */
	void	Get		(CInfoMapEventBase *pDst);							/* 取得 */



public:
	//{{AFX_DATA(CDlgAdminMapEventMAPMOVE)
	enum { IDD = IDD_MAP_EVENT_MAPMOVE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventMAPMOVE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventMAPMOVE)
	virtual BOOL OnInitDialog();
	virtual afx_msg LRESULT OnAdminMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwMapID;
	int m_nPosX;
	int m_nPosY;
	int m_nDirection;
	CComboBox m_ctlDirection;
} CDlgAdminMapEventMAPMOVE, *PCDlgAdminMapEventMAPMOVE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
