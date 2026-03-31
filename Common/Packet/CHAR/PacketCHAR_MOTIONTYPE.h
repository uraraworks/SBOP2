/// @file PacketCHAR_MOTIONTYPE.h
/// @brief コマンド(管理者系:モーション種別情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/05/31
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

class CLibInfoMotionType;

// クラス宣言

typedef class CPacketCHAR_MOTIONTYPE : public CPacketBase
{
public:
			CPacketCHAR_MOTIONTYPE();	// コンストラクタ
	virtual ~CPacketCHAR_MOTIONTYPE();	// デストラクタ

	void	Make(DWORD dwMotionTypeID, CLibInfoMotionType *pSrc);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwMotionTypeID;	// モーション種別ID
	CLibInfoMotionType	*m_pInfo;	// モーション種別情報
} CPacketCHAR_MOTIONTYPE, *PCPacketCHAR_MOTIONTYPE;
