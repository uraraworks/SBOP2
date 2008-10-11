/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowHAIRTYPE.cpp											 */
/* ���e			:���^�I���E�B���h�E�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowHAIRTYPE.h"

/* ========================================================================= */
/* �֐���	:CWindowHAIRTYPE::CWindowHAIRTYPE								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

CWindowHAIRTYPE::CWindowHAIRTYPE()
{
	m_nPos			= 1;
	m_nPosMax		= 4;//22;
	m_nPosX			= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_HAIRTYPE;
	m_ptViewPos.x	= 88;
	m_ptViewPos.y	= 160;
	m_sizeWindow.cx	= 16 * 2 + 16 * 8;
	m_sizeWindow.cy	= 16 * 2 + 8 * 5;
}


/* ========================================================================= */
/* �֐���	:CWindowHAIRTYPE::~CWindowHAIRTYPE								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

CWindowHAIRTYPE::~CWindowHAIRTYPE()
{
}


/* ========================================================================= */
/* �֐���	:CWindowHAIRTYPE::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

void CWindowHAIRTYPE::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);
}


/* ========================================================================= */
/* �֐���	:CWindowHAIRTYPE::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

void CWindowHAIRTYPE::Draw(PCImg32 pDst)
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

	TextOut4 (hDC, 64, 8, "���^", clText);
	strTmp.Format ("%02d", m_nPos);
	TextOut4 (hDC, 16, 32, strTmp, clText);
//	TextOut2 (hDC, 40, 32, m_pMgrData->GetHairTypeName ((WORD)m_nPos), clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_pDib->BltFrom256 (16 + m_nPosX * 8 - 2, 21, 10, 8, m_pDibSystem, 96, 24, TRUE);
	m_pDib->BltFrom256 (16 + m_nPosX * 8 - 2, 48, 10, 8, m_pDibSystem, 96, 32, TRUE);

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowHAIRTYPE::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRTYPE::TimerProc(void)
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
/* �֐���	:CWindowHAIRTYPE::SetType										 */
/* ���e		:���^��ݒ�														 */
/* ���t		:2007/04/18														 */
/* ========================================================================= */

void CWindowHAIRTYPE::SetType(WORD wHairType)
{
	m_nPos = wHairType;
	Redraw ();
}


/* ========================================================================= */
/* �֐���	:CWindowHAIRTYPE::GetType										 */
/* ���e		:���^���擾														 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

WORD CWindowHAIRTYPE::GetType(void)
{
	return m_nPos;
}


/* ========================================================================= */
/* �֐���	:CWindowHAIRTYPE::OnUp											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRTYPE::OnUp(void)
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
/* �֐���	:CWindowHAIRTYPE::OnDown										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRTYPE::OnDown(void)
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
/* �֐���	:CWindowHAIRTYPE::OnLeft										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRTYPE::OnLeft(void)
{
	if (m_nPosX == 1) {
		m_nPosX = 0;
	}
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowHAIRTYPE::OnRight										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRTYPE::OnRight(void)
{
	if (m_nPosX == 0) {
		m_nPosX = 1;
	}
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowHAIRTYPE::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRTYPE::OnX(BOOL bDown)
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
/* �֐���	:CWindowHAIRTYPE::OnZ											 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

BOOL CWindowHAIRTYPE::OnZ(BOOL bDown)
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
