/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_RENEWSTATUS.cpp							 */
/* 内容			:コマンド(管理者系:ステータス情報更新) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHAR_RENEWSTATUS.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_RENEWSTATUS::CPacketADMIN_CHAR_RENEWSTATUS	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEWSTATUS::CPacketADMIN_CHAR_RENEWSTATUS()
{
	m_byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	m_byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWSTATUS;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_RENEWSTATUS::~CPacketADMIN_CHAR_RENEWSTATUS	 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEWSTATUS::~CPacketADMIN_CHAR_RENEWSTATUS()
{
}

/* Copyright(C)URARA-works 2008 */
