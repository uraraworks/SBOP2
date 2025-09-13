/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMapEventINITSTATUS.h									 */
/* 内容			:イベント情報(ステータス初期化)クラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/05													 */
/* ========================================================================= */

#pragma once

#include "InfoMapEventBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMapEventINITSTATUS : public CInfoMapEventBase
{
public:
			CInfoMapEventINITSTATUS();						/* コンストラクタ */
	virtual ~CInfoMapEventINITSTATUS();						/* デストラクタ */

	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	void	Copy				(CInfoMapEventBase *pSrc);			/* コピー */


public:
	DWORD		m_dwEffectID;		/* エフェクトID */
} CInfoMapEventINITSTATUS, *PCInfoMapEventINITSTATUS;

/* Copyright(C)URARA-works 2008 */
