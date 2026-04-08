/// @file WindowSTYLESELECT.cpp
/// @brief 容姿選択ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/10
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "MgrDraw.h"
#include "WindowSEX.h"
#include "WindowHAIRTYPE.h"
#include "WindowHAIRCOLOR.h"
#include "WindowEYECOLOR.h"
#include "WindowSTYLESELECT.h"

CWindowSTYLESELECT::CWindowSTYLESELECT()
{
	m_bAnime	= FALSE;
	m_nPos	= 0;
	m_nPosMax	= 5-2;
	m_bInput	= TRUE;
	m_nID	= WINDOWTYPE_STYLESELECT;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 80;
	m_sizeWindow.cx	= 320;
	m_sizeWindow.cy	= 320;

	m_nDirection	= 0;
	m_nSex	= SEX_MALE;
	m_nSexBack	= SEX_MALE;
	m_wHairTypeID	= 1;
	m_wHairTypeIDBack	= 1;
	m_wHairColorID	= 1;
	m_wHairColorIDBack	= 1;
	m_wEyeColorID	= 1;
	m_wEyeColorIDBack	= 1;

	m_pMgrWindow	= NULL;
	m_pMgrDraw	= NULL;
	m_pWindowSEX	= NULL;
	m_pWindowHAIRTYPE	= NULL;
	m_pWindowHAIRCOLOR	= NULL;
	m_pWindowEYECOLOR	= NULL;

	m_pInfoCharCli = new CInfoCharCli;
}


CWindowSTYLESELECT::~CWindowSTYLESELECT()
{
	SAFE_DELETE(m_pInfoCharCli);
}


void CWindowSTYLESELECT::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_pMgrWindow	= m_pMgrData->GetMgrWindow();
	m_pMgrDraw	= m_pMgrData->GetMgrDraw();

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
	m_pInfoCharCli->Create(m_pMgrData);
	m_pInfoCharCli->m_dwMotionTypeID = 2;
	m_pInfoCharCli->MakeCharGrp();
	m_pMgrData->GetLibInfoChar()->RenewMotionInfo(m_pInfoCharCli);

	PostWindowMessage(1);
}


void CWindowSTYLESELECT::SetFamilyType(WORD wFamilyID)
{
	m_pInfoCharCli->m_wFamilyID = wFamilyID;
	m_pInfoCharCli->MakeCharGrp();
}


void CWindowSTYLESELECT::SetInfoChar(CInfoCharCli *pInfoCharCli)
{
	m_pInfoCharCli->Copy(pInfoCharCli);
	m_pInfoCharCli->MakeCharGrp();

	m_nSex	= m_pInfoCharCli->m_nSex;
	m_wHairTypeID	= m_pInfoCharCli->m_wGrpIDHairType;
	m_wHairColorID	= m_pInfoCharCli->m_wGrpIDHairColor;
	m_wEyeColorID	= m_pInfoCharCli->m_wGrpIDEyeColor;
}


