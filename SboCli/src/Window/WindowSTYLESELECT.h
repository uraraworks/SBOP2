/// @file WindowSTYLESELECT.h
/// @brief 容姿選択ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/10
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;
class CMgrWindow;
class CMgrDraw;
class CWindowSEX;
class CWindowHAIRTYPE;
class CWindowHAIRCOLOR;
class CWindowEYECOLOR;
class CWindowCHARNAME;
class CInfoCharCli;

typedef class CWindowSTYLESELECT : public CWindowBase
{
public:
			CWindowSTYLESELECT();	// コンストラクタ
	virtual ~CWindowSTYLESELECT();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	SetFamilyType(WORD wFamilyID);	// 種族を設定
	void	SetInfoChar(CInfoCharCli *pInfoCharCli);	// キャラ情報を設定
	void	OnWindowMsg(int nType, DWORD dwPara);	// メッセージハンドラ(WM_WINDOWMSG)
	void	Draw(CImg32 *pDst);	// 描画
	BOOL	TimerProc(void);	// 時間処理
	BOOL	HandleSDLKeyDown(UINT vk);	// SDLキー押下を処理(名前欄フォーカス時はCHARNAMEへ転送)
	void	HandleSDLTextInput(LPCSTR pszText);	// SDLテキスト入力を処理(名前欄フォーカス時はCHARNAMEへ転送)
	void	HandleSDLTextEditing(LPCSTR pszText);	// SDLテキスト編集中を処理(名前欄フォーカス時はCHARNAMEへ転送)


private:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Y)
	BOOL	TryRegist(void);	// 名前チェックの上で登録を試みる(名前欄Enter確定/登録項目Xから共通利用)


public:
	CInfoCharCli	*m_pInfoCharCli;	// 作成中のキャラ情報


private:
	BOOL	m_bAnime;	// アニメーションする判定
	int	m_nSex,	// 性別
				m_nSexBack,	// 性別保存用
				m_nDirection;	// 向き番号
	WORD	m_wFamilyID,	// 種族ID
				m_wFamilyIDBack,	// 種族ID保存用
				m_wHairTypeID,	// 髪型ID
				m_wHairTypeIDBack,	// 髪型ID保存用
				m_wHairColorID,	// 髪色ID
				m_wHairColorIDBack,	// 髪色ID保存用
				m_wEyeColorID,	// 目色ID
				m_wEyeColorIDBack;	// 目色ID保存用
	CMgrWindow	*m_pMgrWindow;	// ウィンドウマネージャ
	CMgrDraw	*m_pMgrDraw;	// 描画マネージャ
	CWindowSEX	*m_pWindowSEX;	// 性別入力ウィンドウ
	CWindowHAIRTYPE	*m_pWindowHAIRTYPE;	// 髪型選択ウィンドウ
	CWindowHAIRCOLOR	*m_pWindowHAIRCOLOR;	// 髪色選択ウィンドウ
	CWindowEYECOLOR	*m_pWindowEYECOLOR;	// 目色選択ウィンドウ
	CWindowCHARNAME	*m_pWindowCHARNAME;	// キャラ名入力ウィンドウ
	CmyString	m_strNameDrawn;	// 前回描画時の子(CHARNAME)の確定文字列(TimerProcでの再描画判定用)
	CmyString	m_strCompositionDrawn;	// 前回描画時の子(CHARNAME)のIME変換中文字列(TimerProcでの再描画判定用)
} CWindowSTYLESELECT, *PCWindowSTYLESELECT;
