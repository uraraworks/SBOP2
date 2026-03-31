/// @file WindowCHARNAME.h
/// @brief キャラ名入力ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/08
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

typedef class CWindowCHARNAME : public CWindowBase
{
public:
			CWindowCHARNAME();	// コンストラクタ
	virtual ~CWindowCHARNAME();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
	void	SetActive(BOOL bActive);	// アクティブか設定


private:
	void	MakeWindow(void);	// ウィンドウ作成
	static LRESULT CALLBACK CharNameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	// キャラ名入力欄プロシージャ


public:
	CmyString	m_strName;	// 入力されたキャラ名


private:
	HWND	m_hWndCharName;	// キャラ名入力欄

	WNDPROC	m_OrgWndProcCharName;	// キャラ名入力欄の元ウィンドウプロシージャ
} CWindowCHARNAME, *PCWindowCHARNAME;
