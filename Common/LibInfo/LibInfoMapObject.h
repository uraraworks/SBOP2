/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoMapObject.h											 */
/* 内容			:マップオブジェクト情報ライブラリクラス 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/01													 */
/* ========================================================================= */

#pragma once

#include "InfoMapObject.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoMapObject : public CLibInfoBase
{
public:
			CLibInfoMapObject();						/* コンストラクタ */
	virtual ~CLibInfoMapObject();						/* デストラクタ */

	void Create			(void);									/* 作成 */
	void Destroy		(void);									/* 破棄 */

	PCInfoBase GetNew	(void);									/* 新規データを取得 */

	int		GetCount	(void);									/* データ数を取得 */
	void	Add			(PCInfoBase pInfo);						/* 追加 */
	void	Delete		(int nNo);								/* 削除 */
	void	Delete		(DWORD dwObjectID);						/* 削除 */
	void	DeleteAll	(void);									/* 全て削除 */
	void	Merge		(CLibInfoMapObject *pSrc);				/* 取り込み */

	PCInfoBase	GetPtr (int nNo);								/* 情報を取得 */
	PCInfoBase	GetPtr (DWORD dwObjectID);						/* 情報を取得 */

	DWORD	GetSendDataSize		(void);							/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);							/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* 送信データから取り込み */


protected:
	DWORD	GetNewID	(void);									/* 新しいIDを取得 */


protected:
	PARRAYMAPOBJECT	m_paInfo;				/* マップオブジェクト情報 */
} CLibInfoMapObject, *PCLibInfoMapObject;

/* Copyright(C)URARA-works 2008 */
