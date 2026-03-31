/// @file InfoCharATACKANIMECli.cpp
/// @brief キャラ情報(攻撃受けるとアニメーション)クライアントクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/10
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "MgrSound.h"
#include "InfoCharATACKANIMECli.h"


CInfoCharATACKANIMECli::CInfoCharATACKANIMECli()
{
	m_bMotionDirection = FALSE;
	m_nAnime = 0;

	m_adwMotionID[CHARMOTIONID_STAND]	= CHARMOTIONLISTID_STAND;	// モーションID(立ち)
	m_adwMotionID[CHARMOTIONID_WALK]	= CHARMOTIONLISTID_STAND_SIMPLE;	// モーションID(歩き)
	m_adwMotionID[CHARMOTIONID_ANIME]	= CHARMOTIONLISTID_DESTROY;	// モーションID(アニメーション)
}


CInfoCharATACKANIMECli::~CInfoCharATACKANIMECli()
{
}


void CInfoCharATACKANIMECli::ChgMoveState(int nMoveState)
{
	CInfoCharCli::ChgMoveState(nMoveState);

	m_nAnime = 0;
}


BOOL CInfoCharATACKANIMECli::RenewAnime(
	DWORD dwTime,	// [in] 時間
	int nAdd/*=1*/)	// [in] 次のパターンへの増加分
{
	BOOL bRet;
	int nCount;
	DWORD dwTmp;

	bRet	= FALSE;
	nCount	= 0;

	if (m_nMoveState != CHARMOVESTATE_ANIME) {
		bRet = TRUE;
		goto Exit;
	}

	bRet = CInfoCharCli::RenewAnime(dwTime);
	GetMotionInfo(&nCount);

	if (m_nAnime >= nCount - 1) {
		dwTmp = dwTime - m_dwLastTimeAnime;
		if (dwTmp > 5000) {
//Todo:とりあえず
			ChgMoveState(CHARMOVESTATE_STAND);
		}
		goto Exit;
	}

	if (bRet && m_nAnime == 1) {
//Todo:とりあえず
		switch (m_wGrpIDNPC) {
		case 8:	m_pMgrSound->PlaySound(SOUNDID_CRASH20_D);	break;
		case 9:	m_pMgrSound->PlaySound(SOUNDID_KUSA);	break;
		case 10:	m_pMgrSound->PlaySound(SOUNDID_KAI);	break;
		}
	}

Exit:
	return bRet;
}

