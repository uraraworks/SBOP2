/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_REQ_FILELIST.cpp								 */
/* 内容			:コマンド(バージョン系:ファイルリスト要求) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/03/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_FILELIST.h"

/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILELIST::CPacketVERSION_REQ_FILELIST		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILELIST::CPacketVERSION_REQ_FILELIST()
{
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILELIST::~CPacketVERSION_REQ_FILELIST		 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILELIST::~CPacketVERSION_REQ_FILELIST()
{
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILELIST::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/03/08														 */
/* ========================================================================= */

void CPacketVERSION_REQ_FILELIST::Make(void)
{
	PBYTE pData;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_FILELIST;

	RenewPacket (pData, dwSize);
}

/* Copyright(C)URARA-works 2008 */
