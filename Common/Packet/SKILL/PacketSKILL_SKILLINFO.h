/// @file PacketSKILL_SKILLINFO.h
/// @brief コマンド(スキル系:スキル情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/06
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"
#include "LibInfoSkill.h"

// クラス宣言

typedef class CPacketSKILL_SKILLINFO : public CPacketBase
{
public:
			CPacketSKILL_SKILLINFO();	// コンストラクタ
	virtual ~CPacketSKILL_SKILLINFO();	// デストラクタ

	void	Make(PCLibInfoSkill pInfo);	// パケットを作成
	void	Make(PCInfoSkillBase pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	PCLibInfoSkill	m_pLibInfo;	// スキル情報
} CPacketSKILL_SKILLINFO, *PCPacketSKILL_SKILLINFO;
