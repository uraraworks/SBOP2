/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_RENEW_CLIENTVERSION.h							 */
/* 内容			:コマンド(管理者系:クライアントバージョン更新) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/07													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoAccount;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_RENEW_CLIENTVERSION : public CPacketBase
{
public:
			CPacketADMIN_RENEW_CLIENTVERSION();			/* コンストラクタ */
	virtual ~CPacketADMIN_RENEW_CLIENTVERSION();		/* デストラクタ */

	void	Make	(LPCSTR pszClientVersion);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CmyString	m_strClientVersion;		/* クライアントバージョン */
} CPacketADMIN_RENEW_CLIENTVERSION, *PCPacketADMIN_RENEW_CLIENTVERSION;

/* Copyright(C)URARA-works 2008 */
