/// @file PacketCHAR_RES_TALKEVENT.h
/// @brief コマンド(キャラ系:会話イベント情報応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/27
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

class CInfoTalkEvent;

// クラス宣言

typedef class CPacketCHAR_RES_TALKEVENT : public CPacketBase
{
public:
			CPacketCHAR_RES_TALKEVENT();	// コンストラクタ
	virtual ~CPacketCHAR_RES_TALKEVENT();	// デストラクタ

	void	Make(CInfoTalkEvent *pInfo, DWORD dwParam);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwParam;	// パラメータ
	CInfoTalkEvent	*m_pInfo;	// 会話イベント情報
} CPacketCHAR_RES_TALKEVENT, *PCPacketCHAR_RES_TALKEVENT;
