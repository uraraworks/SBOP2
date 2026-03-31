/// @file InfoCharPUTNPC.cpp
/// @brief キャラ情報(NPC発生)サーバークラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/12
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "InfoCharPUTNPC.h"

CInfoCharPUTNPC::CInfoCharPUTNPC()
{
	m_nPutMoveType	= CHARMOVETYPE_BATTLE1;

	m_dwLastTimePut	= 0;
	m_nPutCount	= 0;
}

CInfoCharPUTNPC::~CInfoCharPUTNPC()
{
}

void CInfoCharPUTNPC::SetMoveState(int nMoveState)
{
	CInfoCharSvr::SetMoveState(nMoveState);
}

BOOL CInfoCharPUTNPC::TimerProc(DWORD dwTime)
{
	DWORD dwTmp;

	dwTmp = dwTime - m_dwLastTimePut;
	if (dwTmp >= m_dwPutCycle * 1000) {
		if (m_nPutCount < m_nMaxPutCount) {
			// 発生？
			if ((genrand() % 100) > 100 - m_nPutAverage) {
				// 発生予約
				m_bChgPutNpc = TRUE;
			}
		}
		m_dwLastTimePut	= dwTime;
	}

	return CInfoCharSvr::TimerProc(dwTime);
}

void CInfoCharPUTNPC::IncPutCount(void)
{
	m_nPutCount ++;
}

void CInfoCharPUTNPC::DecPutCount(void)
{
	m_nPutCount --;
}
