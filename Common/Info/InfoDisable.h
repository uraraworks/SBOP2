/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:InfoDisable.h												 */
/* 内容			:拒否情報クラス 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/08													 */
/* ========================================================================= */

#pragma once

#include "InfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoDisable : public CInfoBase
{
public:
			CInfoDisable();									/* コンストラクタ */
	virtual ~CInfoDisable();								/* デストラクタ */

	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	void	Copy				(CInfoDisable *pSrc);					/* コピー */


public:
	/* 保存するデータ */
	DWORD		m_dwDisableID;								/* 拒否情報ID */
	CmyString	m_strMacAddress;							/* MACアドレス */
} CInfoDisable, *PCInfoDisable;
typedef CmyArray<PCInfoDisable, PCInfoDisable>	   ARRAYDISABLEINFO;
typedef CmyArray<PCInfoDisable, PCInfoDisable>	 *PARRAYDISABLEINFO;

/* Copyright(C)URARA-works 2009 */
