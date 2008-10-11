/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_RENEWMOTIONTYPE.h							 */
/* 内容			:コマンド(管理者系:キャラモーション種別情報の更新) 定義ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/05/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CLibInfoMotionType;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RENEWMOTIONTYPE : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RENEWMOTIONTYPE();				/* コンストラクタ */
	virtual ~CPacketADMIN_CHAR_RENEWMOTIONTYPE();				/* デストラクタ */

	void	Make	(DWORD dwMotionTypeID, CLibInfoMotionType *pSrc);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);									/* パケットを設定 */


public:
	DWORD				m_dwMotionTypeID;	/* モーション種別ID */
	CLibInfoMotionType	*m_pInfo;			/* モーション種別情報 */
} CPacketADMIN_CHAR_RENEWMOTIONTYPE, *PCPacketADMIN_CHAR_RENEWMOTIONTYPE;

/* Copyright(C)URARA-works 2008 */
