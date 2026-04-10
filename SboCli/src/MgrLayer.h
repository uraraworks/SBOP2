/// @file MgrLayer.h
/// @brief レイヤーマネージャクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "LayerBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

// クラス宣言

typedef class CMgrLayer
{
public:
			CMgrLayer();						// コンストラクタ
	virtual ~CMgrLayer();						// デストラクタ

	void Create(CMgrData *pMgrData);			// 作成
	void Destroy(void);							// 破棄

	void Draw(CImg32 *pDst);					// 描画
	BOOL TimerProc(void);						// 時間処理
	void Delete(int nID);						// 指定レイヤーを破棄
	void DeleteAll(void);						// 全てのレイヤーを破棄
	CLayerBase *Get(int nID);					// 指定IDのレイヤーを取得
	int GetLayerCount(void) const { return m_paLayer ? (int)m_paLayer->size() : 0; }	// レイヤー数取得

	// レイヤー作成
	void MakeLOGO(void);						// ロゴ
	void MakeTITLE(void);						// タイトル
	void MakeINFO(void);						// お知らせ
	void MakeLOGINMENU(void);					// ログインメニュー
	void MakeCHARSELECT(DWORD dwAccountID);	// キャラ選択
	void MakeMAP(void);							// マップ
	void MakeCLOUD(void);						// 雲
	void MakeSYSTEMMSG(void);					// システムメッセージ


private:
	CMgrData		*m_pMgrData;		// データ管理
	CMgrGrpData		*m_pMgrGrpData;		// グラフィックデータ管理

	PARRAYLAYERBASE	m_paLayer;			// レイヤー描画
} CMgrLayer, *PCMgrLayer;
