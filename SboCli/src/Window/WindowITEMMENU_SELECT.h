/// @file WindowITEMMENU_SELECT.h
/// @brief アイテムどうするかメニューウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/13
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CInfoItem;
class CLibInfoItem;

enum {
	ITEMMENU_SELECT_COMMAND_NONE = 0,	
	ITEMMENU_SELECT_COMMAND_PUT,	// 地面に置く
	ITEMMENU_SELECT_COMMAND_EQUIP,	// 装備する
	ITEMMENU_SELECT_COMMAND_EQUIP_UNSET,	// 装備を外す
	ITEMMENU_SELECT_COMMAND_USE,	// 使う
	ITEMMENU_SELECT_COMMAND_MAX
};


typedef class CWindowITEMMENU_SELECT : public CWindowBase
{
public:
			CWindowITEMMENU_SELECT();	// コンストラクタ
	virtual ~CWindowITEMMENU_SELECT();	// デストラクタ

	void	Create(CMgrData *pMgrData);	// 作成
	void	Draw(CImg32 *pDst);	// 描画
	void	SetItemID(int nType, DWORD dwItemID);	// アイテムIDを指定
	int	GetType(void)	{ return m_nType; }	// アイテムの種類を取得


protected:
	BOOL	OnUp(void);	// キーハンドラ(↑)
	BOOL	OnDown(void);	// キーハンドラ(↓)
	BOOL	OnLeft(void);	// キーハンドラ(←)
	BOOL	OnRight(void);	// キーハンドラ(→)
	BOOL	OnX(BOOL bDown);	// キーハンドラ(X)
	BOOL	OnZ(BOOL bDown);	// キーハンドラ(Z)


protected:
	int	m_nType;	// アイテムの種類
	CInfoItem	*m_pInfoItem;	// 処理中のアイテム情報
	CLibInfoItem	*m_pLibInfoItem;	// アイテム情報ライブラリ
	ARRAYINT	m_anCommand;	// コマンド種別
} CWindowITEMMENU_SELECT, *PCWindowITEMMENU_SELECT;
