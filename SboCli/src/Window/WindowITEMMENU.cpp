/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WindowITEMMENU.cpp											 */
/* ���e			:�A�C�e�����j���[�E�B���h�E�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/30													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoItem.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowITEMMENU.h"


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU::CWindowITEMMENU								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

CWindowITEMMENU::CWindowITEMMENU()
{
	m_nPos			= EQUIPTYPE_MAX + 12;
	m_nPosMax		= EQUIPTYPE_MAX + 24;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_ITEMMENU;
	m_ptViewPos.x	= 24;
	m_ptViewPos.y	= 64;
	m_sizeWindow.cx	= 186;
	m_sizeWindow.cy	= 301;

	m_dwSelectItemID = 0;

	m_pPlayerChar	= NULL;
	m_pLibInfoItem	= NULL;
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU::~CWindowITEMMENU								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

CWindowITEMMENU::~CWindowITEMMENU()
{
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

void CWindowITEMMENU::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);

	m_pPlayerChar	= m_pMgrData->GetPlayerChar ();
	m_pLibInfoItem	= m_pMgrData->GetLibInfoItem ();
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

void CWindowITEMMENU::Draw(PCImg32 pDst)
{
	int i, nCount, nLevel, x, y;
	HDC hDC;
	HFONT hFontOld;
	PCInfoItem pInfoItem;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_strName.Empty ();
	m_pDib->BltFrom256 (186 / 2 - 62, 0, 124, 84, m_pDibSystem, 368, 200, TRUE);
	m_pDib->BltFrom256 (0, 84, 186, 217, m_pDibSystem, 0, 200, TRUE);

	/* �����A�C�e����`�� */
	DrawEquip (0, m_pPlayerChar->m_dwEquipItemIDCloth);		/* �� */
	DrawEquip (1, m_pPlayerChar->m_dwEquipItemIDAcce1);		/* �A�N�Z�T��1 */
	DrawEquip (2, m_pPlayerChar->m_dwEquipItemIDArmsRight);	/* ������ */
	DrawEquip (3, m_pPlayerChar->m_dwEquipItemIDArmsLeft);	/* �� */

	/* �A�C�e����`�� */
	nCount = m_pPlayerChar->m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pPlayerChar->m_adwItemID[i]);
		if (pInfoItem == NULL) {
			continue;
		}
		x = pInfoItem->m_ptBackPack.x;
		y = pInfoItem->m_ptBackPack.y;
		m_pMgrDraw->DrawItem (m_pDib, 10 + (x * 33), 95 + (y * 33), pInfoItem);
		if (m_strName.IsEmpty () == FALSE) {
			continue;
		}
		if (((m_nPos - EQUIPTYPE_MAX) % 5 == x) &&
			((m_nPos - EQUIPTYPE_MAX) / 5 == y)) {
			m_strName = pInfoItem->m_strName;
		}
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

	/* �A�C�e������\�� */
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
/* �֐���	:CWindowITEMMENU::OnUp											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos < EQUIPTYPE_MAX) {
		if (m_nPos < 3) {
			goto Exit;
		}
		m_nPos -= 3;
	} else {
		if (m_nPos < 10) {
			switch (m_nPos) {
			case 6:	m_nPos = 3;	break;
			case 7:	m_nPos = 4;	break;
			case 8:	m_nPos = 4;	break;
			case 9:	m_nPos = 5;	break;
			}
		} else {
			m_nPos -= 5;
		}
	}

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU::OnDown										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos < EQUIPTYPE_MAX) {
		if (m_nPos < 3) {
			m_nPos += 3;
		} else {
			switch (m_nPos) {
			case 3:	m_nPos = 6;	break;
			case 4:	m_nPos = 7;	break;
			case 5:	m_nPos = 8;	break;
			}
		}
	} else {
		if (m_nPos >= m_nPosMax - 4) {
			goto Exit;
		}
		m_nPos += 5;
	}

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU::OnLeft										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnLeft(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos < EQUIPTYPE_MAX) {
		if (m_nPos % 3 == 0) {
			goto Exit;
		}
	} else {
		if ((m_nPos - EQUIPTYPE_MAX) % 5 == 0) {
			goto Exit;
		}
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
/* �֐���	:CWindowITEMMENU::OnRight										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnRight(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_nPos < EQUIPTYPE_MAX) {
		if (m_nPos % 3 == 2) {
			goto Exit;
		}
	} else {
		if ((m_nPos - EQUIPTYPE_MAX) % 5 == 4) {
			goto Exit;
		}
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
/* �֐���	:CWindowITEMMENU::OnB											 */
/* ���e		:�L�[�n���h��(B)												 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnB(BOOL bDown)
{
	return OnZ (bDown);
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnX(BOOL bDown)
{
	int i, nCount;
	POINT ptPos;
	BOOL bRet;
	DWORD dwItemID;
	PCInfoItem pInfoItem;

	bRet		= FALSE;
	dwItemID	= 0;

	if (bDown) {
		goto Exit;
	}

	if (m_nPos < EQUIPTYPE_MAX) {
		switch (m_nPos) {
		case EQUIPTYPE_ACCE1:		dwItemID = m_pPlayerChar->m_dwEquipItemIDAcce1;		break;	/* �A�N�Z�T��1 */
		case EQUIPTYPE_HEAD:		dwItemID = m_pPlayerChar->m_dwEquipItemIDHead;		break;	/* �� */
		case EQUIPTYPE_ACCE2:		dwItemID = m_pPlayerChar->m_dwEquipItemIDAcce2;		break;	/* �A�N�Z�T��2 */
		case EQUIPTYPE_ARMSRIGHT:	dwItemID = m_pPlayerChar->m_dwEquipItemIDArmsRight;	break;	/* �E�� */
		case EQUIPTYPE_CLOTH:		dwItemID = m_pPlayerChar->m_dwEquipItemIDCloth;		break;	/* �� */
		case EQUIPTYPE_ARMSLEFT:	dwItemID = m_pPlayerChar->m_dwEquipItemIDArmsLeft;	break;	/* ���� */
		}
	} else {
		ptPos.x = (m_nPos - EQUIPTYPE_MAX) % 5;
		ptPos.y = (m_nPos - EQUIPTYPE_MAX) / 5;

		nCount = m_pPlayerChar->m_adwItemID.GetSize ();
		for (i = 0; i < nCount; i ++) {
			pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pPlayerChar->m_adwItemID[i]);
			if (pInfoItem == NULL) {
				continue;
			}
			/* �I�����Ă���ꏊ�ɃA�C�e�����������H */
			if ((pInfoItem->m_ptBackPack.x == ptPos.x) &&
				(pInfoItem->m_ptBackPack.y == ptPos.y)) {
				dwItemID = pInfoItem->m_dwItemID;
				break;
			}
		}
	}

	if (dwItemID == 0) {
		goto Exit;
	}

	m_dwSelectItemID = dwItemID;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, dwItemID);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU::OnZ											 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

