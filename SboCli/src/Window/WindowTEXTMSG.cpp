/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowTEXTMSG.cpp											 */
/* 内容			:テキストメッセージ表示ウィンドウクラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Command.h"
#include "PacketCHAR_PARA1.h"
#include "UraraSockTCPSBO.h"
#include "Img32.h"
#include "LibInfoItem.h"
#include "InfoTalkEvent.h"
#include "InfoTalkEventPAGE.h"
#include "InfoTalkEventMENU.h"
#include "InfoCharCli.h"
#include "MgrKeyInput.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrSound.h"
#include "WindowTEXTMSG.h"
#include "myString.h"

#ifdef _UNICODE
namespace
{
static size_t GetUtf8CharLength(const char* psz)
{
        if ((psz == nullptr) || (*psz == '\0')) {
                return 0;
        }
        const unsigned char ch = static_cast<unsigned char>(*psz);
        if ((ch & 0x80u) == 0) {
                return 1;
        }
        if ((ch & 0xE0u) == 0xC0u) {
                return 2;
        }
        if ((ch & 0xF0u) == 0xE0u) {
                return 3;
        }
        if ((ch & 0xF8u) == 0xF0u) {
                return 4;
        }
        return 1;
}

static void CopyNextMessageChar(char* pszDst, size_t cchDst, const char* pszSrc)
{
        if ((pszDst == nullptr) || (pszSrc == nullptr) || (cchDst == 0)) {
                return;
        }

        const size_t len = GetUtf8CharLength(pszSrc);
        const size_t copyLen = (len < cchDst) ? len : (cchDst - 1);
        memcpy(pszDst, pszSrc, copyLen);
        pszDst[copyLen] = '\0';
}
}
#else
namespace
{
static void CopyNextMessageChar(char* pszDst, size_t cchDst, const char* pszSrc)
{
        if ((pszDst == nullptr) || (pszSrc == nullptr) || (cchDst == 0)) {
                return;
        }

        const char* pszNext = ::CharNextA(pszSrc);
        size_t len = static_cast<size_t>(pszNext - pszSrc);
        if (len >= cchDst) {
                len = cchDst - 1;
        }
        memcpy(pszDst, pszSrc, len);
        pszDst[len] = '\0';
}
}
#endif

/* 表示状態 */
enum {
	STATE_TEXT = 0,			/* メッセージ表示 */
	STATE_MENU,				/* 項目選択 */
	STATE_EVENTPROC,		/* 会話イベント処理 */
};

/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::CWindowTEXTMSG									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

