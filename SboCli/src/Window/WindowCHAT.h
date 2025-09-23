/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名：	WindowCHAT.h												 */
/* 内容：		チャットウィンドウクラス 定義ファイル						 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2007/02/03													 */
/* ========================================================================= */

#pragma once
#include <vector>

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowCHAT : public CWindowBase
{
public:
			CWindowCHAT();									/* コンストラクタ */
	virtual ~CWindowCHAT();									/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */
	void	SetActive	(BOOL bActive);								/* アクティブか設定 */
	int		GetType		(void);										/* チャット種別を取得 */


private:
	void	MakeWindow	(void);										/* ウィンドウ作成 */
	static LRESULT CALLBACK ChatWndProcEntry(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* チャット入力欄プロシージャ */
	LRESULT ChatWndProc	(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* チャット入力欄プロシージャ */


public:
	CmyString	m_strChat;					/* 入力された文字列 */


private:
	int		m_nType;						/* チャット種別 */
	BOOL	m_bPushEnter,					/* Enterキー押下状態 */
			m_bPushEsc;						/* ESCキー押下状態 */
	HWND	m_hWndChat;						/* チャット入力欄 */

	std::vector<int> m_aArrayType;		/* チャット種別 */

	WNDPROC	m_OrgWndProcChat;				/* チャット入力欄の元ウィンドウプロシージャ */
} CWindowCHAT, *PCWindowCHAT;

/* Copyright(C)URARA-works 2007 */
