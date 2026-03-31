/// @file PacketADMIN_CHAR_ADDMOTIONTYPE.h
/// @brief コマンド(管理者系:キャラモーション種別情報の追加) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/09
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_CHAR_ADDMOTIONTYPE : public CPacketBase
{
public:
			CPacketADMIN_CHAR_ADDMOTIONTYPE();	// コンストラクタ
	virtual ~CPacketADMIN_CHAR_ADDMOTIONTYPE();	// デストラクタ

	void	Make(void);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
} CPacketADMIN_CHAR_ADDMOTIONTYPE, *PCPacketADMIN_CHAR_ADDMOTIONTYPE;
