/// @file PacketADMIN_MAP_SETMAPNAME.h
/// @brief コマンド(管理者系:マップ名変更) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/05/24
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

// クラス宣言

typedef class CPacketADMIN_MAP_SETMAPNAME : public CPacketBase
{
public:
			CPacketADMIN_MAP_SETMAPNAME();	// コンストラクタ
	virtual ~CPacketADMIN_MAP_SETMAPNAME();	// デストラクタ

	void	Make(DWORD dwMapID, DWORD dwBGMID, DWORD dwWeatherType, BOOL bEnableBattle, BOOL bRecovery, BYTE byLevel, LPCSTR pszMapName);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	DWORD	m_dwMapID,	// マップID
				m_dwBGMID,	// BGMID
				m_dwWeatherType;	// 天気種別
	BOOL	m_bEnableBattle,	// 戦闘許可
				m_bRecovery;	// 気絶後回復する
	BYTE	m_byLevel;	// 明るさレベル
	CmyString	m_strMapName;	// マップ名
} CPacketADMIN_MAP_SETMAPNAME, *PCPacketADMIN_MAP_SETMAPNAME;
