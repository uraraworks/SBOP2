/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名：	RouteSearch.h												 */
/* 内容：		経路探索クラス 定義ファイル									 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2009/06/13													 */
/* ========================================================================= */

#pragma once
#include <vector>
#include "myArray.h"

class CInfoMapBase;

/* ========================================================================= */
/* 構造体宣言																 */
/* ========================================================================= */

/* 検索結果 */
typedef struct _SEARCHRESULT {
	POINT	pt;					/* 座標 */
	BYTE	byDirection;		/* 向き */
} SEARCHRESULT, *PSEARCHRESULT;
using ARRAYSEARCHRESULT = CStdArray<SEARCHRESULT>;
using PARRAYSEARCHRESULT = ARRAYSEARCHRESULT *;

/* 検索情報 */
typedef struct _SEARCHINFO {
	POINT	pt;					/* 座標 */
	BYTE	byDirection;		/* 向き */
	BYTE	byState;			/* 状態 */
	WORD	wScore;				/* スコア */
} SEARCHINFO, *PSEARCHINFO;
using ARRAYSEARCHINFO = CStdArray<SEARCHINFO>;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CRouteSearch
{
public:
			CRouteSearch();								/* コンストラクタ */
	virtual ~CRouteSearch();							/* デストラクタ */

	void	SetMapInfo		(CInfoMapBase *pInfoMap);			/* 検索対象マップ情報 */
	void	SetSearchArea	(const RECT &rcSearch);				/* 検索範囲を設定 */
	void	SetStartPos		(int x, int y);						/* 開始位置を設定 */
	void	SetEndPos		(int x, int y);						/* 最終位置を設定 */
	PARRAYSEARCHRESULT	Search	(void);								/* 検索 */


private:
	void	InitProcInfo	(void);								/* 検索処理情報を初期化 */
	void	ProcSEARCH		(void);								/* 検索処理(検索中) */
	BOOL	ProcSEARCHMOVE	(void);								/* 検索処理(検索移動中) */
	void	InfoCleanup		(int x, int y);						/* 検索情報最適化 */


private:
	int		m_nState;					/* 検索状態 */
	POINT	m_ptNow,					/* 現在位置 */
			m_ptStart,					/* 開始位置 */
			m_ptEnd;					/* 最終位置 */
	PWORD	m_pMap;						/* 検索中マップテンポラリ */
	SIZE	m_sizeMap;					/* 検索範囲サイズ */
	RECT	m_rcSearch;					/* 検索範囲 */
	CInfoMapBase		*m_pInfoMap;	/* 検索対象マップ情報 */
	ARRAYSEARCHINFO		m_aInfo;		/* 検索情報 */
	ARRAYSEARCHRESULT	m_aResult;		/* 検索結果 */
};

/* Copyright(C)URARA-works 2009 */
