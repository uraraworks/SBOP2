/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:LayerInfo.h												 */
/* 内容			:レイヤー描画クラス(お知らせ) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/08/16													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLayerInfo : public CLayerBase
{
public:
			CLayerInfo();							/* コンストラクタ */
	virtual ~CLayerInfo();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);				/* 作成 */
	void	Draw		(CImg32 *pDst);						/* 描画 */


private:
	void	DrawFrame	(CImg32 *pDst);						/* フレームの描画 */


private:
	int		m_nX;					/* 表示位置(横) */
	int		m_nY;					/* 表示位置(縦) */
	int		m_nCx;					/* 幅 */
	int		m_nCy;					/* 高さ */
} CLayerInfo, *PCLayerInfo;

/* Copyright(C)URARA-works 2005 */
