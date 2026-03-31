/// @file PacketCHAR_RES_CHARINFO.h
/// @brief コマンド(キャラ系:キャラ情報応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/12/31
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

// クラス宣言

typedef class CPacketCHAR_RES_CHARINFO : public CPacketBase
{
public:
			CPacketCHAR_RES_CHARINFO();	// コンストラクタ
	virtual ~CPacketCHAR_RES_CHARINFO();	// デストラクタ

	void	Make(CInfoCharBase *pInfo, BOOL bChgScreenPos = FALSE);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	BOOL	m_bChgScreenPos;	// 画面位置変更
	CInfoCharBase	*m_pInfo;	// キャラ情報
} CPacketCHAR_RES_CHARINFO, *PCPacketCHAR_RES_CHARINFO;
