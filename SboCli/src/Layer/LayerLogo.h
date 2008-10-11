/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名：	LayerLogo.h													 */
/* 内容：		レイヤー描画クラス(ロゴ) 定義ファイル						 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2005/05/17													 */
/* ========================================================================= */

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLayerLogo : public CLayerBase
{
public:
			CLayerLogo();								/* コンストラクタ */
	virtual ~CLayerLogo();								/* デストラクタ */

	void	Create		(CMgrData *pMgrData);				/* 作成 */
	void	Draw		(CImg32 *pDst);						/* 描画 */


private:
	CImg32			*m_pDibLogo;						/* ロゴ画像 */
} CLayerLogo, *PCLayerLogo;

/* Copyright(C)URARA-works 2005 */
