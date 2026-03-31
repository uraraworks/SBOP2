/// @file PacketACCOUNT_RES_MAKECHAR.h
/// @brief コマンド(アカウント系:キャラ作成応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/12/03
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

// 定数の定義

enum {
	MAKECHARRES_NONE = 0,
	MAKECHARRES_OK,	// 問題無し
	MAKECHARRES_NG_USE,	// 使用済み
	MAKECHARRES_NG_SPACE,	// 空白が使用されている
};

// クラス宣言

typedef class CPacketACCOUNT_RES_MAKECHAR : public CPacketBase
{
public:
			CPacketACCOUNT_RES_MAKECHAR();	// コンストラクタ
	virtual ~CPacketACCOUNT_RES_MAKECHAR();	// デストラクタ

	void	Make(int nResult, DWORD dwCharID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	int	m_nResult;	// 結果
	DWORD	m_dwCharID;	// キャラID
} CPacketACCOUNT_RES_MAKECHAR, *PCPacketACCOUNT_RES_MAKECHAR;
