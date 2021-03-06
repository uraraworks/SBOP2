/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_SETMAPNAME.cpp								 */
/* 内容			:コマンド(管理者系:キャラ情報通知) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_MAP_SETMAPNAME.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_SETMAPNAME::CPacketADMIN_MAP_SETMAPNAME		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/05/24														 */
/* ========================================================================= */

CPacketADMIN_MAP_SETMAPNAME::CPacketADMIN_MAP_SETMAPNAME()
{
	m_dwMapID		= 0;			/* マップID */
	m_dwBGMID		= 0;			/* BGMID */
	m_dwWeatherType	= 0;			/* 天気種別 */
	m_bEnableBattle	= TRUE;			/* 戦闘許可 */
	m_bRecovery		= TRUE;			/* 気絶後回復する */
	m_byLevel		= 0;			/* 明るさレベル */
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_SETMAPNAME::~CPacketADMIN_MAP_SETMAPNAME		 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/05/24														 */
/* ========================================================================= */

CPacketADMIN_MAP_SETMAPNAME::~CPacketADMIN_MAP_SETMAPNAME()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_SETMAPNAME::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/05/24														 */
/* ========================================================================= */

void CPacketADMIN_MAP_SETMAPNAME::Make(
	DWORD dwMapID,			/* [in] マップID */
	DWORD dwBGMID,			/* [in] BGMID */
	DWORD dwWeatherType,	/* [in] 天気種別 */
	BOOL bEnableBattle,		/* [in] 戦闘許可 */
	BOOL bRecovery,			/* [in] 気絶後回復する */
	BYTE byLevel,			/* [in] 明るさレベル */
	LPCSTR pszMapName)		/* [in] マップ名 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 sizeof (dwBGMID)		+
			 sizeof (dwWeatherType)	+
			 sizeof (bEnableBattle)	+
			 sizeof (bRecovery)		+
			 sizeof (byLevel)		+
			 (strlen (pszMapName) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_SETMAPNAME;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,		sizeof (dwMapID),		pDataTmp);	/* マップID */
	CopyMemoryRenew (pDataTmp, &dwBGMID,		sizeof (dwBGMID),		pDataTmp);	/* BGMID */
	CopyMemoryRenew (pDataTmp, &dwWeatherType,	sizeof (dwWeatherType),	pDataTmp);	/* 天気種別 */
	CopyMemoryRenew (pDataTmp, &bEnableBattle,	sizeof (bEnableBattle),	pDataTmp);	/* 戦闘許可 */
	CopyMemoryRenew (pDataTmp, &bRecovery,		sizeof (bRecovery),		pDataTmp);	/* 気絶後回復する */
	CopyMemoryRenew (pDataTmp, &byLevel,		sizeof (byLevel),		pDataTmp);	/* 明るさレベル */
	strcpyRenew ((LPSTR)pDataTmp, pszMapName, pDataTmp);				/* マップ名 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_SETMAPNAME::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/05/24														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_SETMAPNAME::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,		pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* マップID */
	CopyMemoryRenew (&m_dwBGMID,		pDataTmp, sizeof (m_dwBGMID),		pDataTmp);	/* BGMID */
	CopyMemoryRenew (&m_dwWeatherType,	pDataTmp, sizeof (m_dwWeatherType),	pDataTmp);	/* 天気種別 */
	CopyMemoryRenew (&m_bEnableBattle,	pDataTmp, sizeof (m_bEnableBattle),	pDataTmp);	/* 戦闘許可 */
	CopyMemoryRenew (&m_bRecovery,		pDataTmp, sizeof (m_bRecovery),		pDataTmp);	/* 気絶後回復する */
	CopyMemoryRenew (&m_byLevel,		pDataTmp, sizeof (m_byLevel),		pDataTmp);	/* 明るさレベル */
	StoreRenew (m_strMapName, (LPCSTR)pDataTmp, pDataTmp);								/* マップ名 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
