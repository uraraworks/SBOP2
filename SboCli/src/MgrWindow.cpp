/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrWindow.cpp												 */
/* ���e			:�E�B���h�E�}�l�[�W���N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/03													 */
/* ========================================================================= */

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
#include "MgrWindow.h"


/* ========================================================================= */
/* �֐���	:CMgrWindow::CMgrWindow											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/05/06														 */
/* ========================================================================= */

CMgrWindow::CMgrWindow()
{
	m_bDraw			= FALSE;
	m_bKeyInput		= FALSE;
	m_pMgrGrpData	= NULL;
	m_pMgrData		= NULL;
	m_paWindow		= new ARRAYWINDOWBASE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::~CMgrWindow										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/05/06														 */
/* ========================================================================= */

CMgrWindow::~CMgrWindow()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2005/05/21														 */
/* ========================================================================= */

void CMgrWindow::Create(
	CMgrData	*pMgrData)		/* [in] �f�[�^�Ǘ� */
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();
	m_pMgrKeyInput	= m_pMgrData->GetMgrKeyInput ();
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::Destroy											 */
/* ���e		:�j��															 */
/* ���t		:2005/05/21														 */
/* ========================================================================= */

void CMgrWindow::Destroy(void)
{
	DeleteAll ();
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::Update												 */
/* ���e		:�S�ẴE�B���h�E���X�V											 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CMgrWindow::Update(void)
{
	int i, nCount;
	PCWindowBase pWindow;

	nCount = m_paWindow->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pWindow = m_paWindow->GetAt (i);
		pWindow->Update ();
	}
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2005/05/06														 */
/* ========================================================================= */

void CMgrWindow::Draw(PCImg32 pDst)
{
	int i, nCount;
	PCWindowBase pWindow;

	nCount = m_paWindow->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pWindow = m_paWindow->GetAt (i);
		if (pWindow->IsShow () == FALSE) {
			continue;
		}
		pWindow->Draw (pDst);
	}
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::TimerProc											 */
/* ���e		:���ԏ���														 */
/* ���t		:2005/06/12														 */
/* ========================================================================= */

BOOL CMgrWindow::TimerProc(void)
{
	int i, nCount;
	BOOL bRet;
	PCWindowBase pWindow;

	bRet = FALSE;
	nCount = m_paWindow->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pWindow = m_paWindow->GetAt (i);
		if (pWindow->IsDelete ()) {
			Delete (pWindow->GetID ());
			bRet = TRUE;

		} else {
			bRet |= pWindow->TimerProc ();
		}
	}

	if (m_bDraw) {
		m_bDraw	= FALSE;
		bRet	= TRUE;
	}
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::KeyProc											 */
/* ���e		:�L�[����														 */
/* ���t		:2005/05/06														 */
/* ========================================================================= */

void CMgrWindow::KeyProc(BYTE byEvent, BOOL bDown)
{
	int i, nCount;
	PCWindowBase pWindow;

	nCount = m_paWindow->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pWindow = m_paWindow->GetAt (i);
		if (pWindow->IsActive ()) {
			pWindow->KeyProc (byEvent, bDown);
			break;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::DeleteAll											 */
/* ���e		:�S�ẴE�B���h�E��j��											 */
/* ���t		:2005/05/22														 */
/* ========================================================================= */

void CMgrWindow::DeleteAll(void)
{
	HWND hWndMain;
	PCWindowBase pTmp;

	pTmp = NULL;
	FreeInfo (m_paWindow, pTmp);
	m_bDraw		= TRUE;
	m_bKeyInput	= FALSE;

	if (m_pMgrData) {
		hWndMain = m_pMgrData->GetMainWindow ();
		SetFocus (hWndMain);
	}
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::Delete												 */
/* ���e		:�w��ID�̃E�B���h�E���폜										 */
/* ���t		:2005/06/16														 */
/* ========================================================================= */

void CMgrWindow::Delete(int nID)
{
	int i, j, nCount, nCount2;
	PCWindowBase pWindow, pChild;

	nCount = m_paWindow->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pWindow = m_paWindow->GetAt (i);
		if (pWindow->GetID () == nID) {
			nCount2 = pWindow->m_apChild.GetSize ();
			for (j = 0; j < nCount2; j ++) {
				pChild = pWindow->m_apChild[j];
				pChild->m_bDelete = TRUE;
			}

			SAFE_DELETE (pWindow);
			m_paWindow->RemoveAt (i);
			SetActive ();
			m_bDraw = TRUE;
			m_pMgrKeyInput->Reset ();
			break;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::GetWindow											 */
/* ���e		:�w��ID�̃E�B���h�E���擾										 */
/* ���t		:2005/06/11														 */
/* ========================================================================= */

CWindowBase *CMgrWindow::GetWindow(int nID)
{
	int i, nCount;
	PCWindowBase pRet, pTmp;

	pRet	= NULL;
	nCount	= m_paWindow->GetSize ();

	for (i = 0; i < nCount; i ++) {
		pTmp = m_paWindow->GetAt (i);
		if (pTmp->GetID () == nID) {
			pRet = pTmp;
			break;
		}
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::IsKeyInput											 */
/* ���e		:�L�[�����̕K�v�����邩����										 */
/* ���t		:2005/06/26														 */
/* ========================================================================= */

BOOL CMgrWindow::IsKeyInput(void)
{
	int i, nCount;
	BOOL bRet;
	PCWindowBase pTmp;

	bRet	= FALSE;
	nCount	= m_paWindow->GetSize ();

	if (nCount <= 0) {
		goto Exit;
	}

	for (i = 0; i < nCount; i ++) {
		pTmp = m_paWindow->GetAt (i);
		if (pTmp->IsActive ()) {
			bRet = TRUE;
			break;
		}
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowLOGIN									 */
/* ���e		:���O�C�����													 */
/* ���t		:2005/05/06														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowLOGIN(void)
{
	PCWindowLOGIN pWindowNew;

	pWindowNew = new CWindowLOGIN;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowMSG										 */
/* ���e		:���b�Z�[�W�\��													 */
/* ���t		:2005/06/11														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowMSG(LPCSTR pszMsg, DWORD dwTime, int nType)
{
	PCWindowMSG pWindowNew;
	PCWindowBase pWindowTmp;

	pWindowTmp = GetWindow (WINDOWTYPE_MSG);
	if (pWindowTmp) {
		pWindowNew = (PCWindowMSG)pWindowTmp;

	} else {
		pWindowNew = new CWindowMSG;
		pWindowNew->Create (m_pMgrData);
		m_paWindow->Add (pWindowNew);
	}

	pWindowNew->SetMsg (pszMsg, dwTime, nType);
	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowLOGINMENU								 */
/* ���e		:���j���[														 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowLOGINMENU(void)
{
	PCWindowLOGINMENU pWindowNew;

	pWindowNew = new CWindowLOGINMENU;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowCHARNAME									 */
/* ���e		:�L����������													 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowCHARNAME(void)
{
	PCWindowCHARNAME pWindowNew;

	pWindowNew = new CWindowCHARNAME;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowSEX										 */
/* ���e		:����															 */
/* ���t		:2006/11/14														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSEX(void)
{
	PCWindowSEX pWindowNew;

	pWindowNew = new CWindowSEX;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowACCOUNTINFO								 */
/* ���e		:�A�J�E���g���													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowACCOUNTINFO(void)
{
	PCWindowACCOUNTINFO pWindowNew;

	pWindowNew = new CWindowACCOUNTINFO;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowSTATUS									 */
/* ���e		:����															 */
/* ���t		:2005/06/26														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSTATUS(BOOL bShow)
{
	PCWindowSTATUS pWindowNew;

	pWindowNew = new CWindowSTATUS;
	pWindowNew->Create (m_pMgrData);
	pWindowNew->SetShow (bShow);
	m_paWindow->Add (pWindowNew);
	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowCHAT										 */
/* ���e		:�`���b�g����													 */
/* ���t		:2007/02/03														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowCHAT(void)
{
	PCWindowCHAT pWindowNew;

	pWindowNew = new CWindowCHAT;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowFAMILYTYPE								 */
/* ���e		:�푰�I��														 */
/* ���t		:2007/04/07														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowFAMILYTYPE(void)
{
	PCWindowFAMILYTYPE pWindowNew;

	pWindowNew = new CWindowFAMILYTYPE;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowHAIRTYPE									 */
/* ���e		:���^�I��														 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowHAIRTYPE(void)
{
	PCWindowHAIRTYPE pWindowNew;

	pWindowNew = new CWindowHAIRTYPE;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowHAIRCOLOR								 */
/* ���e		:���F�I��														 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowHAIRCOLOR(void)
{
	PCWindowHAIRCOLOR pWindowNew;

	pWindowNew = new CWindowHAIRCOLOR;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowEYECOLOR									 */
/* ���e		:�ڐF�I��														 */
/* ���t		:2007/04/08														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowEYECOLOR(void)
{
	PCWindowEYECOLOR pWindowNew;

	pWindowNew = new CWindowEYECOLOR;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowSTYLESELECT								 */
/* ���e		:�e�p�I��														 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSTYLESELECT(void)
{
	PCWindowSTYLESELECT pWindowNew;

	pWindowNew = new CWindowSTYLESELECT;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowNAMEINPUT								 */
/* ���e		:���O����														 */
/* ���t		:2007/04/10														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowNAMEINPUT(void)
{
	PCWindowNAMEINPUT pWindowNew;

	pWindowNew = new CWindowNAMEINPUT;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowSYSTEMMENU								 */
/* ���e		:�V�X�e�����j���[												 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSYSTEMMENU(void)
{
	PCWindowSYSTEMMENU pWindowNew;

	pWindowNew = new CWindowSYSTEMMENU;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowSETCOLOR									 */
/* ���e		:���O�Ɣ����F�̐ݒ�												 */
/* ���t		:2007/06/20														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSETCOLOR(void)
{
	PCWindowSETCOLOR pWindowNew;

	pWindowNew = new CWindowSETCOLOR;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowSETBGMVOLUME								 */
/* ���e		:BGM���ʂ̐ݒ�													 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSETBGMVOLUME(void)
{
	PCWindowSETBGMVOLUME pWindowNew;

	pWindowNew = new CWindowSETBGMVOLUME;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowSETSOUNDVOLUME							 */
/* ���e		:���ʉ����ʂ̐ݒ�												 */
/* ���t		:2007/06/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSETSOUNDVOLUME(void)
{
	PCWindowSETSOUNDVOLUME pWindowNew;

	pWindowNew = new CWindowSETSOUNDVOLUME;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowSETDRAWMODE								 */
/* ���e		:�\�����e�̐ݒ�													 */
/* ���t		:2007/07/24														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowSETDRAWMODE(void)
{
	PCWindowSETDRAWMODE pWindowNew;

	pWindowNew = new CWindowSETDRAWMODE;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowITEMMENU									 */
/* ���e		:�A�C�e�����j���[												 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowITEMMENU(void)
{
	PCWindowITEMMENU pWindowNew;

	pWindowNew = new CWindowITEMMENU;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowITEMMENU_SELECT							 */
/* ���e		:�A�C�e���ǂ����邩���j���[										 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowITEMMENU_SELECT(int nPos, DWORD dwItemID)
{
	PCWindowITEMMENU_SELECT pWindowNew;

	pWindowNew = new CWindowITEMMENU_SELECT;
	pWindowNew->Create (m_pMgrData);
	pWindowNew->SetItemID (nPos, dwItemID);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowOPTION									 */
/* ���e		:�I�v�V����														 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION(void)
{
	PCWindowOPTION pWindowNew;

	pWindowNew = new CWindowOPTION;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowOPTION_VIEWSET							 */
/* ���e		:�I�v�V����-�\���ݒ�											 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_VIEWSET(void)
{
	PCWindowOPTION_VIEWSET pWindowNew;

	pWindowNew = new CWindowOPTION_VIEWSET;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowOPTION_VOLUMESET							 */
/* ���e		:�I�v�V����-���ʐݒ�											 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_VOLUMESET(void)
{
	PCWindowOPTION_VOLUMESET pWindowNew;

	pWindowNew = new CWindowOPTION_VOLUMESET;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowOPTION_INPUTSET							 */
/* ���e		:�I�v�V����-���͐ݒ�											 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_INPUTSET(void)
{
	PCWindowOPTION_INPUTSET pWindowNew;

	pWindowNew = new CWindowOPTION_INPUTSET;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowOPTION_TASKBAR							 */
/* ���e		:�I�v�V����(�������Ƀ^�X�N�o�[�`�J�`�J)							 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_TASKBAR(void)
{
	PCWindowOPTION_TASKBAR pWindowNew;

	pWindowNew = new CWindowOPTION_TASKBAR;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowOPTION_INPUTSET_SETDEVICE				 */
/* ���e		:�I�v�V����-���͐ݒ�-���̓f�o�C�X�̐ݒ�							 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_INPUTSET_SETDEVICE(void)
{
	PCWindowOPTION_INPUTSET_SETDEVICE pWindowNew;

	pWindowNew = new CWindowOPTION_INPUTSET_SETDEVICE;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowOPTION_ACTIONSET							 */
/* ���e		:�I�v�V����-����ݒ�											 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_ACTIONSET(void)
{
	PCWindowOPTION_ACTIONSET pWindowNew;

	pWindowNew = new CWindowOPTION_ACTIONSET;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowOPTION_ACTIONSET_SLEEPTIMER				 */
/* ���e		:�I�v�V����-����ݒ�-���Ђ�˃^�C�}�[�̐ݒ�						 */
/* ���t		:2008/07/21														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowOPTION_ACTIONSET_SLEEPTIMER(void)
{
	PCWindowOPTION_ACTIONSET_SLEEPTIMER pWindowNew;

	pWindowNew = new CWindowOPTION_ACTIONSET_SLEEPTIMER;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowCOMMANDMENU								 */
/* ���e		:�R�}���h���j���[												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowCOMMANDMENU(void)
{
	PCWindowCOMMANDMENU pWindowNew;

	pWindowNew = new CWindowCOMMANDMENU;
	pWindowNew->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::MakeWindowCHAR_STATUS								 */
/* ���e		:�L����-�X�e�[�^�X												 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

void CMgrWindow::MakeWindowCHAR_STATUS(void)
{
	PCWindowCHAR_STATUS pWindowNew;
	PCWindowCHAR_STATUS2 pWindowNew2;
	PCWindowCHAR_STATUS3 pWindowNew3;

	pWindowNew2 = new CWindowCHAR_STATUS2;
	pWindowNew2->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew2);
	pWindowNew3 = new CWindowCHAR_STATUS3;
	pWindowNew3->Create (m_pMgrData);
	m_paWindow->Add (pWindowNew3);
	pWindowNew = new CWindowCHAR_STATUS;
	pWindowNew->Create (m_pMgrData);
	pWindowNew->AddChild (pWindowNew2);
	pWindowNew->AddChild (pWindowNew3);
	m_paWindow->Add (pWindowNew);
	SetActive ();

	m_bDraw = TRUE;
}


/* ========================================================================= */
/* �֐���	:CMgrWindow::SetActive											 */
/* ���e		:�A�N�e�B�u�ȃE�B���h�E�̐ݒ�									 */
/* ���t		:2005/06/16														 */
/* ========================================================================= */

void CMgrWindow::SetActive(void)
{
	int i, nCount;
	BOOL bSet;
	PCWindowBase pWindow;

	bSet		= FALSE;
	m_bKeyInput	= FALSE;
	nCount		= m_paWindow->GetSize ();
	if (nCount <= 0) {
		return;
	}

	for (i = nCount - 1; i >= 0; i --) {
		pWindow = m_paWindow->GetAt (i);
		if (pWindow->IsInput () == FALSE) {
			continue;
		}
		pWindow->SetActive (1 - bSet);
		bSet = TRUE;
	}

	m_bKeyInput = bSet;
}

/* Copyright(C)URARA-works 2006 */
