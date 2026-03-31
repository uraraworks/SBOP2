/// @file StateProcINFO.cpp
/// @brief 状態処理クラス(お知らせ) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/08/16
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "StateProcINFO.h"

CStateProcINFO::CStateProcINFO()
{
}

CStateProcINFO::~CStateProcINFO()
{
}

void CStateProcINFO::Init(void)
{
	m_pMgrLayer->MakeTITLE();
	m_pMgrLayer->MakeINFO();
}

void CStateProcINFO::OnLButtonDown(int x, int y)
{
	PostMessage(m_hWndMain, WM_MAINFRAME, MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGINMENU);
}

BOOL CStateProcINFO::OnX(BOOL bDown)
{
	if (bDown == TRUE) {
		goto Exit;
	}

	OnLButtonDown(0, 0);

Exit:
	return FALSE;
}
