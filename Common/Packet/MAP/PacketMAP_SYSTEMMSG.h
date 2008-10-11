/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_SYSTEMMSG.h										 */
/* 内容			:コマンド(マップ系:システムメッセージ通知) 定義ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/02/10													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* メッセージ種別 */
enum {
	SYSTEMMSGTYPE_DEFAULT = 0,		/* 通常 */
	SYSTEMMSGTYPE_NOLOG,			/* ログに残さない */
	SYSTEMMSGTYPE_BATTLE,			/* 戦闘関連 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_SYSTEMMSG : public CPacketBase
{
public:
			CPacketMAP_SYSTEMMSG();						/* コンストラクタ */
	virtual ~CPacketMAP_SYSTEMMSG();					/* デストラクタ */

	void	Make	(LPCSTR pszMsg, COLORREF clMsg = 0, BOOL bSound = TRUE, int nMsgType = SYSTEMMSGTYPE_DEFAULT);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);											/* パケットを設定 */


public:
	int			m_nMsgType;			/* メッセージ種別 */
	BOOL		m_bSound;			/* 表示する時に音をならすか判定 */
	COLORREF	m_clMsg;			/* 表示色 */
	CmyString	m_strMsg;			/* システムメッセージ */
} CPacketMAP_SYSTEMMSG, *PCPacketMAP_SYSTEMMSG;

/* Copyright(C)URARA-works 2007 */
