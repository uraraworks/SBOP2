/// @file PacketVERSION_RES_FILE.h
/// @brief コマンド(バージョン系:ファイル応答) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/20
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketVERSION_RES_FILE : public CPacketBase
{
public:
			CPacketVERSION_RES_FILE();	// コンストラクタ
	virtual ~CPacketVERSION_RES_FILE();	// デストラクタ

	void	Make(DWORD dwOffset, DWORD dwDataSize, LPCSTR pszFileName, PBYTE pFileData);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwOffset,	// 開始オフセット
				m_dwSize;	// サイズ
	CmyString	m_strFileName;	// 相対パス付きファイル名
	PBYTE	m_pFileData;	// ファイルデータ
} CPacketVERSION_RES_FILE, *PCPacketVERSION_RES_FILE;
