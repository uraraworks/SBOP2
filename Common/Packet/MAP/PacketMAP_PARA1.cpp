/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_PARA1.cpp										 */
/* 内容			:コマンド(マップ系:汎用[パラメータ1個]) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/28													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_PARA1.h"

/* ========================================================================= */
/* 関数名	:CPacketMAP_PARA1::CPacketMAP_PARA1								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

CPacketMAP_PARA1::CPacketMAP_PARA1()
{
	m_dwCharID	= 0;
	m_dwPara	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_PARA1::~CPacketMAP_PARA1							 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

CPacketMAP_PARA1::~CPacketMAP_PARA1()
{
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_PARA1::Make											 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

void CPacketMAP_PARA1::Make(
	BYTE byCmdSub,		/* [in] コマンドIDサブ */
	DWORD dwCharID,		/* [in] キャラID */
	DWORD dwPara)		/* [in] パラメータ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwPara);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= byCmdSub;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwPara,		sizeof (dwPara),	pDataTmp);	/* パラメータ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_PARA1::Set											 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

PBYTE CPacketMAP_PARA1::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwPara,		pDataTmp, sizeof (m_dwPara),	pDataTmp);	/* パラメータ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
