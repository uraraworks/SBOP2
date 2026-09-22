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
	void	SetEmbedded(int nScreenX, int nScreenY, int nWidth);	// 埋め込みモードに設定(親ウィンドウ内にインライン表示)
	virtual BOOL	HandleSDLKeyDown(UINT vk);	// SDLキー押下を処理
	virtual void	HandleSDLTextInput(LPCSTR pszText);	// SDLテキスト入力を処理
	virtual void	HandleSDLTextEditing(LPCSTR pszText);	// SDLテキスト編集中を処理
	virtual BOOL	HandleSDLMouseLeftButtonDown(int x, int y);	// SDL左クリックを処理
	void	SetCompositionTextFromBrowser(LPCSTR pszText);	// browser IME変換中テキストを設定
	void	CommitTextFromBrowser(LPCSTR pszText);	// browser IME確定テキストを入力
	void	SetNameFromBrowser(LPCSTR pszText);	// browser DOM入力欄の値でキャラ名を置換
	void	SubmitFromBrowser(void);	// browser DOM入力欄からの確定
	LPCTSTR	GetName(void) { return (LPCTSTR)m_strName; }	// 入力されたキャラ名を取得(親が埋め込み描画に使う)
	LPCTSTR	GetComposition(void) { return (LPCTSTR)m_strComposition; }	// IME変換中の未確定文字列を取得(親が埋め込み描画に使う)
			// 埋め込み時はCMgrWindowの最前面判定(m_bActive)を汚さず親を覆い隠さないため、
			// フォーカス状態はm_bFocused(埋め込み時)/m_bActive(通常時)のどちらかで判定する
			BOOL	IsFocused(void) { return m_bEmbedded ? m_bFocused : m_bActive; }


private:
	void	MakeWindow(void);	// ウィンドウ作成
	void	UpdateSDLTextInput(void);	// SDLテキスト入力状態を更新
	void	DeleteBackward(void);	// 1文字削除
	void	AppendText(LPCSTR pszText);	// テキスト追記
	void	SetCompositionText(LPCSTR pszText);	// IME変換中テキストを設定
	void	SubmitCharName(void);	// キャラ名確定
#if defined(__EMSCRIPTEN__)
	void	UpdateBrowserDom(void);	// DOM入力欄オーバーレイを更新
	void	HideBrowserDom(void);	// DOM入力欄オーバーレイを隠す
#endif


public:
	CmyString	m_strName;	// 入力されたキャラ名
	CmyString	m_strComposition;	// IME変換中の未確定文字列


private:
	DWORD	m_dwSuppressSubmitUntil;	// browser IME確定直後のEnter送信抑制期限
	BOOL	m_bTextInputActive;	// SDLテキスト入力状態
	BOOL	m_bEmbedded;	// 埋め込みモード(親ウィンドウ内にインライン表示)か
	BOOL	m_bFocused;	// 埋め込みモード時のフォーカス状態(m_bActiveの代わり)
} CWindowCHARNAME, *PCWindowCHARNAME;
