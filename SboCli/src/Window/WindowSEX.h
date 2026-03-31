/// @file WindowSEX.h
/// @brief 性別ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/14
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

typedef class CWindowSEX : public CWindowBase
{
public:
			CWindowSEX();	// コンストラクタ
	virtual ~CWindowSEX();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
	BOOL	TimerProc(void);	// 時間処理
	void	SetType(int nSex);	// 性別を設定
	int	GetType(void);	// 性別を取得


private:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Y)


private:
	int	m_nPos;	// 選択位置
} CWindowSEX, *PCWindowSEX;
