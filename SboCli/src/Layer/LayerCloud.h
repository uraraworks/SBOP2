/// @file LayerCloud.h
/// @brief レイヤー描画クラス(雲) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/09/08
/// @copyright Copyright(C)URARA-works 2005

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;
class CLayerMap;

// クラス宣言

typedef class CLayerCloud : public CLayerBase
{
public:
			CLayerCloud(); // コンストラクタ
	virtual ~CLayerCloud(); // デストラクタ

	void Create(CMgrData *pMgrData); // 作成
	void Draw(CImg32 *pDst); // 描画
	BOOL TimerProc(void); // 時間処理


public:
	CLayerMap *m_pLayerMap; // マップレイヤー


protected:
	int m_nCount, // 移動カウンタ
		m_nPosXBack, // 基準点(横)
		m_nPosYBack, // 基準点(縦)
		m_nPosX, // 基準点(横)
		m_nPosY; // 基準点(縦)
	DWORD m_dwLastProc; // 前回の処理時間
	CImg32 *m_pDibCloud; // 雲画像
} CLayerCloud, *PCLayerCloud;
