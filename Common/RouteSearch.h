/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名：	RouteSearch.h												 */
/* 内容：		経路探索クラス 定義ファイル									 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2009/06/13													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* 構造体宣言																 */
/* ========================================================================= */

/* 検索情報 */
typedef struct _SEARCHINFO {
	POINT	pt;					/* 座標 */
	BYTE	byDirection;		/* 向き */
	BYTE	byState;			/* 状態 */
	WORD	wScore;				/* スコア */
} SEARCHINFO, *PSEARCHINFO;
typedef CmyArray<SEARCHINFO, SEARCHINFO>		ARRAYSEARCHINFO;

/* 検索処理情報 */
typedef struct _SEARCHPROCINFO {
	int		nState,				/* 検索状態 */
			nDirection,			/* 向き */
			nTurn;				/* ターン */
	POINT	ptNow,				/* 現在位置 */
			ptStart,			/* 開始位置 */
			ptEnd;				/* 最終位置 */
	SIZE	sizeMap;			/* 検索用マップサイズ */
	PBYTE	pSearchMap;			/* 検索中マップ */
	PWORD	pMap;				/* 検索範囲マップ */
	ARRAYSEARCHINFO	aInfo;		/* 検索情報 */
} SEARCHPROCINFO, *PSEARCHPROCINFO;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CRouteSearch
{
public:
			CRouteSearch();						/* コンストラクタ */
	virtual ~CRouteSearch();					/* デストラクタ */

	void	SetStartPos		(int x, int y);								/* 開始位置を設定 */
	void	SetEndPos		(int x, int y);								/* 最終位置を設定 */
	void	SetSize			(int cx, int cy);							/* 範囲サイズを設定 */
	void	SetBlock		(int x, int y, BOOL bSet);					/* 障害物を設定 */
	void	Search			(void);										/* 検索 */
	BOOL	GetPos			(int &nDstDirection, POINT &ptDstPos);		/* 次の座標を取得 */


private:
	void	InitProcInfo	(void);								/* 検索処理情報を初期化 */
	void	ChgState		(int nState);						/* 状態遷移 */
	void	Proc			(void);								/* 検索処理 */
	BOOL	ProcSEARCH		(void);								/* 検索処理(検索中) */
	BOOL	ProcSEARCHMOVE	(void);								/* 検索処理(検索移動中) */
	void	InfoCleanup		(int x, int y);						/* 検索情報最適化 */
//	void	ProcMOVE		(void);								/* 検索処理(移動中) */


private:
	SEARCHPROCINFO	m_ProcInfo;			/* 検索処理情報 */
};

/* Copyright(C)URARA-works 2009 */
