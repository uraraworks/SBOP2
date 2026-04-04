/// @file StateProcBase.cpp
/// @brief 状態処理基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/10/01
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "MgrWindow.h"
#include "MgrKeyInput.h"
#include "StateProcBase.h"

CStateProcBase::CStateProcBase()
{
	m_pSock = NULL;
	m_pMgrData = NULL;
	m_pMgrSound = NULL;
	m_pMgrDraw = NULL;
	m_pMgrLayer = NULL;
	m_pMgrWindow = NULL;
	m_pMgrKeyInput = NULL;
}

CStateProcBase::~CStateProcBase()
{
}

void CStateProcBase::Create(CMgrData *pMgrData, CUraraSockTCPSBO *pSock)
{
	m_pSock = pSock;
	m_pMgrData = pMgrData;
	m_pMgrSound = m_pMgrData->GetMgrSound();
	m_pMgrDraw = m_pMgrData->GetMgrDraw();
	m_pMgrLayer = m_pMgrData->GetMgrLayer();
	m_pMgrWindow = m_pMgrData->GetMgrWindow();
	m_pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
}

void CStateProcBase::Init(void)
{
}

BOOL CStateProcBase::TimerProc(void)
{
	return FALSE;
}

void CStateProcBase::KeyProc(
	BYTE byCode,  // [in] イベント
	BOOL bDown)   // [in] 押下状態
{
	BOOL bResult;

	switch (byCode) {
	case 'X':       bResult = OnX(bDown);      break;
	case 'Z':       bResult = OnZ(bDown);      break;
	case 'S':       bResult = OnS(bDown);      break;
	case 'A':       bResult = OnA(bDown);      break;
	case 'R':       bResult = OnR(bDown);      break;
	case 'B':       bResult = OnB(bDown);      break;
	case 'N':       bResult = OnN(bDown);      break;
	case 'V':       bResult = OnV(bDown);      break;
	case 'F':       bResult = OnF(bDown);      break;
	case 'J':       bResult = OnJ(bDown);      break;
	case 'L':       bResult = OnL(bDown);      break;
	case VK_OEM_3:  bResult = OnAt(bDown);     break;
	case VK_UP:     bResult = OnUp(bDown);     break;
	case VK_DOWN:   bResult = OnDown(bDown);   break;
	case VK_RIGHT:  bResult = OnRight(bDown);  break;
	case VK_LEFT:   bResult = OnLeft(bDown);   break;
	case VK_RETURN: bResult = OnEnter(bDown);  break;
	case VK_ESCAPE: bResult = OnEscape(bDown); break;
	case VK_TAB:    bResult = OnTab(bDown);    break;
	case VK_SHIFT:  bResult = OnShift(bDown);  break;
	case VK_CONTROL: bResult = OnCtrl(bDown);  break;
	case VK_SPACE:  bResult = OnSpace(bDown);  break;
	}
}

void CStateProcBase::OnLButtonDown(int x, int y)
{
}

void CStateProcBase::OnRButtonDown(int x, int y)
{
}

void CStateProcBase::OnRButtonDblClk(int x, int y)
{
}

void CStateProcBase::OnMouseMove(int x, int y)
{
}

void CStateProcBase::OnKeyDown(UINT vk)
{
}

void CStateProcBase::OnWindowMsg(int nType, DWORD dwPara)
{
}

void CStateProcBase::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
}

void CStateProcBase::OnAdminMsg(int nCode, DWORD dwPara)
{
}

void CStateProcBase::OnMgrDraw(int nCode, DWORD dwPara)
{
	switch (nCode) {
	case MGRDRAWMSG_START_FADEIN:  OnMgrDrawSTART_FADEIN(dwPara);  break; // フェードイン開始
	case MGRDRAWMSG_START_FADEOUT: OnMgrDrawSTART_FADEOUT(dwPara); break; // フェードアウト開始
	case MGRDRAWMSG_END_FADEIN:    OnMgrDrawEND_FADEIN(dwPara);    break; // フェードイン完了
	case MGRDRAWMSG_END_FADEOUT:   OnMgrDrawEND_FADEOUT(dwPara);   break; // フェードアウト完了
	}
}

BOOL CStateProcBase::OnUp(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnDown(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnLeft(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnRight(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnX(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnZ(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnS(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnA(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnB(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnR(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnI(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnN(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnV(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnF(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnJ(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnL(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnAt(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnEnter(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnEscape(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnTab(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnShift(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnCtrl(BOOL bDown)
{
	return FALSE;
}

BOOL CStateProcBase::OnSpace(BOOL bDown)
{
	return FALSE;
}
