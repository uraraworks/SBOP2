/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:WindowBase.cpp												 */
/* ���e			:�E�B���h�E���N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WindowBase.h"

/* ========================================================================= */
/* �֐���	:CWindowBase::CWindowBase										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/05/06														 */
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
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "�l�r �o�S�V�b�N");
	m_hFont12 = CreateFont (12, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "�l�r �o�S�V�b�N");
	m_hFont14 = CreateFont (14, 0, 0, 0, FW_ULTRABOLD,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "�l�r �o�S�V�b�N");

	m_pDib->SetColorKey (RGB (255, 0, 255));
}


/* ========================================================================= */
/* �֐���	:CWindowBase::~CWindowBase										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/05/06														 */
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
	if (m_hFont14) {
		DeleteObject (m_hFont14);
		m_hFont14 = NULL;
	}
	SAFE_DELETE (m_pDib);
}


/* ========================================================================= */
/* �֐���	:CWindowBase::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2006/11/03														 */
/* ========================================================================= */

void CWindowBase::Create(CMgrData *pMgrData)
{
	m_pMgrData		= pMgrData;
	m_pMgrSound		= m_pMgrData->		GetMgrSound ();
	m_pMgrGrpData	= m_pMgrData->		GetMgrGrpData ();
	m_hWndMain		= m_pMgrData->		GetMainWindow ();
	m_pDibSystem	= m_pMgrGrpData->	GetDibSystem ();
}


/* ========================================================================= */
/* �֐���	:CWindowBase::OnWindowMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_WINDOWMSG)								 */
/* ���t		:2007/04/07														 */
/* ========================================================================= */

void CWindowBase::OnWindowMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CWindowBase::Update											 */
/* ���e		:�X�V															 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CWindowBase::Update(void)
{
	m_dwTimeDrawStart = 0;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2005/05/06														 */
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
/* �֐���	:CWindowBase::TimerProc											 */
/* ���e		:���ԏ���														 */
/* ���t		:2005/06/12														 */
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
/* �֐���	:CWindowBase::KeyProc											 */
/* ���e		:�L�[����														 */
/* ���t		:2005/05/06														 */
/* ========================================================================= */

