/// @file PacketCHAR_RES_PUSH.h
/// @brief コマンド(キャラ系:押す応答) 定義ファイル
/// @date 2026/09/13
/// @copyright Copyright(C)URARA-works 2026
///
/// docs/push-object-redesign.md S3。REQ_PUSH を送った本人へ返す応答。
/// 受理/却下と、押せる物の確定座標(サーバー権威)を通知する。

#pragma once

#include "PacketBase.h"
#include "Command.h"

typedef class CPacketCHAR_RES_PUSH : public CPacketBase
{
public:
	CPacketCHAR_RES_PUSH()
	{
		m_dwObjCharID = 0;
		m_ptObj.x = m_ptObj.y = 0;
		m_bAccepted = FALSE;
	}
	virtual ~CPacketCHAR_RES_PUSH() {}

	void Make(DWORD dwObjCharID, POINT ptObj, BOOL bAccepted)
	{
		PBYTE pData, pDataTmp;
		DWORD dwSize;
		PPACKETBASE pPacketBase;

		dwSize = sizeof (PACKETBASE) +
				 sizeof (dwObjCharID) +
				 sizeof (ptObj) +
				 sizeof (bAccepted);

		pData = new BYTE[dwSize];
		ZeroMemory(pData, dwSize);
		pPacketBase = (PPACKETBASE)pData;

		pPacketBase->byCmdMain = SBOCOMMANDID_MAIN_CHAR;
		pPacketBase->byCmdSub = SBOCOMMANDID_SUB_CHAR_RES_PUSH;

		pDataTmp = (PBYTE)(pPacketBase + 1);
		CopyMemoryRenew(pDataTmp, &dwObjCharID, sizeof (dwObjCharID), pDataTmp);
		CopyMemoryRenew(pDataTmp, &ptObj, sizeof (ptObj), pDataTmp);
		CopyMemoryRenew(pDataTmp, &bAccepted, sizeof (bAccepted), pDataTmp);

		RenewPacket(pData, dwSize);
	}

	PBYTE Set(PBYTE pPacket)
	{
		PBYTE pRet, pDataTmp;

		pRet = pPacket;
		pDataTmp = CPacketBase::Set(pPacket);

		CopyMemoryRenew(&m_dwObjCharID, pDataTmp, sizeof (m_dwObjCharID), pDataTmp);
		CopyMemoryRenew(&m_ptObj, pDataTmp, sizeof (m_ptObj), pDataTmp);
		CopyMemoryRenew(&m_bAccepted, pDataTmp, sizeof (m_bAccepted), pDataTmp);

		pRet = pDataTmp;
		return pRet;
	}

public:
	DWORD m_dwObjCharID;	// 押せる物のキャラID
	POINT m_ptObj;	// 押せる物の確定座標(サーバー権威)
	BOOL m_bAccepted;	// 受理したか
} CPacketCHAR_RES_PUSH, *PCPacketCHAR_RES_PUSH;
