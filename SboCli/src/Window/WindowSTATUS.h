/// @file WindowSTATUS.h
/// @brief 強さウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/06/26
/// @copyright Copyright(C)URARA-works 2005

#pragma once

#include "WindowBase.h"

class CMgrData;

typedef class CWindowSTATUS : public CWindowBase
{
public:
			CWindowSTATUS();	// コンストラクタ
	virtual ~CWindowSTATUS();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
	void	SetID(DWORD dwID);	// キャラIDを設定


private:
	DWORD	m_dwCharID;	// キャラID
} CWindowSTATUS, *PCWindowSTATUS;
