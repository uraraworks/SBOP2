/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:WindowSTATUS.h												 */
/* 内容			:強さウィンドウクラス 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/06/26													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowSTATUS : public CWindowBase
{
public:
			CWindowSTATUS();								/* コンストラクタ */
	virtual ~CWindowSTATUS();								/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */
	void	Draw		(CImg32 *pDst);								/* 描画 */
	void	SetID		(DWORD dwID);								/* キャラIDを設定 */


private:
	DWORD	m_dwCharID;						/* キャラID */
} CWindowSTATUS, *PCWindowSTATUS;

/* Copyright(C)URARA-works 2005 */
