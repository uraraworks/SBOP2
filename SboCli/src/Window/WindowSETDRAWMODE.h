/// @file WindowSETDRAWMODE.h
/// @brief 表示内容の設定ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/24
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowSETDRAWMODE : public CWindowBase
{
public:
			CWindowSETDRAWMODE();	// コンストラクタ
	virtual ~CWindowSETDRAWMODE();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画


private:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Z)
} CWindowSETDRAWMODE, *PCWindowSETDRAWMODE;
