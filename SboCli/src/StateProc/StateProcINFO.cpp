/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:StateProcINFO.cpp											 */
/* ���e			:��ԏ����N���X(���m�点) �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/08/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "StateProcINFO.h"


/* ========================================================================= */
/* �֐���	:CStateProcINFO::CStateProcINFO									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/08/16														 */
/* ========================================================================= */

CStateProcINFO::CStateProcINFO()
{
}


/* ========================================================================= */
/* �֐���	:CStateProcINFO::~CStateProcINFO								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/08/16														 */
/* ========================================================================= */

CStateProcINFO::~CStateProcINFO()
{
}


/* ========================================================================= */
/* �֐���	:CStateProcINFO::Init											 */
/* ���e		:������															 */
/* ���t		:2005/08/16														 */
/* ========================================================================= */

void CStateProcINFO::Init(void)
{
	m_pMgrLayer->MakeTITLE ();
	m_pMgrLayer->MakeINFO ();
}


/* ========================================================================= */
/* �֐���	:CStateProcINFO::OnLButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2005/08/16														 */
/* ========================================================================= */

void CStateProcINFO::OnLButtonDown(int x, int y)
{
	PostMessage (m_hWndMain, WM_MAINFRAME, MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGINMENU);
}


/* ========================================================================= */
/* �֐���	:CStateProcINFO::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2005/08/21														 */
/* ========================================================================= */

BOOL CStateProcINFO::OnX(BOOL bDown)
{
	if (bDown == TRUE) {
		goto Exit;
	}

	OnLButtonDown (0, 0);

Exit:
	return FALSE;
}

/* Copyright(C)URARA-works 2005 */
