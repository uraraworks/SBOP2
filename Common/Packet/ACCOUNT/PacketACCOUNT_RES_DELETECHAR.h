/// @file PacketACCOUNT_RES_DELETECHAR.h
/// @brief コマンド(アカウント系:キャラ削除応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/14
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

// 定数の定義

enum {
	DELETECHARRES_NONE = 0,
	DELETECHARRES_OK,	// 問題無し
	DELETECHARRES_NG,	// 失敗
};

// クラス宣言

typedef class CPacketACCOUNT_RES_DELETECHAR : public CPacketBase
{
public:
			CPacketACCOUNT_RES_DELETECHAR();	// コンストラクタ
	virtual ~CPacketACCOUNT_RES_DELETECHAR();	// デストラクタ

	void	Make(int nResult, DWORD dwCharID);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	int	m_nResult;	// 結果
	DWORD	m_dwCharID;	// キャラID
} CPacketACCOUNT_RES_DELETECHAR, *PCPacketACCOUNT_RES_DELETECHAR;
