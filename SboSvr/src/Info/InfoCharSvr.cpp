/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharSvr.cpp											 */
/* 内容			:キャラ情報サーバークラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/14													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotion.h"
#include "InfoCharSvr.h"


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::CInfoCharSvr										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/01/14														 */
/* ========================================================================= */

CInfoCharSvr::CInfoCharSvr()
{
	m_nReserveChgEfect		= 0;
	m_nReserveChgMoveState	= 0;
	m_nMoveCount			= 0;
	m_bChgPos				= FALSE;
	m_bChgMap				= FALSE;
	m_bChgUpdatePos			= FALSE;
	m_bChgSpeak				= FALSE;
	m_bChgInfo				= FALSE;
	m_bChgMoveState			= FALSE;
	m_bChgProcState			= FALSE;
	m_bChgScreenPos			= FALSE;
	m_bChgGrp				= FALSE;
	m_bChgEfcBalloon		= FALSE;
	m_bChgMotion			= FALSE;
	m_bChgStatus			= FALSE;
	m_bChgFishingHit		= FALSE;
	m_bChgMoveCount			= FALSE;
	m_bChgPutNpc			= FALSE;
	m_bChgTargetChar		= FALSE;
	m_bWaitCheckMapEvent	= FALSE;
	m_bDropItem				= FALSE;
	m_bAtack				= FALSE;
	m_bRenewTargetPos		= FALSE;
	m_bProcMoveMapIn		= FALSE;
	m_bProcMoveMapOut		= FALSE;
	m_bProcMoveMarkPos		= FALSE;
	m_bProcSwoon			= FALSE;
	m_bProcInvincible		= FALSE;
	m_bStateFadeInOut		= FALSE;
	m_bStatusInvincible		= FALSE;
	m_pInfoMap				= NULL;
	m_dwChgWait				= 0;
	m_dwLastTimeChg			= 0;
	m_dwEfcBalloonID		= 0;
	m_dwMotionID			= 0;
	m_dwMoveCount			= 0;

	m_pLibInfoCharSvr		= NULL;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::~CInfoCharSvr									 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/01/14														 */
/* ========================================================================= */

CInfoCharSvr::~CInfoCharSvr()
{
	DeleteAllProcInfo ();
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::SetSpeak											 */
/* 内容		:発言内容を更新													 */
/* 日付		:2007/02/24														 */
/* ========================================================================= */

void CInfoCharSvr::SetSpeak(LPCSTR pszSpeak)
{
	CInfoCharBase::SetSpeak (pszSpeak);

	if (m_strSpeak.IsEmpty () == FALSE) {
		m_bChgSpeak = TRUE;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::SetMoveState										 */
/* 内容		:移動状態を変更													 */
/* 日付		:2007/04/20														 */
/* ========================================================================= */

void CInfoCharSvr::SetMoveState(int nMoveState)
{
	if (m_nMoveState == CHARMOVESTATE_DELETE) {
		/* 削除は取り消せないようにする */
		return;
	}
	if (nMoveState == m_nMoveState) {
		return;
	}

	m_bChgMoveState = TRUE;
	switch (nMoveState) {
	case CHARMOVESTATE_BATTLEATACK_WAIT:		/* 戦闘攻撃後の待ち時間 */
		m_dwChgWait = 1000;
		m_dwLastTimeChg = timeGetTime ();
		m_bChgMoveState = FALSE;
		break;
	case CHARMOVESTATE_BATTLE:
		/* 溜め攻撃解除 */
		m_bChargeAtack = FALSE;
		break;
	case CHARMOVESTATE_SWOON:					/* 気絶 */
		/* 溜め攻撃解除 */
		m_bChargeAtack = FALSE;
		/* 行動情報を全て削除 */
		DeleteAllProcInfo ();
		SetMotion (-1);
		SetProcState (CHARPROCSTATEID_NORMAL);
		break;
	}

	CInfoCharBase::SetMoveState (nMoveState);
}


/* ========================================================================= */
/* 関数名	:CInfoCharBase::SetProcState									 */
/* 内容		:行動状態を変更													 */
/* 日付		:2008/06/11														 */
/* ========================================================================= */

void CInfoCharSvr::SetProcState(int nProcState)
{
	m_bChgProcState = TRUE;

	CInfoCharBase::SetProcState (nProcState);
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::SetMap											 */
/* 内容		:マップ情報を設定												 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharSvr::SetMap(CInfoMapBase *pInfoMap)
{
	m_pInfoMap = pInfoMap;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::SetLibInfoChar									 */
/* 内容		:キャラ情報ライブラリを設定										 */
/* 日付		:2009/01/17														 */
/* ========================================================================= */

void CInfoCharSvr::SetLibInfoChar(CLibInfoCharSvr *pLibInfoChar)
{
	m_pLibInfoCharSvr = pLibInfoChar;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::SetEfcBalloon									 */
/* 内容		:噴出しを設定													 */
/* 日付		:2008/01/03														 */
/* ========================================================================= */

void CInfoCharSvr::SetEfcBalloon(DWORD dwEfcBalloonID)
{
	m_dwEfcBalloonID = dwEfcBalloonID;
	m_bChgEfcBalloon = TRUE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::SetMotion										 */
/* 内容		:モーションを設定												 */
/* 日付		:2008/01/03														 */
/* ========================================================================= */

void CInfoCharSvr::SetMotion(DWORD dwMotionID)
{
	m_dwMotionID = dwMotionID;
	m_bChgMotion = TRUE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::IsEnableBattle									 */
/* 内容		:戦闘状態に遷移できるか判定										 */
/* 日付		:2008/07/08														 */
/* ========================================================================= */

BOOL CInfoCharSvr::IsEnableBattle(void)
{
	BOOL bRet;

	bRet = FALSE;

	/* ついていっている？ */
	if (m_dwFrontCharID) {
		goto Exit;
	}
	/* 気絶中？ */
	if (m_nMoveState == CHARMOVESTATE_SWOON) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::IsEnableMove										 */
/* 内容		:移動できる状態か判定											 */
/* 日付		:2008/07/28														 */
/* 戻り値	:TRUE:移動可													 */
/* ========================================================================= */

BOOL CInfoCharSvr::IsEnableMove(void)
{
	BOOL bRet;

	bRet = CInfoCharBase::IsEnableMove ();
	if (bRet == FALSE) {
		goto Exit;
	}
	if (m_bStateFadeInOut) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::CopyAll											 */
/* 内容		:派生先の情報も全てコピー										 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharSvr::CopyAll(CInfoCharSvr *pSrc)
{
	CInfoCharBase::Copy (pSrc);

	m_abyMark = pSrc->m_abyMark;
	m_bNPC				= pSrc->m_bNPC;
	m_dwSessionID		= pSrc->m_dwSessionID;
	m_dwAccountID		= pSrc->m_dwAccountID;
	m_dwLastTimeSpeak	= pSrc->m_dwLastTimeSpeak;
	m_dwTailCharID		= pSrc->m_dwTailCharID;
	m_dwFrontCharID		= pSrc->m_dwFrontCharID;
	m_nReserveChgEfect	= pSrc->m_nReserveChgEfect;
	m_bChgPos			= pSrc->m_bChgPos;
	m_bChgUpdatePos		= pSrc->m_bChgUpdatePos;
	m_bChgSpeak			= pSrc->m_bChgSpeak;
	m_bChgInfo			= pSrc->m_bChgInfo;
	m_bChgMoveState		= pSrc->m_bChgMoveState;
	m_bChgScreenPos		= pSrc->m_bChgScreenPos;
	m_bChgPosRenew		= pSrc->m_bChgPosRenew;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::Copy												 */
/* 内容		:コピー															 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

void CInfoCharSvr::Copy(CInfoCharBase *pSrc)
{
	PCInfoCharSvr pSrcTmp;

	pSrcTmp = (PCInfoCharSvr)pSrc;
	CInfoCharBase::Copy (pSrc);

	m_dwMoveCount	= pSrcTmp->m_dwMoveCount;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::ProcAtack										 */
/* 内容		:処理(攻撃した時)												 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharSvr::ProcAtack(void)
{
	/* 溜め攻撃解除 */
	m_bChargeAtack = FALSE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::ProcHit											 */
/* 内容		:処理(攻撃を受けた時)											 */
/* 日付		:2007/09/17														 */
/* 戻り値	:TRUE:以降の処理を続行する										 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcHit(CInfoCharSvr *pInfoChar)
{
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

BOOL CInfoCharSvr::TimerProc(DWORD dwTime)
{
	BOOL bRet;

	bRet = CInfoCharBase::TimerProc (dwTime);

	if (m_nMoveState == CHARMOVESTATE_BATTLEATACK_WAIT) {
		if (m_dwChgWait) {
			if (dwTime - m_dwLastTimeChg > m_dwChgWait) {
				m_dwChgWait = 0;
				m_dwLastTimeChg = 0;
				SetMoveState (CHARMOVESTATE_BATTLEATACK);
			}
		}
	}
	bRet |= TimerProcMOVE (dwTime);

	Proc (dwTime);

	if (m_dwLightTime != 0) {
		if (dwTime > m_dwLightTime) {
			m_nLightLevel = 0;
			m_dwLightTime = 0;
			m_bChgStatus  = TRUE;
		}
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::TimerProcMOVE									 */
/* 内容		:時間処理(移動)													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

BOOL CInfoCharSvr::TimerProcMOVE(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTimeTmp;

	bRet = FALSE;
	if (m_nMoveCount == 0) {
		goto Exit;
	}

	dwTimeTmp = dwTime - m_dwLastTimeMove;
//Todo:暫定
	if (dwTimeTmp < 150) {
		goto Exit;
	}
	m_bChgMoveCount = TRUE;
	m_nMoveCount --;
	m_dwLastTimeMove = dwTime;

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::IsAtackTarget									 */
/* 内容		:攻撃対象となるか判定											 */
/* 日付		:2008/07/12														 */
/* 戻り値	:TRUE:攻撃可													 */
/* ========================================================================= */

BOOL CInfoCharSvr::IsAtackTarget(void)
{
	BOOL bRet;

	bRet = CInfoCharBase::IsAtackTarget ();
	if (bRet == FALSE) {
		goto Exit;
	}
	if (m_bStatusInvincible) {
		goto Exit;
	}
	switch (m_nMoveType) {
	case CHARMOVETYPE_STAND:		/* 移動しない */
	case CHARMOVETYPE_BALL:			/* ボール */
	case CHARMOVETYPE_SCORE:		/* 得点 */
	case CHARMOVETYPE_PUTNPC:		/* NPC発生 */
		bRet = FALSE;
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::GetHitEffectID									 */
/* 内容		:ヒット時に相手に表示するエフェクトIDを取得						 */
/* 日付		:2009/01/12														 */
/* ========================================================================= */

DWORD CInfoCharSvr::GetHitEffectID(void)
{
	return 0;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::GetDamage										 */
/* 内容		:ダメージ値を取得												 */
/* 日付		:2009/01/12														 */
/* ========================================================================= */

DWORD CInfoCharSvr::GetDamage(void)
{
	return 0;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::Proc												 */
/* 内容		:行動処理														 */
/* 日付		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharSvr::Proc(DWORD dwTime)
{
	int i, nCount;
	BOOL bResult;
	DWORD dwTimeTmp;
	PCHARPROCINFO pInfo;

	nCount = m_apProcInfo.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_apProcInfo[i];
		dwTimeTmp = dwTime - pInfo->dwProcSetTime;
		if (dwTimeTmp < pInfo->dwProcStartTime) {
			continue;
		}

		switch (pInfo->dwProcID) {
		case CHARPROCID_FISHING:		/* 釣り */
			bResult = ProcFISHING (pInfo->dwPara);
			break;
		case CHARPROCID_FISHING_HIT:	/* 釣り(ヒット) */
			bResult = ProcFISHING_HIT (pInfo->dwPara);
			break;
		case CHARPROCID_MAPMOVEIN:		/* マップ内移動 */
			bResult = ProcMAPMOVEIN (pInfo->dwPara);
			break;
		case CHARPROCID_MAPMOVEOUT:		/* マップ外移動 */
			bResult = ProcMAPMOVEOUT (pInfo->dwPara);
			break;
		case CHARPROCID_SWOON:			/* 気絶 */
			bResult = ProcSWOON (pInfo->dwPara);
			break;
		case CHARPROCID_INVINCIBLE:		/* 無敵 */
			bResult = ProcINVINCIBLE (pInfo->dwPara);
			break;
		}
		if (bResult) {
			DeleteProcInfo (i);
			break;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::ProcFISHING										 */
/* 内容		:行動処理(釣り)													 */
/* 日付		:2008/01/01														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcFISHING(DWORD dwPara)
{
	int nState;

	nState = CHARMOVESTATE_STAND;
	if (IsStateBattle ()) {
		nState = CHARMOVESTATE_BATTLE;
	}

	/* 釣れなかった */
	if ((genrand () % 100) < 60) {
//Todo:
		SetEfcBalloon (8);
		SetProcState (CHARPROCSTATEID_NORMAL);
		SetMoveState (nState);
	} else {
		SetMotion (CHARMOTIONLISTID_FISHING_HIT_UP);
		AddProcInfo (CHARPROCID_FISHING_HIT, 5000, 0);
	}

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::ProcFISHING_HIT									 */
/* 内容		:行動処理(釣り(ヒット))											 */
/* 日付		:2008/01/02														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcFISHING_HIT(DWORD dwPara)
{
	int nState;

	nState = CHARMOVESTATE_STAND;
	if (IsStateBattle ()) {
		nState = CHARMOVESTATE_BATTLE;
	}

	/* 釣れなかった */
	if ((genrand () % 100) < 60) {
//Todo:
		SetEfcBalloon (8);

	} else {
		SetEfcBalloon (2);
		m_bChgFishingHit = TRUE;
	}
	SetMotion (-1);
	SetMoveState (nState);
	SetProcState (CHARPROCSTATEID_NORMAL);
	DeleteProcInfo (CHARPROCID_FISHING);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::ProcMAPMOVEIN									 */
/* 内容		:行動処理(マップ内移動)											 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcMAPMOVEIN(DWORD dwPara)
{
	m_bProcMoveMapIn = TRUE;
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::ProcMAPMOVEOUT									 */
/* 内容		:行動処理(マップ外移動)											 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcMAPMOVEOUT(DWORD dwPara)
{
	m_bProcMoveMapOut = TRUE;
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::ProcSWOON										 */
/* 内容		:行動処理(気絶)													 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcSWOON(DWORD dwPara)
{
	m_bProcSwoon = TRUE;
	m_nReserveChgEfect = -1;

	/* 記録位置へ戻る？ */
	if (dwPara != 0) {
		m_bProcMoveMarkPos = TRUE;
	}

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::ProcINVINCIBLE									 */
/* 内容		:行動処理(無敵)													 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

BOOL CInfoCharSvr::ProcINVINCIBLE(DWORD dwPara)
{
	m_bProcInvincible = TRUE;
	/* 無敵解除 */
	m_bStatusInvincible = FALSE;
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::DeleteProcInfo									 */
/* 内容		:行動情報を削除													 */
/* 日付		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharSvr::DeleteProcInfo(int nNo)
{
	PCHARPROCINFO pInfo;

	if (nNo >= m_apProcInfo.size()) {
		return;
	}

	pInfo = m_apProcInfo[nNo];
	SAFE_DELETE (pInfo);
	m_apProcInfo.erase (m_apProcInfo.begin () + nNo);
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::DeleteProcInfo									 */
/* 内容		:行動情報を削除													 */
/* 日付		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharSvr::DeleteProcInfo(DWORD dwProcID)
{
	int i, nCount;
	PCHARPROCINFO pInfo;

	nCount = m_apProcInfo.size();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_apProcInfo[i];
		if (pInfo->dwProcID != dwProcID) {
			continue;
		}
		DeleteProcInfo (i);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::DeleteAllProcInfo								 */
/* 内容		:行動情報を全て削除												 */
/* 日付		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharSvr::DeleteAllProcInfo(void)
{
	int i, nCount;

	nCount = m_apProcInfo.size();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteProcInfo (i);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::AddProcInfo										 */
/* 内容		:行動情報を追加													 */
/* 日付		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharSvr::AddProcInfo(
	DWORD dwProcID,			/* [in] 処理ID */
	DWORD dwStartTime,		/* [in] 開始時間 */
	DWORD dwPara)			/* [in] パラメータ */
{
	PCHARPROCINFO pInfo;

	pInfo = new CHARPROCINFO;
	pInfo->dwProcID			= dwProcID;			/* 行動ID */
	pInfo->dwProcSetTime	= timeGetTime ();	/* 処理設定時間 */
	pInfo->dwProcStartTime	= dwStartTime;		/* 処理開始時間 */
	pInfo->dwPara			= dwPara;			/* パラメータ */

	m_apProcInfo.push_back (pInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::IncPutCount										 */
/* 内容		:発生NPC数を増加												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */
void CInfoCharSvr::IncPutCount(void)
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::DecPutCount										 */
/* 内容		:発生NPC数を減少												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */
void CInfoCharSvr::DecPutCount(void)
{
}

/* Copyright(C)URARA-works 2007 */
