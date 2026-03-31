/// @file PacketADMIN_PARA2.h
/// @brief コマンド(管理者系:汎用[パラメータ2個]) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/25
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_PARA2 : public CPacketBase
{
public:
			CPacketADMIN_PARA2();	// コンストラクタ
	virtual ~CPacketADMIN_PARA2();	// デストラクタ

	void	Make(BYTE byCmdSub, DWORD dwPara1=0, DWORD dwPara=0);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwPara1,	// パラメータ1
			m_dwPara2;	// パラメータ2
} CPacketADMIN_PARA2, *PCPacketADMIN_PARA2;
