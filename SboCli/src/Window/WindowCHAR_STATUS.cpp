/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowCHAR_STATUS.cpp										 */
/* ���e			:�L����-�X�e�[�^�X�E�B���h�E�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowCHAR_STATUS.h"


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS::CWindowCHAR_STATUS							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

CWindowCHAR_STATUS::CWindowCHAR_STATUS()
{
	m_nPosMax		= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_CHAR_STATUS;
	m_ptViewPos.x	= 8 * 2;
	m_ptViewPos.y	= 16 * 3;
	m_sizeWindow.cx	= 16 * 2 + 12 * 10;
	m_sizeWindow.cy	= 16 * 2 + 12 * 3;
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS::~CWindowCHAR_STATUS						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

CWindowCHAR_STATUS::~CWindowCHAR_STATUS()
{
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

void CWindowCHAR_STATUS::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS::Draw										 */
/* ���e		:�`��															 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

void CWindowCHAR_STATUS::Draw(PCImg32 pDst)
{
	int nLevel, y;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	PCInfoCharCli pInfoChar;
	CmyString strTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	pInfoChar = m_pMgrData->GetPlayerChar ();
	DrawFrame ();

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
	SetBkMode (hDC, TRANSPARENT);

	clText = RGB (1, 1, 1);

	y = 0;
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)pInfoChar->m_strCharName, clText);
	y ++;
	strTmp = "���x��";
	TextOut2 (hDC, 16, 16 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wLevel);
	TextOut2 (hDC, 16 + 12 * 7, 16 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "HP";
	TextOut2 (hDC, 16, 16 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_dwHP);
	TextOut2 (hDC, 16 + 12 * 7, 16 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "SP";
	TextOut2 (hDC, 16, 16 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_dwSP);
	TextOut2 (hDC, 16 + 12 * 7, 16 + 12 * y, (LPCSTR)strTmp, clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS::OnX										 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

BOOL CWindowCHAR_STATUS::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS::OnZ										 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

BOOL CWindowCHAR_STATUS::OnZ(BOOL bDown)
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

/* Copyright(C)URARA-works 2008 */
