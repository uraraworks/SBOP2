/// @file WindowHAIRTYPE.h
/// @brief 髪型選択ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/08
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

typedef class CWindowHAIRTYPE : public CWindowBase
{
public:
			CWindowHAIRTYPE();	// コンストラクタ
	virtual ~CWindowHAIRTYPE();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
	BOOL	TimerProc(void);	// 時間処理
	void	SetType(WORD wHairType);	// 髪型を設定
	WORD	GetType(void);	// 髪型を取得


private:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Y)


private:
	int	m_nPosX;	// 桁の選択位置
} CWindowHAIRTYPE, *PCWindowHAIRTYPE;
