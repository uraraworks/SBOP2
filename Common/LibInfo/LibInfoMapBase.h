/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LibInfoMapBase.h											 */
/* 内容			:マップ情報ライブラリ基底クラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/15													 */
/* ========================================================================= */

#pragma once

#include "InfoMapBase.h"
#include "LibInfoBase.h"

class CLibInfoMapParts;
class CLibInfoMapObject;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoMapBase : public CLibInfoBase
{
public:
			CLibInfoMapBase();							/* コンストラクタ */
	virtual ~CLibInfoMapBase();							/* デストラクタ */

	void Create			(CLibInfoMapParts *pLibInfoMapParts);	/* 作成 */
	void Destroy		(void);									/* 破棄 */

	PCInfoBase GetNew	(void);									/* 新規データを取得 */

	void	RenewMapEvent	(void);								/* マップイベント更新 */
	void	RenewHitTmp		(void);								/* マップパーツ以外での当たり判定を更新 */
	void	SetMapObject	(CLibInfoMapObject *pLibInfo);		/* マップオブジェクト情報を設定 */
	int		GetCount		(void);								/* データ数を取得 */
	void	Add				(PCInfoBase pInfo);					/* 追加 */
	void	Delete			(int nNo);							/* 削除 */
	void	Delete			(DWORD dwMapID);					/* 削除 */
	void	DeleteAll		(void);								/* 全て削除 */
	void	DeleteParts		(DWORD dwPartsID);					/* 指定パーツを削除 */
	void	DeleteShadow	(DWORD dwShadowID);					/* 指定マップ影を削除 */

	PCInfoBase	GetPtr (int nNo);								/* マップ情報を取得 */
	PCInfoBase	GetPtr (DWORD dwMapID);							/* マップ情報を取得 */


protected:
	DWORD	GetNewID	(void);									/* 新しいマップIDを取得 */


protected:
	PARRAYINFOMAPBASE	m_paInfo;				/* マップ情報 */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* マップパーツ情報 */
} CLibInfoMapBase, *PCLibInfoMapBase;

/* Copyright(C)URARA-works 2006 */
