/// @file WindowSETBGMVOLUME.h
/// @brief BGM音量の設定ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/21
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowSETBGMVOLUME : public CWindowBase
{
public:
			CWindowSETBGMVOLUME();	// コンストラクタ
	virtual ~CWindowSETBGMVOLUME();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画


private:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Z)
} CWindowSETBGMVOLUME, *PCWindowSETBGMVOLUME;
