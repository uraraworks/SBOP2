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
	m_nPos			= EQUIPTYPE_MAX;
	m_nPosMax		= EQUIPTYPE_MAX + 24;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_ITEMMENU;
	m_ptViewPos.x	= 24;
	m_ptViewPos.y	= 64;
	m_sizeWindow.cx	= 186;
	m_sizeWindow.cy	= 301;

	m_dwSelectItemID = 0;

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
}


/* ========================================================================= */
/* 関数名	:CWindowITEMMENU::Draw											 */
/* 内容		:描画															 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

void CWindowITEMMENU::Draw(PCImg32 pDst)
{
	int i, nCount, nLevel, x, y;
	HDC hDC;
	HFONT hFontOld;
	PCInfoItem pInfoItem;
	PCMgrDraw pMgrDraw;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_strName.Empty ();
	pMgrDraw = m_pMgrData->GetMgrDraw ();
	m_pDib->BltFrom256 (186 / 2 - 62, 0, 124, 84, m_pDibSystem, 368, 200, TRUE);
	m_pDib->BltFrom256 (0, 84, 186, 217, m_pDibSystem, 0, 200, TRUE);

	/* 装備アイテムを描画 */
	/* 服 */
	if (m_pPlayerChar->m_dwEquipItemIDCloth) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pPlayerChar->m_dwEquipItemIDCloth);
		if (pInfoItem) {
			pMgrDraw->DrawItem (m_pDib, 41 + 36 * 1, 10 + 36 * 1, pInfoItem);
			if (m_nPos == EQUIPTYPE_CLOTH) {
				m_strName = pInfoItem->m_strName;
			}
		}
	}
	/* アクセサリ1 */
	if (m_pPlayerChar->m_dwEquipItemIDAcce1) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pPlayerChar->m_dwEquipItemIDAcce1);
		if (pInfoItem) {
			pMgrDraw->DrawItem (m_pDib, 41 + 36 * 0, 10 + 36 * 0, pInfoItem);
			if (m_nPos == EQUIPTYPE_ACCE1) {
				m_strName = pInfoItem->m_strName;
			}
		}
	}
	/* 持ち物 */
	if (m_pPlayerChar->m_dwEquipItemIDArmsRight) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pPlayerChar->m_dwEquipItemIDArmsRight);
		if (pInfoItem) {
			pMgrDraw->DrawItem (m_pDib, 41 + 36 * 0, 10 + 36 * 1, pInfoItem);
			if (m_nPos == EQUIPTYPE_ARMSRIGHT) {
				m_strName = pInfoItem->m_strName;
			}
		}
	}
	/* 盾 */
	if (m_pPlayerChar->m_dwEquipItemIDArmsLeft) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pPlayerChar->m_dwEquipItemIDArmsLeft);
		if (pInfoItem) {
			pMgrDraw->DrawItem (m_pDib, 41 + 36 * 2, 10 + 36 * 1, pInfoItem);
			if (m_nPos == EQUIPTYPE_ARMSLEFT) {
				m_strName = pInfoItem->m_strName;
			}
		}
	}

	/* アイテムを描画 */
	nCount = m_pPlayerChar->m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pPlayerChar->m_adwItemID[i]);
		if (pInfoItem == NULL) {
			continue;
		}
		pMgrDraw->DrawItem (m_pDib,
				10 + (pInfoItem->m_ptBackPack.x * 33),
				95 + (pInfoItem->m_ptBackPack.y * 33),
				pInfoItem);
		if (m_strName.IsEmpty ()) {
			if (((m_nPos - EQUIPTYPE_MAX) % 5 == pInfoItem->m_ptBackPack.x) &&
				((m_nPos - EQUIPTYPE_MAX) / 5 == pInfoItem->m_ptBackPack.y)) {
				m_strName = pInfoItem->m_strName;
			}
		}
	}

	if (m_nPos < EQUIPTYPE_MAX) {
		x = 36 * (m_nPos % 3) + 41 + 1;
		y = 36 * (m_nPos / 3) + 10 + 1;
	} else {
		x = 33 * ((m_nPos - EQUIPTYPE_MAX) % 5) + 11;
		y = 33 * ((m_nPos - EQUIPTYPE_MAX) / 5) + 96;
	}
	m_pDib->BltFrom256 (x - 8, y, 24, 24, m_pDibSystem, 48, 24, TRUE);
	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);

	if (m_strName.IsEmpty () == FALSE) {
		hDC			= pDst->Lock ();
		hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
		SetBkMode (hDC, TRANSPARENT);

		if (m_nPos < EQUIPTYPE_MAX) {
			x = 36 * (m_nPos % 3) + 41 + 1;
			y = 36 * (m_nPos / 3) + 10 + 1;
		} else {
			x = 33 * ((m_nPos - EQUIPTYPE_MAX) % 5) + 11;
			y = 33 * ((m_nPos - EQUIPTYPE_MAX) / 5) + 96;
		}
		DrawFrame2 (m_ptViewPos.x + 32 + x - 8, m_ptViewPos.y + 32 + y - 24, m_strName.GetLength () * 6, 16, 0, pDst, 4);
		TextOut2 (hDC, m_ptViewPos.x + 32 + x - 8, m_ptViewPos.y + 32 + y - 24, (LPCSTR)m_strName, RGB (10, 10, 10), FALSE);

		SelectObject (hDC, hFontOld);
		pDst->Unlock ();
	}
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
	DWORD dwItemID;
	PCInfoItem pInfoItem;

	bRet		= FALSE;
	dwItemID	= 0;

	if (bDown) {
		goto Exit;
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
		ptPos.x = (m_nPos - EQUIPTYPE_MAX) % 5;
		ptPos.y = (m_nPos - EQUIPTYPE_MAX) / 5;

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
				break;
			}
		}
	}

	if (dwItemID == 0) {
		goto Exit;
	}

	m_dwSelectItemID = dwItemID;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, dwItemID);

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

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2007 */
