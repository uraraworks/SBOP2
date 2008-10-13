/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LayerMisty.h												 */
/* 内容			:レイヤー描画クラス(霧) 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/13													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLayerMisty : public CLayerBase
{
public:
			CLayerMisty();						/* コンストラクタ */
	virtual ~CLayerMisty();						/* デストラクタ */

	void	Create		(CMgrData *pMgrData);			/* 作成 */
	void	Draw		(CImg32 *pDst);					/* 描画 */
} CLayerMisty, *PCLayerMisty;

/* Copyright(C)URARA-works 2008 */
