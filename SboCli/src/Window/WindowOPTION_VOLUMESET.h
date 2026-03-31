/// @file WindowOPTION_VOLUMESET.h
/// @brief オプション-音量設定ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/28
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowOPTION_VOLUMESET : public CWindowBase
{
public:
			CWindowOPTION_VOLUMESET();	// コンストラクタ
	virtual ~CWindowOPTION_VOLUMESET();	// デストラクタ

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
	int	m_nVolumeBgm,	// BGM音量
			m_nVolumeSound,	// 効果音音量
			m_nBackVolumeBgm,	// 変更前のBGM音量
			m_nBackVolumeSound;	// 変更前の効果音音量
} CWindowOPTION_VOLUMESET, *PCWindowOPTION_VOLUMESET;
