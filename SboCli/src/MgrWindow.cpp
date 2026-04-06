/// @file MgrWindow.cpp
/// @brief ウィンドウマネージャクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/03
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrKeyInput.h"
#include "WindowCHARNAME.h"
#include "WindowCHAT.h"
#include "WindowLOGIN.h"
#include "WindowLOGINNull.h"
#include "WindowMSG.h"
#if defined(_WIN32)
#include "WindowLOGINMENU.h"
#include "WindowSEX.h"
#include "WindowACCOUNTINFO.h"
#include "WindowSTATUS.h"
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
#endif
#include "MgrWindow.h"

#if !defined(_WIN32)

CMgrWindow::CMgrWindow()
{
	m_bDraw = FALSE;
	m_bKeyInput = FALSE;
	m_pMgrGrpData = NULL;
	m_pMgrData = NULL;
	m_paWindow = new ARRAYWINDOWBASE;
}

CMgrWindow::~CMgrWindow()
{
	Destroy();
}

void CMgrWindow::Create(CMgrData *pMgrData)
{
	m_pMgrData = pMgrData;
	m_pMgrGrpData = (m_pMgrData != NULL) ? m_pMgrData->GetMgrGrpData() : NULL;
	m_pMgrKeyInput = (m_pMgrData != NULL) ? m_pMgrData->GetMgrKeyInput() : NULL;
}

void CMgrWindow::Destroy(void)
{
	DeleteAll();
}

void CMgrWindow::Update(void)
{
	int nCount = m_paWindow->size();
	for (int i = 0; i < nCount; ++i) {
		m_paWindow->at(i)->Update();
	}
}

void CMgrWindow::Draw(PCImg32 pDst)
{
	int nCount = m_paWindow->size();
	for (int i = 0; i < nCount; ++i) {
		PCWindowBase pWindow = m_paWindow->at(i);
		if (pWindow->IsShow()) {
			pWindow->Draw(pDst);
		}
	}
}

BOOL CMgrWindow::TimerProc(void)
{
	BOOL bRet = FALSE;
	for (int i = (int)m_paWindow->size() - 1; i >= 0; --i) {
		PCWindowBase pWindow = m_paWindow->at(i);
		if (pWindow->IsDelete()) {
			Delete(pWindow->GetID());
			bRet = TRUE;
		} else {
			bRet |= pWindow->TimerProc();
		}
	}

	if (m_bDraw) {
		m_bDraw = FALSE;
		bRet = TRUE;
	}
	return bRet;
}

void CMgrWindow::KeyProc(BYTE byEvent, BOOL bDown)
{
	for (int i = (int)m_paWindow->size() - 1; i >= 0; --i) {
		PCWindowBase pWindow = m_paWindow->at(i);
		if (pWindow->IsActive()) {
			pWindow->KeyProc(byEvent, bDown);
			break;
		}
	}
}

void CMgrWindow::DeleteAll(void)
{
	PCWindowBase pTmp = NULL;
	FreeInfo(m_paWindow, pTmp);
	m_bDraw = TRUE;
	m_bKeyInput = FALSE;
}

void CMgrWindow::Delete(int nID)
{
	for (int i = 0; i < (int)m_paWindow->size(); ++i) {
		PCWindowBase pWindow = m_paWindow->at(i);
		if (pWindow->GetID() == nID) {
			SAFE_DELETE(pWindow);
			m_paWindow->erase(m_paWindow->begin() + i);
			SetActive();
			m_bDraw = TRUE;
			if (m_pMgrKeyInput) {
				m_pMgrKeyInput->Reset();
			}
			break;
		}
	}
}

CWindowBase *CMgrWindow::GetWindow(int nID)
{
	for (int i = 0; i < (int)m_paWindow->size(); ++i) {
		PCWindowBase pTmp = m_paWindow->at(i);
		if (pTmp->GetID() == nID) {
			return pTmp;
		}
	}
	return NULL;
}

CWindowBase *CMgrWindow::GetActiveWindow(void)
{
	for (int i = (int)m_paWindow->size() - 1; i >= 0; --i) {
		PCWindowBase pWindow = m_paWindow->at(i);
		if (pWindow->IsActive()) {
			return pWindow;
		}
	}
	return NULL;
}

ILoginWindow *CMgrWindow::GetLoginWindow(void)
{
	CWindowBase *pWindow = GetWindow(WINDOWTYPE_LOGIN);
	return (pWindow != NULL) ? pWindow->GetLoginWindowInterface() : NULL;
}

BOOL CMgrWindow::IsKeyInput(void)
{
	return m_bKeyInput;
}

void CMgrWindow::MakeWindowLOGIN(void)
{
	CWindowBase *pWindowNew = new CWindowLOGIN;

	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();
	m_bDraw = TRUE;
}

void CMgrWindow::MakeWindowMSG(LPCSTR pszMsg, DWORD dwTime, int nType)
{
	PCWindowMSG pWindowNew = new CWindowMSG;
	pWindowNew->Create(m_pMgrData);
	pWindowNew->SetMsg(pszMsg, dwTime, nType);
	m_paWindow->Add(pWindowNew);
	SetActive();
	m_bDraw = TRUE;
}

