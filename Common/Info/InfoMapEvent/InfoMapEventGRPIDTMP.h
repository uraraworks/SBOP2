/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMapEventGRPIDTMP.h										 */
/* 内容			:イベント情報(一時画像設定)クラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/12													 */
/* ========================================================================= */

#pragma once

#include "InfoMapEventBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

enum {
	GRPIDTMPTYPE_OFF = 0,			/* 解除 */
	GRPIDTMPTYPE_ON,				/* 設定 */
	GRPIDTMPTYPE_MAX
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMapEventGRPIDTMP : public CInfoMapEventBase
{
public:
			CInfoMapEventGRPIDTMP();						/* コンストラクタ */
	virtual ~CInfoMapEventGRPIDTMP();						/* デストラクタ */

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
	int			m_nSetType;		/* 設定種別 */
	DWORD		m_dwIDMain,		/* 画像IDメイン */
				m_dwIDSub;		/* 画像IDサブ */
} CInfoMapEventGRPIDTMP, *PCInfoMapEventGRPIDTMP;

/* Copyright(C)URARA-works 2008 */
