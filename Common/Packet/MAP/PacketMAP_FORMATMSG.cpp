/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_FORMATMSG.cpp									 */
/* 内容			:コマンド(マップ系:フォーマットメッセージ通知) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_FORMATMSG.h"

/* ========================================================================= */
/* 関数名	:CPacketMAP_FORMATMSG::CPacketMAP_FORMATMSG						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/03														 */
/* ========================================================================= */

CPacketMAP_FORMATMSG::CPacketMAP_FORMATMSG()
{
	m_nMsgType	= FORMATMSGTYPE_DEFAULT;
	m_bSound	= TRUE;
	m_dwMsgID	= 0;
	m_dwPara1	= 0;
	m_dwPara2	= 0;
	m_clMsg		= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_FORMATMSG::~CPacketMAP_FORMATMSG					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/03														 */
/* ========================================================================= */

CPacketMAP_FORMATMSG::~CPacketMAP_FORMATMSG()
{
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_FORMATMSG::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/07/03														 */
/* ========================================================================= */

void CPacketMAP_FORMATMSG::Make(
	DWORD dwMsgID,				/* [in] メッセージID */
	DWORD dwPara1,	/*=0*/		/* [in] パラメータ1 */
	DWORD dwPara2,	/*=0*/		/* [in] パラメータ2 */
	COLORREF clMsg,	/*=0*/		/* [in] 表示色 */
	BOOL bSound,	/*=TRUE*/	/* [in] 表示する時に音をならすか判定 */
	int nMsgType)	/*=FORMATMSGTYPE_DEFAULT*/	/* [in] メッセージ種別 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMsgID)		+
			 sizeof (dwPara1)		+
			 sizeof (dwPara2)		+
			 sizeof (clMsg)			+
			 sizeof (bSound)		+
			 sizeof (m_nMsgType);

	if (clMsg == 0) {
		clMsg = RGB (0, 200, 255);
	}

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_FORMATMSG;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMsgID,	sizeof (dwMsgID),	pDataTmp);	/* メッセージID */
	CopyMemoryRenew (pDataTmp, &dwPara1,	sizeof (dwPara1),	pDataTmp);	/* パラメータ1 */
	CopyMemoryRenew (pDataTmp, &dwPara2,	sizeof (dwPara2),	pDataTmp);	/* パラメータ2 */
	CopyMemoryRenew (pDataTmp, &clMsg,		sizeof (clMsg),		pDataTmp);	/* 表示色 */
	CopyMemoryRenew (pDataTmp, &bSound,		sizeof (bSound),	pDataTmp);	/* 表示する時に音をならすか判定 */
	CopyMemoryRenew (pDataTmp, &nMsgType,	sizeof (nMsgType),	pDataTmp);	/* メッセージ種別 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_FORMATMSG::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/07/03														 */
/* ========================================================================= */

PBYTE CPacketMAP_FORMATMSG::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMsgID,	pDataTmp, sizeof (m_dwMsgID),	pDataTmp);	/* メッセージID */
	CopyMemoryRenew (&m_dwPara1,	pDataTmp, sizeof (m_dwPara1),	pDataTmp);	/* パラメータ1 */
	CopyMemoryRenew (&m_dwPara2,	pDataTmp, sizeof (m_dwPara2),	pDataTmp);	/* パラメータ2 */
	CopyMemoryRenew (&m_clMsg,		pDataTmp, sizeof (m_clMsg),		pDataTmp);	/* 表示色 */
	CopyMemoryRenew (&m_bSound,		pDataTmp, sizeof (m_bSound),	pDataTmp);	/* 表示する時に音をならすか判定 */
	CopyMemoryRenew (&m_nMsgType,	pDataTmp, sizeof (m_nMsgType),	pDataTmp);	/* メッセージ種別 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
