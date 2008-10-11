/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_RENEWMOTION.h								 */
/* 内容			:コマンド(管理者系:キャラモーション情報の更新) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/11/23													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMotion;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RENEWMOTION : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RENEWMOTION();				/* コンストラクタ */
	virtual ~CPacketADMIN_CHAR_RENEWMOTION();				/* デストラクタ */

	void	Make	(DWORD dwMotionTypeID, DWORD dwMotionListID, CLibInfoMotion *pSrc);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);								/* パケットを設定 */


public:
	DWORD			m_dwMotionTypeID,	/* モーション種別ID */
					m_dwMotionListID;	/* モーションリストID */
	CLibInfoMotion	*m_pInfo;			/* モーション情報 */
} CPacketADMIN_CHAR_RENEWMOTION, *PCPacketADMIN_CHAR_RENEWMOTION;

/* Copyright(C)URARA-works 2007 */
