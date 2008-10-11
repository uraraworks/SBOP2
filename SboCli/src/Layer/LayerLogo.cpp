/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:LayerLogo.cpp												 */
/* ���e			:���C���[�`��N���X(���S) �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/05/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerLogo.h"


/* ========================================================================= */
/* �֐���	:CLayerLogo::CLayerLogo											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/05/17														 */
/* ========================================================================= */

CLayerLogo::CLayerLogo()
{
	m_nID		= LAYERTYPE_LOGO;
	m_pDibLogo	= NULL;
}


/* ========================================================================= */
/* �֐���	:CLayerLogo::~CLayerLogo										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/05/17														 */
/* ========================================================================= */

CLayerLogo::~CLayerLogo()
{
	SAFE_DELETE (m_pDibLogo);
}


/* ========================================================================= */
/* �֐���	:CLayerLogo::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2005/05/21														 */
/* ========================================================================= */

void CLayerLogo::Create(
	CMgrData	*pMgrData)		/* [in] �f�[�^�Ǘ� */
{
	CLayerBase::Create (pMgrData);

	m_pDibLogo = m_pMgrGrpData->GetDibTmpLogo ();
}


/* ========================================================================= */
/* �֐���	:CLayerLogo::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2005/05/17														 */
/* ========================================================================= */

void CLayerLogo::Draw(PCImg32 pDst)
{
	int cx, cy;
	HDC hDCBmp, hDCBmp2;

	cx = m_pDibBase->Width ();
	cy = m_pDibBase->Height ();
	m_pDibBase->FillRect (0, 0, cx, cy, RGB (255, 255, 255));
	m_pDibBase->BltFrom256 (cx / 2 - 104, cy / 2 - 40, 208, 80, m_pDibLogo, 0, 0, TRUE);

	hDCBmp2	= m_pDibBase->Lock ();
	hDCBmp	= pDst->Lock ();

	StretchBlt (hDCBmp, 32, 32, pDst->Width () - 64, pDst->Height () - 64,
			hDCBmp2, 0, 0, m_pDibBase->Width (), m_pDibBase->Height (), SRCCOPY);

	m_pDibBase->Unlock ();
	pDst->Unlock ();
}

/* Copyright(C)URARA-works 2005 */
