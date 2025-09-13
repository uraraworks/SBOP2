/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_TEXTEFFECT.cpp									 */
/* 内容			:コマンド(キャラ系:文字エフェクト通知) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/13													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_TEXTEFFECT.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_TEXTEFFECT::CPacketCHAR_TEXTEFFECT					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

CPacketCHAR_TEXTEFFECT::CPacketCHAR_TEXTEFFECT()
{
	m_nColor	= 0;
	m_dwCharID	= 0;
	m_dwData	= -1;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_TEXTEFFECT::~CPacketCHAR_TEXTEFFECT				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

CPacketCHAR_TEXTEFFECT::~CPacketCHAR_TEXTEFFECT()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_TEXTEFFECT::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

void CPacketCHAR_TEXTEFFECT::Make(
	DWORD dwCharID,			/* [in] キャラID */
	DWORD dwData,			/* [in] 数値データ */
	int nColor,/*0*/		/* [in] 色 */
	int nSize)/*0*/			/* [in] サイズ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwData)		+
			 sizeof (nColor)		+
			 sizeof (nSize)			+
			 1;	/* 文字列終端用 */

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_TEXTEFFECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwData,		sizeof (dwData),	pDataTmp);	/* 数値データ */
	CopyMemoryRenew (pDataTmp, &nColor,		sizeof (nColor),	pDataTmp);	/* 色 */
	CopyMemoryRenew (pDataTmp, &nSize,		sizeof (nSize),		pDataTmp);	/* サイズ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_TEXTEFFECT::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

void CPacketCHAR_TEXTEFFECT::Make(
	DWORD dwCharID,			/* [in] キャラID */
	LPCSTR pszData,			/* [in] 文字列データ */
	int nColor,/*0*/		/* [in] 色 */
	int nSize)/*0*/			/* [in] サイズ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwTmp;
	PPACKETBASE pPacketBase;

	dwTmp  = -1;
	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (DWORD)			+	/* 数値データ用 */
			 sizeof (nColor)		+
			 sizeof (nSize)			+
			 (strlen (pszData) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_TEXTEFFECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwTmp,		sizeof (dwTmp),		pDataTmp);	/* 数値データ */
	CopyMemoryRenew (pDataTmp, &nColor,		sizeof (nColor),	pDataTmp);	/* 色 */
	CopyMemoryRenew (pDataTmp, &nSize,		sizeof (nSize),		pDataTmp);	/* サイズ */
	strcpyRenew ((LPSTR)pDataTmp, pszData, pDataTmp);						/* 文字列データ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_TEXTEFFECT::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

PBYTE CPacketCHAR_TEXTEFFECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwData,		pDataTmp, sizeof (m_dwData),	pDataTmp);	/* 数値データ */
	CopyMemoryRenew (&m_nColor,		pDataTmp, sizeof (m_nColor),	pDataTmp);	/* 色 */
	CopyMemoryRenew (&m_nSize,		pDataTmp, sizeof (m_nSize),		pDataTmp);	/* サイズ */
	StoreRenew (m_strData, (LPCSTR)pDataTmp, pDataTmp);							/* 文字列データ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
