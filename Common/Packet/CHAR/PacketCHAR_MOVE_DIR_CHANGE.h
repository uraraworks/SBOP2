/* Copyright(C)URARA-works 2026 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_MOVE_DIR_CHANGE.h					 */
/* 内容			:コマンド(キャラ系:移動方向変更通知) 定義ファイル		 */
/* 作成開始日	:2026/02/28								 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"
#include "Command.h"

typedef class CPacketCHAR_MOVE_DIR_CHANGE : public CPacketBase
{
public:
	CPacketCHAR_MOVE_DIR_CHANGE()
	{
		m_bUpdate = FALSE;
		m_dwMapID = 0;
		m_dwCharID = 0;
		m_nDirection = 0;
		m_pos.x = m_pos.y = 0;
		m_nSpeedLevel = 1;
		m_dwTimeStamp = 0;
	}
	virtual ~CPacketCHAR_MOVE_DIR_CHANGE() {}

	void Make(DWORD dwMapID, DWORD dwCharID, int nDirection, int x, int y, BOOL bUpdate, int nSpeedLevel, DWORD dwTimeStamp)
	{
		PBYTE pData, pDataTmp;
		DWORD dwSize;
		PPACKETBASE pPacketBase;

		dwSize = sizeof (PACKETBASE) +
				 sizeof (dwMapID) +
				 sizeof (dwCharID) +
				 sizeof (nDirection) +
				 sizeof (x) +
				 sizeof (y) +
				 sizeof (bUpdate) +
				 sizeof (nSpeedLevel) +
				 sizeof (dwTimeStamp);

		pData = new BYTE[dwSize];
		ZeroMemory (pData, dwSize);
		pPacketBase = (PPACKETBASE)pData;

		pPacketBase->byCmdMain = SBOCOMMANDID_MAIN_CHAR;
		pPacketBase->byCmdSub = SBOCOMMANDID_SUB_CHAR_MOVE_DIR_CHANGE;

		pDataTmp = (PBYTE)(pPacketBase + 1);
		CopyMemoryRenew (pDataTmp, &dwMapID, sizeof (dwMapID), pDataTmp);
		CopyMemoryRenew (pDataTmp, &dwCharID, sizeof (dwCharID), pDataTmp);
		CopyMemoryRenew (pDataTmp, &nDirection, sizeof (nDirection), pDataTmp);
		CopyMemoryRenew (pDataTmp, &x, sizeof (x), pDataTmp);
		CopyMemoryRenew (pDataTmp, &y, sizeof (y), pDataTmp);
		CopyMemoryRenew (pDataTmp, &bUpdate, sizeof (bUpdate), pDataTmp);
		CopyMemoryRenew (pDataTmp, &nSpeedLevel, sizeof (nSpeedLevel), pDataTmp);
		CopyMemoryRenew (pDataTmp, &dwTimeStamp, sizeof (dwTimeStamp), pDataTmp);

		RenewPacket (pData, dwSize);
	}

	PBYTE Set(PBYTE pPacket)
	{
		int x, y;
		PBYTE pRet, pDataTmp;

		pRet = pPacket;
		pDataTmp = CPacketBase::Set (pPacket);

		CopyMemoryRenew (&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);
		CopyMemoryRenew (&m_dwCharID, pDataTmp, sizeof (m_dwCharID), pDataTmp);
		CopyMemoryRenew (&m_nDirection, pDataTmp, sizeof (m_nDirection), pDataTmp);
		CopyMemoryRenew (&x, pDataTmp, sizeof (x), pDataTmp);
		CopyMemoryRenew (&y, pDataTmp, sizeof (y), pDataTmp);
		CopyMemoryRenew (&m_bUpdate, pDataTmp, sizeof (m_bUpdate), pDataTmp);
		CopyMemoryRenew (&m_nSpeedLevel, pDataTmp, sizeof (m_nSpeedLevel), pDataTmp);
		CopyMemoryRenew (&m_dwTimeStamp, pDataTmp, sizeof (m_dwTimeStamp), pDataTmp);

		m_pos.x = x;
		m_pos.y = y;
		pRet = pDataTmp;
		return pRet;
	}

public:
	BOOL m_bUpdate;
	DWORD m_dwMapID, m_dwCharID;
	int m_nDirection;
	POINT m_pos;
	int m_nSpeedLevel;
	DWORD m_dwTimeStamp;
} CPacketCHAR_MOVE_DIR_CHANGE, *PCPacketCHAR_MOVE_DIR_CHANGE;

/* Copyright(C)URARA-works 2026 */
