/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowCOMMANDMENU.h										 */
/* 内容			:コマンドメニューウィンドウクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/02													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowCOMMANDMENU : public CWindowBase
{
public:
			CWindowCOMMANDMENU();						/* コンストラクタ */
	virtual ~CWindowCOMMANDMENU();						/* デストラクタ */

	void	Create		(CMgrData *pMgrData);					/* 作成 */
	void	Draw		(CImg32 *pDst);							/* 描画 */
	int		GetPosSub	(void)	{ return m_nPosSub; }			/* サブメニューの位置を取得 */


protected:
	BOOL	OnUp			(void);							/* キーハンドラ(↑) */
	BOOL	OnDown			(void);							/* キーハンドラ(↓) */
	BOOL	OnLeft			(void);							/* キーハンドラ(←) */
	BOOL	OnRight			(void);							/* キーハンドラ(→) */
	BOOL	OnB				(BOOL bDown);					/* キーハンドラ(B) */
	BOOL	OnX				(BOOL bDown);					/* キーハンドラ(X) */
	BOOL	OnZ				(BOOL bDown);					/* キーハンドラ(Z) */
	BOOL	OnSpace			(BOOL bDown);					/* キーハンドラ(Space) */
	void	RenewCommand	(void);							/* 選択中のコマンドを更新 */


protected:
	int			m_nPosSub;					/* サブメニューの位置 */
	CmyString	m_strCommand;				/* 選択中のコマンド */
} CWindowCOMMANDMENU, *PCWindowCOMMANDMENU;

/* Copyright(C)URARA-works 2008 */