void CWindowSTYLESELECT::OnWindowMsg(int nType, DWORD dwPara)
{
	BOOL bMake, bAnime;

	bMake	= FALSE;
	bAnime	= FALSE;

	switch (nType) {
	case WINDOWTYPE_SEX:	// 性別
		switch (dwPara) {
		case 0:
			m_nSex = m_pWindowSEX->GetType();
			m_pMgrWindow->Delete(WINDOWTYPE_SEX);
			m_pWindowSEX = NULL;
			break;
		case (DWORD)-1:
			m_nSex = m_nSexBack;
			break;
		default:
			m_nSex = m_pWindowSEX->GetType();
			break;
		}
		m_pInfoCharCli->m_nSex = m_nSex;
		bMake = TRUE;
		break;
	case WINDOWTYPE_HAIRTYPE:	// 髪型
		switch (dwPara) {
		case 0:
			m_wHairTypeID = m_pWindowHAIRTYPE->GetType();
			m_pMgrWindow->Delete(WINDOWTYPE_HAIRTYPE);
			m_pWindowHAIRTYPE = NULL;
			m_wHairTypeIDBack = m_wHairTypeID;
			bAnime = TRUE;
			break;
		case (DWORD)-1:
			m_wHairTypeID = m_wHairTypeIDBack;
			m_pWindowHAIRTYPE = NULL;
			bAnime = TRUE;
			break;
		default:
			m_wHairTypeID = m_pWindowHAIRTYPE->GetType();
			break;
		}
		m_pInfoCharCli->m_wGrpIDHairType = m_wHairTypeID;
		bMake = TRUE;
		break;
	case WINDOWTYPE_HAIRCOLOR:	// 髪色
		switch (dwPara) {
		case 0:
			m_wHairColorID = m_pWindowHAIRCOLOR->GetType();
			m_pMgrWindow->Delete(WINDOWTYPE_HAIRCOLOR);
			m_pWindowHAIRCOLOR = NULL;
			m_wHairColorIDBack = m_wHairColorID;
			bAnime = TRUE;
			break;
		case (DWORD)-1:
			m_wHairColorID = m_wHairColorIDBack;
			m_pWindowHAIRCOLOR = NULL;
			bAnime = TRUE;
			break;
		default:
			m_wHairColorID = m_pWindowHAIRCOLOR->GetType();
			break;
		}
		m_pInfoCharCli->m_wGrpIDHairColor = m_wHairColorID;
		bMake = TRUE;
		break;
	case WINDOWTYPE_EYECOLOR:	// 目色
		switch (dwPara) {
		case 0:
			m_wEyeColorID = m_pWindowEYECOLOR->GetType();
			m_pMgrWindow->Delete(WINDOWTYPE_EYECOLOR);
			m_pWindowEYECOLOR = NULL;
			m_wEyeColorIDBack = m_wEyeColorID;
			break;
		case (DWORD)-1:
			m_wEyeColorID = m_wEyeColorIDBack;
			m_pWindowEYECOLOR = NULL;
			break;
		default:
			m_wEyeColorID = m_pWindowEYECOLOR->GetType();
			break;
		}
		m_pInfoCharCli->m_wGrpIDEyeColor = m_wEyeColorID;
		bMake = TRUE;
		break;
	}

	if (bMake) {
		m_pInfoCharCli->MakeCharGrp();
		Redraw();
	}
	if (bAnime) {
		m_bAnime	= FALSE;
		m_nDirection	= 4;
		m_dwLastTimerProc	= 0;
		m_pInfoCharCli->m_nDirection = 1;
	}
}


