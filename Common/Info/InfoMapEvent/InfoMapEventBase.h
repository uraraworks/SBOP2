/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoMapEventBase.h											 */
/* 内容			:イベント情報基底クラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* イベント種別 */
enum {
	MAPEVENTTYPE_NONE = 0,
	MAPEVENTTYPE_MOVE,									/* マップ内移動 */
	MAPEVENTTYPE_MAPMOVE,								/* マップ間移動 */
	MAPEVENTTYPE_TRASHBOX,								/* ゴミ箱 */
	MAPEVENTTYPE_INITSTATUS,							/* ステータス初期化 */
	MAPEVENTTYPE_GRPIDTMP,								/* 一時画像設定 */
	MAPEVENTTYPE_LIGHT,									/* 灯り */
	MAPEVENTTYPE_MAX
};

/* 当たり判定種別 */
enum {
	MAPEVENTHITTYPE_MAPPOS = 0,							/* マップ座標で判定 */
	MAPEVENTHITTYPE_CHARPOS,							/* キャラ座標で判定 */
	MAPEVENTHITTYPE_AREA,								/* 範囲で判定 */
	MAPEVENTHITTYPE_MAX
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMapEventBase : public CInfoBase
{
public:
			CInfoMapEventBase();									/* コンストラクタ */
	virtual ~CInfoMapEventBase();									/* デストラクタ */

	virtual void	RenewSize			(int nDirection, int nSize);		/* サイズ更新 */
	virtual int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	virtual DWORD	GetDataSize			(void);								/* データサイズを取得 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	virtual LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	virtual DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	virtual PBYTE	GetSendData			(void);								/* 送信データを取得 */
	virtual PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	virtual void	Copy				(CInfoMapEventBase *pSrc);			/* コピー */


public:
	int			m_nElementCountBase;			/* 基底クラスの要素数 */

	DWORD		m_dwMapEventID,					/* マップイベントID */
				m_dwSoundID;					/* 実行時の効果音ID */
	int			m_nType,						/* イベント種別 */
				m_nHitType,						/* 当たり判定種別 */
				m_nHitDirection;				/* 判定向き */
	POINT		m_ptPos,						/* 座標1 */
				m_ptPos2;						/* 座標2 */
} CInfoMapEventBase, *PCInfoMapEventBase;
typedef CmyArray<PCInfoMapEventBase, PCInfoMapEventBase>	  ARRAYMAPEVENTBASEINFO;
typedef CmyArray<PCInfoMapEventBase, PCInfoMapEventBase>	*PARRAYMAPEVENTBASEINFO;

/* Copyright(C)URARA-works 2007 */
