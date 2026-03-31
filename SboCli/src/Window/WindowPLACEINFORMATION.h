/// @file WindowPLACEINFORMATION.h
/// @brief 場所情報ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/02/11
/// @copyright Copyright(C)URARA-works 2009

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowPLACEINFORMATION : public CWindowBase
{
public:
			CWindowPLACEINFORMATION();	// コンストラクタ
	virtual ~CWindowPLACEINFORMATION();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画


protected:
} CWindowPLACEINFORMATION, *PCWindowPLACEINFORMATION;
