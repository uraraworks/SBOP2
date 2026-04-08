/// @file WindowITEMMENU_SELECT.cpp
/// @brief アイテムどうするかメニューウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/13
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "LibInfoItem.h"
#include "LibInfoItemType.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowITEMMENU_SELECT.h"


CWindowITEMMENU_SELECT::CWindowITEMMENU_SELECT()
{
	m_nPosMax	= 0;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_ITEMMENU_SELECT;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 112;
	m_sizeWindow.cx	= 16 * 2 + 16 * 6;
	m_sizeWindow.cy	= 16 * 2 + 16 * 1;

	m_nType	= -1;
	m_pInfoItem	= NULL;
	m_pLibInfoItem	= NULL;
}


CWindowITEMMENU_SELECT::~CWindowITEMMENU_SELECT()
{
}


void CWindowITEMMENU_SELECT::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);

	m_pLibInfoItem = m_pMgrData->GetLibInfoItem();
}


void CWindowITEMMENU_SELECT::Draw(PCImg32 pDst)
{
	int i, nCount, nLevel;
	HDC hDC;
	COLORREF clText;
	LPCTSTR pszTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame();

	clText	= RGB(1, 1, 1);
	hDC	= m_pDib->Lock();

	clText = RGB(1, 1, 1);

	nCount = m_anCommand.size();
	for (i = 0; i < nCount; i ++) {
		pszTmp = _T("");
		switch (m_anCommand[i]) {
		case ITEMMENU_SELECT_COMMAND_PUT:	pszTmp = _T("地面に置く");	break;	// 地面に置く
		case ITEMMENU_SELECT_COMMAND_EQUIP:	pszTmp = _T("装備する");	break;	// 装備する
		case ITEMMENU_SELECT_COMMAND_EQUIP_UNSET:	pszTmp = _T("装備を外す");	break;	// 装備を外す
		case ITEMMENU_SELECT_COMMAND_USE:	pszTmp = _T("使う");	break;	// 使う
		}
		TextOut2(hDC, m_hFont, 32, 16 + 16 * i, pszTmp, clText);
	}

	m_pDib->Unlock();

	DrawCursor(8, 16 + 16 * m_nPos);
	m_dwTimeDrawStart = timeGetTime();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


void CWindowITEMMENU_SELECT::SetItemID(int nType, DWORD dwItemID)
{
	int nPosMaxBack;
	DWORD dwItemTypeID;

	m_pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(dwItemID);
	if (m_pInfoItem == NULL) {
		return;
	}

	m_nType	= nType;
	nPosMaxBack	= m_nPosMax;

	m_anCommand.clear();

	if (m_nType < EQUIPTYPE_MAX) {
		m_anCommand.push_back(ITEMMENU_SELECT_COMMAND_EQUIP_UNSET);
	} else {
		dwItemTypeID = m_pLibInfoItem->GetItemType(m_pInfoItem->m_dwItemID);
		switch (dwItemTypeID) {
		case ITEMTYPEID_CLOTH:	// 服
		case ITEMTYPEID_ACCE:	// アクセサリ
		case ITEMTYPEID_ARMS:	// 持ち物
		case ITEMTYPEID_SHIELD:	// 盾
			m_anCommand.push_back(ITEMMENU_SELECT_COMMAND_EQUIP);
			break;
		}
		m_anCommand.push_back(ITEMMENU_SELECT_COMMAND_USE);
		m_anCommand.push_back(ITEMMENU_SELECT_COMMAND_PUT);
	}

	m_nPosMax	= m_anCommand.size() - 1;
	m_nPos	= min(m_nPos, m_nPosMax);
	if (nPosMaxBack != m_nPosMax) {
		m_pDib->Destroy();
		// サイズが変わったので画像再作成
		m_sizeWindow.cy	= 16 * 2 + 16 * (m_nPosMax + 1);
		m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
		m_pDib->SetColorKey(0);
		m_dwTimeDrawStart = 0;
	}
}


BOOL CWindowITEMMENU_SELECT::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowITEMMENU_SELECT::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax) {
		goto Exit;
	}
	m_nPos ++;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowITEMMENU_SELECT::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowITEMMENU_SELECT::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);

	return TRUE;
}


BOOL CWindowITEMMENU_SELECT::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

//	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	PostWindowMessage(m_anCommand[m_nPos]);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowITEMMENU_SELECT::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound(SOUNDID_CANCEL);

	bRet = TRUE;
Exit:
	return bRet;
}