void CMgrWindow::MakeWindowLOGINMENU(void) {}
void CMgrWindow::MakeWindowCHARNAME(void)
{
	PCWindowCHARNAME pWindowNew = new CWindowCHARNAME;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();
	m_bDraw = TRUE;
}
void CMgrWindow::MakeWindowSEX(void) {}
void CMgrWindow::MakeWindowACCOUNTINFO(void) {}
void CMgrWindow::MakeWindowSTATUS(BOOL bShow) { (void)bShow; }
void CMgrWindow::MakeWindowCHAT(void)
{
	PCWindowCHAT pWindowNew = new CWindowCHAT;
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();
	m_bDraw = TRUE;
}
void CMgrWindow::MakeWindowFAMILYTYPE(void) {}
void CMgrWindow::MakeWindowHAIRTYPE(void) {}
void CMgrWindow::MakeWindowHAIRCOLOR(void) {}
void CMgrWindow::MakeWindowEYECOLOR(void) {}
void CMgrWindow::MakeWindowSTYLESELECT(void) {}
void CMgrWindow::MakeWindowNAMEINPUT(void) {}
void CMgrWindow::MakeWindowSYSTEMMENU(void) {}
void CMgrWindow::MakeWindowSETCOLOR(void) {}
void CMgrWindow::MakeWindowSETBGMVOLUME(void) {}
void CMgrWindow::MakeWindowSETSOUNDVOLUME(void) {}
void CMgrWindow::MakeWindowSETDRAWMODE(void) {}
void CMgrWindow::MakeWindowITEMMENU(void) {}
void CMgrWindow::MakeWindowITEMMENU_SELECT(int nPos, DWORD dwItemID) { (void)nPos; (void)dwItemID; }
void CMgrWindow::MakeWindowOPTION(void) {}
void CMgrWindow::MakeWindowOPTION_VIEWSET(void) {}
void CMgrWindow::MakeWindowOPTION_VOLUMESET(void) {}
void CMgrWindow::MakeWindowOPTION_INPUTSET(void) {}
void CMgrWindow::MakeWindowOPTION_TASKBAR(void) {}
void CMgrWindow::MakeWindowOPTION_INPUTSET_SETDEVICE(void) {}
void CMgrWindow::MakeWindowOPTION_ACTIONSET(void) {}
void CMgrWindow::MakeWindowOPTION_ACTIONSET_SLEEPTIMER(void) {}
void CMgrWindow::MakeWindowCOMMANDMENU(void) {}
void CMgrWindow::MakeWindowCHAR_STATUS(void) {}
void CMgrWindow::MakeWindowCHAR_STATUS4(void) {}
void CMgrWindow::MakeWindowTEXTMSG(LPCSTR pszTitle, LPCSTR pszName, LPCSTR pszMsg)
{
	(void)pszTitle;
	(void)pszName;
	MakeWindowMSG(pszMsg, 0, 0);
}
void CMgrWindow::MakeWindowTEXTMSG(LPCSTR pszTitle, LPCSTR pszName, CInfoTalkEvent *pInfo)
{
	(void)pszTitle;
	(void)pszName;
	(void)pInfo;
}
void CMgrWindow::MakeWindowSWOON(void) {}
void CMgrWindow::MakeWindowSKILLMENU(int nType) { (void)nType; }
void CMgrWindow::MakeWindowPLACEINFORMATION(void) {}

void CMgrWindow::SetActive(void)
{
	BOOL bSet = FALSE;
	m_bKeyInput = FALSE;

	for (int i = (int)m_paWindow->size() - 1; i >= 0; --i) {
		PCWindowBase pWindow = m_paWindow->at(i);
		if (pWindow->IsInput() == FALSE) {
			continue;
		}
		pWindow->SetActive(1 - bSet);
		bSet = TRUE;
	}

	m_bKeyInput = bSet;
}

#else


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
	PCWindowBase pTmp;

	pTmp = NULL;
	FreeInfo(m_paWindow, pTmp);
	m_bDraw		= TRUE;
	m_bKeyInput	= FALSE;

#if defined(_WIN32)
	if (m_pMgrData) {
		HWND hWndMain;

		hWndMain = m_pMgrData->GetMainWindow();
		SetFocus(hWndMain);
	}
#endif
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


CWindowBase *CMgrWindow::GetActiveWindow(void)
{
	int i, nCount;
	PCWindowBase pWindow;

	nCount = m_paWindow->size();
	for (i = nCount - 1; i >= 0; i--) {
		pWindow = m_paWindow->at(i);
		if (pWindow->IsActive()) {
			return pWindow;
		}
	}

	return NULL;
}


ILoginWindow *CMgrWindow::GetLoginWindow(void)
{
	CWindowBase *pWindow;

	pWindow = GetWindow(WINDOWTYPE_LOGIN);
	if (pWindow == NULL) {
		return NULL;
	}

	return pWindow->GetLoginWindowInterface();
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
	CWindowBase *pWindowNew;

	if (m_pMgrData->IsLocalTitleMode()) {
		pWindowNew = new CWindowLOGINNull;
	} else {
		pWindowNew = new CWindowLOGIN;
	}
	pWindowNew->Create(m_pMgrData);
	m_paWindow->Add(pWindowNew);
	SetActive();

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

#endif
