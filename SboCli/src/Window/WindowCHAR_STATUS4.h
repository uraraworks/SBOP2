/// @file WindowCHAR_STATUS4.h
/// @brief キャラ情報ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/02/15
/// @copyright Copyright(C)URARA-works 2009

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowCHAR_STATUS4 : public CWindowBase
{
public:
			CWindowCHAR_STATUS4();	// コンストラクタ
	virtual ~CWindowCHAR_STATUS4();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画


protected:
} CWindowCHAR_STATUS4, *PCWindowCHAR_STATUS4;
