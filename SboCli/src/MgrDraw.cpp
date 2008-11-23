/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrDraw.cpp												 */
/* ���e			:�`��}�l�[�W���N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapShadow.h"
#include "LibInfoMapObject.h"
#include "LibInfoItem.h"
#include "LibInfoMotionType.h"
#include "InfoMotion.h"
#include "InfoTextEffect.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrLayer.h"
#include "MgrWindow.h"
#include "MgrDraw.h"


/* ========================================================================= */
/* �֐���	:CMgrDraw::CMgrDraw												 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

CMgrDraw::CMgrDraw()
{
	m_byLevel		= 0;
	m_byFadeState	= FADESTATE_NONE;
	m_dwLastFade	= 0;

	m_pMgrData				= NULL;
	m_pMgrGrpData			= NULL;
	m_pMgrLayer				= NULL;
	m_pMgrWindow			= NULL;
	m_pLibInfoMapParts		= NULL;
	m_pLibInfoMapShadow		= NULL;
	m_pLibInfoItem			= NULL;
	m_pLibInfoMotionType	= NULL;

	m_pDibBack = new CImg32;
	m_pDibBack->Create (SCRSIZEX + 64, SCRSIZEY + 64);
	m_pDibTmp = new CImg32;
	m_pDibTmp->Create ((SCRSIZEX + 64) * 2, (SCRSIZEY + 64) * 2);
	m_pDibTmp->SetColorKey (RGB (255, 0, 255));
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::~CMgrDraw											 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

CMgrDraw::~CMgrDraw()
{
	SAFE_DELETE (m_pDibBack);
	SAFE_DELETE (m_pDibTmp);
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::Create(
	CMgrData	*pMgrData)		/* [in] �f�[�^�Ǘ� */
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();
	m_pMgrLayer		= m_pMgrData->GetMgrLayer ();
	m_pMgrWindow	= m_pMgrData->GetMgrWindow ();

	m_pLibInfoMapParts		= m_pMgrData->GetLibInfoMapParts ();
	m_pLibInfoMapShadow		= m_pMgrData->GetLibInfoMapShadow ();
	m_pLibInfoItem			= m_pMgrData->GetLibInfoItem ();
	m_pLibInfoMotionType	= m_pMgrData->GetLibInfoMotionType ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::Destroy												 */
/* ���e		:�j��															 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::Destroy(void)
{
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::Draw													 */
/* ���e		:�`��															 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::Draw(HDC hDC)
{
	HDC hDCTmp;

	m_pMgrLayer->	Draw (m_pDibBack);		/* ���C���[ */
	m_pMgrWindow->	Draw (m_pDibBack);		/* �E�B���h�E */

	/* ���x�w�肠��H */
	if (m_byLevel < 100 && m_byLevel > 0) {
		m_pDibBack->ChgLevel (32, 32, SCRSIZEX, SCRSIZEY, (m_byLevel == 1) ? 0 : m_byLevel);
	}

	hDCTmp = m_pDibBack->Lock ();
	BitBlt (hDC, 0, 0, SCRSIZEX, SCRSIZEY, hDCTmp, 32, 32, SRCCOPY);
