/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:LayerInfo.cpp												 */
/* ���e			:���C���[�`��N���X(���m�点) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/08/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerInfo.h"


/* ========================================================================= */
/* �֐���	:CLayerInfo::CLayerInfo											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/08/16														 */
/* ========================================================================= */

CLayerInfo::CLayerInfo()
{
	m_nID			= LAYERTYPE_INFO;
	m_nX			= 64;
	m_nY			= 64;
	m_nCx			= 24;
	m_nCy			= 24;
	m_pDibSystem	= NULL;
}


/* ========================================================================= */
/* �֐���	:CLayerInfo::~CLayerInfo										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/08/16														 */
/* ========================================================================= */

CLayerInfo::~CLayerInfo()
{
}


/* ========================================================================= */
/* �֐����FCLayerInfo::Create												 */
/* ���e�F�쐬																 */
/* ���t�F2005/08/16															 */
/* ========================================================================= */

void CLayerInfo::Create(
	CMgrData *pMgrData)		/* [in] �f�[�^�Ǘ� */
{
	CLayerBase::Create (pMgrData);
}


/* ========================================================================= */
/* �֐���	:CLayerInfo::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2005/08/16														 */
/* ========================================================================= */

void CLayerInfo::Draw(PCImg32 pDst)
{
	char szTmp[128];
	HFONT hFontOld;
	HDC hDCTmp;

	DrawFrame (pDst);

	hDCTmp = pDst->Lock ();
	hFontOld = (HFONT)SelectObject (hDCTmp, m_hFont);
	SetBkMode (hDCTmp, TRANSPARENT);

	wsprintf (szTmp, "���m�点");
	TextOut1 (hDCTmp, m_nX + 16, m_nY + 32, szTmp, RGB (0, 0, 0));

	SelectObject (hDCTmp, hFontOld);
	pDst->Unlock ();
}



/* ========================================================================= */
/* �֐���	:CLayerInfo::DrawFrame											 */
/* ���e		:�t���[���̕`��													 */
/* ���t		:2005/08/16														 */
/* ========================================================================= */

void CLayerInfo::DrawFrame(PCImg32 pDst)
{
	int i, x, y, cx, cy;

	x = m_nX;
	y = m_nY;
	cx = m_nCx;
	cy = m_nCy;

	pDst->BltFrom256 (x, y, 16, 16, m_pDibSystem, 0, 48, TRUE);
	for (i = 0; i < cx; i ++) {
		pDst->BltFrom256 (x + 16 + (i * 16), y, 16, 16, m_pDibSystem, 16, 48, TRUE);
	}
	pDst->BltFrom256 (x + 16 + (i * 16), y, 16, 16, m_pDibSystem, 32, 48, TRUE);

	y += 16;
	for (i = 0; i < cy; i ++) {
		pDst->BltFrom256 (x, y + (i * 16), 16, 16, m_pDibSystem, 0, 64, TRUE);
		pDst->BltFrom256 (x + 16 + (cx * 16), y + (i * 16), 16, 16, m_pDibSystem, 32, 64, TRUE);
	}

	y += (i * 16);
	pDst->BltFrom256 (x, y, 16, 16, m_pDibSystem, 0, 80, TRUE);
	for (i = 0; i < cx; i ++) {
		pDst->BltFrom256 (x + 16 + (i * 16), y, 16, 16, m_pDibSystem, 16, 80, TRUE);
	}
	pDst->BltFrom256 (x + 16 + (i * 16), y, 16, 16, m_pDibSystem, 32, 80, TRUE);

	pDst->FillRect (m_nX + 16, m_nY + 16, cx * 16, cy * 16, RGB (255, 235, 200));
}

/* Copyright(C)URARA-works 2005 */
