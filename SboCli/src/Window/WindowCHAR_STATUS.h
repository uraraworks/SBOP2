/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowCHAR_STATUS.h										 */
/* 内容			:キャラ-ステータスウィンドウクラス 定義ファイル				 */
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

typedef class CWindowCHAR_STATUS : public CWindowBase
{
public:
			CWindowCHAR_STATUS();							/* コンストラクタ */
	virtual ~CWindowCHAR_STATUS();							/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */


private:
	BOOL	OnX			(BOOL bDown);						/* キーハンドラ(X) */
	BOOL	OnZ			(BOOL bDown);						/* キーハンドラ(Z) */
	BOOL	OnJ			(BOOL bDown);						/* キーハンドラ(J) */
} CWindowCHAR_STATUS, *PCWindowCHAR_STATUS;

/* Copyright(C)URARA-works 2008 */
