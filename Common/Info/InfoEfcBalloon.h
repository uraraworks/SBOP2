/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoEfcBalloon.h											 */
/* 内容			:噴出し情報クラス 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#pragma once

#include "InfoAnime.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoEfcBalloon : public CInfoBase
{
public:
			CInfoEfcBalloon();								/* コンストラクタ */
	virtual ~CInfoEfcBalloon();								/* デストラクタ */

	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	void	Copy				(CInfoEfcBalloon *pSrc);			/* コピー */
	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */


public:
	/* 保存しないデータ */

	/* 保存するデータ */
	BYTE		m_byWait;							/* 待ち時間(×１０ミリ秒) */
	BOOL		m_bLoop;							/* ループ再生 */
	DWORD		m_dwEfcBalloonID,					/* 噴出しID */
				m_dwListID,							/* 噴出し種別ID */
				m_dwAnimeID,						/* コマ番号 */
				m_dwSoundID,						/* 効果音ID */
				m_dwGrpID;							/* 画像ID */
	CmyString	m_strName;							/* 噴出し名 */
} CInfoEfcBalloon, *PCInfoEfcBalloon;
typedef CmyArray<PCInfoEfcBalloon, PCInfoEfcBalloon>	  ARRAYEFCBALLOON;
typedef CmyArray<PCInfoEfcBalloon, PCInfoEfcBalloon>	*PARRAYEFCBALLOON;

/* Copyright(C)URARA-works 2007 */
