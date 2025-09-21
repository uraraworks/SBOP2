/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LayerLoginMenu.h											 */
/* 内容			:レイヤー描画クラス(ログインメニュー) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/15													 */
/* ========================================================================= */

#pragma once

#include "LayerCloud.h"

class CImg32;
class CMgrData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CLayerLoginMenu : public CLayerCloud
{
public:
			CLayerLoginMenu();						/* コンストラクタ */
	virtual ~CLayerLoginMenu();						/* デストラクタ */

	void	Create		(CMgrData *pMgrData);				/* 作成 */
	void	Draw		(CImg32 *pDst);						/* 描画 */


private:
	DWORD		m_dwLastTimeProc;				/* 最終処理時間 */
	CImg32		*m_pDibBack;					/* 背景画像 */
} CLayerLoginMenu, *PCLayerLoginMenu;

/* Copyright(C)URARA-works 2007 */
