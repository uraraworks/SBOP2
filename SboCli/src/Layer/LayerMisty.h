/// @file LayerMisty.h
/// @brief レイヤー描画クラス(霧) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/13
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;

// クラス宣言

typedef class CLayerMisty : public CLayerBase
{
public:
			CLayerMisty(); // コンストラクタ
	virtual ~CLayerMisty(); // デストラクタ

	void Create(CMgrData *pMgrData); // 作成
	void Draw(CImg32 *pDst); // 描画
} CLayerMisty, *PCLayerMisty;
