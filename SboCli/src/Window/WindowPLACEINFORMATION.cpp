/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:WindowPLACEINFORMATION.cpp									 */
/* ���e			:�ꏊ���E�B���h�E�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/02/11													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "InfoMapBase.h"
#include "InfoCharCli.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WindowPLACEINFORMATION.h"

/* ========================================================================= */
/* �֐���	:CWindowPLACEINFORMATION::CWindowPLACEINFORMATION				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/02/11														 */
/* ========================================================================= */

CWindowPLACEINFORMATION::CWindowPLACEINFORMATION()
{
	m_nID			= WINDOWTYPE_PLACEINFORMATION;
	m_ptViewPos.x	= 0;
	m_ptViewPos.y	= 0;
	m_sizeWindow.cx	= 480;
	m_sizeWindow.cy	= 23;
}


/* ========================================================================= */
/* �֐���	:CWindowPLACEINFORMATION::~CWindowPLACEINFORMATION				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/02/11														 */
/* ========================================================================= */

CWindowPLACEINFORMATION::~CWindowPLACEINFORMATION()
{
}


/* ========================================================================= */
/* �֐���	:CWindowPLACEINFORMATION::Create								 */
/* ���e		:�쐬															 */
/* ���t		:2009/02/11														 */
/* ========================================================================= */

void CWindowPLACEINFORMATION::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
}


/* ========================================================================= */
/* �֐���	:CWindowPLACEINFORMATION::Draw									 */
/* ���e		:�`��															 */
/* ���t		:2009/02/11														 */
/* ========================================================================= */

void CWindowPLACEINFORMATION::Draw(PCImg32 pDst)
{
	int nTmp;
	float fTmp;
	HDC hDC;
	HFONT hFontOld;
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pInfoChar;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}
	pInfoMap = m_pMgrData->GetMap ();
	if (pInfoMap == NULL) {
		goto Exit;
	}
	pInfoChar = m_pMgrData->GetPlayerChar ();
	if (pInfoChar == NULL) {
		goto Exit;
	}

	m_pDib->BltFrom256 (0, 0, m_sizeWindow.cx, m_sizeWindow.cy, m_pDibSystem, 0, 624, TRUE);

	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
	SetBkMode (hDC, TRANSPARENT);

	TextOut2 (hDC, 34, 6, (LPCSTR)pInfoMap->m_strMapName, RGB (1, 1, 1));

	fTmp = (float)pInfoChar->m_dwHP * 100.0f / (float)pInfoChar->m_dwMaxHP;
	nTmp = (int)((float)65 * fTmp / 100.0f);
	m_pDib->BltFrom256 (216, 13, nTmp, 7, m_pDibSystem, 0, 758, TRUE);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();
Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}

/* Copyright(C)URARA-works 2009 */
