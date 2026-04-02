/// @file WindowCOMMANDMENU.cpp
/// @brief コマンドメニューウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/02
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowCOMMANDMENU.h"


CWindowCOMMANDMENU::CWindowCOMMANDMENU()
{
	m_nPos	= 2;
	m_nPosMax	= 4;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_COMMANDMENU;
	m_sizeWindow.cx	= 186 + 24;
	m_sizeWindow.cy	= 125;
	m_ptViewPos.x	= SCRSIZEX - m_sizeWindow.cx;
	m_ptViewPos.y	= SCRSIZEY - m_sizeWindow.cy;

	m_nPosSub = 0;
}


CWindowCOMMANDMENU::~CWindowCOMMANDMENU()
{
}


void CWindowCOMMANDMENU::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);

	RenewCommand();
}


void CWindowCOMMANDMENU::Draw(PCImg32 pDst)
{
	int nLevel, x, y;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_pDib->FillRect(0, 0, m_sizeWindow.cx, m_sizeWindow.cy, 0);
	m_pDib->BltFrom256(m_sizeWindow.cx - 186, m_sizeWindow.cy - 43, 186, 43, m_pDibSystem, 272, 786, TRUE);

	x = 273 + m_nPos * 37;
	y = 704;
	m_pDib->BltFrom256(m_nPos * 37 + 25, 0, 36, 82, m_pDibSystem, x, y, TRUE);

	x = 24 + 3 + (m_nPos * 38);
	y = m_sizeWindow.cy - 24;
	y -= ((m_nPosSub * 41) + 3);
	m_pMgrDraw->DrawCursor(m_pDib, x - 16, y - 8, 1);
	m_dwTimeDrawStart = timeGetTime();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


BOOL CWindowCOMMANDMENU::OnUp(void)
{
	int anSubMax[] = {1, 2, 1, 2, 2};
	BOOL bRet;

	bRet = FALSE;

	if (m_nPosSub >= anSubMax[m_nPos]) {
		goto Exit;
	}
	m_nPosSub ++;

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	RenewCommand();

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowCOMMANDMENU::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPosSub == 0) {
		goto Exit;
	}
	m_nPosSub --;

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	RenewCommand();

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowCOMMANDMENU::OnLeft(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos == 0) {
		goto Exit;
	}

	m_nPos --;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	RenewCommand();

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowCOMMANDMENU::OnRight(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax) {
		goto Exit;
	}

	m_nPos ++;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	RenewCommand();

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowCOMMANDMENU::OnB(BOOL bDown)
{
	return OnZ(bDown);
}


BOOL CWindowCOMMANDMENU::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	PostWindowMessage(m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowCOMMANDMENU::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound(SOUNDID_CANCEL);
	PostWindowMessage(-1);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowCOMMANDMENU::OnSpace(BOOL bDown)
{
	return OnZ(bDown);
}


void CWindowCOMMANDMENU::RenewCommand(void)
{
	int anPosSubMax[] = {1, 2, 1, 2, 2};

	m_nPosSub = min(m_nPosSub, anPosSubMax[m_nPos]);
	m_strCommand.Empty();

	switch (m_nPos) {
	case 0:	// キャラクター(C)
		switch (m_nPosSub) {
		case 1:
			m_strCommand = "進行中のクエスト";
			break;
		default:
			m_strCommand = "キャラクター(C)";
			break;
		}
		break;
	case 1:	// スキル(S)
		switch (m_nPosSub) {
		case 1:
			m_strCommand = "戦闘スキル(F)";
			break;
		case 2:
			m_strCommand = "生活スキル(L)";
			break;
		default:
			m_strCommand = "スキル(S)";
			break;
		}
		break;
	case 2:	// バッグ(B)
		switch (m_nPosSub) {
		case 1:
			m_strCommand = "マップ(M)";
			break;
		default:
			m_strCommand = "バッグ(B)";
			break;
		}
		break;
	case 3:	// 招待(I)
		switch (m_nPosSub) {
		case 1:
			m_strCommand = "パーティー(P)";
			break;
		case 2:
			m_strCommand = "ギルド(G)";
			break;
		default:
			m_strCommand = "招待(I)";
			break;
		}
		break;
	case 4:	// システム(ESC)
		switch (m_nPosSub) {
		case 1:
			m_strCommand = "ヘルプ(H)";
			break;
		case 2:
			m_strCommand = "ＧＭコール";
			break;
		default:
			m_strCommand = "システム(ESC)";
			break;
		}
		break;
	}
}