BOOL CWindowITEMMENU::OnZ(BOOL bDown)
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
/* �֐���	:CWindowITEMMENU::DrawEquip										 */
/* ���e		:�����A�C�e����`��												 */
/* ���t		:2008/11/19														 */
/* ========================================================================= */

void CWindowITEMMENU::DrawEquip(
	int nType,					/* [in] ��� */
	DWORD dwEquipItemID)		/* [in] �����A�C�e��ID */
{
	int x, y, anDrawInfo[] = {
			1, 1, EQUIPTYPE_CLOTH,		/* �� */
			0, 0, EQUIPTYPE_ACCE1,		/* �A�N�Z�T��1 */
			0, 1, EQUIPTYPE_ARMSRIGHT,	/* �E�� */
			2, 1, EQUIPTYPE_ARMSLEFT,	/* ���� */
//			EQUIPTYPE_HEAD,			/* �� */
//			EQUIPTYPE_ACCE2,		/* �A�N�Z�T��2 */
//			EQUIPTYPE_CLOTH,		/* �� */
			0, 0, 0};
	PCInfoItem pInfoItem;

	if (dwEquipItemID == 0) {
		return;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwEquipItemID);
	if (pInfoItem == NULL) {
		return;
	}
	x = 41 + 36 * anDrawInfo[nType * 3 + 0];
	y = 10 + 36 * anDrawInfo[nType * 3 + 1];
	m_pMgrDraw->DrawItem (m_pDib, x, y, pInfoItem);
	if (m_nPos == anDrawInfo[nType * 3 + 2]) {
		/* �������̃A�C�e���Ȃ̂ŃA�C�e�������X�V */
		m_strName = pInfoItem->m_strName;
	}
}


/* ========================================================================= */
/* �֐���	:CWindowITEMMENU::GetDrawPos									 */
/* ���e		:�A�C�e���`��ʒu���擾											 */
/* ���t		:2008/11/19														 */
/* ========================================================================= */

void CWindowITEMMENU::GetDrawPos(
	int nPos,		/* [in] �A�C�e���ʒu */
	int &nDstX,		/* [out] X���W */
	int &nDstY)		/* [out] Y���W */
{
	if (nPos < EQUIPTYPE_MAX) {
		nDstX = 36 * (nPos % 3) + 41 + 1;
		nDstY = 36 * (nPos / 3) + 10 + 1;
	} else {
		nDstX = 33 * ((nPos - EQUIPTYPE_MAX) % 5) + 11;
		nDstY = 33 * ((nPos - EQUIPTYPE_MAX) / 5) + 96;
	}
}

/* Copyright(C)URARA-works 2007 */
