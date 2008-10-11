/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowOPTION_VIEWSET.cpp									 */
/* ���e			:�I�v�V����-�\���ݒ�E�B���h�E�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowOPTION_VIEWSET.h"


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VIEWSET::CWindowOPTION_VIEWSET					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

CWindowOPTION_VIEWSET::CWindowOPTION_VIEWSET()
{
	m_nPosMax		= 5;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_OPTION_VIEWSET;
	m_ptViewPos.x	= 8 * 13;
	m_ptViewPos.y	= 16 * 8;
	m_sizeWindow.cx	= 16 * 2 + 16 * 16 + 8;
	m_sizeWindow.cy	= 16 * 2 + 16 * 6;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VIEWSET::~CWindowOPTION_VIEWSET					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

CWindowOPTION_VIEWSET::~CWindowOPTION_VIEWSET()
{
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VIEWSET::Create									 */
/* ���e		:�쐬															 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

void CWindowOPTION_VIEWSET::Create(CMgrData *pMgrData)
{
	BOOL bResult;
	int i, nCount;

	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
	m_nPos = 0;

	nCount = m_nPosMax + 1;
	for (i = 0; i < nCount; i ++) {
		bResult = GetCheck (i);
		m_anCheck.Add ((bResult) ? 464 : 400);
		m_adwCheckTime.Add (0);
	}
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VIEWSET::Draw									 */
/* ���e		:�`��															 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

void CWindowOPTION_VIEWSET::Draw(PCImg32 pDst)
{
	int nLevel, i, nCount;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	TextOut2 (hDC, 32 + 24, 16 + 16 * 0, "�������Ƀ^�X�N�o�[�`�J�`�J",	clText);
	TextOut2 (hDC, 32 + 24, 16 + 16 * 1, "���O��\������",				clText);
	TextOut2 (hDC, 32 + 24, 16 + 16 * 2, "������\������",				clText);
	TextOut2 (hDC, 32 + 24, 16 + 16 * 3, "�w���v�A�C�R����\������",	clText);
	TextOut2 (hDC, 32 + 24, 16 + 16 * 4, "�퓬���b�Z�[�W�����O�Ɏc��",	clText);
	TextOut2 (hDC, 32 + 24, 16 + 16 * 5, "60�t���[���ŕ\������",		clText);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	nCount = m_adwCheckTime.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_pDib->BltFrom256 (32, 16 + 16 * i, 16, 16, m_pDibSystem, m_anCheck[i], 0, TRUE);
	}

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
/* �֐���	:CWindowOPTION_VIEWSET::TimerProc								 */
/* ���e		:���ԏ���														 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

BOOL CWindowOPTION_VIEWSET::TimerProc(void)
{
	BOOL bRet;
	int i, nCount;
	DWORD dwTime;

	bRet = CWindowBase::TimerProc ();

	dwTime = timeGetTime ();
	nCount = m_adwCheckTime.GetSize ();
	for (i = 0; i < nCount; i ++) {
		if (m_adwCheckTime[i] == 0) {
			continue;
		}
		if (dwTime - m_adwCheckTime[i] > 40) {
			m_adwCheckTime[i] = dwTime;
			m_anCheck[i] += 16;
			if (m_anCheck[i] >= 464) {
				m_adwCheckTime[i] = 0;
			}
			Redraw ();
		}
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VIEWSET::OnUp									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

BOOL CWindowOPTION_VIEWSET::OnUp(void)
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
/* �֐���	:CWindowOPTION_VIEWSET::OnDown									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

BOOL CWindowOPTION_VIEWSET::OnDown(void)
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
/* �֐���	:CWindowOPTION_VIEWSET::OnLeft									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

BOOL CWindowOPTION_VIEWSET::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VIEWSET::OnRight									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

BOOL CWindowOPTION_VIEWSET::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VIEWSET::OnX										 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

BOOL CWindowOPTION_VIEWSET::OnX(BOOL bDown)
{
	BOOL bRet, bOn;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, m_nPos);

	bOn = GetCheck (m_nPos);

	m_anCheck[m_nPos] = 400;
	if (bOn == FALSE) {
		m_adwCheckTime[m_nPos] = timeGetTime ();
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowOPTION_VIEWSET::OnZ										 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2008/06/27														 */
/* ========================================================================= */

BOOL CWindowOPTION_VIEWSET::OnZ(BOOL bDown)
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
/* �֐���	:CWindowOPTION_VIEWSET::GetCheck								 */
/* ���e		:�`�F�b�N��Ԃ��擾												 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

BOOL CWindowOPTION_VIEWSET::GetCheck(int nNo)
{
	BOOL bRet;

	bRet = FALSE;
	switch (nNo) {
	case 0:
		bRet = m_pMgrData->GetOptionTaskbar ();
		break;
	case 1:
		bRet = m_pMgrData->GetDrawMode ();
		break;
	case 2:
		bRet = m_pMgrData->GetOptionViewChat ();
		break;
	case 3:
		bRet = m_pMgrData->GetOptionViewHelpIcon ();
		break;
	case 4:
		bRet = m_pMgrData->GetOptionBattleMsgLog ();
		break;
	case 5:
		bRet = m_pMgrData->GetOption60Frame ();
		break;
	}

	return bRet;
}

/* Copyright(C)URARA-works 2008 */