//	m_pDibBack->Unlock ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawChar												 */
/* ���e		:�L������`��													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::DrawChar(
	CImg32 *pDst,					/* [in] �`��� */
	int x,							/* [in] �`��ʒu(X) */
	int y,							/* [in] �`��ʒu(Y) */
	CInfoCharCli *pInfoChar,		/* [in] �L������� */
	BOOL bLock)/*TRUE*/				/* [in] �e���|���������b�N���� */
{
	WORD wGrpIDMain, wGrpIDSub, wGrpIDMainBase, wGrpIdNPC, wGrpID;
	int cx, cy, cxChar, cyChar, nTmp, nDirection, nEyeID, i, nCount, nFadeLevel;
	POINT ptTmp;
	PCInfoMotion pInfoMotion;
	PCImg32 pSrc, pDibTmp;
	HDC hDCBmp;

	LockDibTmp ();
	pDibTmp = GetDibTmp ();
	pInfoMotion = pInfoChar->GetMotionInfo ();
	if (pInfoMotion == NULL) {
		goto Exit;
	}

	wGrpID			= 0;
	wGrpIDMainBase	= pInfoMotion->m_wGrpIDMainBase;
	wGrpIdNPC		= pInfoChar->m_wGrpIDNPC;
	if (wGrpIdNPC != 0) {
		if (wGrpIdNPC >= 50000) {
			wGrpIdNPC -= 50000;
			if (wGrpIDMainBase == GRPIDMAIN_2X2_CHAR) {
				wGrpIDMainBase = GRPIDMAIN_2X2_NPC;
			} else {
				wGrpIDMainBase = GRPIDMAIN_NPC;
				if (pInfoChar->m_bMotionDirection == FALSE) {
					wGrpID = wGrpIdNPC;
				}
			}
		} else {
			wGrpIDMainBase = GRPIDMAIN_2X2_NPC;
		}
	}

	nDirection = pInfoChar->GetDrawDirection (-1);
	x += pInfoChar->m_ptMove.x;
	y += pInfoChar->m_ptMove.y;
	cx = cy = m_pMgrGrpData->GetGrpSize (wGrpIDMainBase);
	cxChar = cyChar = cx;

//Todo;
	nEyeID = 0;
	if (pInfoChar->IsDamage ()) {
		nEyeID = 1;
	}
	if (pInfoChar->m_nMoveState == CHARMOVESTATE_SWOON) {
		nEyeID = 2;
	}
	if (pInfoChar->m_nMoveState == CHARMOVESTATE_SLEEPTIMER) {
		nEyeID = 3;
	}

	nFadeLevel = pInfoChar->m_nFadeLevel;
	if (pInfoChar->m_nMoveType == CHARMOVETYPE_PUTNPC) {
		if (m_pMgrData->GetAdminLevel () == ADMINLEVEL_NONE) {
			return;
		}
		if (nFadeLevel == 0) {
			nFadeLevel = 50;
		}
	}

	/* �L�����e�̕`�� */
	switch (wGrpIDMainBase) {
	case GRPIDMAIN_2X2_CHAR:
		m_pMgrGrpData->GetGrpPos (wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, ptTmp);
		pSrc = m_pMgrGrpData->GetDib2x2CharShadow (0);
		pDibTmp->BltFrom256 (0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y);
		hDCBmp = pDibTmp->Lock ();
		StretchBlt (hDCBmp, 0, cyChar * 2, cxChar * 2, cyChar * 2, hDCBmp, 0, 0, cxChar, cyChar, SRCCOPY);
		if (bLock) {
			pDibTmp->Unlock ();
		}
		pDst->BltAlpha (
			x + pInfoMotion->m_ptDrawPosPile0.x,
			y + pInfoMotion->m_ptDrawPosPile0.y,
			cxChar * 2, cyChar * 2, pDibTmp, 0, cyChar * 2, 75, TRUE);
		break;

	case GRPIDMAIN_2X2_NPC:
		m_pMgrGrpData->GetGrpPos (wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, ptTmp);
		nTmp = 0;
		if (pInfoChar->m_nSex == SEX_FEMALE) {
			nTmp += 32 * 4;
		}
		ptTmp.y += nTmp;
		pSrc = m_pMgrGrpData->GetDib2x2NPCShadow (wGrpIdNPC);
		pDibTmp->BltFrom256 (0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y);
		hDCBmp = pDibTmp->Lock ();
		StretchBlt (hDCBmp, 0, cyChar * 2, cxChar * 2, cyChar * 2, hDCBmp, 0, 0, cxChar, cyChar, SRCCOPY);
		if (bLock) {
			pDibTmp->Unlock ();
		}
		pDst->BltAlpha (
			x + pInfoMotion->m_ptDrawPosPile0.x,
			y + pInfoMotion->m_ptDrawPosPile0.y,
			cxChar * 2, cyChar * 2, pDibTmp, 0, cyChar * 2, 75, TRUE);
		break;
	}

	nCount = pInfoMotion->m_anDrawList.GetSize ();
	for (i = 0; i < nCount; i ++) {
		switch (pInfoMotion->m_anDrawList[i]) {
		case 0:
			/* 2x2�T�C�Y�ȊO�H */
			if (wGrpIDMainBase != GRPIDMAIN_2X2_CHAR) {
				nTmp = 0;
				if (wGrpIDMainBase == GRPIDMAIN_2X2_NPC) {
					if (pInfoChar->m_nSex == SEX_FEMALE) {
						nTmp += 32 * 4;
					}
				}
				m_pMgrGrpData->GetGrpPos (wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, ptTmp, wGrpID);
				ptTmp.y += nTmp;
				if (wGrpIDMainBase == GRPIDMAIN_CHAR || ((wGrpIDMainBase == GRPIDMAIN_NPC) && (pInfoChar->m_bMotionDirection))) {
					pSrc = pInfoChar->m_pDibChar;
					pDibTmp->Blt (0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y);
				} else {
					pSrc = m_pMgrGrpData->GetDib (wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, wGrpIdNPC);
					pDibTmp->BltFrom256 (0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y);
				}

				if (pInfoMotion->m_bRedrawHand) {
					pSrc = m_pMgrGrpData->GetDib (wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase, pInfoChar->m_wFamilyID);
					if (pSrc) {
						pDibTmp->Blt (10, 9,  2, 4, pSrc, ptTmp.x + 10,  9, TRUE);
						pDibTmp->Blt (9,  10, 4, 2, pSrc, ptTmp.x +  9, 10, TRUE);
					}
				}
				hDCBmp = pDibTmp->Lock ();
				StretchBlt (hDCBmp, 0, cyChar * 2, cxChar * 2, cyChar * 2, hDCBmp, 0, 0, cxChar, cyChar, SRCCOPY);
				if (bLock) {
					pDibTmp->Unlock ();
				}
				pDst->BltAlpha (
					x + pInfoMotion->m_ptDrawPosPile0.x,
					y + pInfoMotion->m_ptDrawPosPile0.y,
					cxChar * 2, cyChar * 2, pDibTmp, 0, cyChar * 2, nFadeLevel, TRUE);

			/* 2x2�T�C�Y */
			} else {
				m_pMgrGrpData->GetGrpPos (wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, ptTmp);
				nTmp = 0;
				if (pInfoChar->m_nSex == SEX_FEMALE) {
					nTmp += 32 * 4;
				}
				pSrc = m_pMgrGrpData->GetDib (wGrpIDMainBase, pInfoMotion->m_wGrpIDSubBase - 1, 0);
				/* �� */
				pDibTmp->BltFrom256 (0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y + nTmp);
				if ((pInfoChar->m_wGrpIDSP == 0) && (pInfoChar->m_wGrpIDTmpMain == 0)) {
					wGrpIDMain = GRPIDMAIN_2X2_CLOTH;
					wGrpIDSub  = pInfoChar->m_wGrpIDCloth;
				} else {
					wGrpIDMain = GRPIDMAIN_2X2_SPCLOTH;
					wGrpIDSub  = pInfoChar->m_wGrpIDSP;
					if (pInfoChar->m_wGrpIDTmpMain != 0) {
						wGrpIDMain = pInfoChar->m_wGrpIDTmpMain;
						wGrpIDSub  = pInfoChar->m_wGrpIDTmpSub;
					}
				}
				pSrc = m_pMgrGrpData->GetDib (wGrpIDMain, 0, wGrpIDSub);
				/* �� */
				pDibTmp->BltFrom256 (0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y + nTmp, TRUE);
				pSrc = m_pMgrGrpData->GetDib (GRPIDMAIN_2X2_HAIR, 0, pInfoChar->m_wGrpIDHairType);
				/* �� */
				pDibTmp->BltFrom256 (0, 0, cxChar, cyChar, pSrc, ptTmp.x, ptTmp.y + nTmp, TRUE);
				if (nDirection != 0) {
					/* �� */
					pSrc = m_pMgrGrpData->GetDib (GRPIDMAIN_2X2_EYE, 0, nEyeID);
					pDibTmp->BltFrom256 (0, 0, cxChar, cyChar, pSrc, ptTmp.x - 32 * 4, ptTmp.y, TRUE);
				}
				hDCBmp = pDibTmp->Lock ();
				StretchBlt (hDCBmp, 0, cyChar * 2, cxChar * 2, cyChar * 2, hDCBmp, 0, 0, cxChar, cyChar, SRCCOPY);
				if (bLock) {
					pDibTmp->Unlock ();
				}
				pDst->BltAlpha (
					x + pInfoMotion->m_ptDrawPosPile0.x,
					y + pInfoMotion->m_ptDrawPosPile0.y,
					cxChar * 2, cyChar * 2, pDibTmp, 0, cyChar * 2, nFadeLevel, TRUE);
			}
			break;
		case 1:		/* �d�ˉ摜�P */
			wGrpIDMain = pInfoMotion->m_wGrpIDMainPile1;
			wGrpIDSub  = pInfoMotion->m_wGrpIDSubPile1;
			if (wGrpIDSub == 0) {
				break;
			}
			pInfoChar->GetGrpID (wGrpIDMain, wGrpIDSub);
			pSrc = m_pMgrGrpData->GetDib (wGrpIDMain, wGrpIDSub - 1, 0);
			if (pSrc == NULL) {
				break;
			}
			cx = cy = m_pMgrGrpData->GetGrpSize (wGrpIDMain);
			nTmp = 0;
			if (cx > cxChar) {
				nTmp = (cx - cxChar) * -1;
			}
			m_pMgrGrpData->GetGrpPos (wGrpIDMain, wGrpIDSub - 1, ptTmp);
			pDibTmp->BltFrom256 (0, 0, cx, cy, pSrc, ptTmp.x, ptTmp.y);

			hDCBmp = pDibTmp->Lock ();
			StretchBlt (hDCBmp, 0, cy * 2, cx * 2, cy * 2, hDCBmp, 0, 0, cx, cy, SRCCOPY);
			if (bLock) {
				pDibTmp->Unlock ();
			}
			pDst->BltAlpha (
				x + nTmp + pInfoMotion->m_ptDrawPosPile1.x,
				y + nTmp + pInfoMotion->m_ptDrawPosPile1.y,
				cx * 2, cy * 2, pDibTmp, 0, cy * 2, pInfoMotion->m_byLevel1, TRUE);
			break;
		case 2:		/* �d�ˉ摜�Q */
			wGrpIDMain = pInfoMotion->m_wGrpIDMainPile2;
			wGrpIDSub  = pInfoMotion->m_wGrpIDSubPile2;
			if (wGrpIDSub == 0) {
				break;
			}
			pInfoChar->GetGrpID (wGrpIDMain, wGrpIDSub);
			pSrc = m_pMgrGrpData->GetDib (wGrpIDMain, wGrpIDSub - 1, 0);
			if (pSrc == NULL) {
				break;
			}
			cx = cy = m_pMgrGrpData->GetGrpSize (wGrpIDMain);
			nTmp = 0;
			if (cx > cxChar) {
				nTmp = (cx - cxChar) * -1;
			}
			m_pMgrGrpData->GetGrpPos (wGrpIDMain, wGrpIDSub - 1, ptTmp);
			pDibTmp->BltFrom256 (0, 0, cx, cy, pSrc, ptTmp.x, ptTmp.y);

			hDCBmp = pDibTmp->Lock ();
			StretchBlt (hDCBmp, 0, cy * 2, cx * 2, cy * 2, hDCBmp, 0, 0, cx, cy, SRCCOPY);
			if (bLock) {
				pDibTmp->Unlock ();
			}
			pDst->BltAlpha (
				x + nTmp + pInfoMotion->m_ptDrawPosPile2.x,
				y + nTmp + pInfoMotion->m_ptDrawPosPile2.y,
				cx * 2, cy * 2, pDibTmp, 0, cy * 2, pInfoMotion->m_byLevel2, TRUE);
			break;
		case 3:		/* �d�ˉ摜�R */
			wGrpIDMain = pInfoMotion->m_wGrpIDMainPile3;
			wGrpIDSub  = pInfoMotion->m_wGrpIDSubPile3;
			if (wGrpIDSub == 0) {
				break;
			}
			pInfoChar->GetGrpID (wGrpIDMain, wGrpIDSub);
			pSrc = m_pMgrGrpData->GetDib (wGrpIDMain, wGrpIDSub - 1, 0);
			if (pSrc == NULL) {
				break;
			}
			cx = cy = m_pMgrGrpData->GetGrpSize (wGrpIDMain);
			nTmp = 0;
			if (cx > cxChar) {
				nTmp = (cx - cxChar) * -1;
			}
			m_pMgrGrpData->GetGrpPos (wGrpIDMain, wGrpIDSub - 1, ptTmp);
			pDibTmp->BltFrom256 (0, 0, cx, cy, pSrc, ptTmp.x, ptTmp.y);

			hDCBmp = pDibTmp->Lock ();
			StretchBlt (hDCBmp, 0, cy * 2, cx * 2, cy * 2, hDCBmp, 0, 0, cx, cy, SRCCOPY);
			if (bLock) {
				pDibTmp->Unlock ();
			}
			pDst->BltAlpha (
				x + nTmp + pInfoMotion->m_ptDrawPosPile3.x,
				y + nTmp + pInfoMotion->m_ptDrawPosPile3.y,
				cx * 2, cy * 2, pDibTmp, 0, cy * 2, pInfoMotion->m_byLevel3, TRUE);
			break;
		}
	}

	/* �G�t�F�N�g */
	pSrc = pInfoChar->GetEfcImg ();
	if (pSrc) {
		pInfoChar->GetEfcGrpPos (ptTmp);
		cx = cy = pInfoChar->GetEfcGrpSize ();
		nTmp = 0;
		if (cx > cxChar) {
			nTmp = (cx - cxChar) * -1;
		}
		pDibTmp->BltFrom256 (0, 0, cx, cy, pSrc, ptTmp.x, ptTmp.y);
		hDCBmp = pDibTmp->Lock ();
		StretchBlt (hDCBmp, 0, cy * 2, cx * 2, cy * 2, hDCBmp, 0, 0, cx, cy, SRCCOPY);
		if (bLock) {
			pDibTmp->Unlock ();
		}
		pDst->BltAlpha (x + nTmp, y + nTmp, cx * 2, cy * 2, pDibTmp, 0, cy * 2, pInfoChar->GetEfcLevel (), TRUE);
	}

Exit:
	ReleaseDibTmp ();
	UnLockDibTmp ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawChar												 */
/* ���e		:�L������`��													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::DrawChar(
	CImg32 *pDst,					/* [in] �`��� */
	int x,							/* [in] �`��ʒu(X) */
	int y,							/* [in] �`��ʒu(Y) */
	BYTE byDirection,				/* [in] ���� */
	BYTE byAnimeNo,					/* [in] �A�j���[�V�����ԍ� */
	BYTE byLevel,					/* [in] �����x */
	CInfoCharCli *pInfoChar)		/* [in] �L������� */
{
	BYTE byAnimeNoTmp;
	int yy, cx, cy;
	PCImg32 pSrc, pDibTmp;
	HDC hDCBmp;

	yy		= 0;
	pSrc	= pInfoChar->m_pDibChar;
	if (pSrc == NULL) {
		return;
	}

	pDibTmp = GetDibTmp ();
	cx = 16;
	cy = 16;

	byAnimeNoTmp = byAnimeNo / 2;
	if ((byAnimeNo % 2) != 0) {
		byAnimeNoTmp = 2;
	}

	pDibTmp->Blt (0, 0, cx, cy, pSrc, (byDirection * 4 + byAnimeNoTmp) * cx, yy);

	hDCBmp = pDibTmp->Lock ();
	StretchBlt (hDCBmp, 0, cy * 2, cx * 2, cy * 2, hDCBmp, 0, 0, cx, cy, SRCCOPY);
	pDibTmp->Unlock ();

	pDst->BltAlpha (x, y, cx * 2, cy * 2, pDibTmp, 0, cy * 2, byLevel, TRUE);
	ReleaseDibTmp ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawMapParts											 */
/* ���e		:�}�b�v�p�[�c��`��												 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::DrawMapParts(
	CImg32 *pDst,				/* [in] �`��� */
	int x,						/* [in] �`��ʒu(X) */
	int y,						/* [in] �`��ʒu(Y) */
	DWORD dwPartsID,			/* [in] �}�b�v�p�[�cID */
	int nMode,					/* [in] �`�惂�[�h(0:���n�̂� 1:�d�ˍ��킹�̂� 2:�S�� 3:�w�i���Əd�Ȃ�Ȃ�S��) */
	BOOL bSingleSize,			/* [in] ���{�w�� */
	BOOL bUseColorKey,/*TRUE*/	/* [in] TRUE:�d�ˍ��킹���� */
	BYTE byLevel)/*0*/			/* [in] ���߃��x�� */
{
	PCInfoMapParts pInfoMapParts;

	pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (dwPartsID);
	DrawMapParts (pDst, x, y, pInfoMapParts, nMode, bSingleSize, bUseColorKey, TRUE, byLevel);
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawMapParts											 */
/* ���e		:�}�b�v�p�[�c��`��												 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::DrawMapParts(
	CImg32 *pDst,						/* [in] �`��� */
	int x,								/* [in] �`��ʒu(X) */
	int y,								/* [in] �`��ʒu(Y) */
	CInfoMapParts *pInfoMapParts,		/* [in] �}�b�v�p�[�c��� */
	int nMode,							/* [in] �`�惂�[�h(0:���n�̂� 1:�d�ˍ��킹�̂� 2:�S�� 3:�w�i���Əd�Ȃ�Ȃ�S��) */
	BOOL bSingleSize,					/* [in] ���{�w�� */
	BOOL bUseColorKey,/*TRUE*/			/* [in] TRUE:�d�ˍ��킹���� */
	BOOL bLock,/*TRUE*/					/* [in] �e���|���������b�N���� */
	BYTE byLevel)/*0*/					/* [in] ���߃��x�� */
{
	int nSize, nSizeDst;
	WORD wGrpIDBase, wGrpIDPile;
	PCImg32 pImg, pDibTmp;
	PCInfoAnime pAnime;
	HDC hDCBmp;

	nSize = 16;
	nSizeDst = 32;
	if (bSingleSize) {
		nSizeDst = 16;
	}

	pDibTmp = GetDibTmp ();
	if(pInfoMapParts == NULL) {
		pDst->FillRect (x, y, nSizeDst, nSizeDst, RGB (0, 0, 0));
		goto Exit;
	}

	wGrpIDBase = pInfoMapParts->m_wGrpIDBase;
	wGrpIDPile = pInfoMapParts->m_wGrpIDPile;
	if (pInfoMapParts->GetAnimeCount () > 0) {
		pAnime = pInfoMapParts->m_paAnimeInfo->GetAt (pInfoMapParts->m_byAnimeNo);
		wGrpIDBase = pAnime->m_wGrpIDBase;
		wGrpIDPile = pAnime->m_wGrpIDPile;
	}
	if (nMode == 0) {
		wGrpIDPile = 0;
	}
	if (nMode == 3) {
		/* �w�i���Əd�Ȃ� */
		if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILEBACK) {
			nMode = 2;
		/* �d�Ȃ� */
		} else if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILE) {
			nMode = 1;
		} else {
			goto Exit;
		}
	}
	if ((nMode == 1) && (wGrpIDPile == 0)) {
		goto Exit;
	}

	if (nMode != 1) {
		pImg = m_pMgrGrpData->GetDibMapParts (wGrpIDBase / 1024);
		pDibTmp->BltFrom256 (0, 0, nSize, nSize, pImg, ((wGrpIDBase % 1024) % 32) * nSize, ((wGrpIDBase % 1024) / 32) * nSize);
	} else {
		pDibTmp->FillRect (0, 0, nSize, nSize, RGB (255, 0, 255));
	}
	if (wGrpIDPile > 0) {
		pImg = m_pMgrGrpData->GetDibMapParts (wGrpIDPile / 1024);
		pDibTmp->BltFrom256 (0, 0, nSize+1, nSize, pImg, ((wGrpIDPile % 1024) % 32) * nSize, ((wGrpIDPile % 1024) / 32) * nSize, TRUE);
	}

	hDCBmp = pDibTmp->Lock ();
	StretchBlt (hDCBmp, 0, nSizeDst, nSizeDst, nSizeDst, hDCBmp, 0, 0, nSize, nSize, SRCCOPY);
	if (bLock) {
		pDibTmp->Unlock ();
	}

	if (byLevel == 0) {
		pDst->Blt (x, y, nSizeDst, nSizeDst, pDibTmp, 0, nSizeDst, bUseColorKey);
	} else {
		pDst->BltAlpha (x, y, nSizeDst, nSizeDst, pDibTmp, 0, nSizeDst, byLevel, bUseColorKey);
	}

