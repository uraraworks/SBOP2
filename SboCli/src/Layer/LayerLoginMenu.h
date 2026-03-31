/// @file LayerLoginMenu.h
/// @brief レイヤー描画クラス(ログインメニュー) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/15
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "LayerCloud.h"

class CImg32;
class CMgrData;

// クラス宣言

typedef class CLayerLoginMenu : public CLayerCloud
{
public:
			CLayerLoginMenu(); // コンストラクタ
	virtual ~CLayerLoginMenu(); // デストラクタ

	void Create(CMgrData *pMgrData); // 作成
	void Draw(CImg32 *pDst); // 描画


private:
	DWORD m_dwLastTimeProc; // 最終処理時間
	CImg32 *m_pDibBack; // 背景画像
} CLayerLoginMenu, *PCLayerLoginMenu;
