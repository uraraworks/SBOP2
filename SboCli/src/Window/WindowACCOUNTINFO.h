/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:WindowACCOUNTINFO.h										 */
/* 内容			:アカウント情報ウィンドウクラス 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/29													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowACCOUNTINFO : public CWindowBase
{
public:
			CWindowACCOUNTINFO();							/* コンストラクタ */
	virtual ~CWindowACCOUNTINFO();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */


private:
} CWindowACCOUNTINFO, *PCWindowACCOUNTINFO;

/* Copyright(C)URARA-works 2006 */
