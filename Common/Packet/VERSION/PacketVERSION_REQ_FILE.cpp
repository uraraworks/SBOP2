/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketVERSION_REQ_FILE.cpp									 */
/* 内容			:コマンド(バージョン系:ファイル要求) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/01/20													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_FILE.h"

/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILE::CPacketVERSION_REQ_FILE				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/01/20														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILE::CPacketVERSION_REQ_FILE()
{
	m_dwOffset	= 0;
	m_dwReqSize	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILE::~CPacketVERSION_REQ_FILE				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/01/20														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILE::~CPacketVERSION_REQ_FILE()
{
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILE::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/01/20														 */
/* ========================================================================= */

void CPacketVERSION_REQ_FILE::Make(
	DWORD dwOffset,			/* [in] 開始オフセット */
	DWORD dwReqSize,		/* [in] 要求サイズ */
	LPCSTR pszFileName)		/* [in] バージョンコード */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwOffset)		+
			 sizeof (dwReqSize)		+
			 (strlen (pszFileName) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_FILE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwOffset,	sizeof (dwOffset),	pDataTmp);	/* 開始オフセット */
	CopyMemoryRenew (pDataTmp, &dwReqSize,	sizeof (dwReqSize),	pDataTmp);	/* 要求サイズ */
	strcpyRenew ((LPSTR)pDataTmp, pszFileName, pDataTmp);					/* 相対パス付きファイル名 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketVERSION_REQ_FILE::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/01/20														 */
/* ========================================================================= */

PBYTE CPacketVERSION_REQ_FILE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwOffset,	pDataTmp, sizeof (m_dwOffset),	pDataTmp);	/* 開始オフセット */
	CopyMemoryRenew (&m_dwReqSize,	pDataTmp, sizeof (m_dwReqSize),	pDataTmp);	/* 要求サイズ */
	StoreRenew (m_strFileName,	(LPCSTR)pDataTmp, pDataTmp);					/* 相対パス付きファイル名 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
