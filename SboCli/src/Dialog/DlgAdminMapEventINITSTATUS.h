/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventINITSTATUS.h								 */
/* 内容			:マップイベント(ステータス初期化)ダイアログクラス 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/05													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminMapEventNONE.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminMapEventINITSTATUS : public CDlgAdminMapEventNONE
{
public:
			CDlgAdminMapEventINITSTATUS(CWnd* pParent = NULL);	/* コンストラクタ */
	virtual ~CDlgAdminMapEventINITSTATUS();						/* デストラクタ */

	void	Set		(CInfoMapEventBase *pSrc);							/* 設定 */
	void	Get		(CInfoMapEventBase *pDst);							/* 取得 */


protected:
	DWORD	m_dwEffectID;			/* エフェクトID */



public:
	//{{AFX_DATA(CDlgAdminMapEventINITSTATUS)
	enum { IDD = IDD_MAP_EVENT_INITSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEventINITSTATUS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEventINITSTATUS)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_ctlEffectID;
} CDlgAdminMapEventINITSTATUS, *PCDlgAdminMapEventINITSTATUS;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
