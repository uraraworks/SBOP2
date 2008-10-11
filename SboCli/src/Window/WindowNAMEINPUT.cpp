/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowNAMEINPUT.cpp										 */
/* ���e			:���O���̓E�B���h�E�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/10													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoCharBase.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "MgrDraw.h"
#include "WindowCHARNAME.h"
#include "WindowNAMEINPUT.h"

/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::CWindowNAMEINPUT								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

CWindowNAMEINPUT::CWindowNAMEINPUT()
{
	m_nPos			= 0;
	m_nPosMax		= 2;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_NAMEINPUT;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 80;
	m_sizeWindow.cx	= 320;
	m_sizeWindow.cy	= 320;

	m_pMgrWindow		= NULL;
	m_pMgrDraw			= NULL;
	m_pWindowCHARNAME	= NULL;
	m_pInfoCharCli		= new CInfoCharCli;
}


/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::~CWindowNAMEINPUT							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

CWindowNAMEINPUT::~CWindowNAMEINPUT()
{
	SAFE_DELETE (m_pInfoCharCli);
}


/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

void CWindowNAMEINPUT::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pMgrWindow	= m_pMgrData->GetMgrWindow ();
	m_pMgrDraw		= m_pMgrData->GetMgrDraw ();

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_pInfoCharCli->Create (m_pMgrData);
	m_pInfoCharCli->m_dwMotionTypeID = 2;
	m_pInfoCharCli->MakeCharGrp ();
	m_pMgrData->GetLibInfoChar ()->RenewMotionInfo (m_pInfoCharCli);

	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);
}


/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::OnWindowMsg									 */
/* ���e		:���b�Z�[�W�n���h��(WM_WINDOWMSG)								 */
/* ���t		:2007/04/11														 */
/* ========================================================================= */

void CWindowNAMEINPUT::OnWindowMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case WINDOWTYPE_CHARNAME:		/* �L���������� */
		m_pInfoCharCli->m_strCharName = m_pWindowCHARNAME->m_strName;
		m_pMgrWindow->Delete (WINDOWTYPE_CHARNAME);
		m_pWindowCHARNAME = NULL;
		Redraw ();
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

void CWindowNAMEINPUT::Draw(PCImg32 pDst)
{
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	POINT nCursorPos[] = {
		108, 240,
		132, 292
	};

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame (2);
	DrawFrame (112, 96, 100, 104, 2);
	DrawFrame2 (108, 240, 16 * 7, 8, 2);

	clText		= RGB (124, 123, 232);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont14);
	SetBkMode (hDC, TRANSPARENT);

	TextOut4 (hDC, 88,	24,		"�V�K�L�����N�^�[�쐬", clText);
	TextOut4 (hDC, 136,	48,		"���O����", clText);
	TextOut4 (hDC, 112,	216,	"�L�����N�^�[��", clText);
	TextOut4 (hDC, 148,	288,	"�o�^", clText);
	TextOut4 (hDC, 256,	288,	"�R�^�R", clText);
	TextOut2 (hDC, 108, 240 - 2, m_pInfoCharCli->m_strCharName, clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	DrawCursor (nCursorPos[m_nPos].x - 32, nCursorPos[m_nPos].y - 6);
	m_dwTimeDrawStart = timeGetTime ();

	m_pMgrDraw->DrawChar (m_pDib, 130, 124, m_pInfoCharCli);

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::TimerProc									 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::TimerProc(void)
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
/* �֐���	:CWindowNAMEINPUT::OnUp											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::OnDown										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax - 1) {
		goto Exit;
	}
	m_nPos ++;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::OnLeft										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnLeft(void)
{
	m_nPos = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::OnRight										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnRight(void)
{
	m_nPos = m_nPosMax - 1;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 1);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnX(BOOL bDown)
{
	BOOL bRet, bResult;
	CLibInfoCharBase LibInfoChar;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	switch (m_nPos) {
	case 0:	/* ���O */
		m_pMgrWindow->MakeWindowCHARNAME ();
		m_pWindowCHARNAME = (PCWindowCHARNAME)m_pMgrWindow->GetWindow (WINDOWTYPE_CHARNAME);
		break;
	case 1:	/* �o�^ */
		if (m_pInfoCharCli->m_strCharName.IsEmpty ()) {
			m_pMgrWindow->MakeWindowMSG ("���O����͂��Ă�������", 3000);
			break;
		}
		bResult = LibInfoChar.NameCheck (m_pInfoCharCli->m_strCharName);
		if (bResult == FALSE) {
			m_pMgrWindow->MakeWindowMSG ("���O�ɋ󔒂͎g���܂���", 3000);
			break;
		}
		PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, 0);
		break;
	}
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowNAMEINPUT::OnZ											 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

BOOL CWindowNAMEINPUT::OnZ(BOOL bDown)
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
