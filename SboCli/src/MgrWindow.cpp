/// @file MgrWindow.cpp
/// @brief ウィンドウマネージャクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/03
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrKeyInput.h"
#include "WindowLOGIN.h"
#include "WindowMSG.h"
#include "WindowLOGINMENU.h"
#include "WindowCHARNAME.h"
#include "WindowSEX.h"
#include "WindowACCOUNTINFO.h"
#include "WindowSTATUS.h"
#include "WindowCHAT.h"
#include "WindowFAMILYTYPE.h"
#include "WindowHAIRTYPE.h"
#include "WindowHAIRCOLOR.h"
#include "WindowEYECOLOR.h"
#include "WindowSTYLESELECT.h"
#include "WindowNAMEINPUT.h"
#include "WindowSYSTEMMENU.h"
#include "WindowSETCOLOR.h"
#include "WindowSETBGMVOLUME.h"
#include "WindowSETSOUNDVOLUME.h"
#include "WindowSETDRAWMODE.h"
#include "WindowITEMMENU.h"
#include "WindowITEMMENU_SELECT.h"
#include "WindowOPTION.h"
#include "WindowOPTION_VIEWSET.h"
#include "WindowOPTION_VOLUMESET.h"
#include "WindowOPTION_INPUTSET.h"
#include "WindowOPTION_TASKBAR.h"
#include "WindowOPTION_INPUTSET_SETDEVICE.h"
#include "WindowOPTION_ACTIONSET.h"
#include "WindowOPTION_ACTIONSET_SLEEPTIMER.h"
#include "WindowCOMMANDMENU.h"
#include "WindowCHAR_STATUS.h"
#include "WindowCHAR_STATUS2.h"
#include "WindowCHAR_STATUS3.h"
#include "WindowCHAR_STATUS4.h"
#include "WindowTEXTMSG.h"
#include "WindowSWOON.h"
#include "WindowSKILLMENU.h"
#include "WindowPLACEINFORMATION.h"
#include "MgrWindow.h"


CMgrWindow::CMgrWindow()
{
	m_bDraw			= FALSE;
	m_bKeyInput		= FALSE;
	m_pMgrGrpData	= NULL;
	m_pMgrData		= NULL;
	m_paWindow		= new ARRAYWINDOWBASE;
}


CMgrWindow::~CMgrWindow()
{
	Destroy();
}


void CMgrWindow::Create(
	CMgrData *pMgrData)		// [in] データ管理
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData();
	m_pMgrKeyInput	= m_pMgrData->GetMgrKeyInput();
}


void CMgrWindow::Destroy(void)
{
	DeleteAll();
}


void CMgrWindow::Update(void)
{
	int i, nCount;
	PCWindowBase pWindow;

	nCount = m_paWindow->size();
	for (i = 0; i < nCount; i++) {
		pWindow = m_paWindow->at(i);
		pWindow->Update();
	}
}


void CMgrWindow::Draw(PCImg32 pDst)
{
	int i, nCount;
	PCWindowBase pWindow;

	nCount = m_paWindow->size();
	for (i = 0; i < nCount; i++) {
		pWindow = m_paWindow->at(i);
		if (pWindow->IsShow() == FALSE) {
			continue;
		}
		pWindow->Draw(pDst);
	}
}


BOOL CMgrWindow::TimerProc(void)
{
	int i, nCount;
	BOOL bRet;
	PCWindowBase pWindow;

	bRet = FALSE;
	nCount = m_paWindow->size();
	for (i = nCount - 1; i >= 0; i--) {
		pWindow = m_paWindow->at(i);
		if (pWindow->IsDelete()) {
			Delete(pWindow->GetID());
			bRet = TRUE;

		} else {
			bRet |= pWindow->TimerProc();
		}
	}

	if (m_bDraw) {
		m_bDraw	= FALSE;
		bRet	= TRUE;
	}
	return bRet;
}


void CMgrWindow::KeyProc(BYTE byEvent, BOOL bDown)
{
	int i, nCount;
	PCWindowBase pWindow;

	nCount = m_paWindow->size();
	for (i = nCount - 1; i >= 0; i--) {
		pWindow = m_paWindow->at(i);
		if (pWindow->IsActive()) {
			pWindow->KeyProc(byEvent, bDown);
			break;
		}
	}
}


void CMgrWindow::DeleteAll(void)
{
	HWND hWndMain;
	PCWindowBase pTmp;

	pTmp = NULL;
	FreeInfo(m_paWindow, pTmp);
	m_bDraw		= TRUE;
	m_bKeyInput	= FALSE;

	if (m_pMgrData) {
		hWndMain = m_pMgrData->GetMainWindow();
		SetFocus(hWndMain);
	}
}


