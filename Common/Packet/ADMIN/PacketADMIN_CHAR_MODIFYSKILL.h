/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_MODIFYSKILL.h								 */
/* 内容			:コマンド(管理者系:所持スキルの変更) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/18													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoCharBase;

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

enum {
	CHARMODIFYSKILLTYPE_NONE = 0,
	CHARMODIFYSKILLTYPE_ADD,						/* 追加 */
	CHARMODIFYSKILLTYPE_DELETE,						/* 削除 */
};


/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_MODIFYSKILL : public CPacketBase
{
public:
			CPacketADMIN_CHAR_MODIFYSKILL();				/* コンストラクタ */
	virtual ~CPacketADMIN_CHAR_MODIFYSKILL();				/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwSkillID, int nType);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD		m_dwCharID,					/* キャラID */
				m_dwSkillID;				/* スキルID */
	int			m_nType;					/* 種別 */
} CPacketADMIN_CHAR_MODIFYSKILL, *PCPacketADMIN_CHAR_MODIFYSKILL;

/* Copyright(C)URARA-works 2009 */
