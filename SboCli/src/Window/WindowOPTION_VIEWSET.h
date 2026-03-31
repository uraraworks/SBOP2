/// @file WindowOPTION_VIEWSET.h
/// @brief オプション-表示設定ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/27
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowOPTION_VIEWSET : public CWindowBase
{
public:
			CWindowOPTION_VIEWSET();	// コンストラクタ
	virtual ~CWindowOPTION_VIEWSET();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
	BOOL	TimerProc(void);	// 時間処理


protected:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Z)
	BOOL	GetCheck(int nNo);	// チェック状態を取得


protected:
	ARRAYINT	m_anCheck;	// チェックボックス描画用
	ARRAYDWORD	m_adwCheckTime;	// チェックボックス描画用
} CWindowOPTION_VIEWSET, *PCWindowOPTION_VIEWSET;
