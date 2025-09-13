/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LayerTitle.h												 */
/* 内容			:レイヤー描画クラス(タイトル) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/03/26													 */
/* ========================================================================= */

#pragma once

#include "LayerCloud.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLayerTitle : public CLayerBase
{
public:
			CLayerTitle();							/* コンストラクタ */
	virtual ~CLayerTitle();						/* デストラクタ */

	void	Create		(CMgrData *pMgrData);				/* 作成 */
	void	Draw		(CImg32 *pDst);						/* 描画 */
	BOOL	TimerProc	(void);								/* 時間処理 */
	void	StartFadeIn	(void);								/* フェードイン開始 */
	void	EndFadeIn	(void);								/* フェードイン終了 */
	BOOL	IsFadeInEnd	(void);								/* フェードイン終了？ */


private:
	int			m_nAnime,						/* アニメーション番号 */
				m_nFadeLevel,					/* フェードレベル */
				m_nCloudPos;					/* 雲の位置用 */
	DWORD		m_dwLastTimeProc,				/* 最終処理時間 */
				m_dwLastTimeFadeIn;				/* 最終処理時間(フェードイン) */
	CImg32		*m_pDibTitle,					/* タイトル画像 */
				*m_pDibTitleBack,				/* タイトル背景画像 */
				*m_pDibTitleCloud;				/* タイトル雲画像 */
} CLayerTitle, *PCLayerTitle;

/* Copyright(C)URARA-works 2007 */
