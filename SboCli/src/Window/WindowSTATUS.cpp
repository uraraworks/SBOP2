/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:WindowSTATUS.cpp											 */
/* ���e			:�����E�B���h�E�N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/06/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include <stdio.h>
#include "InfoCharCli.h"
#include "LibInfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "WindowSTATUS.h"


/* ========================================================================= */
/* �֐���	:CWindowSTATUS::CWindowSTATUS									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/06/26														 */
/* ========================================================================= */

CWindowSTATUS::CWindowSTATUS()
{
	m_nID			= WINDOWTYPE_STATUS;
	m_ptViewPos.x	= 16 + 16 * 18 - 8;
	m_ptViewPos.y	= 16;
	m_sizeWindow.cx	= 16 * 2 + 16 * 9;
	m_sizeWindow.cy	= 16 * 2 + 16 * 12;

	m_dwCharID = 0;
}


/* ========================================================================= */
/* �֐���	:CWindowSTATUS::~CWindowSTATUS									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/06/26														 */
/* ========================================================================= */

CWindowSTATUS::~CWindowSTATUS()
{
}


/* ========================================================================= */
/* �֐���	:CWindowSTATUS::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2007/03/21														 */
/* ========================================================================= */

void CWindowSTATUS::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (RGB (255, 0, 255));
}


/* ========================================================================= */
/* �֐���	:CWindowSTATUS::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2005/06/26														 */
/* ========================================================================= */

void CWindowSTATUS::Draw(PCImg32 pDst)
{
//	int i;
	HDC hDC;
	HFONT hFontOld;
//	char szTmp[128];
	PCInfoCharCli pChar;
	PCLibInfoCharCli pLibInfoChar;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
	pChar			= (PCInfoCharCli)pLibInfoChar->GetPtr (m_dwCharID);
	if (pChar == NULL) {
		return;
	}

	DrawFrame (2);

	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkColor (hDC, RGB (255, 255, 255));
#if 0
	for (i = 0; i < 2; i ++) {
		m_pDib->BltFrom256 (16, 32 + i * 16, 16, 16, m_pDibSystem, 224 + i * 16, 32, TRUE);
	}
	for (i = 0; i < 8; i ++) {
		m_pDib->BltFrom256 (16, 32 + 32 + i * 16, 16, 16, m_pDibSystem, 128 + i * 16, 48, TRUE);
	}
#endif

//	TextOut2 (hDC, 32, 0, "����", RGB (0, 0, 0));
	SetBkMode (hDC, TRANSPARENT);
	TextOut2 (hDC, 16 + 8 * 0, 16 * 1, "���x��", RGB (0, 0, 0));
#if 0
	TextOut2 (hDC, 32 + 8 * 9, 16 * 1, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_byLevel);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 1, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 2, "��", RGB (255, 255, 255));
	TextOut2 (hDC, 32 + 8 * 9, 16 * 2, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_byPW);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 2, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 3, "�f����", RGB (255, 255, 255));
	TextOut2 (hDC, 32 + 8 * 9, 16 * 3, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_byDEX);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 3, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 4, "�̗�", RGB (255, 255, 255));
	TextOut2 (hDC, 32 + 8 * 9, 16 * 4, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_byST);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 4, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 5, "����", RGB (255, 255, 255));
	TextOut2 (hDC, 32 + 8 * 9, 16 * 5, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_byIT);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 5, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 6, "�^�̂悳", RGB (255, 255, 255));
	TextOut2 (hDC, 32 + 8 * 9, 16 * 6, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_byLK);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 6, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 7, "�ő�HP", RGB (255, 255, 255));
	TextOut2 (hDC, 32 + 8 * 9, 16 * 7, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_dwMaxHP);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 7, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 8, "�ő�Mana", RGB (255, 255, 255));
	TextOut2 (hDC, 32 + 8 * 9, 16 * 8, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_dwMaxMana);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 8, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 9, "�U����", RGB (255, 255, 255));
	TextOut2 (hDC, 32 + 8 * 9, 16 * 9, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_byAT);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 9, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 10, "�h���", RGB (255, 255, 255));
	TextOut2 (hDC, 32 + 8 * 9, 16 * 10, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_byDF);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 10, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 11, "���@�h��", RGB (255, 255, 255));
	TextOut2 (hDC, 32 + 8 * 9, 16 * 11, ":", RGB (255, 255, 255));
	wsprintf (szTmp, "%d", pChar->m_byMagicDF);
	TextOut2 (hDC, 32 + 8 * 10, 16 * 11, szTmp, RGB (150, 255, 200));

	TextOut2 (hDC, 32 + 8 * 0, 16 * 12, "Exp:", RGB (255, 255, 255));
	sprintf (szTmp, "%I64d", pChar->m_ulExp);
	TextOut2 (hDC, 32 + 8 * 5, 16 * 12, szTmp, RGB (150, 255, 200));
#endif
	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->BltAlpha2 (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, 15, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowSTATUS::SetID											 */
/* ���e		:�L����ID��ݒ�													 */
/* ���t		:2005/07/09														 */
/* ========================================================================= */

void CWindowSTATUS::SetID(DWORD dwID)
{
	m_dwCharID			= dwID;
	m_dwTimeDrawStart	= 0;
}

/* Copyright(C)URARA-works 2005 */