void CWindowBase::KeyProc(
	BYTE byCode,		/* [in] �C�x���g */
	BOOL bDown)			/* [in] ������� */
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

	} else if (byCode == 'X') {
		bResult = OnX (bDown);

	} else if (byCode == 'Z') {
		bResult = OnZ (bDown);

	} else if (byCode == 'I') {
		bResult = OnI (bDown);

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
/* �֐���	:CWindowBase::SetShow											 */
/* ���e		:�\�����邩�ݒ�													 */
/* ���t		:2005/06/26														 */
/* ========================================================================= */

void CWindowBase::SetShow(BOOL bShow)
{
	m_bShow				= bShow;
	m_dwLastTimerProc	= 0;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::Redraw											 */
/* ���e		:�ĕ`��															 */
/* ���t		:2007/04/07														 */
/* ========================================================================= */

void CWindowBase::Redraw(void)
{
	m_dwTimeDrawStart = 0;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::SetPos											 */
/* ���e		:�ʒu�̐ݒ�														 */
/* ���t		:2007/04/23														 */
/* ========================================================================= */

void CWindowBase::SetPos(int nPos)
{
	m_nPos = nPos;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::AddChild											 */
/* ���e		:�q�E�B���h�E��ǉ�												 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

void CWindowBase::AddChild(CWindowBase *pChild)
{
	m_apChild.Add (pChild);
}


/* ========================================================================= */
/* �֐���	:CWindowBase::DeleteChild										 */
/* ���e		:�q�E�B���h�E���폜												 */
/* ���t		:2008/10/27														 */
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
/* �֐���	:CWindowBase::OnUp												 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowBase::OnUp(void)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::OnDown											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowBase::OnDown(void)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::OnLeft											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowBase::OnLeft(void)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::OnRight											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2005/06/17														 */
/* ========================================================================= */

BOOL CWindowBase::OnRight(void)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::OnB												 */
/* ���e		:�L�[�n���h��(B)												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowBase::OnB(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::OnX												 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2005/06/21														 */
/* ========================================================================= */

BOOL CWindowBase::OnX(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::OnZ												 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2005/06/22														 */
/* ========================================================================= */

BOOL CWindowBase::OnZ(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::OnI												 */
/* ���e		:�L�[�n���h��(I)												 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

BOOL CWindowBase::OnI(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::OnEscape											 */
/* ���e		:�L�[�n���h��(Escape)											 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CWindowBase::OnEscape(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::OnSpace											 */
/* ���e		:�L�[�n���h��(Space)											 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowBase::OnSpace(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowBase::TextOut2											 */
/* ���e		:�������ŕ����`��												 */
/* ���t		:2005/05/06														 */
/* ========================================================================= */

void CWindowBase::TextOut2(HDC hDC, int x, int y, LPCSTR pStr, COLORREF Color, BOOL bDraw)
{
	int nLen;

	nLen = strlen (pStr);
	/* ����肷��H */
	if (bDraw) {
		SetTextColor (hDC, RGB(10, 10, 10));
		TextOut (hDC, x - 1, y, pStr, nLen);
		TextOut (hDC, x + 1, y, pStr, nLen);
		TextOut (hDC, x, y - 1, pStr, nLen);
		TextOut (hDC, x, y + 1, pStr, nLen);
	}
	SetTextColor (hDC, Color);
	TextOut (hDC, x, y, pStr, nLen);
}


/* ========================================================================= */
/* �֐���	:CWindowBase::TextOut3											 */
/* ���e		:�������ŕ����`��(�E�l)										 */
/* ���t		:2005/06/26														 */
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
/* �֐���	:CWindowBase::TextOut4											 */
/* ���e		:�������ŕ����`��												 */
/* ���t		:2007/04/03														 */
/* ========================================================================= */

void CWindowBase::TextOut4(HDC hDC, int x, int y, LPCSTR pStr, COLORREF Color)
{
	int nLen;

	nLen = strlen (pStr);
	SetTextColor (hDC, Color);
#if 0
	TextOut (hDC, x - 1, y, pStr, nLen);
	TextOut (hDC, x - 1, y - 1, pStr, nLen);
	TextOut (hDC, x + 1, y, pStr, nLen);
	TextOut (hDC, x + 1, y + 1, pStr, nLen);
	TextOut (hDC, x, y - 1, pStr, nLen);
	TextOut (hDC, x + 1, y - 1, pStr, nLen);
	TextOut (hDC, x, y + 1, pStr, nLen);
	TextOut (hDC, x - 1, y + 1, pStr, nLen);
#else
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
#endif
	SetTextColor (hDC, RGB (255, 255, 255));
	TextOut (hDC, x, y, pStr, nLen);
}


/* ========================================================================= */
/* �֐���	:CWindowBase::DrawFrame											 */
/* ���e		:�t���[����`��													 */
/* ���t		:2005/05/06														 */
/* ========================================================================= */

void CWindowBase::DrawFrame(int nType)
{
	DrawFrame (0, 0, m_sizeWindow.cx, m_sizeWindow.cy, nType);
}


/* ========================================================================= */
/* �֐���	:CWindowBase::DrawFrame											 */
/* ���e		:�t���[����`��													 */
/* ���t		:2007/04/01														 */
/* ========================================================================= */

void CWindowBase::DrawFrame(int x, int y, int cx, int cy, int nType)
{
	int i, nTmp, xx, yy, aTypeX[] = {0, 0, 48, 96, 0}, aTypeY[] = {0, 48, 48, 48, 96};
	COLORREF clTmp, clType[] = {RGB (255, 235, 200), RGB (255, 235, 200), RGB (255, 235, 200), RGB (255, 235, 200), RGB (255, 211, 76)};

	xx		= aTypeX[nType];
	yy		= aTypeY[nType];
	clTmp	= clType[nType];
	nTmp	= (nType == 4) ? 3 : 0;

	/* �w�i�h��Ԃ� */
	m_pDib->FillRect (x + 16, y + 16, cx - 32, cy - 32, clTmp);

	/* �l�� */
	m_pDib->BltFrom256 (x,				y,				16, 16 + nTmp,	m_pDibSystem, xx, yy, TRUE);
	m_pDib->BltFrom256 (x + cx - 16,	y,				16, 16 + nTmp,	m_pDibSystem, xx + 32, yy, TRUE);
	m_pDib->BltFrom256 (x,				y + cy - 16,	16, 16,			m_pDibSystem, xx, yy + 32, TRUE);
	m_pDib->BltFrom256 (x + cx - 16,	y + cy - 16,	16, 16,			m_pDibSystem, xx + 32, yy + 32, TRUE);

	/* ���� */
	for (i = 1; i < cx / 8 - 2; i ++) {
		m_pDib->BltFrom256 (x + (i + 1) * 8, y,				8, 16, m_pDibSystem, xx + 16, yy, TRUE);
		m_pDib->BltFrom256 (x + (i + 1) * 8, y + cy - 16,	8, 16, m_pDibSystem, xx + 16, yy + 32, TRUE);
	}

	/* �c�� */
	for (i = 1; i < cy / 8 - 2; i ++) {
		if ((i == 1) && (nTmp != 0)) {
			m_pDib->BltFrom256 (x,				y + (i + 1) * 8 + nTmp, 16, 8 - nTmp, m_pDibSystem, xx,			yy + 16 + nTmp, TRUE);
			m_pDib->BltFrom256 (x + cx - 16,	y + (i + 1) * 8 + nTmp, 16, 8 - nTmp, m_pDibSystem, xx + 32,	yy + 16 + nTmp, TRUE);
		} else {
			m_pDib->BltFrom256 (x,				y + (i + 1) * 8, 16, 8, m_pDibSystem, xx,		yy + 16 + nTmp, TRUE);
			m_pDib->BltFrom256 (x + cx - 16,	y + (i + 1) * 8, 16, 8, m_pDibSystem, xx + 32,	yy + 16 + nTmp, TRUE);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CWindowBase::DrawFrame2										 */
/* ���e		:�t���[����`��													 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

void CWindowBase::DrawFrame2(int x, int y, int cx, int cy, int nType, CImg32 *pDst/*NULL*/)
{
	int i, xx, yy, aTypeX[] = {0, 24, 0}, aTypeY[] = {144, 144, 168};
	COLORREF clTmp, clType[] = {RGB (255, 235, 200), RGB (255, 255, 255), RGB (255, 235, 200)};

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

	/* �w�i�h��Ԃ� */
	pDst->FillRect (x, y, cx, cy, clTmp);

	/* �l�� */
	pDst->BltFrom256 (x - 8,	y - 8,	8, 8, m_pDibSystem, xx,		 yy,		TRUE);
	pDst->BltFrom256 (x + cx,	y - 8,	8, 8, m_pDibSystem, xx + 16, yy,		TRUE);
	pDst->BltFrom256 (x - 8,	y + cy,	8, 8, m_pDibSystem, xx,		 yy + 16,	TRUE);
	pDst->BltFrom256 (x + cx,	y + cy,	8, 8, m_pDibSystem, xx + 16, yy + 16,	TRUE);

	/* ���� */
	for (i = 0; i < cx / 8; i ++) {
		pDst->BltFrom256 (x + i * 8, y - 8,		8, 8, m_pDibSystem, xx + 8, yy,			TRUE);
		pDst->BltFrom256 (x + i * 8, y + cy,	8, 8, m_pDibSystem, xx + 8, yy + 16,	TRUE);
	}

	/* �c�� */
	for (i = 0; i < cy / 8; i ++) {
		pDst->BltFrom256 (x - 8,	y + i * 8, 8, 8, m_pDibSystem, xx,		yy + 8, TRUE);
		pDst->BltFrom256 (x + cx,	y + i * 8, 8, 8, m_pDibSystem, xx + 16,	yy + 8, TRUE);
	}
}


/* ========================================================================= */
/* �֐���	:CWindowBase::DrawFrame3										 */
/* ���e		:�t���[����`��													 */
/* ���t		:2007/04/02														 */
/* ========================================================================= */

void CWindowBase::DrawFrame3(int x, int y, int cx, int cy, int nType)
{
	int i, xx, yy, aTypeX[] = {96}, aTypeY[] = {48};
	COLORREF clTmp, clType[] = {RGB (139, 213, 255)};

	xx		= aTypeX[nType];
	yy		= aTypeY[nType];
	clTmp	= clType[nType];

	/* �w�i�h��Ԃ� */
	m_pDib->FillRect (x + 16, y + 32, cx - 32, cy - 32, clTmp);

	/* �l�� */
	m_pDib->BltFrom256 (x,				y,				16, 32, m_pDibSystem, xx,		yy,			TRUE);
	m_pDib->BltFrom256 (x + cx - 16,	y,				16, 32, m_pDibSystem, xx + 32,	yy,			TRUE);
	m_pDib->BltFrom256 (x,				y + cy - 16,	16, 16, m_pDibSystem, xx,		yy + 32,	TRUE);
	m_pDib->BltFrom256 (x + cx - 16,	y + cy - 16,	16, 16, m_pDibSystem, xx + 32,	yy + 32,	TRUE);

	/* ���� */
	for (i = 0; i < cx / 16 - 2; i ++) {
		m_pDib->BltFrom256 (x + 16 + i * 16, y,			16, 32, m_pDibSystem, xx + 16, yy,		TRUE);
		m_pDib->BltFrom256 (x + 16 + i * 16, y + cy - 16,	16, 16, m_pDibSystem, xx + 16, yy + 32,	TRUE);
	}

	/* �c�� */
	for (i = 0; i < cy / 16 - 2; i ++) {
		m_pDib->BltFrom256 (x,				y + 32 + i * 16, 16, 16, m_pDibSystem, xx,		yy + 24, TRUE);
		m_pDib->BltFrom256 (x + cx - 16,	y + 32 + i * 16, 16, 16, m_pDibSystem, xx + 32,	yy + 24, TRUE);
	}
}


/* ========================================================================= */
/* �֐���	:CWindowBase::DrawCursor										 */
/* ���e		:�J�[�\����`��													 */
/* ���t		:2005/06/16														 */
/* ========================================================================= */

void CWindowBase::DrawCursor(int x, int y)
{
//	if (m_bActive && (m_nCursorAnime == 0)) {
//		return;
//	}

	m_pDib->BltFrom256 (x, y, 24, 24, m_pDibSystem, 48, 0, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowBase::DrawInputFrame1									 */
/* ���e		:���͗��p�t���[����`��1										 */
/* ���t		:2005/06/16														 */
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

/* Copyright(C)URARA-works 2006 */
