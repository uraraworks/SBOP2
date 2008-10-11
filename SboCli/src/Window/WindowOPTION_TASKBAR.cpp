/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowOPTION_TASKBAR.cpp									 */
/* ���e			:�I�v�V����(�������Ƀ^�X�N�o�[�`�J�`�J)�E�B���h�E�N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/21													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowOPTION_TASKBAR.h"


/* ========================================================================= */
/* �֐���	:CWindowOPTION_TASKBAR::CWindowOPTION_TASKBAR					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

CWindowOPTION_TASKBAR::CWindowOPTION_TASKBAR()
{
	m_nPosMax		= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_OPTION_TASKBAR;
	m_ptViewPos.x	= 160;
	m_ptViewPos.y	= 180;
	m_sizeWindow.cx	= 16 * 2 + 16 * 5;
	m_sizeWindow.cy	= 16 * 2 + 16 * 2;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_TASKBAR::~CWindowOPTION_TASKBAR					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

CWindowOPTION_TASKBAR::~CWindowOPTION_TASKBAR()
{
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_TASKBAR::Create									 */
/* ���e		:�쐬															 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

void CWindowOPTION_TASKBAR::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
	m_nPos = (m_pMgrData->GetOptionTaskbar ()) ? 0 : 1;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_TASKBAR::Draw									 */
/* ���e		:�`��															 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

void CWindowOPTION_TASKBAR::Draw(PCImg32 pDst)
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

	TextOut2 (hDC, 32, 16 + 16 * 0, "ON",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 1, "OFF",	clText);

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
/* �֐���	:CWindowOPTION_TASKBAR::OnUp									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_TASKBAR::OnUp(void)
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
/* �֐���	:CWindowOPTION_TASKBAR::OnDown									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_TASKBAR::OnDown(void)
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
/* �֐���	:CWindowOPTION_TASKBAR::OnLeft									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_TASKBAR::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_TASKBAR::OnRight									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_TASKBAR::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_TASKBAR::OnX										 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_TASKBAR::OnX(BOOL bDown)
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
/* �֐���	:CWindowOPTION_TASKBAR::OnZ										 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_TASKBAR::OnZ(BOOL bDown)
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

/* Copyright(C)URARA-works 2008 */
