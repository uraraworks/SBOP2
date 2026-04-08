/// @file WindowMSG.cpp
/// @brief メッセージ表示ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/03
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "Img32.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WindowMSG.h"

CWindowMSG::CWindowMSG()
{
	m_nID	= WINDOWTYPE_MSG;
	m_ptViewPos.x	= 112;
	m_ptViewPos.y	= SCRSIZEY / 2 - 24;
	m_sizeWindow.cx	= 16 * 16;
	m_sizeWindow.cy	= 16 * 3;
	m_dwDeleteTime	= 0;
	m_nType	= 0;
}


CWindowMSG::~CWindowMSG()
{
}


void CWindowMSG::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
}


void CWindowMSG::Draw(PCImg32 pDst)
{
	int nTmp;
	HDC hDC;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame(m_nType);

	nTmp = 0;
	if (m_nType == 4) {
		nTmp = 5;
	}
	hDC	= m_pDib->Lock();

	if (m_strMsg.IsEmpty() == FALSE) {
		TextOut2(hDC, m_hFont, 16, 16 + nTmp, m_strMsg, RGB(1, 1, 1));
	}

	m_pDib->Unlock();

	m_dwTimeDrawStart = timeGetTime();

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


BOOL CWindowMSG::TimerProc(void)
{
	BOOL bRet;
	DWORD dwTime;

	bRet = FALSE;
	if (m_dwDeleteTime == 0) {
		goto Exit;
	}

	dwTime = timeGetTime();
	if (dwTime - m_dwLastTimerProc > m_dwDeleteTime) {
		// 指定時間表示したので削除する
		m_bDelete = TRUE;
	}

Exit:
	return bRet;
}


void CWindowMSG::SetMsg(LPCSTR pszMsg, DWORD dwTime, int nType)
{
	m_strMsg	= pszMsg;
	m_nType	= nType;

	// ウィンドウを更新
	m_dwTimeDrawStart	= 0;
	m_dwDeleteTime	= dwTime;
	m_dwLastTimerProc	= timeGetTime();
	m_sizeWindow.cx	= 16 * 3 + (m_strMsg.GetLength() * 8);
	m_ptViewPos.x	= SCRSIZEX / 2 - m_sizeWindow.cx / 2;

	if (nType == 4) {
		m_ptViewPos.y = SCRSIZEY - 72;
	}

	m_pDib->Destroy();
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
}
