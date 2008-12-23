/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:InfoCharCli.cpp											 */
/* 内容			:キャラ情報クライアントクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoEffect.h"
#include "LibInfoEffect.h"
#include "LibInfoEfcBalloon.h"
#include "UraraSockTCPSBO.h"
#include "Packet.h"
#include "Command.h"
#include "Img32.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "InfoCharCli.h"

/* ========================================================================= */
/* 関数名	:CInfoCharCli::CInfoCharCli										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CInfoCharCli::CInfoCharCli()
{
	m_bRedraw				= FALSE;
	m_bChgWait				= FALSE;
	m_bSkipMove				= TRUE;
	m_bWaitCheckMapEvent	= FALSE;
	m_bMotionDirection		= TRUE;
	m_bMotionInterrupt		= FALSE;
	m_bModeSleepTimer		= FALSE;
	m_nAnimeBack			= 0;
	m_nViewState			= INFOCHARCLI_VIEWSTATE_NONE;
	m_nFadeLevel			= 0;
	m_nMoveStateReserve		= 0;
	m_nMoveStateBack		= 0;
	m_dwMoveWaitOnce		= 0;
	m_dwLastTimeAnime		= 0;
	m_dwLastTimeViewState	= 0;
	m_dwBalloonID			= 0;
	m_dwBalloonGrpID		= 0;
	m_dwBalloonAnimeID		= 0;
	m_dwLastTimeBalloon		= 0;
	m_dwLastTimeDamage		= 0;
	m_pMgrData				= NULL;
	m_pDibChar				= NULL;
	m_pDibName				= NULL;
	m_pDibSpeak				= NULL;
	m_pInfoEffect			= NULL;
	m_pSock					= NULL;

	m_ptMove.x = m_ptMove.y = 0;

	m_hFont = CreateFont (12, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "ＭＳ ゴシック");
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::~CInfoCharCli									 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CInfoCharCli::~CInfoCharCli()
{
	DeleteAllTextEffect ();
	DeleteAllMovePosQue ();
	if (m_hFont) {
		DeleteObject (m_hFont);
		m_hFont = NULL;
	}
	SAFE_DELETE (m_pDibChar);
	SAFE_DELETE (m_pDibName);
	SAFE_DELETE (m_pDibSpeak);
	SAFE_DELETE (m_pInfoEffect);
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::Create											 */
/* 内容		:作成															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CInfoCharCli::Create(CMgrData *pMgrData)
{
	m_pMgrData	= pMgrData;
	m_pMgrSound	= m_pMgrData->GetMgrSound ();
	m_pSock		= m_pMgrData->GetUraraSockTCP ();
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::ChgDirection										 */
/* 内容		:向き変更														 */
/* 日付		:2006/10/08														 */
/* ========================================================================= */

