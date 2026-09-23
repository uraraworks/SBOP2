/// @file WindowTEXTMSG.cpp
/// @brief テキストメッセージ表示ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/22
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Platform/SdlFont.h"
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

// 表示状態
enum {
	STATE_TEXT = 0,	// メッセージ表示
	STATE_MENU,	// 項目選択
	STATE_EVENTPROC,	// 会話イベント処理
};

CWindowTEXTMSG::CWindowTEXTMSG()
{
	m_nSpaceHeight	= 16 * 3;

	m_nID	= WINDOWTYPE_TEXTMSG;
	m_bInput	= TRUE;
	m_ptViewPos.x	= 16;
	m_sizeWindow.cx	= 16 * 2 + 16 * 26;
	m_sizeWindow.cy	= 16 * 2 + TEXT_LINE_HEIGHT * 5 + m_nSpaceHeight;
	m_ptViewPos.y	= SCRSIZEY - 16 - m_sizeWindow.cy;

	m_bSkip	= FALSE;
	m_bInputWait	= FALSE;
	m_nState	= STATE_TEXT;
	m_nType	= 0;
	m_nProcPos	= 0;
	m_nProcPosTmp	= 0;
	m_nProcEventPage	= 0;
	m_nProcEventNo	= 0;
	m_dwLastProc	= 0;
	m_pDibTitle	= NULL;
	m_pDibText	= NULL;
	m_ptDraw.x = m_ptDraw.y = 0;

	m_pInfoTalkEvent = NULL;
}


CWindowTEXTMSG::~CWindowTEXTMSG()
{
	SAFE_DELETE(m_pDibTitle);
	SAFE_DELETE(m_pDibText);
	SAFE_DELETE(m_pInfoTalkEvent);
}


void CWindowTEXTMSG::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pSock = m_pMgrData->GetUraraSockTCP();
	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);

	m_pDibTitle = new CImg32;
	m_pDibTitle->Create(16 * 6, 16 * 2);
	m_pDibText = new CImg32;
	m_pDibText->Create(m_sizeWindow.cx - 16 * 2 + 2, m_sizeWindow.cy - 16 * 2 - m_nSpaceHeight + 2);
}


