/// @file InfoBase.cpp
/// @brief 情報基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/06/06
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include "InfoBase.h"

CInfoBase::CInfoBase()
{
	m_nElementCount = 0;
}

CInfoBase::~CInfoBase()
{
}

int CInfoBase::GetElementCount(void)
{
	return m_nElementCount;
}

int CInfoBase::GetElementNo(LPCSTR pszName)
{
	return 0;
}

DWORD CInfoBase::GetDataSize(void)
{
	return 0;
}

DWORD CInfoBase::GetDataSizeNo(int nNo)
{
	return 0;
}

LPCSTR CInfoBase::GetName(int nNo)
{
	return NULL;
}

PBYTE CInfoBase::GetWriteData(int nNo, PDWORD pdwSize)
{
	return NULL;
}

DWORD CInfoBase::ReadElementData(
	PBYTE pSrc,	// [in] データの読み込み元
	int nNo)	// [in] 要素番号
{
	return 0;
}

