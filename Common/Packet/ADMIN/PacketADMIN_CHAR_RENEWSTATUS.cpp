/// @file PacketADMIN_CHAR_RENEWSTATUS.cpp
/// @brief コマンド(管理者系:ステータス情報更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/12
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHAR_RENEWSTATUS.h"

CPacketADMIN_CHAR_RENEWSTATUS::CPacketADMIN_CHAR_RENEWSTATUS()
{
	m_byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	m_byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWSTATUS;
}

CPacketADMIN_CHAR_RENEWSTATUS::~CPacketADMIN_CHAR_RENEWSTATUS()
{
}