CWindowTEXTMSG::CWindowTEXTMSG()
{
	m_nSpaceHeight	= 16 * 3;

	m_nID			= WINDOWTYPE_TEXTMSG;
	m_ptViewPos.x	= 16;
	m_sizeWindow.cx	= 16 * 2 + 16 * 26;
	m_sizeWindow.cy	= 16 * 2 + 16 * 5 + m_nSpaceHeight;
	m_ptViewPos.y	= SCRSIZEY - 16 - m_sizeWindow.cy;

	m_bSkip				= FALSE;
	m_bInputWait		= FALSE;
	m_nState			= STATE_TEXT;
	m_nType				= 0;
	m_nProcPos			= 0;
	m_nProcPosTmp		= 0;
	m_nProcEventPage	= 0;
	m_nProcEventNo		= 0;
	m_dwLastProc		= 0;
	m_pDibTitle			= NULL;
	m_pDibText			= NULL;
	m_ptDraw.x = m_ptDraw.y = 0;

	m_pInfoTalkEvent = NULL;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::~CWindowTEXTMSG								 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

CWindowTEXTMSG::~CWindowTEXTMSG()
{
	SAFE_DELETE (m_pDibTitle);
	SAFE_DELETE (m_pDibText);
	SAFE_DELETE (m_pInfoTalkEvent);
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::Create											 */
/* 内容		:作成															 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CWindowTEXTMSG::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pSock = m_pMgrData->GetUraraSockTCP ();
	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_pDibTitle = new CImg32;
	m_pDibTitle->Create (16 * 6, 16 * 2);
	m_pDibText = new CImg32;
	m_pDibText->Create (m_sizeWindow.cx - 16 * 2 + 2, m_sizeWindow.cy - 16 * 2 - m_nSpaceHeight + 2);
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::Draw											 */
/* 内容		:描画															 */
/* 日付		:2005/06/12														 */
/* ========================================================================= */

void CWindowTEXTMSG::Draw(PCImg32 pDst)
{
	int nTmp, x, y, cx, cy;
	SIZE sizeWindow;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	sizeWindow = m_sizeWindow;
	/* 初期化 */
	switch (m_nState) {
	case STATE_TEXT:		/* メッセージ表示 */
		break;
	case STATE_MENU:		/* 項目選択 */
		sizeWindow.cx /= 2;
		break;
	}

	m_pDib->FillRect (0, 0, m_pDib->Width (), m_pDib->Height (), RGB (0, 0, 0));
	switch (m_nState) {
	case STATE_TEXT:		/* メッセージ表示 */
		if ((m_ptDraw.x == 0) && (m_ptDraw.y == 0)) {
			break;
		}
		DrawFrame (0, m_nSpaceHeight, sizeWindow.cx, sizeWindow.cy - m_nSpaceHeight, m_nType);

		nTmp = 0;
		if (m_strTitle.IsEmpty () == FALSE) {
			nTmp ++;
		}
		if (m_strName.IsEmpty () == FALSE) {
			nTmp ++;
		}
		if (nTmp > 0) {
			cx = m_pDibTitle->Width () + 8 * 2;
			cy = 16 * (2 + nTmp);
			x = sizeWindow.cx / 2 - cx / 2;
			y = (2 - nTmp) * 16;
			DrawFrame (x, y, cx, cy, m_nType);
			m_pDib->Blt (x + 16, y + 16, m_pDibTitle->Width (), m_pDibTitle->Height (), m_pDibTitle, 0, 0, TRUE);
		}

		m_pDib->Blt (16, 16 + m_nSpaceHeight, m_pDibText->Width (), m_pDibText->Height (), m_pDibText, 0, 0, TRUE);
		break;
	case STATE_MENU:		/* 項目選択 */
		sizeWindow.cy = m_astrMenu.size() * 16 + 32;
		DrawFrame (0, m_nSpaceHeight, sizeWindow.cx, sizeWindow.cy, m_nType);

		nTmp = 0;
		if (m_strTitle.IsEmpty () == FALSE) {
			nTmp ++;
		}
		if (m_strName.IsEmpty () == FALSE) {
			nTmp ++;
		}
		if (nTmp > 0) {
			cx = m_pDibTitle->Width ()  + 16 * 2;
			cy = 16 * (2 + nTmp);
			x = sizeWindow.cx / 2 - cx / 2;
			y = (2 - nTmp) * 16;
			DrawFrame (x, y, cx, cy, m_nType);
			m_pDib->Blt (x + 16, y + 16, m_pDibTitle->Width (), m_pDibTitle->Height (), m_pDibTitle, 0, 0, TRUE);
		}
		{
			int i, nCount;
			HDC hDC;
			HFONT hFontOld;
			COLORREF clText;

			clText		= RGB (1, 1, 1);
			hDC			= m_pDib->Lock ();
			hFontOld	= (HFONT)SelectObject (hDC, m_hFont16Normal);
			SetBkMode (hDC, TRANSPARENT);

			nCount = m_astrMenu.size();
			for (i = 0; i < nCount; i ++) {
				TextOut2 (hDC, 32, y + cy + i * 16, (LPCTSTR)m_astrMenu[i], clText);
			}

			SelectObject (hDC, hFontOld);
			m_pDib->Unlock ();
		}
		DrawCursor (8, y + cy + 16 * m_nPos);
		break;
	}

	m_dwTimeDrawStart = timeGetTime ();
Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::TimerProc(void)
{
	BOOL bRet, bResult;
	int *pnProcPos;
	DWORD dwTimeTmp;
	LPCSTR pszTmp;
	char szTmp[8];
	PCInfoCharCli pPlayerChar;
	PCMgrKeyInput pMgrKeyInput;

	bRet = FALSE;
	pPlayerChar = m_pMgrData->GetPlayerChar ();

	if (m_pDibText == NULL) {
		goto Exit;
	}
	pMgrKeyInput = m_pMgrData->GetMgrKeyInput ();

	dwTimeTmp	= timeGetTime () - m_dwLastProc;
	bResult		= pMgrKeyInput->IsInput ('X');
	if (bResult == FALSE) {
		if (dwTimeTmp < 100) {
			goto Exit;
		}
	}
	m_dwLastProc = timeGetTime ();

	switch (m_nState) {
	case STATE_TEXT:		/* メッセージ表示 */
		ZeroMemory (szTmp, sizeof (szTmp));
		pnProcPos = &m_nProcPos;
		pszTmp = (LPCSTR)m_strMsg;
		if (m_strMsgTmp.IsEmpty () == FALSE) {
			pszTmp = (LPCSTR)m_strMsgTmp;
			pnProcPos = &m_nProcPosTmp;
		}

		if ((pszTmp[*pnProcPos] == 0) || (*pnProcPos >= (int)strlen (pszTmp))) {
			if (m_strMsgTmp.IsEmpty () == FALSE) {
				m_strMsgTmp.Empty ();
				m_nProcPosTmp = 0;
				goto Exit;
			}
			m_bInputWait = TRUE;
			goto Exit;
		}

                if (strncmp (&pszTmp[*pnProcPos], "\r\n", 2) == 0) {
                        CopyMemory (szTmp, "\r\n", 2);
                        szTmp[2] = '\0';

                } else {
                        CopyNextMessageChar (szTmp, _countof (szTmp), &pszTmp[*pnProcPos]);
                }

		if (szTmp[0] == '@') {
			MsgProc ();

		} else {
			*pnProcPos += strlen (szTmp);

			DrawChar (szTmp);
		}
		break;
	case STATE_MENU:		/* 項目選択 */
		break;
	case STATE_EVENTPROC:	/* 会話イベント処理 */
		{
			PCLibInfoItem pLibInfoItem = m_pMgrData->GetLibInfoItem ();
			PCInfoTalkEventBase pInfoTalkEvent = m_pInfoTalkEvent->GetPtr (m_nProcEventPage, m_nProcEventNo);
			if (pInfoTalkEvent == NULL) {
				m_bDelete = TRUE;
				break;
			}
			switch (pInfoTalkEvent->m_nEventType) {
			case TALKEVENTTYPE_PAGE:			/* ページ切り替え */
				{
					int i, nCount;
					BOOL bJump;
					PCInfoTalkEventPAGE pInfoTmp = (PCInfoTalkEventPAGE)pInfoTalkEvent;
					PARRAYDWORD padwItemID;

					bJump = FALSE;
					switch (pInfoTmp->m_nPageChgCondition) {
					case CHGPAGECONDITION_ITEM:			/* アイテムあり */
					case CHGPAGECONDITION_NOITEM:		/* アイテムなし */
						padwItemID = pPlayerChar->GetItem ();
						nCount = padwItemID->size();
						for (i = 0; i < nCount; i ++) {
							if (pInfoTmp->m_dwData == pLibInfoItem->GetItemTypeID (padwItemID->at(i))) {
								break;
							}
						}
						switch (pInfoTmp->m_nPageChgCondition) {
						case CHGPAGECONDITION_ITEM:			/* アイテムあり */
							if (i < nCount) {
								bJump = TRUE;
							}
							break;
						case CHGPAGECONDITION_NOITEM:		/* アイテムなし */
							if (i >= nCount) {
								bJump = TRUE;
							}
							break;
						}
						break;
					default:
						bJump = TRUE;
						break;
					}
					m_nProcEventNo ++;
					if (bJump) {
						m_nProcEventPage = pInfoTmp->m_nPageJump;
						m_nProcEventNo	 = 0;
					}
				}
				break;
			case TALKEVENTTYPE_MSG:				/* メッセージ表示 */
				SetMsg ((LPCSTR)pInfoTalkEvent->m_strText);
				m_nState = STATE_TEXT;
				break;
			case TALKEVENTTYPE_MENU:			/* 項目選択 */
				{
					int i, nCount;
					PSTTALKEVENTMENUINFO pMenuInfo;	
					PCInfoTalkEventMENU pInfoTmp = (PCInfoTalkEventMENU)pInfoTalkEvent;

					m_astrMenu.clear();
					nCount = pInfoTmp->GetMenuInfoCount ();
					for (i = 0; i < nCount; i ++) {
						pMenuInfo = pInfoTmp->GetPtr (i);
						m_astrMenu.push_back (pMenuInfo->strName);
					}
					m_nState	= STATE_MENU;
					m_nPos		= 0;
					m_nPosMax	= nCount - 1;
				}
				break;
			case TALKEVENTTYPE_ADDSKILL:		/* スキル追加 */
				{
					CPacketCHAR_PARA1 PacketPara1;

					PacketPara1.Make (SBOCOMMANDID_SUB_CHAR_REQ_ADDSKILL, pPlayerChar->m_dwCharID, pInfoTalkEvent->m_dwData);
					m_pSock->Send (&PacketPara1);
					m_nProcEventNo ++;
				}
				break;
			}
		}
		break;
	}

	Redraw ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::SetTitle										 */
/* 内容		:肩書きを設定													 */
/* 日付		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::SetTitle(LPCSTR pszTitle)
{
	m_strTitle = pszTitle;
	RenewTitle ();
	Redraw ();
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::SetName										 */
/* 内容		:名前を設定														 */
/* 日付		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::SetName(LPCSTR pszName)
{
	int nLen;

	m_strName = pszName;
	nLen = max (m_strName.GetLength (), 6);
	m_pDibTitle->Destroy ();
	m_pDibTitle->Create (8 * nLen + 8, 16 * 2);
	RenewTitle ();
	Redraw ();
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::SetMsg											 */
/* 内容		:メッセージ設定													 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CWindowTEXTMSG::SetMsg(LPCSTR pszMsg)
{
	m_ptDraw.x = m_ptDraw.y = 0;
	m_nProcPos		= 0;
	m_dwLastProc	= timeGetTime ();
	m_strMsg		= pszMsg;

	m_pDibText->FillRect (0, 0, m_pDibText->Width (), m_pDibText->Height (), RGB (0, 0, 0));
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::SetTalkEvent									 */
/* 内容		:会話イベント設定												 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CWindowTEXTMSG::SetTalkEvent(CInfoTalkEvent *pInfo)
{
	SAFE_DELETE (m_pInfoTalkEvent);
	m_pInfoTalkEvent = new CInfoTalkEvent;
	m_pInfoTalkEvent->Copy (pInfo);

	m_nProcEventPage	= 0;
	m_nProcEventNo		= 0;
	m_nState			= STATE_EVENTPROC;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::OnUp											 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2008/11/27														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nState) {
	case STATE_MENU:		/* 項目選択 */
		if (m_nPos <= 0) {
			goto Exit;
		}
		m_nPos --;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::OnDown											 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2008/11/27														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nState) {
	case STATE_MENU:		/* 項目選択 */
		if (m_nPos >= m_nPosMax) {
			goto Exit;
		}
		m_nPos ++;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::OnLeft											 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2008/11/27														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnLeft(void)
{
	switch (m_nState) {
	case STATE_MENU:		/* 項目選択 */
		m_nPos = 0;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
		break;
	}

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::OnRight										 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2008/11/27														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnRight(void)
{
	switch (m_nState) {
	case STATE_MENU:		/* 項目選択 */
		m_nPos = m_nPosMax;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
		break;
	}

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnX(BOOL bDown)
{
	if (bDown == FALSE) {
		switch (m_nState) {
		case STATE_TEXT:		/* メッセージ表示 */
			if (m_bSkip) {
				m_bSkip = FALSE;
				break;
			}
			if (m_bInputWait) {
				m_bInputWait = FALSE;
				m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
				if (m_pInfoTalkEvent) {
					m_nProcEventNo ++;
					m_nState = STATE_EVENTPROC;
					break;
				}
				m_bDelete = TRUE;
			}
			break;
		case STATE_MENU:		/* 項目選択 */
			m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
			m_nState = STATE_TEXT;
			if (m_pInfoTalkEvent) {
				PSTTALKEVENTMENUINFO pMenuInfo;	
				PCInfoTalkEventMENU pInfoTmp = (PCInfoTalkEventMENU)m_pInfoTalkEvent->GetPtr (m_nProcEventPage, m_nProcEventNo);
				pMenuInfo = pInfoTmp->GetPtr (m_nPos);
				m_nProcEventPage = pMenuInfo->nPage;
				m_nProcEventNo = 0;
				m_nState = STATE_EVENTPROC;
			}
			break;
		}
	} else {
		if (m_bInputWait == FALSE) {
			m_bSkip = TRUE;
		}
	}

	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::OnZ											 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnZ(BOOL bDown)
{
	if (bDown == FALSE) {
		switch (m_nState) {
		case STATE_TEXT:		/* メッセージ表示 */
			if (m_bInputWait) {
				m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
				m_bDelete = TRUE;
			}
			break;
		case STATE_MENU:		/* 項目選択 */
			m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
			m_bDelete = TRUE;
			break;
		}
	}

	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::DrawChar										 */
/* 内容		:１文字表示														 */
/* 日付		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::DrawChar(LPCSTR pszText)
{
	int cx, cy;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	LPCTSTR pszDraw = NULL;

	if (pszText == NULL) {
		return;
	}

#ifdef _UNICODE
	CString strDraw = Utf8ToTString(pszText);
	pszDraw = (LPCTSTR)strDraw;
#else
	pszDraw = pszText;
#endif

	if ((pszDraw == NULL) || (*pszDraw == 0)) {
		return;
	}

	cx = m_pDibText->Width ();
	cy = m_pDibText->Height () - 16;

	clText			= RGB (1, 1, 1);
	hDC			= m_pDibText->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont16Normal);
	SetBkMode (hDC, TRANSPARENT);

	clText = RGB (1, 1, 1);
	TextOut2 (hDC, m_ptDraw.x, m_ptDraw.y, pszDraw, clText);

	if (strncmp (pszText, "\r\n", 2) == 0) {
		m_ptDraw.x = cx;
	}

	// 文字幅を実際の描画幅で進める
	SIZE sizeText = {0, 0};
#ifdef UNICODE
	GetTextExtentPoint32W(hDC, pszDraw, (int)_tcslen(pszDraw), &sizeText);
#else
	GetTextExtentPoint32A(hDC, pszDraw, (int)strlen(pszDraw), &sizeText);
#endif
	m_ptDraw.x += sizeText.cx;
	if (m_ptDraw.x + 8 >= cx) {
		m_ptDraw.x = 0;
		m_ptDraw.y += 16;
		if (m_ptDraw.y >= cy) {
			m_ptDraw.y -= 16;
			m_pDibText->Blt (0, 0, cx, cy, m_pDibText, 0, 16);
			m_pDibText->FillRect (0, cy, cx, 16, RGB (0, 0, 0));
		}
	}

	SelectObject (hDC, hFontOld);
	m_pDibText->Unlock ();
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::RenewTitle										 */
/* 内容		:肩書と名前画像を更新											 */
/* 日付		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::RenewTitle(void)
{
	int y;
	HDC hDC;
	COLORREF clText, clFrame;
	HFONT hFontOld;

	m_pDibTitle->FillRect (0, 0, m_pDibTitle->Width (), m_pDibTitle->Height (), RGB (0, 0, 0));

	hDC			= m_pDibTitle->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont16);
	SetBkMode (hDC, TRANSPARENT);

	y = 1;
	clText  = RGB (255, 255, 255);
	clFrame = RGB (1, 1, 1);
	if (m_strTitle.GetLength () > 0) {
		TextOut2 (hDC, 1, 1, (LPCTSTR)m_strTitle, clText, TRUE, clFrame);
		y += 16;
	}
	if (m_strName.GetLength () > 0) {
		TextOut2 (hDC, 1, y, (LPCTSTR)m_strName, clText, TRUE, clFrame);
	}

	SelectObject (hDC, hFontOld);
	m_pDibTitle->Unlock ();
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::InitText										 */
/* 内容		:メッセージ画像を初期化											 */
/* 日付		:2008/11/26														 */
/* ========================================================================= */

void CWindowTEXTMSG::InitText(void)
{
	m_ptDraw.x = m_ptDraw.y = 0;
	m_pDibText->FillRect (0, 0, m_pDibText->Width (), m_pDibText->Height (), RGB (0, 0, 0));
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::MsgProc										 */
/* 内容		:メッセージ解析処理												 */
/* 日付		:2008/11/27														 */
/* ========================================================================= */

void CWindowTEXTMSG::MsgProc(void)
{
	BOOL bSkip;
	int nPos, nLen;
	char szTmp[2];
	LPCSTR pszSrc, pszTmp;
	CmyString strTmp, strTmp2;

	pszSrc	= (LPCSTR)m_strMsg;
	pszSrc	= &pszSrc[m_nProcPos];
	nLen	= strlen (pszSrc);

	GetLineText (pszSrc, strTmp);
	pszTmp = (LPCSTR)strTmp;

	m_astrMenu.clear();

	nPos = 0;
	/* メニュー？ */
	if (_strnicmp(&pszTmp[nPos], "@menu", 5) == 0) {
		while (1) {
			nPos += (strTmp.GetLength () + 2);
			if (nPos >= nLen) {
				break;
			}

			GetLineText (&pszSrc[nPos], strTmp);
			pszTmp = (LPCSTR)strTmp;
			if (pszTmp[0] == '{') {
				continue;
			}
			if (pszTmp[0] == '}') {
				nPos += (strTmp.GetLength () + 2);
				break;
			}
			strTmp2 = strTmp;
			TrimSpace (strTmp2);
			m_astrMenu.push_back (strTmp2);
		}
		m_nProcPos += nPos;
		m_nPos		= 0;
		m_nPosMax	= m_astrMenu.size();
		if (m_nPosMax > 0) {
			m_nPosMax --;
			m_nState = STATE_MENU;
		}

	/* 項目選択？ */
	} else if (_strnicmp(&pszTmp[nPos], "@select", 7) == 0) {
		szTmp[0] = pszTmp[nPos + 8];
		szTmp[1] = 0;
		bSkip = FALSE;
		if (m_nPos != atoi (szTmp)) {
			bSkip = TRUE;
		}
		m_nProcPosTmp = 0;
		m_strMsgTmp.Empty ();
		while (1) {
			nPos += (strTmp.GetLength () + 2);
			if (nPos >= nLen) {
				break;
			}

			GetLineText (&pszSrc[nPos], strTmp);
			pszTmp = (LPCSTR)strTmp;
			if (pszTmp[0] == '{') {
				continue;
			}
			if (pszTmp[0] == '}') {
				nPos += (strTmp.GetLength () + 2);
				break;
			}
			if (bSkip == FALSE) {
				strTmp2 = strTmp;
				TrimSpace (strTmp2);
                        m_strMsgTmp += (LPCTSTR)strTmp2;
			}
		}
		m_nProcPos += nPos;
	}
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::GetLineText									 */
/* 内容		:1行取得														 */
/* 日付		:2008/11/27														 */
/* ========================================================================= */

void CWindowTEXTMSG::GetLineText(LPCSTR pszSrc, CmyString &strDst)
{
	int i, nLen;
	char szTmp[8];

	strDst.Empty ();

	nLen = strlen (pszSrc);
        for (i = 0; i < nLen - 1;) {
                ZeroMemory (szTmp, sizeof (szTmp));
                CopyNextMessageChar (szTmp, _countof (szTmp), &pszSrc[i]);
                if (szTmp[0] == 0) {
                        break;
                }

                if (strncmp (&pszSrc[i], "\r\n", 2) == 0) {
                        break;
                }
                strDst += szTmp;
                i += static_cast<int>(strlen (szTmp));
        }
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::TrimSpace										 */
/* 内容		:前後の空白を除去												 */
/* 日付		:2008/11/27														 */
/* ========================================================================= */

void CWindowTEXTMSG::TrimSpace(CmyString &strSrc)
{
	int i, nLen;
	char szTmp[8];
	LPCSTR pszSrc;
	CmyString strTmp;

	strTmp = strSrc;
	pszSrc = (LPCSTR)strTmp;

        nLen = strSrc.GetLength ();
        for (i = 0; i < nLen;) {
                ZeroMemory (szTmp, sizeof (szTmp));
                CopyNextMessageChar (szTmp, _countof (szTmp), &pszSrc[i]);
                if (szTmp[0] == 0) {
                        break;
                }

                if ((strcmp (szTmp, " ") == 0) || (strcmp (szTmp, "　") == 0)) {
                        i += static_cast<int>(strlen (szTmp));
                        continue;
                }
                strSrc = &pszSrc[i];
                break;
        }
}


/* ========================================================================= */
/* 関数名	:CWindowTEXTMSG::GetBlock										 */
/* 内容		:大かっこ1ブロック分取得										 */
/* 日付		:2008/11/27														 */
/* ========================================================================= */

void CWindowTEXTMSG::GetBlock(LPCSTR pszSrc, CmyString &strDst)
{
	strDst.Empty ();
}

/* Copyright(C)URARA-works 2008 */
