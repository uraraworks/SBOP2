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
	void	DragOff		(void);									/* ドラッグ解除 */

	DWORD	GetSelectItemID	(void)	{ return m_dwSelectItemID;	}	/* 選択されているアイテムIDを取得 */
	DWORD	GetDragItemID	(void)	{ return m_dwDragItemID;	}	/* ドラッグされたアイテムIDを取得 */
	POINT	GetDragPos		(void)	{ return m_ptDrop;			}	/* ドロップ位置を取得 */
	int		GetPos			(void)	{ return m_nPos;			}	/* カーソル位置を取得 */


protected:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnB			(BOOL bDown);						/* キーハンドラ(B) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Z) */
	void	DrawEquip	(int nType, DWORD dwEquipItemID);	/* 装備アイテムを描画 */
	void	GetDrawPos	(int nPos, int &nDstX, int &nDstY);	/* アイテム描画位置を取得 */


protected:
	POINT			m_ptDrag,				/* ドラッグ開始座標 */
					m_ptDrop;				/* ドラッグ終了座標 */
	DWORD			m_dwSelectItemID,		/* 選択されたアイテムID */
					m_dwDragItemID;			/* ドラッグされたアイテムID */
	CmyString		m_strName;				/* 選択中のアイテム名 */
	CInfoCharCli	*m_pPlayerChar;			/* 操作中のキャラ情報 */
	CLibInfoItem	*m_pLibInfoItem;		/* アイテム情報ライブラリ */
} CWindowITEMMENU, *PCWindowITEMMENU;

/* Copyright(C)URARA-works 2007 */
