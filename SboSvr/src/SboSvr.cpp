/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:SboSvr.cpp													 */
/* ���e			:�T�[�o�[���C������ �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MainFrame.h"

/* ========================================================================= */
/* �֐���	:WinMain														 */
/* ���e		:�����̎n�܂�													 */
/* ���t		:2006/11/04														 */
/* ========================================================================= */

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
	int nRet;
	CMainFrame MainFrame;

	nRet = MainFrame.MainLoop (hInstance);

	return nRet;
}

/* Copyright(C)URARA-works 2006 */
