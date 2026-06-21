/// @file WindowDELCONFIRM.h
/// @brief 削除確認ダイアログクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/14
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowDELCONFIRM : public CWindowBase
{
public:
			CWindowDELCONFIRM();	// コンストラクタ
	virtual ~CWindowDELCONFIRM();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
	BOOL	TimerProc(void);	// 時間処理
	void	SetCharName(LPCTSTR pszName);	// キャラ名を設定


private:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Z)


private:
	int	m_nPos;	// 選択位置(0=はい, 1=いいえ)
	CString	m_strMsg;	// 表示メッセージ
} CWindowDELCONFIRM, *PCWindowDELCONFIRM;
