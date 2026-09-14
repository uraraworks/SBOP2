/// @file PacketCHAR_REQ_PUSH.h
/// @brief コマンド(キャラ系:押す要求) 定義ファイル
/// @date 2026/09/13
/// @copyright Copyright(C)URARA-works 2026
///
/// docs/push-object-redesign.md S3。押せる物(Push=1 NPC)への押し要求。
/// 旧クライアント(歩数方式)との互換は無し。サーバーは送信元セッションから
/// 「押している本人」を特定するため、本人の座標を自己申告として載せる
/// (PushDecision::CheckSelfPosValid でサーバー座標との乖離を検証する)。

#pragma once

#include "PacketBase.h"
#include "Command.h"

// 押しの種別
enum {
	PUSHTYPE_PUSH = 0,	// 通常の押し
	PUSHTYPE_SWAP,	// 入れ替わり(ボールのみ。S5で実装)
};

typedef class CPacketCHAR_REQ_PUSH : public CPacketBase
{
public:
	CPacketCHAR_REQ_PUSH()
	{
		m_dwObjCharID = 0;
		m_nDirection = 0;
		m_nPushType = PUSHTYPE_PUSH;
		m_ptObjTarget.x = m_ptObjTarget.y = 0;
		m_ptSelf.x = m_ptSelf.y = 0;
		m_dwTimeStamp = 0;
		m_bRelease = FALSE;
	}
	virtual ~CPacketCHAR_REQ_PUSH() {}

	void Make(DWORD dwObjCharID, int nDirection, int nPushType, POINT ptObjTarget, POINT ptSelf, DWORD dwTimeStamp, BOOL bRelease)
	{
		PBYTE pData, pDataTmp;
		DWORD dwSize;
		PPACKETBASE pPacketBase;

		dwSize = sizeof (PACKETBASE) +
				 sizeof (dwObjCharID) +
				 sizeof (nDirection) +
				 sizeof (nPushType) +
				 sizeof (ptObjTarget) +
				 sizeof (ptSelf) +
				 sizeof (dwTimeStamp) +
				 sizeof (bRelease);

		pData = new BYTE[dwSize];
		ZeroMemory(pData, dwSize);
		pPacketBase = (PPACKETBASE)pData;

		pPacketBase->byCmdMain = SBOCOMMANDID_MAIN_CHAR;
		pPacketBase->byCmdSub = SBOCOMMANDID_SUB_CHAR_REQ_PUSH;

		pDataTmp = (PBYTE)(pPacketBase + 1);
		CopyMemoryRenew(pDataTmp, &dwObjCharID, sizeof (dwObjCharID), pDataTmp);
		CopyMemoryRenew(pDataTmp, &nDirection, sizeof (nDirection), pDataTmp);
		CopyMemoryRenew(pDataTmp, &nPushType, sizeof (nPushType), pDataTmp);
		CopyMemoryRenew(pDataTmp, &ptObjTarget, sizeof (ptObjTarget), pDataTmp);
		CopyMemoryRenew(pDataTmp, &ptSelf, sizeof (ptSelf), pDataTmp);
		CopyMemoryRenew(pDataTmp, &dwTimeStamp, sizeof (dwTimeStamp), pDataTmp);
		CopyMemoryRenew(pDataTmp, &bRelease, sizeof (bRelease), pDataTmp);

		RenewPacket(pData, dwSize);
	}

	PBYTE Set(PBYTE pPacket)
	{
		PBYTE pRet, pDataTmp;

		pRet = pPacket;
		pDataTmp = CPacketBase::Set(pPacket);

		CopyMemoryRenew(&m_dwObjCharID, pDataTmp, sizeof (m_dwObjCharID), pDataTmp);
		CopyMemoryRenew(&m_nDirection, pDataTmp, sizeof (m_nDirection), pDataTmp);
		CopyMemoryRenew(&m_nPushType, pDataTmp, sizeof (m_nPushType), pDataTmp);
		CopyMemoryRenew(&m_ptObjTarget, pDataTmp, sizeof (m_ptObjTarget), pDataTmp);
		CopyMemoryRenew(&m_ptSelf, pDataTmp, sizeof (m_ptSelf), pDataTmp);
		CopyMemoryRenew(&m_dwTimeStamp, pDataTmp, sizeof (m_dwTimeStamp), pDataTmp);
		CopyMemoryRenew(&m_bRelease, pDataTmp, sizeof (m_bRelease), pDataTmp);

		pRet = pDataTmp;
		return pRet;
	}

public:
	DWORD m_dwObjCharID;	// 押せる物のキャラID
	int m_nDirection;	// 押す向き(0=上,1=下,2=左,3=右)
	int m_nPushType;	// 種別(PUSHTYPE_*)
	POINT m_ptObjTarget;	// 押せる物の目標座標(自己申告。サーバーが空いている所まで進める)
	POINT m_ptSelf;	// 押している本人の座標(自己申告。サーバー座標との乖離チェックに使う)
	DWORD m_dwTimeStamp;	// クライアント時刻(ms)
	BOOL m_bRelease;	// 押すのをやめた最後の送信か(TRUEならサーバーは150msタイムアウトを待たず即停止)
} CPacketCHAR_REQ_PUSH, *PCPacketCHAR_REQ_PUSH;
