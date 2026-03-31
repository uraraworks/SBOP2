/// @file PacketSKILL_PARA2.h
/// @brief コマンド(スキル系:汎用[パラメータ2個]) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/06
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketSKILL_PARA2 : public CPacketBase
{
public:
			CPacketSKILL_PARA2();	// コンストラクタ
	virtual ~CPacketSKILL_PARA2();	// デストラクタ

	void	Make(BYTE byCmdSub, DWORD dwPara1=0, DWORD dwPara=0);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwPara1,	// パラメータ1
			m_dwPara2;	// パラメータ2
} CPacketSKILL_PARA2, *PCPacketSKILL_PARA2;
