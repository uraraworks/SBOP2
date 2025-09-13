/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketSYSTEM_INFO.cpp										 */
/* 内容			:コマンド(システム系:システム情報通知) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/29													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoSystem.h"
#include "PacketSYSTEM_INFO.h"

/* ========================================================================= */
/* 関数名	:CPacketSYSTEM_INFO::CPacketSYSTEM_INFO							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/09/29														 */
/* ========================================================================= */

CPacketSYSTEM_INFO::CPacketSYSTEM_INFO()
{
	m_pInfoSystem = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketSYSTEM_INFO::~CPacketSYSTEM_INFO						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/09/29														 */
/* ========================================================================= */

CPacketSYSTEM_INFO::~CPacketSYSTEM_INFO()
{
	SAFE_DELETE (m_pInfoSystem);
}


/* ========================================================================= */
/* 関数名	:CPacketSYSTEM_INFO::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/09/29														 */
/* ========================================================================= */

void CPacketSYSTEM_INFO::Make(
	CInfoSystem *pInfoSystem)		/* [in] システム情報 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeInfoSystem;
	PBYTE pDataInfoSystem;
	PPACKETBASE pPacketBase;

	dwSizeInfoSystem = pInfoSystem->GetSendDataSize ();
	pDataInfoSystem	 = pInfoSystem->GetSendData ();

	dwSize = sizeof (PACKETBASE);
	dwSize += dwSizeInfoSystem;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_SYSTEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_SYSTEM_INFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pDataInfoSystem, dwSizeInfoSystem, pDataTmp);	/* システム情報 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketSYSTEM_INFO::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/09/29														 */
/* ========================================================================= */

PBYTE CPacketSYSTEM_INFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	SAFE_DELETE (m_pInfoSystem);
	m_pInfoSystem = new CInfoSystem;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);
	pDataTmp	= m_pInfoSystem->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
