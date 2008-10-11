/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:StateProcLOGIN.cpp											 */
/* ���e			:��ԏ����N���X(���O�C�����) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "WindowLOGIN.h"
#include "LayerTitle.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "StateProcLOGIN.h"

/* ========================================================================= */
/* �֐���	:CStateProcLOGIN::CStateProcLOGIN								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

CStateProcLOGIN::CStateProcLOGIN()
{
	m_bFadeIn = FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGIN::~CStateProcLOGIN								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

CStateProcLOGIN::~CStateProcLOGIN()
{
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGIN::Init											 */
/* ���e		:������															 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

void CStateProcLOGIN::Init(void)
{
	PCWindowLOGIN pWindow;

	m_pMgrWindow->	MakeWindowLOGIN ();
	m_pMgrDraw->	SetFadeState (FADESTATE_FADEIN);
	m_pMgrLayer->	MakeTITLE ();

	pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGIN);
	pWindow->SetShow (FALSE);

	m_pMgrSound->PlayBGM (BGMID_HISYOU);
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGIN::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2007/03/26														 */
/* ========================================================================= */

BOOL CStateProcLOGIN::TimerProc(void)
{
	PCLayerTitle pLayer;
	PCWindowLOGIN pWindow;

	if (m_bFadeIn) {
		return FALSE;
	}

	pLayer = (PCLayerTitle)m_pMgrLayer->Get (LAYERTYPE_TITLE);
	if (pLayer->IsFadeInEnd ()) {
		m_bFadeIn = TRUE;

		pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGIN);
		pWindow->SetShow (TRUE);
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGIN::OnLButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/04/02														 */
/* ========================================================================= */

void CStateProcLOGIN::OnLButtonDown(int x, int y)
{
	PCLayerTitle pLayer;

	pLayer = (PCLayerTitle)m_pMgrLayer->Get (LAYERTYPE_TITLE);
	pLayer->EndFadeIn ();
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGIN::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2007/04/02														 */
/* ========================================================================= */

BOOL CStateProcLOGIN::OnX(BOOL bDown)
{
	OnLButtonDown (0, 0);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGIN::OnMgrDrawSTART_FADEIN							 */
/* ���e		:�t�F�[�h�C���J�n												 */
/* ���t		:2007/03/26														 */
/* ========================================================================= */

void CStateProcLOGIN::OnMgrDrawSTART_FADEIN(DWORD dwPara)
{
	PCLayerTitle pLayer;

	pLayer = (PCLayerTitle)m_pMgrLayer->Get (LAYERTYPE_TITLE);
	pLayer->StartFadeIn ();
}

/* Copyright(C)URARA-works 2006 */
