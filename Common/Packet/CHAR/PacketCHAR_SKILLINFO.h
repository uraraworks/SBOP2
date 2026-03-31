/// @file PacketCHAR_SKILLINFO.h
/// @brief コマンド(キャラ系:スキル情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/31
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCHAR_SKILLINFO : public CPacketBase
{
public:
			CPacketCHAR_SKILLINFO();	// コンストラクタ
	virtual ~CPacketCHAR_SKILLINFO();	// デストラクタ

	void	Make(DWORD dwCharID, PARRAYDWORD paSrc);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID;	// キャラID
	ARRAYDWORD	m_adwSkillID;	// スキルID
} CPacketCHAR_SKILLINFO, *PCPacketCHAR_SKILLINFO;
