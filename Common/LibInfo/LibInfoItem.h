/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoItem.h												 */
/* 内容			:アイテム情報ライブラリ基底クラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "InfoItem.h"
#include "LibInfoBase.h"

class CInfoCharBase;
class CLibInfoItemType;
class CLibInfoItemWeapon;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoItem : public CLibInfoBase
{
public:
			CLibInfoItem();									/* コンストラクタ */
	virtual ~CLibInfoItem();								/* デストラクタ */

	void Create			(void);										/* 作成 */
	void Destroy		(void);										/* 破棄 */
	void SetTypeInfo	(CLibInfoItemType *pLibInfoItemType);		/* アイテム種別情報を設定 */
	void SetWeaponInfo	(CLibInfoItemWeapon *pLibInfoItemWeapon);	/* 武器情報を設定 */
	void RenewSize		(DWORD dwMapID, int nDirection, int nSize);	/* マップサイズ更新 */

	PCInfoBase GetNew	(void);									/* 新規データを取得 */

	int		GetCount	(void);									/* データ数を取得 */
	void	Add			(PCInfoBase pInfo);						/* 追加 */
	void	Delete		(int nNo);								/* 削除 */
	void	Delete		(DWORD dwItemID);						/* 削除 */
	void	DeleteAll	(void);									/* 全て削除 */
	void	Sort		(void);									/* 表示位置に合わせてソート */
	LPCSTR	GetTypeName		(DWORD dwTypeID);					/* アイテム種別名を取得 */
	DWORD	GetItemType		(DWORD dwItemID);					/* アイテム種別を取得 */
	DWORD	GetItemTypeID	(DWORD dwItemID);					/* アイテム種別IDを取得 */

	DWORD	GetMotionIDAtack		(DWORD dwItemID);			/* 使用可能な攻撃モーションIDを取得 */
	DWORD	GetMotionIDBattleStand	(DWORD dwItemID);			/* 戦闘モード中の立ちモーションIDを取得 */
	DWORD	GetMotionIDBattleWalk	(DWORD dwItemID);			/* 戦闘モード中のすり足モーションIDを取得 */

	PCInfoBase	GetPtr (int nNo);								/* アイテム情報を取得 */
	PCInfoBase	GetPtr (DWORD dwItemID);						/* アイテム情報を取得 */
	PCInfoBase	GetPtr (DWORD dwMapID, POINT *pptPos, BOOL bPoint = TRUE);	/* アイテム情報を取得 */
	PCInfoBase	GetItemTypePtr	(DWORD dwItemID);				/* アイテム種別情報を取得 */

	DWORD	GetSendDataSize		(void);							/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);							/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);					/* 送信データから取り込み */

	void	AddItem		(DWORD dwCharID, DWORD dwItemID, ARRAYDWORD *padwItemID);			/* 所持アイテムを追加 */
	void	DeleteItem	(DWORD dwItemID, CInfoCharBase *pInfoChar, BOOL bNoPos = FALSE);	/* 所持アイテムを削除 */
	void	Equip		(CInfoCharBase *pInfoChar, DWORD dwItemIDOld, DWORD dwItemIDNew);	/* アイテム位置を装備用に入れ替え */
	DWORD	MakeItem	(DWORD dwMapID, POINT *pptPos, DWORD dwItemTypeID);					/* 指定場所にアイテムを作成 */
	BOOL	GetFreePos	(POINT &ptDst, ARRAYDWORD *padwItemID);	/* アイテムの空き場所を取得 */
	BOOL	IsUseItem	(DWORD dwItemID);						/* 使用できるアイテムか判定 */

	void		SetArea			(DWORD dwMapID, RECT *prcArea);	/* 範囲指定一覧作成 */
	int			GetAreaCount	(void);							/* 範囲指定一覧数取得 */
	PCInfoBase	GetPtrArea		(int nNo);						/* 範囲指定一覧からアイテム情報を取得 */


protected:
	DWORD	GetNewID	(void);										/* 新しいアイテムIDを取得 */
	BOOL	IsItemPos	(POINT *ptItem, ARRAYDWORD *padwItemID);	/* 指定場所が使用済みかチェック */


protected:
	DWORD	m_dwNewIDTmp;						/* 新規ID作成用 */

	ARRAYDWORD			m_adwAreaID;			/* 範囲指定一覧 */
	PARRAYITEMINFO		m_paInfo;				/* アイテム情報 */
	CLibInfoItemType	*m_pLibInfoItemType;	/* アイテム種別情報 */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	/* 武器情報 */
} CLibInfoItem, *PCLibInfoItem;

/* Copyright(C)URARA-works 2007 */
