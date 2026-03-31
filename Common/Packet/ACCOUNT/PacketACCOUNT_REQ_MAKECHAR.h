/// @file PacketACCOUNT_REQ_MAKECHAR.h
/// @brief コマンド(アカウント系:キャラ作成要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/14
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

// クラス宣言

typedef class CPacketACCOUNT_REQ_MAKECHAR : public CPacketBase
{
public:
			CPacketACCOUNT_REQ_MAKECHAR();	// コンストラクタ
	virtual ~CPacketACCOUNT_REQ_MAKECHAR();	// デストラクタ

	void	Make(DWORD dwAccountID, CInfoCharBase *pInfoCharBase);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwAccountID;	// アカウントID
	CInfoCharBase	*m_pInfoCharBase;	// キャラ情報
} CPacketACCOUNT_REQ_MAKECHAR, *PCPacketACCOUNT_REQ_MAKECHAR;
