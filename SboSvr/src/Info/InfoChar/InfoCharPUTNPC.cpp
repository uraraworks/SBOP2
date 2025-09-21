/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoCharPUTNPC.cpp											 */
/* 内容			:キャラ情報(NPC発生)サーバークラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharPUTNPC.h"


/* ========================================================================= */
/* 関数名	:CInfoCharPUTNPC::CInfoCharPUTNPC								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CInfoCharPUTNPC::CInfoCharPUTNPC()
{
	m_nPutMoveType	= CHARMOVETYPE_BATTLE1;

	m_dwLastTimePut	= 0;
	m_nPutCount		= 0;
}


/* ========================================================================= */
/* 関数名	:CInfoCharPUTNPC::~CInfoCharPUTNPC								 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CInfoCharPUTNPC::~CInfoCharPUTNPC()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharPUTNPC::SetMoveState									 */
/* 内容		:移動状態を変更													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CInfoCharPUTNPC::SetMoveState(int nMoveState)
{
	CInfoCharSvr::SetMoveState (nMoveState);
}


/* ========================================================================= */
/* 関数名	:CInfoCharPUTNPC::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CInfoCharPUTNPC::TimerProc(DWORD dwTime)
{
	DWORD dwTmp;

	dwTmp = dwTime - m_dwLastTimePut;
	if (dwTmp >= m_dwPutCycle * 1000) {
		if (m_nPutCount < m_nMaxPutCount) {
			/* 発生？ */
			if ((genrand () % 100) > 100 - m_nPutAverage) {
				/* 発生予約 */
				m_bChgPutNpc = TRUE;
			}
		}
		m_dwLastTimePut	= dwTime;
	}

	return CInfoCharSvr::TimerProc (dwTime);
}


/* ========================================================================= */
/* 関数名	:CInfoCharPUTNPC::IncPutCount									 */
/* 内容		:発生NPC数を増加												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */
void CInfoCharPUTNPC::IncPutCount(void)
{
	m_nPutCount ++;
}


/* ========================================================================= */
/* 関数名	:CInfoCharPUTNPC::DecPutCount									 */
/* 内容		:発生NPC数を減少												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */
void CInfoCharPUTNPC::DecPutCount(void)
{
	m_nPutCount --;
}

/* Copyright(C)URARA-works 2008 */
