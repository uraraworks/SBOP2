/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoFishing.h												 */
/* 内容			:釣り情報クラス 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/02													 */
/* ========================================================================= */

#pragma once
#include <vector>
#include "myArray.h"

#include "InfoBase.h"

/* ========================================================================= */
/* 釣り情報パラメータ														 */
/* ========================================================================= */
typedef struct _INFOFISHINGPARAM {
	DWORD		dwItemTypeID;			/* アイテム種別ID */
	int			nAverage;				/* 釣りきる確率 */
} INFOFISHINGPARAM, *PINFOFISHINGPARAM;


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoFishing : public CInfoBase
{
public:
			CInfoFishing();									/* コンストラクタ */
	virtual ~CInfoFishing();								/* デストラクタ */

	int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	DWORD	GetDataSize			(void);								/* データサイズを取得 */
	DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */

	DWORD	GetSendDataSize		(void);								/* 送信データサイズを取得 */
	PBYTE	GetSendData			(void);								/* 送信データを取得 */
	PBYTE	SetSendData			(PBYTE pSrc);						/* 送信データから取り込み */

	void	Copy				(CInfoFishing *pSrc);				/* コピー */

	int		GetCount			(void);								/* 釣り情報パラメータ数を取得 */
	void	AddParam			(DWORD dwItemTypeID, int nAverage);	/* 釣り情報パラメータを追加 */
	void	DeleteParam			(int nNo);							/* 釣り情報パラメータを削除 */
	void	DeleteAllParam		(void);								/* 釣り情報パラメータを全て削除 */
	PINFOFISHINGPARAM	GetPtr	(int nNo);							/* 釣り情報パラメータを取得 */


public:
	/* 保存しないデータ */

	/* 保存するデータ */
	DWORD		m_dwFishingID;					/* 釣りID */
	int			m_nAverage;						/* 釣れる確率 */
	CmyString	m_strName;						/* 釣り場名 */
	std::vector<PINFOFISHINGPARAM>	m_apParam;	/* 釣り情報パラメータ */
} CInfoFishing, *PCInfoFishing;
using ARRAYITEMINFO = CStdArray<PCInfoFishing>;
using PARRAYITEMINFO = ARRAYITEMINFO *;

/* Copyright(C)URARA-works 2008 */
