/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventLIGHT.h									 */
/* 内容			:マップイベント(灯り)ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/07													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminMapEventNONE.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventLIGHT : public CDlgAdminMapEventNONE
{
public:
			CDlgAdminMapEventLIGHT(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminMapEventLIGHT();							/* デストラクタ */

	void	Set		(CInfoMapEventBase *pSrc);							/* 設定 */
	void	Get		(CInfoMapEventBase *pDst);							/* 取得 */



public:
	//{{AFX_DATA(CDlgAdminMapEventLIGHT)
	enum { IDD = IDD_MAP_EVENT_LIGHT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventLIGHT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventLIGHT)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nSelect;
	DWORD m_dwTime;
} CDlgAdminMapEventLIGHT, *PCDlgAdminMapEventLIGHT;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
