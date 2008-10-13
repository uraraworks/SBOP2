/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LayerMisty.cpp												 */
/* ���e			:���C���[�`��N���X(�_) �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/13													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "InfoMapBase.h"
#include "LayerMisty.h"


/* ========================================================================= */
/* �֐���	:CLayerMisty::CLayerMisty										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/10/13														 */
/* ========================================================================= */

CLayerMisty::CLayerMisty()
{
	m_nID = WEATHERTYPE_MISTY;
}


/* ========================================================================= */
/* �֐���	:CLayerMisty::~CLayerMisty										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/10/13														 */
/* ========================================================================= */

CLayerMisty::~CLayerMisty()
{
}


/* ========================================================================= */
/* �֐���	:CLayerMisty::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2008/10/13														 */
/* ========================================================================= */

void CLayerMisty::Create(
	CMgrData	*pMgrData)		/* [in] �f�[�^�Ǘ� */
{
	CLayerBase::Create (pMgrData);

	m_pDibBase->FillRect (0, 0, m_pDibBase->Width (), m_pDibBase->Height (), RGB (255, 255, 255));
}


/* ========================================================================= */
/* �֐���	:CLayerMisty::Draw												 */
/* ���e		:�`��															 */
/* ���t		:2008/10/13														 */
/* ========================================================================= */

void CLayerMisty::Draw(PCImg32 pDst)
{
	int cx, cy;

	cx = m_pDibBase->Width ();
	cy = m_pDibBase->Height ();

	pDst->BltAlpha (32,		 32,	  cx, cy, m_pDibBase, 0, 0, 50, TRUE);
	pDst->BltAlpha (32 + cx, 32,	  cx, cy, m_pDibBase, 0, 0, 50, TRUE);
	pDst->BltAlpha (32,		 32 + cx, cx, cy, m_pDibBase, 0, 0, 50, TRUE);
	pDst->BltAlpha (32 + cx, 32 + cx, cx, cy, m_pDibBase, 0, 0, 50, TRUE);
}

/* Copyright(C)URARA-works 2008 */
