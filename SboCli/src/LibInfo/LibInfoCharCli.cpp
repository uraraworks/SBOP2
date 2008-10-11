/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LibInfoCharCli.cpp											 */
/* 内容			:キャラ情報ライブラリクライアントクラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMotion.h"
#include "LibInfoItem.h"
#include "InfoMapBase.h"
#include "InfoItemTypeBase.h"
#include "InfoMotion.h"
#include "InfoCharCli.h"
#include "InfoCharBALLCli.h"
#include "InfoCharSCORECli.h"
#include "InfoCharATACKANIMECli.h"
#include "InfoCharMOVEATACKCli.h"
#include "MgrData.h"
#include "LibInfoCharCli.h"


/* ========================================================================= */
/* 関数名	:CLibInfoCharCli::CLibInfoCharCli								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CLibInfoCharCli::CLibInfoCharCli()
{
	m_nProcNo		= 0;
	m_pMainFrame	= NULL;
	m_pMgrData		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharCli::~CLibInfoCharCli								 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CLibInfoCharCli::~CLibInfoCharCli()
{
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharCli::Create										 */
/* 内容		:作成															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharCli::Create(CMgrData *pMgrData)
{
	CLibInfoCharBase::Create ();

	m_pMgrData		= pMgrData;
	m_pMainFrame	= m_pMgrData->GetMainFrame ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharCli::Proc											 */
/* 内容		:処理															 */
/* 戻り値	:TRUE:処理した FALS:処理していない								 */
/* 日付		:2007/01/24														 */
/* ========================================================================= */

