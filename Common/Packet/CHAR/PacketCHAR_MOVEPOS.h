/// @file PacketCHAR_MOVEPOS.h
/// @brief コマンド(キャラ系:移動通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/14
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketCHAR_MOVEPOS : public CPacketBase
{
public:
			CPacketCHAR_MOVEPOS();	// コンストラクタ
	virtual ~CPacketCHAR_MOVEPOS();	// デストラクタ

	void	Make(DWORD dwMapID, DWORD dwCharID, int nDirection, int x, int y, BOOL bUpdate);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	BOOL	m_bUpdate;	// 自キャラの座標を更新する
	DWORD	m_dwMapID,	// マップID
			m_dwCharID;	// キャラID
	int	m_nDirection;	// 向き
	POINT	m_pos;	// 座標
} CPacketCHAR_MOVEPOS, *PCPacketCHAR_MOVEPOS;
