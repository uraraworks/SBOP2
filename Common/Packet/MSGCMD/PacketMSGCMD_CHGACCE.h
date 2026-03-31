/// @file PacketMSGCMD_CHGACCE.h
/// @brief コマンド(メッセージコマンド系:アクセサリ変更) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/04
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketMSGCMD_CHGACCE : public CPacketBase
{
public:
			CPacketMSGCMD_CHGACCE();	// コンストラクタ
	virtual ~CPacketMSGCMD_CHGACCE();	// デストラクタ

	void	Make(DWORD dwCharID, int nType);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID;	// キャラID
	int	m_nType;	// 種別
} CPacketMSGCMD_CHGACCE, *PCPacketMSGCMD_CHGACCE;
