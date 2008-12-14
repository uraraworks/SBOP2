/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LayerSnow.h												 */
/* 内容			:レイヤー描画クラス(雪) 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/14													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* 構造体宣言																 */
/* ========================================================================= */
/* 雪情報 */
typedef struct _STLAYERSNOW_SNOWINFO {
	int		nSize,			/* サイズ */
			nEndY,			/* 着地点 */
			x,				/* 横座標 */
			y;				/* 縦座標 */
	DWORD	dwStartWait,	/* 開始までの待ち時間 */
			dwWait,			/* 速度 */
			dwLastProc;		/* 前回の処理時間 */
} STLAYERSNOW_SNOWINFO, *PSTLAYERSNOW_SNOWINFO;
typedef CmyArray<PSTLAYERSNOW_SNOWINFO, PSTLAYERSNOW_SNOWINFO> ARRAYSNOWINFO;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLayerSnow : public CLayerBase
{
public:
			CLayerSnow();						/* コンストラクタ */
	virtual ~CLayerSnow();						/* デストラクタ */

	void	Create		(CMgrData *pMgrData);			/* 作成 */
	void	Draw		(CImg32 *pDst);					/* 描画 */
	BOOL	TimerProc	(void);							/* 時間処理 */


protected:
	void	RenewSnowInfo		(int nCount);			/* 雪情報を更新 */
	void	DeleteSnowInfoAll	(void);					/* 雪情報を全て削除 */


protected:
	DWORD			m_dwLastProc;		/* 前回の処理時間 */
	ARRAYSNOWINFO	m_aSnowInfo;		/* 雪情報 */
} CLayerSnow, *PCLayerSnow;

/* Copyright(C)URARA-works 2008 */