void CMgrWindow::Delete(int nID)
{
	int i, j, nCount, nCount2;
	PCWindowBase pWindow, pChild;

	nCount = m_paWindow->size();
	for (i = 0; i < nCount; i++) {
		pWindow = m_paWindow->at(i);
		if (pWindow->GetID() == nID) {
			nCount2 = pWindow->m_apChild.size();
			for (j = 0; j < nCount2; j++) {
				pChild = pWindow->m_apChild[j];
				pChild->m_bDelete = TRUE;
			}

			SAFE_DELETE(pWindow);
			m_paWindow->erase(m_paWindow->begin() + i);
			SetActive();
			m_bDraw = TRUE;
			m_pMgrKeyInput->Reset();
			break;
		}
	}
}


CWindowBase *CMgrWindow::GetWindow(int nID)
{
	int i, nCount;
	PCWindowBase pRet, pTmp;

	pRet	= NULL;
	nCount	= m_paWindow->size();

	for (i = 0; i < nCount; i++) {
		pTmp = m_paWindow->at(i);
		if (pTmp->GetID() == nID) {
			pRet = pTmp;
			break;
		}
	}

	return pRet;
}


BOOL CMgrWindow::IsKeyInput(void)
{
	int i, nCount;
	BOOL bRet;
	PCWindowBase pTmp;

	bRet	= FALSE;
	nCount	= m_paWindow->size();

	if (nCount <= 0) {
		goto Exit;
	}

	for (i = 0; i < nCount; i++) {
		pTmp = m_paWindow->at(i);
		if (pTmp->IsActive()) {
			bRet = TRUE;
			break;
		}
	}

Exit:
	return bRet;
}


