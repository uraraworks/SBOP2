/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:StateProcLOGO.cpp											 */
/* ���e			:��ԏ����N���X(���S) �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/05/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "MgrSound.h"
#include "StateProcLOGO.h"


/* ========================================================================= */
/* �֐���	:CStateProcLOGO::CStateProcLOGO									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

CStateProcLOGO::CStateProcLOGO()
{
	m_dwTimeWait = 0;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGO::~CStateProcLOGO								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

CStateProcLOGO::~CStateProcLOGO()
{
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGO::Init											 */
/* ���e		:������															 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

void CStateProcLOGO::Init(void)
{
	m_pMgrDraw->SetLevel (1);
	m_pMgrDraw->SetFadeState (FADESTATE_FADEIN);
	m_pMgrLayer->MakeLOGO ();
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGO::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

BOOL CStateProcLOGO::TimerProc(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_dwTimeWait == 0) {
		goto Exit;
	}

	if (timeGetTime () - m_dwTimeWait < 3000) {
		goto Exit;
	}

	m_pMgrDraw->SetFadeState (FADESTATE_FADEOUT);
	m_dwTimeWait = 0;

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGO::OnLButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

void CStateProcLOGO::OnLButtonDown(int x, int y)
{
	m_pMgrDraw->SetFadeState (FADESTATE_FADEOUT);
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGO::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2007/03/21														 */
/* ========================================================================= */

BOOL CStateProcLOGO::OnX(BOOL bDown)
{
	OnLButtonDown (0, 0);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGO::OnZ											 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2007/03/21														 */
/* ========================================================================= */

BOOL CStateProcLOGO::OnZ(BOOL bDown)
{
	OnLButtonDown (0, 0);

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGO::OnMgrDrawEND_FADEIN							 */
/* ���e		:�t�F�[�h�C������												 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

void CStateProcLOGO::OnMgrDrawEND_FADEIN(DWORD dwPara)
{
	m_dwTimeWait = timeGetTime ();
	m_pMgrSound->PlaySound (SOUNDID_URARA_WORKS);
}


/* ========================================================================= */
/* �֐���	:CStateProcLOGO::OnMgrDrawEND_FADEOUT							 */
/* ���e		:�t�F�[�h�A�E�g����												 */
/* ���t		:2005/05/29														 */
/* ========================================================================= */

void CStateProcLOGO::OnMgrDrawEND_FADEOUT(DWORD dwPara)
{
	PostMessage (m_hWndMain, WM_MAINFRAME, MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGIN);
}

/* Copyright(C)URARA-works 2005 */
