/// @file PacketADMIN_CHAR_ADDNPC.h
/// @brief コマンド(管理者系:NPCの追加) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/01
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

// クラス宣言

typedef class CPacketADMIN_CHAR_ADDNPC : public CPacketBase
{
public:
			CPacketADMIN_CHAR_ADDNPC();	// コンストラクタ
	virtual ~CPacketADMIN_CHAR_ADDNPC();	// デストラクタ

	void	Make(CInfoCharBase *pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CInfoCharBase	*m_pInfoChar;	// キャラ情報
} CPacketADMIN_CHAR_ADDNPC, *PCPacketADMIN_CHAR_ADDNPC;