void CWindowTEXTMSG::Draw(PCImg32 pDst)
{
	int nTmp, x, y, cx, cy;
	SIZE sizeWindow;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	// 選択肢表示中は、必要なら本文枠と選択肢枠が両方入るようウィンドウを拡張する
	if (m_nState == STATE_MENU) {
		EnsureMenuSpace();
	}

	sizeWindow = m_sizeWindow;

	m_pDib->FillRect(0, 0, m_pDib->Width(), m_pDib->Height(), RGB(0, 0, 0));
	switch (m_nState) {
	case STATE_TEXT:	// メッセージ表示
	case STATE_MENU:	// 項目選択(本文枠・本文・名前枠はSTATE_TEXTと共通)
		if ((m_ptDraw.x == 0) && (m_ptDraw.y == 0)) {
			break;
		}
		DrawFrame(0, m_nSpaceHeight, sizeWindow.cx, sizeWindow.cy - m_nSpaceHeight, m_nType);

		nTmp = 0;
		if (m_strTitle.IsEmpty() == FALSE) {
			nTmp ++;
		}
		if (m_strName.IsEmpty() == FALSE) {
			nTmp ++;
		}
		if (nTmp > 0) {
			// 名前枠の幅はSTATE_MENU側と同じ+16*2に揃える(+8*2だと右8pxが枠線にかぶり、
			// 不透明化した枠線がm_pDibTitleの縁取りを上書きしてしまう)
			cx = m_pDibTitle->Width() + 16 * 2;
			cy = 16 * (2 + nTmp);
			x = sizeWindow.cx / 2 - cx / 2;
			// 名前枠の下端が本文枠の上端から16px下に来るよう、m_nSpaceHeight基準で位置決めする
			y = m_nSpaceHeight - 16 * (nTmp + 1);
			DrawFrame(x, y, cx, cy, m_nType);
			// 名前枠の下線(枠線8px)を上書きしないよう、貼り付け高さを枠内側に収める
			m_pDib->Blt(x + 16, y + 16, m_pDibTitle->Width(), min(m_pDibTitle->Height(), cy - 16 - 8), m_pDibTitle, 0, 0, TRUE);
		}

		m_pDib->Blt(16, 16 + m_nSpaceHeight, m_pDibText->Width(), m_pDibText->Height(), m_pDibText, 0, 0, TRUE);

		// 選択肢枠は本文枠・名前枠の上に重ねて最後に描く
		if (m_nState == STATE_MENU) {
			DrawMenuBox(sizeWindow);
		}
		break;
	}

	m_dwTimeDrawStart = timeGetTime();
Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


// 選択肢枠の幅・高さを計算(最長項目の実測幅+カーソル分+余白、幅は16の倍数に切り上げ)
void CWindowTEXTMSG::ComputeMenuBoxSize(int *pnWidth, int *pnHeight)
{
	int i, nCount, nWidthMax, textW, textH;

	nCount = m_astrMenu.size();

	nWidthMax = 0;
	for (i = 0; i < nCount; i ++) {
		textW = textH = 0;
		SdlFontGetTextExtent((void*)m_hFont16Normal, (LPCTSTR)m_astrMenu[i], m_astrMenu[i].GetLength(), &textW, &textH);
		if (textW > nWidthMax) {
			nWidthMax = textW;
		}
	}

	*pnWidth = nWidthMax + 24 + 16 * 2;
	*pnWidth = ((*pnWidth + 15) / 16) * 16;
	*pnHeight = nCount * 16 + 32;
}


// 選択肢枠が入るよう、必要ならm_nSpaceHeightを増やしてウィンドウを拡張する(下端位置は維持)
void CWindowTEXTMSG::EnsureMenuSpace(void)
{
	int nMenuWidth, nMenuHeight, nNeedSpaceHeight, nDelta;

	if (m_astrMenu.size() <= 0) {
		return;
	}

	ComputeMenuBoxSize(&nMenuWidth, &nMenuHeight);

	// 選択肢枠の下端は本文枠の上端から16px下。上端が0px以上に収まる余白を確保する
	nNeedSpaceHeight = nMenuHeight - 16;
	if (nNeedSpaceHeight <= m_nSpaceHeight) {
		return;
	}

	nDelta = nNeedSpaceHeight - m_nSpaceHeight;
	m_nSpaceHeight = nNeedSpaceHeight;
	m_sizeWindow.cy += nDelta;
	m_ptViewPos.y = SCRSIZEY - 16 - m_sizeWindow.cy;

	m_pDib->Destroy();
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
}


// 選択肢枠(本文枠の右上に重ねる)を描画
void CWindowTEXTMSG::DrawMenuBox(const SIZE &sizeWindow)
{
	int i, nCount, nMenuWidth, nMenuHeight, x, y;
	HDC hDC;
	COLORREF clText;

	nCount = m_astrMenu.size();
	if (nCount <= 0) {
		return;
	}

	ComputeMenuBoxSize(&nMenuWidth, &nMenuHeight);

	// 右端を本文枠の右端に、下端を本文枠の上端から16px下に揃える(名前枠と同じ重なり具合)
	x = sizeWindow.cx - nMenuWidth;
	y = m_nSpaceHeight + 16 - nMenuHeight;

	DrawFrame(x, y, nMenuWidth, nMenuHeight, m_nType);

	clText	= RGB(1, 1, 1);
	hDC	= m_pDib->Lock();
	for (i = 0; i < nCount; i ++) {
		TextOut2(hDC, m_hFont16Normal, x + 32, y + 16 + i * 16, (LPCTSTR)m_astrMenu[i], clText);
	}
	m_pDib->Unlock();

	DrawCursor(x + 8, y + 16 + 16 * m_nPos);
}


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
	pPlayerChar = m_pMgrData->GetPlayerChar();

	if (m_pDibText == NULL) {
		goto Exit;
	}
	pMgrKeyInput = m_pMgrData->GetMgrKeyInput();

	dwTimeTmp	= timeGetTime() - m_dwLastProc;
	bResult	= pMgrKeyInput->IsInput('X');
	if (bResult == FALSE) {
		if (dwTimeTmp < 100) {
			goto Exit;
		}
	}
	m_dwLastProc = timeGetTime();

	switch (m_nState) {
	case STATE_TEXT:	// メッセージ表示
		ZeroMemory(szTmp, sizeof(szTmp));
		pnProcPos = &m_nProcPos;
		pszTmp = (LPCSTR)m_strMsg;
		if (m_strMsgTmp.IsEmpty() == FALSE) {
			pszTmp = (LPCSTR)m_strMsgTmp;
			pnProcPos = &m_nProcPosTmp;
		}

		if ((pszTmp[*pnProcPos] == 0) || (*pnProcPos >= (int)strlen(pszTmp))) {
			if (m_strMsgTmp.IsEmpty() == FALSE) {
				m_strMsgTmp.Empty();
				m_nProcPosTmp = 0;
				goto Exit;
			}
			m_bInputWait = TRUE;
			goto Exit;
		}

                if (strncmp(&pszTmp[*pnProcPos], "\r\n", 2) == 0) {
                        CopyMemory(szTmp, "\r\n", 2);
                        szTmp[2] = '\0';

                } else {
                        CopyNextMessageChar(szTmp, _countof(szTmp), &pszTmp[*pnProcPos]);
                }

		if (szTmp[0] == '@') {
			MsgProc();

		} else {
			*pnProcPos += strlen(szTmp);

			DrawChar(szTmp);
		}
		break;
	case STATE_MENU:	// 項目選択
		break;
	case STATE_EVENTPROC:	// 会話イベント処理
		{
			PCLibInfoItem pLibInfoItem = m_pMgrData->GetLibInfoItem();
			PCInfoTalkEventBase pInfoTalkEvent = m_pInfoTalkEvent->GetPtr(m_nProcEventPage, m_nProcEventNo);
			if (pInfoTalkEvent == NULL) {
				m_bDelete = TRUE;
				break;
			}
			switch (pInfoTalkEvent->m_nEventType) {
			case TALKEVENTTYPE_PAGE:	// ページ切り替え
				{
					int i, nCount;
					BOOL bJump;
					PCInfoTalkEventPAGE pInfoTmp = (PCInfoTalkEventPAGE)pInfoTalkEvent;
					PARRAYDWORD padwItemID;

					bJump = FALSE;
					switch (pInfoTmp->m_nPageChgCondition) {
					case CHGPAGECONDITION_ITEM:	// アイテムあり
					case CHGPAGECONDITION_NOITEM:	// アイテムなし
						padwItemID = pPlayerChar->GetItem();
						nCount = padwItemID->size();
						for (i = 0; i < nCount; i ++) {
							if (pInfoTmp->m_dwData == pLibInfoItem->GetItemTypeID(padwItemID->at(i))) {
								break;
							}
						}
						switch (pInfoTmp->m_nPageChgCondition) {
						case CHGPAGECONDITION_ITEM:	// アイテムあり
							if (i < nCount) {
								bJump = TRUE;
							}
							break;
						case CHGPAGECONDITION_NOITEM:	// アイテムなし
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
			case TALKEVENTTYPE_MSG:	// メッセージ表示
				SetMsg((LPCSTR)pInfoTalkEvent->m_strText);
				m_nState = STATE_TEXT;
				break;
			case TALKEVENTTYPE_MENU:	// 項目選択
				{
					int i, nCount;
					PSTTALKEVENTMENUINFO pMenuInfo;	
					PCInfoTalkEventMENU pInfoTmp = (PCInfoTalkEventMENU)pInfoTalkEvent;

					m_astrMenu.clear();
					nCount = pInfoTmp->GetMenuInfoCount();
					for (i = 0; i < nCount; i ++) {
						pMenuInfo = pInfoTmp->GetPtr(i);
						m_astrMenu.push_back(pMenuInfo->strName);
					}
					m_nState	= STATE_MENU;
					m_nPos	= 0;
					m_nPosMax	= nCount - 1;
				}
				break;
			case TALKEVENTTYPE_ADDSKILL:	// スキル追加
				{
					CPacketCHAR_PARA1 PacketPara1;

					PacketPara1.Make(SBOCOMMANDID_SUB_CHAR_REQ_ADDSKILL, pPlayerChar->m_dwCharID, pInfoTalkEvent->m_dwData);
					m_pSock->Send(&PacketPara1);
					m_nProcEventNo ++;
				}
				break;
			}
		}
		break;
	}

	Redraw();

	bRet = TRUE;
Exit:
	return bRet;
}


void CWindowTEXTMSG::SetTitle(LPCSTR pszTitle)
{
	m_strTitle = pszTitle;
	RenewTitle();
	Redraw();
}


void CWindowTEXTMSG::SetName(LPCSTR pszName)
{
	int nWidthTitle, nWidthName, nWidth, nHeightTmp;

	m_strName = pszName;

	nWidthTitle = 0;
	nHeightTmp = 0;
	if (m_strTitle.GetLength() > 0) {
		SdlFontGetTextExtent((void*)m_hFont16, (LPCTSTR)m_strTitle, m_strTitle.GetLength(), &nWidthTitle, &nHeightTmp);
	}
	nWidthName = 0;
	if (m_strName.GetLength() > 0) {
		SdlFontGetTextExtent((void*)m_hFont16, (LPCTSTR)m_strName, m_strName.GetLength(), &nWidthName, &nHeightTmp);
	}
	nWidth = max(nWidthTitle, nWidthName) + 8;
	nWidth = max(nWidth, 16 * 6);

	m_pDibTitle->Destroy();
	m_pDibTitle->Create(nWidth, 16 * 2);
	RenewTitle();
	Redraw();
}


void CWindowTEXTMSG::SetMsg(LPCSTR pszMsg)
{
	m_ptDraw.x = m_ptDraw.y = 0;
	m_nProcPos	= 0;
	m_dwLastProc	= timeGetTime();
	m_strMsg	= pszMsg;

	// 背景を黒(RGB(0,0,0))で塗るとカラーキー0透過でSDL_ttfの縁取り・アンチエイリアスの
	// 暗いピクセルまで透明になってしまうため、枠の背景色で塗る
	m_pDibText->FillRect(0, 0, m_pDibText->Width(), m_pDibText->Height(), GetFrameBackColor(m_nType));
}


void CWindowTEXTMSG::SetTalkEvent(CInfoTalkEvent *pInfo)
{
	SAFE_DELETE(m_pInfoTalkEvent);
	m_pInfoTalkEvent = new CInfoTalkEvent;
	m_pInfoTalkEvent->Copy(pInfo);

	m_nProcEventPage	= 0;
	m_nProcEventNo	= 0;
	m_nState	= STATE_EVENTPROC;
}


BOOL CWindowTEXTMSG::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nState) {
	case STATE_MENU:	// 項目選択
		if (m_nPos <= 0) {
			goto Exit;
		}
		m_nPos --;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowTEXTMSG::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nState) {
	case STATE_MENU:	// 項目選択
		if (m_nPos >= m_nPosMax) {
			goto Exit;
		}
		m_nPos ++;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowTEXTMSG::OnLeft(void)
{
	switch (m_nState) {
	case STATE_MENU:	// 項目選択
		m_nPos = 0;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
		break;
	}

	return TRUE;
}


BOOL CWindowTEXTMSG::OnRight(void)
{
	switch (m_nState) {
	case STATE_MENU:	// 項目選択
		m_nPos = m_nPosMax;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
		break;
	}

	return TRUE;
}


BOOL CWindowTEXTMSG::OnX(BOOL bDown)
{
	if (bDown == FALSE) {
		switch (m_nState) {
		case STATE_TEXT:	// メッセージ表示
			if (m_bSkip) {
				m_bSkip = FALSE;
				break;
			}
			if (m_bInputWait) {
				m_bInputWait = FALSE;
				m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
				if (m_pInfoTalkEvent) {
					m_nProcEventNo ++;
					m_nState = STATE_EVENTPROC;
					break;
				}
				m_bDelete = TRUE;
			}
			break;
		case STATE_MENU:	// 項目選択
			m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
			m_nState = STATE_TEXT;
			m_bSkip = FALSE;	// 選択確定時のX押下がSTATE_TEXT移行後のスキップに誤消費されないようにする
			if (m_pInfoTalkEvent) {
				PSTTALKEVENTMENUINFO pMenuInfo;
				PCInfoTalkEventMENU pInfoTmp = (PCInfoTalkEventMENU)m_pInfoTalkEvent->GetPtr(m_nProcEventPage, m_nProcEventNo);
				pMenuInfo = pInfoTmp->GetPtr(m_nPos);
				m_nProcEventPage = pMenuInfo->nPage;
				m_nProcEventNo = 0;
				m_nState = STATE_EVENTPROC;
			}
			break;
		}
	} else {
		// STATE_MENUでのX押下でスキップを立てると、直後の文章表示完了時の
		// 最初のXリリースがスキップ解除に消費されウィンドウが閉じない(2回押し)ため、
		// スキップ対象はSTATE_TEXTのときのみとする
		if (m_nState == STATE_TEXT && m_bInputWait == FALSE) {
			m_bSkip = TRUE;
		}
	}

	return FALSE;
}


BOOL CWindowTEXTMSG::OnZ(BOOL bDown)
{
	if (bDown == FALSE) {
		switch (m_nState) {
		case STATE_TEXT:	// メッセージ表示
			if (m_bInputWait) {
				m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
				m_bDelete = TRUE;
			}
			break;
		case STATE_MENU:	// 項目選択
			m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
			m_bDelete = TRUE;
			break;
		}
	}

	return FALSE;
}


void CWindowTEXTMSG::DrawChar(LPCSTR pszText)
{
	int cx, cy;
	HDC hDC;
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

	cx = m_pDibText->Width();
	cy = m_pDibText->Height() - TEXT_LINE_HEIGHT;

	clText	= RGB(1, 1, 1);
	hDC	= m_pDibText->Lock();

	clText = RGB(1, 1, 1);

	// "\r\n"は改行制御なので描画・幅計測はせず改行処理だけ行う
	// (SDL_ttfが制御文字を豆腐等で描いてしまう可能性があるため)
	if (strncmp(pszText, "\r\n", 2) == 0) {
		m_ptDraw.x = cx;
	} else {
		TextOut2(hDC, m_hFont16Normal, m_ptDraw.x, m_ptDraw.y, pszDraw, clText);

		// SdlFontGetTextExtent でフォント直接計測
		int textW = 0, textH = 0;
		SdlFontGetTextExtent((void*)m_hFont16Normal, pszDraw, (int)_tcslen(pszDraw), &textW, &textH);
		m_ptDraw.x += textW;
	}
	if (m_ptDraw.x + 8 >= cx) {
		m_ptDraw.x = 0;
		m_ptDraw.y += TEXT_LINE_HEIGHT;
		if (m_ptDraw.y >= cy) {
			m_ptDraw.y -= TEXT_LINE_HEIGHT;
			m_pDibText->Blt(0, 0, cx, cy, m_pDibText, 0, TEXT_LINE_HEIGHT);
			// 黒背景だとカラーキー透過で縁取り・ぼかしが消えるため枠の背景色で塗る
			m_pDibText->FillRect(0, cy, cx, TEXT_LINE_HEIGHT, GetFrameBackColor(m_nType));
		}
	}

	m_pDibText->Unlock();
}


void CWindowTEXTMSG::RenewTitle(void)
{
	int x, y, textW, textH;
	HDC hDC;
	COLORREF clText, clFrame;

	// 黒背景だとカラーキー透過で名前の縁取りが消えるため枠の背景色で塗る
	m_pDibTitle->FillRect(0, 0, m_pDibTitle->Width(), m_pDibTitle->Height(), GetFrameBackColor(m_nType));

	hDC	= m_pDibTitle->Lock();

	y = 1;
	clText  = RGB(255, 255, 255);
	clFrame = RGB(1, 1, 1);
	if (m_strTitle.GetLength() > 0) {
		textW = textH = 0;
		SdlFontGetTextExtent((void*)m_hFont16, (LPCTSTR)m_strTitle, m_strTitle.GetLength(), &textW, &textH);
		x = (m_pDibTitle->Width() - textW) / 2;
		if (x < 1) {
			x = 1;
		}
		TextOut2(hDC, m_hFont16, x, 1, (LPCTSTR)m_strTitle, clText, TRUE, clFrame);
		y += 16;
	}
	if (m_strName.GetLength() > 0) {
		textW = textH = 0;
		SdlFontGetTextExtent((void*)m_hFont16, (LPCTSTR)m_strName, m_strName.GetLength(), &textW, &textH);
		x = (m_pDibTitle->Width() - textW) / 2;
		if (x < 1) {
			x = 1;
		}
		TextOut2(hDC, m_hFont16, x, y, (LPCTSTR)m_strName, clText, TRUE, clFrame);
	}

	m_pDibTitle->Unlock();
}


void CWindowTEXTMSG::InitText(void)
{
	m_ptDraw.x = m_ptDraw.y = 0;
	// 黒背景だとカラーキー透過で縁取り・ぼかしが消えるため枠の背景色で塗る
	m_pDibText->FillRect(0, 0, m_pDibText->Width(), m_pDibText->Height(), GetFrameBackColor(m_nType));
}


void CWindowTEXTMSG::MsgProc(void)
{
	BOOL bSkip;
	int nPos, nLen;
	char szTmp[2];
	LPCSTR pszSrc, pszTmp;
	CmyString strTmp, strTmp2;

	pszSrc	= (LPCSTR)m_strMsg;
	pszSrc	= &pszSrc[m_nProcPos];
	nLen	= strlen(pszSrc);

	GetLineText(pszSrc, strTmp);
	pszTmp = (LPCSTR)strTmp;

	m_astrMenu.clear();

	nPos = 0;
	// メニュー？
	if (_strnicmp(&pszTmp[nPos], "@menu", 5) == 0) {
		while (1) {
			nPos += ((int)strlen((LPCSTR)strTmp) + 2);	// GetLength()はワイド文字数なのでバイトオフセットにはstrlenを使う
			if (nPos >= nLen) {
				break;
			}

			GetLineText(&pszSrc[nPos], strTmp);
			pszTmp = (LPCSTR)strTmp;
			if (pszTmp[0] == '{') {
				continue;
			}
			if (pszTmp[0] == '}') {
				nPos += ((int)strlen((LPCSTR)strTmp) + 2);
				break;
			}
			strTmp2 = strTmp;
			TrimSpace(strTmp2);
			m_astrMenu.push_back(strTmp2);
		}
		m_nProcPos += nPos;
		m_nPos	= 0;
		m_nPosMax	= m_astrMenu.size();
		if (m_nPosMax > 0) {
			m_nPosMax --;
			m_nState = STATE_MENU;
		}

	// 項目選択？
	} else if (_strnicmp(&pszTmp[nPos], "@select", 7) == 0) {
		szTmp[0] = pszTmp[nPos + 8];
		szTmp[1] = 0;
		bSkip = FALSE;
		if (m_nPos != atoi(szTmp)) {
			bSkip = TRUE;
		}
		m_nProcPosTmp = 0;
		m_strMsgTmp.Empty();
		while (1) {
			nPos += ((int)strlen((LPCSTR)strTmp) + 2);	// GetLength()はワイド文字数なのでバイトオフセットにはstrlenを使う
			if (nPos >= nLen) {
				break;
			}

			GetLineText(&pszSrc[nPos], strTmp);
			pszTmp = (LPCSTR)strTmp;
			if (pszTmp[0] == '{') {
				continue;
			}
			if (pszTmp[0] == '}') {
				nPos += ((int)strlen((LPCSTR)strTmp) + 2);
				break;
			}
			if (bSkip == FALSE) {
				strTmp2 = strTmp;
				TrimSpace(strTmp2);
                        m_strMsgTmp += (LPCTSTR)strTmp2;
			}
		}
		m_nProcPos += nPos;
	}
}


void CWindowTEXTMSG::GetLineText(LPCSTR pszSrc, CmyString &strDst)
{
	int i, nLen;
	char szTmp[8];

	strDst.Empty();

	nLen = strlen(pszSrc);
        for (i = 0; i < nLen - 1;) {
                ZeroMemory(szTmp, sizeof(szTmp));
                CopyNextMessageChar(szTmp, _countof(szTmp), &pszSrc[i]);
                if (szTmp[0] == 0) {
                        break;
                }

                if (strncmp(&pszSrc[i], "\r\n", 2) == 0) {
                        break;
                }
                strDst += szTmp;
                i += static_cast<int>(strlen(szTmp));
        }
}


void CWindowTEXTMSG::TrimSpace(CmyString &strSrc)
{
	int i, nLen;
	char szTmp[8];
	LPCSTR pszSrc;
	CmyString strTmp;

	strTmp = strSrc;
	pszSrc = (LPCSTR)strTmp;

        nLen = (int)strlen(pszSrc);	// GetLength()はワイド文字数なのでバイトループ上限にはstrlenを使う
        for (i = 0; i < nLen;) {
                ZeroMemory(szTmp, sizeof(szTmp));
                CopyNextMessageChar(szTmp, _countof(szTmp), &pszSrc[i]);
                if (szTmp[0] == 0) {
                        break;
                }

                if ((strcmp(szTmp, " ") == 0) || (strcmp(szTmp, "　") == 0)) {
                        i += static_cast<int>(strlen(szTmp));
                        continue;
                }
                strSrc = &pszSrc[i];
                break;
        }
}


void CWindowTEXTMSG::GetBlock(LPCSTR pszSrc, CmyString &strDst)
{
	strDst.Empty();
}
