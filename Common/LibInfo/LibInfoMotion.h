/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoMotion.h											 */
/* 内容			:モーション情報ライブラリ基底クラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/29													 */
/* ========================================================================= */

#pragma once

#include "LibInfoBase.h"
#include "InfoMotion.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoMotion : public CLibInfoBase
{
public:
			CLibInfoMotion();								/* コンストラクタ */
	virtual ~CLibInfoMotion();								/* デストラクタ */

	void Create			(void);										/* 作成 */
	void Destroy		(void);										/* 破棄 */

	virtual PCInfoBase GetNew	(void);								/* 新規データを取得 */
	virtual PCInfoBase GetPtr	(int nNo);							/* 情報を取得 */

	void	Revice		(void);											/* データの補正 */
	int		GetCount	(void);											/* データ数を取得 */
	void	Add			(PCInfoBase pInfo);								/* 追加 */
	void	Add			(PCInfoBase pInfo, int nNo);					/* 追加 */
	void	AddType		(DWORD dwMotionTypeID);							/* 指定種別で一覧追加 */
	void	Delete		(int nNo);										/* 削除 */
	void	Delete		(DWORD dwMotionID);								/* 削除 */
	void	DeleteAll	(void);											/* 全て削除 */
	void	SetList		(DWORD dwMotionTypeID, DWORD dwMotionListID, CLibInfoMotion *pSrc);	/* 指定IDの情報を設定 */
	BOOL	IsDirection	(DWORD dwMotionListID);							/* 向き毎に設定があるモーションか判定 */
	DWORD	GetWaitTime	(DWORD dwMotionTypeID, DWORD dwMotionListID);	/* 全コマの表示時間を取得 */

	void	GetMotionInfo		(DWORD dwMotionTypeID, DWORD dwMotionListID, ARRAYMOTIONINFO &aDst);	/* 指定IDのモーション情報一覧を取得 */
	DWORD	GetSendDataSize		(DWORD dwMotionTypeID, DWORD dwMotionListID);							/* 送信データサイズを取得 */
	PBYTE	GetSendData			(DWORD dwMotionTypeID, DWORD dwMotionListID);							/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);									/* 送信データから取り込み */

	DWORD	GetSendDataSizeType	(DWORD dwMotionTypeID);							/* 指定種別IDの送信データサイズを取得 */
	PBYTE	GetSendDataType		(DWORD dwMotionTypeID);							/* 指定種別IDの送信データを取得 */
	PBYTE	SetSendDataType		(PBYTE pSrc);									/* 送信データから取り込み */


protected:
	DWORD	GetNewID	(void);										/* 新しいアカウントIDを取得 */


protected:
	DWORD	m_dwNewIDTmp;						/* 新規ID作成用 */
	PARRAYMOTIONINFO	m_paInfo;				/* モーション情報 */
} CLibInfoMotion, *PCLibInfoMotion;

/* Copyright(C)URARA-works 2007 */
