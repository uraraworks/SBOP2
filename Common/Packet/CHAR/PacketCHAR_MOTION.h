/// @file PacketCHAR_MOTION.h
/// @brief コマンド(管理者系:モーション情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/11/23
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

class CLibInfoMotion;

// クラス宣言

typedef class CPacketCHAR_MOTION : public CPacketBase
{
public:
			CPacketCHAR_MOTION();	// コンストラクタ
	virtual ~CPacketCHAR_MOTION();	// デストラクタ

	void	Make(DWORD dwMotionTypeID, DWORD dwMotionListID, CLibInfoMotion *pSrc);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwMotionTypeID,	// モーション種別ID
					m_dwMotionListID;	// モーションリストID
	CLibInfoMotion	*m_pInfo;	// モーション情報
} CPacketCHAR_MOTION, *PCPacketCHAR_MOTION;
