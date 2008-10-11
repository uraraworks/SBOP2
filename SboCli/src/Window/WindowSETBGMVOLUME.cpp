/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowSETBGMVOLUME.cpp										 */
/* ���e			:BGM���ʂ̐ݒ�E�B���h�E�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/21													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSETBGMVOLUME.h"


/* ========================================================================= */
/* �֐���	:CWindowSETBGMVOLUME::CWindowSETBGMVOLUME						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

CWindowSETBGMVOLUME::CWindowSETBGMVOLUME()
{
	m_nPosMax		= 4;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_SETBGMVOLUME;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 96;
	m_sizeWindow.cx	= 16 * 2 + 16 * 4;
	m_sizeWindow.cy	= 16 * 2 + 16 * 5;
}


/* ========================================================================= */
/* �֐���	:CWindowSETBGMVOLUME::~CWindowSETBGMVOLUME						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

CWindowSETBGMVOLUME::~CWindowSETBGMVOLUME()
{
}


/* ========================================================================= */
/* �֐���	:CWindowSETBGMVOLUME::Create									 */
/* ���e		:�쐬															 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

void CWindowSETBGMVOLUME::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_nPos = m_pMgrData->GetBGMVolume ();
}


/* ========================================================================= */
/* �֐���	:CWindowSETBGMVOLUME::Draw										 */
/* ���e		:�`��															 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

void CWindowSETBGMVOLUME::Draw(PCImg32 pDst)
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
	TextOut2 (hDC, 32, 16 + 16 * 0, "����",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 1, "�P",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 2, "�Q",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 3, "�R",	clText);
	TextOut2 (hDC, 32, 16 + 16 * 4, "�S",	clText);

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
/* �֐���	:CWindowSETBGMVOLUME::OnUp										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnUp(void)
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
/* �֐���	:CWindowSETBGMVOLUME::OnDown									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnDown(void)
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
/* �֐���	:CWindowSETBGMVOLUME::OnLeft									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowSETBGMVOLUME::OnRight									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowSETBGMVOLUME::OnX										 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnX(BOOL bDown)
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
/* �֐���	:CWindowSETBGMVOLUME::OnZ										 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

BOOL CWindowSETBGMVOLUME::OnZ(BOOL bDown)
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
