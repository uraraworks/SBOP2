/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoItemWeapon.h										 */
/* 内容			:アイテム武器情報ライブラリクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/10													 */
/* ========================================================================= */

#pragma once

#include "InfoItemWeapon.h"
#include "LibInfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoItemWeapon : public CLibInfoBase
{
public:
			CLibInfoItemWeapon();						/* コンストラクタ */
	virtual ~CLibInfoItemWeapon();						/* デストラクタ */

	void Create			(void);									/* 作成 */
	void Destroy		(void);									/* 破棄 */

	PCInfoBase GetNew	(void);									/* 新規データを取得 */
	int		GetCount	(void);									/* データ数を取得 */
	void	Add			(PCInfoBase pInfo);						/* 追加 */
	void	Delete		(int nNo);								/* 削除 */
	void	Delete		(DWORD dwWeaponInfoID);					/* 削除 */
	void	DeleteAll	(void);									/* 全て削除 */

	PCInfoBase	GetPtr (int nNo);								/* 武器情報を取得 */
	PCInfoBase	GetPtr (DWORD dwWeaponInfoID);					/* 武器情報を取得 */

	DWORD	GetSendDataSize		(void);							/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);							/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* 送信データから取り込み */
	PBYTE	GetNewSendData		(PBYTE pSrc, PCInfoItemWeapon &pDst);	/* 送信データから新規データを取得 */


protected:
	DWORD	GetNewID	(void);									/* 新しいアイテムIDを取得 */


protected:
	DWORD	m_dwNewIDTmp;						/* 新規ID作成用 */
	PARRAYWEAPONINFO	m_paInfo;				/* 武器情報 */
} CLibInfoItemWeapon, *PCLibInfoItemWeapon;

/* Copyright(C)URARA-works 2008 */
