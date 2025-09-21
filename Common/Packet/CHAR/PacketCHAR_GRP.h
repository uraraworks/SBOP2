/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_GRP.h											 */
/* 内容			:コマンド(キャラ系:画像通知) 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_GRP : public CPacketBase
{
public:
			CPacketCHAR_GRP();							/* コンストラクタ */
	virtual ~CPacketCHAR_GRP();							/* デストラクタ */

	void	Make		(CInfoCharBase *pInfo);					/* パケットを作成 */
	PBYTE	Set			(PBYTE pPacket);						/* パケットを設定 */
	void	SetParam	(CInfoCharBase *pInfo);					/* 指定キャラ情報へパラメータを反映 */


public:
	DWORD	m_dwCharID,					/* キャラID */
			m_dwEquipItemIDCloth,		/* 装備アイテムID(服) */
			m_dwEquipItemIDAcce1,		/* 装備アイテムID(アクセサリ) */
			m_dwEquipItemIDArmsRight,	/* 装備アイテムID(持ち物) */
			m_dwEquipItemIDArmsLeft;	/* 装備アイテムID:左手 */
	WORD	m_wGrpIDNPC,				/* 画像ID(NPC) */
			m_wGrpIDCloth,				/* 画像ID(服) */
			m_wGrpIDEye,				/* 画像ID(目) */
			m_wGrpIDEyeColor,			/* 画像ID(目色) */
			m_wGrpIDHairType,			/* 画像ID(髪) */
			m_wGrpIDHairColor,			/* 画像ID(髪色) */
			m_wGrpIDSP,					/* 画像ID(特殊服) */
			m_wGrpIDAcce,				/* 画像ID(アクセサリ) */
			m_wGrpIDArmsMain,			/* 画像ID(持ち物:メイン) */
			m_wGrpIDArmsSub,			/* 画像ID(持ち物:サブ) */
			m_wGrpIDArmsLeftMain,		/* 画像ID(盾:メイン) */
			m_wGrpIDArmsLeftSub,		/* 画像ID(盾:サブ) */
			m_wGrpIDTmpMain,			/* 画像ID(一時服:メイン) */
			m_wGrpIDTmpSub;				/* 画像ID(一時服:サブ) */
} CPacketCHAR_GRP, *PCPacketCHAR_GRP;

/* Copyright(C)URARA-works 2007 */
