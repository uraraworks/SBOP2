/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoMapEventMOVE.h											 */
/* 内容			:イベント情報(マップ内移動)クラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/05													 */
/* ========================================================================= */

#pragma once

#include "InfoMapEventBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMapEventMOVE : public CInfoMapEventBase
{
public:
			CInfoMapEventMOVE();							/* コンストラクタ */
	virtual ~CInfoMapEventMOVE();							/* デストラクタ */

	void	RenewSize			(int nDirection, int nSize, SIZE *pSize);	/* サイズ更新 */
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
	POINT		m_ptDst;			/* 移動先 */
	int			m_nDirection;		/* 移動後の向き */
} CInfoMapEventMOVE, *PCInfoMapEventMOVE;
typedef CmyArray<PCInfoMapEventMOVE, PCInfoMapEventMOVE>	  ARRAYMAPEVENTMAPINFO;
typedef CmyArray<PCInfoMapEventMOVE, PCInfoMapEventMOVE>	*PARRAYMAPEVENTMAPINFO;

/* Copyright(C)URARA-works 2007 */
