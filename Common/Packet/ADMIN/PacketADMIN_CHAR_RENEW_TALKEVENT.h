/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_RENEW_TALKEVENT.h							 */
/* 内容			:コマンド(管理者系:会話イベント情報更新) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/27													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoTalkEvent;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RENEW_TALKEVENT : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RENEW_TALKEVENT();				/* コンストラクタ */
	virtual ~CPacketADMIN_CHAR_RENEW_TALKEVENT();				/* デストラクタ */

	void	Make	(CInfoTalkEvent *pInfo, DWORD dwParam);				/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);									/* パケットを設定 */


public:
	DWORD			m_dwParam;				/* パラメータ */
	CInfoTalkEvent	*m_pInfo;				/* 会話イベント情報 */
} CPacketADMIN_CHAR_RENEW_TALKEVENT, *PCPacketADMIN_CHAR_RENEW_TALKEVENT;

/* Copyright(C)URARA-works 2008 */
