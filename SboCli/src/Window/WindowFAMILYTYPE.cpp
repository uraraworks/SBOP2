/// @file WindowFAMILYTYPE.cpp
/// @brief 種族選択ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/07
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowFAMILYTYPE.h"
#include "myString.h"
#include "TextRenderer.h"

CWindowFAMILYTYPE::CWindowFAMILYTYPE()
{
	m_nPos	= 0;
	m_nPosMax	= 4-3;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_FAMILYTYPE;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 80;
	m_sizeWindow.cx	= 320;
	m_sizeWindow.cy	= 320;
}


CWindowFAMILYTYPE::~CWindowFAMILYTYPE()
{
}


void CWindowFAMILYTYPE::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
}


void CWindowFAMILYTYPE::Draw(PCImg32 pDst)
{
	int i, nCount;
	COLORREF clText;
	POINT nCursorPos[] = {
		40, 104
//	40, 152,
//	40, 200,
//	40, 248
	};

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	nCount = 4-3;
	DrawFrame(2);
	for (i = 0; i < nCount; i ++) {
		DrawFrame2(nCursorPos[i].x, nCursorPos[i].y, 96, 8, 2);
	}
	DrawFrame(160, 96, 132, 168, 2);

//	m_pDib->BltFrom256(166, 128, 120, 104, m_pDibSystem, m_nPos * 120, 520, TRUE);

	clText = RGB(124, 123, 232);
	TextOut4(m_pDib, FONTID_PGOTHIC_14_BOLD, 88,	24,	_T("新規キャラクター作成"), clText);
	TextOut4(m_pDib, FONTID_PGOTHIC_14_BOLD, 136,	48,	_T("種族選択"), clText);
	TextOut4(m_pDib, FONTID_PGOTHIC_14_BOLD, 256,	288,	_T("１／３"), clText);

	for (i = 0; i < nCount; i ++) {
		CString strFamily = Utf8ToTString(m_pMgrData->GetFamilyTypeName((WORD)(FAMILYTYPE_HUMAN + i)));
		TextOut2(m_pDib, FONTID_PGOTHIC_14_BOLD, nCursorPos[i].x, nCursorPos[i].y - 4, strFamily, clText);
	}

	DrawCursor(nCursorPos[m_nPos].x - 32, nCursorPos[m_nPos].y - 6);
	m_dwTimeDrawStart = timeGetTime();

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


BOOL CWindowFAMILYTYPE::TimerProc(void)
{
	BOOL bRet;
	int nCursorAnimeBack;

	bRet = FALSE;

	nCursorAnimeBack = m_nCursorAnime;
	CWindowBase::TimerProc();
	if (nCursorAnimeBack == m_nCursorAnime) {
		goto Exit;
	}

	m_dwTimeDrawStart = 0;
	bRet = TRUE;
Exit:
	return bRet;
}


WORD CWindowFAMILYTYPE::GetType(void)
{
	return (FAMILYTYPE_NONE + 1 + m_nPos);
}


BOOL CWindowFAMILYTYPE::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowFAMILYTYPE::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax - 1) {
		goto Exit;
	}
	m_nPos ++;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowFAMILYTYPE::OnLeft(void)
{
	m_nPos = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	return TRUE;
}


BOOL CWindowFAMILYTYPE::OnRight(void)
{
	m_nPos = m_nPosMax - 1;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	return TRUE;
}


BOOL CWindowFAMILYTYPE::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	PostWindowMessage(0);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowFAMILYTYPE::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_CANCEL);
	PostWindowMessage(-1);
	m_bDelete = TRUE;

	bRet = TRUE;
Exit:
	return bRet;
}
