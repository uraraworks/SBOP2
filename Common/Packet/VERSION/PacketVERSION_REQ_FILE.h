/// @file PacketVERSION_REQ_FILE.h
/// @brief コマンド(バージョン系:ファイル要求) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/20
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketVERSION_REQ_FILE : public CPacketBase
{
public:
			CPacketVERSION_REQ_FILE();	// コンストラクタ
	virtual ~CPacketVERSION_REQ_FILE();	// デストラクタ

	void	Make(DWORD dwOffset, DWORD dwReqSize, LPCSTR pszFileName);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwOffset,	// 開始オフセット
				m_dwReqSize;	// 要求サイズ
	CmyString	m_strFileName;	// 相対パス付きファイル名
} CPacketVERSION_REQ_FILE, *PCPacketVERSION_REQ_FILE;
