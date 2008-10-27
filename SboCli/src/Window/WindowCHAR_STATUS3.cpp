/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowCHAR_STATUS3.cpp										 */
/* ���e			:�L����-�X�e�[�^�X3�E�B���h�E�N���X �����t�@�C��			 */
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
#include "WindowCHAR_STATUS3.h"


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS3::CWindowCHAR_STATUS3						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

CWindowCHAR_STATUS3::CWindowCHAR_STATUS3()
{
	m_nPosMax		= 1;
	m_nID			= WINDOWTYPE_CHAR_STATUS3;
	m_ptViewPos.x	= 8 * 2;
	m_ptViewPos.y	= 16 * 3 + 16 * 2 + 12 * 3 + 16 * 2 + 12 * 16;
	m_sizeWindow.cx	= 16 * 2 + 12 * 20;
	m_sizeWindow.cy	= 16 * 2 + 12 * 1;
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS3::~CWindowCHAR_STATUS3						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

CWindowCHAR_STATUS3::~CWindowCHAR_STATUS3()
{
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS3::Create									 */
/* ���e		:�쐬															 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

void CWindowCHAR_STATUS3::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS3::Draw										 */
/* ���e		:�`��															 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

void CWindowCHAR_STATUS3::Draw(PCImg32 pDst)
{
	int nLevel, x, y;
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

	x = y = 0;
	strTmp = "����[��]";
	TextOut2 (hDC, 16 + 12 * 7 * x, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%3d%%", pInfoChar->m_wAttrFire);
	TextOut2 (hDC, 16 + 12 * 7 * x + 12 * 4, 12 + 12 * y, (LPCSTR)strTmp, clText);
	x ++;
	strTmp = "����[��]";
	TextOut2 (hDC, 16 + 12 * 7 * x, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%3d%%", pInfoChar->m_wAttrWind);
	TextOut2 (hDC, 16 + 12 * 7 * x + 12 * 4, 12 + 12 * y, (LPCSTR)strTmp, clText);
	x ++;
	strTmp = "����[��]";
	TextOut2 (hDC, 16 + 12 * 7 * x, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%3d%%", pInfoChar->m_wAttrWater);
	TextOut2 (hDC, 16 + 12 * 7 * x + 12 * 4, 12 + 12 * y, (LPCSTR)strTmp, clText);
	x = 0;
	y ++;
	strTmp = "����[�y]";
	TextOut2 (hDC, 16 + 12 * 7 * x, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%3d%%", pInfoChar->m_wAttrEarth);
	TextOut2 (hDC, 16 + 12 * 7 * x + 12 * 4, 12 + 12 * y, (LPCSTR)strTmp, clText);
	x ++;
	strTmp = "����[��]";
	TextOut2 (hDC, 16 + 12 * 7 * x, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%3d%%", pInfoChar->m_wAttrLight);
	TextOut2 (hDC, 16 + 12 * 7 * x + 12 * 4, 12 + 12 * y, (LPCSTR)strTmp, clText);
	x ++;
	strTmp = "����[��]";
	TextOut2 (hDC, 16 + 12 * 7 * x, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%3d%%", pInfoChar->m_wAttrDark);
	TextOut2 (hDC, 16 + 12 * 7 * x + 12 * 4, 12 + 12 * y, (LPCSTR)strTmp, clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}

/* Copyright(C)URARA-works 2008 */
