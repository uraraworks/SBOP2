/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_SKILL_RENEWSKILL.h								 */
/* 内容			:コマンド(管理者系:スキル情報通知) 定義ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/08													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoSkillBase;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_SKILL_RENEWSKILL : public CPacketBase
{
public:
			CPacketADMIN_SKILL_RENEWSKILL();			/* コンストラクタ */
	virtual ~CPacketADMIN_SKILL_RENEWSKILL();			/* デストラクタ */

	void	Make	(CInfoSkillBase *pInfo);					/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);							/* パケットを設定 */


public:
	CInfoSkillBase	*m_pInfo;				/* スキル情報 */
} CPacketADMIN_SKILL_RENEWSKILL, *PCPacketADMIN_SKILL_RENEWSKILL;

/* Copyright(C)URARA-works 2008 */
