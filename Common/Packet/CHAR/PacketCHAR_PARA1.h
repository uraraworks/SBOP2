/// @file PacketCHAR_PARA1.h
/// @brief コマンド(キャラ系:汎用[パラメータ1個]) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/01
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCHAR_PARA1 : public CPacketBase
{
public:
			CPacketCHAR_PARA1();	// コンストラクタ
	virtual ~CPacketCHAR_PARA1();	// デストラクタ

	void	Make(BYTE byCmdSub, DWORD dwCharID, DWORD dwPara);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID,	// キャラID
			m_dwPara;	// パラメータ
} CPacketCHAR_PARA1, *PCPacketCHAR_PARA1;
