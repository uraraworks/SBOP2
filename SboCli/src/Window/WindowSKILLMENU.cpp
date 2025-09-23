﻿/* Copyright(C)URARA-works 2008 */
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
	m_sizeWindow.cx	= 208;
	m_sizeWindow.cy	= 280;

	m_nType			= 0;
	m_nMode			= 1;
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
	m_nPos			= m_pMgrData->GetWindowPosSKILLMENUPos ();
	m_nType			= m_pMgrData->GetWindowPosSKILLMENUType ();
	SetType (m_nType);
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::Draw											 */
/* 内容		:描画															 */
/* 日付		:2008/12/31														 */
/* ========================================================================= */

void CWindowSKILLMENU::Draw(PCImg32 pDst)
{
	LPCSTR aszTitle[] = {"戦闘(F)", "生活(L)", "能力"};
	int i, nCount, nLevel, x, y, nTmp;
	HDC hDC;
	HFONT hFontOld;
	PCInfoSkillBase pInfoSkill;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_strName.Empty ();
	DrawFrame (5);

	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
	SetBkMode (hDC, TRANSPARENT);
	for (i = 0; i < 3; i ++) {
		int nType = (m_nType == i) ? 7 : 5;
		DrawFrame (7 + 49 * i, 7, 48, 24, nType);
		TextOut2 (hDC, 7 + 5 + 49 * i, 7 + 4, aszTitle[i],
			(m_nType == i) ? RGB (255, 255, 255) : RGB (196, 140, 81));
	}
	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	DrawFrame (4, 23, 200, 248, 6);

	for (y = 0; y < 5; y ++) {
		for (x = 0; x < 5; x ++) {
			DrawIconFrame (12 + 36 * x, 32 + 47 * y);
		}
	}

	nTmp = 0;
	/* スキルを描画 */
	nCount = m_adwSkillID.size();
	for (i = 0; i < nCount; i ++) {
		pInfoSkill = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (m_adwSkillID[i]);
		if (pInfoSkill == NULL) {
			continue;
		}
		GetDrawPos (nTmp, x, y);
		m_pMgrDraw->DrawIcon (m_pDib, x, y, pInfoSkill->m_dwIconID);
		if (nTmp == m_nPos) {
			m_strName = (LPCSTR)pInfoSkill->m_strName;
		}
		nTmp ++;
	}

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
	switch (m_nMode) {
	case 0:
		m_pMgrDraw->DrawCursor (pDst, m_ptViewPos.x + 32 + 49 * m_nType - 16, m_ptViewPos.y + 32 + 4, 0);
		break;
	case 1:
		GetDrawPos (m_nPos, x, y);
		m_pMgrDraw->DrawCursor (pDst, m_ptViewPos.x + 32 + x - 8, m_ptViewPos.y + 32 + y, 2);
		break;
	}

	/* スキル名を表示 */
	if ((m_nMode == 1) && (m_strName.IsEmpty () == FALSE)) {
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
/* 関数名	:CWindowSKILLMENU::SetType										 */
/* 内容		:スキル種別を設定												 */
/* 日付		:2009/01/11														 */
/* ========================================================================= */

void CWindowSKILLMENU::SetType(int nType)
{
	int i, nCount;
	DWORD dwSkillID;
	PARRAYDWORD paSkillID;
	PCInfoSkillBase pInfoSkill;
	int anSkillType[] = {SKILLTYPEMAIN_BATTLE, SKILLTYPEMAIN_LIFE, SKILLTYPEMAIN_NONE};

	m_nType = nType;
	m_adwSkillID.clear();

	/* スキルを描画 */
	paSkillID = m_pPlayerChar->GetSkill ();
	nCount = paSkillID->size();
	for (i = 0; i < nCount; i ++) {
		dwSkillID = paSkillID->at(i);
		pInfoSkill = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (dwSkillID);
		if (pInfoSkill == NULL) {
			continue;
		}
		if (anSkillType[m_nType] != pInfoSkill->m_nTypeMain) {
			continue;
		}
		m_adwSkillID.push_back (dwSkillID);
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

	switch (m_nMode) {
	case 0:
		goto Exit;
	case 1:
		if (m_nPos < 5) {
			m_nMode = 0;
			break;
		}
		m_nPos -= 5;
		break;
	}

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

	switch (m_nMode) {
	case 0:
		m_nMode = 1;
		m_nPos = 0;
		break;
	case 1:
		if (m_nPos >= m_nPosMax - 4) {
			goto Exit;
		}
		m_nPos += 5;
		break;
	}

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

	switch (m_nMode) {
	case 0:
		if (m_nType == 0) {
			goto Exit;
		}
		SetType (m_nType -1);
		break;
	case 1:
		if (m_nPos % 5 == 0) {
			if (m_nType == 0) {
				goto Exit;
			}
			SetType (m_nType -1);
			m_nPos += 5;
		}
		m_nPos --;
		break;
	}
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

	switch (m_nMode) {
	case 0:
		if (m_nType >= 2) {
			goto Exit;
		}
		SetType (m_nType + 1);
		break;
	case 1:
		if (m_nPos % 5 == 4) {
			if (m_nType >= 2) {
				goto Exit;
			}
			SetType (m_nType + 1);
			m_nPos -= 5;
		}
		m_nPos ++;
		break;
	}
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::OnF											 */
/* 内容		:キーハンドラ(F)												 */
/* 日付		:2009/04/18														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnF(BOOL bDown)
{
	if (bDown) {
		return FALSE;
	}

	SetType (0);
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowSKILLMENU::OnL											 */
/* 内容		:キーハンドラ(L)												 */
/* 日付		:2009/04/18														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnL(BOOL bDown)
{
	if (bDown) {
		return FALSE;
	}

	SetType (1);
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
	return TRUE;
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
	POINT ptPos;
	BOOL bRet;
	DWORD dwSkillID;

	bRet		 = FALSE;
	dwSkillID	 = 0;

	ptPos.x = m_nPos % 5;
	ptPos.y = m_nPos / 5;

	if (bDown) {
		goto Exit;
	}

	if (m_nPos < m_adwSkillID.size()) {
		dwSkillID = m_adwSkillID[m_nPos];
	}
	if (dwSkillID == 0) {
		goto Exit;
	}

	m_dwSelectID = dwSkillID;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, dwSkillID);

	m_pMgrData->SetWindowPosSKILLMENUPos (m_nPos);
	m_pMgrData->SetWindowPosSKILLMENUType (m_nType);

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

	m_pMgrData->SetWindowPosSKILLMENUPos (m_nPos);
	m_pMgrData->SetWindowPosSKILLMENUType (m_nType);

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
	nDstX = 13 + 36 * (nPos % 5);
	nDstY = 33 + 47 * (nPos / 5);
}

/* Copyright(C)URARA-works 2008 */
