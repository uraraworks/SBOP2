/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_SERVER_SAVEINFO.cpp							 */
/* 内容			:コマンド(管理者系:サーバー情報保存) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_SERVER_SAVEINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_SERVER_SAVEINFO::CPacketADMIN_SERVER_SAVEINFO		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/08														 */
/* ========================================================================= */

CPacketADMIN_SERVER_SAVEINFO::CPacketADMIN_SERVER_SAVEINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_SERVER_SAVEINFO::~CPacketADMIN_SERVER_SAVEINFO	 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/08														 */
/* ========================================================================= */

CPacketADMIN_SERVER_SAVEINFO::~CPacketADMIN_SERVER_SAVEINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_SERVER_SAVEINFO::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/07/08														 */
/* ========================================================================= */

void CPacketADMIN_SERVER_SAVEINFO::Make(void)
{
	PBYTE pData;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_SERVER_SAVEINFO;

	RenewPacket (pData, dwSize);
}

/* Copyright(C)URARA-works 2007 */
