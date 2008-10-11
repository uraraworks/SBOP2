/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:InfoMapParts.h												 */
/* 内容			:マップパーツクラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/04/25													 */
/* ========================================================================= */

#pragma once

#include "InfoAnime.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

#define BIT_PARTSHIT_BLOCK		(0x00000001)		/* ぶつかる */
#define BIT_PARTSHIT_PILE		(0x00000002)		/* 重なる */
#define BIT_PARTSHIT_PILEBACK	(0x00000004)		/* 下地も重ねる */
#define BIT_PARTSHIT_FISHING	(0x00000008)		/* 釣り場 */

#define BIT_PARTSBLOCK_UP		(0x01)				/* 上にぶつかる */
#define BIT_PARTSBLOCK_DOWN		(0x02)				/* 下にぶつかる */
#define BIT_PARTSBLOCK_LEFT		(0x04)				/* 左にぶつかる */
#define BIT_PARTSBLOCK_RIGHT	(0x08)				/* 右にぶつかる */

#define BIT_PARTSMOVE_UP		(0x01)				/* 上に移動する */
#define BIT_PARTSMOVE_DOWN		(0x02)				/* 下に移動する */
#define BIT_PARTSMOVE_LEFT		(0x04)				/* 左に移動する */
#define BIT_PARTSMOVE_RIGHT		(0x08)				/* 右に移動する */


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMapParts : public CInfoBase
{
public:
			CInfoMapParts();								/* コンストラクタ */
	virtual ~CInfoMapParts();								/* デストラクタ */

	int		GetElementCount		(void);								/* 要素数を取得 */
	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	void	Copy				(CInfoMapParts *pSrc);				/* コピー */
	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	BOOL		IsBlock				(int nDirection);				/* ぶつかり判定 */
	BOOL		TimerProc			(DWORD dwTime);					/* 時間処理 */
	int			GetMoveDirection	(void);							/* 移動する方向を取得 */
	int			GetAnimeCount		(void);							/* アニメーションコマ数を取得 */
	void		AddAnime			(void);							/* アニメーションコマを追加 */
	void		DeleteAnime			(int nNo);						/* アニメーションコマを削除 */
	void		DeleteAllAnime		(void);							/* アニメーションコマを全て削除 */
	PCInfoAnime	GetAnimePtr			(int nNo);						/* アニメーションコマを取得 */


public:
	/* 保存しないデータ */
	BYTE				m_byAnimeNo;						/* アニメーションコマ番号 */
	DWORD				m_dwLastAnime;						/* 最後にアニメーションした時間 */

	/* 保存するデータ */
	BYTE				m_byViewType,						/* 表示種別 */
						m_byAnimeType,						/* アニメーション種別 */
						m_byAnimeCount,						/* アニメーションコマ数 */
						m_byLevel,							/* 透明度 */
						m_byBlockDirection,					/* 方向によるぶつかり判定 */
						m_byMoveDirection;					/* 乗ると移動する方向 */
	WORD				m_wGrpIDBase,						/* グラフィックID(下地) */
						m_wGrpIDPile;						/* グラフィックID(重ね合わせ用ID) */
	DWORD				m_dwPartsID,						/* パーツID */
						m_dwPartsType;						/* パーツの属性 */
	POINT				m_ptViewPos;						/* 編集画面での表示位置 */
	PARRAYANIMEINFO		m_paAnimeInfo;						/* アニメーション情報 */
} CInfoMapParts, *PCInfoMapParts;
typedef CmyArray<PCInfoMapParts, PCInfoMapParts>	  ARRAYMAPPARTS;
typedef CmyArray<PCInfoMapParts, PCInfoMapParts>	*PARRAYMAPPARTS;

/* Copyright(C)URARA-works 2005 */
