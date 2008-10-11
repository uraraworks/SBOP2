/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowEYECOLOR.cpp											 */
/* ���e			:�ڐF�I���E�B���h�E�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowEYECOLOR.h"

/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::CWindowEYECOLOR								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

CWindowEYECOLOR::CWindowEYECOLOR()
{
	m_nPos			= 1;
	m_nPosMax		= 6;
	m_nPosX			= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_EYECOLOR;
	m_ptViewPos.x	= 88;
	m_ptViewPos.y	= 160;
	m_sizeWindow.cx	= 16 * 2 + 16 * 8;
	m_sizeWindow.cy	= 16 * 2 + 16 * 2;
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::~CWindowEYECOLOR								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

CWindowEYECOLOR::~CWindowEYECOLOR()
{
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

void CWindowEYECOLOR::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

void CWindowEYECOLOR::Draw(PCImg32 pDst)
{
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	clText		= RGB (255, 127, 53);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont14);
	SetBkMode (hDC, TRANSPARENT);

	TextOut4 (hDC, 56, 8, "�ڂ̐F", clText);
	strTmp.Format ("%02d", m_nPos);
	TextOut4 (hDC, 16, 32, strTmp, clText);
	TextOut2 (hDC, 40, 32, m_pMgrData->GetEyeColorName ((WORD)m_nPos), clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_pDib->BltFrom256 (16 + m_nPosX * 8 - 2, 21, 10, 8, m_pDibSystem, 96, 24, TRUE);
	m_pDib->BltFrom256 (16 + m_nPosX * 8 - 2, 48, 10, 8, m_pDibSystem, 96, 32, TRUE);

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowEYECOLOR::TimerProc(void)
{
	BOOL bRet;
	int nCursorAnimeBack;

	bRet = FALSE;

	nCursorAnimeBack = m_nCursorAnime;
	CWindowBase::TimerProc ();
	if (nCursorAnimeBack == m_nCursorAnime) {
		goto Exit;
	}

	m_dwTimeDrawStart = 0;
	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::SetType										 */
/* ���e		:�ڐF��ݒ�														 */
/* ���t		:2007/04/18														 */
/* ========================================================================= */

void CWindowEYECOLOR::SetType(WORD wEyeColor)
{
	m_nPos = wEyeColor;
	Redraw ();
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::GetType										 */
/* ���e		:�ڐF���擾														 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

WORD CWindowEYECOLOR::GetType(void)
{
	return m_nPos;
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::OnUp											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowEYECOLOR::OnUp(void)
{
	int nTmp;
	BOOL bRet;

	bRet = FALSE;

	if (m_nPosX == 0) {
		nTmp = m_nPos / 10;
		nTmp = (nTmp + 1) * 10;
		m_nPos = (nTmp / 10 * 10) + (m_nPos % 10);
	} else {
		m_nPos ++;
	}
	m_nPos = (m_nPos > m_nPosMax) ? 1 : m_nPos;
	m_nPos = (m_nPos <= 0) ? m_nPosMax : m_nPos;

	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	bRet = TRUE;
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::OnDown										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowEYECOLOR::OnDown(void)
{
	int nTmp;
	BOOL bRet;

	bRet = FALSE;

	if (m_nPosX == 0) {
		nTmp = m_nPos / 10;
		nTmp -= 10;
		m_nPos = (nTmp / 10 * 10) + (m_nPos % 10);
	} else {
		nTmp = m_nPos % 10;
		nTmp --;
		m_nPos = (m_nPos / 10 * 10) + (nTmp % 10);
	}
	m_nPos = (m_nPos > m_nPosMax) ? 1 : m_nPos;
	m_nPos = (m_nPos <= 0) ? m_nPosMax : m_nPos;

	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	bRet = TRUE;
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::OnLeft										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowEYECOLOR::OnLeft(void)
{
	if (m_nPosX == 1) {
		m_nPosX = 0;
	}
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::OnRight										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowEYECOLOR::OnRight(void)
{
	if (m_nPosX == 0) {
		m_nPosX = 1;
	}
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowEYECOLOR::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 0);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowEYECOLOR::OnZ											 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowEYECOLOR::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound (SOUNDID_CANCEL);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, -1);
	m_bDelete = TRUE;

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2007 */
