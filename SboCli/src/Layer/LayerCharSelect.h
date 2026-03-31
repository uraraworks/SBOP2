/// @file LayerCharSelect.h
/// @brief レイヤー描画クラス(キャラ選択) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/07/04
/// @copyright Copyright(C)URARA-works 2005

#pragma once

#include "LayerBase.h"

class CImg32;
class CInfoAccount;

// クラス宣言

typedef class CLayerCharSelect : public CLayerBase
{
public:
			CLayerCharSelect(); // コンストラクタ
	virtual ~CLayerCharSelect(); // デストラクタ

	void Create(CMgrData *pMgrData, DWORD dwAccountID); // 作成
	void Draw(CImg32 *pDst); // 描画
	BOOL TimerProc(void); // 時間処理

	void SetSelect(int nNo); // 選択位置を設定
	DWORD GetSelectCharID(void); // 選択中のキャラIDを取得


private:
	void DrawFrame(CImg32 *pDst, int x, int y, int cx, int cy, int nType); // フレームを描画


private:
	int m_nSelect; // 選択位置
	BYTE m_byAnime; // アニメーション番号
	DWORD m_dwTimeLastAnime; // 前回のアニメーション処理時間

	CInfoAccount *m_pInfoAccount; // アカウント情報
} CLayerCharSelect, *PCLayerCharSelect;
