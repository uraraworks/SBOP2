/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowOPTION_VOLUMESET.cpp									 */
/* ���e			:�I�v�V����-�\���ݒ�E�B���h�E�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowOPTION_VOLUMESET.h"


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VOLUMESET::CWindowOPTION_VOLUMESET				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

CWindowOPTION_VOLUMESET::CWindowOPTION_VOLUMESET()
{
	m_nPosMax		= 1;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_OPTION_VOLUMESET;
	m_ptViewPos.x	= 8 * 13;
	m_ptViewPos.y	= 16 * 9;
	m_sizeWindow.cx	= 16 * 2 + 16 * 21;
	m_sizeWindow.cy	= 16 * 2 + 16 * 2;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VOLUMESET::~CWindowOPTION_VOLUMESET				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

CWindowOPTION_VOLUMESET::~CWindowOPTION_VOLUMESET()
{
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VOLUMESET::Create								 */
/* ���e		:�쐬															 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

void CWindowOPTION_VOLUMESET::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
	m_nPos = 0;

	m_nBackVolumeBgm	= m_nVolumeBgm		= m_pMgrData->GetBGMVolume ();
	m_nBackVolumeSound	= m_nVolumeSound	= m_pMgrData->GetSEVolume ();
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VOLUMESET::Draw									 */
/* ���e		:�`��															 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

void CWindowOPTION_VOLUMESET::Draw(PCImg32 pDst)
{
	int i, nLevel;
	HDC hDC;
	HFONT hFontOld;
	LPCSTR apszTmp[] = {"�O", "�P", "�Q", "�R", "�S", "MAX"};
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	TextOut2 (hDC, 32, 16 + 16 * 0, "BGM", clText);
	TextOut2 (hDC, 32, 16 + 16 * 1, "���ʉ�", clText);
	TextOut2 (hDC, 32 + 72, 16 + 16 * 0, "MIN", clText);
	TextOut2 (hDC, 32 + 72, 16 + 16 * 1, "MIN", clText);
	for (i = 0; i < 6; i ++) {
		TextOut2 (hDC, 32 + 72 + (16 * 3) + (i * 32), 16 + 16 * 0, apszTmp[i], clText);
		TextOut2 (hDC, 32 + 72 + (16 * 3) + (i * 32), 16 + 16 * 1, apszTmp[i], clText);
	}

	DrawCursor (32 + 24 + 72 + (m_nVolumeBgm * 32), 16 + 16 * 0);
	DrawCursor (32 + 24 + 72 + (m_nVolumeSound * 32), 16 + 16 * 1);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	DrawCursor (8, 16 + 16 * m_nPos);
	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VOLUMESET::OnUp									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

BOOL CWindowOPTION_VOLUMESET::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos <= 0) {
		goto Exit;
	}
	m_nPos --;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VOLUMESET::OnDown								 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

BOOL CWindowOPTION_VOLUMESET::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax) {
		goto Exit;
	}
	m_nPos ++;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VOLUMESET::OnLeft								 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

BOOL CWindowOPTION_VOLUMESET::OnLeft(void)
{
	switch (m_nPos) {
	case 0:		/* BGM */
		m_nVolumeBgm --;
		m_nVolumeBgm = max (m_nVolumeBgm, 0);
		m_pMgrData->SetBGMVolume (m_nVolumeBgm);
		m_pMgrSound->SetBGMVolume (m_nVolumeBgm);
		break;
	case 1:		/* ���ʉ� */
		m_nVolumeSound --;
		m_nVolumeSound = max (m_nVolumeSound, 0);
		m_pMgrData->SetSEVolume (m_nVolumeSound);
		m_pMgrSound->SetSEVolume (m_nVolumeSound);
		break;
	}

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VOLUMESET::OnRight								 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

BOOL CWindowOPTION_VOLUMESET::OnRight(void)
{
	switch (m_nPos) {
	case 0:		/* BGM */
		m_nVolumeBgm ++;
		m_nVolumeBgm = min (m_nVolumeBgm, 4);
		m_pMgrData->SetBGMVolume (m_nVolumeBgm);
		m_pMgrSound->SetBGMVolume (m_nVolumeBgm);
		break;
	case 1:		/* ���ʉ� */
		m_nVolumeSound ++;
		m_nVolumeSound = min (m_nVolumeSound, 4);
		m_pMgrData->SetSEVolume (m_nVolumeSound);
		m_pMgrSound->SetSEVolume (m_nVolumeSound);
		break;
	}

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VOLUMESET::OnX									 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

BOOL CWindowOPTION_VOLUMESET::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, m_nPos);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VOLUMESET::OnZ									 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

BOOL CWindowOPTION_VOLUMESET::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_CANCEL);

	m_pMgrData->SetBGMVolume (m_nBackVolumeBgm);
	m_pMgrSound->SetBGMVolume (m_nBackVolumeBgm);
	m_pMgrData->SetSEVolume (m_nBackVolumeSound);
	m_pMgrSound->SetSEVolume (m_nBackVolumeSound);

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2008 */
