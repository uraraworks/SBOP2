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

/* �\����� */
enum {
	STATE_TEXT = 0,			/* ���b�Z�[�W�\�� */
	STATE_MENU,				/* ���ڑI�� */
};

/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::CWindowTEXTMSG									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

CWindowTEXTMSG::CWindowTEXTMSG()
{
	m_nSpaceHeight	= 16 * 3;

	m_nID			= WINDOWTYPE_TEXTMSG;
	m_ptViewPos.x	= 16;
	m_sizeWindow.cx	= 16 * 2 + 16 * 26;
	m_sizeWindow.cy	= 16 * 2 + 16 * 5 + m_nSpaceHeight;
	m_ptViewPos.y	= SCRSIZEY - 16 - m_sizeWindow.cy;

	m_bInputWait	= FALSE;
	m_nState		= STATE_TEXT;
	m_nType			= 0;
	m_nProcPos		= 0;
	m_nProcPosTmp	= 0;
	m_dwLastProc	= 0;
	m_pDibTitle		= NULL;
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
	SAFE_DELETE (m_pDibTitle);
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
	m_pDib->SetColorKey (0);

	m_pDibTitle = new CImg32;
	m_pDibTitle->Create (16 * 6, 16 * 2);
	m_pDibText = new CImg32;
	m_pDibText->Create (m_sizeWindow.cx - 16 * 2 + 2, m_sizeWindow.cy - 16 * 2 - m_nSpaceHeight + 2);
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2005/06/12														 */
/* ========================================================================= */

void CWindowTEXTMSG::Draw(PCImg32 pDst)
{
	int nTmp, x, y, cx, cy;
	SIZE sizeWindow;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	sizeWindow = m_sizeWindow;
	/* ������ */
	switch (m_nState) {
	case STATE_TEXT:		/* ���b�Z�[�W�\�� */
		break;
	case STATE_MENU:		/* ���ڑI�� */
		sizeWindow.cx /= 2;
		break;
	}

	m_pDib->FillRect (0, 0, m_pDib->Width (), m_pDib->Height (), RGB (0, 0, 0));
	switch (m_nState) {
	case STATE_TEXT:		/* ���b�Z�[�W�\�� */
		if ((m_ptDraw.x == 0) && (m_ptDraw.y == 0)) {
			break;
		}
		DrawFrame (0, m_nSpaceHeight, sizeWindow.cx, sizeWindow.cy - m_nSpaceHeight, m_nType);

		nTmp = 0;
		if (m_strTitle.IsEmpty () == FALSE) {
			nTmp ++;
		}
		if (m_strName.IsEmpty () == FALSE) {
			nTmp ++;
		}
		if (nTmp > 0) {
			cx = m_pDibTitle->Width ()  + 16 * 2;
			cy = 16 * (2 + nTmp);
			x = sizeWindow.cx / 2 - cx / 2;
			y = (2 - nTmp) * 16;
			DrawFrame (x, y, cx, cy, m_nType);
			m_pDib->Blt (x + 16, y + 16, m_pDibTitle->Width (), m_pDibTitle->Height (), m_pDibTitle, 0, 0, TRUE);
		}

		m_pDib->Blt (16, 16 + m_nSpaceHeight, m_pDibText->Width (), m_pDibText->Height (), m_pDibText, 0, 0, TRUE);
		break;
	case STATE_MENU:		/* ���ڑI�� */
		sizeWindow.cy = m_astrMenu.GetSize () * 16 + 32;
		DrawFrame (0, m_nSpaceHeight, sizeWindow.cx, sizeWindow.cy, m_nType);

		nTmp = 0;
		if (m_strTitle.IsEmpty () == FALSE) {
			nTmp ++;
		}
		if (m_strName.IsEmpty () == FALSE) {
			nTmp ++;
		}
		if (nTmp > 0) {
			cx = m_pDibTitle->Width ()  + 16 * 2;
			cy = 16 * (2 + nTmp);
			x = sizeWindow.cx / 2 - cx / 2;
			y = (2 - nTmp) * 16;
			DrawFrame (x, y, cx, cy, m_nType);
			m_pDib->Blt (x + 16, y + 16, m_pDibTitle->Width (), m_pDibTitle->Height (), m_pDibTitle, 0, 0, TRUE);
		}
		{
			int i, nCount;
			HDC hDC;
			HFONT hFontOld;
			COLORREF clText;

			clText		= RGB (1, 1, 1);
			hDC			= m_pDib->Lock ();
			hFontOld	= (HFONT)SelectObject (hDC, m_hFont16Normal);
			SetBkMode (hDC, TRANSPARENT);

			nCount = m_astrMenu.GetSize ();
			for (i = 0; i < nCount; i ++) {
				TextOut2 (hDC, 32, y + cy + i * 16, (LPCSTR)m_astrMenu[i], clText);
			}

			SelectObject (hDC, hFontOld);
			m_pDib->Unlock ();
		}
		DrawCursor (8, y + cy + 16 * m_nPos);
		break;
	}

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
	BOOL bRet;
	int *pnProcPos;
	DWORD dwTimeTmp;
	LPCSTR pszTmp;
	char szTmp[3];

	bRet = FALSE;

	if (m_pDibText == NULL) {
		goto Exit;
	}
	dwTimeTmp = timeGetTime () - m_dwLastProc;
	if (dwTimeTmp < 100) {
		goto Exit;
	}
	m_dwLastProc = timeGetTime ();

	switch (m_nState) {
	case STATE_TEXT:		/* ���b�Z�[�W�\�� */
		ZeroMemory (szTmp, sizeof (szTmp));
		pnProcPos = &m_nProcPos;
		pszTmp = (LPCSTR)m_strMsg;
		if (m_strMsgTmp.IsEmpty () == FALSE) {
			pszTmp = (LPCSTR)m_strMsgTmp;
			pnProcPos = &m_nProcPosTmp;
		}

		if ((pszTmp[*pnProcPos] == 0) || (*pnProcPos >= (int)strlen (pszTmp))) {
			if (m_strMsgTmp.IsEmpty () == FALSE) {
				m_strMsgTmp.Empty ();
				m_nProcPosTmp = 0;
				goto Exit;
			}
			m_bInputWait = TRUE;
			goto Exit;
		}

		if (strncmp (&pszTmp[*pnProcPos], "\r\n", 2) == 0) {
			CopyMemory (szTmp, "\r\n", 2);

		} else {
			_tcsnccpy (szTmp, &pszTmp[*pnProcPos], 1);
		}

		if (szTmp[0] == '@') {
			MsgProc ();

		} else {
			*pnProcPos += strlen (szTmp);

			DrawChar (szTmp);
		}
		break;
	case STATE_MENU:		/* ���ڑI�� */
		break;
	}

	Redraw ();

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::SetTitle										 */
/* ���e		:��������ݒ�													 */
/* ���t		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::SetTitle(LPCSTR pszTitle)
{
	m_strTitle = pszTitle;
	RenewTitle ();
	Redraw ();
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::SetName										 */
/* ���e		:���O��ݒ�														 */
/* ���t		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::SetName(LPCSTR pszName)
{
	m_strName = pszName;
	RenewTitle ();
	Redraw ();
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
/* �֐���	:CWindowTEXTMSG::OnUp											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/11/27														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nState) {
	case STATE_MENU:		/* ���ڑI�� */
		if (m_nPos <= 0) {
			goto Exit;
		}
		m_nPos --;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::OnDown											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/11/27														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	switch (m_nState) {
	case STATE_MENU:		/* ���ڑI�� */
		if (m_nPos >= m_nPosMax) {
			goto Exit;
		}
		m_nPos ++;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::OnLeft											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/11/27														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnLeft(void)
{
	switch (m_nState) {
	case STATE_MENU:		/* ���ڑI�� */
		m_nPos = 0;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
		break;
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::OnRight										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2008/11/27														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnRight(void)
{
	switch (m_nState) {
	case STATE_MENU:		/* ���ڑI�� */
		m_nPos = m_nPosMax;
		m_nCursorAnime = 0;
		m_dwLastTimeCursor = 0;
		m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);
		break;
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

BOOL CWindowTEXTMSG::OnX(BOOL bDown)
{
	if (bDown == FALSE) {
		switch (m_nState) {
		case STATE_TEXT:		/* ���b�Z�[�W�\�� */
			if (m_bInputWait) {
				m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
				m_bDelete = TRUE;
			}
			break;
		case STATE_MENU:		/* ���ڑI�� */
			m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
			m_nState = STATE_TEXT;
			break;
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
		switch (m_nState) {
		case STATE_TEXT:		/* ���b�Z�[�W�\�� */
			if (m_bInputWait) {
				m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
				m_bDelete = TRUE;
			}
			break;
		case STATE_MENU:		/* ���ڑI�� */
			m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
			m_bDelete = TRUE;
			break;
		}
	}

	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::DrawChar										 */
/* ���e		:�P�����\��														 */
/* ���t		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::DrawChar(LPCSTR pszText)
{
	int cx, cy;
	HDC hDC;
	HFONT hFontOld;
	COLORREF clText;

	if (pszText == NULL) {
		return;
	}

	cx = m_pDibText->Width ();
	cy = m_pDibText->Height () - 16;

	clText		= RGB (1, 1, 1);
	hDC			= m_pDibText->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont16Normal);
	SetBkMode (hDC, TRANSPARENT);

	clText = RGB (1, 1, 1);
	TextOut2 (hDC, m_ptDraw.x, m_ptDraw.y, pszText, clText);

	if (strncmp (pszText, "\r\n", 2) == 0) {
		m_ptDraw.x = cx;
	}

	m_ptDraw.x += (strlen (pszText) * 8);
	if (m_ptDraw.x + 8 >= cx) {
		m_ptDraw.x = 0;
		m_ptDraw.y += 16;
		if (m_ptDraw.y >= cy) {
			m_ptDraw.y -= 16;
			m_pDibText->Blt (0, 0, cx, cy, m_pDibText, 0, 16);
			m_pDibText->FillRect (0, cy, cx, 16, RGB (0, 0, 0));
		}
	}

	SelectObject (hDC, hFontOld);
	m_pDibText->Unlock ();
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::RenewTitle										 */
/* ���e		:�����Ɩ��O�摜���X�V											 */
/* ���t		:2008/11/24														 */
/* ========================================================================= */

void CWindowTEXTMSG::RenewTitle(void)
{
	int y;
	HDC hDC;
	COLORREF clText, clFrame;
	HFONT hFontOld;

	m_pDibTitle->FillRect (0, 0, m_pDibTitle->Width (), m_pDibTitle->Height (), RGB (0, 0, 0));

	hDC			= m_pDibTitle->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont16);
	SetBkMode (hDC, TRANSPARENT);

	y = 1;
	clText  = RGB (255, 255, 255);
	clFrame = RGB (1, 1, 1);
	if (m_strTitle.GetLength () > 0) {
		TextOut2 (hDC, 1, 1, (LPCSTR)m_strTitle, clText, TRUE, clFrame);
		y += 16;
	}
	if (m_strName.GetLength () > 0) {
		TextOut2 (hDC, 1, y, (LPCSTR)m_strName, clText, TRUE, clFrame);
	}

	SelectObject (hDC, hFontOld);
	m_pDibTitle->Unlock ();
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::InitText										 */
/* ���e		:���b�Z�[�W�摜��������											 */
/* ���t		:2008/11/26														 */
/* ========================================================================= */

void CWindowTEXTMSG::InitText(void)
{
	m_ptDraw.x = m_ptDraw.y = 0;
	m_pDibText->FillRect (0, 0, m_pDibText->Width (), m_pDibText->Height (), RGB (0, 0, 0));
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::MsgProc										 */
/* ���e		:���b�Z�[�W��͏���												 */
/* ���t		:2008/11/27														 */
/* ========================================================================= */

void CWindowTEXTMSG::MsgProc(void)
{
	BOOL bSkip;
	int nPos, nLen;
	char szTmp[2];
	LPCSTR pszSrc, pszTmp;
	CmyString strTmp, strTmp2;

	pszSrc	= (LPCSTR)m_strMsg;
	pszSrc	= &pszSrc[m_nProcPos];
	nLen	= strlen (pszSrc);

	GetLineText (pszSrc, strTmp);
	pszTmp = (LPCSTR)strTmp;

	m_astrMenu.RemoveAll ();

	nPos = 0;
	/* ���j���[�H */
	if (_strnicmp(&pszTmp[nPos], "@menu", 5) == 0) {
		while (1) {
			nPos += (strTmp.GetLength () + 2);
			if (nPos >= nLen) {
				break;
			}

			GetLineText (&pszSrc[nPos], strTmp);
			pszTmp = (LPCSTR)strTmp;
			if (pszTmp[0] == '{') {
				continue;
			}
			if (pszTmp[0] == '}') {
				nPos += (strTmp.GetLength () + 2);
				break;
			}
			strTmp2 = strTmp;
			TrimSpace (strTmp2);
			m_astrMenu.Add (strTmp2);
		}
		m_nProcPos += nPos;
		m_nPos		= 0;
		m_nPosMax	= m_astrMenu.GetSize ();
		if (m_nPosMax > 0) {
			m_nPosMax --;
			m_nState = STATE_MENU;
		}

	/* ���ڑI���H */
	} else if (_strnicmp(&pszTmp[nPos], "@select", 7) == 0) {
		szTmp[0] = pszTmp[nPos + 8];
		szTmp[1] = 0;
		bSkip = FALSE;
		if (m_nPos != atoi (szTmp)) {
			bSkip = TRUE;
		}
		m_nProcPosTmp = 0;
		m_strMsgTmp.Empty ();
		while (1) {
			nPos += (strTmp.GetLength () + 2);
			if (nPos >= nLen) {
				break;
			}

			GetLineText (&pszSrc[nPos], strTmp);
			pszTmp = (LPCSTR)strTmp;
			if (pszTmp[0] == '{') {
				continue;
			}
			if (pszTmp[0] == '}') {
				nPos += (strTmp.GetLength () + 2);
				break;
			}
			if (bSkip == FALSE) {
				strTmp2 = strTmp;
				TrimSpace (strTmp2);
				m_strMsgTmp += strTmp2;
			}
		}
		m_nProcPos += nPos;
	}
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::GetLineText									 */
/* ���e		:1�s�擾														 */
/* ���t		:2008/11/27														 */
/* ========================================================================= */

void CWindowTEXTMSG::GetLineText(LPCSTR pszSrc, CmyString &strDst)
{
	int i, nLen;
	char szTmp[3];

	strDst.Empty ();

	nLen = strlen (pszSrc);
	for (i = 0; i < nLen - 1;) {
		ZeroMemory (szTmp, sizeof (szTmp));
		_tcsnccpy (szTmp, &pszSrc[i], 1);
		if (szTmp[0] == 0) {
			break;
		}

		if (strncmp (&pszSrc[i], "\r\n", 2) == 0) {
			break;
		}
		strDst += szTmp;
		i += strlen (szTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::TrimSpace										 */
/* ���e		:�O��̋󔒂�����												 */
/* ���t		:2008/11/27														 */
/* ========================================================================= */

void CWindowTEXTMSG::TrimSpace(CmyString &strSrc)
{
	int i, nLen;
	char szTmp[3];
	LPCSTR pszSrc;
	CmyString strTmp;

	strTmp = strSrc;
	pszSrc = (LPCSTR)strTmp;

	nLen = strSrc.GetLength ();
	for (i = 0; i < nLen;) {
		ZeroMemory (szTmp, sizeof (szTmp));
		_tcsnccpy (szTmp, &pszSrc[i], 1);
		if (szTmp[0] == 0) {
			break;
		}

		if ((strcmp (szTmp, " ") == 0) || (strcmp (szTmp, "�@") == 0)) {
			i += strlen (szTmp);
			continue;
		}
		strSrc = &pszSrc[i];
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CWindowTEXTMSG::GetBlock										 */
/* ���e		:�傩����1�u���b�N���擾										 */
/* ���t		:2008/11/27														 */
/* ========================================================================= */

void CWindowTEXTMSG::GetBlock(LPCSTR pszSrc, CmyString &strDst)
{
	strDst.Empty ();
}

/* Copyright(C)URARA-works 2008 */
