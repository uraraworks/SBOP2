/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowCOMMANDMENU.cpp										 */
/* ���e			:�R�}���h���j���[�E�B���h�E�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowCOMMANDMENU.h"


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::CWindowCOMMANDMENU							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

CWindowCOMMANDMENU::CWindowCOMMANDMENU()
{
	m_nPos			= 2;
	m_nPosMax		= 4;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_COMMANDMENU;
	m_sizeWindow.cx	= 166;
	m_sizeWindow.cy	= 50 + 67;
	m_ptViewPos.x	= (SCRSIZEX / 2) - (m_sizeWindow.cx / 2);
	m_ptViewPos.y	= SCRSIZEY - 54 - 67;

	m_nPosSub = 0;
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::~CWindowCOMMANDMENU						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

CWindowCOMMANDMENU::~CWindowCOMMANDMENU()
{
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

void CWindowCOMMANDMENU::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	RenewCommand ();
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::Draw										 */
/* ���e		:�`��															 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

void CWindowCOMMANDMENU::Draw(PCImg32 pDst)
{
	int nLevel, x, y;
	HDC hDC;
	HFONT hFontOld;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_pDib->FillRect (0, 0, m_sizeWindow.cx, m_sizeWindow.cy, 0);
	m_pDib->BltFrom256 (0, 67, 166, 50, m_pDibSystem, 492, 263, TRUE);

	x = 492 + m_nPos * 38;
	y = 194;
	m_pDib->BltFrom256 (m_nPos * 32, 0, 38, 69, m_pDibSystem, x, y, TRUE);

	x = 3 + m_nPos * 32;
	y = 3 + 67;
	y -= ((m_nPosSub * 32) + 3);
	m_pDib->BltFrom256 (x - 8, y, 24, 24, m_pDibSystem, 72, 0, TRUE);
	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);

	if (m_strCommand.IsEmpty () == FALSE) {
		hDC			= pDst->Lock ();
		hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
		SetBkMode (hDC, TRANSPARENT);

		x = 3 + m_nPos * 32;
		y = 3;
		y -= ((m_nPosSub * 32) + 3);
		DrawFrame2 (m_ptViewPos.x + 32 + x - 8, m_ptViewPos.y + 32 + 67 + y - 20, m_strCommand.GetLength () * 6, 16, 0, pDst);
		TextOut2 (hDC, m_ptViewPos.x + 32 + x - 8, m_ptViewPos.y + 32 + 67 + y - 20, (LPCSTR)m_strCommand, RGB (10, 10, 10), FALSE);

		SelectObject (hDC, hFontOld);
		pDst->Unlock ();
	}
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::OnUp										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowCOMMANDMENU::OnUp(void)
{
	int anSubMax[] = {1, 2, 1, 2, 2};
	BOOL bRet;

	bRet = FALSE;

	if (m_nPosSub >= anSubMax[m_nPos]) {
		goto Exit;
	}
	m_nPosSub ++;

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	RenewCommand ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::OnDown										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowCOMMANDMENU::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPosSub == 0) {
		goto Exit;
	}
	m_nPosSub --;

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	RenewCommand ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::OnLeft										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowCOMMANDMENU::OnLeft(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos == 0) {
		goto Exit;
	}

	m_nPos --;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	RenewCommand ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::OnRight									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowCOMMANDMENU::OnRight(void)
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
	RenewCommand ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::OnB										 */
/* ���e		:�L�[�n���h��(B)												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowCOMMANDMENU::OnB(BOOL bDown)
{
	return OnZ (bDown);
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::OnX										 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowCOMMANDMENU::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::OnZ										 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CWindowCOMMANDMENU::OnZ(BOOL bDown)
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
/* �֐���	:CWindowCOMMANDMENU::OnSpace									 */
/* ���e		:�L�[�n���h��(Space)											 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CWindowCOMMANDMENU::OnSpace(BOOL bDown)
{
	return OnZ (bDown);
}


/* ========================================================================= */
/* �֐���	:CWindowCOMMANDMENU::RenewCommand								 */
/* ���e		:�I�𒆂̃R�}���h���X�V											 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

void CWindowCOMMANDMENU::RenewCommand(void)
{
	int anPosSubMax[] = {1, 2, 1, 2, 2};

	m_nPosSub = min (m_nPosSub, anPosSubMax[m_nPos]);
	m_strCommand.Empty ();

	switch (m_nPos) {
	case 0:		/* �L�����N�^�[(C) */
		switch (m_nPosSub) {
		case 1:
			m_strCommand = "�i�s���̃N�G�X�g";
			break;
		default:
			m_strCommand = "�L�����N�^�[(C)";
			break;
		}
		break;
	case 1:		/* �X�L��(S) */
		switch (m_nPosSub) {
		case 1:
			m_strCommand = "�퓬�X�L��(F)";
			break;
		case 2:
			m_strCommand = "�����X�L��(L)";
			break;
		default:
			m_strCommand = "�X�L��(S)";
			break;
		}
		break;
	case 2:		/* �o�b�O(B) */
		switch (m_nPosSub) {
		case 1:
			m_strCommand = "�}�b�v(M)";
			break;
		default:
			m_strCommand = "�o�b�O(B)";
			break;
		}
		break;
	case 3:		/* ����(I) */
		switch (m_nPosSub) {
		case 1:
			m_strCommand = "�p�[�e�B�[(P)";
			break;
		case 2:
			m_strCommand = "�M���h(G)";
			break;
		default:
			m_strCommand = "����(I)";
			break;
		}
		break;
	case 4:		/* �V�X�e��(ESC) */
		switch (m_nPosSub) {
		case 1:
			m_strCommand = "�w���v(H)";
			break;
		case 2:
			m_strCommand = "�f�l�R�[��";
			break;
		default:
			m_strCommand = "�V�X�e��(ESC)";
			break;
		}
		break;
	}
}

/* Copyright(C)URARA-works 2008 */
