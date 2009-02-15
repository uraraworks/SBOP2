/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:WindowCHAR_STATUS4.h										 */
/* 内容			:キャラ情報ウィンドウクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/02/15													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowCHAR_STATUS4 : public CWindowBase
{
public:
			CWindowCHAR_STATUS4();							/* コンストラクタ */
	virtual ~CWindowCHAR_STATUS4();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */


protected:
} CWindowCHAR_STATUS4, *PCWindowCHAR_STATUS4;

/* Copyright(C)URARA-works 2009 */