void CMgrWindow::MakeWindowLOGIN(void)
{
	PCWindowLOGIN pWindowNew;

	pWindowNew = new CWindowLOGIN;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowMSG(LPCSTR pszMsg, DWORD dwTime, int nType)
{
	PCWindowMSG pWindowNew;
	PCWindowBase pWindowTmp;

	pWindowTmp = GetWindow(WINDOWTYPE_MSG);
	if (pWindowTmp) {
		pWindowNew = (PCWindowMSG)pWindowTmp;

	} else {
		pWindowNew = new CWindowMSG;
		pWindowNew->Create(m_pMgrData);
		m_paWindow->Add(pWindowNew);
	}

	pWindowNew->SetMsg(pszMsg, dwTime, nType);
	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowLOGINMENU(void)
{
	PCWindowLOGINMENU pWindowNew;

	pWindowNew = new CWindowLOGINMENU;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowCHARNAME(void)
{
	PCWindowCHARNAME pWindowNew;

	pWindowNew = new CWindowCHARNAME;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowSEX(void)
{
	PCWindowSEX pWindowNew;

	pWindowNew = new CWindowSEX;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowACCOUNTINFO(void)
{
	PCWindowACCOUNTINFO pWindowNew;

	pWindowNew = new CWindowACCOUNTINFO;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowSTATUS(BOOL bShow)
{
	PCWindowSTATUS pWindowNew;

	pWindowNew = new CWindowSTATUS;
	pWindowNew->Create(m_pMgrData);
	pWindowNew->SetShow(bShow);
	m_paWindow->Add(pWindowNew);
	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowCHAT(void)
{
	PCWindowCHAT pWindowNew;

	pWindowNew = new CWindowCHAT;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowFAMILYTYPE(void)
{
	PCWindowFAMILYTYPE pWindowNew;

	pWindowNew = new CWindowFAMILYTYPE;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowHAIRTYPE(void)
{
	PCWindowHAIRTYPE pWindowNew;

	pWindowNew = new CWindowHAIRTYPE;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowHAIRCOLOR(void)
{
	PCWindowHAIRCOLOR pWindowNew;

	pWindowNew = new CWindowHAIRCOLOR;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowEYECOLOR(void)
{
	PCWindowEYECOLOR pWindowNew;

	pWindowNew = new CWindowEYECOLOR;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowSTYLESELECT(void)
{
	PCWindowSTYLESELECT pWindowNew;

	pWindowNew = new CWindowSTYLESELECT;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowNAMEINPUT(void)
{
	PCWindowNAMEINPUT pWindowNew;

	pWindowNew = new CWindowNAMEINPUT;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowSYSTEMMENU(void)
{
	PCWindowSYSTEMMENU pWindowNew;

	pWindowNew = new CWindowSYSTEMMENU;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowSETCOLOR(void)
{
	PCWindowSETCOLOR pWindowNew;

	pWindowNew = new CWindowSETCOLOR;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowSETBGMVOLUME(void)
{
	PCWindowSETBGMVOLUME pWindowNew;

	pWindowNew = new CWindowSETBGMVOLUME;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowSETSOUNDVOLUME(void)
{
	PCWindowSETSOUNDVOLUME pWindowNew;

	pWindowNew = new CWindowSETSOUNDVOLUME;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowSETDRAWMODE(void)
{
	PCWindowSETDRAWMODE pWindowNew;

	pWindowNew = new CWindowSETDRAWMODE;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowITEMMENU(void)
{
	PCWindowITEMMENU pWindowNew;

	pWindowNew = new CWindowITEMMENU;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowITEMMENU_SELECT(int nPos, DWORD dwItemID)
{
	PCWindowITEMMENU_SELECT pWindowNew;

	pWindowNew = new CWindowITEMMENU_SELECT;
	pWindowNew->Create(m_pMgrData);
	pWindowNew->SetItemID(nPos, dwItemID);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowOPTION(void)
{
	PCWindowOPTION pWindowNew;

	pWindowNew = new CWindowOPTION;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowOPTION_VIEWSET(void)
{
	PCWindowOPTION_VIEWSET pWindowNew;

	pWindowNew = new CWindowOPTION_VIEWSET;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowOPTION_VOLUMESET(void)
{
	PCWindowOPTION_VOLUMESET pWindowNew;

	pWindowNew = new CWindowOPTION_VOLUMESET;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowOPTION_INPUTSET(void)
{
	PCWindowOPTION_INPUTSET pWindowNew;

	pWindowNew = new CWindowOPTION_INPUTSET;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowOPTION_TASKBAR(void)
{
	PCWindowOPTION_TASKBAR pWindowNew;

	pWindowNew = new CWindowOPTION_TASKBAR;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowOPTION_INPUTSET_SETDEVICE(void)
{
	PCWindowOPTION_INPUTSET_SETDEVICE pWindowNew;

	pWindowNew = new CWindowOPTION_INPUTSET_SETDEVICE;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowOPTION_ACTIONSET(void)
{
	PCWindowOPTION_ACTIONSET pWindowNew;

	pWindowNew = new CWindowOPTION_ACTIONSET;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowOPTION_ACTIONSET_SLEEPTIMER(void)
{
	PCWindowOPTION_ACTIONSET_SLEEPTIMER pWindowNew;

	pWindowNew = new CWindowOPTION_ACTIONSET_SLEEPTIMER;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowCOMMANDMENU(void)
{
	PCWindowCOMMANDMENU pWindowNew;

	pWindowNew = new CWindowCOMMANDMENU;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowCHAR_STATUS(void)
{
	PCWindowCHAR_STATUS pWindowNew;

	pWindowNew = new CWindowCHAR_STATUS;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowCHAR_STATUS4(void)
{
	PCWindowCHAR_STATUS4 pWindowNew;

	pWindowNew = new CWindowCHAR_STATUS4;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowTEXTMSG(LPCSTR pszTitle, LPCSTR pszName, LPCSTR pszMsg)
{
	PCWindowTEXTMSG pWindowNew;

	pWindowNew = new CWindowTEXTMSG;
	pWindowNew->Create(m_pMgrData);
	pWindowNew->SetTitle(pszTitle);
	pWindowNew->SetName(pszName);
	pWindowNew->SetMsg(pszMsg);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowTEXTMSG(LPCSTR pszTitle, LPCSTR pszName, CInfoTalkEvent *pInfo)
{
	PCWindowTEXTMSG pWindowNew;

	pWindowNew = new CWindowTEXTMSG;
	pWindowNew->Create(m_pMgrData);
	pWindowNew->SetTitle(pszTitle);
	pWindowNew->SetName(pszName);
	pWindowNew->SetTalkEvent(pInfo);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowSWOON(void)
{
	PCWindowSWOON pWindowNew;

	pWindowNew = new CWindowSWOON;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowSKILLMENU(int nType/*-1*/)
{
	PCWindowSKILLMENU pWindowNew;

	pWindowNew = new CWindowSKILLMENU;
	pWindowNew->Create(m_pMgrData);
	if (nType != -1) {
		pWindowNew->SetType(nType);
	}
	m_paWindow->Add(pWindowNew);
	SetActive();

	m_bDraw = TRUE;
}


void CMgrWindow::MakeWindowPLACEINFORMATION(void)
{
	PCWindowPLACEINFORMATION pWindowNew;

	pWindowNew = new CWindowPLACEINFORMATION;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);

	m_bDraw = TRUE;
}


void CMgrWindow::SetActive(void)
{
	int i, nCount;
	BOOL bSet;
	PCWindowBase pWindow;

	bSet		= FALSE;
	m_bKeyInput	= FALSE;
	nCount		= m_paWindow->size();
	if (nCount <= 0) {
		return;
	}

	for (i = nCount - 1; i >= 0; i--) {
		pWindow = m_paWindow->at(i);
		if (pWindow->IsInput() == FALSE) {
			continue;
		}
		pWindow->SetActive(1 - bSet);
		bSet = TRUE;
	}

	m_bKeyInput = bSet;
}
