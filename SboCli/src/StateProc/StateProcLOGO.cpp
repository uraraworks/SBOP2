/// @file StateProcLOGO.cpp
/// @brief 状態処理クラス(ロゴ) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/05/29
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "MgrSound.h"
#include "MainFrame.h"
#include "StateProcLOGO.h"

CStateProcLOGO::CStateProcLOGO()
{
	m_dwTimeWait = 0;
}

CStateProcLOGO::~CStateProcLOGO()
{
}

void CStateProcLOGO::Init(void)
{
	m_pMgrLayer->MakeLOGO();
	m_pMgrDraw->SetLevel(1);
	m_pMgrDraw->SetFadeState(FADESTATE_FADEIN);
}

BOOL CStateProcLOGO::TimerProc(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_dwTimeWait == 0) {
		goto Exit;
	}

	if (timeGetTime() - m_dwTimeWait < 3000) {
		goto Exit;
	}

	m_pMgrDraw->SetFadeState(FADESTATE_FADEOUT);
	m_dwTimeWait = 0;

	bRet = TRUE;
Exit:
	return bRet;
}

void CStateProcLOGO::OnLButtonDown(int x, int y)
{
	m_pMgrDraw->SetFadeState(FADESTATE_FADEOUT);
}

BOOL CStateProcLOGO::OnX(BOOL bDown)
{
	OnLButtonDown(0, 0);

	return TRUE;
}

BOOL CStateProcLOGO::OnZ(BOOL bDown)
{
	OnLButtonDown(0, 0);

	return TRUE;
}

void CStateProcLOGO::OnMgrDrawEND_FADEIN(DWORD dwPara)
{
	m_dwTimeWait = timeGetTime();
	m_pMgrSound->PlaySound(SOUNDID_URARA_WORKS);
}

void CStateProcLOGO::OnMgrDrawEND_FADEOUT(DWORD dwPara)
{
	m_pMgrData->PostMainFrameMessage(MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGIN);
}
