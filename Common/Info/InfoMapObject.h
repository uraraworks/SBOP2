/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMapObject.h											 */
/* 内容			:マップオブジェクトクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/01													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ========================================================================= */
/* 構造体定義																 */
/* ========================================================================= */

/* マップオブジェクトアニメ情報 */
typedef struct _STMAPOBJECTANIMEINFO {
	BYTE	byWait,				/* 待ち時間(×１０ミリ秒) */
			byLevel;			/* 透明度 */
	PWORD	pwGrpID;			/* 画像ID */
} STMAPOBJECTANIMEINFO, *PSTMAPOBJECTANIMEINFO;
typedef CmyArray<PSTMAPOBJECTANIMEINFO, PSTMAPOBJECTANIMEINFO>	   ARRAYMAPOBJECTANIMEINFO;
typedef CmyArray<PSTMAPOBJECTANIMEINFO, PSTMAPOBJECTANIMEINFO>	 *PARRAYMAPOBJECTANIMEINFO;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMapObject : public CInfoBase
{
public:
			CInfoMapObject();								/* コンストラクタ */
	virtual ~CInfoMapObject();								/* デストラクタ */

	int		GetElementCount		(void);								/* 要素数を取得 */
	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	void	Copy				(CInfoMapObject *pSrc);				/* コピー */
	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */
	void	GetAnimeSize		(int nNo = -1);						/* オブジェクトアニメ情報サイズを取得 */

	int		GetAnimeCount		(void);								/* アニメーションコマ数を取得 */
	void	AddAnime			(void);								/* アニメーションコマを追加 */
	void	DeleteAnime			(int nNo);							/* アニメーションコマを削除 */
	void	DeleteAllAnime		(void);								/* アニメーションコマを全て削除 */
	void	RenewGrpSize		(int cx = -1, int cy = -1);			/* 画像サイズを更新 */
	void	SetGrpID			(int nNo, int x, int y, WORD wGrpID);	/* 画像IDを設定 */

	PSTMAPOBJECTANIMEINFO	GetAnimePtr	(int nNo);					/* アニメーションコマを取得 */


public:
	/* 保存するデータ */
	DWORD				m_dwObjectID,						/* オブジェクトID */
						m_dwAttr;							/* オブジェクトの属性 */
	SIZE				m_sizeGrp;							/* 画像サイズ */
	CmyString			m_strName;							/* オブジェクト名 */

	ARRAYMAPOBJECTANIMEINFO	m_aInfoAnime;					/* オブジェクトアニメ情報 */
} CInfoMapObject, *PCInfoMapObject;
typedef CmyArray<PCInfoMapObject, PCInfoMapObject>	  ARRAYMAPOBJECT;
typedef CmyArray<PCInfoMapObject, PCInfoMapObject>	*PARRAYMAPOBJECT;

/* Copyright(C)URARA-works 2008 */
