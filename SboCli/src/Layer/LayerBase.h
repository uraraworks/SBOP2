/// @file LayerBase.h
/// @brief レイヤー描画基底クラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#pragma once
#include <vector>
#include "myArray.h"

class CImg32;
class CMgrData;
class CMgrGrpData;
class CMgrDraw;
class CLibInfoCharCli;
class CLibInfoMapBase;

// クラス宣言

typedef class CLayerBase
{
public:
			CLayerBase(); // コンストラクタ
	virtual ~CLayerBase(); // デストラクタ

	virtual void Create(CMgrData *pMgrData); // 作成
	virtual void Destroy(void); // 破棄

	virtual void Draw(CImg32 *pDst); // 描画
	virtual BOOL TimerProc(void); // 時間処理

	int GetID(void) { return m_nID; } // IDを取得


protected:
	void TextOut1(CImg32 *pDst, int nFontId, int x, int y, LPCTSTR pStr, COLORREF color); // SDL_ttf版 文字描画
	void TextOut2(CImg32 *pDst, int nFontId, int x, int y, LPCTSTR pStr, COLORREF color, COLORREF colorFrame = RGB(10, 10, 10)); // SDL_ttf版 縁取り文字描画
	void TextOut3(CImg32 *pDst, int nFontId, int x, int y, LPCTSTR pStr, COLORREF color, COLORREF colorFrame = RGB(10, 10, 10)); // SDL_ttf版 縁取り文字描画


protected:
	int m_nID; // レイヤーID
	CImg32 *m_pDib, // バックバッファ
		*m_pDibBase; // バックバッファ(元サイズ)
	CImg32 *m_pDibSystem; // システム画像

	CMgrData *m_pMgrData; // データ管理
	CMgrGrpData *m_pMgrGrpData; // グラフィックデータ管理
	CMgrDraw *m_pMgrDraw; // 描画管理

	CLibInfoCharCli *m_pLibInfoChar; // キャラ情報ライブラリ
	CLibInfoMapBase *m_pLibInfoMap; // マップ情報ライブラリ
} CLayerBase, *PCLayerBase;
using ARRAYLAYERBASE = CStdArray<PCLayerBase>;
using PARRAYLAYERBASE = ARRAYLAYERBASE *;
