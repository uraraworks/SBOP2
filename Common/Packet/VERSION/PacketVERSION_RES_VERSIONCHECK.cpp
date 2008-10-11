/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_RES_VERSIONCHECK.cpp							 */
/* 内容			:コマンド(バージョン系:バージョンチェック応答) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_RES_VERSIONCHECK.h"

/* ========================================================================= */
/* 関数名	:CPacketVERSION_RES_VERSIONCHECK::CPacketVERSION_RES_VERSIONCHECK */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CPacketVERSION_RES_VERSIONCHECK::CPacketVERSION_RES_VERSIONCHECK()
{
	m_nResult = VERSIONCHECKRES_NONE;
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_RES_VERSIONCHECK::~CPacketVERSION_RES_VERSIONCHECK */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CPacketVERSION_RES_VERSIONCHECK::~CPacketVERSION_RES_VERSIONCHECK()
{
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_RES_VERSIONCHECK::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CPacketVERSION_RES_VERSIONCHECK::Make(
	int nResult)		/* [in] 結果 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (nResult);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_RES_VERSIONCHECK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nResult, sizeof (nResult), pDataTmp);		/* 結果 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_RES_VERSIONCHECK::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

PBYTE CPacketVERSION_RES_VERSIONCHECK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nResult, pDataTmp, sizeof (m_nResult), pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
