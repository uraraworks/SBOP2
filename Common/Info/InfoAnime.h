/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:InfoAnime.h												 */
/* 内容			:アニメーション情報クラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/04/29													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

#define PREFIX_INFOANIME		"pAnime_"					/* 要素名のプレフィックス */


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoAnime : public CInfoBase
{
public:
			CInfoAnime();									/* コンストラクタ */
	virtual ~CInfoAnime();									/* デストラクタ */

	int		GetElementCount		(void);								/* 要素数を取得 */
	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	void	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */


public:
	int			m_nElementCount;							/* 要素数 */

	/* 保存しないデータ */

	/* 保存するデータ */
	BYTE		m_byWait,									/* 待ち時間(×１０ミリ秒) */
				m_byLevel;									/* 透明度 */
	WORD		m_wGrpIDBase,								/* グラフィックID(下地) */
				m_wGrpIDPile;								/* グラフィックID(重ね合わせ用ID) */
} CInfoAnime, *PCInfoAnime;
typedef CmyArray<PCInfoAnime, PCInfoAnime>	   ARRAYANIMEINFO;
typedef CmyArray<PCInfoAnime, PCInfoAnime>	 *PARRAYANIMEINFO;

/* Copyright(C)URARA-works 2005 */
