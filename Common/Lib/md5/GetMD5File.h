/// @file GetMD5File.h
/// @brief MD5計算クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/28
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "GetMD5.h"

class CGetMD5File : public CGetMD5
{
public:
			CGetMD5File();	// コンストラクタ
	virtual ~CGetMD5File();	// デストラクタ

	void Update(LPCSTR pszFileName);	// 更新
};

