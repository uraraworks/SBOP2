/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowSKILLMENU.cpp										 */
/* 内容			:スキルメニューウィンドウクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/31													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoSkill.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSKILLMENU.h"

#define MENUPOSY	(51)	/* メニュー本体表示位置(縦) */

/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::CWindowSKILLMENU								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

CWindowSKILLMENU::CWindowSKILLMENU()
{
	m_nPos			= 12;
	m_nPosMax		= 24;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_SKILLMENU;
	m_ptViewPos.x	= 24;
	m_ptViewPos.y	= 112;
	m_sizeWindow.cx	= 192;
	m_sizeWindow.cy	= 244;

	m_nType			= 0;
	m_dwSelectID	= 0;
	m_pPlayerChar	= NULL;
	m_pLibInfoSkill	= NULL;
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::~CWindowSKILLMENU							 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

CWindowSKILLMENU::~CWindowSKILLMENU()
{
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::Create										 */
/* 内容		:作成															 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

void CWindowSKILLMENU::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_pPlayerChar	= m_pMgrData->GetPlayerChar ();
	m_pLibInfoSkill	= m_pMgrData->GetLibInfoSkill ();
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::Draw											 */
/* 内容		:描画															 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

void CWindowSKILLMENU::Draw(PCImg32 pDst)
{
	BOOL bActive;
	int i, nCount, nLevel, x, y;
	HDC hDC;
	HFONT hFontOld;
	PARRAYDWORD paSkillID;
	PCInfoSkillBase pInfoSkill;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_strName.Empty ();
	for (i = 0; i < 3; i ++) {
		bActive = (i == m_nType) ? TRUE : FALSE;
		m_pMgrDraw->DrawSkillType (m_pDib, 52 * i, 0, i, bActive);
	}
	m_pMgrDraw->DrawSkillMenu (m_pDib, 0, MENUPOSY);

	/* スキルを描画 */
	paSkillID = m_pPlayerChar->GetSkill ();
	nCount = paSkillID->GetSize ();
	for (i = 0; i < nCount; i ++) {
//Todo:とりあえず生活スキルだけ
		if (m_nType != 1) {
			continue;
		}
		pInfoSkill = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (paSkillID->GetAt (i));
		if (pInfoSkill == NULL) {
			continue;
		}
		GetDrawPos (i, x, y);
//Todo:とりあえず釣りアイコンだけ
		m_pMgrDraw->DrawIcon (m_pDib, x, y, 2);
		if (i == m_nPos) {
			m_strName = (LPCSTR)pInfoSkill->m_strName;
		}
		break;
	}

	GetDrawPos (m_nPos, x, y);
	m_pMgrDraw->DrawCursor (m_pDib, x - 8, y, 2);
	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);

	/* スキル名を表示 */
	if (m_strName.IsEmpty () == FALSE) {
		hDC			= pDst->Lock ();
		hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
		SetBkMode (hDC, TRANSPARENT);

		GetDrawPos (m_nPos, x, y);
		x = m_ptViewPos.x + 32 + x - 8;
		y = m_ptViewPos.y + 32 + y - 24;
		DrawFrame2 (x, y, m_strName.GetLength () * 6, 16, 0, pDst, 4);
		TextOut2 (hDC, x, y, (LPCSTR)m_strName, RGB (10, 10, 10), FALSE);

		SelectObject (hDC, hFontOld);
		pDst->Unlock ();
	}
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::OnUp											 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos < 5) {
		goto Exit;
	}
	m_nPos -= 5;

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::OnDown										 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax - 4) {
		goto Exit;
	}
	m_nPos += 5;

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::OnLeft										 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnLeft(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos % 5 == 0) {
		if (m_nType == 0) {
			goto Exit;
		}
		m_nType --;
		m_nPos += 5;
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
/* 関数名	:CWindowSKILLMENU::OnRight										 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnRight(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos % 5 == 4) {
		if (m_nType >= 2) {
			goto Exit;
		}
		m_nType ++;
		m_nPos -= 5;
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
/* 関数名	:CWindowSKILLMENU::OnS											 */
/* 内容		:キーハンドラ(S)												 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnS(BOOL bDown)
{
	return OnZ (bDown);
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnX(BOOL bDown)
{
//	int i, nCount;
	POINT ptPos;
	BOOL bRet;
	DWORD dwSkillID;
	PARRAYDWORD paSkillID;
//	PCInfoSkillBase pInfoSkill;

	bRet		 = FALSE;
	dwSkillID	 = 0;

	ptPos.x = m_nPos % 5;
	ptPos.y = m_nPos / 5;

	if (bDown) {
		goto Exit;
	}

//Todo:選択中のID取得
	if (m_nType == 1) {
		if (m_nPos == 0) {
			paSkillID = m_pPlayerChar->GetSkill ();
			if (paSkillID->GetSize () >= 0) {
				dwSkillID = paSkillID->GetAt (0);
			}
		}
	}
	if (dwSkillID == 0) {
		goto Exit;
	}

	m_dwSelectID = dwSkillID;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, dwSkillID);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::OnZ											 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnZ(BOOL bDown)
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
/* 関数名	:CWindowSKILLMENU::GetDrawPos									 */
/* 内容		:スキル描画位置を取得											 */
/* 日付		:2008/11/19														 */
/* ========================================================================= */

void CWindowSKILLMENU::GetDrawPos(
	int nPos,		/* [in] スキル位置 */
	int &nDstX,		/* [out] X座標 */
	int &nDstY)		/* [out] Y座標 */
{
	nDstX = 33 * (nPos % 5) + 10;
	nDstY = 33 * (nPos / 5) + 10 + MENUPOSY;
}

/* Copyright(C)URARA-works 2008 */
