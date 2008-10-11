/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:WindowCHARNAME.cpp											 */
/* ���e			:�L���������̓E�B���h�E�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "WindowCHARNAME.h"


/* ========================================================================= */
/* �֐���	:CWindowCHARNAME::CWindowCHARNAME								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

CWindowCHARNAME::CWindowCHARNAME()
{
	m_bInput				= TRUE;
	m_nID					= WINDOWTYPE_CHARNAME;
	m_ptViewPos.x			= 136 + 32;
	m_ptViewPos.y			= 180;
	m_sizeWindow.cx			= 16 * 2 + 8 * 15;
	m_sizeWindow.cy			= 16 * 2 + 16 * 3;
	m_hWndCharName			= NULL;
	m_OrgWndProcCharName	= NULL;
}


/* ========================================================================= */
/* �֐���	:CWindowCHARNAME::~CWindowCHARNAME								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

CWindowCHARNAME::~CWindowCHARNAME()
{
	HIMC hImc;

	if (m_hWndCharName) {
		/* IME���I�t�ɂ��� */
		hImc = ImmGetContext (m_hWndCharName);
		ImmSetOpenStatus (hImc, FALSE);
		ImmReleaseContext (m_hWndCharName, hImc);

		SetWindowLong (m_hWndCharName, GWL_WNDPROC, (LONG)m_OrgWndProcCharName);
		m_OrgWndProcCharName = NULL;
		SAFE_DESTROYWND (m_hWndCharName);
	}
}


/* ========================================================================= */
/* �֐���	:CWindowCHARNAME::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CWindowCHARNAME::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
	MakeWindow ();
}


/* ========================================================================= */
/* �֐���	:CWindowCHARNAME::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CWindowCHARNAME::Draw(PCImg32 pDst)
{
	HDC hDC;
	HFONT hFontOld;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();
	DrawInputFrame1 (16, 48, 8 * MAXLEN_CHARNAME, 14, 0);

	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont14);
	SetBkMode (hDC, TRANSPARENT);

	TextOut4 (hDC, 24, 16, "�L�����N�^�[��", RGB (255, 127, 53));

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowCHARNAME::SetActive										 */
/* ���e		:�A�N�e�B�u���ݒ�												 */
/* ���t		:2006/06/22														 */
/* ========================================================================= */

void CWindowCHARNAME::SetActive(BOOL bActive)
{
	HWND hWnd;

	CWindowBase::SetActive (bActive);

	hWnd = m_hWndCharName;

	EnableWindow (hWnd, bActive);
	if (bActive == FALSE) {
		hWnd = m_hWndMain;
	}
	SetFocus (hWnd);
}


/* ========================================================================= */
/* �֐���	:CWindowCHARNAME::MakeWindow									 */
/* ���e		:�E�B���h�E�쐬													 */
/* ���t		:2006/05/14														 */
/* ========================================================================= */

void CWindowCHARNAME::MakeWindow(void)
{
	HINSTANCE hInstance;
	HWND hWndMain;
	HIMC hImc;

	hInstance	= m_pMgrData->GetInstance ();
	hWndMain	= m_pMgrData->GetMainWindow ();

	/* �L���������͗� */
	m_hWndCharName = CreateWindow ("EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
			m_ptViewPos.x + 16, m_ptViewPos.y + 48, 8 * MAXLEN_CHARNAME, 14, hWndMain, NULL, hInstance, NULL);
	m_OrgWndProcCharName = (WNDPROC)GetWindowLong (m_hWndCharName, GWL_WNDPROC);
	SendMessage			(m_hWndCharName, WM_SETFONT, (WPARAM)GetStockObject (DEFAULT_GUI_FONT), 0);
	SetWindowLong		(m_hWndCharName, GWL_USERDATA, (LONG)this);
	SetWindowLong		(m_hWndCharName, GWL_WNDPROC, (LONG)CharNameWndProc);

	/* IME���I���ɂ��� */
	hImc = ImmGetContext (m_hWndCharName);
	ImmSetOpenStatus (hImc, TRUE);
	ImmReleaseContext (m_hWndCharName, hImc);

	SetFocus (m_hWndCharName);
}


/* ========================================================================= */
/* �֐���	:CWindowCHARNAME::CharNameWndProc								 */
/* ���e		:�L���������͗����b�Z�[�W����									 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

LRESULT CALLBACK CWindowCHARNAME::CharNameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nCount;
	char szTmp[128];
	LRESULT hResult;
	HIMC hImc;
	PCWindowCHARNAME pThis;

	pThis	= (PCWindowCHARNAME)GetWindowLong (hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_RETURN) {
			GetWindowText (hWnd, szTmp, sizeof (szTmp) - 1);
			nCount = strlen (szTmp);
			if (nCount > 0) {
				if (nCount >= MAXLEN_CHARNAME) {
					if (IsDBCSLeadByte ((BYTE)szTmp[MAXLEN_CHARNAME - 1])) {
						szTmp[MAXLEN_CHARNAME - 1] = 0;
					} else {
						szTmp[MAXLEN_CHARNAME] = 0;
					}
				}
				pThis->m_strName = szTmp;
				TrimViewString (pThis->m_strName, szTmp);
				PostMessage (pThis->m_hWndMain, WM_WINDOWMSG, WINDOWTYPE_CHARNAME, 0);
			}
			/* IME���I�t�ɂ��� */
			hImc = ImmGetContext (pThis->m_hWndCharName);
			ImmSetOpenStatus (hImc, FALSE);
			ImmReleaseContext (pThis->m_hWndCharName, hImc);
			hResult = 0;

		} else if (wParam == VK_ESCAPE) {
			pThis->m_bDelete = TRUE;
			hResult = 0;
		}
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc (pThis->m_OrgWndProcCharName, hWnd, message, wParam, lParam);
	}
	return hResult;
}

/* Copyright(C)URARA-works 2006 */
