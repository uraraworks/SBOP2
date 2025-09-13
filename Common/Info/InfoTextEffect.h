/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoTextEffect.h											 */
/* 内容			:文字エフェクトクラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/13													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* 表示種別 */
enum {
	TEXTEFFECTTYPE_NORMAL = 0,				/* 通常表示 */
	TEXTEFFECTTYPE_WAVE,					/* 1文字ずつウェーブ表示 */
};
/* 文字サイズ(数値用) */
enum {
	TEXTEFFECTNUMSIZE_SMALL = 0,			/* 小 */
	TEXTEFFECTNUMSIZE_MEDIUM,				/* 並 */
	TEXTEFFECTNUMSIZE_LARGE,				/* 大 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoTextEffect : public CInfoBase
{
public:
			CInfoTextEffect();										/* コンストラクタ */
	virtual ~CInfoTextEffect();										/* デストラクタ */

	BOOL	TimerProc	(DWORD dwTime);										/* 時間処理 */
	void	Set			(DWORD dwData, int nType, int nColor, int nSize);	/* 設定(数値データ) */
	void	Set			(LPCSTR pszData, int nType, int nColor, int nSize);	/* 設定(文字データ) */

	int		GetGrpCount	(void);										/* 画像数を取得 */
	int		GetGrpNo	(int nNo);									/* 指定番号の数値を取得 */
	BOOL	GetDrawPos	(int nNo, POINT &ptDst, BYTE &byLevel);		/* 指定番号の表示位置補正値を取得 */
	DWORD	GetData		(void)	{ return m_dwData; }				/* 数値データを取得 */


public:
	int				m_nType,				/* 表示種別 */
					m_nColor,				/* 色 */
					m_nSize;				/* サイズ */
	BOOL			m_bAnimeEnd;			/* アニメーション終了？ */
	BYTE			m_byAnimeNo;			/* アニメーションコマ番号 */
	BYTE			m_byAnimeCount;			/* アニメーションコマ数 */
	DWORD			m_dwLastAnime,			/* 最後にアニメーションした時間 */
					m_dwData;				/* データ */
	CmyString		m_strData;				/* データ */
} CInfoTextEffect, *PCInfoTextEffect;
typedef CmyArray<PCInfoTextEffect, PCInfoTextEffect>	  ARRAYTEXTEFFECT;
typedef CmyArray<PCInfoTextEffect, PCInfoTextEffect>	*PARRAYTEXTEFFECT;

/* Copyright(C)URARA-works 2008 */
