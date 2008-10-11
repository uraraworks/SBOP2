/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowSYSTEMMENU.cpp										 */
/* ���e			:�V�X�e�����j���[�E�B���h�E�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/19													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSYSTEMMENU.h"


/* ========================================================================= */
/* �֐���	:CWindowSYSTEMMENU::CWindowSYSTEMMENU							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

CWindowSYSTEMMENU::CWindowSYSTEMMENU()
{
	m_nPosMax		= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_SYSTEMMENU;
	m_ptViewPos.x	= 8 * 5;
	m_ptViewPos.y	= 16 * 3;
	m_sizeWindow.cx	= 16 * 2 + 16 * 11;
	m_sizeWindow.cy	= 16 * 2 + 16 * 2;
}


/* ========================================================================= */
/* �֐���	:CWindowSYSTEMMENU::~CWindowSYSTEMMENU							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

CWindowSYSTEMMENU::~CWindowSYSTEMMENU()
{
}


/* ========================================================================= */
/* �֐���	:CWindowSYSTEMMENU::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

void CWindowSYSTEMMENU::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
}


/* ========================================================================= */
/* �֐���	:CWindowSYSTEMMENU::Draw										 */
/* ���e		:�`��															 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

void CWindowSYSTEMMENU::Draw(PCImg32 pDst)
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

	clText = RGB (1, 1, 1);
	TextOut2 (hDC, 32, 16 + 16 * 0, "���O�Ɣ����F�̐ݒ�",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 1, "�I�v�V����",			clText);

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
/* �֐���	:CWindowSYSTEMMENU::OnUp										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CWindowSYSTEMMENU::OnUp(void)
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
/* �֐���	:CWindowSYSTEMMENU::OnDown										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CWindowSYSTEMMENU::OnDown(void)
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
/* �֐���	:CWindowSYSTEMMENU::OnLeft										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CWindowSYSTEMMENU::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowSYSTEMMENU::OnRight										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CWindowSYSTEMMENU::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowSYSTEMMENU::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CWindowSYSTEMMENU::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, WINDOWTYPE_SYSTEMMENU, m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowSYSTEMMENU::OnZ											 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CWindowSYSTEMMENU::OnZ(BOOL bDown)
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


/* ========================================================================= */
/* �֐���	:CWindowSYSTEMMENU::OnEscape									 */
/* ���e		:�L�[�n���h��(Escape)											 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CWindowSYSTEMMENU::OnEscape(BOOL bDown)
{
	return OnZ (bDown);
}

/* Copyright(C)URARA-works 2007 */
