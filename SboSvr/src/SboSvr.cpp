/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:SboSvr.cpp													 */
/* 内容			:サーバーメイン処理 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MainFrame.h"

/* ========================================================================= */
/* 関数名	:WinMain														 */
/* 内容		:処理の始まり													 */
/* 日付		:2006/11/04														 */
/* ========================================================================= */

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
	int nRet;
	CMainFrame MainFrame;

	nRet = MainFrame.MainLoop (hInstance);

	return nRet;
}

/* Copyright(C)URARA-works 2006 */
