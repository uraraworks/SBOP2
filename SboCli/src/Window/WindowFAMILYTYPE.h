/// @file WindowFAMILYTYPE.h
/// @brief 種族選択ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/07
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

typedef class CWindowFAMILYTYPE : public CWindowBase
{
public:
			CWindowFAMILYTYPE();	// コンストラクタ
	virtual ~CWindowFAMILYTYPE();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
	BOOL	TimerProc(void);	// 時間処理
	WORD	GetType(void);	// 種族を取得


private:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Y)


private:
	int	m_nPos;	// 選択位置
} CWindowFAMILYTYPE, *PCWindowFAMILYTYPE;
