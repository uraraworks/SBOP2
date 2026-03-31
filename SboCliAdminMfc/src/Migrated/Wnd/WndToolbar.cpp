/// @file WndToolbar.cpp
/// @brief ツールバーコントロールクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/09/13
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "afxpriv.h"
#include "WndToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラス設定

BEGIN_MESSAGE_MAP(CWndToolbar, CToolBar)
	//{{AFX_MSG_MAP(CWndToolbar)
	ON_MESSAGE(WM_IDLEUPDATECMDUI,OnIdleUpdateCmdUI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CWndToolbar::CWndToolbar()
{
}

CWndToolbar::~CWndToolbar()
{
}

LRESULT CWndToolbar::OnIdleUpdateCmdUI(WPARAM wParam,LPARAM lParam)
{
	if (IsWindowVisible()){
		CFrameWnd *pParent = (CFrameWnd *)GetParent();
		if (pParent) {
			// ON_UPDATE_COMMAND_UIが使えるように関数呼び出し
			OnUpdateCmdUI(pParent,(BOOL)wParam);
		}
	}
	return 0;
}

