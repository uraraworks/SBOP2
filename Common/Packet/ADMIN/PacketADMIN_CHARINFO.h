/// @file PacketADMIN_CHARINFO.h
/// @brief コマンド(管理者系:キャラ情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/03/18
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

// クラス宣言

typedef class CPacketADMIN_CHARINFO : public CPacketBase
{
public:
			CPacketADMIN_CHARINFO();	// コンストラクタ
	virtual ~CPacketADMIN_CHARINFO();	// デストラクタ

	void	Make(CInfoCharBase *pInfo, BOOL bChgScreenPos);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	BOOL	m_bChgScreenPos;	// 画面位置変更
	CInfoCharBase	*m_pInfoChar;	// キャラ情報
} CPacketADMIN_CHARINFO, *PCPacketADMIN_CHARINFO;
