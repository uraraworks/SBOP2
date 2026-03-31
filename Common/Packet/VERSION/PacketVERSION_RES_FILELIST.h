/// @file PacketVERSION_RES_FILELIST.h
/// @brief コマンド(バージョン系:ファイルリスト応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/03/08
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

class CInfoFileList;

// クラス宣言

typedef class CPacketVERSION_RES_FILELIST : public CPacketBase
{
public:
			CPacketVERSION_RES_FILELIST();	// コンストラクタ
	virtual ~CPacketVERSION_RES_FILELIST();	// デストラクタ

	void	Make(CInfoFileList *pSrc);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CInfoFileList	*m_pInfo;	// ファイルリスト情報
} CPacketVERSION_RES_FILELIST, *PCPacketVERSION_RES_FILELIST;
