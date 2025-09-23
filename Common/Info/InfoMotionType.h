/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMotionType.h											 */
/* 内容			:モーション種別情報クラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/26													 */
/* ========================================================================= */

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoMotionType : public CInfoBase
{
public:
			CInfoMotionType();								/* コンストラクタ */
	virtual ~CInfoMotionType();								/* デストラクタ */

	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	void	Copy				(CInfoMotionType *pSrc);			/* コピー */


public:
	/* 保存しないデータ */

	/* 保存するデータ */
	DWORD		m_dwMotionTypeID;				/* モーション種別ID */
	WORD		m_wGrpIDSub;					/* プレビュー用グラフィックIDサブ */
	CmyString	m_strName;						/* モーション種別名 */
} CInfoMotionType, *PCInfoMotionType;
using ARRAYMOTIONTYPEINFO = CStdArray<PCInfoMotionType>;
using PARRAYMOTIONTYPEINFO = ARRAYMOTIONTYPEINFO *;

/* Copyright(C)URARA-works 2008 */
