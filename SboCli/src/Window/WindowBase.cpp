/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:WindowBase.cpp												 */
/* 内容			:ウィンドウ基底クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WindowBase.h"

/* ========================================================================= */
/* 関数名	:CWindowBase::CWindowBase										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

CWindowBase::CWindowBase()
{
	m_nID				= WINDOWTYPE_NONE;
	m_nPos				= 0;
	m_nPosMax			= 0;
	m_nCursorAnime		= 0;
	m_nCountKeyRepeate	= 0;
	m_dwTimeDrawStart	= 0;
	m_dwLastTimeKey		= 0;
	m_pParent			= NULL;
	m_pDib				= new CImg32;
	m_pMgrData			= NULL;
	m_pMgrDraw			= NULL;
	m_pMgrSound			= NULL;
	m_pMgrGrpData		= NULL;
	m_pDibSystem		= NULL;
	m_hWndMain			= NULL;
	m_dwLastTimerProc	= 0;
	m_dwLastTimeCursor	= 0;
	m_bShow				= TRUE;
	m_bDelete			= FALSE;
	m_bInput			= FALSE;
	m_bActive			= FALSE;

	m_ptViewPos.x	= m_ptViewPos.y		= 0;
	m_sizeWindow.cx	= m_sizeWindow.cy	= 0;

	m_hFont = CreateFont (16, 0, 0, 0, FW_ULTRABOLD,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "ＭＳ Ｐゴシック");
	m_hFont12 = CreateFont (12, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "ＭＳ Ｐゴシック");
	m_hFont12Bold = CreateFont (12, 0, 0, 0, FW_ULTRABOLD,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "ＭＳ Ｐゴシック");
	m_hFont14 = CreateFont (14, 0, 0, 0, FW_ULTRABOLD,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "ＭＳ Ｐゴシック");
	m_hFont16 = CreateFont (16, 0, 0, 0, FW_ULTRABOLD,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "ＭＳ ゴシック");
	m_hFont16Normal = CreateFont (16, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "ＭＳ ゴシック");

	m_pDib->SetColorKey (RGB (255, 0, 255));
}


/* ========================================================================= */
/* 関数名	:CWindowBase::~CWindowBase										 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

CWindowBase::~CWindowBase()
{
	int i, nCount;
	PCWindowBase pChild;

	nCount = m_apChild.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pChild = m_apChild[i];
		pChild->m_bDelete = TRUE;
	}
	if (m_pParent) {
		m_pParent->DeleteChild (this);
	}

	if (m_hFont) {
		DeleteObject (m_hFont);
		m_hFont = NULL;
	}
	if (m_hFont12) {
		DeleteObject (m_hFont12);
		m_hFont12 = NULL;
	}
	if (m_hFont12Bold) {
		DeleteObject (m_hFont12Bold);
		m_hFont12Bold = NULL;
	}
	if (m_hFont14) {
		DeleteObject (m_hFont14);
		m_hFont14 = NULL;
	}
	if (m_hFont16) {
		DeleteObject (m_hFont16);
		m_hFont16 = NULL;
	}
	if (m_hFont16Normal) {
		DeleteObject (m_hFont16Normal);
		m_hFont16Normal = NULL;
	}
	SAFE_DELETE (m_pDib);
}


/* ========================================================================= */
/* 関数名	:CWindowBase::Create											 */
/* 内容		:作成															 */
/* 日付		:2006/11/03														 */
/* ========================================================================= */

