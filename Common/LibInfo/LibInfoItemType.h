/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoItemType.h											 */
/* 内容			:アイテム種別情報ライブラリ基底クラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/27													 */
/* ========================================================================= */

#pragma once

#include "InfoItemTypeBase.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoItemType : public CLibInfoBase
{
public:
			CLibInfoItemType();							/* コンストラクタ */
	virtual ~CLibInfoItemType();						/* デストラクタ */

	void Create			(void);									/* 作成 */
	void Destroy		(void);									/* 破棄 */

			PCInfoBase GetNew	(void);							/* 新規データを取得 */
	virtual	PCInfoBase GetNew	(DWORD dwItemTypeID);			/* 新規データを取得 */

	void	Revice		(void);									/* データの補正 */
	int		GetCount	(void);									/* データ数を取得 */
	void	Add			(PCInfoBase pInfo);						/* 追加 */
	void	Delete		(int nNo);								/* 削除 */
	void	Delete		(DWORD dwTypeID);						/* 削除 */
	void	DeleteAll	(void);									/* 全て削除 */

	int		GetGrpIDMainCount	(DWORD dwTypeID);						/* アイテム種別の画像IDメイン数を取得 */
	LPCSTR	GetTypeName			(DWORD dwItemTypeID);					/* アイテム種別名を取得 */
	LPCSTR	GetTypeNameTypeID	(DWORD dwTypeID);						/* アイテム種別名を取得 */
	LPCSTR	GetGrpIDMainName	(DWORD dwTypeID, WORD wGrpIDMain);		/* 画像IDメイン種別名を取得 */
	DWORD	GetGrpIDMain		(DWORD dwTypeID, WORD wGrpIDMain);		/* 画像IDメインを取得 */

	PCInfoBase	GetPtr (int nNo);										/* アイテム情報を取得 */
	PCInfoBase	GetPtr (DWORD dwTypeID);								/* アイテム情報を取得 */

	DWORD	GetSendDataSize		(void);									/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);									/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);							/* 送信データから取り込み */
	PBYTE	GetNewSendData		(PBYTE pSrc, PCInfoItemTypeBase &pDst);	/* 送信データから新規データを取得 */


protected:
	DWORD	GetNewID	(void);									/* 新しいアイテムIDを取得 */


protected:
	DWORD	m_dwNewIDTmp;						/* 新規ID作成用 */
	PARRAYITEMTYPEINFO	m_paInfo;				/* アイテム種別情報 */
} CLibInfoItemType, *PCLibInfoItemType;

/* Copyright(C)URARA-works 2007 */
