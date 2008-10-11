/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_FORMATMSG.h										 */
/* 内容			:コマンド(マップ系:フォーマットメッセージ通知) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/03													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* メッセージ種別 */
enum {
	FORMATMSGTYPE_DEFAULT = 0,		/* 通常 */
	FORMATMSGTYPE_NOLOG,			/* ログに残さない */
	FORMATMSGTYPE_BATTLE,			/* 戦闘関連 */
};

/* メッセージID */
enum {
	FORMATMSGID_NONE = 0,
	FORMATMSGID_DAMAGE1,			/* %sから %d のダメージ */
	FORMATMSGID_DAMAGE2,			/* %sに %d のダメージ */
	FORMATMSGID_SWOON1,				/* %sは%sを倒した */
	FORMATMSGID_SWOON2,				/* %sを倒した */
	FORMATMSGID_SWOON3,				/* %sに倒された */
	FORMATMSGID_NOUSE,				/* %sは効果がありませんでした */
	FORMATMSGID_NOUNEQUIP_ITEMFULL,	/* 持ち物がいっぱいで装備を外せません */
	FORMATMSGID_DROPITEM,			/* %sは%sを落とした */
};

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketMAP_FORMATMSG : public CPacketBase
{
public:
			CPacketMAP_FORMATMSG();						/* コンストラクタ */
	virtual ~CPacketMAP_FORMATMSG();					/* デストラクタ */

	void	Make	(DWORD dwMsgID, DWORD dwPara1 = 0, DWORD dwPara2 = 0, COLORREF clMsg = 0, BOOL bSound = TRUE, int nMsgType = FORMATMSGTYPE_DEFAULT);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);											/* パケットを設定 */


public:
	int			m_nMsgType;			/* メッセージ種別 */
	BOOL		m_bSound;			/* 表示する時に音をならすか判定 */
	DWORD		m_dwMsgID,			/* メッセージID */
				m_dwPara1,			/* パラメータ1 */
				m_dwPara2;			/* パラメータ2 */
	COLORREF	m_clMsg;			/* 表示色 */
} CPacketMAP_FORMATMSG, *PCPacketMAP_FORMATMSG;

/* Copyright(C)URARA-works 2008 */
