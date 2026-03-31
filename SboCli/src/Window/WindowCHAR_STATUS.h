/// @file WindowCHAR_STATUS.h
/// @brief キャラ-ステータスウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/27
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowCHAR_STATUS : public CWindowBase
{
public:
			CWindowCHAR_STATUS();	// コンストラクタ
	virtual ~CWindowCHAR_STATUS();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画


private:
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Z)
	BOOL	OnJ(BOOL bDown);	// キーハンドラ(J)
} CWindowCHAR_STATUS, *PCWindowCHAR_STATUS;
