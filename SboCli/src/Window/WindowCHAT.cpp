/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowCHAT.cpp												 */
/* ���e			:�`���b�g���E�B���h�E�N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "PacketCHAR_REQ_CHAT.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "WindowCHAT.h"


/* ========================================================================= */
/* �֐���	:CWindowCHAT::CWindowCHAT										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/02/03														 */
/* ========================================================================= */

CWindowCHAT::CWindowCHAT()
{
	m_bInput				= TRUE;
	m_bPushEnter			= FALSE;
	m_bPushEsc				= FALSE;
	m_nID					= WINDOWTYPE_CHAT;
	m_nType					= 0;
	m_ptViewPos.y			= SCRSIZEY - 88;
	m_sizeWindow.cx			= SCRSIZEX - (m_ptViewPos.x * 2) - 6;
	m_sizeWindow.cy			= 16 + 14 + 6 + 4;
	m_ptViewPos.x			= SCRSIZEX / 2 - m_sizeWindow.cx / 2;
	m_hWndChat				= NULL;
	m_OrgWndProcChat		= NULL;
}


/* ========================================================================= */
/* �֐���	:CWindowCHAT::~CWindowCHAT										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/02/03														 */
/* ========================================================================= */

CWindowCHAT::~CWindowCHAT()
{
	HIMC hImc;

	if (m_hWndChat) {
		/* IME���I�t�ɂ��� */
		hImc = ImmGetContext (m_hWndChat);
		ImmSetOpenStatus (hImc, FALSE);
		ImmReleaseContext (m_hWndChat, hImc);

		SetWindowLong (m_hWndChat, GWL_WNDPROC, (LONG)m_OrgWndProcChat);
		m_OrgWndProcChat = NULL;
		SAFE_DESTROYWND (m_hWndChat);
	}
}


/* ========================================================================= */
/* �֐���	:CWindowCHAT::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2007/02/03														 */
/* ========================================================================= */

void CWindowCHAT::Create(CMgrData *pMgrData)
{
	int nResult;

	CWindowBase::Create (pMgrData);

	m_aArrayType.Add (0);
	nResult = m_pMgrData->GetAdminLevel ();
	if (nResult == ADMINLEVEL_ALL) {
		m_aArrayType.Add (CHATTYPE_ADMIN);
	}
	m_nType = m_pMgrData->GetChatModeBack ();
	if (m_nType >= m_aArrayType.GetSize ()) {
		m_nType = 0;
	}

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (RGB (255, 0, 255));
	m_pDib->FillRect (0, 0, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib->GetColorKey ());
	MakeWindow ();
}


/* ========================================================================= */
/* �֐���	:CWindowCHAT::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2007/02/03														 */
/* ========================================================================= */

