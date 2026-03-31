/// @file GetMD5.cpp
/// @brief MD5計算クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/06
/// @copyright Copyright(C)URARA-works 2005

#include "StdAfx.h"
#include <cstdio>
#include "GetMD5.h"

CGetMD5::CGetMD5()
{
	Init();
}

CGetMD5::~CGetMD5()
{
}

void CGetMD5::Init(void)
{
	MD5Init(&m_stContext);
}

void CGetMD5::Update(
	PBYTE pSrc,	// [in] 計算したいバッファ
	DWORD dwSize)	// [in] バッファサイズ
{
	MD5Update(&m_stContext, pSrc, dwSize);
}

void CGetMD5::GetBin(
	PBYTE pDst)	// [ou] 計算された値(16バイト)
{
	MD5Final(pDst, &m_stContext);
	Init();
}

void CGetMD5::GetStr(
	LPSTR pszDst)	// [ou] 計算された値(33バイト)
{
	int i;
	BYTE aTmp[16];

	MD5Final(aTmp, &m_stContext);

       for (i = 0; i < 16; i ++) {
               sprintf_s(&pszDst[i * 2], 3, "%02X", aTmp[i]);
       }
}

