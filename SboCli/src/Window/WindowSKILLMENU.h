/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowSKILLMENU.h											 */
/* 内容			:スキルメニューウィンドウクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/31													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CInfoCharCli;
class CLibInfoSkill;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowSKILLMENU : public CWindowBase
{
public:
			CWindowSKILLMENU();							/* コンストラクタ */
	virtual ~CWindowSKILLMENU();						/* デストラクタ */

	void	Create		(CMgrData *pMgrData);					/* 作成 */
	void	Draw		(CImg32 *pDst);							/* 描画 */
	void	DragOff		(void);									/* ドラッグ解除 */

	DWORD	GetSelectID	(void)	{ return m_dwSelectID;	}		/* 選択されているスキルIDを取得 */
	int		GetPos		(void)	{ return m_nPos;		}		/* カーソル位置を取得 */
	void	SetType		(int nType) { m_nType = nType; }		/* スキル種別を設定 */


protected:
	BOOL	OnUp		(void);								/* キーハンドラ(↑) */
	BOOL	OnDown		(void);								/* キーハンドラ(↓) */
	BOOL	OnLeft		(void);								/* キーハンドラ(←) */
	BOOL	OnRight		(void);								/* キーハンドラ(→) */
	BOOL	OnS			(BOOL bDown);						/* キーハンドラ(S) */
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Z) */
	void	GetDrawPos	(int nPos, int &nDstX, int &nDstY);	/* 描画位置を取得 */


protected:
	int				m_nType;				/* スキル種別 */
	DWORD			m_dwSelectID;			/* 選択されたスキルID */
	CmyString		m_strName;				/* 選択中のスキル名 */
	CInfoCharCli	*m_pPlayerChar;			/* 操作中のキャラ情報 */
	CLibInfoSkill	*m_pLibInfoSkill;		/* スキル情報ライブラリ */
} CWindowSKILLMENU, *PCWindowSKILLMENU;

/* Copyright(C)URARA-works 2008 */
