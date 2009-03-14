/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoMapShadow.h											 */
/* 内容			:マップ影情報ライブラリクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/05													 */
/* ========================================================================= */

#pragma once

#include "InfoMapShadow.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoMapShadow : public CLibInfoBase
{
public:
			CLibInfoMapShadow();						/* コンストラクタ */
	virtual ~CLibInfoMapShadow();						/* デストラクタ */

	void Create			(void);									/* 作成 */
	void Destroy		(void);									/* 破棄 */
	BOOL Proc			(void);									/* 処理 */

	PCInfoBase GetNew	(void);									/* 新規データを取得 */

	int		GetCount	(void);									/* データ数を取得 */
	void	Add			(PCInfoBase pInfo);						/* 追加 */
	void	Delete		(int nNo);								/* 削除 */
	void	Delete		(DWORD dwShadowID);						/* 削除 */
	void	DeleteAll	(void);									/* 全て削除 */
	void	Merge		(CLibInfoMapShadow *pSrc);				/* 取り込み */
	void	ResetAnime	(void);									/* アニメーション状態を初期化 */

	PCInfoBase	GetPtr (int nNo);								/* 情報を取得 */
	PCInfoBase	GetPtr (DWORD dwShadowID);						/* 情報を取得 */
	PCInfoBase	GetPtr (POINT *pPos);							/* 情報を取得 */

	DWORD	GetSendDataSize		(void);							/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);							/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* 送信データから取り込み */


protected:
	DWORD	GetNewID	(void);									/* 新しいIDを取得 */
	void	RenewIDPtr	(void);									/* ID検索用マップを更新 */


protected:
	PARRAYMAPSHADOW	m_paInfo;				/* マップ影情報 */
} CLibInfoMapShadow, *PCLibInfoMapShadow;

/* Copyright(C)URARA-works 2007 */
