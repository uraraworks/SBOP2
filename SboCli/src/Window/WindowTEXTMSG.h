/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名：	WindowTEXTMSG.h												 */
/* 内容：		テキストメッセージ表示ウィンドウクラス 定義ファイル			 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2008/11/22													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CInfoTalkEvent;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowTEXTMSG : public CWindowBase
{
public:
			CWindowTEXTMSG();						/* コンストラクタ */
	virtual ~CWindowTEXTMSG();						/* デストラクタ */

	void	Create		(CMgrData *pMgrData);				/* 作成 */

	void	Draw		(CImg32 *pDst);						/* 描画 */
	BOOL	TimerProc	(void);								/* 時間処理 */
	void	SetTitle	(LPCSTR pszTitle);					/* 肩書きを設定 */
	void	SetName		(LPCSTR pszName);					/* 名前を設定 */
	void	SetMsg		(LPCSTR pszMsg);					/* メッセージ設定 */
	void	SetTalkEvent(CInfoTalkEvent *pInfo);			/* 会話イベント設定 */


protected:
	BOOL OnUp			(void);								/* キーハンドラ(↑) */
	BOOL OnDown			(void);								/* キーハンドラ(↓) */
	BOOL OnLeft			(void);								/* キーハンドラ(←) */
	BOOL OnRight		(void);								/* キーハンドラ(→) */
	BOOL OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL OnZ			(BOOL bDown);						/* キーハンドラ(Z) */
	void DrawChar		(LPCSTR pszText);					/* １文字表示 */
	void RenewTitle		(void);								/* 肩書と名前画像を更新 */
	void InitText		(void);								/* メッセージ画像を初期化 */
	void MsgProc		(void);								/* メッセージ解析処理 */
	void GetLineText	(LPCSTR pszSrc, CmyString &strDst);	/* 1行取得 */
	void TrimSpace		(CmyString &strSrc);				/* 前後の空白を除去 */
	void GetBlock		(LPCSTR pszSrc, CmyString &strDst);	/* 大かっこ1ブロック分取得 */


protected:
	BOOL		m_bInputWait;				/* 入力待ち */
	POINT		m_ptDraw;					/* 描画位置 */
	int			m_nState,					/* 表示状態 */
				m_nType,					/* ウィンドウ種別 */
				m_nProcPos,					/* 処理中の文字位置 */
				m_nProcPosTmp,				/* 処理中の文字位置 */
				m_nProcEventPage,			/* 処理中イベントページ */
				m_nProcEventNo,				/* 処理中イベント番号 */
				m_nSpaceHeight;				/* ウィンドウ上の余白の高さ */
	DWORD		m_dwLastProc;				/* 前回の処理時間 */
	CmyString	m_strTitle,					/* 肩書き */
				m_strName,					/* 名前 */
				m_strMsg,					/* メッセージ */
				m_strMsgTmp;				/* メッセージ */
	CmyStringArray	m_astrMenu;				/* メニュー項目 */
	CImg32		*m_pDibTitle,				/* 肩書きと名前部分 */
				*m_pDibText;				/* 文字描画テンポラリ */
	CInfoTalkEvent	*m_pInfoTalkEvent;		/* 処理中の会話イベント */
} CWindowTEXTMSG, *PCWindowTEXTMSG;

/* Copyright(C)URARA-works 2008 */
