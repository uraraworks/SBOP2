/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WndToolbar.h												 */
/* 内容			:ツールバーコントロールクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/13													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWndToolbar : public CToolBar
{
public:
			CWndToolbar();							/* コンストラクタ */
	virtual ~CWndToolbar();							/* デストラクタ */


public:
	//{{AFX_VIRTUAL(CWndToolbar)
	public:
	protected:
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWndToolbar)
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
} CWndToolbar, *PCWndToolbar;

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
