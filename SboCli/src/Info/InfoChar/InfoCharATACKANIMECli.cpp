/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:InfoCharATACKANIMECli.cpp									 */
/* 内容			:キャラ情報(攻撃受けるとアニメーション)クライアントクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/10													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrSound.h"
#include "InfoCharATACKANIMECli.h"


/* ========================================================================= */
/* 関数名	:CInfoCharATACKANIMECli::CInfoCharATACKANIMECli					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/10														 */
/* ========================================================================= */

CInfoCharATACKANIMECli::CInfoCharATACKANIMECli()
{
	m_bMotionDirection = FALSE;
	m_nAnime = 0;

	m_adwMotionID[CHARMOTIONID_STAND]	= CHARMOTIONLISTID_STAND;			/* モーションID(立ち) */
	m_adwMotionID[CHARMOTIONID_WALK]	= CHARMOTIONLISTID_STAND_SIMPLE;	/* モーションID(歩き) */
	m_adwMotionID[CHARMOTIONID_ANIME]	= CHARMOTIONLISTID_DESTROY;			/* モーションID(アニメーション) */
}


/* ========================================================================= */
/* 関数名	:CInfoCharATACKANIMECli::~CInfoCharATACKANIMECli				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/10														 */
/* ========================================================================= */

CInfoCharATACKANIMECli::~CInfoCharATACKANIMECli()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharATACKANIMECli::ChgMoveState							 */
/* 内容		:移動状態変更													 */
/* 日付		:2007/09/10														 */
/* ========================================================================= */

void CInfoCharATACKANIMECli::ChgMoveState(int nMoveState)
{
	CInfoCharCli::ChgMoveState (nMoveState);

	m_nAnime = 0;
}


/* ========================================================================= */
/* 関数名	:CInfoCharATACKANIMECli::RenewAnime								 */
/* 内容		:アニメーションの更新											 */
/* 日付		:2007/09/11														 */
/* ========================================================================= */

BOOL CInfoCharATACKANIMECli::RenewAnime(
	DWORD dwTime,		/* [in] 時間 */
	int nAdd/*=1*/)		/* [in] 次のパターンへの増加分 */
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

	bRet = CInfoCharCli::RenewAnime (dwTime);
	GetMotionInfo (&nCount);

	if (m_nAnime >= nCount - 1) {
		dwTmp = dwTime - m_dwLastTimeAnime;
		if (dwTmp > 5000) {
//Todo:とりあえず
			ChgMoveState (CHARMOVESTATE_STAND);
		}
		goto Exit;
	}

	if (bRet && m_nAnime == 1) {
//Todo:とりあえず
		switch (m_wGrpIDNPC) {
		case 8:		m_pMgrSound->PlaySound (SOUNDID_CRASH20_D);	break;
		case 9:		m_pMgrSound->PlaySound (SOUNDID_KUSA);		break;
		case 10:	m_pMgrSound->PlaySound (SOUNDID_KAI);		break;
		}
	}

Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2007 */
