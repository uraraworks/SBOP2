/// @file GetMD5.h
/// @brief MD5計算クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/06
/// @copyright Copyright(C)URARA-works 2005

#pragma once

extern "C"{
#include "md5.h"
}

class CGetMD5
{
public:
			CGetMD5();	// コンストラクタ
	virtual ~CGetMD5();	// デストラクタ

	void Init(void);	// 初期化
	void Update(PBYTE pSrc, DWORD dwSize);	// 更新
	void GetBin(PBYTE pDst);	// 取得(バイナリ)
	void GetStr(LPSTR pszDst);	// 取得(文字列)

private:
	MD5_CTX	m_stContext;	// バッファ
};

