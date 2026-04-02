/// @file WindowCHAT.h
/// @brief チャットウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/02/03
/// @copyright Copyright(C)URARA-works 2007

#pragma once
#include <vector>

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

typedef class CWindowCHAT : public CWindowBase
{
public:
			CWindowCHAT();	// コンストラクタ
	virtual ~CWindowCHAT();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
	void	SetActive(BOOL bActive);	// アクティブか設定
	virtual BOOL	HandleSDLKeyDown(UINT vk);	// SDLキー押下を処理
	virtual void	HandleSDLTextInput(LPCSTR pszText);	// SDLテキスト入力を処理
	virtual BOOL	HandleSDLMouseLeftButtonDown(int x, int y);	// SDL左クリックを処理
	int	GetType(void);	// チャット種別を取得


private:
	void	MakeWindow(void);	// ウィンドウ作成
	void	UpdateSDLTextInput(void);	// SDLテキスト入力状態を更新
	void	DeleteBackward(void);	// 1文字削除
	void	AppendText(LPCSTR pszText);	// テキスト追記
	void	SubmitChat(void);	// チャット確定
	static LRESULT CALLBACK ChatWndProcEntry(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	// チャット入力欄プロシージャ
	LRESULT ChatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	// チャット入力欄プロシージャ


public:
	CmyString	m_strChat;	// 入力された文字列


private:
	int	m_nType;	// チャット種別
	BOOL	m_bPushEnter,	// Enterキー押下状態
			m_bPushEsc,	// ESCキー押下状態
			m_bTextInputActive;	// SDLテキスト入力状態
	HWND	m_hWndChat;	// チャット入力欄

	std::vector<int> m_aArrayType;	// チャット種別

	WNDPROC	m_OrgWndProcChat;	// チャット入力欄の元ウィンドウプロシージャ
} CWindowCHAT, *PCWindowCHAT;
