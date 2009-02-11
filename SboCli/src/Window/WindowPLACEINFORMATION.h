/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:WindowPLACEINFORMATION.h									 */
/* 内容			:場所情報ウィンドウクラス 定義ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/02/11													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowPLACEINFORMATION : public CWindowBase
{
public:
			CWindowPLACEINFORMATION();						/* コンストラクタ */
	virtual ~CWindowPLACEINFORMATION();						/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */


protected:
} CWindowPLACEINFORMATION, *PCWindowPLACEINFORMATION;

/* Copyright(C)URARA-works 2009 */
