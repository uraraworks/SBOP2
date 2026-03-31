/// @file PacketMSGCMD_PARA1.h
/// @brief コマンド(メッセージコマンド系:汎用[パラメータ1個]) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/12/31
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketMSGCMD_PARA1 : public CPacketBase
{
public:
			CPacketMSGCMD_PARA1();	// コンストラクタ
	virtual ~CPacketMSGCMD_PARA1();	// デストラクタ

	void	Make(BYTE byCmdSub, DWORD dwCharID, DWORD dwPara);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwCharID,	// キャラID
			m_dwPara;	// パラメータ
} CPacketMSGCMD_PARA1, *PCPacketMSGCMD_PARA1;
