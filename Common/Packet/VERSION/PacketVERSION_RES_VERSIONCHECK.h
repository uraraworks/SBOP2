/// @file PacketVERSION_RES_VERSIONCHECK.h
/// @brief コマンド(バージョン系:バージョンチェック応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

// 定数の定義

enum {
	VERSIONCHECKRES_NONE = 0,
	VERSIONCHECKRES_OK,	// 問題無し
	VERSIONCHECKRES_NG_VERSION,	// バージョン不一致
};

// クラス宣言

typedef class CPacketVERSION_RES_VERSIONCHECK : public CPacketBase
{
public:
			CPacketVERSION_RES_VERSIONCHECK();	// コンストラクタ
	virtual ~CPacketVERSION_RES_VERSIONCHECK();	// デストラクタ

	void	Make(int nResult);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	int	m_nResult;	// 結果
} CPacketVERSION_RES_VERSIONCHECK, *PCPacketVERSION_RES_VERSIONCHECK;
