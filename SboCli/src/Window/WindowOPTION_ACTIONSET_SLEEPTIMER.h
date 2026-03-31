/// @file WindowOPTION_ACTIONSET_SLEEPTIMER.h
/// @brief オプション-動作設定-おひるねタイマーの設定ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/21
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowOPTION_ACTIONSET_SLEEPTIMER : public CWindowBase
{
public:
			CWindowOPTION_ACTIONSET_SLEEPTIMER();	// コンストラクタ
	virtual ~CWindowOPTION_ACTIONSET_SLEEPTIMER();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画


private:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Z)


protected:
	int	m_nSleepTimer;	// おひるねタイマー
} CWindowOPTION_ACTIONSET_SLEEPTIMER, *PCWindowOPTION_ACTIONSET_SLEEPTIMER;
