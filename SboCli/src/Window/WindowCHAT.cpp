/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowCHAT.cpp												 */
/* 内容			:チャット情報ウィンドウクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/02/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "PacketCHAR_REQ_CHAT.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "WindowCHAT.h"


/* ========================================================================= */
/* 関数名	:CWindowCHAT::CWindowCHAT										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/02/03														 */
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
/* 関数名	:CWindowCHAT::~CWindowCHAT										 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/02/03														 */
/* ========================================================================= */

CWindowCHAT::~CWindowCHAT()
{
	HIMC hImc;

	if (m_hWndChat) {
		/* IMEをオフにする */
		hImc = ImmGetContext (m_hWndChat);
		ImmSetOpenStatus (hImc, FALSE);
		ImmReleaseContext (m_hWndChat, hImc);

		SetWindowLong (m_hWndChat, GWL_WNDPROC, (LONG)m_OrgWndProcChat);
		m_OrgWndProcChat = NULL;
		SAFE_DESTROYWND (m_hWndChat);
	}
}


/* ========================================================================= */
/* 関数名	:CWindowCHAT::Create											 */
/* 内容		:作成															 */
/* 日付		:2007/02/03														 */
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
/* 関数名	:CWindowCHAT::Draw												 */
/* 内容		:描画															 */
/* 日付		:2007/02/03														 */
/* ========================================================================= */

void CWindowCHAT::Draw(PCImg32 pDst)
{
	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	/* タイトル */
	m_pDib->BltFrom256 (8, 0, 40, 16, m_pDibSystem, 48, 96);

	/* 枠 */
	DrawInputFrame1 (8 + 6, 16 + 6, m_sizeWindow.cx - 16 - 6 - 6, 14, 0);

	/* チャット種別アイコン */
	m_pDib->BltFrom256 (50, 0, 16, 16, m_pDibSystem, 48 + (m_aArrayType[m_nType] * 16), 112);

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowCHAT::SetActive											 */
/* 内容		:アクティブか設定												 */
/* 日付		:2007/02/03														 */
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
/* 関数名	:CWindowCHAT::GetType											 */
/* 内容		:チャット種別を取得												 */
/* 日付		:2007/07/29														 */
/* ========================================================================= */

int CWindowCHAT::GetType(void)
{
	int nRet;

	nRet = m_aArrayType[m_nType];

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CWindowCHAT::MakeWindow										 */
/* 内容		:ウィンドウ作成													 */
/* 日付		:2007/02/03														 */
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
	SetWindowLong		(m_hWndChat, GWL_WNDPROC, (LONG)ChatWndProcEntry);

	/* IMEをオンにする */
	hImc = ImmGetContext (m_hWndChat);
	ImmSetOpenStatus (hImc, TRUE);
	ImmReleaseContext (m_hWndChat, hImc);

	SetFocus (m_hWndChat);
}


/* ========================================================================= */
/* 関数名	:CWindowCHAT::ChatWndProcEntry									 */
/* 内容		:チャット入力欄プロシージャ										 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

LRESULT CALLBACK CWindowCHAT::ChatWndProcEntry(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hResult;
	PCWindowCHAT pThis;

	pThis	= (PCWindowCHAT)GetWindowLong (hWnd, GWL_USERDATA);
	hResult	= -1;

	if (pThis) {
		hResult = pThis->ChatWndProc (hWnd, message, wParam, lParam);
	}
	return hResult;
}


/* ========================================================================= */
/* 関数名	:CWindowCHAT::ChatWndProc										 */
/* 内容		:チャット入力欄プロシージャ										 */
/* 日付		:2007/02/03														 */
/* ========================================================================= */

LRESULT CWindowCHAT::ChatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hResult = -1;

	switch (message) {
	case WM_CHAR:
		switch (wParam) {
		case VK_RETURN:
		case VK_ESCAPE:
			hResult = 0;
			break;
		}
		break;

	case WM_KEYDOWN:
		{
			int nCount;
			nCount = m_aArrayType.GetSize ();

			switch (wParam) {
			case VK_RETURN:
				m_bPushEnter = TRUE;
				hResult = 0;
				break;
			case VK_ESCAPE:
				m_bPushEsc = TRUE;
				hResult = 0;
				break;
			case VK_UP:
				m_nType --;
				if (m_nType < 0) {
					m_nType = nCount - 1;
				}
				m_dwTimeDrawStart = 0;
				hResult = 0;
				break;

			case VK_DOWN:
				m_nType ++;
				if (m_nType >= nCount) {
					m_nType = 0;
				}
				m_dwTimeDrawStart = 0;
				hResult = 0;
				break;
			}
		}
		break;

	case WM_KEYUP:
		switch (wParam) {
		case VK_RETURN:
			{
				char szTmp[256], szTmp2[256];

				if (m_bPushEnter == FALSE) {
					break;
				}
				GetWindowText (hWnd, szTmp, sizeof (szTmp) - 1);
				if (strlen (szTmp) > 0) {
					ZeroMemory (szTmp2, sizeof (szTmp2));
					_tcsnccat (szTmp2, szTmp, 100);
					TrimViewString (m_strChat, szTmp2);
					m_pMgrData->SetChatModeBack (m_nType);
					PostMessage (m_hWndMain, WM_WINDOWMSG, WINDOWTYPE_CHAT, 0);

				} else {
					m_bDelete = TRUE;
				}
				m_bPushEnter = FALSE;
				hResult = 0;
			}
			break;
		case VK_ESCAPE:
			if (m_bPushEsc == FALSE) {
				break;
			}
			m_bPushEsc = FALSE;
			m_bDelete  = TRUE;
			m_pMgrData->SetChatModeBack (m_nType);
			hResult = 0;
			break;
		}
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc (m_OrgWndProcChat, hWnd, message, wParam, lParam);
	}
	return hResult;
}

/* Copyright(C)URARA-works 2007 */
