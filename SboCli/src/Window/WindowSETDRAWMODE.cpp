/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowSETDRAWMODE.cpp										 */
/* ���e			:�\�����e�̐ݒ�E�B���h�E�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSETDRAWMODE.h"


/* ========================================================================= */
/* �֐���	:CWindowSETDRAWMODE::CWindowSETDRAWMODE							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

CWindowSETDRAWMODE::CWindowSETDRAWMODE()
{
	m_nPosMax		= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_SETDRAWMODE;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 128;
	m_sizeWindow.cx	= 16 * 2 + 16 * 10;
	m_sizeWindow.cy	= 16 * 2 + 16 * 2;
}


/* ========================================================================= */
/* �֐���	:CWindowSETDRAWMODE::~CWindowSETDRAWMODE						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

CWindowSETDRAWMODE::~CWindowSETDRAWMODE()
{
}


/* ========================================================================= */
/* �֐���	:CWindowSETDRAWMODE::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

void CWindowSETDRAWMODE::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
	m_nPos = m_pMgrData->GetDrawMode ();
}


/* ========================================================================= */
/* �֐���	:CWindowSETDRAWMODE::Draw										 */
/* ���e		:�`��															 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

void CWindowSETDRAWMODE::Draw(PCImg32 pDst)
{
	int nLevel;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	TextOut2 (hDC, 32, 16 + 16 * 0, "���O��\������",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 1, "���O��\�����Ȃ�",	clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	DrawCursor (8, 16 + 16 * m_nPos);
	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowSETDRAWMODE::OnUp										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

BOOL CWindowSETDRAWMODE::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowSETDRAWMODE::OnDown										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

BOOL CWindowSETDRAWMODE::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax) {
		goto Exit;
	}
	m_nPos ++;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowSETDRAWMODE::OnLeft										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

BOOL CWindowSETDRAWMODE::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowSETDRAWMODE::OnRight									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

BOOL CWindowSETDRAWMODE::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowSETDRAWMODE::OnX										 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

BOOL CWindowSETDRAWMODE::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowSETDRAWMODE::OnZ										 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

BOOL CWindowSETDRAWMODE::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_CANCEL);

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2007 */
