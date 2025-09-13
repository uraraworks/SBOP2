/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoEfcBalloon.h										 */
/* 内容			:噴出し情報ライブラリ基底クラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "LibInfoBase.h"
#include "InfoEfcBalloon.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLibInfoEfcBalloon : public CLibInfoBase
{
public:
			CLibInfoEfcBalloon();							/* コンストラクタ */
	virtual ~CLibInfoEfcBalloon();							/* デストラクタ */

	void Create			(void);										/* 作成 */
	void Destroy		(void);										/* 破棄 */

	virtual PCInfoBase GetNew	(void);								/* 新規データを取得 */
	virtual PCInfoBase GetPtr	(int nNo);							/* 情報を取得 */

	int		GetCount	(void);										/* データ数を取得 */
	void	Add			(PCInfoBase pInfo);							/* 追加 */
	void	Delete		(int nNo);									/* 削除 */
	void	Delete		(DWORD dwEfcBalloonID);						/* 削除 */
	void	DeleteAll	(void);										/* 全て削除 */
	void	SetList		(DWORD dwListID, CLibInfoEfcBalloon *pSrc);	/* 指定種別IDの情報を設定 */
	void	GetListID	(ARRAYDWORD &aDst);							/* 種別ID一覧を取得 */
	void	GetName		(DWORD dwListID, CmyString &strDst);		/* 噴出し名を取得 */
	void	Sort		(void);										/* 種別IDでソート */

	DWORD			GetNextAnimeID	(DWORD dwListID, DWORD dwAnimeID);		/* 次のコマ番号を取得 */
	CInfoEfcBalloon	*GetPtr			(DWORD dwListID, DWORD dwAnimeID);		/* 噴出し情報を取得 */

	void	GetEfcBalloonInfo	(DWORD dwListID, ARRAYEFCBALLOON &aDst);	/* 指定IDの噴出し情報一覧を取得 */
	DWORD	GetSendDataSize		(DWORD dwListID);							/* 送信データサイズを取得 */
	PBYTE	GetSendData			(DWORD dwListID);							/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);								/* 送信データから取り込み */


protected:
	DWORD	GetNewID	(void);										/* 新しいIDを取得 */


protected:
	DWORD	m_dwNewIDTmp;						/* 新規ID作成用 */
	PARRAYEFCBALLOON	m_paInfo;				/* 噴出し情報 */
} CLibInfoEfcBalloon, *PCLibInfoEfcBalloon;

/* Copyright(C)URARA-works 2007 */
