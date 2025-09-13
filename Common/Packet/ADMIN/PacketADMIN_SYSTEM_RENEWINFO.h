/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_SYSTEM_RENEWINFO.h								 */
/* 内容			:コマンド(管理者系:システム情報の更新) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoSystem;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_SYSTEM_RENEWINFO : public CPacketBase
{
public:
			CPacketADMIN_SYSTEM_RENEWINFO();						/* コンストラクタ */
	virtual ~CPacketADMIN_SYSTEM_RENEWINFO();						/* デストラクタ */

	void	Make	(CInfoSystem *pInfoSystem);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);						/* パケットを設定 */


public:
	CInfoSystem		*m_pInfoSystem;				/* システム情報 */
} CPacketADMIN_SYSTEM_RENEWINFO, *PCPacketADMIN_SYSTEM_RENEWINFO;

/* Copyright(C)URARA-works 2008 */
