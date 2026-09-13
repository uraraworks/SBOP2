/// @file WindowOPTION_INPUTSET.cpp
/// @brief オプション-入力設定ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/12
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowOPTION_INPUTSET.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
#include <emscripten/emscripten.h>

// バーチャルパッドの表示モード（0=自動 1=表示 2=非表示）。localStorage 'sbop2.virtualPad' に
// 保存される（ブラウザ版の ini はリロードで消えるため localStorage 側を正とする）。
// JS 側 (sbocli-title.shell.html) の window.sbop2ApplyPadMode() が実際の表示切替を行う。
EM_JS(int, SBOP2_GetVirtualPadMode, (), {
	try {
		var m = localStorage.getItem('sbop2.virtualPad');
		if (m === 'on') { return 1; }
		if (m === 'off') { return 2; }
	} catch (e) {}
	return 0;
});

EM_JS(void, SBOP2_SetVirtualPadMode, (int mode), {
	try {
		var name = (mode === 1) ? 'on' : (mode === 2) ? 'off' : 'auto';
		localStorage.setItem('sbop2.virtualPad', name);
	} catch (e) {}
	if (typeof window.sbop2ApplyPadMode === 'function') {
		window.sbop2ApplyPadMode();
	}
});
#endif


CWindowOPTION_INPUTSET::CWindowOPTION_INPUTSET()
{
	m_nPosMax	= 1;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_OPTION_INPUTSET;
	m_ptViewPos.x	= 8 * 13;
	m_ptViewPos.y	= 16 * 10;
	m_sizeWindow.cx	= 16 * 2 + 16 * 16 + 8;
	m_sizeWindow.cy	= 16 * 2 + 16 * (m_nPosMax + 1);
}


CWindowOPTION_INPUTSET::~CWindowOPTION_INPUTSET()
{
}


void CWindowOPTION_INPUTSET::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
	m_nPos = 0;
}


void CWindowOPTION_INPUTSET::Draw(PCImg32 pDst)
{
	int nLevel;
	HDC hDC;
	COLORREF clText;
	CmyString strPadLine;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	clText	= RGB(1, 1, 1);
	hDC	= m_pDib->Lock();

	TextOut2(hDC, m_hFont, 32, 16 + 16 * 0, _T("使用するジョイパッドの設定"), clText);

	// ブラウザ版(wchar_t 版 vswprintf)では Format("...%s...") の %s が空になる方言差があるため、
	// %s に依存せず文字列連結で組み立てる。
	strPadLine = _T("バーチャルパッド: ");
	strPadLine += (LPCTSTR)GetVirtualPadModeLabel();
	TextOut2(hDC, m_hFont, 32, 16 + 16 * 1, (LPCTSTR)strPadLine, clText);

	m_pDib->Unlock();

	DrawCursor(8, 16 + 16 * m_nPos);
	m_dwTimeDrawStart = timeGetTime();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


BOOL CWindowOPTION_INPUTSET::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowOPTION_INPUTSET::OnDown(void)
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

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowOPTION_INPUTSET::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowOPTION_INPUTSET::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowOPTION_INPUTSET::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	PostWindowMessage(m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


// バーチャルパッド表示モードの現在値を表示用ラベルにして返す（ブラウザ版以外は常に「自動」）
CmyString CWindowOPTION_INPUTSET::GetVirtualPadModeLabel(void)
{
	CmyString strRet = _T("自動");
#if defined(__EMSCRIPTEN__)
	switch (SBOP2_GetVirtualPadMode()) {
	case 1:
		strRet = _T("表示");
		break;
	case 2:
		strRet = _T("非表示");
		break;
	default:
		strRet = _T("自動");
		break;
	}
#endif
	return strRet;
}


// バーチャルパッド表示モードを 自動→表示→非表示→自動 と切り替える（ブラウザ版以外は何もしない）
void CWindowOPTION_INPUTSET::CycleVirtualPadMode(void)
{
#if defined(__EMSCRIPTEN__)
	int nMode;

	nMode = SBOP2_GetVirtualPadMode();
	nMode = (nMode + 1) % 3;	// 0=自動 1=表示 2=非表示
	SBOP2_SetVirtualPadMode(nMode);
#endif
}


BOOL CWindowOPTION_INPUTSET::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound(SOUNDID_CANCEL);

	bRet = TRUE;
Exit:
	return bRet;
}
