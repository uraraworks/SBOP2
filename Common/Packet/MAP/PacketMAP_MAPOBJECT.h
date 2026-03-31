/// @file PacketMAP_MAPOBJECT.h
/// @brief コマンド(マップ系:マップオブジェクト情報通知) 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/01
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#include "PacketBase.h"

class CLibInfoMapObject;
class CInfoMapObject;

// クラス宣言

typedef class CPacketMAP_MAPOBJECT : public CPacketBase
{
public:
			CPacketMAP_MAPOBJECT();	// コンストラクタ
	virtual ~CPacketMAP_MAPOBJECT();	// デストラクタ

	void	Make(CLibInfoMapObject *pLibInfo);	// パケットを作成
	void	Make(CInfoMapObject *pInfo);	// パケットを作成
	PBYTE	Set(PBYTE pPacket);	// パケットを設定

public:
	CLibInfoMapObject	*m_pLibInfoMapObject;	// マップオブジェクト情報
} CPacketMAP_MAPOBJECT, *PCPacketMAP_MAPOBJECT;
