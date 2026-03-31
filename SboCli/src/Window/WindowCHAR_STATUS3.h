/// @file WindowCHAR_STATUS3.h
/// @brief キャラ-ステータス3ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/27
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowCHAR_STATUS3 : public CWindowBase
{
public:
			CWindowCHAR_STATUS3();	// コンストラクタ
	virtual ~CWindowCHAR_STATUS3();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
} CWindowCHAR_STATUS3, *PCWindowCHAR_STATUS3;
