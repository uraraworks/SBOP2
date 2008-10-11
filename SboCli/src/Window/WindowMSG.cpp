/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:WindowMSG.cpp												 */
/* ���e			:���b�Z�[�W�\���E�B���h�E�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WindowMSG.h"

/* ========================================================================= */
/* �֐���	:CWindowMSG::CWindowMSG											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/06/12														 */
/* ========================================================================= */

CWindowMSG::CWindowMSG()
{
	m_nID			= WINDOWTYPE_MSG;
	m_ptViewPos.x	= 112;
	m_ptViewPos.y	= SCRSIZEY / 2 - 24;
	m_sizeWindow.cx	= 16 * 16;
	m_sizeWindow.cy	= 16 * 3;
	m_dwDeleteTime	= 0;
	m_nType			= 0;
}


/* ========================================================================= */
/* �֐���	:CWindowMSG::~CWindowMSG										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/06/12														 */
/* ========================================================================= */

CWindowMSG::~CWindowMSG()
{
}


/* ========================================================================= */
/* �֐���	:CWindowMSG::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2006/11/03														 */
/* ========================================================================= */

void CWindowMSG::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
}


/* ========================================================================= */
/* �֐���	:CWindowMSG::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2005/06/12														 */
/* ========================================================================= */

void CWindowMSG::Draw(PCImg32 pDst)
{
	int nTmp;
	HDC hDC;
	HFONT hFontOld;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame (m_nType);

	nTmp = 0;
	if (m_nType == 4) {
		nTmp = 5;
	}
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	if (m_strMsg.IsEmpty () == FALSE) {
		TextOut2 (hDC, 16, 16 + nTmp, m_strMsg, RGB (1, 1, 1));
	}

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowMSG::TimerProc											 */
/* ���e		:���ԏ���														 */
/* ���t		:2005/06/12														 */
/* ========================================================================= */

BOOL CWindowMSG::TimerProc(void)
{
	BOOL bRet;
	DWORD dwTime;

	bRet = FALSE;
	if (m_dwDeleteTime == 0) {
		goto Exit;
	}

	dwTime = timeGetTime ();
	if (dwTime - m_dwLastTimerProc > m_dwDeleteTime) {
		/* �w�莞�ԕ\�������̂ō폜���� */
		m_bDelete = TRUE;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowMSG::SetMsg												 */
/* ���e		:���b�Z�[�W�ݒ�													 */
/* ���t		:2005/06/12														 */
/* ========================================================================= */

void CWindowMSG::SetMsg(LPCSTR pszMsg, DWORD dwTime, int nType)
{
	m_strMsg	= pszMsg;
	m_nType		= nType;

	/* �E�B���h�E���X�V */
	m_dwTimeDrawStart	= 0;
	m_dwDeleteTime		= dwTime;
	m_dwLastTimerProc	= timeGetTime ();
	m_sizeWindow.cx		= 16 * 3 + (m_strMsg.GetLength () * 8);
	m_ptViewPos.x		= SCRSIZEX / 2 - m_sizeWindow.cx / 2;

	if (nType == 4) {
		m_ptViewPos.y = SCRSIZEY - 72;
	}

	m_pDib->Destroy ();
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
}

/* Copyright(C)URARA-works 2006 */
