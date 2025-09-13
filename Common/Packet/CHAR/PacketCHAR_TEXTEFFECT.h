/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_TEXTEFFECT.h									 */
/* 内容			:コマンド(キャラ系:文字エフェクト通知) 定義ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/13													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CPacketCHAR_TEXTEFFECT : public CPacketBase
{
public:
			CPacketCHAR_TEXTEFFECT();							/* コンストラクタ */
	virtual ~CPacketCHAR_TEXTEFFECT();							/* デストラクタ */

	void	Make	(DWORD dwCharID, DWORD dwData, int nColor = 0, int nSize = 0);		/* パケットを作成 */
	void	Make	(DWORD dwCharID, LPCSTR pszData, int nColor = 0, int nSize = 0);	/* パケットを作成 */
	PBYTE	Set		(PBYTE pPacket);									/* パケットを設定 */


public:
	int			m_nColor,				/* 色 */
				m_nSize;				/* サイズ */
	DWORD		m_dwCharID,				/* キャラID */
				m_dwData;				/* 数値データ */
	CmyString	m_strData;				/* 文字列データ */
} CPacketCHAR_TEXTEFFECT, *PCPacketCHAR_TEXTEFFECT;

/* Copyright(C)URARA-works 2008 */
