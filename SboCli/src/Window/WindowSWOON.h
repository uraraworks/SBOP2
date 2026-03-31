/// @file WindowSWOON.h
/// @brief 気絶メニューウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/02
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowSWOON : public CWindowBase
{
public:
			CWindowSWOON();	// コンストラクタ
	virtual ~CWindowSWOON();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画


private:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Z)
} CWindowSWOON, *PCWindowSWOON;
