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
	m_sizeWindow.cx	= 16 * 2 + 8 * 22;
	m_sizeWindow.cy	= 16 * 2 + 8 * 41;
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

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
	SetBkMode (hDC, TRANSPARENT);

	DrawFrame (5);
	DrawFrame (12, 7, 80, 24, 7);
	TextOut2 (hDC, 12 + 5, 7 + 4, "�L�������(J)", RGB (255, 255, 255));
	y = 8 * 3;
	DrawFrame (4, y, 200, 64, 6);
	y += 6;
	x = 12;
	DrawFrame (x, y, 184, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "���O", RGB (255, 255, 255));
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)pInfoChar->m_strCharName, RGB (1, 1, 1));
	y += 18;
	DrawFrame (x, y, 120, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "�E��", RGB (255, 255, 255));
	x = 12 + 8 * 16;
	DrawFrame (x, y, 56, 16, 6);
	DrawFrame (x, y, 32, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "LV", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wLevel);
	TextOut2 (hDC, x + 27, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 184, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "����", RGB (255, 255, 255));

	y = 8 * 11 + 7;
	x = 12;
	DrawFrame (x, y, 48, 24, 7);
	TextOut2 (hDC, x + 5, y + 4, "��{�l", RGB (255, 255, 255));
	y += 16;
	DrawFrame (4, y, 200, 64, 6);
	y += 6;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 3, y + 2, "HP", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_dwMaxHP);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 3, y + 2, "MP", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_dwMaxSP);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "��", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPower);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "����", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wMagic);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "�̗�", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wStrength);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "��p", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wSkillful);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));

	y += 8 * 4;
	x = 12;
	DrawFrame (x, y, 64, 24, 7);
	TextOut2 (hDC, x + 5, y + 4, "�X�e�[�^�X", RGB (255, 255, 255));
	y += 16;
	DrawFrame (4, y, 200, 152, 6);
	y += 6;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "�U��", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPAtack);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "�h��", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPDefense);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "�U�p", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAbillityAT);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "�h�p", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAbillityDF);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "���U", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPMagic);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "���h", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPMagicDefense);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "����", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPHitAverage);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "���", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPAvoidAverage);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5, y + 2, "�K�E", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wPCriticalAverage);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "��", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrFire);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "��", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrWater);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "��", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrWind);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "�y", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrEarth);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	y += 18;
	x = 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "��", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrLight);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));
	x = 12 + 8 * 12;
	DrawFrame (x, y, 8 * 11, 16, 6);
	DrawFrame (x, y, 40, 16, 7, TRUE);
	TextOut2 (hDC, x + 5 + 6, y + 2, "��", RGB (255, 255, 255));
	strTmp.Format("%d", pInfoChar->m_wAttrDark);
	TextOut2 (hDC, x + 35, y + 2, (LPCSTR)strTmp, RGB (1, 1, 1));



#if 0
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
#endif

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


/* ========================================================================= */
/* �֐���	:CWindowCHAR_STATUS::OnJ										 */
/* ���e		:�L�[�n���h��(J)												 */
/* ���t		:2009/04/02														 */
/* ========================================================================= */

BOOL CWindowCHAR_STATUS::OnJ(BOOL bDown)
{
	return OnZ (bDown);
}

/* Copyright(C)URARA-works 2008 */
