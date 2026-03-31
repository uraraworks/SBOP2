/// @file InfoBase.h
/// @brief 情報基底クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/06/06
/// @copyright Copyright(C)URARA-works 2005

#pragma once
#include <vector>
#include "myArray.h"

typedef class CInfoBase
{
public:
			CInfoBase();	// コンストラクタ
	virtual ~CInfoBase();	// デストラクタ

	virtual int	GetElementCount(void);	// 要素数を取得
	virtual int	GetElementNo(LPCSTR pszName);	// 要素番号を取得
	virtual DWORD	GetDataSize(void);	// データサイズを取得
	virtual DWORD	GetDataSizeNo(int nNo);	// 指定要素のデータサイズを取得
	virtual LPCSTR	GetName(int nNo);	// 要素名を取得
	virtual PBYTE	GetWriteData(int nNo, PDWORD pdwSize);	// 指定要素の保存用データを取得
	virtual DWORD	ReadElementData(PBYTE pSrc, int nNo);	// 指定要素データを読み込み

public:
	int	m_nElementCount;	// 要素数
} CInfoBase, *PCInfoBase;
using ARRAYBASEINFO = CStdArray<PCInfoBase>;
using PARRAYBASEINFO = ARRAYBASEINFO *;