void CWindowBase::Create(CMgrData *pMgrData)
{
	m_pMgrData		= pMgrData;
	m_pMgrDraw		= m_pMgrData->		GetMgrDraw();
	m_pMgrSound		= m_pMgrData->		GetMgrSound ();
	m_pMgrGrpData	= m_pMgrData->		GetMgrGrpData ();
	m_hWndMain		= m_pMgrData->		GetMainWindow ();
	m_pDibSystem	= m_pMgrGrpData->	GetDibSystem ();
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnWindowMsg										 */
/* 内容		:メッセージハンドラ(WM_WINDOWMSG)								 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

void CWindowBase::OnWindowMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CWindowBase::Update											 */
/* 内容		:更新															 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CWindowBase::Update(void)
{
	m_dwTimeDrawStart = 0;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::Draw												 */
/* 内容		:描画															 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

void CWindowBase::Draw(PCImg32 pDst)
{
	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	return;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::TimerProc											 */
/* 内容		:時間処理														 */
/* 日付		:2005/06/12														 */
/* ========================================================================= */

BOOL CWindowBase::TimerProc(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (timeGetTime () - m_dwLastTimeCursor > 300) {
		m_dwLastTimeCursor = timeGetTime ();
		m_nCursorAnime ++;
		if (m_nCursorAnime >= 2) {
			m_nCursorAnime = 0;
		}
	}

	if (m_dwTimeDrawStart == 0) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::KeyProc											 */
/* 内容		:キー処理														 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

void CWindowBase::KeyProc(
	BYTE byCode,		/* [in] イベント */
	BOOL bDown)			/* [in] 押下状態 */
{
	BOOL bResult;
	DWORD dwTime, dwTimeTmp;

	if (m_bActive == FALSE) {
		return;
	}

	dwTime = 250;
	if (m_nCountKeyRepeate > 1) {
		dwTime = 125;
	}
	dwTimeTmp = timeGetTime () - m_dwLastTimeKey;

	bResult = FALSE;
	if (byCode == 'B') {
		bResult = OnB (bDown);

	} else if (byCode == 'F') {
		bResult = OnF (bDown);

	} else if (byCode == 'I') {
		bResult = OnI (bDown);

	} else if (byCode == 'J') {
		bResult = OnJ (bDown);

	} else if (byCode == 'K') {
		bResult = OnK (bDown);

	} else if (byCode == 'L') {
		bResult = OnL (bDown);

	} else if (byCode == 'S') {
		bResult = OnS (bDown);

	} else if (byCode == 'X') {
		bResult = OnX (bDown);

	} else if (byCode == 'Z') {
		bResult = OnZ (bDown);

	} else if (byCode == VK_ESCAPE) {
		bResult = OnEscape (bDown);

	} else if (byCode == VK_SPACE) {
		bResult = OnSpace (bDown);

	} else if ((bDown) && (dwTimeTmp > dwTime)) {
		bResult = TRUE;
		if (byCode == VK_UP) {
			bResult = OnUp ();

		} else if (byCode == VK_DOWN) {
			bResult = OnDown ();

		} else if (byCode == VK_RIGHT) {
			bResult = OnRight ();

		} else if (byCode == VK_LEFT) {
			bResult = OnLeft ();

		} else {
			bResult = FALSE;
		}

		if (bResult) {
			m_nCursorAnime = 0;
			m_nCountKeyRepeate ++;
		}

	} else if (bDown == FALSE) {
		m_dwLastTimeKey		= 0;
		m_nCountKeyRepeate	= 0;
	}

	if (bResult) {
		m_dwTimeDrawStart	= 0;
		m_dwLastTimeKey		= timeGetTime ();
	}
}


/* ========================================================================= */
/* 関数名	:CWindowBase::SetShow											 */
/* 内容		:表示するか設定													 */
/* 日付		:2005/06/26														 */
/* ========================================================================= */

void CWindowBase::SetShow(BOOL bShow)
{
	m_bShow				= bShow;
	m_dwLastTimerProc	= 0;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::Redraw											 */
/* 内容		:再描画															 */
/* 日付		:2007/04/07														 */
/* ========================================================================= */

void CWindowBase::Redraw(void)
{
	m_dwTimeDrawStart = 0;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::SetPos											 */
/* 内容		:位置の設定														 */
/* 日付		:2007/04/23														 */
/* ========================================================================= */

void CWindowBase::SetPos(int nPos)
{
	m_nPos = nPos;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::AddChild											 */
/* 内容		:子ウィンドウを追加												 */
/* 日付		:2008/10/27														 */
/* ========================================================================= */

void CWindowBase::AddChild(CWindowBase *pChild)
{
	m_apChild.Add (pChild);
}


/* ========================================================================= */
/* 関数名	:CWindowBase::DeleteChild										 */
/* 内容		:子ウィンドウを削除												 */
/* 日付		:2008/10/27														 */
/* ========================================================================= */

void CWindowBase::DeleteChild(CWindowBase *pChild)
{
	int i, nCount;
	PCWindowBase pChildTmp;

	nCount = m_apChild.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pChildTmp = m_apChild[i];
		m_apChild.RemoveAt (i);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnUp												 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowBase::OnUp(void)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnDown											 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowBase::OnDown(void)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnLeft											 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowBase::OnLeft(void)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnRight											 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowBase::OnRight(void)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnB												 */
/* 内容		:キーハンドラ(B)												 */
/* 日付		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowBase::OnB(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnF												 */
/* 内容		:キーハンドラ(F)												 */
/* 日付		:2009/04/18														 */
/* ========================================================================= */

BOOL CWindowBase::OnF(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnI												 */
/* 内容		:キーハンドラ(I)												 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

BOOL CWindowBase::OnI(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnJ												 */
/* 内容		:キーハンドラ(J)												 */
/* 日付		:2009/04/02														 */
/* ========================================================================= */

BOOL CWindowBase::OnJ(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnK												 */
/* 内容		:キーハンドラ(K)												 */
/* 日付		:2009/04/18														 */
/* ========================================================================= */

BOOL CWindowBase::OnK(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnL												 */
/* 内容		:キーハンドラ(L)												 */
/* 日付		:2009/04/18														 */
/* ========================================================================= */

BOOL CWindowBase::OnL(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnS												 */
/* 内容		:キーハンドラ(S)												 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowBase::OnS(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnX												 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2005/06/21														 */
/* ========================================================================= */

BOOL CWindowBase::OnX(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnZ												 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2005/06/22														 */
/* ========================================================================= */

BOOL CWindowBase::OnZ(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnEscape											 */
/* 内容		:キーハンドラ(Escape)											 */
/* 日付		:2007/06/19														 */
/* ========================================================================= */

BOOL CWindowBase::OnEscape(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::OnSpace											 */
/* 内容		:キーハンドラ(Space)											 */
/* 日付		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowBase::OnSpace(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowBase::TextOut2											 */
/* 内容		:黒縁取りで文字描画												 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

void CWindowBase::TextOut2(HDC hDC, int x, int y, LPCSTR pStr, COLORREF Color, BOOL bDraw, COLORREF ColorFrame)
{
	int nLen;

	nLen = strlen (pStr);
	/* 縁取りする？ */
	if (bDraw) {
		SetTextColor (hDC, ColorFrame);
		TextOut (hDC, x - 1, y, pStr, nLen);
		TextOut (hDC, x + 1, y, pStr, nLen);
		TextOut (hDC, x, y - 1, pStr, nLen);
		TextOut (hDC, x, y + 1, pStr, nLen);
	}
	SetTextColor (hDC, Color);
	TextOut (hDC, x, y, pStr, nLen);
}


/* ========================================================================= */
/* 関数名	:CWindowBase::TextOut3											 */
/* 内容		:黒縁取りで文字描画(右詰)										 */
/* 日付		:2005/06/26														 */
/* ========================================================================= */

void CWindowBase::TextOut3(HDC hDC, int x, int y, int cx, int cy, LPCSTR pStr, COLORREF Color)
{
	int nLen;
	RECT rc;

	nLen = strlen (pStr);
	SetRect (&rc, x, y, cx, cy);
	SetTextColor (hDC, RGB(10, 10, 10));

	SetRect (&rc, x - 1, y, x + cx, y + cy);	DrawText (hDC, pStr, nLen, &rc, DT_RIGHT | DT_NOCLIP);
	SetRect (&rc, x + 1, y, x + cx, y + cy);	DrawText (hDC, pStr, nLen, &rc, DT_RIGHT | DT_NOCLIP);
	SetRect (&rc, x, y - 1, x + cx, y + cy);	DrawText (hDC, pStr, nLen, &rc, DT_RIGHT | DT_NOCLIP);
	SetRect (&rc, x, y + 1, x + cx, y + cy);	DrawText (hDC, pStr, nLen, &rc, DT_RIGHT | DT_NOCLIP);

	SetTextColor (hDC, Color);
	SetRect (&rc, x, y, x + cx, y + cy);		DrawText (hDC, pStr, nLen, &rc, DT_RIGHT | DT_NOCLIP);
}


/* ========================================================================= */
/* 関数名	:CWindowBase::TextOut4											 */
/* 内容		:黒縁取りで文字描画												 */
/* 日付		:2007/04/03														 */
/* ========================================================================= */

void CWindowBase::TextOut4(HDC hDC, int x, int y, LPCSTR pStr, COLORREF ColorFrame, COLORREF Color)
{
	int nLen;

	nLen = strlen (pStr);
	SetTextColor (hDC, ColorFrame);

	TextOut (hDC, x - 2, y, pStr, nLen);
	TextOut (hDC, x - 1, y, pStr, nLen);
	TextOut (hDC, x - 1, y - 2, pStr, nLen);
	TextOut (hDC, x - 2, y - 1, pStr, nLen);
	TextOut (hDC, x - 1, y - 1, pStr, nLen);
	TextOut (hDC, x - 2, y + 1, pStr, nLen);
	TextOut (hDC, x - 1, y + 1, pStr, nLen);
	TextOut (hDC, x - 1, y + 2, pStr, nLen);

	TextOut (hDC, x + 2, y, pStr, nLen);
	TextOut (hDC, x + 1, y, pStr, nLen);
	TextOut (hDC, x + 1, y - 2, pStr, nLen);
	TextOut (hDC, x + 2, y - 1, pStr, nLen);
	TextOut (hDC, x + 1, y - 1, pStr, nLen);
	TextOut (hDC, x + 2, y + 1, pStr, nLen);
	TextOut (hDC, x + 1, y + 1, pStr, nLen);
	TextOut (hDC, x + 1, y + 2, pStr, nLen);

	TextOut (hDC, x, y - 2, pStr, nLen);
	TextOut (hDC, x, y - 1, pStr, nLen);
	TextOut (hDC, x, y + 2, pStr, nLen);
	TextOut (hDC, x, y + 1, pStr, nLen);

	SetTextColor (hDC, Color);
	TextOut (hDC, x, y, pStr, nLen);
}


/* ========================================================================= */
/* 関数名	:CWindowBase::DrawFrame											 */
/* 内容		:フレームを描画													 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

void CWindowBase::DrawFrame(int nType)
{
	DrawFrame (0, 0, m_sizeWindow.cx, m_sizeWindow.cy, nType);
}


/* ========================================================================= */
/* 関数名	:CWindowBase::DrawFrame											 */
/* 内容		:フレームを描画													 */
/* 日付		:2007/04/01														 */
/* メモ		:１ブロックが16x16ドットの枠を描画								 */
/* ========================================================================= */

void CWindowBase::DrawFrame(int x, int y, int cx, int cy, int nType, BOOL bRightErace/*FALSE*/)
{
	typedef struct _FRAMEINFO {
		POINT		pt;		/* 左上の開始位置 */
		COLORREF	cl;		/* 背景色 */
		int			nSize;	/* 枠のサイズ */
		int			nPos;	/* 表示位置補正 */
	} FRAMEINFO, *PFRAMEINFO;
	int i, nTmp, xx, yy, nSize, nPos;
	COLORREF clTmp;
	FRAMEINFO astFrameinfo[] = {
		 0,	  0, RGB (255, 235, 200),	16, 32,
		 0,	 48, RGB (255, 235, 200),	16, 32,
		48,	 48, RGB (255, 235, 200),	16, 32,
		96,	 48, RGB (255, 235, 200),	16, 32,
		 0,	 96, RGB (255, 211, 76),	16, 32,
		 0,	816, RGB (255, 235, 200),	 8, 40,
		48,	816, RGB (255, 235, 200),	 8, 40,
		96,	816, RGB (196, 140, 81),	 8, 40
	};

	xx		= astFrameinfo[nType].pt.x;
	yy		= astFrameinfo[nType].pt.y;
	clTmp	= astFrameinfo[nType].cl;
	nTmp	= (nType == 4) ? 3 : 0;
	nSize	= astFrameinfo[nType].nSize;
	nPos	= astFrameinfo[nType].nPos;

	/* 背景塗りつぶし */
	m_pDib->FillRect (x + 0, y + 8, cx -  0, cy - 16, clTmp);
	m_pDib->FillRect (x + 8, y + 0, cx - 16, cy -  0, clTmp);

	/* 横線 */
	for (i = 0; i < cx / 8 - 2; i ++) {
		m_pDib->BltFrom256 (x + (i + 1) * 8, y,				8, 16, m_pDibSystem, xx + 16, yy, TRUE);
		m_pDib->BltFrom256 (x + (i + 1) * 8, y + cy - 8,	8, 8,  m_pDibSystem, xx + 16, yy + 40, TRUE);
	}

	/* 縦線 */
	for (i = 0; i < cy / 8 - 2; i ++) {
		if ((i == 1) && (nTmp != 0)) {
			m_pDib->BltFrom256 (x,			y + (i + 1) * 8 + nTmp, 8, 8 - nTmp, m_pDibSystem, xx,			yy + 16 + nTmp, TRUE);
			m_pDib->BltFrom256 (x + cx - 8,	y + (i + 1) * 8 + nTmp, 8, 8 - nTmp, m_pDibSystem, xx + 40,	yy + 16 + nTmp, TRUE);
		} else {
			m_pDib->BltFrom256 (x,			y + (i + 1) * 8, 8, 8, m_pDibSystem, xx,		yy + 16 + nTmp, TRUE);
			m_pDib->BltFrom256 (x + cx - 8,	y + (i + 1) * 8, 8, 8, m_pDibSystem, xx + 40,	yy + 16 + nTmp, TRUE);
		}
	}

	/* 四隅 */
	m_pDib->BltFrom256 (x, y,			16, 16 + nTmp,	m_pDibSystem, xx, yy, TRUE);
	m_pDib->BltFrom256 (x, y + cy - 16,	16, 16,			m_pDibSystem, xx, yy + 32, TRUE);
	if (bRightErace == FALSE) {
		m_pDib->BltFrom256 (x + cx - nSize, y, nSize, nSize + nTmp, m_pDibSystem, xx + nPos, yy, TRUE);
		m_pDib->BltFrom256 (x + cx - nSize, y + cy - nSize, nSize, nSize, m_pDibSystem, xx + nPos, yy + nPos, TRUE);
	}
}


/* ========================================================================= */
/* 関数名	:CWindowBase::DrawFrame2										 */
/* 内容		:フレームを描画													 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

void CWindowBase::DrawFrame2(int x, int y, int cx, int cy, int nType, CImg32 *pDst/*NULL*/, int nBottom/*0*/)
{
	int i, xx, yy, aTypeX[] = {0, 24, 0}, aTypeY[] = {144, 144, 168};
	COLORREF clTmp, clType[] = {RGB (255, 255, 255), RGB (255, 255, 255), RGB (255, 235, 200)};

	xx		= aTypeX[nType];
	yy		= aTypeY[nType];
	clTmp	= clType[nType];

	if (pDst == NULL) {
		pDst = m_pDib;
	}
	if (cx % 8) {
		cx += (8 - (cx % 8));
	}
	if (cy % 8) {
		cy += (8 - (cy % 8));
	}

	/* 背景塗りつぶし */
	pDst->FillRect (x, y, cx, cy - nBottom, clTmp);

	/* 四隅 */
	pDst->BltFrom256 (x - 8,	y - 8,	8, 8, m_pDibSystem, xx,		 yy, TRUE);
	pDst->BltFrom256 (x + cx,	y - 8,	8, 8, m_pDibSystem, xx + 16, yy, TRUE);
	pDst->BltFrom256 (x - 8,	y + cy - nBottom, 8, 8, m_pDibSystem, xx,		yy + 16, TRUE);
	pDst->BltFrom256 (x + cx,	y + cy - nBottom, 8, 8, m_pDibSystem, xx + 16,  yy + 16, TRUE);

	/* 横線 */
	for (i = 0; i < cx / 8; i ++) {
		pDst->BltFrom256 (x + i * 8, y - 8, 8, 8, m_pDibSystem, xx + 8, yy, TRUE);
		pDst->BltFrom256 (x + i * 8, y + cy - nBottom, 8, 8, m_pDibSystem, xx + 8, yy + 16,	TRUE);
	}

	/* 縦線 */
	for (i = 0; i < cy / 8; i ++) {
		pDst->BltFrom256 (x - 8, y + i * 8, 8, 8, m_pDibSystem, xx, yy + 8, TRUE);
		pDst->BltFrom256 (x + cx, y + i * 8 - nBottom, 8, 8, m_pDibSystem, xx + 16,	yy + 8, TRUE);
	}
}


/* ========================================================================= */
/* 関数名	:CWindowBase::DrawFrame3										 */
/* 内容		:フレームを描画													 */
/* 日付		:2007/04/02														 */
/* ========================================================================= */

void CWindowBase::DrawFrame3(int x, int y, int cx, int cy, int nType)
{
	int i, xx, yy, aTypeX[] = {96}, aTypeY[] = {48};
	COLORREF clTmp, clType[] = {RGB (139, 213, 255)};

	xx		= aTypeX[nType];
	yy		= aTypeY[nType];
	clTmp	= clType[nType];

	/* 背景塗りつぶし */
	m_pDib->FillRect (x + 16, y + 32, cx - 32, cy - 32, clTmp);

	/* 四隅 */
	m_pDib->BltFrom256 (x,				y,				16, 32, m_pDibSystem, xx,		yy,			TRUE);
	m_pDib->BltFrom256 (x + cx - 16,	y,				16, 32, m_pDibSystem, xx + 32,	yy,			TRUE);
	m_pDib->BltFrom256 (x,				y + cy - 16,	16, 16, m_pDibSystem, xx,		yy + 32,	TRUE);
	m_pDib->BltFrom256 (x + cx - 16,	y + cy - 16,	16, 16, m_pDibSystem, xx + 32,	yy + 32,	TRUE);

	/* 横線 */
	for (i = 0; i < cx / 16 - 2; i ++) {
		m_pDib->BltFrom256 (x + 16 + i * 16, y,			16, 32, m_pDibSystem, xx + 16, yy,		TRUE);
		m_pDib->BltFrom256 (x + 16 + i * 16, y + cy - 16,	16, 16, m_pDibSystem, xx + 16, yy + 32,	TRUE);
	}

	/* 縦線 */
	for (i = 0; i < cy / 16 - 2; i ++) {
		m_pDib->BltFrom256 (x,				y + 32 + i * 16, 16, 16, m_pDibSystem, xx,		yy + 24, TRUE);
		m_pDib->BltFrom256 (x + cx - 16,	y + 32 + i * 16, 16, 16, m_pDibSystem, xx + 32,	yy + 24, TRUE);
	}
}


/* ========================================================================= */
/* 関数名	:CWindowBase::DrawCursor										 */
/* 内容		:カーソルを描画													 */
/* 日付		:2005/06/16														 */
/* ========================================================================= */

void CWindowBase::DrawCursor(int x, int y)
{
//	if (m_bActive && (m_nCursorAnime == 0)) {
//		return;
//	}

	m_pDib->BltFrom256 (x, y, 24, 24, m_pDibSystem, 48, 0, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowBase::DrawInputFrame1									 */
/* 内容		:入力欄用フレームを描画1										 */
/* 日付		:2005/06/16														 */
/* ========================================================================= */

void CWindowBase::DrawInputFrame1(int x, int y, int cx, int cy, int nType)
{
	int i, yy, nCount, nyyType[] = {128, 152};

	yy		= nyyType[nType];
	nCount	= cx;

	m_pDib->BltFrom256 (x - 6, y - 6, 16, 24, m_pDibSystem, 48, yy, TRUE);
	for (i = 0; i < nCount; i ++) {
		m_pDib->BltFrom256 (x + i, y - 6, 1, 24, m_pDibSystem, 63, yy, TRUE);
	}
	m_pDib->BltFrom256 (x + i, y - 6, 5, 24, m_pDibSystem, 107, yy, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowBase::DrawIconFrame										 */
/* 内容		:アイコン用フレームを描画										 */
/* 日付		:2008/03/14														 */
/* ========================================================================= */

void CWindowBase::DrawIconFrame(int x, int y)
{
	m_pDib->BltFrom256 (x, y, 34, 45, m_pDibSystem, 0, 883, TRUE);
}

/* Copyright(C)URARA-works 2006 */
