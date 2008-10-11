/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WndToolbar.cpp												 */
/* 内容			:ツールバーコントロールクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/13													 */
/* ========================================================================= */

#include "stdafx.h"
#include "afxpriv.h"
#include "WndToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* クラス設定																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CWndToolbar, CToolBar)
	//{{AFX_MSG_MAP(CWndToolbar)
	ON_MESSAGE(WM_IDLEUPDATECMDUI,OnIdleUpdateCmdUI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CWndToolbar::CWndToolbar										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/09/13														 */
/* ========================================================================= */

CWndToolbar::CWndToolbar()
{
}


/* ========================================================================= */
/* 関数名	:CWndToolbar::~CWndToolbar										 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/09/13														 */
/* ========================================================================= */

CWndToolbar::~CWndToolbar()
{
}


/* ========================================================================= */
/* 関数名	:CWndToolbar::OnIdleUpdateCmdUI									 */
/* 内容		:メッセージハンドラ(WM_IDLEUPDATECMDUI)							 */
/* 日付		:2008/09/13														 */
/* ========================================================================= */

LRESULT CWndToolbar::OnIdleUpdateCmdUI(WPARAM wParam,LPARAM lParam)
{
	if (IsWindowVisible ()){
		CFrameWnd *pParent = (CFrameWnd *)GetParent ();
		if (pParent) {
			/* ON_UPDATE_COMMAND_UIが使えるように関数呼び出し */
			OnUpdateCmdUI (pParent,(BOOL)wParam);
		}
	}
	return 0;
}

/* Copyright(C)URARA-works 2008 */