BOOL CLibInfoCharCli::Proc(void)
{
	BOOL bRet, bResult;
	int i, nCount;
	PCInfoCharCli pInfoChar;

	bRet = FALSE;

	nCount = GetCount ();
	if (nCount <= 0) {
		goto Exit;
	}
	for (i = nCount - 1; i >= 0; i --) {
		m_nProcNo = i;
		if (m_nProcNo >= nCount) {
			m_nProcNo = 0;
		}

		pInfoChar = (PCInfoCharCli)GetPtr (m_nProcNo);
		m_nProcNo ++;
		if (pInfoChar == NULL) {
			goto Exit;
		}

		bResult = FALSE;
		switch (pInfoChar->m_nMoveState) {
		case CHARMOVESTATE_DELETE:			/* 消去 */
			if (pInfoChar == m_pMgrData->GetPlayerChar ()) {
				m_pMgrData->SetPlayerChar (NULL);
			}
			Delete (pInfoChar->m_dwCharID);
			PostMessage (m_pMgrData->GetMainWindow (), WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARCOUNT, GetCount ());
			m_nProcNo --;
			bResult = TRUE;
			break;
//		case CHARMOVESTATE_STAND:			/* 立ち */
//		case CHARMOVESTATE_MOVE:			/* 移動中 */
//		case CHARMOVESTATE_DELETEREADY:		/* 消去準備 */
		default:
			bResult = pInfoChar->TimerProc (timeGetTime ());
			break;
		}
		bRet |= bResult;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharCli::DeleteOutScreen								 */
/* 内容		:画面外のキャラを削除											 */
/* 戻り値	:TRUE:削除された												 */
/* 日付		:2007/02/12														 */
/* ========================================================================= */

BOOL CLibInfoCharCli::DeleteOutScreen(CInfoCharCli *pChar, BOOL bDelete/*FALSE*/)
{
	BOOL bRet;
	int i, nCount, nState;
	PCInfoCharCli pCharTmp;

	bRet = FALSE;
	if (pChar == NULL) {
		return bRet;
	}

	nState = CHARMOVESTATE_DELETEREADY;
	if (bDelete) {
		nState = CHARMOVESTATE_DELETE;
	}

	nCount = GetCount ();
	for (i = nCount - 1; i >= 0; i --) {
		pCharTmp = (PCInfoCharCli)GetPtr (i);
		if (pCharTmp == pChar) {
			continue;
		}
		/* 同マップ？ */
		if (pCharTmp->m_dwMapID == pChar->m_dwMapID) {
			/* 画面内？ */
			if ((abs (pCharTmp->m_nMapX - pChar->m_nMapX) < (DRAW_PARTS_X * 2) + 1) &&
				(abs (pCharTmp->m_nMapY - pChar->m_nMapY) < (DRAW_PARTS_Y * 2) + 1)) {
				continue;
			}
		}
		pCharTmp->ChgMoveState (nState);
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharCli::RenewMotionInfo								 */
/* 内容		:モーション情報を更新											 */
/* 日付		:2007/11/24														 */
/* ========================================================================= */

void CLibInfoCharCli::RenewMotionInfo(DWORD dwCharID)
{
	int i, nCount;
	PCInfoCharCli pChar;

	if (dwCharID) {
		pChar = (PCInfoCharCli)GetPtr (dwCharID);
		RenewMotionInfo (pChar);

	} else {
		nCount = GetCount ();
		for (i = 0; i < nCount; i ++) {
			pChar = (PCInfoCharCli)GetPtr (i);
			RenewMotionInfo (pChar);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharCli::RenewMotionInfo								 */
/* 内容		:モーション情報を更新											 */
/* 日付		:2007/11/24														 */
/* ========================================================================= */

void CLibInfoCharCli::RenewMotionInfo(CInfoCharCli *pChar)
{
	int i;
	DWORD dwMotionID;

	if (pChar == NULL) {
		return;
	}

	for (i = 0; i < CHARMOTIONID_MAX; i ++) {
		dwMotionID = pChar->m_adwMotionID[i];
		SetMotionInfo (pChar, i, dwMotionID);
	}
	pChar->InitMotionInfo (-1);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharCli::SetMotionInfo									 */
/* 内容		:モーション情報を設定											 */
/* 日付		:2008/01/03														 */
/* ========================================================================= */

void CLibInfoCharCli::SetMotionInfo(
	CInfoCharCli *pChar,		/* [in] キャラ情報 */
	DWORD dwMotionID,			/* [in] モーションID */
	DWORD dwListID)				/* [in] モーションリストID */
{
	int i;
	DWORD dwMotionType;
	PCLibInfoMotion pLibInfoMotion;
	PCLibInfoItem pLibInfoItem;

	pLibInfoMotion	= m_pMgrData->GetLibInfoMotion ();
	pLibInfoItem	= m_pMgrData->GetLibInfoItem ();

	switch (dwMotionID) {
	case CHARMOTIONID_ATACK:			// 攻撃
		dwListID	 = CHARMOTIONLISTID_SWING_UP;
		dwMotionType = pLibInfoItem->GetMotionIDAtack (pChar->m_dwEquipItemIDArmsRight);
//Todo:暫定
		if ((dwMotionType & INFOITEMARMS_MOTION_SWING) && (dwMotionType & INFOITEMARMS_MOTION_POKE)) {
			if (rand () % 100 > 50) {
				dwListID = CHARMOTIONLISTID_SWING_UP;
			} else {
				dwListID = CHARMOTIONLISTID_POKE_UP;
			}
		} else {
			if (dwMotionType & INFOITEMARMS_MOTION_SWING) {
				dwListID = CHARMOTIONLISTID_SWING_UP;
			}
			if (dwMotionType & INFOITEMARMS_MOTION_POKE) {
				dwListID = CHARMOTIONLISTID_POKE_UP;
			}
		}
		if (dwMotionType & INFOITEMARMS_MOTION_BOW) {
			dwListID = CHARMOTIONLISTID_BOW_UP;
		}
		if ((dwMotionType == 0) || (dwMotionType & INFOITEMARMS_MOTION_BLOW)) {
			dwListID = CHARMOTIONLISTID_GLOVE_UP;
		}
		if (dwMotionType & INFOITEMARMS_MOTION_FISHING) {
			dwListID = CHARMOTIONLISTID_FISHING_UP;
		}
		break;
	case CHARMOTIONID_BATTLESTAND:		// 戦闘立ち
		dwListID = pLibInfoItem->GetMotionIDBattleStand (pChar->m_dwEquipItemIDArmsRight);
		if (dwListID == 0) {
			dwListID = CHARMOTIONLISTID_BATTLESTAND_UP;
		}
		break;
	case CHARMOTIONID_BATTLEWALK:		// すり足
		dwListID = pLibInfoItem->GetMotionIDBattleWalk (pChar->m_dwEquipItemIDArmsRight);
		if (dwListID == 0) {
			dwListID = CHARMOTIONLISTID_BATTLEWALK_UP;
		}
		break;
	case CHARMOTIONID_BATTLEDEFENSE:	// 防御
		dwListID = CHARMOTIONLISTID_DEFENSE_UP;
		dwMotionType = pLibInfoItem->GetMotionIDAtack (pChar->m_dwEquipItemIDArmsRight);
		if (dwMotionType & INFOITEMARMS_MOTION_BOW) {
			dwListID = CHARMOTIONLISTID_BOWDEFENSE_UP;
		}
		break;
	}
	/* 向きに応じてモーションを切り替える？ */
	if (pChar->m_bMotionDirection) {
		for (i = 0; i < 4; i ++) {
			pLibInfoMotion->GetMotionInfo (pChar->m_dwMotionTypeID, dwListID + i, pChar->m_aMotion[dwMotionID][i]);
		}
	} else {
		pLibInfoMotion->GetMotionInfo (pChar->m_dwMotionTypeID, dwListID, pChar->m_aMotion[dwMotionID][0]);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::RenewGrpID									 */
/* 内容		:装備画像IDを更新												 */
/* 日付		:2007/12/22														 */
/* ========================================================================= */

void CLibInfoCharCli::RenewGrpID(DWORD dwCharID)
{
	int i, nCount;
	PCInfoCharBase pInfoChar;
	PCInfoItemTypeBase pInfoItemType;
	PCLibInfoItem pLibInfoItem;

	pLibInfoItem = m_pMgrData->GetLibInfoItem ();

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (dwCharID == 0) {
			pInfoChar = (PCInfoCharBase)GetPtr (i);
		} else {
			pInfoChar = (PCInfoCharBase)GetPtr (dwCharID);
			if (pInfoChar == NULL) {
				break;
			}
			i = nCount;
		}

		/* 服 */
		pInfoItemType = (PCInfoItemTypeBase)pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDCloth);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDCloth, &pInfoChar->m_wGrpIDSP);
		}
		/* アクセサリ */
		pInfoItemType = (PCInfoItemTypeBase)pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDAcce1);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDAcce, NULL);
		}
		/* 持ち物 */
		pInfoItemType = (PCInfoItemTypeBase)pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsRight);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDArmsMain, &pInfoChar->m_wGrpIDArmsSub);
		}
		/* 盾 */
		pInfoItemType = (PCInfoItemTypeBase)pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsLeft);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDArmsLeftMain, &pInfoChar->m_wGrpIDArmsLeftSub);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharCli::IsMove										 */
