/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEvent.h											 */
/* 内容			:マップイベント一覧ダイアログクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/24													 */
/* ========================================================================= */

#pragma once

#include "DlgAdminBase.h"

class CInfoMapBase;
class CLibInfoMapEvent;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CDlgAdminMapEvent : public CDlgAdminBase
{
public:
			CDlgAdminMapEvent(CWnd* pParent = NULL);			/* コンストラクタ */
	virtual ~CDlgAdminMapEvent();								/* デストラクタ */

	void	Init		(CMgrData *pMgrData);							/* 初期化 */
	void	Renew		(void);											/* 一覧を更新 */
	void	OnAdminMsg	(int nType, DWORD dwPara);						/* メッセージハンドラ(WM_ADMINMSG) */


protected:


protected:
	CWnd				*m_pWndNotify;				/* 通知先ウィンドウ */
	CInfoMapBase		*m_pInfoMap;				/* 編集中のマップ情報 */
	CLibInfoMapEvent	*m_pLibInfoMapEvent;		/* 編集中のマップイベント情報 */



public:
	//{{AFX_DATA(CDlgAdminMapEvent)
	enum { IDD = IDD_MAP_EVENT };
	CListCtrl	m_List;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgAdminMapEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgAdminMapEvent)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CDlgAdminMapEvent, *PCDlgAdminMapEvent;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
