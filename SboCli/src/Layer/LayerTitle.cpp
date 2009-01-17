/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:LayerTitle.cpp												 */
/* ���e			:���C���[�`��N���X(�^�C�g��) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/03/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerTitle.h"


/* ========================================================================= */
/* �֐���	:CLayerTitle::CLayerTitle										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/03/26														 */
/* ========================================================================= */

CLayerTitle::CLayerTitle()
{
	m_nID				= LAYERTYPE_TITLE;
	m_nAnime			= 0;
	m_nFadeLevel		= 0;
	m_nCloudPos			= 1000;
	m_dwLastTimeProc	= 0;
	m_dwLastTimeFadeIn	= 0;
	m_pDibTitle			= NULL;
	m_pDibTitleBack		= NULL;
	m_pDibTitleCloud	= NULL;
}


/* ========================================================================= */
/* �֐���	:CLayerTitle::~CLayerTitle										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/03/26														 */
/* ========================================================================= */

CLayerTitle::~CLayerTitle()
{
	SAFE_DELETE (m_pDibTitle);
	SAFE_DELETE (m_pDibTitleBack);
	SAFE_DELETE (m_pDibTitleCloud);
}


/* ========================================================================= */
/* �֐���	:CLayerTitle::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2005/05/21														 */
/* ========================================================================= */

void CLayerTitle::Create(
	CMgrData	*pMgrData)		/* [in] �f�[�^�Ǘ� */
{
	CLayerBase::Create (pMgrData);

	m_pDibTitle			= m_pMgrGrpData->GetDibTmpTitle ();
	m_pDibTitleBack		= m_pMgrGrpData->GetDibTmpTitleBack ();
	m_pDibTitleCloud	= m_pMgrGrpData->GetDibTmpTitleCloud ();
}


/* ========================================================================= */
/* �֐���	:CLayerTitle::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2007/03/26														 */
/* ========================================================================= */

void CLayerTitle::Draw(PCImg32 pDst)
{
	int i, cx, cy, nTmp, x;
	HDC hDCTmp;
	HFONT hFontOld;
	CmyString strTmp;

	nTmp = 100 - (m_nFadeLevel / 2);
	if (m_nFadeLevel > 100) {
		nTmp = 0;
	}
	pDst->BltFrom256 (32, 32, 480, 480, m_pDibTitleBack, 0, 0);

	for (i = 0; i < 6; i ++) {
		x = (m_nCloudPos / (6 - i));
		x = (x % 780);
		x -= 260;
		x += 32;
		pDst->BltFrom256 (x, 32 + 96, 260, 112, m_pDibTitleCloud, 0, i * 112, TRUE);
	}

	if (m_nFadeLevel > 100) {
		cx = m_pDibTitle->Width ();
		cy = m_pDibTitle->Height ();
		nTmp = 100 - (m_nFadeLevel - 100);
		pDst->BltAlphaFrom256 (SCRSIZEX / 2 - (cx / 2) + 32, 80, cx, cy, m_pDibTitle, 0, 0, nTmp, TRUE);
	}

	if (m_dwLastTimeFadeIn == 0) {
		hDCTmp = pDst->Lock ();
		hFontOld = (HFONT)SelectObject (hDCTmp, m_hFont);
		SetBkMode (hDCTmp, TRANSPARENT);
		strTmp = "Copyright (C)2003-2009 URARA-WORKS. All rights reserved.";
		TextOut1 (hDCTmp, (480 - (strTmp.GetLength () * 6)) / 2 + 32, SCRSIZEY - 12 + 32, strTmp, RGB (255, 255, 255));
		SelectObject (hDCTmp, hFontOld);
		pDst->Unlock ();
	}
}


/* ========================================================================= */
/* �֐���	:CLayerTitle::TimerProc											 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/03/26														 */
/* ========================================================================= */

BOOL CLayerTitle::TimerProc(void)
{
	BOOL bRet;
	DWORD dwTime, dwTmp;

	bRet	= CLayerBase::TimerProc ();
	dwTime	= timeGetTime ();

	if (m_dwLastTimeFadeIn != 0) {
		dwTmp = dwTime - m_dwLastTimeFadeIn;
		if (dwTmp > 50) {
			m_nFadeLevel += (dwTmp / 10);
			if (m_nFadeLevel > 200) {
				m_nFadeLevel		= 200;
				m_dwLastTimeFadeIn	= 0;
			} else {
				m_dwLastTimeFadeIn = dwTime;
			}
			bRet = TRUE;
		}
	}
	dwTmp = dwTime - m_dwLastTimeProc;
	if (dwTmp > 100) {
		m_dwLastTimeProc = dwTime;
		m_nCloudPos ++;
		if (m_nCloudPos <= 0) {
			m_nCloudPos = 0;
		}
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLayerTitle::StartFadeIn										 */
/* ���e		:�t�F�[�h�C���J�n												 */
/* ���t		:2007/03/26														 */
/* ========================================================================= */

void CLayerTitle::StartFadeIn(void)
{
	m_dwLastTimeFadeIn = timeGetTime ();
}


/* ========================================================================= */
/* �֐���	:CLayerTitle::EndFadeIn											 */
/* ���e		:�t�F�[�h�C���I��												 */
/* ���t		:2007/04/02														 */
/* ========================================================================= */

void CLayerTitle::EndFadeIn(void)
{
	m_nFadeLevel		= 200;
	m_dwLastTimeFadeIn	= 0;
}


/* ========================================================================= */
/* �֐���	:CLayerTitle::IsFadeInEnd										 */
/* ���e		:�t�F�[�h�C���I���H												 */
/* ���t		:2007/03/26														 */
/* ========================================================================= */

BOOL CLayerTitle::IsFadeInEnd(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_nFadeLevel >= 200) {
		bRet = TRUE;
	}
	return bRet;
}

/* Copyright(C)URARA-works 2005 */