void CInfoCharCli::ChgDirection(int nDirection)
{
	switch (m_nMoveState) {
	case CHARMOVESTATE_STAND:			/* 立ち */
	case CHARMOVESTATE_SIT:				/* 座り中 */
	case CHARMOVESTATE_BATTLE:			/* 戦闘中 */
	case CHARMOVESTATE_BATTLE_DEFENSE:	/* 防御中 */
		break;
	default:
		goto Exit;
	}
	m_nDirection = nDirection;

Exit:
	return;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::ChgMoveState										 */
/* 内容		:移動状態変更													 */
/* 日付		:2006/10/08														 */
/* ========================================================================= */

void CInfoCharCli::ChgMoveState(int nMoveState)
{
	BOOL bResult, bChange, bCheck = TRUE;
	int i, nAnime, nCount, nDirection, anInitMovePosX[] = {0, 0, 16, -16, -16, -16, 16, 16}, anInitMovePosY[] = {16, -16, 0, 0, 16, -16, -16, 16};
	PCInfoMotion pInfoMotion;

	if (nMoveState < 0) {
		return;
	}
	if (m_nMoveState == CHARMOVESTATE_DELETEREADY) {
		return;
	}

	bResult = IsStateMove ();
	nAnime	= -1;
	bChange	= (m_nMoveState != nMoveState) ? TRUE : FALSE;

	if (bResult == FALSE) {
		m_ptMove.x = m_ptMove.y = 0;
	}
	pInfoMotion = GetMotionInfo (&nCount);

	if (m_nMoveState == CHARMOVESTATE_SLEEPTIMER) {
		SetBalloonID (0);
	}

	nDirection = m_nDirection;
	if (m_nMoveDirection != -1) {
		nDirection = m_nMoveDirection;
	}

	switch (nMoveState) {
	case CHARMOVESTATE_STAND:		/* 立ち */
	case CHARMOVESTATE_SIT:			/* 座り中 */
	case CHARMOVESTATE_WALKANIME:	/* 足踏み */
		if (bResult) {
			AddMovePosQue (nMoveState, -1, -1, -1);
			return;
		}
		m_nAnimeBack = m_nAnime;
		nAnime = 0;
		m_nMoveDirection = -1;
		RenewBlockMapArea (0, 0, -1);
		/* 溜め攻撃解除 */
		m_bChargeAtack = FALSE;
		bCheck = FALSE;
		break;
	case CHARMOVESTATE_MOVE:		/* 移動中 */
	case CHARMOVESTATE_BATTLEMOVE:	/* 戦闘移動中 */
		if (bChange) {
			nAnime = m_nAnimeBack;
		}
		m_ptMove.x = anInitMovePosX[nDirection];
		m_ptMove.y = anInitMovePosY[nDirection];
		bCheck = FALSE;
		break;
	case CHARMOVESTATE_BATTLE:		/* 戦闘中 */
		if (m_nMoveState == CHARMOVESTATE_BATTLEATACK) {
			if (m_nMoveStateReserve == 0) {
				if (m_nAnime <= nCount - 1 && m_nProcState == 0) {
					/* モーション終わってから遷移するように予約しておく */
					m_nMoveStateReserve = nMoveState;
					return;
				}
			}
		} else {
			m_nAnimeBack = m_nAnime;
		}
		nAnime = 0;
		RenewBlockMapArea (0, 0, -1);
		break;
	case CHARMOVESTATE_BATTLEATACK:		/* 戦闘攻撃中 */
		if (m_nMoveState == nMoveState) {
			break;
		}
		if (m_bChargeAtack) {
			/* 溜め攻撃解除 */
			m_bChargeAtack = FALSE;
			nAnime = 1;
		} else {
			nAnime = 0;
		}
		m_dwLastTimeAnime = timeGetTime ();
		break;
	case CHARMOVESTATE_ANIME:			/* アニメーション */
		nAnime = 0;
		m_dwLastTimeAnime = timeGetTime ();
		break;
	case CHARMOVESTATE_SWOON:			/* 気絶 */
		/* 溜め攻撃解除 */
		m_bChargeAtack = FALSE;
		m_bModeSleepTimer = FALSE;
		nAnime = 0;
		break;
	case CHARMOVESTATE_BATTLE_DEFENSE:	/* 防御中 */
		nAnime = 0;
		if (bChange == FALSE) {
			bCheck = FALSE;
		}
		break;
	case CHARMOVESTATE_DELETEREADY:		/* 消去準備 */
		if (bResult) {
			AddMovePosQue (nMoveState, -1, -1, -1);
			return;
		}
		break;
	default:
		bCheck = FALSE;
		break;
	}
	m_nMoveStateReserve = 0;

	m_nMoveStateBack = nMoveState;
	SetMoveState (nMoveState);

	/* モーションの行動チェックする？ */
	if (bCheck) {
		/* 1コマ目以外から開始の場合に行動をとりこぼすのでチェック */
		for (i = 0; i <= nAnime; i ++) {
			m_nAnime = i;
			RenewAnime (0, 0);
		}
	}
	if (nAnime != -1) {
		m_nAnime = nAnime;
	}

	if (IsStateMove () == FALSE) {
		DeleteAllMovePosQue ();
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CInfoCharCli::TimerProc(DWORD dwTime)
{
	int i, nCount;
	BOOL bRet, bResult, bResultTmp;
	PCInfoTextEffect pInfoTextEffect;

	bRet	= FALSE;
	bResult	= CInfoCharBase::TimerProc (dwTime);

	bResult |= RenewAnime			(dwTime);
	switch (m_nMoveState) {
	case CHARMOVESTATE_MOVE:		/* 移動中 */
	case CHARMOVESTATE_BATTLEMOVE:	/* 戦闘移動中 */
		bResult |= TimerProcMove	(dwTime);
		break;
	case CHARMOVESTATE_BATTLEATACK:	/* 戦闘攻撃中 */
		bResult |= TimerProcAtack	(dwTime);
		break;
	}
	bResult |= TimerProcViewState	(dwTime);
	bResult |= TimerProcBalloon		(dwTime);
	bResult |= m_bRedraw;
	m_bRedraw = FALSE;

	nCount = GetTextEffectCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTextEffect = GetTextEffect (i);
		bResult |= pInfoTextEffect->TimerProc (dwTime);
		if (pInfoTextEffect->m_bAnimeEnd) {
			SAFE_DELETE (pInfoTextEffect);
			m_aTextEffect.RemoveAt (i);
			i --;
			nCount --;
		}
	}

	if (m_pInfoEffect) {
		bResultTmp = m_pInfoEffect->TimerProc (dwTime);
		if (bResultTmp) {
			if ((m_pInfoEffect->m_byAnimeNo == 0) && (m_pInfoEffect->m_bLoopSound)) {
				if (m_pInfoEffect->m_dwSoundID) {
					m_pMgrSound->PlaySound (m_pInfoEffect->m_dwSoundID);
				}
			}
		}
		bResult |= bResultTmp;
		bResultTmp = m_pInfoEffect->IsAnimeEnd ();
		/* アニメーション終了？ */
		if (bResultTmp) {
			/* エフェクト解除 */
			SetEffectID (0);
		}
	}

	if (m_dwLastTimeDamage) {
		if (dwTime - m_dwLastTimeDamage > 500) {
			m_dwLastTimeDamage = 0;
		}
	}

	bRet |= bResult;
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::SetMoveState										 */
/* 内容		:移動状態を変更													 */
/* 日付		:2007/02/26														 */
/* ========================================================================= */

void CInfoCharCli::SetMoveState(int nMoveState)
{
	switch (nMoveState) {
	case CHARMOVESTATE_DAMAGE:			/* ダメージ */
		m_dwLastTimeDamage = timeGetTime ();
		/* 時間の更新だけで状態は変更しない */
		return;
	}

	m_nMoveStateBack = nMoveState;
	CInfoCharBase::SetMoveState (nMoveState);

	/* 消去準備？ */
	if (m_nMoveState == CHARMOVESTATE_DELETEREADY) {
		/* フェードアウトしてから消すようにする */
		SetViewState (INFOCHARCLI_VIEWSTATE_FADEOUT);
	}

}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::MakeCharGrp										 */
/* 内容		:キャラ画像を作成												 */
/* 日付		:2007/03/25														 */
/* ========================================================================= */

void CInfoCharCli::MakeCharGrp(void)
{
	WORD wGrpIDNPC;
	int nTmp, x, y, nHairY, nClothY, nAcceX, nAcceY;
	PCMgrGrpData pMgrGrpData;
	PCImg32 pImgBody, pImgEar, pImgCloth, pImgHairD, pImgHairU, pImgEye, pImgAcce;

	/* 歩き用の画像を用意 */
	SAFE_DELETE (m_pDibChar);
	m_pDibChar = new CImg32;
	m_pDibChar->CreateWithoutGdi (m_nGrpSize * 16, m_nGrpSize);
	m_pDibChar->FillRect (0, 0, m_nGrpSize * 16, m_nGrpSize, RGB (255, 0, 255));
	m_pDibChar->SetColorKey (RGB (255, 0, 255));

	pImgAcce	= NULL;
	pMgrGrpData = m_pMgrData->GetMgrGrpData ();
	pImgBody	= pMgrGrpData->GetDibBody	(m_wFamilyID);
	pImgEar		= pMgrGrpData->GetDibEar	(m_wFamilyID);
	pImgCloth	= pMgrGrpData->GetDibCloth	(m_wGrpIDCloth / 32);
	pImgHairD	= pMgrGrpData->GetDibHairD	(m_wGrpIDHairColor - 1);
	pImgHairU	= pMgrGrpData->GetDibHairU	(m_wGrpIDHairColor - 1);
	pImgEye		= pMgrGrpData->GetDibEye	(m_wGrpIDEyeColor - 1);

//Todo:暫定
	m_ptViewCharPos.x = m_ptViewCharPos.y = 0;
	if (m_dwMotionTypeID != 1) {
		m_ptViewCharPos.x = 16;
		m_ptViewCharPos.y = 32;
	}

	nAcceX = nAcceY = 0;
	if (m_wGrpIDAcce) {
		pImgAcce	= pMgrGrpData->GetDibAcce (m_wGrpIDAcce / 32);
		nAcceY		= (m_wGrpIDAcce % 32) * 16;
	}

	x = y = nHairY = 0;
	if (m_nSex == SEX_FEMALE) {
		x = m_nGrpSize * 16;
	}
	wGrpIDNPC = m_wGrpIDNPC;
	if (wGrpIDNPC >= 50000) {
		wGrpIDNPC -= 50000;
	}
	nClothY = m_wGrpIDCloth % 32;
	if (m_wGrpIDSP > 0) {
		pImgCloth = pMgrGrpData->GetDibSP (0);
		nClothY = m_wGrpIDSP - 1;
		x = 0;
	} else if (wGrpIDNPC > 0) {
		pImgCloth = pMgrGrpData->GetDibNPC (0);
		nClothY = wGrpIDNPC;
		x = 0;
	}
	nClothY *= 16;

	y		= 16;
	nHairY	= m_wGrpIDHairType * m_nGrpSize;

	/* 各向き用の画像を作成 */
	/* 上 */
	if ((m_wGrpIDSP != 0) || (wGrpIDNPC != 0)) {
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x, nClothY, TRUE);
	} else {
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgBody, x, 0, TRUE);
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairD, x, nHairY, TRUE);
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x, nClothY, TRUE);
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgEar, 0, 0, TRUE);
		m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairU, x, nHairY, TRUE);
	}
	m_pDibChar->BltFrom256 (0, 0, m_nGrpSize * 4, m_nGrpSize, pImgAcce, nAcceX, nAcceY, TRUE);

	/* 下 */
	nTmp = 4 * m_nGrpSize;
	if ((m_wGrpIDSP != 0) || (wGrpIDNPC != 0)) {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
	} else {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgBody, x + nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairD, x + nTmp, nHairY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgEar, nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairU, x + nTmp, nHairY, TRUE);
	}
	if (wGrpIDNPC == 0) {
		m_pDibChar->BltFrom256 (nTmp +  0, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 0 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 16, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 0 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 32, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 0 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 48, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 1 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
	}
	m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgAcce, nAcceX + nTmp, nAcceY, TRUE);

	/* 左 */
	nTmp = 8 * m_nGrpSize;
	if ((m_wGrpIDSP != 0) || (wGrpIDNPC != 0)) {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
	} else {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgBody, x + nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairD, x + nTmp, nHairY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgEar, nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairU, x + nTmp, nHairY, TRUE);
	}
	if (wGrpIDNPC == 0) {
		m_pDibChar->BltFrom256 (nTmp +  0, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 2 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 16, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 2 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 32, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 2 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 48, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 3 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
	}
	m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgAcce, nAcceX + nTmp, nAcceY, TRUE);

	/* 右 */
	nTmp = 12 * m_nGrpSize;
	if ((m_wGrpIDSP != 0) || (wGrpIDNPC != 0)) {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
	} else {
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgBody, x + nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairD, x + nTmp, nHairY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgCloth, x + nTmp, nClothY, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgEar, nTmp, 0, TRUE);
		m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgHairU, x + nTmp, nHairY, TRUE);
	}
	if (wGrpIDNPC == 0) {
		m_pDibChar->BltFrom256 (nTmp +  0, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 4 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 16, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 4 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 32, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 4 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
		m_pDibChar->BltFrom256 (nTmp + 48, 0, m_nGrpSize * 1, m_nGrpSize, pImgEye, 5 * m_nGrpSize, m_wGrpIDEye * y, TRUE);
	}
	m_pDibChar->BltFrom256 (nTmp, 0, m_nGrpSize * 4, m_nGrpSize, pImgAcce, nAcceX + nTmp, nAcceY, TRUE);
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::SetName											 */
/* 内容		:キャラ名を更新													 */
/* 日付		:2007/01/28														 */
/* ========================================================================= */

void CInfoCharCli::SetName(LPCSTR pszName)
{
	int i, nCount, nLen, x;
	HFONT hFontOld;
	HDC hDCTmp;
	PCImg32 pDibSystem;

	SAFE_DELETE (m_pDibName);
	CInfoCharBase::SetName (pszName);

	if (pszName == NULL) {
		return;
	}
	pDibSystem = m_pMgrData->GetMgrGrpData ()->GetDibSystem ();

	x		= 0;
	nCount	= m_abyMark.GetSize ();

	m_pDibName = new CImg32;
	m_pDibName->Create (strlen (pszName) * 6 + 2 + (nCount * 16), 16);
	m_pDibName->Clear ();

	hDCTmp = m_pDibName->Lock ();
	hFontOld = (HFONT)SelectObject (hDCTmp, m_hFont);
	SetBkMode (hDCTmp, TRANSPARENT);

	for (i = 0; i < nCount; i ++) {
		m_pDibName->BltFrom256 (x, 1, 16, 16, pDibSystem, 176 + (m_abyMark[i] - 1) * 16, 0, TRUE);
		x += 16;
	}

	nLen = strlen (pszName);
	SetTextColor (hDCTmp, RGB (10, 10, 10));
	TextOut (hDCTmp, x + 0, 2, pszName, nLen);
	TextOut (hDCTmp, x + 2, 2, pszName, nLen);
	TextOut (hDCTmp, x + 1, 1, pszName, nLen);
	TextOut (hDCTmp, x + 1, 3, pszName, nLen);
	SetTextColor (hDCTmp, m_clName);
	TextOut (hDCTmp, x + 1, 2, pszName, nLen);

	SelectObject (hDCTmp, hFontOld);
	m_pDibName->Unlock ();
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::SetSpeak											 */
/* 内容		:発言内容を更新													 */
/* 日付		:2007/02/04														 */
/* ========================================================================= */

void CInfoCharCli::SetSpeak(LPCSTR pszSpeak)
{
	char szTmp[32];
	BOOL bResult;
	int nPos, nLen, nLenTmp, nCount, nWidth, nHeight, nLine;
	HFONT hFontOld;
	HDC hDCTmp;
	CmyString strSpeak1, strSpeak2;

	SAFE_DELETE (m_pDibSpeak);
	CInfoCharBase::SetSpeak (pszSpeak);

	if (pszSpeak == NULL) {
		return;
	}
	nLen = strlen (pszSpeak);
	if (nLen <= 0) {
		return;
	}

	/* 画像作成準備 */
	nPos	= 0;
	nLine	= 0;
	nWidth	= (nLen > 20) ? 20 : nLen;
	nHeight = nLen / 20 + 1;
	if (nLen % 20 == 0) {
		/* 改行丁度の文字数の場合は画像サイズを1行分減らす */
		nHeight --;
		nHeight = max (nHeight, 1);
	}

	m_pDibSpeak = new CImg32;
	m_pDibSpeak->Create ((nWidth + 1) * 6 + 2, nHeight * 14);
	m_pDibSpeak->Clear ();

	hDCTmp = m_pDibSpeak->Lock ();
	hFontOld = (HFONT)SelectObject (hDCTmp, m_hFont);
	SetBkMode (hDCTmp, TRANSPARENT);

	/* 全角10文字単位で1行ずつ画像を作成 */
	while (1) {
		ZeroMemory (szTmp, sizeof (szTmp));
		/* 1行分の文字列を作成 */
		for (nCount = 0; nCount < 20; nCount ++) {
			if (nPos + nCount >= nLen) {
				break;
			}
			szTmp[nCount] = pszSpeak[nPos + nCount];
			bResult = IsDBCSLeadByte (pszSpeak[nPos + nCount]);
			if (bResult) {
				/* 全角文字の先頭バイトの時は後ろの1バイトも追加 */
				nCount ++;
				szTmp[nCount] = pszSpeak[nPos + nCount];
			}
		}
		nLenTmp = strlen (szTmp);
		nPos += nCount;
		if (nLenTmp > 0) {
			SetTextColor (hDCTmp, RGB (10, 10, 10));
			TextOut (hDCTmp, 0, 1 + nLine * 14, szTmp, nLenTmp);
			TextOut (hDCTmp, 2, 1 + nLine * 14, szTmp, nLenTmp);
			TextOut (hDCTmp, 1, 0 + nLine * 14, szTmp, nLenTmp);
			TextOut (hDCTmp, 1, 2 + nLine * 14, szTmp, nLenTmp);
			SetTextColor (hDCTmp, m_clSpeak);
			TextOut (hDCTmp, 1, 1 + nLine * 14, szTmp, nLenTmp);
			nLine ++;
		}

		if (nPos >= nLen) {
			break;
		}
	}

	SelectObject (hDCTmp, hFontOld);
	m_pDibSpeak->Unlock ();
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::SetSleepTimer									 */
/* 内容		:おやすみタイマーを設定											 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

void CInfoCharCli::SetSleepTimer(BOOL bSleepTimer)
{
	CPacketMSGCMD_PARA1 Packet;

	m_bModeSleepTimer = bSleepTimer;
	if (bSleepTimer && (m_dwBalloonID == 0)) {
//Todo:暫定
		Packet.Make (SBOCOMMANDID_SUB_MSGCMD_BALLOON, m_dwCharID, 33);
		m_pSock->Send (&Packet);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::SetViewState										 */
/* 内容		:表示状態設定													 */
/* 日付		:2007/02/26														 */
/* ========================================================================= */

void CInfoCharCli::SetViewState(int nViewState)
{
	m_dwLastTimeViewState	= timeGetTime ();
	m_nViewState			= nViewState;

	switch (m_nViewState) {
	case INFOCHARCLI_VIEWSTATE_NONE:
		m_dwLastTimeViewState	= 0;
		m_nFadeLevel			= 0;
		break;
	case INFOCHARCLI_VIEWSTATE_FADEIN:			/* フェードイン */
		m_nFadeLevel = 99;
		break;
	case INFOCHARCLI_VIEWSTATE_FADEOUT:			/* フェードアウト */
		m_nFadeLevel = 1;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetMotionInfo									 */
/* 内容		:モーション情報を取得											 */
/* 日付		:2007/11/24														 */
/* ========================================================================= */

CInfoMotion *CInfoCharCli::GetMotionInfo(int *pnCount)
{
	int nAnime, nCount, nDirection, nMoveState, anDirection[] = {0, 1, 2, 3, 0, 1, 1, 0};
	PCInfoMotion pRet;
	PARRAYMOTIONINFO paMotionInfo;

	pRet	= NULL;
	nCount	= 0;
	paMotionInfo = NULL;

	nAnime = m_nAnime;
	nDirection = anDirection[m_nDirection];
	if (m_bMotionDirection == FALSE) {
		nDirection = 0;
	}
	nMoveState = m_nMoveState;
	switch (nMoveState) {
	case CHARMOVESTATE_DELETE:		/* 消去 */
	case CHARMOVESTATE_DELETEREADY:	/* 消去準備 */
		nMoveState = m_nMoveStateBack;
		break;
	}

	/* 割り込み再生あり？ */
	if (m_bMotionInterrupt) {
		paMotionInfo = &m_aMotion[CHARMOTIONID_INTERRUUPT][nDirection];

	} else {
		/* 溜め攻撃中？ */
		if (m_bChargeAtack) {
			switch (nMoveState) {
			case CHARMOVESTATE_BATTLEMOVE:	/* 戦闘移動中 */
				break;
			default:
				/* 溜め中は攻撃モーションの1コマ目を表示する */
				nAnime		= 0;
				nMoveState	= CHARMOVESTATE_BATTLEATACK;
				break;
			}
		}

		switch (nMoveState) {
		case CHARMOVESTATE_STAND:		/* 立ち */
		case CHARMOVESTATE_DELETE:		/* 消去 */
		case CHARMOVESTATE_DELETEREADY:	/* 消去準備 */
			paMotionInfo = &m_aMotion[CHARMOTIONID_STAND][nDirection];
			break;
		case CHARMOVESTATE_SIT:			/* 座り中 */
		case CHARMOVESTATE_SWOON:		/* 気絶 */
		case CHARMOVESTATE_SLEEPTIMER:	/* おやすみタイマー中 */
			paMotionInfo = &m_aMotion[CHARMOTIONID_SIT][nDirection];
			break;
		case CHARMOVESTATE_MOVE:		/* 移動中 */
		case CHARMOVESTATE_WALKANIME:	/* 足踏み */
			paMotionInfo = &m_aMotion[CHARMOTIONID_WALK][nDirection];
			break;
		case CHARMOVESTATE_BATTLE:		/* 戦闘中 */
			paMotionInfo = &m_aMotion[CHARMOTIONID_BATTLESTAND][nDirection];
			break;
		case CHARMOVESTATE_BATTLEMOVE:	/* 戦闘移動中 */
			paMotionInfo = &m_aMotion[CHARMOTIONID_BATTLEWALK][nDirection];
			break;
		case CHARMOVESTATE_BATTLEATACK:	/* 戦闘攻撃中 */
			paMotionInfo = &m_aMotion[CHARMOTIONID_ATACK][nDirection];
			break;
		case CHARMOVESTATE_ANIME:		/* アニメーション */
			paMotionInfo = &m_aMotion[CHARMOTIONID_ANIME][nDirection];
			break;
		case CHARMOVESTATE_BATTLE_DEFENSE:	/* 防御 */
			paMotionInfo = &m_aMotion[CHARMOTIONID_BATTLEDEFENSE][nDirection];
			break;
		}
	}
	if (paMotionInfo == NULL) {
		goto Exit;
	}

	nCount = paMotionInfo->GetSize ();
	if (nCount == 0) {
		goto Exit;
	}
	if (nAnime >= nCount) {
		nAnime = 0;
	}
	pRet = paMotionInfo->GetAt (nAnime);

Exit:
	if (pnCount) {
		*pnCount = nCount;
	}
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::IsChgWait										 */
/* 内容		:状態変更待ちか判定												 */
/* 日付		:2007/07/24														 */
/* ========================================================================= */

BOOL CInfoCharCli::IsChgWait(void)
{
	BOOL bRet;

	bRet = TRUE;

	if (m_bChgWait) {
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::SetEffectID										 */
/* 内容		:エフェクトIDを指定												 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CInfoCharCli::SetEffectID(DWORD dwEffectID)
{
	PCLibInfoEffect pLibInfo;
	PCInfoEffect pInfo;

	pLibInfo	= m_pMgrData->GetLibInfoEffect ();
	pInfo		= (PCInfoEffect)pLibInfo->GetPtr (dwEffectID);

	SAFE_DELETE (m_pInfoEffect);
	if (pInfo == NULL) {
		goto Exit;
	}

	m_pInfoEffect = new CInfoEffect;
	m_pInfoEffect->Copy (pInfo);
	if (m_pInfoEffect->m_dwSoundID) {
		m_pMgrSound->PlaySound (m_pInfoEffect->m_dwSoundID);
	}

Exit:
	return;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::SetBalloonID										 */
/* 内容		:噴出しIDを指定													 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

void CInfoCharCli::SetBalloonID(DWORD dwBalloonID)
{
	m_dwLastTimeBalloon = timeGetTime ();
	m_dwBalloonID		= dwBalloonID;
	m_dwBalloonAnimeID	= 0;
	m_dwBalloonGrpID	= 0;
	if (dwBalloonID == 0) {
		m_dwLastTimeBalloon = 0;
		m_dwBalloonID		= 0;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetGrpID											 */
/* 内容		:グラフィックIDを取得											 */
/* 日付		:2007/12/01														 */
/* 備考		:モーション情報のIDから武器画像の場合にIDを変換する				 */
/* ========================================================================= */

void CInfoCharCli::GetGrpID(WORD &wGrpIDMain, WORD &wGrpIDSub)
{
	int nCountX;
	BOOL bInit;
	WORD wGrpIDMainTmp, wGrpIDSubTmp;
	PCMgrGrpData pMgrGrpData;

	pMgrGrpData	= m_pMgrData->GetMgrGrpData ();
	bInit		= TRUE;

	switch (wGrpIDMain) {
	case GRPIDMAIN_WEAPON:			/* 武器 */
	case GRPIDMAIN_WEAPON_BOW:		/* 武器(弓) */
	case GRPIDMAIN_WEAPON_GLOVE:	/* 武器(打撃) */
	case GRPIDMAIN_WEAPON_ETC:		/* 武器(その他) */
	case GRPIDMAIN_2X2_ARMS:		/* 持ち物(2x2) */
		if (m_dwEquipItemIDArmsRight == 0) {
			goto Exit;
		}
		break;
	case GRPIDMAIN_2X2_SHIELD:		/* 盾(2x2) */
		if (m_dwEquipItemIDArmsLeft == 0) {
			goto Exit;
		}
		break;
	}

	wGrpIDMainTmp	= wGrpIDMain;
	wGrpIDSubTmp	= wGrpIDSub;

	switch (wGrpIDMainTmp) {
	case GRPIDMAIN_WEAPON:			/* 武器 */
		wGrpIDMain = GRPIDMAIN_WEAPON;
		break;
	case GRPIDMAIN_WEAPON_BOW:		/* 武器(弓) */
		wGrpIDMain = GRPIDMAIN_WEAPON_BOW;
		break;
	case GRPIDMAIN_WEAPON_GLOVE:	/* 武器(打撃) */
		wGrpIDMain = GRPIDMAIN_WEAPON_GLOVE;
		break;
	case GRPIDMAIN_WEAPON_ETC:		/* 武器(その他) */
		wGrpIDMain = GRPIDMAIN_WEAPON_ETC;
		break;
	case GRPIDMAIN_2X2_ARMS:		/* 持ち物(2x2) */
	case GRPIDMAIN_2X2_SHIELD:		/* 盾(2x2) */
		wGrpIDMain = wGrpIDMainTmp;
		break;
	default:
		break;
	}

	nCountX = pMgrGrpData->GetGrpCountX (wGrpIDMain);

	switch (wGrpIDMainTmp) {
	case GRPIDMAIN_WEAPON:			/* 武器 */
	case GRPIDMAIN_WEAPON_BOW:		/* 武器(弓) */
	case GRPIDMAIN_WEAPON_GLOVE:	/* 武器(打撃) */
	case GRPIDMAIN_WEAPON_ETC:		/* 武器(その他) */
	case GRPIDMAIN_2X2_ARMS:		/* 持ち物(2x2) */
		if ((wGrpIDSub == 0) || (m_wGrpIDArmsSub == 0)) {
			break;
		}
		wGrpIDSub = (wGrpIDSubTmp / nCountX) * nCountX + (wGrpIDSubTmp % nCountX);
		wGrpIDSub += ((m_wGrpIDArmsSub - 1) * nCountX);
		bInit = FALSE;
		break;
	case GRPIDMAIN_2X2_SHIELD:		/* 盾(2x2) */
		if ((wGrpIDSub == 0) || (m_wGrpIDArmsLeftSub == 0)) {
			break;
		}
		wGrpIDSub = (wGrpIDSubTmp / nCountX) * nCountX + (wGrpIDSubTmp % nCountX);
		wGrpIDSub += ((m_wGrpIDArmsLeftSub - 1) * nCountX);
		bInit = FALSE;
		break;
	default:
		bInit = FALSE;
		break;
	}

Exit:
	if (bInit) {
		wGrpIDMain = wGrpIDSub = 0;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::InitMotionInfo									 */
/* 内容		:モーション情報の初期化											 */
/* 日付		:2008/01/03														 */
/* ========================================================================= */

void CInfoCharCli::InitMotionInfo(DWORD dwMotionID)
{
	m_nAnime			= 0;
	m_bMotionInterrupt	= FALSE;
	m_dwLastTimeAnime	= timeGetTime ();

	if (dwMotionID == CHARMOTIONID_INTERRUUPT) {
		m_bMotionInterrupt = TRUE;
		RenewAnime (0, 0);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetEfcGrpPos										 */
/* 内容		:エフェクト画像の開始座標を取得									 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CInfoCharCli::GetEfcGrpPos(POINT &ptDst)
{
	int nSize, nCountX;
	WORD wGrpID;
	PCMgrGrpData pMgrGrpData;

	pMgrGrpData = m_pMgrData->GetMgrGrpData ();

	ptDst.x = ptDst.y = 0;
	nSize	= GetEfcGrpSize ();
	nCountX	= pMgrGrpData->GetGrpCountX (m_pInfoEffect->m_dwGrpIDMain);

	if (m_pInfoEffect == NULL) {
		goto Exit;
	}

//Todo:2枚目以降の対応
	wGrpID = m_pInfoEffect->GetGrpID () - 1;
	ptDst.x = (wGrpID % nCountX) * nSize;
	ptDst.y = (wGrpID / nCountX) * nSize;

Exit:
	return;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetEfcImg										 */
/* 内容		:描画元のエフェクト画像イメージを取得							 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CImg32 *CInfoCharCli::GetEfcImg(void)
{
	PCImg32 pRet;

	pRet = NULL;
	if (m_pInfoEffect == NULL) {
		goto Exit;
	}
//Todo:2枚目以降の指定
	pRet = m_pMgrData->GetMgrGrpData ()->GetDib (m_pInfoEffect->m_dwGrpIDMain, 0, 0);
Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetEfcLevel										 */
/* 内容		:エフェクト画像の透過レベルを取得								 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

int CInfoCharCli::GetEfcLevel(void)
{
	int nRet;

	nRet = 0;
	if (m_pInfoEffect == NULL) {
		goto Exit;
	}
	nRet = m_pInfoEffect->GetLevel ();

Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetEfcGrpSize									 */
/* 内容		:エフェクト画像のサイズを取得									 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

int CInfoCharCli::GetEfcGrpSize(void)
{
	int nRet;

	nRet = m_pMgrData->GetMgrGrpData ()->GetGrpSize (m_pInfoEffect->m_dwGrpIDMain);
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetEfcDrawPos									 */
/* 内容		:エフェクト画像の描画補正幅を取得								 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CInfoCharCli::GetEfcDrawPos(POINT &ptDst)
{
	ptDst.x = -1 * (m_nGrpSize / 2);
	ptDst.y = -1 * (m_nGrpSize / 2);

	switch (m_nMoveState) {
	case CHARMOVESTATE_BATTLE:		/* 戦闘中 */
	case CHARMOVESTATE_BATTLEATACK:	/* 戦闘攻撃中 */
	case CHARMOVESTATE_BATTLEMOVE:	/* 戦闘移動中 */
		ptDst.x = ptDst.y = 0;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetBalloonGrpID									 */
/* 内容		:噴出し画像IDを取得												 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

DWORD CInfoCharCli::GetBalloonGrpID(void)
{
	return m_dwBalloonGrpID;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::IsEnableMove										 */
/* 内容		:移動できる状態か判定											 */
/* 日付		:2008/06/28														 */
/* 戻り値	:TRUE:移動可													 */
/* ========================================================================= */

BOOL CInfoCharCli::IsEnableMove(void)
{
	BOOL bRet;

	bRet = CInfoCharBase::IsEnableMove ();
	if (bRet == FALSE) {
		goto Exit;
	}
	/* マップイベントチェック待ち？ */
	if (m_bWaitCheckMapEvent) {
		bRet = FALSE;
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::IsDamage											 */
/* 内容		:ダメージ受け中か判定											 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

BOOL CInfoCharCli::IsDamage(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_dwLastTimeDamage) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetTextEffectCount								 */
/* 内容		:登録数を取得													 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

int CInfoCharCli::GetTextEffectCount(void)
{
	return m_aTextEffect.GetSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::AddTextEffect									 */
/* 内容		:文字エフェクトを追加											 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

void CInfoCharCli::AddTextEffect(DWORD dwData, int nColor, int nSize)
{
	PCInfoTextEffect pInfo;

	pInfo = new CInfoTextEffect;
//Todo;指定方法
	pInfo->Set (dwData, TEXTEFFECTTYPE_WAVE, nColor, nSize);

	m_aTextEffect.Add (pInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::AddTextEffect									 */
/* 内容		:文字エフェクトを追加											 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

void CInfoCharCli::AddTextEffect(LPCSTR pszData, int nColor, int nSize)
{
	PCInfoTextEffect pInfo;

	pInfo = new CInfoTextEffect;
//Todo;指定方法
	pInfo->Set (pszData, TEXTEFFECTTYPE_NORMAL, nColor, nSize);

	m_aTextEffect.Add (pInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::DeleteAllTextEffect								 */
/* 内容		:文字エフェクトを全て削除										 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

void CInfoCharCli::DeleteAllTextEffect(void)
{
	int i, nCount;
	PCInfoTextEffect pInfo;

	nCount = m_aTextEffect.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aTextEffect[i];
		SAFE_DELETE (pInfo);
	}
	m_aTextEffect.RemoveAll ();
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetTextEffectGrpCount							 */
/* 内容		:画像数を取得													 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

int CInfoCharCli::GetTextEffectGrpCount(int nIndex)
{
	return m_aTextEffect[nIndex]->GetGrpCount ();
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetTextEffectGrpNo								 */
/* 内容		:指定番号の数値を取得											 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

int CInfoCharCli::GetTextEffectGrpNo(int nIndex, int nNo)
{
	return m_aTextEffect[nIndex]->GetGrpNo (nNo);
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetTextEffectDrawPos								 */
/* 内容		:指定番号の表示位置補正値を取得									 */
/* 戻り値	:TRUE:描画する FALSE:描画しない									 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

BOOL CInfoCharCli::GetTextEffectDrawPos(int nIndex, int nNo, POINT &ptDst, BYTE &byLevel)
{
	return m_aTextEffect[nIndex]->GetDrawPos (nNo, ptDst, byLevel);
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetTextEffectData								 */
/* 内容		:指定番号の表示位置補正値を取得									 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

DWORD CInfoCharCli::GetTextEffectData(int nIndex)
{
	return m_aTextEffect[nIndex]->GetData ();
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::GetTextEffect									 */
/* 内容		:文字エフェクトを取得											 */
/* 日付		:2008/07/13														 */
/* ========================================================================= */

CInfoTextEffect *CInfoCharCli::GetTextEffect(int nIndex)
{
	return m_aTextEffect[nIndex];
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::AddMovePosQue									 */
/* 内容		:追加															 */
/* 日付		:2008/08/13														 */
/* ========================================================================= */

void CInfoCharCli::AddMovePosQue(int nState, int nDirection, int x, int y)
{
	PMOVEPOSQUE pInfo;

	if (m_nMoveState == CHARMOVESTATE_DELETEREADY) {
		return;
	}

	pInfo = new MOVEPOSQUE;
	pInfo->nState		= nState;
	pInfo->nDirection	= nDirection;
	pInfo->ptPos.x		= x;
	pInfo->ptPos.y		= y;

	m_apMovePosQue.Add (pInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::DeleteMovePosQue									 */
/* 内容		:削除															 */
/* 日付		:2008/08/13														 */
/* ========================================================================= */

void CInfoCharCli::DeleteMovePosQue(int nNo)
{
	PMOVEPOSQUE pInfo;

	if (nNo >= m_apMovePosQue.GetSize ()) {
		return;
	}
	pInfo = m_apMovePosQue[nNo];
	SAFE_DELETE (pInfo);
	m_apMovePosQue.RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::DeleteAllMovePosQue								 */
/* 内容		:全て削除														 */
/* 日付		:2008/08/13														 */
/* ========================================================================= */

void CInfoCharCli::DeleteAllMovePosQue(void)
{
	int i, nCount;

	nCount = m_apMovePosQue.GetSize ();
	for (i = 0; i < nCount; i ++) {
		DeleteMovePosQue (0);
	}
	m_apMovePosQue.RemoveAll ();
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::RenewAnime										 */
/* 内容		:アニメーションの更新											 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

BOOL CInfoCharCli::RenewAnime(
	DWORD dwTime,		/* [in] 時間 */
	int nAdd/*=1*/)		/* [in] 次のパターンへの増加分 */
{
	BOOL bRet, bCheck;
	int nCount;
	DWORD dwTmp, dwWait;
	PCInfoMotion pInfoMotion;

	bRet = FALSE;

	pInfoMotion = GetMotionInfo (&nCount);
	if (pInfoMotion == NULL) {
		goto Exit;
	}
	if (pInfoMotion->m_byWait == 0) {
		/* 待ち時間未設定の場合はコマ移動しない */
		goto Exit;
	}

	if (nAdd != 0) {
		dwTmp  = dwTime - m_dwLastTimeAnime;
		dwWait = (DWORD)pInfoMotion->m_byWait * 10;
		if (dwTmp < dwWait) {
			goto Exit;
		}
	}

	bCheck = FALSE;
	if (m_nAnime + nAdd >= nCount) {
		if (pInfoMotion->m_bLoop) {
			bCheck = TRUE;
			m_nAnime = 0;
			m_dwLastTimeAnime = dwTime;
		} else {
			switch (pInfoMotion->m_dwMotionListID) {
			case CHARMOTIONLISTID_DEAD_UP:			/* 死亡(上) */
			case CHARMOTIONLISTID_DEAD_DOWN:		/* 死亡(下) */
			case CHARMOTIONLISTID_DEAD_LEFT:		/* 死亡(左) */
			case CHARMOTIONLISTID_DEAD_RIGHT:		/* 死亡(右) */
				m_dwLastTimeAnime = dwTime;
				/* 死亡モーション後はそのままにしておく */
				break;
			default:
				if (m_bMotionInterrupt) {
					m_nAnime = 0;
				}
				m_bMotionInterrupt = FALSE;
				if (m_nMoveStateReserve) {
					/* 予約していた状態へ遷移 */
					ChgMoveState (m_nMoveStateReserve);
				}
				break;
			}
		}

	} else {
		m_nAnime += nAdd;
		bCheck = TRUE;
		if (nAdd != 0) {
			m_dwLastTimeAnime = dwTime;
		}
	}
	if (bCheck || (nAdd == 0)) {
		pInfoMotion = GetMotionInfo (&nCount);
		if (pInfoMotion) {
			if (pInfoMotion->m_dwSoundID) {
				m_pMgrSound->PlaySound (pInfoMotion->m_dwSoundID);
			}
			if (pInfoMotion->m_dwProcID) {
				MotionProc (pInfoMotion->m_dwProcID);
			}
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::TimerProcMove									 */
/* 内容		:移動処理														 */
/* 日付		:2006/10/08														 */
/* ========================================================================= */

BOOL CInfoCharCli::TimerProcMove(DWORD dwTime)
{
	BOOL bRet;
	int nStandState, nDirection;
	DWORD dwTmp, dwWait;
	PMOVEPOSQUE pQue;

	bRet		= FALSE;
	dwWait		= GetMoveWait ();
	if (m_dwMoveWaitOnce != 0) {
		dwWait = m_dwMoveWaitOnce;
	}
	if (m_bSkipMove) {
		if (m_apMovePosQue.GetSize () > 0) {
			/* キューに積まれている場合は待ち時間無し */
			dwWait = 1;
		}
	}
	nStandState	= CHARMOVESTATE_STAND;

	switch (m_nMoveState) {
	case CHARMOVESTATE_MOVE:		/* 移動中 */
		break;
	case CHARMOVESTATE_BATTLEMOVE:	/* 戦闘移動中 */
		nStandState = CHARMOVESTATE_BATTLE;
		break;
	default:
		goto Exit;
	}
	if (m_bChargeAtack) {
		dwWait *= 2;
	}

	nDirection = m_nDirection;
	if (m_nMoveDirection != -1) {
		nDirection = m_nMoveDirection;
	}

	dwTmp = dwTime - m_dwLastTimeMove;
	if (m_dwLastTimeMove == 0) {
		m_dwLastTimeMove = dwTime;
		goto Exit;
	}

	dwWait = max (dwWait, 1);
	dwTmp /= dwWait;

	switch (nDirection) {
	case 0:		/* 上 */
		m_ptMove.y -= dwTmp;
		m_ptMove.y = max (0, m_ptMove.y);
		break;
	case 1:		/* 下 */
		m_ptMove.y += dwTmp;
		m_ptMove.y = min (0, m_ptMove.y);
		break;
	case 2:		/* 左 */
		m_ptMove.x -= dwTmp;
		m_ptMove.x = max (0, m_ptMove.x);
		break;
	case 3:		/* 右 */
		m_ptMove.x += dwTmp;
		m_ptMove.x = min (0, m_ptMove.x);
		break;
	case 4:
		m_ptMove.y -= dwTmp;
		m_ptMove.y = max (0, m_ptMove.y);
		m_ptMove.x += dwTmp;
		m_ptMove.x = min (0, m_ptMove.x);
		break;
	case 5:
		m_ptMove.y += dwTmp;
		m_ptMove.y = min (0, m_ptMove.y);
		m_ptMove.x += dwTmp;
		m_ptMove.x = min (0, m_ptMove.x);
		break;
	case 6:
		m_ptMove.y += dwTmp;
		m_ptMove.y = min (0, m_ptMove.y);
		m_ptMove.x -= dwTmp;
		m_ptMove.x = max (0, m_ptMove.x);
		break;
	case 7:
		m_ptMove.y -= dwTmp;
		m_ptMove.y = max (0, m_ptMove.y);
		m_ptMove.x -= dwTmp;
		m_ptMove.x = max (0, m_ptMove.x);
		break;
	}

	if ((dwTmp > 0) && (!((m_ptMove.x == 0) && (m_ptMove.y == 0)))) {
		m_dwLastTimeMove += (dwTmp * dwWait);
	}
	if ((m_ptMove.x == 0) && (m_ptMove.y == 0)) {
		if (m_pMgrData->GetCharID () != m_dwCharID) {
			if (dwTime - m_dwLastTimeMove < 250) {
				if (m_apMovePosQue.GetSize () == 0) {
					goto Exit;
				}
			}
		}
		m_nAnimeBack = m_nAnime;
		m_nMoveState = 0;
		if (m_apMovePosQue.GetSize () > 0) {
			pQue = m_apMovePosQue[0];
			nStandState = pQue->nState;
			if (pQue->ptPos.x >= 0) {
				SetPos (pQue->ptPos.x, pQue->ptPos.y);
				SetDirection (pQue->nDirection);
				CInfoCharCli::ChgMoveState (nStandState);
				nStandState = -1;
			}
			DeleteMovePosQue (0);
		}
		m_dwMoveWaitOnce = 0;
		ChgMoveState (nStandState);
	}
	if (dwTmp == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::TimerProcViewState								 */
/* 内容		:表示状態処理													 */
/* 日付		:2007/02/26														 */
/* ========================================================================= */

BOOL CInfoCharCli::TimerProcViewState(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTmp;

	bRet	= FALSE;
	dwTmp	= dwTime - m_dwLastTimeViewState;
	dwTmp	/= 10;

	if (dwTmp == 0) {
		goto Exit;
	}
	m_dwLastTimeViewState = dwTime;

	switch (m_nViewState) {
	case INFOCHARCLI_VIEWSTATE_NONE:
		goto Exit;
	case INFOCHARCLI_VIEWSTATE_FADEIN:			/* フェードイン */
		m_nFadeLevel -= dwTmp;
		if (m_nFadeLevel < 0) {
			SetViewState (INFOCHARCLI_VIEWSTATE_NONE);
		}
		break;
	case INFOCHARCLI_VIEWSTATE_FADEOUT:			/* フェードアウト */
		m_nFadeLevel += dwTmp;
		if (m_nFadeLevel > 100) {
			m_nFadeLevel = 100;
			if ((m_pInfoEffect == NULL) || (m_pInfoEffect && m_pInfoEffect->m_bLoop)) {
				SetViewState (INFOCHARCLI_VIEWSTATE_NONE);
				/* 消去準備？ */
				if (m_nMoveState == CHARMOVESTATE_DELETEREADY) {
					SetMoveState (CHARMOVESTATE_DELETE);
				} else {
					SetViewState (INFOCHARCLI_VIEWSTATE_FADEIN);
				}
			}
		}
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::TimerProcAtack									 */
/* 内容		:攻撃処理														 */
/* 日付		:2007/07/12														 */
/* ========================================================================= */

BOOL CInfoCharCli::TimerProcAtack(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTmp, dwWait;
	int nCount;
	PCInfoMotion pInfoMotion;

	bRet = FALSE;
	pInfoMotion = GetMotionInfo (&nCount);
	if (pInfoMotion) {
		if (m_nAnime < nCount - 1) {
			goto Exit;
		}
		dwTmp  = dwTime - m_dwLastTimeAnime;
		dwWait = (DWORD)pInfoMotion->m_byWait * 10;
		/* 最終コマの時間の2倍の間表示した？ */
		if (dwTmp < dwWait * 2) {
			goto Exit;
		}
	}
	ChgMoveState (CHARMOVESTATE_BATTLE);
	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::TimerProcBalloon									 */
/* 内容		:噴出し処理														 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

BOOL CInfoCharCli::TimerProcBalloon(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTimeTmp;
	PCInfoEfcBalloon pInfo;
	PCLibInfoEfcBalloon pLibInfo;

	bRet = FALSE;
	if (m_dwBalloonID == 0) {
		goto Exit;
	}

	pLibInfo = m_pMgrData->GetLibInfoEfcBalloon ();
	pInfo	 = pLibInfo->GetPtr (m_dwBalloonID, m_dwBalloonAnimeID);
	if (pInfo == NULL) {
		goto Exit;
	}

	/* 初回？ */
	if (m_dwBalloonGrpID == 0) {
		m_dwBalloonGrpID	= pInfo->m_dwGrpID;
		m_dwLastTimeBalloon	= dwTime;
		if (pInfo->m_dwSoundID != 0) {
			m_pMgrSound->PlaySound (pInfo->m_dwSoundID);
		}
		bRet = TRUE;
		goto Exit;
	}

	dwTimeTmp = m_dwLastTimeBalloon + (DWORD)pInfo->m_byWait * 10;
	if (dwTime < dwTimeTmp) {
		goto Exit;
	}
	m_dwBalloonAnimeID = pLibInfo->GetNextAnimeID (m_dwBalloonID, m_dwBalloonAnimeID);
	if (m_dwBalloonAnimeID == -1) {
		m_dwBalloonID		= 0;
		m_dwBalloonAnimeID	= 0;
		m_dwLastTimeBalloon = 0;
		m_dwBalloonGrpID	= 0;

	} else {
		pInfo = pLibInfo->GetPtr (m_dwBalloonID, m_dwBalloonAnimeID);
		if (pInfo == NULL) {
			goto Exit;
		}
		m_dwLastTimeBalloon = dwTime;
		m_dwBalloonGrpID	= pInfo->m_dwGrpID;
		if (pInfo->m_dwSoundID != 0) {
			m_pMgrSound->PlaySound (pInfo->m_dwSoundID);
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharCli::MotionProc										 */
/* 内容		:モーション処理													 */
/* 日付		:2008/01/01														 */
/* ========================================================================= */

void CInfoCharCli::MotionProc(DWORD dwProcID)
{
	CPacketBATTLE_REQ_ATACK PacketBATTLE_REQ_ATACK;

	if (dwProcID == 0) {
		return;
	}
	if (m_pMgrData->GetCharID () != m_dwCharID) {
		return;
	}
	if (m_bChargeAtack) {
		return;
	}

	switch (dwProcID) {
	case CHARMOTIONPROCID_ATACK:			/* 攻撃 */
		PacketBATTLE_REQ_ATACK.Make (m_dwCharID, 0);
		m_pSock->Send (&PacketBATTLE_REQ_ATACK);
		break;
	case CHARMOTIONPROCID_FISHING:			/* 釣り */
		{
			CPacketCHAR_PARA1 Packet;

			Packet.Make (SBOCOMMANDID_SUB_CHAR_PROC_FISHING, m_dwCharID, 0);
			m_pSock->Send (&Packet);
			SetProcState (CHARPROCSTATEID_FISHING);
		}
		break;
	}
}

/* Copyright(C)URARA-works 2006 */
