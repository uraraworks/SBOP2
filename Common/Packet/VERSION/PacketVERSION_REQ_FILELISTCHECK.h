/// @file PacketVERSION_REQ_FILELISTCHECK.h
/// @brief コマンド(バージョン系:ファイルリストチェック要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/28
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketVERSION_REQ_FILELISTCHECK : public CPacketBase
{
public:
			CPacketVERSION_REQ_FILELISTCHECK();	// コンストラクタ
	virtual ~CPacketVERSION_REQ_FILELISTCHECK();	// デストラクタ

	void	Make(LPCSTR pszFileListHash);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CmyString	m_strFileListHash;	// ファイルリストハッシュ
} CPacketVERSION_REQ_FILELISTCHECK, *PCPacketVERSION_REQ_FILELISTCHECK;
