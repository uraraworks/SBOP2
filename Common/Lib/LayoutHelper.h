/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名：	LayoutHelper.h												 */
/* 内容：		ウィンドウサイズ変更時の配置補助クラス 定義ファイル			 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2005/06/17													 */
/* ========================================================================= */

#pragma once

#pragma	warning(disable:4786)

#include <vector>
#include <map>

/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

/* 補正フラグ */
#define	LH_CTRL_X			0x00000001		// X方向調整あり
#define	LH_CTRL_Y			0x00000002		// Y方向調整あり
#define	LH_CTRL_WIDTH		0x00000004		// 幅調整あり
#define	LH_CTRL_HEIGHT		0x00000008		// 高さ調整あり

#define	LH_GRP_X			0x00000001		// X方向調整あり
#define	LH_GRP_Y			0x00000002		// Y方向調整あり
#define	LH_GRP_WIDTH		0x00000004		// 幅調整あり
#define	LH_GRP_HEIGHT		0x00000008		// 高さ調整あり

// グループ移動フラグ
#define	LH_GRP_NOXMOVE		0x00000001		// 横方向移動なし
#define	LH_GRP_NOYMOVE		0x00000002		// 縦方向移動なし
#define	LH_GRP_NOMOVE		(LH_GRP_NOXMOVE | LH_GRP_NOYMOVE)	// 移動なし
#define	LH_GRP_NOWSIZE		0x00000004		// 幅設定なし
#define	LH_GRP_NOHSIZE		0x00000008		// 高さ設定なし
#define	LH_GRP_NOSIZE		(LH_GRP_NOWSIZE | LH_GRP_NOHSIZE)	// サイズ設定なし


/* ========================================================================= */
/* 構造体の定義																 */
/* ========================================================================= */

struct LH_CTRL_LAYOUT {
	HWND	hWnd;				/* ウィンドウハンドル */
	int		x;					/* 位置(X) */
	int		y;					/* 位置(Y) */
	int		cx;					/* 幅 */
	int		cy;					/* 高さ */
	DWORD	dwFlags;			/* 補正フラグ */
	UINT	uGroup;				// 所属するグループID
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CLayoutHelper
{
public:
			CLayoutHelper();							/* コンストラクタ */
	virtual	~CLayoutHelper();							/* デストラクタ */

	void	Initialize			(HWND hBase);									/* 初期化 */
	BOOL	RegisterControl		(HWND hWnd, DWORD dwFlags, UINT uGroup = -1);	/* コントロールの登録 */
	BOOL	RegisterControl		(UINT uID, DWORD dwFlags, UINT uGroup = -1);	/* コントロールの登録 */
	BOOL	UnregisterControl	(HWND hWnd);									/* コントロールの解除 */
	BOOL	UnregisterControl	(UINT uID);										/* コントロールの解除 */
	void	DoLayout			(int cx, int cy);								/* 更新 */

	BOOL	AddGroup			(UINT uGroup, DWORD dwFlags);					/* グループの追加 */
	BOOL	DeleteGroup			(UINT uGroup);									/* グループの削除 */
	BOOL	SetGroup			(HWND hWnd, UINT uGroup);						/* グループの設定 */
	BOOL	SetGroup			(UINT uID, UINT uGroup);						/* グループの設定 */
	BOOL	RemoveGroup			(HWND hWnd);									/* グループの解除 */
	BOOL	RemoveGroup			(UINT uID);										/* グループの解除 */
	BOOL	GetGroupRect		(UINT uGroup, RECT* pRect);						/* グループの矩形を取得 */
	void	MoveGroup			(UINT uGroup, int x, int y, int cx, int cy, DWORD dwFlags = 0);	/* グループの移動 */
	void	OffsetGroup			(UINT uGroup, int left, int top, int right, int bottom);		/* グループのサイズ変更 */


private:
	HWND	m_hBase;							/* 基底となるウィンドウハンドル */
	SIZE	m_sizeBase;							/* 基底サイズ */
	std::vector<LH_CTRL_LAYOUT>	m_vLayout;		/* 登録されたレイアウト情報 */
	std::map<UINT, DWORD>	m_mapGroup;			/* 登録されたグループ情報 */
};

/* Copyright(C)URARA-works 2005 */
