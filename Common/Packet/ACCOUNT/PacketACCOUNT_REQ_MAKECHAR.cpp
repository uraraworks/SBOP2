/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketACCOUNT_REQ_MAKECHAR.cpp								 */
/* 内容			:コマンド(アカウント系:キャラ作成要求) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/14													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoCharBase.h"
#include "PacketACCOUNT_REQ_MAKECHAR.h"

/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_REQ_MAKECHAR::CPacketACCOUNT_REQ_MAKECHAR		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/14														 */
/* ========================================================================= */

CPacketACCOUNT_REQ_MAKECHAR::CPacketACCOUNT_REQ_MAKECHAR()
{
	m_pInfoCharBase = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_REQ_MAKECHAR::~CPacketACCOUNT_REQ_MAKECHAR		 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/14														 */
/* ========================================================================= */

CPacketACCOUNT_REQ_MAKECHAR::~CPacketACCOUNT_REQ_MAKECHAR()
{
	SAFE_DELETE (m_pInfoCharBase);
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_REQ_MAKECHAR::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/11/14														 */
/* ========================================================================= */

void CPacketACCOUNT_REQ_MAKECHAR::Make(
	DWORD dwAccountID,					/* [in] アカウントID */
	CInfoCharBase *pInfoCharBase)		/* [in] キャラ情報 */
{
	PBYTE pData, pDataTmp, pCharData;
	DWORD dwSize, dwCharDataSize;
	PPACKETBASE pPacketBase;

	dwCharDataSize	= pInfoCharBase->GetSendDataSize ();
	pCharData		= pInfoCharBase->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwAccountID)	+
			 dwCharDataSize;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_REQ_MAKECHAR;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	/* アカウントID */
	CopyMemoryRenew (pDataTmp, pCharData, dwCharDataSize, pDataTmp);			/* キャラ情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pCharData);
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_REQ_MAKECHAR::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/11/14														 */
/* ========================================================================= */

PBYTE CPacketACCOUNT_REQ_MAKECHAR::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	SAFE_DELETE (m_pInfoCharBase);
	m_pInfoCharBase = new CInfoCharBase;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* アカウントID */
	pDataTmp = m_pInfoCharBase->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
