/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:WindowACCOUNTINFO.cpp										 */
/* ���e			:�A�J�E���g���E�B���h�E�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoAccount.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "WindowACCOUNTINFO.h"


/* ========================================================================= */
/* �֐����F	CWindowACCOUNTINFO::CWindowACCOUNTINFO							 */
/* ���e�F	�R���X�g���N�^													 */
/* ���t�F	2006/12/29														 */
/* ========================================================================= */

CWindowACCOUNTINFO::CWindowACCOUNTINFO()
{
	m_nPos			= 0;
	m_nPosMax		= 2;
	m_nID			= WINDOWTYPE_ACCOUNTINFO;
	m_ptViewPos.x	= 280;
	m_ptViewPos.y	= 48;
	m_sizeWindow.cx	= 16 * 2 + 16 * 10;
	m_sizeWindow.cy	= 16 * 2 + 16 * 4;
}


/* ========================================================================= */
/* �֐����F	CWindowACCOUNTINFO::~CWindowACCOUNTINFO							 */
/* ���e�F	�f�X�g���N�^													 */
/* ���t�F	2006/12/29														 */
/* ========================================================================= */

CWindowACCOUNTINFO::~CWindowACCOUNTINFO()
{
}


/* ========================================================================= */
/* �֐���	:CWindowACCOUNTINFO::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2006/12/29														 */
/* ========================================================================= */

void CWindowACCOUNTINFO::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (RGB (40, 40, 40));
}


/* ========================================================================= */
/* �֐����F	CWindowACCOUNTINFO::Draw										 */
/* ���e�F	�`��															 */
/* ���t�F	2006/12/29														 */
/* ========================================================================= */

void CWindowACCOUNTINFO::Draw(PCImg32 pDst)
{
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	CmyString strTmp;
	PCInfoAccount pInfoAccount;
	PCInfoCharCli pInfoChar;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	pInfoAccount	= m_pMgrData->GetAccount ();
	pInfoChar		= m_pMgrData->GetPlayerChar ();
	if (pInfoChar == NULL) {
		goto Exit;
	}

	clText		= RGB (0, 0, 0);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	strTmp.Format ("�L������:%s", (LPCSTR)pInfoChar->m_strCharName);
	TextOut2 (hDC, 16, 16 + 16 * 0, strTmp,	clText);
	strTmp.Format ("�A�J�E���g�쐬����:");
	TextOut2 (hDC, 16, 16 + 16 * 1, strTmp,	clText);
	TextOut2 (hDC, 16, 16 + 16 * 2, "�O�񃍃O�C������:",	clText);
	TextOut2 (hDC, 16, 16 + 16 * 3, "���O�C����:",		clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}

/* Copyright(C)URARA-works 2006 */
