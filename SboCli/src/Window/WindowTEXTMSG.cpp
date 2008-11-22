/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WindowTEXTMSG.cpp											 */
/* ���e			:�e�L�X�g���b�Z�[�W�\���E�B���h�E�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrSound.h"
#include "WindowTEXTMSG.h"

/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::CWindowTEXTMSG									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

CWindowTEXTMSG::CWindowTEXTMSG()
{
	m_nID			= WINDOWTYPE_TEXTMSG;
	m_ptViewPos.x	= 16;
	m_sizeWindow.cx	= 16 * 2 + 16 * 26;
	m_sizeWindow.cy	= 16 * 2 + 16 * 5;
	m_ptViewPos.y	= SCRSIZEY - 16 - m_sizeWindow.cy;

	m_bInputWait	= FALSE;
	m_nType			= 0;
	m_nProcPos		= 0;
	m_dwLastProc	= 0;
	m_pDibText		= NULL;
	m_ptDraw.x = m_ptDraw.y = 0;
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::~CWindowTEXTMSG								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

CWindowTEXTMSG::~CWindowTEXTMSG()
{
	SAFE_DELETE (m_pDibText);
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CWindowTEXTMSG::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);

	m_pDibText = new CImg32;
	m_pDibText->Create (m_sizeWindow.cx - 16 * 2, m_sizeWindow.cy - 16 * 2);
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2005/06/12														 */
/* ========================================================================= */

void CWindowTEXTMSG::Draw(PCImg32 pDst)
{
	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame (m_nType);

	m_pDib->Blt (16, 16, m_pDibText->Width (), m_pDibText->Height (), m_pDibText, 0, 0, TRUE);

	m_dwTimeDrawStart = timeGetTime ();
Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::TimerProc(void)
{
	BOOL bRet, bResult;
	DWORD dwTimeTmp;
	LPCSTR pszTmp;
	char szTmp[3];
	int cx, cy;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;

	bRet = FALSE;

	if (m_pDibText == NULL) {
		goto Exit;
	}
	dwTimeTmp = timeGetTime () - m_dwLastProc;
	if (dwTimeTmp < 100) {
		goto Exit;
	}
	m_dwLastProc = timeGetTime ();

	cx = m_pDibText->Width ();
	cy = m_pDibText->Height ();
	ZeroMemory (szTmp, sizeof (szTmp));
	pszTmp = (LPCSTR)m_strMsg;

	szTmp[0] = pszTmp[m_nProcPos];
	if (szTmp[0] == 0) {
		m_bInputWait = TRUE;
		goto Exit;
	}

	bResult = IsDBCSLeadByte ((BYTE)pszTmp[m_nProcPos]);
	if (bResult) {
		szTmp[1] = pszTmp[m_nProcPos + 1];
		m_nProcPos ++;
	}

	clText		= RGB (1, 1, 1);
	hDC			= m_pDibText->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont16Normal);
	SetBkMode (hDC, TRANSPARENT);

	clText = RGB (1, 1, 1);
//	clText = RGB (255, 255, 255);
	TextOut2 (hDC, m_ptDraw.x, m_ptDraw.y, szTmp, clText);

	if (pszTmp[m_nProcPos] == 0x0D) {
		if (pszTmp[m_nProcPos + 1] == 0x0A) {
			m_ptDraw.x = cx;
			m_nProcPos ++;
		}
	}

	m_ptDraw.x += (strlen (szTmp) * 8);
	if (m_ptDraw.x + 8 >= cx) {
		m_ptDraw.x = 0;
		m_ptDraw.y += 16;
		if (m_ptDraw.y >= cy) {
			m_ptDraw.y -= 16;
			m_pDibText->Blt (0, 0, cx, cy, m_pDibText, 0, 16);
			m_pDibText->FillRect (0, cy - 16, cx, 16, RGB (0, 0, 0));
		}
	}

	SelectObject (hDC, hFontOld);
	m_pDibText->Unlock ();

	m_nProcPos ++;
	Redraw ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::SetMsg											 */
/* ���e		:���b�Z�[�W�ݒ�													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CWindowTEXTMSG::SetMsg(LPCSTR pszMsg)
{
	m_ptDraw.x = m_ptDraw.y = 0;
	m_nProcPos		= 0;
	m_dwLastProc	= timeGetTime ();
	m_strMsg		= pszMsg;

	m_pDibText->FillRect (0, 0, m_pDibText->Width (), m_pDibText->Height (), RGB (0, 0, 0));
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnX(BOOL bDown)
{
	if (bDown == FALSE) {
		if (m_bInputWait) {
			m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
			m_bDelete = TRUE;
		}
	}

	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::OnZ											 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnZ(BOOL bDown)
{
	if (bDown == FALSE) {
		if (m_bInputWait) {
			m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
			m_bDelete = TRUE;
		}
	}

	return FALSE;
}

/* Copyright(C)URARA-works 2008 */
