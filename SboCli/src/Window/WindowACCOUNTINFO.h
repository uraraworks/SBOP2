/// @file WindowACCOUNTINFO.h
/// @brief アカウント情報ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/12/29
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowACCOUNTINFO : public CWindowBase
{
public:
			CWindowACCOUNTINFO();	// コンストラクタ
	virtual ~CWindowACCOUNTINFO();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画


private:
} CWindowACCOUNTINFO, *PCWindowACCOUNTINFO;
