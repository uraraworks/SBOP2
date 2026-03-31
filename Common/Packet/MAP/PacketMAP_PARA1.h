/// @file PacketMAP_PARA1.h
/// @brief コマンド(マップ系:汎用[パラメータ1個]) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/28
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketMAP_PARA1 : public CPacketBase
{
public:
			CPacketMAP_PARA1();	// コンストラクタ
	virtual ~CPacketMAP_PARA1();	// デストラクタ

	void	Make(BYTE byCmdSub, DWORD dwCharID, DWORD dwPara);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID,	// キャラID
			m_dwPara;	// パラメータ
} CPacketMAP_PARA1, *PCPacketMAP_PARA1;
