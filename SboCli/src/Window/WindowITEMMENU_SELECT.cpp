/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowITEMMENU_SELECT.cpp									 */
/* ���e			:�A�C�e���ǂ����邩���j���[�E�B���h�E�N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/13													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoItem.h"
#include "LibInfoItemType.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowITEMMENU_SELECT.h"


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU_SELECT::CWindowITEMMENU_SELECT					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

CWindowITEMMENU_SELECT::CWindowITEMMENU_SELECT()
{
	m_nPosMax		= 0;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_ITEMMENU_SELECT;
	m_ptViewPos.x	= 80;
	m_ptViewPos.y	= 112;
	m_sizeWindow.cx	= 16 * 2 + 16 * 6;
	m_sizeWindow.cy	= 16 * 2 + 16 * 1;

	m_nType			= -1;
	m_pInfoItem		= NULL;
	m_pLibInfoItem	= NULL;
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU_SELECT::~CWindowITEMMENU_SELECT				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

CWindowITEMMENU_SELECT::~CWindowITEMMENU_SELECT()
{
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU_SELECT::Create									 */
/* ���e		:�쐬															 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

void CWindowITEMMENU_SELECT::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_pLibInfoItem = m_pMgrData->GetLibInfoItem ();
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU_SELECT::Draw									 */
/* ���e		:�`��															 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

void CWindowITEMMENU_SELECT::Draw(PCImg32 pDst)
{
	int i, nCount, nLevel;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;
	LPCSTR pszTmp;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	clText = RGB (1, 1, 1);

	nCount = m_anCommand.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pszTmp = "";
		switch (m_anCommand[i]) {
		case ITEMMENU_SELECT_COMMAND_PUT:			pszTmp = "�n�ʂɒu��";	break;	/* �n�ʂɒu�� */
		case ITEMMENU_SELECT_COMMAND_EQUIP:			pszTmp = "��������";	break;	/* �������� */
		case ITEMMENU_SELECT_COMMAND_EQUIP_UNSET:	pszTmp = "�������O��";	break;	/* �������O�� */
		case ITEMMENU_SELECT_COMMAND_USE:			pszTmp = "�g��";		break;	/* �g�� */
		}
		TextOut2 (hDC, 32, 16 + 16 * i, pszTmp, clText);
	}

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
/* �֐���	:CWindowITEMMENU_SELECT::SetItemID								 */
/* ���e		:�A�C�e��ID���w��												 */
/* ���t		:2007/10/07														 */
/* ========================================================================= */

void CWindowITEMMENU_SELECT::SetItemID(int nType, DWORD dwItemID)
{
	int nPosMaxBack;
	DWORD dwItemTypeID;

	m_pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (m_pInfoItem == NULL) {
		return;
	}

	m_nType		= nType;
	nPosMaxBack	= m_nPosMax;

	m_anCommand.RemoveAll ();

	if (m_nType < EQUIPTYPE_MAX) {
		m_anCommand.Add (ITEMMENU_SELECT_COMMAND_EQUIP_UNSET);
	} else {
		dwItemTypeID = m_pLibInfoItem->GetItemType (m_pInfoItem->m_dwItemID);
		switch (dwItemTypeID) {
		case ITEMTYPEID_CLOTH:		/* �� */
		case ITEMTYPEID_ACCE:		/* �A�N�Z�T�� */
		case ITEMTYPEID_ARMS:		/* ������ */
		case ITEMTYPEID_SHIELD:		/* �� */
			m_anCommand.Add (ITEMMENU_SELECT_COMMAND_EQUIP);
			break;
		}
		m_anCommand.Add (ITEMMENU_SELECT_COMMAND_USE);
		m_anCommand.Add (ITEMMENU_SELECT_COMMAND_PUT);
	}

	m_nPosMax	= m_anCommand.GetSize () - 1;
	m_nPos		= min (m_nPos, m_nPosMax);
	if (nPosMaxBack != m_nPosMax) {
		m_pDib->Destroy ();
		/* �T�C�Y���ς�����̂ŉ摜�č쐬 */
		m_sizeWindow.cy	= 16 * 2 + 16 * (m_nPosMax + 1);
		m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
		m_pDib->SetColorKey (0);
		m_dwTimeDrawStart = 0;
	}
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU_SELECT::OnUp									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

BOOL CWindowITEMMENU_SELECT::OnUp(void)
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
/* �֐���	:CWindowITEMMENU_SELECT::OnDown									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

BOOL CWindowITEMMENU_SELECT::OnDown(void)
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
/* �֐���	:CWindowITEMMENU_SELECT::OnLeft									 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

BOOL CWindowITEMMENU_SELECT::OnLeft(void)
{
	m_nPos = 0;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU_SELECT::OnRight								 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

BOOL CWindowITEMMENU_SELECT::OnRight(void)
{
	m_nPos = m_nPosMax;
	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU_SELECT::OnX									 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

BOOL CWindowITEMMENU_SELECT::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

//	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, m_anCommand[m_nPos]);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU_SELECT::OnZ									 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2007/08/13														 */
/* ========================================================================= */

BOOL CWindowITEMMENU_SELECT::OnZ(BOOL bDown)
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

/* Copyright(C)URARA-works 2007 */
