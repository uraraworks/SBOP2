/// @file PacketVERSION_REQ_FILELIST.h
/// @brief コマンド(バージョン系:ファイルリスト要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/03/08
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketVERSION_REQ_FILELIST : public CPacketBase
{
public:
			CPacketVERSION_REQ_FILELIST();	// コンストラクタ
	virtual ~CPacketVERSION_REQ_FILELIST();	// デストラクタ

	void	Make(void);	// パケットを作成
} CPacketVERSION_REQ_FILELIST, *PCPacketVERSION_REQ_FILELIST;
