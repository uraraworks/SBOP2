/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventPAGE.h									 */
/* 内容			:会話イベント設定(ページ切り替え)ダイアログクラス 定義ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminTalkEventNONE.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminTalkEventPAGE : public CDlgAdminTalkEventNONE
{
public:
			CDlgAdminTalkEventPAGE(CWnd* pParent = NULL);		/* コンストラクタ */
	virtual ~CDlgAdminTalkEventPAGE();							/* デストラクタ */

	void	Set	(CInfoTalkEventBase *pSrc);								/* 設定から画面に反映 */
	void	Get	(CInfoTalkEventBase *pDst);								/* 画面から設定に反映 */


protected:


protected:



public:
	//{{AFX_DATA(CDlgAdminTalkEventPAGE)
	enum { IDD = IDD_TALKEVENT_PAGE };
	CComboBox m_Combo;
	CComboBox m_cmbJump;
	CComboBox m_cmbItem;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminTalkEventPAGE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminTalkEventPAGE)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo();
} CDlgAdminTalkEventPAGE, *PCDlgAdminTalkEventPAGE;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
