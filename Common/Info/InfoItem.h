/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoItem.h													 */
/* 内容			:アイテム情報クラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/05													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoItem : public CInfoBase
{
public:
			CInfoItem();									/* コンストラクタ */
	virtual ~CInfoItem();									/* デストラクタ */

	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	void	Copy				(CInfoItem *pSrc);					/* コピー */


public:
	/* 保存しないデータ */

	/* 保存するデータ */
	BOOL		m_bPutOn;									/* 上に置けるか判定 */
	DWORD		m_dwItemID,									/* アイテムID */
				m_dwItemTypeID,								/* アイテム種別ID */
				m_dwGrpID,									/* 地面にある時の画像ID */
				m_dwIconGrpID,								/* バックパック内の画像ID */
				m_dwMapID,									/* 落ちているマップID */
				m_dwCharID,									/* 所有者キャラID */
				m_dwDropSoundID;							/* 落ちたときの効果音ID */
	int			m_nPosZ;									/* 落ちている高さ(0が地面) */
	POINT		m_ptPos,									/* 落ちている座標 */
				m_ptBackPack;								/* バックパック内の位置 */
	CmyString	m_strName;									/* アイテム名 */
} CInfoItem, *PCInfoItem;
typedef CmyArray<PCInfoItem, PCInfoItem>	   ARRAYITEMINFO;
typedef CmyArray<PCInfoItem, PCInfoItem>	 *PARRAYITEMINFO;

/* Copyright(C)URARA-works 2007 */