void CWindowSTYLESELECT::Draw(PCImg32 pDst)
{
	int i, nCount, nDirection;
	HDC hDC;
	COLORREF clText;
	POINT nCursorPos[] = {
		40, 104,
		40, 152,
//	40, 200,
//	40, 248,
		136, 292
	};
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	nCount = 2;
	DrawFrame(2);
	for (i = 0; i < nCount; i ++) {
		DrawFrame2(nCursorPos[i].x, nCursorPos[i].y, 96, 8, 2);
	}
	DrawFrame(176, 112, 100, 136, 2);

	clText	= RGB(124, 123, 232);
	hDC	= m_pDib->Lock();

	TextOut4(hDC, m_hFont14, 88,	24,	_T("新規キャラクター作成"), clText);
	TextOut4(hDC, m_hFont14, 136,	48,	_T("容姿選択"), clText);

	TextOut4(hDC, m_hFont14, nCursorPos[0].x + 26, nCursorPos[0].y - 24, _T("性別"), clText);
	strTmp.Empty();
	switch (m_nSex) {
	case SEX_MALE:	strTmp = "♂　オトコ";	break;
	case SEX_FEMALE:	strTmp = "♀　オンナ";	break;
	}
	TextOut2(hDC, m_hFont14, 40, nCursorPos[0].y - 2, strTmp, clText);
	TextOut4(hDC, m_hFont14, nCursorPos[1].x + 26, nCursorPos[1].y - 24, _T("髪型"), clText);
	strTmp.Format(_T("%02d"), m_wHairTypeID);
//	strTmp = m_pMgrData->GetHairTypeName(m_wHairTypeID);
	TextOut2(hDC, m_hFont14, 40, nCursorPos[1].y - 2, strTmp, clText);
#if 0
	TextOut4(hDC, m_hFont14, nCursorPos[2].x + 26, nCursorPos[2].y - 24, _T("髪の色"), clText);
//	strTmp.Format(_T("%02d"), m_wHairColorID);
	strTmp = m_pMgrData->GetHairColorName(m_wHairColorID);
	TextOut2(hDC, m_hFont14, 40, nCursorPos[2].y - 2, strTmp, clText);
	TextOut4(hDC, m_hFont14, nCursorPos[3].x + 26, nCursorPos[3].y - 24, _T("目の色"), clText);
//	strTmp.Format(_T("%02d"), m_wEyeColorID);
	strTmp = m_pMgrData->GetEyeColorName(m_wEyeColorID);
	TextOut2(hDC, m_hFont14, 40, nCursorPos[3].y - 2, strTmp, clText);
#endif

	TextOut4(hDC, m_hFont14, 132,	288,	_T("次へ >>"), clText);
	TextOut4(hDC, m_hFont14, 256,	288,	_T("２／３"), clText);

	m_pDib->Unlock();

	DrawCursor(nCursorPos[m_nPos].x - 32, nCursorPos[m_nPos].y - 6);
	m_dwTimeDrawStart = timeGetTime();

	nDirection = m_pInfoCharCli->m_nDirection;
	if ((m_nPos == 0) || (m_nPos >= 3)) {
		nDirection = 1;
	}
	m_pMgrDraw->DrawChar(m_pDib, 193, 177, m_pInfoCharCli);

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


BOOL CWindowSTYLESELECT::TimerProc(void)
{
	BOOL bRet;
	DWORD dwTime, dwWait;
	int nDirectionTbl[] = {1, 2, 0, 3};

	bRet = FALSE;

	if (m_bAnime == FALSE) {
		goto Exit;
	}

	dwWait = 1500;
	if (m_nDirection == 0) {
		dwWait = 3000;
	}

	dwTime = timeGetTime();
	if (dwTime - m_dwLastTimerProc < dwWait) {
		goto Exit;
	}
	m_dwLastTimerProc = dwTime;
	m_nDirection ++;
	m_nDirection = (m_nDirection >= 4) ? 0 : m_nDirection;
	m_pInfoCharCli->m_nDirection = nDirectionTbl[m_nDirection];

	m_dwTimeDrawStart = 0;
	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSTYLESELECT::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	if (m_nPos == 2) {
		m_nDirection	= 4;
		m_dwLastTimerProc	= 0;
		m_pInfoCharCli->m_nDirection = 1;
	}
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSTYLESELECT::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax - 1) {
		goto Exit;
	}
	m_nPos ++;
	if (m_nPos == 1) {
		m_nDirection	= 4;
		m_dwLastTimerProc	= 0;
		m_pInfoCharCli->m_nDirection = 1;
	}
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSTYLESELECT::OnLeft(void)
{
	m_nPos = 0;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	return TRUE;
}


BOOL CWindowSTYLESELECT::OnRight(void)
{
	m_nPos = m_nPosMax - 1;
	m_pMgrSound->PlaySound(SOUNDID_CURSORMOVE);
	PostWindowMessage(1);

	return TRUE;
}


BOOL CWindowSTYLESELECT::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);

	switch (m_nPos) {
	case 0:	// 性別
		m_pMgrWindow->MakeWindowSEX();
		m_pWindowSEX = (PCWindowSEX)m_pMgrWindow->GetWindow(WINDOWTYPE_SEX);
		m_pWindowSEX->SetType(m_nSex);
		break;
	case 1:	// 髪型
		m_pMgrWindow->MakeWindowHAIRTYPE();
		m_pWindowHAIRTYPE = (PCWindowHAIRTYPE)m_pMgrWindow->GetWindow(WINDOWTYPE_HAIRTYPE);
		m_pWindowHAIRTYPE->SetType(m_wHairTypeID);
		m_bAnime = TRUE;
		break;
#if 0
	case 2:	// 髪色
		m_pMgrWindow->MakeWindowHAIRCOLOR();
		m_pWindowHAIRCOLOR = (PCWindowHAIRCOLOR)m_pMgrWindow->GetWindow(WINDOWTYPE_HAIRCOLOR);
		m_pWindowHAIRCOLOR->SetType(m_wHairColorID);
		m_bAnime = TRUE;
		break;
	case 3:	// 目色
		m_pMgrWindow->MakeWindowEYECOLOR();
		m_pWindowEYECOLOR = (PCWindowEYECOLOR)m_pMgrWindow->GetWindow(WINDOWTYPE_EYECOLOR);
		m_pWindowEYECOLOR->SetType(m_wEyeColorID);
		break;
#endif
//	case 4:	// 次へ
	case 2:	// 次へ
	PostWindowMessage(0);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CWindowSTYLESELECT::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound(SOUNDID_CANCEL);
	PostWindowMessage(-1);
	m_bDelete = TRUE;

	bRet = TRUE;
Exit:
	return bRet;
}
