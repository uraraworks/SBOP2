/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowITEMMENU.cpp											 */
/* 内容			:アイテムメニューウィンドウクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/30													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoItem.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowITEMMENU.h"


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::CWindowITEMMENU								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

CWindowITEMMENU::CWindowITEMMENU()
{
	m_ptDrag.x = m_ptDrag.y = -1;
	m_nPos			= EQUIPTYPE_MAX + 12;
	m_nPosMax		= EQUIPTYPE_MAX + 24;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_ITEMMENU;
	m_ptViewPos.x	= 24;
	m_ptViewPos.y	= 32;
	m_sizeWindow.cx	= 8 * 27;
	m_sizeWindow.cy	= 8 * 50;

	m_dwSelectItemID = 0;
	m_dwDragItemID	 = 0;

	m_pPlayerChar	= NULL;
	m_pLibInfoItem	= NULL;
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::~CWindowITEMMENU								 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

CWindowITEMMENU::~CWindowITEMMENU()
{
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

void CWindowITEMMENU::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_pPlayerChar	= m_pMgrData->GetPlayerChar ();
	m_pLibInfoItem	= m_pMgrData->GetLibInfoItem ();
	m_nPos			= m_pMgrData->GetWindowPosITEMMENUPos ();
	if (m_nPos == -1) {
		m_nPos = EQUIPTYPE_MAX + 12;
	}
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::Draw											 */
/* 内容		:描画															 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

void CWindowITEMMENU::Draw(PCImg32 pDst)
{
	int i, nCount, nLevel, x, y, nCursor;
	HDC hDC;
	HFONT hFontOld;
	PCInfoItem pInfoItem, pIntoItemDrag;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_strName.Empty ();
	DrawFrame (5);
	DrawFrame (11, 7, 32 + 4, 24, 7);
	DrawFrame (4, 24, 120, 104, 6);
	DrawFrame (146, 24, 48, 104, 6);
	DrawFrame (11, 134, 104, 24, 7);
	DrawFrame (4, 149, 208, 248, 6);

	for (y = 0; y < 2; y ++) {
		for (x = 0; x < 3; x ++) {
			DrawIconFrame (12 + 36 * x, 31 + 47 * y);
		}
	}
	for (y = 0; y < 2; y ++) {
		DrawIconFrame (153, 31 + 47 * y);
	}
	for (y = 0; y < 5; y ++) {
		for (x = 0; x < 5; x ++) {
			DrawIconFrame (12 + 36 * x, 158 + 47 * y);
		}
	}

	/* 装備アイテムを描画 */
	DrawEquip (0, m_pPlayerChar->m_dwEquipItemIDCloth);		/* 服 */
	DrawEquip (1, m_pPlayerChar->m_dwEquipItemIDAcce1);		/* アクセサリ1 */
	DrawEquip (2, m_pPlayerChar->m_dwEquipItemIDArmsRight);	/* 持ち物 */
	DrawEquip (3, m_pPlayerChar->m_dwEquipItemIDArmsLeft);	/* 盾 */

	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
	SetBkMode (hDC, TRANSPARENT);
	TextOut2 (hDC, 12 + 5, 7 + 4, "装備", RGB (255, 255, 255));
	TextOut2 (hDC, 12 + 5, 134 + 3, "バッグ(B)", RGB (255, 255, 255));

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	pIntoItemDrag = NULL;

	/* アイテムを描画 */
	nCount = m_pPlayerChar->m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pPlayerChar->m_adwItemID[i]);
		if (pInfoItem == NULL) {
			continue;
		}
		x = pInfoItem->m_ptBackPack.x;
		y = pInfoItem->m_ptBackPack.y;
		if ((m_ptDrag.x == x) && (m_ptDrag.y == y)) {
			pIntoItemDrag = pInfoItem;
		}
		m_pMgrDraw->DrawItem (m_pDib, 12 + (x * 36), 159 + (y * 47), pInfoItem);
		if (m_strName.IsEmpty () == FALSE) {
			continue;
		}
		if (((m_nPos - EQUIPTYPE_MAX) % 5 == x) &&
			((m_nPos - EQUIPTYPE_MAX) / 5 == y)) {
			m_strName = pInfoItem->m_strName;
		}
	}

	/* ドラッグ中？ */
	if (pIntoItemDrag) {
		x = (m_nPos - EQUIPTYPE_MAX) % 5;
		y = (m_nPos - EQUIPTYPE_MAX) / 5;
		m_pMgrDraw->DrawItem (m_pDib, 12 + (x * 36), 159 + (y * 47), pIntoItemDrag, 50);
	}

	GetDrawPos (m_nPos, x, y);

	nCursor = (m_ptDrag.x == -1) ? 2 : 3;
	m_pMgrDraw->DrawCursor (m_pDib, x - 8, y, nCursor);
	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);

	/* アイテム名を表示 */
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
/* 関数名	:CWindowITEMMENU::DragOff										 */
/* 内容		:ドラッグ解除													 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CWindowITEMMENU::DragOff(void)
{
	m_ptDrag.x = m_ptDrag.y = -1;
	m_dwDragItemID = 0;
	Redraw ();
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::OnUp											 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos < EQUIPTYPE_MAX) {
		if (m_nPos < 3) {
			goto Exit;
		}
		m_nPos -= 3;
	} else {
		if (m_nPos < 10) {
			switch (m_nPos) {
			case 6:	m_nPos = 3;	break;
			case 7:	m_nPos = 4;	break;
			case 8:	m_nPos = 4;	break;
			case 9:	m_nPos = 5;	break;
			}
		} else {
			m_nPos -= 5;
		}
	}
	if (m_nPos < EQUIPTYPE_MAX) {
		/* ドラッグ解除 */
		DragOff ();
	}

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::OnDown										 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos < EQUIPTYPE_MAX) {
		if (m_nPos < 3) {
			m_nPos += 3;
		} else {
			switch (m_nPos) {
			case 3:	m_nPos = 6;	break;
			case 4:	m_nPos = 7;	break;
			case 5:	m_nPos = 8;	break;
			}
		}
	} else {
		if (m_nPos >= m_nPosMax - 4) {
			goto Exit;
		}
		m_nPos += 5;
	}

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::OnLeft										 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnLeft(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos < EQUIPTYPE_MAX) {
		if (m_nPos % 3 == 0) {
			goto Exit;
		}
	} else {
		if ((m_nPos - EQUIPTYPE_MAX) % 5 == 0) {
			goto Exit;
		}
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
/* 関数名	:CWindowITEMMENU::OnRight										 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnRight(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos < EQUIPTYPE_MAX) {
		if (m_nPos % 3 == 2) {
			goto Exit;
		}
	} else {
		if ((m_nPos - EQUIPTYPE_MAX) % 5 == 4) {
			goto Exit;
		}
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
/* 関数名	:CWindowITEMMENU::OnB											 */
/* 内容		:キーハンドラ(B)												 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnB(BOOL bDown)
{
	return OnZ (bDown);
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnX(BOOL bDown)
{
	int i, nCount;
	POINT ptPos;
	BOOL bRet;
	DWORD dwItemID, dwItemIDDrag;
	PCInfoItem pInfoItem;

	bRet		 = FALSE;
	dwItemID	 = 0;
	dwItemIDDrag = 0;

	ptPos.x = (m_nPos - EQUIPTYPE_MAX) % 5;
	ptPos.y = (m_nPos - EQUIPTYPE_MAX) / 5;

	if (bDown) {
		if (m_ptDrag.x == -1) {
			m_ptDrag = ptPos;
			Redraw ();
		}
		goto Exit;
	}
	if ((m_ptDrag.x == ptPos.x) && (m_ptDrag.y == ptPos.y)) {
		DragOff ();
	}

	if (m_nPos < EQUIPTYPE_MAX) {
		switch (m_nPos) {
		case EQUIPTYPE_ACCE1:		dwItemID = m_pPlayerChar->m_dwEquipItemIDAcce1;		break;	/* アクセサリ1 */
		case EQUIPTYPE_HEAD:		dwItemID = m_pPlayerChar->m_dwEquipItemIDHead;		break;	/* 頭 */
		case EQUIPTYPE_ACCE2:		dwItemID = m_pPlayerChar->m_dwEquipItemIDAcce2;		break;	/* アクセサリ2 */
		case EQUIPTYPE_ARMSRIGHT:	dwItemID = m_pPlayerChar->m_dwEquipItemIDArmsRight;	break;	/* 右手 */
		case EQUIPTYPE_CLOTH:		dwItemID = m_pPlayerChar->m_dwEquipItemIDCloth;		break;	/* 服 */
		case EQUIPTYPE_ARMSLEFT:	dwItemID = m_pPlayerChar->m_dwEquipItemIDArmsLeft;	break;	/* 左手 */
		}
	} else {
		nCount = m_pPlayerChar->m_adwItemID.GetSize ();
		for (i = 0; i < nCount; i ++) {
			pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pPlayerChar->m_adwItemID[i]);
			if (pInfoItem == NULL) {
				continue;
			}
			/* 選択している場所にアイテムがあった？ */
			if ((pInfoItem->m_ptBackPack.x == ptPos.x) &&
				(pInfoItem->m_ptBackPack.y == ptPos.y)) {
				dwItemID = pInfoItem->m_dwItemID;
			}
			if (m_ptDrag.x != -1) {
				/* ドラッグ開始位置のアイテム？ */
				if ((pInfoItem->m_ptBackPack.x == m_ptDrag.x) &&
					(pInfoItem->m_ptBackPack.y == m_ptDrag.y)) {
					dwItemIDDrag = pInfoItem->m_dwItemID;
				}
			}
		}
		if (m_ptDrag.x != -1) {
			/* ドラッグされた？ */
			if (!((m_ptDrag.x == ptPos.x) && (m_ptDrag.y == ptPos.y))) {
				m_ptDrop = ptPos;
			}
		}
	}

	if (dwItemIDDrag == 0) {
		m_ptDrag.x = m_ptDrag.y = -1;
		Redraw ();
		if (dwItemID == 0) {
			goto Exit;
		}
	}

	m_dwSelectItemID = dwItemID;
	m_dwDragItemID	 = dwItemIDDrag;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, dwItemID);
	m_pMgrData->SetWindowPosITEMMENUPos (m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::OnZ											 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_CANCEL);
	m_pMgrData->SetWindowPosITEMMENUPos (m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::DrawEquip										 */
/* 内容		:装備アイテムを描画												 */
/* 日付		:2008/11/19														 */
/* ========================================================================= */

void CWindowITEMMENU::DrawEquip(
	int nType,					/* [in] 種別 */
	DWORD dwEquipItemID)		/* [in] 装備アイテムID */
{
	int x, y, anDrawInfo[] = {
			1, 1, EQUIPTYPE_CLOTH,		/* 服 */
			0, 0, EQUIPTYPE_ACCE1,		/* アクセサリ1 */
			0, 1, EQUIPTYPE_ARMSRIGHT,	/* 右手 */
			2, 1, EQUIPTYPE_ARMSLEFT,	/* 左手 */
//			EQUIPTYPE_HEAD,			/* 頭 */
//			EQUIPTYPE_ACCE2,		/* アクセサリ2 */
//			EQUIPTYPE_CLOTH,		/* 服 */
			0, 0, 0};
	PCInfoItem pInfoItem;

	if (dwEquipItemID == 0) {
		return;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwEquipItemID);
	if (pInfoItem == NULL) {
		return;
	}
	x = 12 + 36 * anDrawInfo[nType * 3 + 0];
	y = 32 + 47 * anDrawInfo[nType * 3 + 1];
	m_pMgrDraw->DrawItem (m_pDib, x, y, pInfoItem);
	if (m_nPos == anDrawInfo[nType * 3 + 2]) {
		/* 装備中のアイテムなのでアイテム名を更新 */
		m_strName = pInfoItem->m_strName;
	}
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::GetDrawPos									 */
/* 内容		:アイテム描画位置を取得											 */
/* 日付		:2008/11/19														 */
/* ========================================================================= */

void CWindowITEMMENU::GetDrawPos(
	int nPos,		/* [in] アイテム位置 */
	int &nDstX,		/* [out] X座標 */
	int &nDstY)		/* [out] Y座標 */
{
	if (nPos < EQUIPTYPE_MAX) {
		nDstX = 36 * (nPos % 3) + 12 + 1;
		nDstY = 47 * (nPos / 3) + 32 + 1;
	} else {
		nDstX = 36 * ((nPos - EQUIPTYPE_MAX) % 5) + 1 + 1;
		nDstY = 47 * ((nPos - EQUIPTYPE_MAX) / 5) + 159 + 1;
	}
}

/* Copyright(C)URARA-works 2007 */
