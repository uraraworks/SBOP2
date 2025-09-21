/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_ADDMOTIONTYPE.cpp							 */
/* 内容			:コマンド(管理者系:キャラモーション種別情報の追加) 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/09													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_CHAR_ADDMOTIONTYPE.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_ADDMOTIONTYPE::CPacketADMIN_CHAR_ADDMOTIONTYPE */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

CPacketADMIN_CHAR_ADDMOTIONTYPE::CPacketADMIN_CHAR_ADDMOTIONTYPE()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_ADDMOTIONTYPE::~CPacketADMIN_CHAR_ADDMOTIONTYPE */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

CPacketADMIN_CHAR_ADDMOTIONTYPE::~CPacketADMIN_CHAR_ADDMOTIONTYPE()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_ADDMOTIONTYPE::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_ADDMOTIONTYPE::Make(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_ADDMOTIONTYPE;

	pDataTmp = (PBYTE)(pPacketBase + 1);

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_ADDMOTIONTYPE::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_ADDMOTIONTYPE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
