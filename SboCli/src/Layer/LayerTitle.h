/// @file LayerTitle.h
/// @brief レイヤー描画クラス(タイトル) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/03/26
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "LayerCloud.h"

class CImg32;
class CMgrData;

// クラス宣言

typedef class CLayerTitle : public CLayerBase
{
public:
			CLayerTitle(); // コンストラクタ
	virtual ~CLayerTitle(); // デストラクタ

	void Create(CMgrData *pMgrData); // 作成
	void Draw(CImg32 *pDst); // 描画
	BOOL TimerProc(void); // 時間処理
	void StartFadeIn(void); // フェードイン開始
	void EndFadeIn(void); // フェードイン終了
	void SetFadeLevel(int nFadeLevel); // フェードレベルを直接設定
	BOOL IsFadeInEnd(void); // フェードイン終了？


private:
	int m_nAnime, // アニメーション番号
		m_nFadeLevel, // フェードレベル
		m_nCloudPos; // 雲の位置用
	DWORD m_dwLastTimeProc, // 最終処理時間
		m_dwLastTimeFadeIn; // 最終処理時間(フェードイン)
	CImg32 *m_pDibTitle, // タイトル画像
		*m_pDibTitleBack, // タイトル背景画像
		*m_pDibTitleCloud; // タイトル雲画像
} CLayerTitle, *PCLayerTitle;
