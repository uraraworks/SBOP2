/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:StateProcBase.cpp											 */
/* 内容			:状態処理基底クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "MgrWindow.h"
#include "MgrKeyInput.h"
#include "StateProcBase.h"


/* ========================================================================= */
/* 関数名	:CStateProcBase::CStateProcBase									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CStateProcBase::CStateProcBase()
{
	m_hWndMain		= NULL;
	m_hWndAdmin		= NULL;

	m_pSock			= NULL;
	m_pMgrData		= NULL;
	m_pMgrSound		= NULL;
	m_pMgrDraw		= NULL;
	m_pMgrLayer		= NULL;
	m_pMgrWindow	= NULL;
	m_pMgrKeyInput	= NULL;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::~CStateProcBase								 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CStateProcBase::~CStateProcBase()
{
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::Create											 */
/* 内容		:作成															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::Create(CMgrData *pMgrData, CUraraSockTCPSBO *pSock)
{
	m_pSock			= pSock;
	m_pMgrData		= pMgrData;
	m_pMgrSound		= m_pMgrData->GetMgrSound ();
	m_hWndMain		= m_pMgrData->GetMainWindow ();
	m_hWndAdmin		= m_pMgrData->GetAdminWindow ();
	m_pMgrDraw		= m_pMgrData->GetMgrDraw ();
	m_pMgrLayer		= m_pMgrData->GetMgrLayer ();
	m_pMgrWindow	= m_pMgrData->GetMgrWindow ();
	m_pMgrKeyInput	= m_pMgrData->GetMgrKeyInput ();
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::Init											 */
/* 内容		:初期化															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::Init(void)
{
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::TimerProc(void)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::KeyProc										 */
/* 内容		:キー処理														 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::KeyProc(
	BYTE byCode,		/* [in] イベント */
	BOOL bDown)			/* [in] 押下状態 */
{
	BOOL bResult;

	switch (byCode) {
	case 'X':			bResult = OnX		(bDown);	break;
	case 'Z':			bResult = OnZ		(bDown);	break;
	case 'S':			bResult = OnS		(bDown);	break;
	case 'A':			bResult = OnA		(bDown);	break;
	case 'R':			bResult = OnR		(bDown);	break;
	case 'B':			bResult = OnB		(bDown);	break;
	case 'N':			bResult = OnN		(bDown);	break;
	case 'V':			bResult = OnV		(bDown);	break;
	case VK_OEM_3:		bResult = OnAt		(bDown);	break;
	case VK_UP:			bResult = OnUp		(bDown);	break;
	case VK_DOWN:		bResult = OnDown	(bDown);	break;
	case VK_RIGHT:		bResult = OnRight	(bDown);	break;
	case VK_LEFT:		bResult = OnLeft	(bDown);	break;
	case VK_RETURN:		bResult = OnEnter	(bDown);	break;
	case VK_ESCAPE:		bResult = OnEscape	(bDown);	break;
	case VK_TAB:		bResult = OnTab		(bDown);	break;
	case VK_SHIFT:		bResult = OnShift	(bDown);	break;
	case VK_CONTROL:	bResult = OnCtrl	(bDown);	break;
	case VK_SPACE:		bResult = OnSpace	(bDown);	break;
	}
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnLButtonDown									 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::OnLButtonDown(int x, int y)
{
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnRButtonDown									 */
/* 内容		:メッセージハンドラ(WM_RBUTTONDOWN)								 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CStateProcBase::OnRButtonDown(int x, int y)
{
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnRButtonDblClk								 */
/* 内容		:メッセージハンドラ(WM_RBUTTONDBLCLK)							 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

void CStateProcBase::OnRButtonDblClk(int x, int y)
{
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnMouseMove									 */
/* 内容		:メッセージハンドラ(WM_MOUSEMOVE)								 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CStateProcBase::OnMouseMove(int x, int y)
{
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnKeyDown										 */
/* 内容		:メッセージハンドラ(WM_KEYDOWN)									 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::OnKeyDown(UINT vk)
{
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnWindowMsg									 */
/* 内容		:メッセージハンドラ(WM_WINDOWMSG)								 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CStateProcBase::OnWindowMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnMainFrame									 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2006/11/03														 */
/* ========================================================================= */

void CStateProcBase::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnAdminMsg										 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CStateProcBase::OnAdminMsg(int nCode, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnMgrDraw										 */
/* 内容		:メッセージハンドラ(WM_MGRDRAW)									 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::OnMgrDraw(int nCode, DWORD dwPara)
{
	switch (nCode) {
	case MGRDRAWMSG_START_FADEIN:		OnMgrDrawSTART_FADEIN	(dwPara);	break;		/* フェードイン開始 */
	case MGRDRAWMSG_START_FADEOUT:		OnMgrDrawSTART_FADEOUT	(dwPara);	break;		/* フェードアウト開始 */
	case MGRDRAWMSG_END_FADEIN:			OnMgrDrawEND_FADEIN		(dwPara);	break;		/* フェードイン完了 */
	case MGRDRAWMSG_END_FADEOUT:		OnMgrDrawEND_FADEOUT	(dwPara);	break;		/* フェードアウト完了 */
	}
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnUp											 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnUp(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnDown											 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnDown(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnLeft											 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnLeft(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnRight										 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnRight(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnX(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnZ											 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnZ(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnS											 */
/* 内容		:キーハンドラ(S)												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnS(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnA											 */
/* 内容		:キーハンドラ(A)												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnA(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnB											 */
/* 内容		:キーハンドラ(B)												 */
/* 日付		:2008/08/02														 */
/* ========================================================================= */

BOOL CStateProcBase::OnB(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnR											 */
/* 内容		:キーハンドラ(R)												 */
/* 日付		:2007/04/20														 */
/* ========================================================================= */

BOOL CStateProcBase::OnR(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnI											 */
/* 内容		:キーハンドラ(I)												 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

BOOL CStateProcBase::OnI(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnN											 */
/* 内容		:キーハンドラ(N)												 */
/* 日付		:2008/07/14														 */
/* ========================================================================= */

BOOL CStateProcBase::OnN(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnV											 */
/* 内容		:キーハンドラ(V)												 */
/* 日付		:2008/11/18														 */
/* ========================================================================= */

BOOL CStateProcBase::OnV(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnAt											 */
/* 内容		:キーハンドラ(@)												 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

BOOL CStateProcBase::OnAt(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnEnter										 */
/* 内容		:キーハンドラ(Enter)											 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnEnter(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnEscape										 */
/* 内容		:キーハンドラ(Escape)											 */
/* 日付		:2007/06/19														 */
/* ========================================================================= */

BOOL CStateProcBase::OnEscape(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnTab											 */
/* 内容		:キーハンドラ(Tab)												 */
/* 日付		:2007/07/08														 */
/* ========================================================================= */

BOOL CStateProcBase::OnTab(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnShift										 */
/* 内容		:キーハンドラ(Shift)											 */
/* 日付		:2007/07/28														 */
/* ========================================================================= */

BOOL CStateProcBase::OnShift(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnCtrl											 */
/* 内容		:キーハンドラ(Ctrl)												 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

BOOL CStateProcBase::OnCtrl(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcBase::OnSpace										 */
/* 内容		:キーハンドラ(Space)											 */
/* 日付		:2008/08/02														 */
/* ========================================================================= */

BOOL CStateProcBase::OnSpace(BOOL bDown)
{
	return FALSE;
}

/* Copyright(C)URARA-works 2006 */
