/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LayerLoginMenu.cpp											 */
/* ���e			:���C���[�`��N���X(���O�C�����j���[) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/15													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerLoginMenu.h"


/* ========================================================================= */
/* �֐���	:CLayerLoginMenu::CLayerLoginMenu								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/15														 */
/* ========================================================================= */

CLayerLoginMenu::CLayerLoginMenu()
{
	m_nID				= LAYERTYPE_LOGINMENU;
	m_dwLastTimeProc	= 0;
	m_pDibBack			= NULL;
}


/* ========================================================================= */
/* �֐���	:CLayerLoginMenu::~CLayerLoginMenu								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/15														 */
/* ========================================================================= */

CLayerLoginMenu::~CLayerLoginMenu()
{
	SAFE_DELETE (m_pDibBack);
}


/* ========================================================================= */
/* �֐���	:CLayerLoginMenu::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2005/05/21														 */
/* ========================================================================= */

void CLayerLoginMenu::Create(
	CMgrData	*pMgrData)		/* [in] �f�[�^�Ǘ� */
{
	CLayerCloud::Create (pMgrData);

	m_pDibBack = m_pMgrGrpData->GetDibTmpLoginMenuBack ();
}


/* ========================================================================= */
/* �֐���	:CLayerLoginMenu::Draw											 */
/* ���e		:�`��															 */
/* ���t		:2007/04/15														 */
/* ========================================================================= */

void CLayerLoginMenu::Draw(PCImg32 pDst)
{
	pDst->Blt (32, 32, m_pDibBack->Width (), m_pDibBack->Height (), m_pDibBack, 0, 0);
	CLayerCloud::Draw (pDst);

	pDst->BltFrom256 (32, SCRSIZEY - 32, 112, 32, m_pDibSystem, 688,  80, TRUE);
	pDst->BltFrom256 (32, SCRSIZEY	   ,  43, 16, m_pDibSystem, 688, 112, TRUE);
	pDst->BltFrom256 (32, SCRSIZEY + 16,  43, 16, m_pDibSystem, 688, 128, TRUE);
}

/* Copyright(C)URARA-works 2007 */