Exit:
	ReleaseDibTmp ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawMapShadow										 */
/* ���e		:�}�b�v�e��`��													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CMgrDraw::DrawMapShadow(
	CImg32 *pDst,				/* [in] �`��� */
	int x,						/* [in] �`��ʒu(X) */
	int y,						/* [in] �`��ʒu(Y) */
	DWORD dwShadowID,			/* [in] �}�b�v�eID */
	BOOL bSingleSize,			/* [in] ���{�w�� */
	BOOL bUseColorKey,/*TRUE*/	/* [in] TRUE:�d�ˍ��킹���� */
	BOOL bLock)/*TRUE*/			/* [in] �e���|���������b�N���� */
{
	int nSize, nSizeDst;
	WORD wGrpIDBase;
	PCImg32 pImg, pDibTmp;
	PCInfoMapShadow pInfoMapShadow;
	PCInfoAnime pAnime;
	HDC hDCBmp;

	pDibTmp = GetDibTmp ();
	nSize = 16;
	nSizeDst = 32;
	if (bSingleSize) {
		nSizeDst = 16;
	}

	pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr (dwShadowID);
	if (pInfoMapShadow == NULL) {
		pDst->FillRect (x, y, nSizeDst, nSizeDst, RGB (0, 0, 0));
		goto Exit;
	}

	wGrpIDBase = pInfoMapShadow->m_wGrpID;
	if (pInfoMapShadow->GetAnimeCount () > 0) {
		pAnime = pInfoMapShadow->m_paAnimeInfo->GetAt (pInfoMapShadow->m_byAnimeNo);
		wGrpIDBase = pAnime->m_wGrpIDBase;
	}
	if (bSingleSize) {
		pImg = m_pMgrGrpData->GetDibMapShadowTmp (wGrpIDBase / 1024);
		pDibTmp->BltFrom256 (0, 0, nSize, nSize, pImg, ((wGrpIDBase % 1024) % 32) * nSize, ((wGrpIDBase % 1024) / 32) * nSize);
	} else {
		pImg = m_pMgrGrpData->GetDibMapShadow (wGrpIDBase / 1024);
		pDibTmp->BltFrom256 (0, 0, nSize, nSize, pImg, ((wGrpIDBase % 1024) % 32) * nSize, ((wGrpIDBase % 1024) / 32) * nSize);
	}

	hDCBmp = pDibTmp->Lock ();
	StretchBlt (hDCBmp, 0, nSizeDst, nSizeDst, nSizeDst, hDCBmp, 0, 0, nSize, nSize, SRCCOPY);
	if (bLock) {
		pDibTmp->Unlock ();
	}

	if (bSingleSize) {
		pDst->Blt (x, y, nSizeDst, nSizeDst, pDibTmp, 0, nSizeDst, bUseColorKey);
	} else {
		pDst->BltAlpha (x, y, nSizeDst, nSizeDst, pDibTmp, 0, nSizeDst, 75, TRUE);
	}

Exit:
	ReleaseDibTmp ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawMapObject										 */
/* ���e		:�}�b�v�I�u�W�F�N�g��`��										 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CMgrDraw::DrawMapObject(
	CImg32 *pDst,				/* [in] �`��� */
	int x,						/* [in] �`��ʒu(X) */
	int y,						/* [in] �`��ʒu(Y) */
	DWORD dwObjectID,			/* [in] �I�u�W�F�N�gID */
	BOOL bUseColorKey,/*TRUE*/	/* [in] TRUE:�d�ˍ��킹���� */
	BOOL bLock)/*TRUE*/			/* [in] �e���|���������b�N���� */
{
	int xx, yy;
	SIZE sizeSrc, sizeDst;
	WORD wGrpID;
	PCImg32 pImg, pDibTmp;
	PCInfoMapObject pInfo;
	HDC hDCBmp;
	PSTMAPOBJECTANIMEINFO pAnimeInfo;
	PCLibInfoMapObject pLibInfoMapObject;

	pDibTmp = GetDibTmp ();

	pLibInfoMapObject = m_pMgrData->GetLibInfoMapObject ();
	pInfo = (PCInfoMapObject)pLibInfoMapObject->GetPtr (dwObjectID);
	if (pInfo == NULL) {
		goto Exit;
	}
	pAnimeInfo = pInfo->GetAnimePtr (0);
	if (pAnimeInfo == NULL) {
		goto Exit;
	}

	sizeSrc = pInfo->m_sizeGrp;
	sizeSrc.cx *= 16;
	sizeSrc.cy *= 16;
	sizeDst = sizeSrc;
	sizeDst.cx *= 2;
	sizeDst.cy *= 2;

	for (yy = 0; yy < pInfo->m_sizeGrp.cy; yy ++) {
		for (xx = 0; xx < pInfo->m_sizeGrp.cx; xx ++) {
			wGrpID = pAnimeInfo->pwGrpID[yy * pInfo->m_sizeGrp.cx + xx];
			pImg = m_pMgrGrpData->GetDibMapParts (wGrpID / 1024);
			if (pImg == NULL) {
				continue;
			}
			pDibTmp->BltFrom256 (xx * 16, yy * 16, 16, 16, pImg, ((wGrpID % 1024) % 32) * 16, ((wGrpID % 1024) / 32) * 16);
		}
	}

	hDCBmp = pDibTmp->Lock ();
	StretchBlt (hDCBmp, 0, sizeDst.cy, sizeDst.cx, sizeDst.cy, hDCBmp, 0, 0, sizeSrc.cx, sizeSrc.cy, SRCCOPY);
	if (bLock) {
		pDibTmp->Unlock ();
	}

	pDst->Blt (x, y - (sizeDst.cy - 32), sizeDst.cx, sizeDst.cy, pDibTmp, 0, sizeDst.cy, bUseColorKey);

Exit:
	ReleaseDibTmp ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawItem												 */
/* ���e		:�A�C�e����`��													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CMgrDraw::DrawItem(
	CImg32 *pDst,		/* [in] �`��� */
	int x,				/* [in] �`��ʒu(X) */
	int y,				/* [in] �`��ʒu(Y) */
	DWORD dwItemID,		/* [in] �A�C�e��ID */
	BYTE byLevel)/*0*/	/* [in] �����x */
{
	PCInfoItem pInfoItem;

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	DrawItem (pDst, x, y, pInfoItem, byLevel);
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawItem												 */
/* ���e		:�A�C�e����`��													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CMgrDraw::DrawItem(
	CImg32 *pDst,				/* [in] �`��� */
	int x,						/* [in] �`��ʒu(X) */
	int y,						/* [in] �`��ʒu(Y) */
	CInfoItem *pInfoItem,		/* [in] �A�C�e����� */
	BYTE byLevel,/*0*/			/* [in] �����x */
	BOOL bLock)/*TRUE*/			/* [in] �e���|���������b�N���� */
{
	PCImg32 pImg, pDibTmp;
	HDC hDCBmp;

	pDibTmp = GetDibTmp ();
	if (pInfoItem == NULL) {
		goto Exit;
	}
//Todo:
	pImg = m_pMgrGrpData->GetDibItem (pInfoItem->m_dwGrpID % 1024);
	if (pImg == NULL) {
		goto Exit;
	}
	pDibTmp->BltFrom256 (0, 0, 16, 16, pImg, (pInfoItem->m_dwGrpID % 32) * 16, (pInfoItem->m_dwGrpID / 32) * 16);

	hDCBmp = pDibTmp->Lock ();
	StretchBlt (hDCBmp, 0, 32, 32, 32, hDCBmp, 0, 0, 16, 16, SRCCOPY);
	if (bLock) {
		pDibTmp->Unlock ();
	}

	if (byLevel == 0) {
		pDst->Blt (x, y, 32, 32, pDibTmp, 0, 32, TRUE);
	} else {
		pDst->BltAlpha (x, y, 32, 32, pDibTmp, 0, 32, byLevel, TRUE);
	}

Exit:
	ReleaseDibTmp ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawBalloon											 */
/* ���e		:���o����`��													 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

void CMgrDraw::DrawBalloon(
	CImg32 *pDst,			/* [in] �`��� */
	int x,					/* [in] �`��ʒu(X) */
	int y,					/* [in] �`��ʒu(Y) */
	DWORD dwBalloonID)		/* [in] ���o��ID */
{
	int cx, cy;
	PCImg32 pImg, pDibTmp;
	HDC hDCBmp;
	POINT ptTmp;

	pDibTmp = GetDibTmp ();
	if (dwBalloonID == 0) {
		goto Exit;
	}
	dwBalloonID --;
//Todo:
	pImg = m_pMgrGrpData->GetDib (GRPIDMAIN_EFCBALLOON, (WORD)dwBalloonID, 0);
	if (pImg == NULL) {
		goto Exit;
	}
	cx = cy = m_pMgrGrpData->GetGrpSize (GRPIDMAIN_EFCBALLOON);
	m_pMgrGrpData->GetGrpPos (GRPIDMAIN_EFCBALLOON, (WORD)dwBalloonID, ptTmp);
	pDibTmp->BltFrom256 (0, 0, 16, 16, pImg, ptTmp.x, ptTmp.y);

	hDCBmp = pDibTmp->Lock ();
	StretchBlt (hDCBmp, 0, cy * 2, cx * 2, cy * 2, hDCBmp, 0, 0, cx, cy, SRCCOPY);
	pDibTmp->Unlock ();

	pDst->Blt (x, y, cx * 2, cy * 2, pDibTmp, 0, cy * 2, TRUE);

Exit:
	ReleaseDibTmp ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawTextEffect										 */
/* ���e		:�����G�t�F�N�g��`��											 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CMgrDraw::DrawTextEffect(
	CImg32 *pDst,							/* [in] �`��� */
	int x,									/* [in] �`��ʒu(X) */
	int y,									/* [in] �`��ʒu(Y) */
	CInfoTextEffect *pInfoTextEffect)		/* [in] �����G�t�F�N�g��� */
{
	BOOL bResult;
	BYTE byLevel;
	int cx, cy, i, nCount;
	PCImg32 pImg, pDibTmp;
	HDC hDCBmp;
	POINT ptSrc, ptTmp;

	pDibTmp = GetDibTmp ();
	if (pInfoTextEffect == NULL) {
		goto Exit;
	}

	cx = 10;
	cy = 12;
	pImg = m_pMgrGrpData->GetDibNumS ();
	if (pInfoTextEffect->m_nSize == TEXTEFFECTNUMSIZE_MEDIUM) {
		pImg = m_pMgrGrpData->GetDibNumM ();
		cx = 14;
		cy = 17;
	}
	if (pInfoTextEffect->m_nSize == TEXTEFFECTNUMSIZE_LARGE) {
		pImg = m_pMgrGrpData->GetDibNumL ();
		cx = 28;
		cy = 34;
	}
	if (pImg == NULL) {
		goto Exit;
	}

	ptSrc.y = pInfoTextEffect->m_nColor * cy;

	nCount = pInfoTextEffect->GetGrpCount ();
	pDibTmp->FillRect (0, 0, nCount * cx, cy * 2 + 20, RGB (255, 0, 255));
	for (i = 0; i < nCount; i ++) {
		bResult = pInfoTextEffect->GetDrawPos (i, ptTmp, byLevel);
		if (bResult == FALSE) {
			continue;
		}
		ptSrc.x = pInfoTextEffect->GetGrpNo (i) * cx;
		pDibTmp->BltFrom256 (cx * i + ptTmp.x, cy + ptTmp.y + 20, cx, cy, pImg, ptSrc.x, ptSrc.y);
	}

	hDCBmp = pDibTmp->Lock ();
	pDibTmp->Unlock ();

	if (byLevel == 0) {
		pDst->Blt (x - (cx * nCount / 2), y - cy, (cx * nCount), cy * 2 + 20, pDibTmp, 0, 0, TRUE);
	} else {
		pDst->BltAlpha (x - (cx * nCount / 2), y - cy, (cx * nCount), cy * 2 + 20, pDibTmp, 0, 0, byLevel, TRUE);
	}

Exit:
	ReleaseDibTmp ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawMapHitMark										 */
/* ���e		:�}�b�v�����蔻��}�[�N��`��									 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

void CMgrDraw::DrawMapHitMark(
	CImg32 *pDst,		/* [in] �`��� */
	int x,				/* [in] �`��ʒu(X) */
	int y,				/* [in] �`��ʒu(Y) */
	int nNo)			/* [in] ��� */
{
	CImg32 *pDibSystem;

	pDibSystem = m_pMgrGrpData->GetDibSystem ();
	pDst->Blt (x, y, 32, 32, pDibSystem, nNo * 32, 544, TRUE);
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::DrawCursor											 */
/* ���e		:�w�J�[�\����`��												 */
/* ���t		:2008/11/19														 */
/* ========================================================================= */

void CMgrDraw::DrawCursor(
	CImg32 *pDst,		/* [in] �`��� */
	int x,				/* [in] �`��ʒu(X) */
	int y,				/* [in] �`��ʒu(Y) */
	int nType)			/* [in] ��� */
{
	POINT apt[] = { 48, 0, 72, 0, 48, 24, 72, 24 };
	CImg32 *pDibSystem;

	if (nType > 3) {
		return;
	}

	pDibSystem = m_pMgrGrpData->GetDibSystem ();
	pDst->BltFrom256 (x, y, 24, 24, pDibSystem, apt[nType].x, apt[nType].y, TRUE);
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::TimerProc											 */
/* ���e		:���ԏ���														 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

BOOL CMgrDraw::TimerProc(void)
{
	BOOL bRet;
	DWORD dwTime;

	dwTime = timeGetTime ();

	bRet = Fade ();

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::Fade													 */
/* ���e		:�t�F�[�h����													 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

BOOL CMgrDraw::Fade(void)
{
	int nTmp;
	BOOL bRet;
	DWORD dwTime;
	HWND hWndMain;
	WPARAM wParam;

	bRet = FALSE;
	if (m_byFadeState == FADESTATE_NONE) {
		goto Exit;
	}

	dwTime = timeGetTime ();

	if (m_dwLastFade == 0) {
		m_dwLastFade = dwTime;
	}

	wParam		= 0;
	hWndMain	= m_pMgrData->GetMainWindow ();
	nTmp		= (dwTime - m_dwLastFade) / 4;
	if (nTmp == 0) {
		goto Exit;
	}

	switch (m_byFadeState) {
	case FADESTATE_FADEIN:
		nTmp = min (100, nTmp + m_byLevel);
		if (nTmp == 100) {
			wParam = (WPARAM)MGRDRAWMSG_END_FADEIN;
		}
		break;
	case FADESTATE_FADEOUT:
		nTmp = max (1, m_byLevel - nTmp);
		if (nTmp == 1) {
			wParam = (WPARAM)MGRDRAWMSG_END_FADEOUT;
		}
		break;
	}

	m_byLevel = (BYTE)nTmp;
	m_dwLastFade = dwTime;

	if (wParam != 0) {
		PostMessage (hWndMain, WM_MGRDRAW, wParam, 0);
		SetFadeState (FADESTATE_NONE);
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::GetDibTmp											 */
/* ���e		:�`��e���|�����擾												 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

CImg32 *CMgrDraw::GetDibTmp(void)
{
	m_CritDataLock.Enter ();
	return m_pDibTmp;
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::ReleaseDibTmp										 */
/* ���e		:�`��e���|�������												 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

void CMgrDraw::ReleaseDibTmp(void)
{
	m_CritDataLock.Leave ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::IsFade												 */
/* ���e		:�t�F�[�h�C��/�A�E�g��������									 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

BOOL CMgrDraw::IsFade(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_byFadeState != FADESTATE_NONE) {
		bRet = TRUE;
	}
	if (m_dwLastFade) {
		bRet = TRUE;
	}
	if (m_byLevel < 100 && m_byLevel > 0) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::SetLevel												 */
/* ���e		:���x�ݒ�														 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::SetLevel(BYTE byLevel)
{
	m_byLevel = byLevel;
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::SetFadeState											 */
/* ���e		:���x�ݒ�														 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::SetFadeState(BYTE byFadeState)
{
	HWND hWndMain;

	hWndMain		= m_pMgrData->GetMainWindow ();
	m_byFadeState	= byFadeState;
	m_dwLastFade	= 0;

	switch (m_byFadeState) {
	case FADESTATE_FADEIN:
		if (m_byLevel == 0) {
			SetLevel (1);
		}
		PostMessage (hWndMain, WM_MGRDRAW, MGRDRAWMSG_START_FADEIN, 0);
		break;
	case FADESTATE_FADEOUT:
		if (m_byLevel == 0) {
			SetLevel (100);
		}
		PostMessage (hWndMain, WM_MGRDRAW, MGRDRAWMSG_START_FADEOUT, 0);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::Clear												 */
/* ���e		:�w�i�N���A														 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::Clear(void)
{
	m_pDibBack->Clear ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::SaveScreenShot										 */
/* ���e		:�X�N���[���V���b�g�̕ۑ�										 */
/* ���t		:2006/09/24														 */
/* ========================================================================= */

void CMgrDraw::SaveScreenShot(void)
{
	int cx, cy;
	char szName[MAX_PATH], szTmp[128], szTmp2[20];
	LPSTR pszTmp;
	SYSTEMTIME sysTime;
	CImg32 ImgTmp;

	/* �t�@�C�����̍쐬 */
	GetModuleFileName (NULL, szName, MAX_PATH);
	pszTmp		= strrchr (szName, '\\');
	pszTmp[1]	= 0;

	GetLocalTime (&sysTime);
	strcat (szName, "ss\\Sbo");
	wsprintf (szTmp2, "%04d", sysTime.wYear);
	wsprintf (szTmp, "%s%02d%02d%02d%02d%02d",
			&szTmp2[2],
			sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strcat (szName, szTmp);
	strcat (szName, ".png");

	cx = m_pDibBack->Width ();
	cy = m_pDibBack->Height ();
	ImgTmp.CreateWithoutGdi (SCRSIZEX, SCRSIZEY);
	ImgTmp.Blt (0, 0, SCRSIZEX, SCRSIZEY, m_pDibBack, (cx - SCRSIZEX) / 2, (cy - SCRSIZEY) / 2);
	m_pMgrGrpData->Write (szName, &ImgTmp);
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::LockDibTmp											 */
/* ���e		:�`��e���|���������b�N											 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

void CMgrDraw::LockDibTmp(void)
{
	GetDibTmp ();
	m_pDibTmp->Lock ();
}


/* ========================================================================= */
/* �֐���	:CMgrDraw::UnLockDibTmp											 */
/* ���e		:�`��e���|�����̃��b�N������									 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

void CMgrDraw::UnLockDibTmp(void)
{
//	m_pDibTmp->Unlock ();
	ReleaseDibTmp ();
}

/* Copyright(C)URARA-works 2005 */
