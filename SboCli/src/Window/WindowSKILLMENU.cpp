/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowSKILLMENU.cpp										 */
/* ���e			:�X�L�����j���[�E�B���h�E�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/31													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoSkill.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowSKILLMENU.h"

#define MENUPOSY	(51)	/* ���j���[�{�̕\���ʒu(�c) */

/* ========================================================================= */
/* �֐���	:CWindowSKILLMENU::CWindowSKILLMENU								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

CWindowSKILLMENU::CWindowSKILLMENU()
{
	m_nPos			= 12;
	m_nPosMax		= 24;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_SKILLMENU;
	m_ptViewPos.x	= 24;
	m_ptViewPos.y	= 112;
	m_sizeWindow.cx	= 208;
	m_sizeWindow.cy	= 280;

	m_nType			= 0;
	m_dwSelectID	= 0;
	m_pPlayerChar	= NULL;
	m_pLibInfoSkill	= NULL;
}


/* ========================================================================= */
/* �֐���	:CWindowSKILLMENU::~CWindowSKILLMENU							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

CWindowSKILLMENU::~CWindowSKILLMENU()
{
}


/* ========================================================================= */
/* �֐���	:CWindowSKILLMENU::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

void CWindowSKILLMENU::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_pPlayerChar	= m_pMgrData->GetPlayerChar ();
	m_pLibInfoSkill	= m_pMgrData->GetLibInfoSkill ();
}


/* ========================================================================= */
/* �֐���	:CWindowSKILLMENU::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

void CWindowSKILLMENU::Draw(PCImg32 pDst)
{
	LPCSTR aszTitle[] = {"��{(K)", "�퓬(F)", "����(L)", "�\��"};
	int i, nCount, nLevel, x, y;
	HDC hDC;
	HFONT hFontOld;
	PCInfoSkillBase pInfoSkill;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_strName.Empty ();
	DrawFrame (5);

	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
	SetBkMode (hDC, TRANSPARENT);
	for (i = 0; i < 4; i ++) {
		DrawFrame (12 + 48 * i, 7, 40 + 6, 24, 7);
		TextOut2 (hDC, 12 + 5 + 48 * i, 7 + 4, aszTitle[i], RGB (255, 255, 255));
	}
	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	DrawFrame (4, 23, 200, 248, 6);

	for (y = 0; y < 5; y ++) {
		for (x = 0; x < 5; x ++) {
			DrawIconFrame (12 + 36 * x, 32 + 47 * y);
		}
	}

	/* �X�L����`�� */
	nCount = m_adwSkillID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoSkill = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (m_adwSkillID[i]);
		if (pInfoSkill == NULL) {
			continue;
		}
		GetDrawPos (i, x, y);
//Todo:�Ƃ肠�����ނ�A�C�R������
		m_pMgrDraw->DrawIcon (m_pDib, x, y, 2);
		if (i == m_nPos) {
			m_strName = (LPCSTR)pInfoSkill->m_strName;
		}
		break;
	}

	GetDrawPos (m_nPos, x, y);
	m_pMgrDraw->DrawCursor (m_pDib, x - 8, y, 2);
	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);

	/* �X�L������\�� */
	if (m_strName.IsEmpty () == FALSE) {
		hDC			= pDst->Lock ();
		hFontOld	= (HFONT)SelectObject (hDC, m_hFont12);
		SetBkMode (hDC, TRANSPARENT);

		GetDrawPos (m_nPos, x, y);
		x = m_ptViewPos.x + 32 + x - 8;
		y = m_ptViewPos.y + 32 + y - 24;
		DrawFrame2 (x, y, m_strName.GetLength () * 6, 16, 0, pDst, 4);
		TextOut2 (hDC, x, y, (LPCSTR)m_strName, RGB (10, 10, 10), FALSE);

		SelectObject (hDC, hFontOld);
		pDst->Unlock ();
	}
}


/* ========================================================================= */
/* �֐���	:CWindowSKILLMENU::SetType										 */
/* ���e		:�X�L����ʂ�ݒ�												 */
/* ���t		:2009/01/11														 */
/* ========================================================================= */

void CWindowSKILLMENU::SetType(int nType)
{
	int i, nCount;
	DWORD dwSkillID;
	PARRAYDWORD paSkillID;
	PCInfoSkillBase pInfoSkill;
	int anSkillType[] = {SKILLTYPEMAIN_BATTLE, SKILLTYPEMAIN_LIFE, SKILLTYPEMAIN_NONE};

	m_nType = nType;
	m_adwSkillID.RemoveAll ();

	/* �X�L����`�� */
	paSkillID = m_pPlayerChar->GetSkill ();
	nCount = paSkillID->GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwSkillID = paSkillID->GetAt (i);
		pInfoSkill = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr (dwSkillID);
		if (pInfoSkill == NULL) {
			continue;
		}
		if (anSkillType[m_nType] != pInfoSkill->m_nTypeMain) {
			continue;
		}
		m_adwSkillID.Add (dwSkillID);
	}
}


/* ========================================================================= */
/* �֐���	:CWindowSKILLMENU::OnUp											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos < 5) {
		goto Exit;
	}
	m_nPos -= 5;

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowSKILLMENU::OnDown										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos >= m_nPosMax - 4) {
		goto Exit;
	}
	m_nPos += 5;

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowSKILLMENU::OnLeft										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnLeft(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos % 5 == 0) {
		if (m_nType == 0) {
			goto Exit;
		}
		SetType (m_nType -1);
		m_nPos += 5;
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
/* �֐���	:CWindowSKILLMENU::OnRight										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnRight(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos % 5 == 4) {
		if (m_nType >= 2) {
			goto Exit;
		}
		SetType (m_nType + 1);
		m_nPos -= 5;
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
/* �֐���	:CWindowSKILLMENU::OnS											 */
/* ���e		:�L�[�n���h��(S)												 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnS(BOOL bDown)
{
	return OnZ (bDown);
}


/* ========================================================================= */
/* �֐���	:CWindowSKILLMENU::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnX(BOOL bDown)
{
//	int i, nCount;
	POINT ptPos;
	BOOL bRet;
	DWORD dwSkillID;

	bRet		 = FALSE;
	dwSkillID	 = 0;

	ptPos.x = m_nPos % 5;
	ptPos.y = m_nPos / 5;

	if (bDown) {
		goto Exit;
	}

	if (m_nPos < m_adwSkillID.GetSize ()) {
		dwSkillID = m_adwSkillID[m_nPos];
	}
	if (dwSkillID == 0) {
		goto Exit;
	}

	m_dwSelectID = dwSkillID;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, dwSkillID);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowSKILLMENU::OnZ											 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

BOOL CWindowSKILLMENU::OnZ(BOOL bDown)
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
/* �֐���	:CWindowSKILLMENU::GetDrawPos									 */
/* ���e		:�X�L���`��ʒu���擾											 */
/* ���t		:2008/11/19														 */
/* ========================================================================= */

void CWindowSKILLMENU::GetDrawPos(
	int nPos,		/* [in] �X�L���ʒu */
	int &nDstX,		/* [out] X���W */
	int &nDstY)		/* [out] Y���W */
{
	nDstX = 13 + 36 * (nPos % 5);
	nDstY = 33 + 47 * (nPos / 5);
}

/* Copyright(C)URARA-works 2008 */
