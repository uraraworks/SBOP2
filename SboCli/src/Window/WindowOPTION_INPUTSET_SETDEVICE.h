/// @file WindowOPTION_INPUTSET_SETDEVICE.h
/// @brief オプション-入力設定-入力デバイスの設定ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/12
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrKeyInput;

typedef class CWindowOPTION_INPUTSET_SETDEVICE : public CWindowBase
{
public:
			CWindowOPTION_INPUTSET_SETDEVICE();	// コンストラクタ
	virtual ~CWindowOPTION_INPUTSET_SETDEVICE();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画


protected:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Z)


protected:
	CMgrKeyInput	*m_MgrKeyInput;	// キー入力管理
} CWindowOPTION_INPUTSET_SETDEVICE, *PCWindowOPTION_INPUTSET_SETDEVICE;
