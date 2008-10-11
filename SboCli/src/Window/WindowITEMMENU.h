/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WindowITEMMENU.h											 */
/* 内容			:アイテムメニューウィンドウクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/30													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CInfoCharCli;
class CLibInfoItem;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowITEMMENU : public CWindowBase
{
public:
			CWindowITEMMENU();							/* コンストラクタ */
	virtual ~CWindowITEMMENU();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);					/* 作成 */
	void	Draw		(CImg32 *pDst);							/* 描画 */

	DWORD	GetSelectItemID	(void)	{ return m_dwSelectItemID;	}	/* 選択されてアイテムIDを取得 */
	int		GetPos			(void)	{ return m_nPos;			}	/* カーソル位置を取得 */


protected:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnB			(BOOL bDown);						/* キーハンドラ(B) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Z) */


protected:
	DWORD			m_dwSelectItemID;		/* 選択されたアイテムID */
	CmyString		m_strName;				/* 選択中のアイテム名 */
	CInfoCharCli	*m_pPlayerChar;			/* 操作中のキャラ情報 */
	CLibInfoItem	*m_pLibInfoItem;		/* アイテム情報ライブラリ */
} CWindowITEMMENU, *PCWindowITEMMENU;

/* Copyright(C)URARA-works 2007 */
