/// @file WindowCHAR_STATUS2.h
/// @brief キャラ-ステータス2ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/27
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowCHAR_STATUS2 : public CWindowBase
{
public:
			CWindowCHAR_STATUS2();	// コンストラクタ
	virtual ~CWindowCHAR_STATUS2();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
} CWindowCHAR_STATUS2, *PCWindowCHAR_STATUS2;
