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
	virtual BOOL	HandleSDLKeyDown(UINT vk);	// SDLキー押下を処理
	virtual void	HandleSDLTextInput(LPCSTR pszText);	// SDLテキスト入力を処理
	virtual BOOL	HandleSDLMouseLeftButtonDown(int x, int y);	// SDL左クリックを処理


private:
	void	MakeWindow(void);	// ウィンドウ作成
	void	UpdateSDLTextInput(void);	// SDLテキスト入力状態を更新
	void	DeleteBackward(void);	// 1文字削除
	void	AppendText(LPCSTR pszText);	// テキスト追記
	void	SubmitCharName(void);	// キャラ名確定
	static LRESULT CALLBACK CharNameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	// キャラ名入力欄プロシージャ


public:
	CmyString	m_strName;	// 入力されたキャラ名


private:
	BOOL	m_bTextInputActive;	// SDLテキスト入力状態
	HWND	m_hWndCharName;	// キャラ名入力欄

	WNDPROC	m_OrgWndProcCharName;	// キャラ名入力欄の元ウィンドウプロシージャ
} CWindowCHARNAME, *PCWindowCHARNAME;
