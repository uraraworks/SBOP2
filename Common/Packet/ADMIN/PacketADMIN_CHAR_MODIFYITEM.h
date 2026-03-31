/// @file PacketADMIN_CHAR_MODIFYITEM.h
/// @brief コマンド(管理者系:所持アイテムの変更) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/24
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

// 定数定義

enum {
	CHARMODIFYITEMTYPE_NONE = 0,
	CHARMODIFYITEMTYPE_ADD,	// 追加
	CHARMODIFYITEMTYPE_DELETE,	// 削除
};

// クラス宣言

typedef class CPacketADMIN_CHAR_MODIFYITEM : public CPacketBase
{
public:
			CPacketADMIN_CHAR_MODIFYITEM();	// コンストラクタ
	virtual ~CPacketADMIN_CHAR_MODIFYITEM();	// デストラクタ

	void	Make(DWORD dwCharID, DWORD dwItemID, int nType);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID,	// キャラID
				m_dwItemID;	// アイテムID
	int	m_nType;	// 種別
} CPacketADMIN_CHAR_MODIFYITEM, *PCPacketADMIN_CHAR_MODIFYITEM;
