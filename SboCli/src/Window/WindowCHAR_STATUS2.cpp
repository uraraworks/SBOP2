/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowCHAR_STATUS2.cpp										 */
/* ���e			:�L����-�X�e�[�^�X2�E�B���h�E�N���X �����t�@�C��			 */
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
#include "WindowCHAR_STATUS2.h"


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS2::CWindowCHAR_STATUS2						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

CWindowCHAR_STATUS2::CWindowCHAR_STATUS2()
{
	m_nPosMax		= 1;
	m_nID			= WINDOWTYPE_CHAR_STATUS2;
	m_ptViewPos.x	= 8 * 2;
	m_ptViewPos.y	= 16 * 3 + 16 * 2 + 12 * 3;
	m_sizeWindow.cx	= 16 * 2 + 12 * 10;
	m_sizeWindow.cy	= 16 * 2 + 12 * 16;
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS2::~CWindowCHAR_STATUS2						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

CWindowCHAR_STATUS2::~CWindowCHAR_STATUS2()
{
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS2::Create									 */
/* ���e		:�쐬															 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

void CWindowCHAR_STATUS2::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
}


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS2::Draw										 */
/* ���e		:�`��															 */
/* ���t		:2008/10/27														 */
/* ========================================================================= */

void CWindowCHAR_STATUS2::Draw(PCImg32 pDst)
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
	strTmp = "�X�^�~�i";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wStamina);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "�r��";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wPower);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "�̗�";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wStrength);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "����";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wMagic);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "��p";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wSkillful);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "�U���Z�\";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wAbillityAT);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "�h��Z�\";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wAbillityDF);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "�U����";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wPAtack);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "�h���";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wPDefense);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "���@��";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wPMagic);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "���@�h���";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wPMagicDefense);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "������";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wPHitAverage);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "���";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wPAvoidAverage);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "�N���e�B�J����";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_wPCriticalAverage);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "�ő�HP";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_dwMaxHP);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp = "�ő�SP";
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);
	strTmp.Format ("%d", pInfoChar->m_dwMaxSP);
	TextOut2 (hDC, 16 + 12 * 7, 12 + 12 * y, (LPCSTR)strTmp, clText);
	y ++;
	strTmp.Format ("Exp  %ld", pInfoChar->m_dwExp);
	TextOut2 (hDC, 16, 12 + 12 * y, (LPCSTR)strTmp, clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}

/* Copyright(C)URARA-works 2008 */