void CWindowCHAT::Draw(PCImg32 pDst)
{
	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	/* �^�C�g�� */
	m_pDib->BltFrom256 (8, 0, 40, 16, m_pDibSystem, 48, 96);

	/* �g */
	DrawInputFrame1 (8 + 6, 16 + 6, m_sizeWindow.cx - 16 - 6 - 6, 14, 0);

	/* �`���b�g��ʃA�C�R�� */
	m_pDib->BltFrom256 (50, 0, 16, 16, m_pDibSystem, 48 + (m_aArrayType[m_nType] * 16), 112);

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowCHAT::SetActive											 */
/* ���e		:�A�N�e�B�u���ݒ�												 */
/* ���t		:2007/02/03														 */
/* ========================================================================= */

void CWindowCHAT::SetActive(BOOL bActive)
{
	HWND hWnd;

	CWindowBase::SetActive (bActive);

	hWnd = m_hWndChat;

	EnableWindow (hWnd, bActive);
	if (bActive == FALSE) {
		hWnd = m_hWndMain;
	}
	SetFocus (hWnd);
}


/* ========================================================================= */
/* �֐���	:CWindowCHAT::GetType											 */
/* ���e		:�`���b�g��ʂ��擾												 */
/* ���t		:2007/07/29														 */
/* ========================================================================= */

int CWindowCHAT::GetType(void)
{
	int nRet;

	nRet = m_aArrayType[m_nType];

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CWindowCHAT::MakeWindow										 */
/* ���e		:�E�B���h�E�쐬													 */
/* ���t		:2007/02/03														 */
/* ========================================================================= */

void CWindowCHAT::MakeWindow(void)
{
	HINSTANCE hInstance;
	HWND hWndMain;
	HIMC hImc;

	hInstance	= m_pMgrData->GetInstance ();
	hWndMain	= m_pMgrData->GetMainWindow ();

	m_hWndChat = CreateWindow ("EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
			m_ptViewPos.x + 8 + 6, m_ptViewPos.y + 16 + 6, m_sizeWindow.cx - (16 + 6 + 6), 14, hWndMain, NULL, hInstance, NULL);
	m_OrgWndProcChat = (WNDPROC)GetWindowLong (m_hWndChat, GWL_WNDPROC);
	SendMessage			(m_hWndChat, WM_SETFONT, (WPARAM)GetStockObject (DEFAULT_GUI_FONT), 0);
	SetWindowLong		(m_hWndChat, GWL_USERDATA, (LONG)this);
	SetWindowLong		(m_hWndChat, GWL_WNDPROC, (LONG)ChatWndProc);

	/* IME���I���ɂ��� */
	hImc = ImmGetContext (m_hWndChat);
	ImmSetOpenStatus (hImc, TRUE);
	ImmReleaseContext (m_hWndChat, hImc);

	SetFocus (m_hWndChat);
}


/* ========================================================================= */
/* �֐���	:CWindowCHAT::ChatWndProc										 */
/* ���e		:�`���b�g���͗��v���V�[�W��										 */
/* ���t		:2007/02/03														 */
/* ========================================================================= */

LRESULT CALLBACK CWindowCHAT::ChatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nCount;
	char szTmp[256], szTmp2[256];
	PCWindowCHAT pThis;
	LRESULT hResult;

	pThis	= (PCWindowCHAT)GetWindowLong (hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_RETURN) {
			hResult = 0;

		} else if (wParam == VK_ESCAPE) {
			hResult = 0;
		}
		break;

	case WM_KEYDOWN:
		nCount = pThis->m_aArrayType.GetSize ();

		switch (wParam) {
		case VK_RETURN:
			pThis->m_bPushEnter = TRUE;
			hResult = 0;
			break;
		case VK_ESCAPE:
			pThis->m_bPushEsc = TRUE;
			hResult = 0;
			break;
		case VK_UP:
			pThis->m_nType --;
			if (pThis->m_nType < 0) {
				pThis->m_nType = nCount - 1;
			}
			pThis->m_dwTimeDrawStart = 0;
			hResult = 0;
			break;

		case VK_DOWN:
			pThis->m_nType ++;
			if (pThis->m_nType >= nCount) {
				pThis->m_nType = 0;
			}
			pThis->m_dwTimeDrawStart = 0;
			hResult = 0;
			break;
		}
		break;

	case WM_KEYUP:
		switch (wParam) {
		case VK_RETURN:
			if (pThis->m_bPushEnter == FALSE) {
				break;
			}
			GetWindowText (hWnd, szTmp, sizeof (szTmp) - 1);
			if (strlen (szTmp) > 0) {
				ZeroMemory (szTmp2, sizeof (szTmp2));
				_tcsnccat (szTmp2, szTmp, 100);
				TrimViewString (pThis->m_strChat, szTmp2);
				pThis->m_pMgrData->SetChatModeBack (pThis->m_nType);
				PostMessage (pThis->m_hWndMain, WM_WINDOWMSG, WINDOWTYPE_CHAT, 0);
			} else {
				pThis->m_bDelete = TRUE;
			}
			pThis->m_bPushEnter = FALSE;
			hResult = 0;
			break;
		case VK_ESCAPE:
			if (pThis->m_bPushEsc == FALSE) {
				break;
			}
			pThis->m_bPushEsc = FALSE;
			pThis->m_bDelete  = TRUE;
			pThis->m_pMgrData->SetChatModeBack (pThis->m_nType);
			hResult = 0;
			break;
		}
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc (pThis->m_OrgWndProcChat, hWnd, message, wParam, lParam);
	}
	return hResult;
}

/* Copyright(C)URARA-works 2007 */
