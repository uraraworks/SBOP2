/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_REQ_VERSIONCHECK.cpp							 */
/* 内容			:コマンド(バージョン系:バージョンチェック要求) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_VERSIONCHECK.h"

/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_VERSIONCHECK::CPacketVERSION_REQ_VERSIONCHECK */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CPacketVERSION_REQ_VERSIONCHECK::CPacketVERSION_REQ_VERSIONCHECK()
{
	m_dwVersion = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_VERSIONCHECK::~CPacketVERSION_REQ_VERSIONCHECK */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CPacketVERSION_REQ_VERSIONCHECK::~CPacketVERSION_REQ_VERSIONCHECK()
{
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_VERSIONCHECK::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CPacketVERSION_REQ_VERSIONCHECK::Make(
	DWORD dwVersion)		/* [in] バージョンコード */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwVersion);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_VERSIONCHECK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwVersion, sizeof (dwVersion), pDataTmp);	/* バージョンコード */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_VERSIONCHECK::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

PBYTE CPacketVERSION_REQ_VERSIONCHECK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwVersion, pDataTmp, sizeof (m_dwVersion), pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
