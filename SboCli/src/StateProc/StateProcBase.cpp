/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:StateProcBase.cpp											 */
/* ���e			:��ԏ������N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "MgrWindow.h"
#include "MgrKeyInput.h"
#include "StateProcBase.h"


/* ========================================================================= */
/* �֐���	:CStateProcBase::CStateProcBase									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CStateProcBase::CStateProcBase()
{
	m_hWndMain		= NULL;
	m_hWndAdmin		= NULL;

	m_pSock			= NULL;
	m_pMgrData		= NULL;
	m_pMgrSound		= NULL;
	m_pMgrDraw		= NULL;
	m_pMgrLayer		= NULL;
	m_pMgrWindow	= NULL;
	m_pMgrKeyInput	= NULL;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::~CStateProcBase								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

CStateProcBase::~CStateProcBase()
{
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::Create(CMgrData *pMgrData, CUraraSockTCPSBO *pSock)
{
	m_pSock			= pSock;
	m_pMgrData		= pMgrData;
	m_pMgrSound		= m_pMgrData->GetMgrSound ();
	m_hWndMain		= m_pMgrData->GetMainWindow ();
	m_hWndAdmin		= m_pMgrData->GetAdminWindow ();
	m_pMgrDraw		= m_pMgrData->GetMgrDraw ();
	m_pMgrLayer		= m_pMgrData->GetMgrLayer ();
	m_pMgrWindow	= m_pMgrData->GetMgrWindow ();
	m_pMgrKeyInput	= m_pMgrData->GetMgrKeyInput ();
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::Init											 */
/* ���e		:������															 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::Init(void)
{
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::TimerProc										 */
/* ���e		:���ԏ���														 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::TimerProc(void)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::KeyProc										 */
/* ���e		:�L�[����														 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::KeyProc(
	BYTE byCode,		/* [in] �C�x���g */
	BOOL bDown)			/* [in] ������� */
{
	BOOL bResult;

	switch (byCode) {
	case 'X':			bResult = OnX		(bDown);	break;
	case 'Z':			bResult = OnZ		(bDown);	break;
	case 'S':			bResult = OnS		(bDown);	break;
	case 'A':			bResult = OnA		(bDown);	break;
	case 'R':			bResult = OnR		(bDown);	break;
	case 'B':			bResult = OnB		(bDown);	break;
	case 'N':			bResult = OnN		(bDown);	break;
	case 'V':			bResult = OnV		(bDown);	break;
	case VK_OEM_3:		bResult = OnAt		(bDown);	break;
	case VK_UP:			bResult = OnUp		(bDown);	break;
	case VK_DOWN:		bResult = OnDown	(bDown);	break;
	case VK_RIGHT:		bResult = OnRight	(bDown);	break;
	case VK_LEFT:		bResult = OnLeft	(bDown);	break;
	case VK_RETURN:		bResult = OnEnter	(bDown);	break;
	case VK_ESCAPE:		bResult = OnEscape	(bDown);	break;
	case VK_TAB:		bResult = OnTab		(bDown);	break;
	case VK_SHIFT:		bResult = OnShift	(bDown);	break;
	case VK_CONTROL:	bResult = OnCtrl	(bDown);	break;
	case VK_SPACE:		bResult = OnSpace	(bDown);	break;
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnLButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::OnLButtonDown(int x, int y)
{
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnRButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDOWN)								 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CStateProcBase::OnRButtonDown(int x, int y)
{
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnRButtonDblClk								 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDBLCLK)							 */
/* ���t		:2007/09/16														 */
/* ========================================================================= */

void CStateProcBase::OnRButtonDblClk(int x, int y)
{
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnMouseMove									 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEMOVE)								 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CStateProcBase::OnMouseMove(int x, int y)
{
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnKeyDown										 */
/* ���e		:���b�Z�[�W�n���h��(WM_KEYDOWN)									 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::OnKeyDown(UINT vk)
{
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnWindowMsg									 */
/* ���e		:���b�Z�[�W�n���h��(WM_WINDOWMSG)								 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CStateProcBase::OnWindowMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnMainFrame									 */
/* ���e		:���b�Z�[�W�n���h��(WM_MAINFRAME)								 */
/* ���t		:2006/11/03														 */
/* ========================================================================= */

void CStateProcBase::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnAdminMsg										 */
/* ���e		:���b�Z�[�W�n���h��(WM_ADMINMSG)								 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CStateProcBase::OnAdminMsg(int nCode, DWORD dwPara)
{
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnMgrDraw										 */
/* ���e		:���b�Z�[�W�n���h��(WM_MGRDRAW)									 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

void CStateProcBase::OnMgrDraw(int nCode, DWORD dwPara)
{
	switch (nCode) {
	case MGRDRAWMSG_START_FADEIN:		OnMgrDrawSTART_FADEIN	(dwPara);	break;		/* �t�F�[�h�C���J�n */
	case MGRDRAWMSG_START_FADEOUT:		OnMgrDrawSTART_FADEOUT	(dwPara);	break;		/* �t�F�[�h�A�E�g�J�n */
	case MGRDRAWMSG_END_FADEIN:			OnMgrDrawEND_FADEIN		(dwPara);	break;		/* �t�F�[�h�C������ */
	case MGRDRAWMSG_END_FADEOUT:		OnMgrDrawEND_FADEOUT	(dwPara);	break;		/* �t�F�[�h�A�E�g���� */
	}
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnUp											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnUp(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnDown											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnDown(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnLeft											 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnLeft(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnRight										 */
/* ���e		:�L�[�n���h��(��)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnRight(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnX											 */
/* ���e		:�L�[�n���h��(X)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnX(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnZ											 */
/* ���e		:�L�[�n���h��(Z)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnZ(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnS											 */
/* ���e		:�L�[�n���h��(S)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnS(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnA											 */
/* ���e		:�L�[�n���h��(A)												 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnA(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnB											 */
/* ���e		:�L�[�n���h��(B)												 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CStateProcBase::OnB(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnR											 */
/* ���e		:�L�[�n���h��(R)												 */
/* ���t		:2007/04/20														 */
/* ========================================================================= */

BOOL CStateProcBase::OnR(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnI											 */
/* ���e		:�L�[�n���h��(I)												 */
/* ���t		:2007/07/30														 */
/* ========================================================================= */

BOOL CStateProcBase::OnI(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnN											 */
/* ���e		:�L�[�n���h��(N)												 */
/* ���t		:2008/07/14														 */
/* ========================================================================= */

BOOL CStateProcBase::OnN(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnV											 */
/* ���e		:�L�[�n���h��(V)												 */
/* ���t		:2008/11/18														 */
/* ========================================================================= */

BOOL CStateProcBase::OnV(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnAt											 */
/* ���e		:�L�[�n���h��(@)												 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

BOOL CStateProcBase::OnAt(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnEnter										 */
/* ���e		:�L�[�n���h��(Enter)											 */
/* ���t		:2006/10/01														 */
/* ========================================================================= */

BOOL CStateProcBase::OnEnter(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnEscape										 */
/* ���e		:�L�[�n���h��(Escape)											 */
/* ���t		:2007/06/19														 */
/* ========================================================================= */

BOOL CStateProcBase::OnEscape(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnTab											 */
/* ���e		:�L�[�n���h��(Tab)												 */
/* ���t		:2007/07/08														 */
/* ========================================================================= */

BOOL CStateProcBase::OnTab(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnShift										 */
/* ���e		:�L�[�n���h��(Shift)											 */
/* ���t		:2007/07/28														 */
/* ========================================================================= */

BOOL CStateProcBase::OnShift(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnCtrl											 */
/* ���e		:�L�[�n���h��(Ctrl)												 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

BOOL CStateProcBase::OnCtrl(BOOL bDown)
{
	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CStateProcBase::OnSpace										 */
/* ���e		:�L�[�n���h��(Space)											 */
/* ���t		:2008/08/02														 */
/* ========================================================================= */

BOOL CStateProcBase::OnSpace(BOOL bDown)
{
	return FALSE;
}

/* Copyright(C)URARA-works 2006 */
