/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoEffect.h												 */
/* 内容			:エフェクトクラス 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#pragma once
#include <vector>

#include "InfoAnime.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoEffect : public CInfoBase
{
public:
			CInfoEffect();									/* コンストラクタ */
	virtual ~CInfoEffect();									/* デストラクタ */

	int		GetElementCount		(void);								/* 要素数を取得 */
	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	void	Copy				(CInfoEffect *pSrc);				/* コピー */
	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	BOOL		TimerProc		(DWORD dwTime);						/* 時間処理 */
	int			GetAnimeCount	(void);								/* アニメーションコマ数を取得 */
	void		AddAnime		(void);								/* アニメーションコマを追加 */
	void		DeleteAnime		(int nNo);							/* アニメーションコマを削除 */
	void		DeleteAllAnime	(void);								/* アニメーションコマを全て削除 */
	PCInfoAnime	GetAnimePtr		(int nNo);							/* アニメーションコマを取得 */
	WORD		GetGrpID		(void);								/* 現在の画像IDを取得 */
	BYTE		GetLevel		(void);								/* 現在の透明度を取得 */
	BOOL		IsAnimeEnd		(void);								/* アニメーション終了か判定 */


public:
	/* 保存しないデータ */
	BOOL				m_bAnimeEnd;						/* アニメーション終了？ */
	BYTE				m_byAnimeNo;						/* アニメーションコマ番号 */
	DWORD				m_dwLastAnime;						/* 最後にアニメーションした時間 */

	/* 保存するデータ */
	BOOL				m_bLoop,							/* ループ判定 */
						m_bLoopSound;						/* ループ時に効果音を再生する */
	BYTE				m_byAnimeCount;						/* アニメーションコマ数 */
	DWORD				m_dwEffectID,						/* エフェクトID */
						m_dwSoundID,						/* 効果音ID */
						m_dwGrpIDMain;						/* 画像メインID */
	CmyString			m_strName;							/* エフェクト名 */
	PARRAYANIMEINFO		m_paAnimeInfo;						/* アニメーション情報 */
} CInfoEffect, *PCInfoEffect;
typedef std::vector<PCInfoEffect>	  ARRAYEFFECT;
typedef std::vector<PCInfoEffect>	*PARRAYEFFECT;

/* Copyright(C)URARA-works 2007 */
