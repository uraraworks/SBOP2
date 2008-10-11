/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_SYSTEMMSG.cpp									 */
/* 内容			:コマンド(マップ系:システムメッセージ通知) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/02/10													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_SYSTEMMSG.h"

/* ========================================================================= */
/* 関数名	:CPacketMAP_SYSTEMMSG::CPacketMAP_SYSTEMMSG						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/02/10														 */
/* ========================================================================= */

CPacketMAP_SYSTEMMSG::CPacketMAP_SYSTEMMSG()
{
	m_nMsgType	= SYSTEMMSGTYPE_DEFAULT;
	m_bSound	= TRUE;
	m_clMsg		= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_SYSTEMMSG::~CPacketMAP_SYSTEMMSG					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/02/10														 */
/* ========================================================================= */

CPacketMAP_SYSTEMMSG::~CPacketMAP_SYSTEMMSG()
{
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_SYSTEMMSG::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/02/10														 */
/* ========================================================================= */

void CPacketMAP_SYSTEMMSG::Make(
	LPCSTR pszMsg,								/* [in] システムメッセージ */
	COLORREF clMsg,	/*=0*/						/* [in] 表示色 */
	BOOL bSound,	/*=TRUE*/					/* [in] 表示する時に音をならすか判定 */
	int nMsgType)	/*=SYSTEMMSGTYPE_DEFAULT*/	/* [in] メッセージ種別 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (clMsg)			+
			 sizeof (bSound)		+
			 sizeof (m_nMsgType)	+
			 (strlen (pszMsg) + 1);

	if (clMsg == 0) {
		clMsg = RGB (0, 200, 255);
	}

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_SYSTEMMSG;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strcpyRenew ((LPSTR)pDataTmp, pszMsg, pDataTmp);						/* システムメッセージ */
	CopyMemoryRenew (pDataTmp, &clMsg,		sizeof (clMsg),		pDataTmp);	/* 表示色 */
	CopyMemoryRenew (pDataTmp, &bSound,		sizeof (bSound),	pDataTmp);	/* 表示する時に音をならすか判定 */
	CopyMemoryRenew (pDataTmp, &nMsgType,	sizeof (nMsgType),	pDataTmp);	/* メッセージ種別 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_SYSTEMMSG::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/02/10														 */
/* ========================================================================= */

PBYTE CPacketMAP_SYSTEMMSG::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	StoreRenew (m_strMsg, (LPCSTR)pDataTmp, pDataTmp);							/* システムメッセージ */
	CopyMemoryRenew (&m_clMsg,		pDataTmp, sizeof (m_clMsg),		pDataTmp);	/* 表示色 */
	CopyMemoryRenew (&m_bSound,		pDataTmp, sizeof (m_bSound),	pDataTmp);	/* 表示する時に音をならすか判定 */
	CopyMemoryRenew (&m_nMsgType,	pDataTmp, sizeof (m_nMsgType),	pDataTmp);	/* メッセージ種別 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
