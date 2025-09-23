/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:InfoBase.h													 */
/* 内容			:情報基底クラス 定義ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/06/06													 */
/* ========================================================================= */

#pragma once
#include <vector>

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CInfoBase
{
public:
			CInfoBase();											/* コンストラクタ */
	virtual ~CInfoBase();											/* デストラクタ */

	virtual int		GetElementCount		(void);								/* 要素数を取得 */
	virtual int		GetElementNo		(LPCSTR pszName);					/* 要素番号を取得 */
	virtual DWORD	GetDataSize			(void);								/* データサイズを取得 */
	virtual DWORD	GetDataSizeNo		(int nNo);							/* 指定要素のデータサイズを取得 */
	virtual LPCSTR	GetName				(int nNo);							/* 要素名を取得 */
	virtual PBYTE	GetWriteData		(int nNo, PDWORD pdwSize);			/* 指定要素の保存用データを取得 */
	virtual DWORD	ReadElementData		(PBYTE pSrc, int nNo);				/* 指定要素データを読み込み */


public:
	int			m_nElementCount;					/* 要素数 */
} CInfoBase, *PCInfoBase;
typedef std::vector<PCInfoBase>	  ARRAYBASEINFO;
typedef std::vector<PCInfoBase>	*PARRAYBASEINFO;

/* Copyright(C)URARA-works 2005 */