/* 内容		:指定方向に進めるかチェック										 */
/* 日付		:2008/06/03														 */
/* 戻り値	:TRUE:進める													 */
/* ========================================================================= */

BOOL CLibInfoCharCli::IsMove(
	PCInfoCharBase pInfoChar,		/* [in] キャラ情報 */
	int &nDirection)				/* [in/ou] 移動する向き */
{
	int i, nCount, nDirectionTmp;
	BOOL bRet, bResult;
	PCInfoMapBase pInfoMap;
	POINT ptFront, ptBack;

	bRet = FALSE;
	pInfoMap = m_pMgrData->GetMap ();
	if (pInfoMap == NULL) {
		goto Exit;
	}

	ptBack.x = pInfoChar->m_nMapX;
	ptBack.y = pInfoChar->m_nMapY;

	switch (nDirection) {
	case 4:
		nDirectionTmp = 0;
		bRet = IsMove (pInfoChar, nDirectionTmp);
		if (bRet == FALSE) {
			nDirection = 3;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 3;
			bRet = IsMove (pInfoChar, nDirectionTmp);
			pInfoChar->m_nMapX = ptBack.x;
			pInfoChar->m_nMapY = ptBack.y;
			if (bRet) {
				return TRUE;
			}
			nDirection = 0;
			return TRUE;
		}
		break;
	case 5:
		nDirectionTmp = 1;
		bRet = IsMove (pInfoChar, nDirectionTmp);
		if (bRet == FALSE) {
			nDirection = 3;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 3;
			bRet = IsMove (pInfoChar, nDirectionTmp);
			pInfoChar->m_nMapX = ptBack.x;
			pInfoChar->m_nMapY = ptBack.y;
			if (bRet) {
				return TRUE;
			}
			nDirection = 1;
			return TRUE;
		}
		break;
	case 6:
		nDirectionTmp = 1;
		bRet = IsMove (pInfoChar, nDirectionTmp);
		if (bRet == FALSE) {
			nDirection = 2;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 2;
			bRet = IsMove (pInfoChar, nDirectionTmp);
			pInfoChar->m_nMapX = ptBack.x;
			pInfoChar->m_nMapY = ptBack.y;
			if (bRet) {
				return TRUE;
			}
			nDirection = 1;
			return TRUE;
		}
		break;
	case 7:
		nDirectionTmp = 0;
		bRet = IsMove (pInfoChar, nDirectionTmp);
		if (bRet == FALSE) {
			nDirection = 2;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 2;
			bRet = IsMove (pInfoChar, nDirectionTmp);
			pInfoChar->m_nMapX = ptBack.x;
			pInfoChar->m_nMapY = ptBack.y;
			if (bRet) {
				return TRUE;
			}
			nDirection = 0;
			return TRUE;
		}
		break;
	}

	/* 脱出可能かチェック */
	bResult = FALSE;
	pInfoChar->RenewBlockMapArea (pInfoChar->m_nMapX, pInfoChar->m_nMapY, nDirection, TRUE);
	nCount = pInfoChar->m_aposBockMapArea.GetSize ();
	for (i = 0; i < nCount; i ++) {
		bResult |= !pInfoMap->IsMoveOut (pInfoChar->m_aposBockMapArea[i].x, pInfoChar->m_aposBockMapArea[i].y, nDirection);
	}
	bResult = !bResult;
	if (bResult == FALSE) {
		goto Exit;
	}

	/* 進めるかチェック */
	bResult = FALSE;
	pInfoChar->GetFrontPos (ptFront, nDirection, TRUE);
	pInfoChar->RenewBlockMapArea (ptFront.x, ptFront.y, nDirection);
	nCount = pInfoChar->m_aposBockMapArea.GetSize ();
	for (i = 0; i < nCount; i ++) {
		bResult |= !pInfoMap->IsMove (pInfoChar->m_aposBockMapArea[i].x, pInfoChar->m_aposBockMapArea[i].y, nDirection);
	}
	bResult = !bResult;
	if (bResult == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CInfoBase *CLibInfoCharCli::GetNew(int nType)
{
	PCInfoCharCli pNew;

	switch (nType) {
	case CHARMOVETYPE_BALL:			pNew = (PCInfoCharCli)new CInfoCharBALLCli;			break;	/* ボール */
	case CHARMOVETYPE_SCORE:		pNew = (PCInfoCharCli)new CInfoCharSCORECli;		break;	/* 得点 */
	case CHARMOVETYPE_ATACKANIME:	pNew = (PCInfoCharCli)new CInfoCharATACKANIMECli;	break;	/* 攻撃受けるとアニメーション */
	case CHARMOVETYPE_MOVEATACK:	pNew = (PCInfoCharCli)new CInfoCharMOVEATACKCli;	break;	/* 移動して攻撃 */
	default:						pNew = (PCInfoCharCli)new CInfoCharCli;				break;
	}
	pNew->Create (m_pMgrData);
	RenewMotionInfo (pNew);

	return (PCInfoBase)pNew;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetPtr										 */
/* 内容		:キャラ情報を取得												 */
/* 日付		:2007/03/21														 */
/* ========================================================================= */

CInfoBase *CLibInfoCharCli::GetPtr(int nNo)
{
	return CLibInfoCharBase::GetPtr (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetNew										 */
/* 内容		:キャラ情報を取得												 */
/* 日付		:2007/03/21														 */
/* ========================================================================= */

CInfoBase *CLibInfoCharCli::GetPtr(DWORD dwCharID)
{
	return CLibInfoCharBase::GetPtr (dwCharID);
}

/* Copyright(C)URARA-works 2006 */
