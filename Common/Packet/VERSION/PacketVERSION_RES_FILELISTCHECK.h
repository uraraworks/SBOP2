/// @file PacketVERSION_RES_FILELISTCHECK.h
/// @brief コマンド(バージョン系:ファイルリストチェック応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/28
/// @copyright Copyright(C)URARA-works 2007

#pragma once

#include "PacketBase.h"

// 定数の定義

enum {
	FILELISTCHECKRES_NONE = 0,
	FILELISTCHECKRES_OK,	// 問題無し
	FILELISTCHECKRES_NG,	// 不一致
};

// クラス宣言

typedef class CPacketVERSION_RES_FILELISTCHECK : public CPacketBase
{
public:
			CPacketVERSION_RES_FILELISTCHECK();	// コンストラクタ
	virtual ~CPacketVERSION_RES_FILELISTCHECK();	// デストラクタ

	void	Make(int nResult);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	int	m_nResult;	// 結果
} CPacketVERSION_RES_FILELISTCHECK, *PCPacketVERSION_RES_FILELISTCHECK;
