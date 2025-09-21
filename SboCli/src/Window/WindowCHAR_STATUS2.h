/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowCHAR_STATUS2.h										 */
/* 内容			:キャラ-ステータス2ウィンドウクラス 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/27													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowCHAR_STATUS2 : public CWindowBase
{
public:
			CWindowCHAR_STATUS2();							/* コンストラクタ */
	virtual ~CWindowCHAR_STATUS2();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */
} CWindowCHAR_STATUS2, *PCWindowCHAR_STATUS2;

/* Copyright(C)URARA-works 2008 */
