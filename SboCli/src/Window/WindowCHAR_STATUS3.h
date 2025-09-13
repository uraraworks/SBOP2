/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowCHAR_STATUS3.h										 */
/* 内容			:キャラ-ステータス3ウィンドウクラス 定義ファイル			 */
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

typedef class CWindowCHAR_STATUS3 : public CWindowBase
{
public:
			CWindowCHAR_STATUS3();							/* コンストラクタ */
	virtual ~CWindowCHAR_STATUS3();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */
} CWindowCHAR_STATUS3, *PCWindowCHAR_STATUS3;

/* Copyright(C)URARA-works 2008 */
