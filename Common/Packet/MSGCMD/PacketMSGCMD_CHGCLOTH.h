/// @file PacketMSGCMD_CHGCLOTH.h
/// @brief コマンド(メッセージコマンド系:服装変更) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/02
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketMSGCMD_CHGCLOTH : public CPacketBase
{
public:
			CPacketMSGCMD_CHGCLOTH();	// コンストラクタ
	virtual ~CPacketMSGCMD_CHGCLOTH();	// デストラクタ

	void	Make(DWORD dwCharID, int nType);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID;	// キャラID
	int	m_nType;	// 種別
} CPacketMSGCMD_CHGCLOTH, *PCPacketMSGCMD_CHGCLOTH;
