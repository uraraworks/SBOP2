/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoMotionType.h										 */
/* 内容			:モーション種別情報ライブラリ基底クラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/26													 */
/* ========================================================================= */

#pragma once

#include "LibInfoBase.h"
#include "InfoMotionType.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoMotionType : public CLibInfoBase
{
public:
			CLibInfoMotionType();									/* コンストラクタ */
	virtual ~CLibInfoMotionType();									/* デストラクタ */

	void Create			(void);												/* 作成 */
	void Destroy		(void);												/* 破棄 */

	virtual PCInfoBase GetNew	(void);										/* 新規データを取得 */
	virtual PCInfoBase GetPtr	(int nNo);									/* 情報を取得 */
	virtual PCInfoBase GetPtr	(DWORD dwMotionTypeID);						/* 情報を取得 */

	int		GetCount	(void);												/* データ数を取得 */
	void	Add			(PCInfoBase pInfo);									/* 追加 */
	void	Delete		(int nNo);											/* 削除 */
	void	Delete		(DWORD dwMotionTypeID);								/* 削除 */
	void	DeleteAll	(void);												/* 全て削除 */

	DWORD	GetSendDataSize		(DWORD dwMotionTypeID);						/* 送信データサイズを取得 */
	PBYTE	GetSendData			(DWORD dwMotionTypeID);						/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);								/* 送信データから取り込み */


protected:
	DWORD	GetNewID	(void);										/* 新しいアカウントIDを取得 */


protected:
	DWORD	m_dwNewIDTmp;						/* 新規ID作成用 */
	PARRAYMOTIONTYPEINFO	m_paInfo;			/* モーション種別情報 */
} CLibInfoMotionType, *PCLibInfoMotionType;

/* Copyright(C)URARA-works 2008 */
