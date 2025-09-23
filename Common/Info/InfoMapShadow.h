/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoMapShadow.h											 */
/* 内容			:マップ影クラス 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/04													 */
/* ========================================================================= */

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoAnime.h"


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMapShadow : public CInfoBase
{
public:
			CInfoMapShadow();								/* コンストラクタ */
	virtual ~CInfoMapShadow();								/* デストラクタ */

	int		GetElementCount		(void);								/* 要素数を取得 */
	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	void	Copy				(CInfoMapShadow *pSrc);				/* コピー */
	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	BOOL		TimerProc			(DWORD dwTime);					/* 時間処理 */
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
						m_byLevel;							/* 透明度 */
	BOOL				m_bLight;							/* 透明度を明度として使う */
	WORD				m_wGrpID;							/* グラフィックID */
	DWORD				m_dwShadowID;						/* 影ID */
	POINT				m_ptViewPos;						/* 編集画面での表示位置 */
	PARRAYANIMEINFO		m_paAnimeInfo;						/* アニメーション情報 */
} CInfoMapShadow, *PCInfoMapShadow;
using ARRAYMAPSHADOW = CStdArray<PCInfoMapShadow>;
using PARRAYMAPSHADOW = ARRAYMAPSHADOW *;

/* Copyright(C)URARA-works 2007 */
