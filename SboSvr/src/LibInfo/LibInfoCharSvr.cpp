/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoCharSvr.cpp											 */
/* 内容			:キャラ情報ライブラリクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/21													 */
/* ========================================================================= */

#include "stdafx.h"
#include <math.h>
#include "SBOGlobal.h"
#include "UraraSockTCPSBO.h"
#include "Packet.h"
#include "Command.h"
#include "InfoMapParts.h"
#include "LibInfoMapBase.h"
#include "LibInfoItemType.h"
#include "LibInfoItem.h"
#include "LibInfoItemWeapon.h"
#include "LibInfoSystem.h"
#include "InfoSystem.h"
#include "InfoMapEvent.h"
#include "InfoCharATACKANIMESvr.h"
#include "InfoCharMOVEATACKSvr.h"
#include "InfoCharBATTLE1Svr.h"
#include "InfoCharBATTLE2Svr.h"
#include "InfoCharPUTNPC.h"
#include "InfoTextEffect.h"
#include "MainFrame.h"
#include "MgrData.h"
#include "LibInfoCharSvr.h"


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::CLibInfoCharSvr								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/01/21														 */
/* ========================================================================= */

CLibInfoCharSvr::CLibInfoCharSvr()
{
	m_pMainFrame		= NULL;
	m_pMgrData			= NULL;
	m_pLibInfoMap		= NULL;
	m_pLibInfoSkill		= NULL;
	m_pLibInfoItemType	= NULL;
	m_pLibInfoItem		= NULL;
	m_pSock				= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::~CLibInfoCharSvr								 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/01/21														 */
/* ========================================================================= */

CLibInfoCharSvr::~CLibInfoCharSvr()
{
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/01/21														 */
/* ========================================================================= */

void CLibInfoCharSvr::Create(CMgrData *pMgrData)
{
	CLibInfoCharBase::Create ();

	m_paInfoLogin = new ARRAYINFOCHARBASE;

	m_pMgrData				= pMgrData;
	m_pMainFrame			= m_pMgrData->GetMainFrame ();
	m_pLibInfoMap			= m_pMgrData->GetLibInfoMap ();
	m_pLibInfoSkill			= m_pMgrData->GetLibInfoSkill ();
	m_pLibInfoItemType		= m_pMgrData->GetLibInfoItemType ();
	m_pLibInfoItem			= m_pMgrData->GetLibInfoItem ();
	m_pLibInfoItemWeapon	= m_pMgrData->GetLibInfoItemWeapon ();
	m_pSock					= (PCUraraSockTCPSBO)m_pMgrData->GetSock ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

void CLibInfoCharSvr::Destroy(void)
{
	CLibInfoCharBase::Destroy ();

	SAFE_DELETE (m_paInfoLogin);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Proc											 */
/* 内容		:処理															 */
/* 戻り値	:TRUE:処理した FALS:処理していない								 */
/* 日付		:2007/01/21														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::Proc(void)
{
	BOOL bRet, bResult;
	int i, nCount;

	bRet	= FALSE;
	bResult	= FALSE;

	nCount = GetCountLogIn ();
	/* 削除されるかもしれないので後ろから処理 */
	for (i = nCount - 1; i >= 0; i --) {
		bResult |= ProcLocal (i);
	}
	if (bResult == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetCountLogIn									 */
/* 内容		:ログイン中キャラ数を取得										 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

int CLibInfoCharSvr::GetCountLogIn(void)
{
	return m_paInfoLogin->GetSize ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetCountOnline								 */
/* 内容		:オンライン中のPC数を取得										 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

int CLibInfoCharSvr::GetCountOnline(DWORD dwMapID/*0*/)
{
	int i, nCount, nRet;
	PCInfoCharSvr pInfoChar;

	nRet = 0;

	if (m_paInfoLogin == NULL) {
		goto Exit;
	}

	nCount = m_paInfoLogin->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoChar = (PCInfoCharSvr)m_paInfoLogin->GetAt (i);
		if (pInfoChar->IsNPC ()) {
			continue;
		}
		if (dwMapID != 0) {
			if (pInfoChar->m_dwMapID != dwMapID) {
				continue;
			}
		}
		nRet ++;
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::LogIn											 */
/* 内容		:ログイン														 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

void CLibInfoCharSvr::LogIn(
	DWORD dwCharID,			/* [in] キャラID */
	DWORD dwSessionID,		/* [in] 接続先セッションID */
	DWORD dwAccountID)		/* [in] アカウントID */
{
	PCInfoCharBase pChar, pCharTmp;

	pChar = (PCInfoCharBase)GetPtr (dwCharID);
	if (pChar == NULL) {
		return;
	}
	pCharTmp = GetPtrLogIn (dwCharID);
	if (pCharTmp) {
		return;
	}
	pChar->m_dwSessionID = dwSessionID;
	pChar->m_dwAccountID = dwAccountID;
	pChar->SetMoveState (CHARMOVESTATE_STAND);
	pChar->m_strSpeak.Empty ();
	pChar->m_ptCharBack.x = pChar->m_nMapX;
	pChar->m_ptCharBack.y = pChar->m_nMapY;
	m_paInfoLogin->Add (pChar);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::LogOut										 */
/* 内容		:ログアウト														 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

void CLibInfoCharSvr::LogOut(DWORD dwCharID)
{
	BOOL bResult;
	int i, nCount;
	PCInfoCharSvr pCharTmp, pCharParent;
	CPacketCHAR_STATE Packet;

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pCharTmp = (PCInfoCharSvr)m_paInfoLogin->GetAt (i);
		if (pCharTmp->m_dwCharID == dwCharID) {
			break;
		}
	}
	if (i >= nCount) {
		/* 見つからなかった */
		return;
	}

	/* 付いて行くのと付いて来ているのを解除させる為2回呼ぶ */
	Tail (pCharTmp, NULL, FALSE);
	Tail (pCharTmp, NULL, FALSE);
	pCharTmp->m_dwSessionID = 0;
	pCharTmp->m_dwAccountID = 0;
	pCharTmp->m_strSpeak.Empty ();

	/* 周りのキャラに通知 */
	Packet.Make (pCharTmp->m_dwCharID, CHARMOVESTATE_DELETE);
	m_pMainFrame->SendToScreenChar (pCharTmp, &Packet);

	pCharParent = (PCInfoCharSvr)GetPtrLogIn (pCharTmp->m_dwParentCharID);

	bResult = pCharTmp->IsNPC ();
	if (bResult) {
		if (pCharParent) {
			if (pCharParent->m_nMoveType == CHARMOVETYPE_PUTNPC) {
				pCharParent->DecPutCount ();
			}
		}

		bResult = pCharTmp->IsLogoutDelete ();
		/* ログアウト時に削除？ */
		if (bResult) {
			Delete (pCharTmp->m_dwCharID);
		}

	} else {
		pCharTmp->SetMoveState (CHARMOVESTATE_STAND);
		pCharTmp->SetProcState (CHARPROCSTATEID_NORMAL);
	}
	m_paInfoLogin->RemoveAt (i);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Revice										 */
/* 内容		:データの補正													 */
/* 日付		:2007/09/01														 */
/* ========================================================================= */

void CLibInfoCharSvr::Revice(void)
{
	int i, j, nCount, nCount2, nNo;
	PCInfoCharSvr pInfoCharTmp, pInfoChar;
	PCInfoItem pInfoItem;
	ARRAYINFOCHARBASE aInfoLogin;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)GetPtr (i);

		/* 移動種別に応じてクラスを更新 */
		pInfoChar = (PCInfoCharSvr)GetNew (pInfoCharTmp->m_nMoveType);
		pInfoChar->Copy (pInfoCharTmp);

		nNo = GetNoLogIn (pInfoChar->m_dwCharID);
		if (nNo >= 0) {
			m_paInfoLogin->SetAt (nNo, pInfoChar);
		}
		SAFE_DELETE (pInfoCharTmp);
		m_paInfo->SetAt (i, pInfoChar);

		if (pInfoChar->m_nMoveType != CHARMOVETYPE_PC) {
			pInfoChar->m_bNPC = TRUE;
			nNo = GetNoLogIn (pInfoChar->m_dwCharID);
			if (nNo < 0) {
				m_paInfoLogin->Add (pInfoChar);
			}
		}
		/* 不明アイテムIDの整理 */
		nCount2 = pInfoChar->m_adwItemID.GetSize ();
		for (j = nCount2 - 1; j >= 0; j --) {
			pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (pInfoChar->m_adwItemID[j]);
			if (pInfoItem == NULL) {
				pInfoChar->m_adwItemID.RemoveAt (j);
			}
		}
//Todo:マップからのはみ出しチェック
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Move											 */
/* 内容		:1歩進める														 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

void CLibInfoCharSvr::Move(
	DWORD dwCharID,		/* [in] キャラID */
	int nDirection,		/* [in] 向き */
	BOOL bTurn)			/* [in] TRUE:ぶつかる時に反転する */
{
	BOOL bResult, bEvent;
	int nTurnDirection;
	PCInfoCharSvr pInfoChar;
	POINT ptFront;

	pInfoChar = (PCInfoCharSvr)GetPtr (dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	bEvent = pInfoChar->IsNPC ();

	/* ぶつかる？ */
	bResult = !IsBlockChar (pInfoChar, nDirection);
	if (bResult) {
		/* 進める？ */
		bResult = IsMove (pInfoChar, nDirection, bEvent);
	}
	if (bResult == FALSE) {
		if (bTurn == FALSE) {
			return;
		}
		/* 逆向きに進めるかチェック */
		nTurnDirection = GetTurnDirection (nDirection);
		bResult = IsMove (pInfoChar, nTurnDirection, bEvent);
		if (bResult == FALSE) {
			/* 反転できない */
			return;
		}
		nDirection = nTurnDirection;
	}
	pInfoChar->GetFrontPos (ptFront, nDirection, TRUE);

	pInfoChar->SetDirection (nDirection);
	pInfoChar->SetPos (ptFront.x, ptFront.y);
	pInfoChar->m_bChgPos		= TRUE;
	pInfoChar->m_bChgUpdatePos	= TRUE;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Tail											 */
/* 内容		:付いて行く設定・解除											 */
/* 戻り値	:付いて行く相手のキャラID										 */
/* 日付		:2007/09/05														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::Tail(
	PCInfoCharBase pChar,			/* [in] 対象となるキャラ */
	PCInfoCharBase pCharTarget,		/* [in] 付いて行く相手(NULLで解除) */
	BOOL bTail)						/* [in] TRUE:ついていく FALSE:解除 */
{
	BOOL bResult;
	DWORD dwRet, dwSessionID;
	POINT ptFrontPos;
	PCInfoCharBase pCharTmp;
	PCInfoMapBase pInfoMap;
	CPacketCHAR_RES_TAIL Packet;
	SIZE size;

	dwRet = 0;
	dwSessionID = 0;

	if (pChar->m_dwFrontCharID) {
		pCharTarget = NULL;
	}

	/* 解除？ */
	if (bTail == FALSE) {
		if (pChar->m_dwFrontCharID) {
			pCharTmp = (PCInfoCharBase)GetPtrLogIn (pChar->m_dwFrontCharID);
			if (pCharTmp) {
				pCharTmp->m_dwTailCharID = 0;
			}
			Packet.Make (pChar->m_dwCharID, 0, RES_TAIL_RESULT_PARGE);
			m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);

			Packet.Make (pChar->m_dwCharID, pCharTmp->m_dwCharID, RES_TAIL_RESULT_PARGE);
			m_pMainFrame->SendToClient (pCharTmp->m_dwSessionID, &Packet);

			pChar->m_dwFrontCharID = 0;

		/* 付いて来ているキャラを解除 */
		} else {
			if (pChar->m_dwTailCharID) {
				pCharTmp = (PCInfoCharBase)GetPtrLogIn (pChar->m_dwTailCharID);
				if (pCharTmp) {
					pCharTmp->m_dwFrontCharID = 0;
					Packet.Make (pCharTmp->m_dwCharID, 0, RES_TAIL_RESULT_PARGE);
					m_pMainFrame->SendToClient (pCharTmp->m_dwSessionID, &Packet);
				}
				pChar->m_dwTailCharID = 0;

				Packet.Make (pChar->m_dwCharID, 0, RES_TAIL_RESULT_TAILPARGE);
				m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);
			}
		}
		goto Exit;
	}

	/*****************
		付いて行く
	*****************/

	if (pCharTarget == NULL) {
		goto Exit;
	}
	/* 既に付いて行っている？ */
	if (pChar->m_dwFrontCharID) {
		goto Exit;
	}
	if ((pCharTarget->m_nMoveState == CHARMOVESTATE_SWOON) || (pChar->m_nMoveState == CHARMOVESTATE_SWOON)) {
		Packet.Make (pChar->m_dwCharID, pCharTarget->m_dwCharID, RES_TAIL_RESULT_NG_SWOON);
		m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);
		goto Exit;
	}
	/* 戦闘モード？ */
	bResult = pCharTarget->IsStateBattle ();
	if (bResult) {
		Packet.Make (pChar->m_dwCharID, pCharTarget->m_dwCharID, RES_TAIL_RESULT_NG_STATE);
		m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);
		goto Exit;
	}
	bResult = pCharTarget->IsLogin ();
	/* ログインしていない？ */
	if (bResult == FALSE) {
		goto Exit;
	}
	/* NPC？ */
	if (pCharTarget->m_bNPC) {
		goto Exit;
	}
	/* マップが違う？ */
	if (pCharTarget->m_dwMapID != pChar->m_dwMapID) {
		goto Exit;
	}
	/* マップイベントの上にいる？ */
	bResult = CheckMapEvent ((PCInfoCharSvr)pCharTarget, TRUE);
	if (bResult) {
		Packet.Make (pChar->m_dwCharID, pCharTarget->m_dwCharID, RES_TAIL_RESULT_NG_PLACE, 0);
		m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);
		goto Exit;
	}

	pChar->GetFrontPos (ptFrontPos, -1, TRUE);
	pChar->GetCharSize (size);
	/* 目の前にいない？ */
	if (pCharTarget->IsHitCharPos (ptFrontPos.x, ptFrontPos.y, &size) == FALSE) {
		goto Exit;
	}
	/* 既に引っ張っている？ */
	if (pCharTarget->m_dwTailCharID) {
		Packet.Make (pChar->m_dwCharID, pCharTarget->m_dwCharID, RES_TAIL_RESULT_NG_TAIL, pCharTarget->m_dwTailCharID);
		m_pMainFrame->SendToClient (pChar->m_dwSessionID, &Packet);
		goto Exit;
	}
	/* 既に自分に繋がっているかチェック */
	if (pCharTarget->m_dwFrontCharID) {
		bResult		= FALSE;
		pCharTmp	= pCharTarget;
		while (1) {
			pCharTmp = (PCInfoCharBase)GetPtrLogIn (pCharTmp->m_dwFrontCharID);
			if (pCharTmp == NULL) {
				break;
			}
			if (pCharTmp->m_dwCharID == pChar->m_dwCharID) {
				bResult = TRUE;
				break;
			}
		}
		if (bResult) {
			goto Exit;
		}
	}
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}

	bResult = IsMove (pChar, pChar->m_nDirection);
	if (bResult == FALSE) {
		goto Exit;
	}

	/* それぞれのキャラIDを設定 */
	pChar->m_dwFrontCharID		= pCharTarget->m_dwCharID;
	pCharTarget->m_dwTailCharID	= pChar->m_dwCharID;
	Packet.Make (pChar->m_dwCharID, pCharTarget->m_dwCharID, RES_TAIL_RESULT_OK);
	m_pMainFrame->SendToClient (pChar->		 m_dwSessionID, &Packet);
	m_pMainFrame->SendToClient (pCharTarget->m_dwSessionID, &Packet);

	dwRet = pCharTarget->m_dwCharID;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Atack											 */
/* 内容		:攻撃															 */
/* 日付		:2007/09/11														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::Atack(PCInfoCharSvr pChar)
{
	int i, nCount;
	BOOL bRet, bHitQuit;
	DWORD dwCharID;
	ARRAYDWORD adwCharID;
	PCInfoCharMOVEATACKSvr pInfoCharTmp;

	bRet = FALSE;

	if (pChar == NULL) {
		return FALSE;
	}

	if (pChar->m_nDirection >= 4) {
		/* 斜め方向には攻撃判定しない */
		goto Exit;
	}
	bHitQuit = TRUE;
	switch (pChar->m_nMoveType) {
	case CHARMOVETYPE_MOVEATACK:		/* 移動して攻撃 */
		pInfoCharTmp = (PCInfoCharMOVEATACKSvr)pChar;
		bHitQuit = pInfoCharTmp->m_bHitQuit;
		break;
	}

	if (bHitQuit) {
		dwCharID = GetFrontCharIDTarget (pChar->m_dwCharID, -1, 1);
		/* 一歩前にいない？ */
		if (dwCharID == 0) {
			goto Exit;
		}
		bRet = AtackImple (pChar, dwCharID);

	} else {
		GetFrontCharIDTarget (pChar->m_dwCharID, -1, 1, &adwCharID);
		nCount = adwCharID.GetSize ();
		for (i = 0; i < nCount; i ++) {
			bRet |= AtackImple (pChar, adwCharID[i]);
		}
	}

Exit:
	pChar->SetMoveState (CHARMOVESTATE_BATTLE);

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Atack											 */
/* 内容		:攻撃実処理														 */
/* 日付		:2009/05/13														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::AtackImple(PCInfoCharSvr pChar, DWORD dwCharID)
{
	int nTmp, nReserveChgEfect;
	BOOL bRet, bResult, bChargeAtack, bCritical;
	DWORD dwPoint;
	PCInfoCharSvr pCharTarget, pCharTmp, pCharParent;
	CPacketCHAR_TEXTEFFECT PacketCHAR_TEXTEFFECT;

	bRet = FALSE;
	pCharTmp = NULL;

	pCharTarget = (PCInfoCharSvr)GetPtrLogIn (dwCharID);
	if (pCharTarget == NULL) {
		goto Exit;
	}
	switch (pChar->m_nAtackTarget) {
	case ATACKTARGETTYPE_NONE:			/* 未設定 */
		if (IsNPC (pChar) && IsNPC (pCharTarget)) {
			goto Exit;
		}
		break;
	case ATACKTARGETTYPE_PC:			/* プレイヤー */
		if (IsNPC (pCharTarget)) {
			goto Exit;
		}
		break;
	case ATACKTARGETTYPE_NPC:			/* NPC */
		if (IsNPC (pCharTarget) == FALSE) {
			goto Exit;
		}
		break;
	case ATACKTARGETTYPE_ALL:			/* 全て */
		break;
	}

	bResult = pChar->IsEnableAtack ();
	if (bResult == FALSE) {
		goto Exit;
	}
	bResult = pCharTarget->IsAtackTarget ();
	if (bResult == FALSE) {
		goto Exit;
	}
	pCharParent = pCharTmp = pChar;
	if (pChar->m_dwParentCharID) {
		pCharParent = (PCInfoCharSvr)GetPtrLogIn (pChar->m_dwParentCharID);
		if (pChar->m_bParentInfo) {
			/* 親がいる場合は親の情報を使用する */
			pCharTmp = pCharParent;
		}
	}

	bChargeAtack = pChar->m_bChargeAtack;
	pChar->ProcAtack ();

	bResult = IsHitAtack (pCharTmp, pCharTarget);
	/* 攻撃失敗？ */
	if (bResult == FALSE) {
		/* 文字エフェクトを通知 */
		PacketCHAR_TEXTEFFECT.Make (pCharTarget->m_dwCharID, (DWORD)0, 5, TEXTEFFECTNUMSIZE_SMALL);
		m_pMainFrame->SendToScreenChar (pCharTarget, &PacketCHAR_TEXTEFFECT);
		bRet = TRUE;
		goto Exit;
	}

	bResult = pCharTarget->ProcHit (pCharParent);
	if (bResult == FALSE) {
		bRet = TRUE;
		goto Exit;
	}

	nReserveChgEfect = 0;
	bCritical = FALSE;
	nTmp = pCharTmp->m_wPCriticalAverage;
	if (nTmp == 0) {
		/* 未指定時は5%にしておく */
		nTmp = 5;
	}
	if (genrand () % 100 >= 100 - nTmp) {
		bCritical = TRUE;
	}

	dwPoint = GetAtackDamage (pCharTmp, pCharTarget);
	if (bChargeAtack) {
		/* 溜め攻撃は威力2倍 */
		dwPoint *= 2;
	}
	if (bCritical) {
		/* クリティカルは威力2倍 */
		dwPoint *= 2;
	}

	nReserveChgEfect = GetAtackEffectID (pCharTmp, bCritical);

	Damage (pChar, pCharTarget, dwPoint, nReserveChgEfect, bCritical);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Equip											 */
/* 内容		:装備															 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::Equip(CInfoCharSvr *pChar, DWORD dwItemID)
{
	BOOL bRet, bResult;
	DWORD dwItemType, dwEquipItemID, dwEquipItemIDBack;
	PCInfoItem pInfoItem;
	PCInfoItemTypeBase pInfoItemType;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;

	bRet = FALSE;
	dwEquipItemID		= dwItemID;
	dwEquipItemIDBack	= 0;

	bResult = pChar->HaveItem (dwItemID);
	if (bResult == FALSE) {
		goto Exit;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (pInfoItem->m_dwItemTypeID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}

	dwItemType = m_pLibInfoItem->GetItemType (pInfoItem->m_dwItemID);
	switch (dwItemType) {
	case ITEMTYPEID_CLOTH:		/* 服 */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDCloth;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDCloth, pInfoItem->m_dwItemID);
		pInfoItemType->SetGrpID (&pChar->m_wGrpIDCloth, &pChar->m_wGrpIDSP);
		pChar->m_dwEquipItemIDCloth = dwItemID;
		pChar->m_bChgGrp = TRUE;
		break;
	case ITEMTYPEID_ACCE:		/* アクセサリ */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDAcce1;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDAcce1, pInfoItem->m_dwItemID);
		pInfoItemType->SetGrpID (&pChar->m_wGrpIDAcce, NULL);
		pChar->m_dwEquipItemIDAcce1 = dwItemID;
		pChar->m_bChgGrp = TRUE;
		break;
	case ITEMTYPEID_ARMS:		/* 持ち物 */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDArmsRight;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDArmsRight, pInfoItem->m_dwItemID);
		pInfoItemType->SetGrpID (&pChar->m_wGrpIDArmsMain, &pChar->m_wGrpIDArmsSub);
		pChar->m_dwEquipItemIDArmsRight = dwItemID;
		pChar->m_bChgGrp = TRUE;
		break;
	case ITEMTYPEID_SHIELD:		/* 盾 */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDArmsLeft;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDArmsLeft, pInfoItem->m_dwItemID);
		pInfoItemType->SetGrpID (&pChar->m_wGrpIDArmsLeftMain, &pChar->m_wGrpIDArmsLeftSub);
		pChar->m_dwEquipItemIDArmsLeft = dwItemID;
		pChar->m_bChgGrp = TRUE;
		break;
	default:
		goto Exit;
	}

	if (pChar->m_dwSessionID == 0) {
		bRet = TRUE;
		goto Exit;
	}

	/* 変更されたアイテム情報を通知 */
	if (dwEquipItemID) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwEquipItemID);
		if (pInfoItem) {
			PacketITEM_RES_ITEMINFO.Make (pInfoItem);
			m_pMainFrame->SendToScreenChar (pChar, &PacketITEM_RES_ITEMINFO);
		}
	}
	if (dwEquipItemIDBack) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwEquipItemIDBack);
		if (pInfoItem) {
			PacketITEM_RES_ITEMINFO.Make (pInfoItem);
			m_pMainFrame->SendToScreenChar (pChar, &PacketITEM_RES_ITEMINFO);
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::UnEquip										 */
/* 内容		:装備解除														 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::UnEquip(CInfoCharSvr *pChar, DWORD dwItemID)
{
	BOOL bRet, bResult;
	DWORD dwItemType, dwEquipItemID, dwEquipItemIDBack;
	PCInfoItem pInfoItem;
	PCInfoItemTypeBase pInfoItemType;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;

	bRet = FALSE;
	dwEquipItemID		= dwItemID;
	dwEquipItemIDBack	= 0;

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (pInfoItem->m_dwItemTypeID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}
	bResult = pChar->IsItemAdd ();
	/* 外したアイテムを持てない？ */
	if (bResult == FALSE) {
		SendFormatMsg (pChar->m_dwSessionID, FORMATMSGID_NOUNEQUIP_ITEMFULL, 0, 0, FORMATMSGTYPE_NOLOG);
		goto Exit;
	}

	dwItemType = m_pLibInfoItem->GetItemType (pInfoItem->m_dwItemID);
	switch (dwItemType) {
	case ITEMTYPEID_CLOTH:		/* 服 */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDCloth;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDCloth, 0);
		/* 初期画像に戻す */
		pChar->m_wGrpIDCloth		= pChar->m_wGrpIDInitCloth;
		pChar->m_wGrpIDSP			= pChar->m_wGrpIDInitSP;
		pChar->m_dwEquipItemIDCloth	= 0;
		pChar->m_bChgGrp			= TRUE;
		break;
	case ITEMTYPEID_ACCE:		/* アクセサリ */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDAcce1;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDAcce1, 0);
		/* 初期画像に戻す */
		pChar->m_wGrpIDAcce			= 0;
		pChar->m_dwEquipItemIDAcce1	= 0;
		pChar->m_bChgGrp			= TRUE;
		break;
	case ITEMTYPEID_ARMS:		/* 持ち物 */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDArmsRight;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDArmsRight, 0);
		/* 初期画像に戻す */
		pChar->m_wGrpIDArmsMain			= 0;
		pChar->m_wGrpIDArmsSub			= 0;
		pChar->m_dwEquipItemIDArmsRight	= 0;
		pChar->m_bChgGrp = TRUE;
		break;
	case ITEMTYPEID_SHIELD:		/* 盾 */
		dwEquipItemIDBack = pChar->m_dwEquipItemIDArmsLeft;
		m_pLibInfoItem->Equip (pChar, pChar->m_dwEquipItemIDArmsLeft, 0);
		/* 初期画像に戻す */
		pChar->m_wGrpIDArmsLeftMain		= 0;
		pChar->m_wGrpIDArmsLeftSub		= 0;
		pChar->m_dwEquipItemIDArmsLeft	= 0;
		pChar->m_bChgGrp = TRUE;
		break;
	default:
		goto Exit;
	}

	if (pChar->m_dwSessionID == 0) {
		bRet = TRUE;
		goto Exit;
	}

	/* 変更されたアイテム情報を通知 */
	if (dwEquipItemID) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwEquipItemID);
		if (pInfoItem) {
			PacketITEM_RES_ITEMINFO.Make (pInfoItem);
			m_pMainFrame->SendToClient (pChar->m_dwSessionID, &PacketITEM_RES_ITEMINFO);
		}
	}
	if (dwEquipItemIDBack) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwEquipItemIDBack);
		if (pInfoItem) {
			PacketITEM_RES_ITEMINFO.Make (pInfoItem);
			m_pMainFrame->SendToClient (pChar->m_dwSessionID, &PacketITEM_RES_ITEMINFO);
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::UseItem										 */
/* 内容		:アイテム使用													 */
/* 日付		:2008/07/05														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::UseItem(CInfoCharSvr *pChar, DWORD dwItemID)
{
	int nResult;
	BOOL bRet, bResult;
	DWORD dwTypeID;
	CPacketCHAR_ITEMINFO PacketCHAR_ITEMINFO;
	CPacketMAP_FORMATMSG PacketMsg;

	bRet = FALSE;
	if (pChar == NULL) {
		goto Exit;
	}
	bResult = pChar->HaveItem (dwItemID);
	if (bResult == FALSE) {
		goto Exit;
	}
	/* 使用できるアイテムか判定 */
	bResult = m_pLibInfoItem->IsUseItem (dwItemID);
	if (bResult == FALSE) {
		SendFormatMsg (pChar->m_dwSessionID, FORMATMSGID_NOUSE, dwItemID, 0, FORMATMSGTYPE_NOLOG);
		goto Exit;
	}

	dwTypeID = m_pLibInfoItem->GetItemType (dwItemID);
	switch (dwTypeID) {
	case ITEMTYPEID_HP:		/* HP増減 */
		nResult = UseItemProcHP (pChar, dwItemID);
		break;
	case ITEMTYPEID_LIGHT:	/* 灯り */
		nResult = UseItemProcLIGHT (pChar, dwItemID);
		break;
	}
	switch (nResult) {
	case 0:		/* 使わなかった */
		SendFormatMsg (pChar->m_dwSessionID, FORMATMSGID_NOUSE, dwItemID, 0, FORMATMSGTYPE_NOLOG);
		break;
	case 1:		/* 使ったので削除する */
		pChar->DeleteItem (dwItemID);
		m_pLibInfoItem->DeleteItem (dwItemID, pChar);
		m_pLibInfoItem->Delete (dwItemID);
		PacketCHAR_ITEMINFO.Make (pChar->m_dwCharID, &pChar->m_adwItemID);
		m_pMainFrame->SendToClient (pChar->m_dwSessionID, &PacketCHAR_ITEMINFO);
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::DragItem										 */
/* 内容		:アイテム位置変更												 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CLibInfoCharSvr::DragItem(CInfoCharSvr *pChar, DWORD dwItemID, POINT ptNewPos)
{
	int i, nCount;
	PCInfoItem pInfoItem, pInfoItemTmp, pInfoItemSrc;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;

	if (pChar == NULL) {
		return;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	pInfoItemSrc = NULL;
	nCount = pChar->m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoItemTmp = (PCInfoItem)m_pLibInfoItem->GetPtr (pChar->m_adwItemID[i]);
		if (pInfoItemTmp == NULL) {
			continue;
		}
		if (!((pInfoItemTmp->m_ptBackPack.x == ptNewPos.x) && (pInfoItemTmp->m_ptBackPack.y == ptNewPos.y))) {
			continue;
		}
		/* 入れ替えるアイテムを発見 */
		pInfoItemSrc = pInfoItemTmp;
		break;
	}
	if (pInfoItemSrc) {
		pInfoItemSrc->m_ptBackPack.x = pInfoItem->m_ptBackPack.x;
		pInfoItemSrc->m_ptBackPack.y = pInfoItem->m_ptBackPack.y;
	}
	pInfoItem->m_ptBackPack.x = ptNewPos.x;
	pInfoItem->m_ptBackPack.y = ptNewPos.y;
	PacketITEM_RES_ITEMINFO.Make (pInfoItem);
	m_pSock->SendTo (pChar->m_dwSessionID, &PacketITEM_RES_ITEMINFO);
	if (pInfoItemSrc) {
		PacketITEM_RES_ITEMINFO.Make (pInfoItemSrc);
		m_pSock->SendTo (pChar->m_dwSessionID, &PacketITEM_RES_ITEMINFO);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::RenewGrpID									 */
/* 内容		:装備画像IDを更新												 */
/* 日付		:2007/12/22														 */
/* ========================================================================= */

void CLibInfoCharSvr::RenewGrpID(DWORD dwCharID)
{
	int i, nCount;
	PCInfoCharBase pInfoChar;
	PCInfoItemTypeBase pInfoItemType;

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
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDCloth);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDCloth, &pInfoChar->m_wGrpIDSP);
		}
		/* アクセサリ */
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDAcce1);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDAcce, NULL);
		}
		/* 持ち物 */
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsRight);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDArmsMain, &pInfoChar->m_wGrpIDArmsSub);
		}
		/* 盾 */
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsLeft);
		if (pInfoItemType) {
			pInfoItemType->SetGrpID (&pInfoChar->m_wGrpIDArmsLeftMain, &pInfoChar->m_wGrpIDArmsLeftSub);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetPlaceName									 */
/* 内容		:最も人が集まっているマップ名を取得								 */
/* 日付		:2008/05/23														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetPlaceName(CmyString &strDst)
{
	int i, nCount, nNo;
	PCInfoCharSvr pInfoChar;
	PCInfoMapBase pInfoMap;
	ARRAYINT anMapLoginCount;

	strDst.Empty ();

	nCount = m_pLibInfoMap->GetCount ();
	if (nCount == 0) {
		return 0;
	}
	for (i = 0; i < nCount; i ++) {
		anMapLoginCount.Add (0);
	}

	/* 各マップの人数を集計 */
	nCount = m_paInfoLogin->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoChar = (PCInfoCharSvr)m_paInfoLogin->GetAt (i);
		if (pInfoChar->IsNPC ()) {
			continue;
		}
		if ((int)pInfoChar->m_dwMapID - 1 >= anMapLoginCount.GetSize ()) {
			continue;
		}
		anMapLoginCount[pInfoChar->m_dwMapID - 1] ++;
	}

	nNo = 0;
	nCount = m_pLibInfoMap->GetCount ();
	for (i = 1; i < nCount; i ++) {
		if (anMapLoginCount[i] > anMapLoginCount[nNo]) {
			nNo = i;
		}
	}
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (nNo);
	strDst = pInfoMap->m_strMapName;

	return pInfoMap->m_dwMapID;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::MoveMapIn										 */
/* 内容		:マップ内移動													 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

void CLibInfoCharSvr::MoveMapIn(CInfoCharSvr *pInfoChar)
{
	PBYTE pTmp;
	DWORD dwSessionID;
	CLibInfoCharSvr LibInfoCharTmp;
	CPacketCHAR_CHARINFO PacketCHAR_CHARINFO;
	CPacketCHAR_RES_CHARINFO PacketCHAR_RES_CHARINFO;
	CPacketCHAR_PARA1 PacketCHAR_PARA1;
	CPacketMAP_PARA1 PacketMAP_PARA1;

	dwSessionID = pInfoChar->m_dwSessionID;
	if (dwSessionID == 0) {
		return;
	}

	/* 周りのキャラ情報を送信 */
	LibInfoCharTmp.Create (m_pMgrData);
	pTmp = GetDataScreen (pInfoChar);
	LibInfoCharTmp.SetSendData (pTmp);
	PacketCHAR_CHARINFO.Make (&LibInfoCharTmp);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_CHARINFO);

	/* 周りのキャラに通知 */
	PacketCHAR_RES_CHARINFO.Make (pInfoChar);
	m_pMainFrame->SendToScreenChar (pInfoChar, &PacketCHAR_RES_CHARINFO);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_RES_CHARINFO);

	pInfoChar->m_bStateFadeInOut = FALSE;
	PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_FADEINOUT, pInfoChar->m_dwMapID, 0);
	m_pSock->SendTo (dwSessionID, &PacketMAP_PARA1);

	PacketCHAR_PARA1.Make (SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT, pInfoChar->m_dwCharID, 0);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_PARA1);

	SAFE_DELETE_ARRAY (pTmp);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::MoveMapOut									 */
/* 内容		:マップ外移動													 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

void CLibInfoCharSvr::MoveMapOut(CInfoCharSvr *pInfoChar)
{
	PCInfoMapBase pInfoMap;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;
	CPacketMAP_RES_MAPINFO PacketRES_MAPINFO;
	CPacketITEM_ITEMINFO PacketITEM_ITEMINFO;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}
	if (pInfoMap->IsEnableBattle () == FALSE) {
		if (pInfoChar->IsStateBattle ()) {
			pInfoChar->SetMoveState (CHARMOVESTATE_STAND);
		}
	}

	PacketRES_CHARINFO.Make (pInfoChar);
	m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketRES_CHARINFO);

	PacketRES_MAPINFO.Make (pInfoMap);
	m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketRES_MAPINFO);
	PacketITEM_ITEMINFO.Make (m_pLibInfoItem);
	m_pSock->SendTo (pInfoChar->m_dwSessionID, &PacketITEM_ITEMINFO);

	MoveMapIn (pInfoChar);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::SetInitStatus									 */
/* 内容		:ステータス初期値設定											 */
/* 日付		:2008/10/05														 */
/* ========================================================================= */

void CLibInfoCharSvr::SetInitStatus(CInfoCharSvr *pInfoChar, BOOL bInitPos/*FALSE*/)
{
	PCInfoSystem pInfoSystem;
	PSTSYSTEM_INITCHARSTATUS pInitCharStatus;

	pInfoSystem		= (PCInfoSystem)m_pMgrData->GetLibInfoSystem ()->GetPtr ();
	pInitCharStatus	= pInfoSystem->m_pInitCharStatus;

	pInfoChar->m_wStamina			= pInitCharStatus->wStamina;			/* スタミナ */
	pInfoChar->m_wPower				= pInitCharStatus->wPower;				/* 腕力 */
	pInfoChar->m_wStrength			= pInitCharStatus->wStrength;			/* 体力 */
	pInfoChar->m_wMagic				= pInitCharStatus->wMagic;				/* 魔力 */
	pInfoChar->m_wSkillful			= pInitCharStatus->wSkillful;			/* 器用 */
	pInfoChar->m_wAbillityAT		= pInitCharStatus->wAbillityAT;			/* 攻撃技能 */
	pInfoChar->m_wAbillityDF		= pInitCharStatus->wAbillityDF;			/* 防御技能 */
	pInfoChar->m_wPAtack			= pInitCharStatus->wPAtack;				/* 攻撃力 */
	pInfoChar->m_wPDefense			= pInitCharStatus->wPDefense;			/* 防御力 */
	pInfoChar->m_wPMagic			= pInitCharStatus->wPMagic;				/* 魔法力 */
	pInfoChar->m_wPMagicDefense		= pInitCharStatus->wPMagicDefense;		/* 魔法防御力 */
	pInfoChar->m_wPHitAverage		= pInitCharStatus->wPHitAverage;		/* 命中率 */
	pInfoChar->m_wPAvoidAverage		= pInitCharStatus->wPAvoidAverage;		/* 回避率 */
	pInfoChar->m_wPCriticalAverage	= pInitCharStatus->wPCriticalAverage;	/* クリティカル率 */
	pInfoChar->m_wAttrFire			= pInitCharStatus->wAttrFire;			/* 属性[火] */
	pInfoChar->m_wAttrWind			= pInitCharStatus->wAttrWind;			/* 属性[風] */
	pInfoChar->m_wAttrWater			= pInitCharStatus->wAttrWater;			/* 属性[水] */
	pInfoChar->m_wAttrEarth			= pInitCharStatus->wAttrEarth;			/* 属性[土] */
	pInfoChar->m_wAttrLight			= pInitCharStatus->wAttrLight;			/* 属性[光] */
	pInfoChar->m_wAttrDark			= pInitCharStatus->wAttrDark;			/* 属性[闇] */
	pInfoChar->m_dwMaxHP			= pInitCharStatus->dwMaxHP;				/* 最大HP */
	pInfoChar->m_dwMaxSP			= pInitCharStatus->dwMaxSP;				/* 最大SP */

	if (bInitPos) {
		pInfoChar->m_dwMapID		= pInitCharStatus->dwInitPosMapID;		/* マップID */
		pInfoChar->m_nMapX			= pInitCharStatus->ptInitPos.x;			/* X座標 */
		pInfoChar->m_nMapY			= pInitCharStatus->ptInitPos.y;			/* Y座標 */
	}

	pInfoChar->m_dwHP = min (pInfoChar->m_dwHP, pInfoChar->m_dwMaxHP);
	pInfoChar->m_dwSP = min (pInfoChar->m_dwSP, pInfoChar->m_dwMaxSP);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::AddNPC										 */
/* 内容		:NPCの追加														 */
/* 日付		:2007/09/01														 */
/* ========================================================================= */

PCInfoCharBase CLibInfoCharSvr::AddNPC(CInfoCharBase *pInfoChar)
{
	PCInfoCharSvr pInfoCharTmp;

	pInfoCharTmp = (PCInfoCharSvr)GetNew (pInfoChar->m_nMoveType);
	pInfoCharTmp->Copy (pInfoChar);

	/* 初期設定 */
	pInfoCharTmp->m_dwCharID	= 0;
	pInfoCharTmp->m_bNPC		= TRUE;
	pInfoCharTmp->m_bChgPos		= TRUE;
	pInfoCharTmp->m_nMoveType	= pInfoChar->m_nMoveType;

	Add (pInfoCharTmp);
	LogIn (pInfoCharTmp->m_dwCharID, 0, 0);

	return (PCInfoCharBase)pInfoCharTmp;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2007/01/21														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharSvr::GetNew(int nType)
{
	PCInfoBase pNew;

	switch (nType) {
	case CHARMOVETYPE_ATACKANIME:	pNew = (PCInfoBase)new CInfoCharATACKANIMESvr;	break;	/* 攻撃受けるとアニメーション */
	case CHARMOVETYPE_MOVEATACK:	pNew = (PCInfoBase)new CInfoCharMOVEATACKSvr;	break;	/* 移動して攻撃 */
	case CHARMOVETYPE_BATTLE1:		pNew = (PCInfoBase)new CInfoCharBATTLE1Svr;		break;	/* 戦闘1 */
	case CHARMOVETYPE_BATTLE2:		pNew = (PCInfoBase)new CInfoCharBATTLE2Svr;		break;	/* 戦闘2 */
	case CHARMOVETYPE_PUTNPC:		pNew = (PCInfoBase)new CInfoCharPUTNPC;			break;	/* NPC発生 */
	default:						pNew = (PCInfoBase)new CInfoCharSvr;			break;
	}

	return pNew;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::SetPtr										 */
/* 内容		:キャラ情報を更新												 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

void CLibInfoCharSvr::SetPtr(DWORD dwCharID, PCInfoCharBase pChar)
{
	int i, nCount;
	PCInfoCharBase pTmp;

	/* 先にログイン中情報を書き換える */
	nCount = m_paInfoLogin->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pTmp = (PCInfoCharBase)m_paInfoLogin->GetAt (i);
		if (pTmp->m_dwCharID == dwCharID) {
			m_paInfoLogin->SetAt (i, pChar);
			break;
		}
	}

	CLibInfoCharBase::SetPtr (dwCharID, pChar);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetPtrSessionID								 */
/* 内容		:キャラ情報を取得(セッションIDから)								 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

PCInfoCharBase CLibInfoCharSvr::GetPtrSessionID(DWORD dwSessionID)
{
	int i, nCount;
	PCInfoCharBase pRet, pInfoTmp;

	pRet = NULL;

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfoLogin->GetAt (i);
		if (pInfoTmp->m_dwSessionID == dwSessionID) {
			pRet = pInfoTmp;
			break;
		}
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetPtrLogIn									 */
/* 内容		:ログイン中キャラ情報を取得										 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

PCInfoCharBase CLibInfoCharSvr::GetPtrLogIn(int nNo)
{
	int nCount;
	PCInfoCharBase pRet;

	pRet = NULL;

	nCount = GetCountLogIn ();
	if (nNo >= nCount) {
		goto Exit;
	}
	pRet = m_paInfoLogin->GetAt (nNo);

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetPtrLogIn									 */
/* 内容		:ログイン中キャラ情報を取得										 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

PCInfoCharBase CLibInfoCharSvr::GetPtrLogIn(DWORD dwCharID)
{
	int i, nCount;
	PCInfoCharBase pRet, pCharTmp;

	pRet = NULL;
	if (dwCharID == 0) {
		goto Exit;
	}

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pCharTmp = m_paInfoLogin->GetAt (i);
		if (pCharTmp->m_dwCharID == dwCharID) {
			pRet = pCharTmp;
			break;
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetPtrParent									 */
/* 内容		:親キャラ情報を取得												 */
/* 日付		:2008/07/19														 */
/* ========================================================================= */

PCInfoCharBase CLibInfoCharSvr::GetPtrParent(CInfoCharSvr *pChar)
{
	PCInfoCharBase pRet;

	if (pChar->m_dwParentCharID == 0) {
		goto Exit;
	}

	pRet = GetPtrLogIn (pChar->m_dwParentCharID);
Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetNoLogIn									 */
/* 内容		:ログイン中キャラの配列番号を取得								 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

int CLibInfoCharSvr::GetNoLogIn(DWORD dwCharID)
{
	int i, nCount, nRet;
	PCInfoCharBase pInfoCharTmp;

	nRet = -1;

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfoLogin->GetAt (i);
		if (pInfoCharTmp->m_dwCharID == dwCharID) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetScreenCharID								 */
/* 内容		:指定キャラの画面内にいるキャラIDを取得							 */
/* 日付		:2007/02/12														 */
/* ========================================================================= */

void CLibInfoCharSvr::GetScreenCharID(
	CInfoCharSvr *pInfoChar,		/* [in] 基準となるキャラ情報 */
	ARRAYDWORD &aDst)				/* [in] 出力先 */
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;

	aDst.RemoveAll ();

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)GetPtrLogIn (i);
		/* 別マップ？ */
		if (pInfoCharTmp->m_dwMapID != pInfoChar->m_dwMapID) {
			continue;
		}
		/* 画面外？ */
		if (!((abs (pInfoCharTmp->m_nMapX - pInfoChar->m_nMapX) < DRAW_PARTS_X * 2) &&
			(abs (pInfoCharTmp->m_nMapY - pInfoChar->m_nMapY) < DRAW_PARTS_Y))) {
			continue;
		}
		aDst.Add (pInfoCharTmp->m_dwCharID);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetScreenCharIDLineOut						 */
/* 内容		:指定キャラの画面端にいるキャラIDを取得							 */
/* 日付		:2007/02/13														 */
/* ========================================================================= */

void CLibInfoCharSvr::GetScreenCharIDLineOut(
	CInfoCharSvr *pInfoChar,		/* [in] 基準となるキャラ情報 */
	ARRAYDWORD &aDst)				/* [in] 出力先 */
{
	int i, nCount, x, y,
		aPosX[] = {0, 0, DRAW_PARTS_X * 2 * -1, DRAW_PARTS_X * 2},
		aPosY[] = {DRAW_PARTS_Y * 2 * -1, DRAW_PARTS_Y * 2, 0, 0};
	PCInfoCharSvr pInfoCharTmp;

	aDst.RemoveAll ();

	x = pInfoChar->m_nMapX + aPosX[pInfoChar->m_nDirection];
	y = pInfoChar->m_nMapY + aPosY[pInfoChar->m_nDirection];

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)GetPtrLogIn (i);
		/* 別マップ？ */
		if (pInfoCharTmp->m_dwMapID != pInfoChar->m_dwMapID) {
			continue;
		}
		switch (pInfoChar->m_nDirection) {
		case 0:
		case 1:
			/* 画面端？ */
			if (abs (pInfoCharTmp->m_nMapY - y) > 2) {
				continue;
			}
			break;
		case 2:
		case 3:
			/* 画面端？ */
			if (abs (pInfoCharTmp->m_nMapX - x) > 2) {
				continue;
			}
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			/* 画面端？ */
			if (abs (pInfoCharTmp->m_nMapY - y) > 2) {
				continue;
			}
			/* 画面端？ */
			if (abs (pInfoCharTmp->m_nMapX - x) > 2) {
				continue;
			}
			break;
		}
		aDst.Add (pInfoCharTmp->m_dwCharID);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetAreaCharInfo								 */
/* 内容		:指定範囲にぶつかるキャラ情報を取得								 */
/* 日付		:2009/01/28														 */
/* ========================================================================= */

void CLibInfoCharSvr::GetAreaCharInfo(
	DWORD dwMapID,				/* [in] 対象マップID */
	RECT *prcSrc,				/* [in] 対象範囲 */
	ARRAYINFOCHARSVR &aDst)		/* [in] 出力先 */
{
	int i, nCount;
	RECT rcTmp;
	PCInfoCharSvr pInfoCharTmp;

	aDst.RemoveAll ();

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)GetPtrLogIn (i);
		/* 別マップ？ */
		if (pInfoCharTmp->m_dwMapID != dwMapID) {
			continue;
		}
		pInfoCharTmp->GetPosRect (rcTmp);
		/* ぶつからない？ */
		if (!((prcSrc->left <= rcTmp.right) &&
			(prcSrc->right >= rcTmp.left) &&
			(prcSrc->top <= rcTmp.bottom) &&
			(prcSrc->bottom >= rcTmp.top))) {
			continue;
		}
		aDst.Add (pInfoCharTmp);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetTailCharInfo								 */
/* 内容		:ついてきているキャラ情報を取得									 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

void CLibInfoCharSvr::GetTailCharInfo(
	CInfoCharSvr *pInfoChar,	/* [in] 基準となるキャラ情報 */
	ARRAYINFOCHARSVR &aDst)		/* [in] 出力先 */
{
	PCInfoCharSvr pInfoCharTmp;

	aDst.RemoveAll ();

	pInfoCharTmp = pInfoChar;
	while (1) {
		if (pInfoCharTmp->m_dwTailCharID == 0) {
			break;
		}
		pInfoCharTmp = (PCInfoCharSvr)GetPtrLogIn (pInfoCharTmp->m_dwTailCharID);
		if (pInfoCharTmp == NULL) {
			break;
		}
		aDst.Add (pInfoCharTmp);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::IsMove										 */
/* 内容		:指定方向に進めるかチェック										 */
/* 日付		:2008/05/01														 */
/* 戻り値	:TRUE:進める													 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::IsMove(
	PCInfoCharBase pInfoChar,		/* [in] キャラ情報 */
	int &nDirection,				/* [in] 移動する向き */
	BOOL bEvent)/*FALSE*/			/* [in] TRUE:マップイベントにぶつかる */
{
	int x, y, nDirectionTmp;
	BOOL bRet, bResult;
	RECT rcMap;
	POINT ptFront, ptBack;
	PCInfoMapBase pInfoMap;
	PCInfoMapEventBase pInfoMapEventBase;

	bRet = FALSE;
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}

	ptBack.x = pInfoChar->m_nMapX;
	ptBack.y = pInfoChar->m_nMapY;

	switch (nDirection) {
	case 4:
		nDirectionTmp = 0;
		bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
		if (bRet == FALSE) {
			nDirection = 3;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 3;
			bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
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
		bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
		if (bRet == FALSE) {
			nDirection = 3;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 3;
			bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
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
		bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
		if (bRet == FALSE) {
			nDirection = 2;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 2;
			bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
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
		bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
		if (bRet == FALSE) {
			nDirection = 2;
		} else {
			pInfoChar->GetFrontPos (ptFront, nDirectionTmp, TRUE);
			pInfoChar->m_nMapX = ptFront.x;
			pInfoChar->m_nMapY = ptFront.y;
			nDirectionTmp = 2;
			bRet = IsMove (pInfoChar, nDirectionTmp, bEvent);
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
	pInfoChar->GetFrontMapPosRect (rcMap, nDirection);
	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			bResult |= !pInfoMap->IsMoveOut (x, y, nDirection);
		}
	}
	bResult = !bResult;
	if (bResult == FALSE) {
		goto Exit;
	}

	/* 進めるかチェック */
	bResult = FALSE;
	pInfoChar->GetFrontMapPosRect (rcMap, nDirection);
	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			bResult |= !pInfoMap->IsMove (x, y, nDirection);
			if (bEvent) {
				pInfoMapEventBase = pInfoMap->GetEvent (x, y);
				if (pInfoMapEventBase) {
					bResult = TRUE;
					break;
				}
			}
		}
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
/* 関数名	:CLibInfoCharSvr::IsNPC											 */
/* 内容		:NPCか判定														 */
/* 日付		:2008/07/25														 */
/* 戻り値	:TRUE:NPC														 */
/* 備考		:親キャラがいれば、その親の属性で判定する						 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::IsNPC(CInfoCharSvr *pInfoChar)
{
	BOOL bRet;
	CInfoCharSvr *pInfoCharParent;

	bRet = FALSE;

	pInfoCharParent = (PCInfoCharSvr)GetPtrLogIn (pInfoChar->m_dwParentCharID);
	if (pInfoCharParent) {
		pInfoChar = pInfoCharParent;
	}
	bRet = pInfoChar->IsNPC ();

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetFrontCharID								 */
/* 内容		:一歩前のキャラIDを取得											 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetFrontCharID(DWORD dwCharID, int nDirection)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoCharBase pInfoCharSrc, pInfoCharTmp;
	POINT ptFront;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharBase)GetPtr (dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	pInfoCharSrc->GetFrontPos (ptFront, nDirection, TRUE);

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfoLogin->GetAt (i);
		if (pInfoCharSrc == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharSrc->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		if (pInfoCharTmp->IsHitCharPos (ptFront.x, ptFront.y) == FALSE) {
			continue;
		}
		if ((pInfoCharSrc->m_nMapX == pInfoCharTmp->m_nMapX) && (pInfoCharSrc->m_nMapY == pInfoCharTmp->m_nMapY)) {
			continue;
		}
		dwRet = pInfoCharTmp->m_dwCharID;
		break;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetFrontCharIDPush							 */
/* 内容		:一歩前の押せるキャラIDを取得									 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetFrontCharIDPush(DWORD dwCharID, int nDirection)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoCharBase pInfoCharSrc, pInfoCharTmp;
	POINT ptFront;
	SIZE size;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharBase)GetPtr (dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	pInfoCharSrc->GetFrontPos (ptFront, nDirection, FALSE);
	pInfoCharSrc->GetCharSize (size);

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfoLogin->GetAt (i);
		if (pInfoCharSrc == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharTmp->m_bPush == FALSE) {
			continue;
		}
		if (pInfoCharSrc->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		if ((pInfoCharSrc->m_nMapX == pInfoCharTmp->m_nMapX) && (pInfoCharSrc->m_nMapY == pInfoCharTmp->m_nMapY)) {
			continue;
		}
		if (pInfoCharTmp->IsHitCharPos (ptFront.x, ptFront.y, &size) == FALSE) {
			continue;
		}
		dwRet = pInfoCharTmp->m_dwCharID;
		break;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetFrontCharIDTarget							 */
/* 内容		:一歩前の攻撃対象キャラIDを取得									 */
/* 日付		:2008/07/26														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetFrontCharIDTarget(
	DWORD dwCharID,					/* [in] 攻撃するキャラID */
	int nDirection/*-1*/,			/* [in] 向き */
	int nXType/*0*/,				/* [in] 1:縦向きの時に、半キャラずれた状態は判定しない */
	PARRAYDWORD padwCharID)/*NULL*/	/* [out] 対象キャラIDを全て取得 */
{
	int i, nCount;
	DWORD dwRet;
	RECT rcFrontRect, rcTmp;
	PCInfoCharSvr pInfoCharSrc, pInfoCharTmp;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharSvr)GetPtr (dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	if (nDirection == -1) {
		nDirection = pInfoCharSrc->m_nDirection;
	}
	pInfoCharSrc->GetPosRectOnce (rcFrontRect, TRUE);

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)m_paInfoLogin->GetAt (i);
		if (pInfoCharSrc == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharTmp->m_dwCharID == pInfoCharSrc->m_dwParentCharID) {
			continue;
		}
		if (pInfoCharTmp->m_dwParentCharID != 0) {
			if (pInfoCharTmp->m_dwParentCharID == pInfoCharSrc->m_dwParentCharID) {
				continue;
			}
		}
		if (pInfoCharTmp->IsAtackTarget () == FALSE) {
			continue;
		}

		pInfoCharTmp->GetPosRect (rcTmp);
		if (!((rcFrontRect.left <= rcTmp.right) && (rcTmp.left <= rcFrontRect.right) &&
			(rcFrontRect.top <= rcTmp.bottom) && (rcTmp.top <= rcFrontRect.bottom))) {
			continue;
		}

		dwRet = pInfoCharTmp->m_dwCharID;
		if (padwCharID == NULL) {
			break;
		}
		padwCharID->Add (dwRet);
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetNearCharID									 */
/* 内容		:近くにいる敵キャラIDを取得										 */
/* 日付		:2009/07/17														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetNearCharID(
	DWORD dwCharID,			/* [in] キャラID */
	SIZE &sizedistance)		/* [in] 対象距離 */
{
	int i, nCount, nMinDistance, nTmp;
	DWORD dwRet;
	RECT rcSrc, rcSearch, rcTmp;
	PCInfoCharSvr pInfoCharSrc, pInfoCharTmp;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharSvr)GetPtr (dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	pInfoCharSrc->GetPosRect (rcSrc);
	rcSearch = rcSrc;
	rcSearch.left	-= sizedistance.cx;
	rcSearch.right  += sizedistance.cx;
	rcSearch.top    -= sizedistance.cy;
	rcSearch.bottom += sizedistance.cy;
	nMinDistance = -1;

	nCount = GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)m_paInfoLogin->GetAt (i);
		if (pInfoCharSrc == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharTmp->m_dwMapID != pInfoCharSrc->m_dwMapID) {
			continue;
		}
		if (pInfoCharTmp->m_dwCharID == pInfoCharSrc->m_dwParentCharID) {
			continue;
		}
		if (pInfoCharTmp->m_dwParentCharID != 0) {
			if (pInfoCharTmp->m_dwParentCharID == pInfoCharSrc->m_dwParentCharID) {
				continue;
			}
		}
		if (pInfoCharTmp->IsAtackTarget () == FALSE) {
			continue;
		}
		if (IsNPC (pInfoCharSrc) && IsNPC (pInfoCharTmp)) {
			continue;
		}

		pInfoCharTmp->GetPosRect (rcTmp);
		if (!((rcSearch.left <= rcTmp.right) && (rcTmp.left <= rcSearch.right) &&
			(rcSearch.top <= rcTmp.bottom) && (rcTmp.top <= rcSearch.bottom))) {
			continue;
		}
		nTmp = abs (rcSrc.left - rcTmp.left) + abs (rcSrc.top - rcTmp.top);
		if (nMinDistance < 0) {
			nMinDistance = nTmp;

		} else {
			if (nMinDistance <= nTmp) {
				continue;
			}
			nMinDistance = nTmp;
		}

		dwRet = pInfoCharTmp->m_dwCharID;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::SetPos										 */
/* 内容		:キャラの座標を設定												 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

void CLibInfoCharSvr::SetPos(
	CInfoCharSvr *pInfoChar,		/* [in] 対象キャラ情報 */
	DWORD dwMapID,					/* [in] マップID */
	int x,							/* [in] X座標 */
	int y,							/* [in] Y座標 */
	BOOL bTail)						/* [in] TRUE:ついてきているキャラも移動 FALSE:切り離す */
{
	int i, nCount, nResult;
	PCInfoCharSvr pInfoCharTmp, pInfoCharFront;
	ARRAYINFOCHARSVR apInfoChar;

	pInfoChar->SetPos (x, y);

	if (bTail) {
		/* ついてきているキャラ一覧を取得 */
		GetTailCharInfo (pInfoChar, apInfoChar);

		nCount = apInfoChar.GetSize ();
		for (i = nCount - 1; i >= 0; i --) {
			pInfoCharTmp	= apInfoChar[i];
			pInfoCharFront	= (PCInfoCharSvr)GetPtrLogIn (pInfoCharTmp->m_dwFrontCharID);
			if (pInfoCharFront == NULL) {
				continue;
			}
			nResult = pInfoCharTmp->GetDirection (pInfoCharFront->m_ptCharBack.x, pInfoCharFront->m_ptCharBack.y);
			if (nResult != -1) {
				pInfoCharTmp->SetDirection (nResult);
			}
			pInfoCharTmp->SetPos (pInfoCharFront->m_ptCharBack.x, pInfoCharFront->m_ptCharBack.y);
			pInfoCharTmp->m_bChgPos			= TRUE;
			pInfoCharTmp->m_bChgUpdatePos	= TRUE;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::RenewItemGrp									 */
/* 内容		:装備中のアイテム画像を更新										 */
/* 日付		:2008/07/09														 */
/* ========================================================================= */

void CLibInfoCharSvr::RenewItemGrp(
	DWORD dwTypeID)		/* [in] 種別ID */
{
	BOOL bRenew;
	int i, nCount;
	DWORD dwTypeIDTmp;
	PCInfoItemTypeBase pInfoItemType;
	PCInfoCharSvr pChar;

	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (dwTypeID);
	if (pInfoItemType == NULL) {
		return;
	}

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharSvr)GetPtr (i);

		bRenew = FALSE;
		switch (pInfoItemType->m_dwItemTypeID) {
		case ITEMTYPEID_ARMS:		/* 持ち物 */
			dwTypeIDTmp = m_pLibInfoItem->GetItemTypeID (pChar->m_dwEquipItemIDArmsRight);
			if (dwTypeID != dwTypeIDTmp) {
				break;
			}
			pInfoItemType->SetGrpID (&pChar->m_wGrpIDArmsMain, &pChar->m_wGrpIDArmsSub);
			bRenew = TRUE;
			break;
		}
		if (bRenew == FALSE) {
			continue;
		}
		pChar->m_bChgGrp = TRUE;
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::ProcAtack										 */
/* 内容		:攻撃処理														 */
/* 日付		:2008/07/16														 */
/* ========================================================================= */

void CLibInfoCharSvr::ProcAtack(CInfoCharSvr *pInfoChar)
{
	ProcLocalStateBATTLEATACK (pInfoChar);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::ProcLocal										 */
/* 内容		:処理															 */
/* 日付		:2007/09/13														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::ProcLocal(int nNo)
{
	BOOL bRet;
	PCInfoCharSvr pInfoChar;

	bRet = FALSE;

	pInfoChar = (PCInfoCharSvr)GetPtrLogIn (nNo);
	if (pInfoChar == NULL) {
		goto Exit;
	}

	pInfoChar->TimerProc (timeGetTime ());

	bRet |= ProcLocalFlgCheck (pInfoChar);
	bRet |= ProcLocalState (pInfoChar);

	if (pInfoChar->m_nMoveState == CHARMOVESTATE_DELETE) {
		LogOut (pInfoChar->m_dwCharID);
		goto Exit;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::SendSystemMsg									 */
/* 内容		:システムメッセージを送信										 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CLibInfoCharSvr::SendSystemMsg(
	DWORD dwSessionID,							/* [in] セッションID */
	LPCSTR pszMsg,								/* [in] 表示メッセージ */
	int nMsgType,	/*=SYSTEMMSGTYPE_NOLOG*/	/* [in] メッセージ種別 */
	COLORREF clMsg,	/*=RGB (255, 255, 255)*/	/* [in] 表示色 */
	BOOL bSound)	/*=TRUE*/					/* [in] 表示する時に音をならすか判定 */
{
	CPacketMAP_SYSTEMMSG PacketMsg;

	if (dwSessionID == 0) {
		return;
	}

	PacketMsg.Make (pszMsg, clMsg, bSound, nMsgType);
	m_pSock->SendTo (dwSessionID, &PacketMsg);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::SendFormatMsg									 */
/* 内容		:フォーマットメッセージを送信									 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CLibInfoCharSvr::SendFormatMsg(
	DWORD dwSessionID,							/* [in] セッションID */
	DWORD dwMsgID,								/* [in] メッセージID */
	DWORD dwPara1,	/*=0*/						/* [in] パラメータ1 */
	DWORD dwPara2,	/*=0*/						/* [in] パラメータ2 */
	int nMsgType,	/*=FORMATMSGTYPE_BATTLE*/	/* [in] メッセージ種別 */
	COLORREF clMsg,	/*=RGB (255, 255, 255)*/	/* [in] 表示色 */
	BOOL bSound)	/*=FALSE*/					/* [in] 表示する時に音をならすか判定 */
{
	CPacketMAP_FORMATMSG PacketMsg;

	if (dwSessionID == 0) {
		return;
	}

	PacketMsg.Make (dwMsgID, dwPara1, dwPara2, clMsg, bSound, nMsgType);
	m_pSock->SendTo (dwSessionID, &PacketMsg);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::ProcLocalFlgCheck								 */
/* 内容		:フラグチェック処理												 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::ProcLocalFlgCheck(CInfoCharSvr *pInfoChar)
{
	BOOL bRet, bResult;

	bRet = FALSE;

	/* ログインしていない？ */
	if (pInfoChar->IsLogin () == FALSE) {
		goto Exit;

	/* アイテムドロップ？ */
	} else if (pInfoChar->m_bDropItem) {
		DropItem (pInfoChar);
		pInfoChar->m_bDropItem = FALSE;

	/* 移動した？ */
	} else if (pInfoChar->m_bChgPos) {
		ProcChgPos (pInfoChar);

	/* マップ移動した？ */
	} else if (pInfoChar->m_bChgMap) {
		ProcChgMap (pInfoChar);

	/* 移動状態が変わった？ */
	} else if (pInfoChar->m_bChgMoveState) {
		CPacketCHAR_STATE Packet;

		Packet.Make (pInfoChar->m_dwCharID, pInfoChar->m_nMoveState);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		pInfoChar->m_bChgMoveState = FALSE;

	/* 行動状態が変わった？ */
	} else if (pInfoChar->m_bChgProcState) {
		CPacketCHAR_PARA1 Packet;

		Packet.Make (SBOCOMMANDID_SUB_CHAR_PROCSTATE, pInfoChar->m_dwCharID, pInfoChar->m_nProcState);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &Packet);
		pInfoChar->m_bChgProcState = FALSE;

	/* エフェクト変更予約あり？ */
	} else if (pInfoChar->m_nReserveChgEfect != 0) {
		CPacketCHAR_SET_EFFECT Packet;

		Packet.Make (pInfoChar->m_dwCharID, pInfoChar->m_nReserveChgEfect);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		pInfoChar->m_nReserveChgEfect = 0;

	/* 発言内容が変わった？ */
	} else if (pInfoChar->m_bChgSpeak) {
		CPacketCHAR_CHAT Packet;

		if (pInfoChar->m_strSpeak.IsEmpty () == FALSE) {
			Packet.Make (pInfoChar->m_dwCharID, pInfoChar->m_strSpeak);
			m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		}
		pInfoChar->m_bChgSpeak = FALSE;

	/* キャラ情報が変わった？ */
	} else if (pInfoChar->m_bChgInfo) {
		CPacketCHAR_RES_CHARINFO Packet;

		Packet.Make (pInfoChar, pInfoChar->m_bChgScreenPos);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		pInfoChar->m_bChgInfo = FALSE;
		pInfoChar->m_bChgScreenPos = FALSE;

	/* キャラ情報が変わった？ */
	} else if (pInfoChar->m_bChgPosRenew) {
		ProcChgPosRenew (pInfoChar);

	/* 画像が変更された？ */
	} else if (pInfoChar->m_bChgGrp) {
		CPacketCHAR_GRP PacketGRP;
		CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

		PacketGRP.Make (pInfoChar);
		m_pMainFrame->SendToScreenChar (pInfoChar, &PacketGRP);
		PacketRES_CHARINFO.Make (pInfoChar);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketRES_CHARINFO);
		pInfoChar->m_bChgGrp = FALSE;

	/* 噴出しが変更された？ */
	} else if (pInfoChar->m_bChgEfcBalloon) {
		CPacketCHAR_SET_EFCBALLOON Packet;

		Packet.Make (pInfoChar->m_dwCharID, pInfoChar->m_dwEfcBalloonID);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		pInfoChar->m_dwEfcBalloonID = 0;
		pInfoChar->m_bChgEfcBalloon = FALSE;

	/* モーションが変更された？ */
	} else if (pInfoChar->m_bChgMotion) {
		CPacketCHAR_PARA1 Packet;

		Packet.Make (SBOCOMMANDID_SUB_CHAR_SET_MOTION, pInfoChar->m_dwCharID, pInfoChar->m_dwMotionID);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);
		pInfoChar->m_dwMotionID = 0;
		pInfoChar->m_bChgMotion = FALSE;

	/* ステータスが変更された？ */
	} else if (pInfoChar->m_bChgStatus) {
		CPacketCHAR_STATUS Packet;

		pInfoChar->m_bChgStatus = FALSE;
		Packet.Make (pInfoChar);
		m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);

	/* 釣れた？ */
	} else if (pInfoChar->m_bChgFishingHit) {
		DWORD dwItemID, dwItemTypeID;
		POINT ptPos;
		PCInfoItem pInfoItem;
		CPacketCHAR_ITEMINFO PacketCHAR_ITEMINFO;
		CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;

		bResult = pInfoChar->IsItemAdd ();
		if (bResult) {
			ptPos.x = ptPos.y = 0;
//Todo:
			dwItemTypeID = 9 + (genrand () % 2);
//			dwItemTypeID = (genrand () % 30) + 1;
//			if (dwItemTypeID == 5) {
//				dwItemTypeID = 16;
//			}
			dwItemID = m_pLibInfoItem->MakeItem (0, &ptPos, dwItemTypeID);
			if (dwItemID != 0) {
				m_pLibInfoItem->AddItem (pInfoChar->m_dwCharID, dwItemID, &pInfoChar->m_adwItemID);
				pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);

				PacketCHAR_ITEMINFO.Make (pInfoChar->m_dwCharID, &pInfoChar->m_adwItemID);
				m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketCHAR_ITEMINFO);
				PacketITEM_RES_ITEMINFO.Make (pInfoItem);
				m_pMainFrame->SendToMapChar (pInfoChar->m_dwMapID, &PacketITEM_RES_ITEMINFO);
			}
		}
		pInfoChar->m_bChgFishingHit = FALSE;

	/* 指定歩数の移動 */
	} else if (pInfoChar->m_bChgMoveCount) {
		pInfoChar->m_bChgMoveCount = FALSE;
		Move (pInfoChar->m_dwCharID, pInfoChar->m_nDirection, FALSE);

	/* NPC発生 */
	} else if (pInfoChar->m_bChgPutNpc) {
		PutNpc (pInfoChar);

	/* 近くのキャラにターゲット変更 */
	} else if (pInfoChar->m_bChgTargetChar) {
		TargetChar (pInfoChar);	

	/* マップイベントチェック待ち */
	} else if (pInfoChar->m_bWaitCheckMapEvent) {
		CPacketCHAR_PARA1 Packet;

		bResult = CheckMapEvent (pInfoChar);
		pInfoChar->m_bWaitCheckMapEvent = FALSE;
		if (bResult) {
			Packet.Make (SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT, pInfoChar->m_dwCharID, 0);
			m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &Packet);
		}

	/* マップ内移動 */
	} else if (pInfoChar->m_bProcMoveMapIn) {
		MoveMapIn (pInfoChar);
		pInfoChar->m_bProcMoveMapIn = FALSE;

	/* マップ外移動 */
	} else if (pInfoChar->m_bProcMoveMapOut) {
		MoveMapOut (pInfoChar);
		pInfoChar->m_bProcMoveMapOut = FALSE;

	/* 記録位置へ移動 */
	} else if (pInfoChar->m_bProcMoveMarkPos) {
		CharProcMoveMarkPos (pInfoChar);
		pInfoChar->m_bProcMoveMarkPos = FALSE;

	/* 気絶 */
	} else if (pInfoChar->m_bProcSwoon) {
		CharProcSWOON (pInfoChar);
		pInfoChar->m_bProcSwoon = FALSE;

	/* 攻撃 */
	} else if (pInfoChar->m_bAtack) {
		CharProcAtack (pInfoChar);
		pInfoChar->m_bAtack = FALSE;

	/* ターゲット座標更新 */
	} else if (pInfoChar->m_bRenewTargetPos) {
		PCInfoCharSvr pCharTarget;

		pCharTarget = (PCInfoCharSvr)GetPtrLogIn (pInfoChar->m_dwTargetCharID);
		pInfoChar->SetTarget (pCharTarget);
		pInfoChar->m_bRenewTargetPos = FALSE;

	/* 移動種別変更予約 */
	} else if (pInfoChar->m_nReserveChgMoveState != 0) {
		pInfoChar->SetMoveState (pInfoChar->m_nReserveChgMoveState);
		pInfoChar->m_nReserveChgMoveState = 0;

	} else {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::ProcLocalState								 */
/* 内容		:状態に応じた処理												 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::ProcLocalState(CInfoCharSvr *pInfoChar)
{
	BOOL bRet, bResult;

	bRet = FALSE;

	if (pInfoChar->IsNPC () == FALSE) {
		goto Exit;
	}
	switch (pInfoChar->m_nMoveState) {
	case CHARMOVESTATE_BATTLEATACK:		/* 戦闘攻撃中 */
		bResult = ProcLocalStateBATTLEATACK (pInfoChar);
		if (bResult == FALSE) {
			goto Exit;
		}
		break;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::ProcChgPos									 */
/* 内容		:移動処理														 */
/* 日付		:2007/02/12														 */
/* ========================================================================= */

void CLibInfoCharSvr::ProcChgPos(CInfoCharSvr *pInfoChar)
{
	CPacketCHAR_MOVEPOS Packet;
	CPacketCHAR_CHARID PacketCHAR_CHARID;
	ARRAYDWORD adwCharID;

	/* 周りのキャラに座標を通知 */
	Packet.Make (
			pInfoChar->m_dwMapID,
			pInfoChar->m_dwCharID,
			pInfoChar->m_nDirection,
			pInfoChar->m_nMapX,
			pInfoChar->m_nMapY,
			pInfoChar->m_bChgUpdatePos);
	m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);

	/* 移動後に見える範囲のキャラIDを通知 */
	GetScreenCharID (pInfoChar, adwCharID);
	PacketCHAR_CHARID.Make (&adwCharID);
	m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketCHAR_CHARID);

	pInfoChar->m_bChgPos		= FALSE;
	pInfoChar->m_bChgUpdatePos	= FALSE;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::ProcChgMap									 */
/* 内容		:マップ移動処理													 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

void CLibInfoCharSvr::ProcChgMap(CInfoCharSvr *pInfoChar)
{
	CPacketMAP_PARA1 PacketMAP_PARA1;

	pInfoChar->m_bChgMap = FALSE;

	pInfoChar->AddProcInfo (CHARPROCID_MAPMOVEOUT, 2000, 0);

	PacketMAP_PARA1.Make (SBOCOMMANDID_SUB_MAP_FADEINOUT, pInfoChar->m_dwMapID, 1);
	m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketMAP_PARA1);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::ProcChgPosRenew								 */
/* 内容		:座標変更処理													 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

void CLibInfoCharSvr::ProcChgPosRenew(CInfoCharSvr *pInfoChar)
{
	CPacketCHAR_MOVEPOS Packet;
	CPacketCHAR_CHARID PacketCHAR_CHARID;
	ARRAYDWORD adwCharID;

	/* 周りのキャラに座標を通知 */
	Packet.Make (
			pInfoChar->m_dwMapID,
			pInfoChar->m_dwCharID,
			pInfoChar->m_nDirection,
			pInfoChar->m_nMapX,
			pInfoChar->m_nMapY,
			pInfoChar->m_bChgUpdatePos);
	m_pMainFrame->SendToScreenChar (pInfoChar, &Packet);

	/* 移動後に見える範囲のキャラIDを通知 */
	GetScreenCharID (pInfoChar, adwCharID);
	PacketCHAR_CHARID.Make (&adwCharID);
	m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketCHAR_CHARID);

	pInfoChar->m_bChgPosRenew = FALSE;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::ProcLocalStateBATTLEATACK						 */
/* 内容		:状態に応じた処理(戦闘攻撃中)									 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::ProcLocalStateBATTLEATACK(CInfoCharSvr *pInfoChar)
{
	BOOL bRet, bResult;
	POINT ptFront;
	int nType;

	bRet = FALSE;

	if (pInfoChar->m_nDirection >= 4) {
		/* 斜め方向には攻撃判定しない */
		goto Exit;
	}

	nType = pInfoChar->GetAtackType ();
	switch (nType) {
	case WEAPONTYPE_SWING:			/* 振る */
	case WEAPONTYPE_POKE:			/* 突く */
		bResult = Atack (pInfoChar);
		if (bResult == FALSE) {
			goto Exit;
		}
		break;
	case WEAPONTYPE_BOW:			/* 弓 */
		{
			PCInfoMapBase pInfoMap;
			PCInfoItemTypeBase pInfoItemType;
			CInfoCharMOVEATACKSvr InfoCharTmp, *pInfoCharTmp;

			pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsRight);
			if (pInfoItemType == NULL) {
				goto Exit;
			}
			pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);

			/* 進めるかチェック */
			bResult = IsMove (pInfoChar, pInfoChar->m_nDirection);
			if (bResult == FALSE) {
				goto Exit;
			}

			pInfoChar->GetFrontPos (ptFront, pInfoChar->m_nDirection, TRUE);
			InfoCharTmp.m_dwParentCharID	= pInfoChar->m_dwCharID;
			InfoCharTmp.m_dwMapID			= pInfoChar->m_dwMapID;
			InfoCharTmp.m_nDirection		= pInfoChar->m_nDirection;
			InfoCharTmp.m_bChargeAtack		= pInfoChar->m_bChargeAtack;
			InfoCharTmp.m_nMapX				= ptFront.x;
			InfoCharTmp.m_nMapY				= ptFront.y;
			InfoCharTmp.m_dwHP				= 1;
			InfoCharTmp.m_nMoveType			= CHARMOVETYPE_MOVEATACK;
			InfoCharTmp.m_wGrpIDNPC			= 50011;
			InfoCharTmp.m_ptViewCharPos.y	= 16;
			InfoCharTmp.m_dwMoveWait		= pInfoItemType->m_dwMoveWait;
			InfoCharTmp.m_dwMoveCount		= pInfoItemType->m_dwMoveCount;

			pInfoCharTmp = (PCInfoCharMOVEATACKSvr)AddNPC (&InfoCharTmp);
			pInfoCharTmp->SetMap (pInfoMap);
			pInfoCharTmp->SetLibInfoChar (this);
		}
		break;
	}

	bRet = TRUE;
Exit:
	pInfoChar->SetMoveState (CHARMOVESTATE_BATTLE);
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::CharProcMoveMarkPos							 */
/* 内容		:キャラ処理(記録位置へ移動)										 */
/* 日付		:2008/12/02														 */
/* ========================================================================= */

void CLibInfoCharSvr::CharProcMoveMarkPos(CInfoCharSvr *pInfoChar)
{
//Todo:記録位置が未実装なので初期位置へ戻る
	PCInfoSystem pInfoSystem;
	PSTSYSTEM_INITCHARSTATUS pInitCharStatus;

	pInfoSystem		= (PCInfoSystem)m_pMgrData->GetLibInfoSystem ()->GetPtr ();
	pInitCharStatus	= pInfoSystem->m_pInitCharStatus;

	/* 初期位置に転送 */
	pInfoChar->m_dwMapID = pInitCharStatus->dwInitPosMapID;		/* マップID */
	pInfoChar->SetPos (pInitCharStatus->ptInitPos.x, pInitCharStatus->ptInitPos.y, TRUE);
	pInfoChar->SetDirection (1);
	pInfoChar->AddProcInfo (CHARPROCID_MAPMOVEOUT, 2000, 0);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::CharProcSWOON									 */
/* 内容		:キャラ処理(気絶)												 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

void CLibInfoCharSvr::CharProcSWOON(CInfoCharSvr *pInfoChar)
{
	CPacketCHAR_STATUS PacketCHAR_STATUS;

	pInfoChar->m_dwHP = pInfoChar->m_dwMaxHP;
	pInfoChar->SetMoveState (CHARMOVESTATE_STAND);

	PacketCHAR_STATUS.Make (pInfoChar);
	m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketCHAR_STATUS);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::CharProcAtack									 */
/* 内容		:キャラ処理(攻撃)												 */
/* 日付		:2008/07/24														 */
/* ========================================================================= */

void CLibInfoCharSvr::CharProcAtack(CInfoCharSvr *pInfoChar)
{
	BOOL bResult;
	DWORD dwCharID;
	PCInfoCharSvr pCharTarget;
	CPacketCHAR_STATE PacketCHAR_STATE;

	dwCharID = GetFrontCharIDTarget (pInfoChar->m_dwCharID);
	/* 一歩前にいない？ */
	if (dwCharID == 0) {
		goto Exit;
	}
	pCharTarget = (PCInfoCharSvr)GetPtrLogIn (dwCharID);
	if (pCharTarget == NULL) {
		goto Exit;
	}
	if (pInfoChar->IsNPC () && pCharTarget->IsNPC ()) {
		goto Exit;
	}
	bResult = pCharTarget->IsAtackTarget ();
	if (bResult == FALSE) {
		goto Exit;
	}

	pInfoChar->SetMoveState (CHARMOVESTATE_BATTLEATACK);
	PacketCHAR_STATE.Make (pInfoChar->m_dwCharID, CHARMOVESTATE_BATTLEATACK);
	m_pMainFrame->SendToScreenChar (pInfoChar, &PacketCHAR_STATE);

Exit:
	return;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::PutNpc										 */
/* 内容		:NPC発生														 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CLibInfoCharSvr::PutNpc(CInfoCharSvr *pInfoChar)
{
	int cx, cy;
	BOOL bResult;
	PCInfoMapBase pInfoMap;
	PCInfoCharSvr pInfoCharTmp, pInfoCharAdd;
	POINT ptPos;
	RECT rcPut;

	pInfoCharTmp = NULL;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}

	/* 座標を設定 */
	rcPut.left		= pInfoChar->m_nMapX - pInfoChar->m_ptPutArea.x;
	rcPut.left		= max (rcPut.left, 0);
	rcPut.right		= pInfoChar->m_nMapX + pInfoChar->m_ptPutArea.x;
	rcPut.right		= min (rcPut.right, (pInfoMap->m_sizeMap.cx - 1) * 2);
	rcPut.top		= pInfoChar->m_nMapY - pInfoChar->m_ptPutArea.y;
	rcPut.top		= max (rcPut.top, 0);
	rcPut.bottom	= pInfoChar->m_nMapY + pInfoChar->m_ptPutArea.y;
	rcPut.bottom	= min (rcPut.bottom, (pInfoMap->m_sizeMap.cy - 1) * 2);

	cx = rcPut.right - rcPut.left + 1;
	cy = rcPut.bottom - rcPut.top + 1;
	ptPos.x = (genrand () % cx) + rcPut.left;
	ptPos.y = (genrand () % cy) + rcPut.top;

	pInfoCharTmp = (PCInfoCharSvr)GetNew (pInfoChar->m_nPutMoveType);
	pInfoCharTmp->Copy (pInfoChar);
	pInfoCharTmp->GetItem ()->RemoveAll ();			/* アイテム情報はコピーしない */
	pInfoCharTmp->m_dwCharID		= 0;
	pInfoCharTmp->m_dwParentCharID	= pInfoChar->m_dwCharID;
	pInfoCharTmp->m_nMoveType		= pInfoChar->m_nPutMoveType;
	pInfoCharTmp->m_nMapX			= ptPos.x;
	pInfoCharTmp->m_nMapY			= ptPos.y;
	pInfoCharTmp->m_nDirection		= 1;

	bResult = IsMove (pInfoCharTmp, pInfoCharTmp->m_nDirection);
	if (bResult == FALSE) {
		goto Exit;
	}

	pInfoCharAdd = (PCInfoCharSvr)AddNPC (pInfoCharTmp);
	pInfoCharAdd->SetMap (pInfoMap);
	pInfoCharAdd->SetLibInfoChar (this);

	pInfoChar->IncPutCount ();
	pInfoChar->m_bChgPutNpc = FALSE;
Exit:
	SAFE_DELETE (pInfoCharTmp);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::TargetChar									 */
/* 内容		:近くのキャラにターゲット変更									 */
/* 日付		:2009/07/17														 */
/* ========================================================================= */

void CLibInfoCharSvr::TargetChar(CInfoCharSvr *pInfoChar)
{
	DWORD dwTargetCharID;
	PCInfoCharSvr pCharTarget;

	dwTargetCharID = GetNearCharID (pInfoChar->m_dwCharID, pInfoChar->m_sizeSearchDistance);
	if (dwTargetCharID == 0) {
		return;
	}
	pCharTarget = (PCInfoCharSvr)GetPtrLogIn (dwTargetCharID);
	if (pCharTarget) {
		pInfoChar->SetTarget (pCharTarget);
	}
	pInfoChar->m_bChgTargetChar = FALSE;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::DropItem										 */
/* 内容		:アイテムドロップ												 */
/* 日付		:2008/07/19														 */
/* ========================================================================= */

void CLibInfoCharSvr::DropItem(CInfoCharSvr *pInfoChar)
{
	int nItemCount;
	DWORD dwItemID;
	POINT ptPos;
	PARRAYDWORD paItem;
	PCInfoItem pInfoItem;
	PCInfoCharSvr pInfoCharParent;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;
	CPacketMAP_FORMATMSG PacketMsg;

	if (genrand () % 100 < 100 - pInfoChar->m_nDropItemAverage) {
		return;
	}

	paItem		= pInfoChar->GetItem ();
	nItemCount	= paItem->GetSize ();
	/* アイテムを持っていなければ親の所持品を対象とする */
	if (nItemCount == 0) {
		pInfoCharParent = (PCInfoCharSvr)GetPtrParent (pInfoChar);
		if (pInfoCharParent == NULL) {
			return;
		}
		paItem		= pInfoCharParent->GetItem ();
		nItemCount	= paItem->GetSize ();
		if (nItemCount == 0) {
			return;
		}
	}

	/* 所持品から1つ選ぶ */
	dwItemID = paItem->GetAt (genrand () % nItemCount);

	ptPos.x = pInfoChar->m_nMapX;
	ptPos.y = pInfoChar->m_nMapY;

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}
	dwItemID = m_pLibInfoItem->MakeItem (pInfoChar->m_dwMapID, &ptPos, pInfoItem->m_dwItemTypeID);
	if (dwItemID == 0) {
		return;
	}
	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}
	PacketITEM_RES_ITEMINFO.Make (pInfoItem);
	m_pMainFrame->SendToMapChar (pInfoChar->m_dwMapID, &PacketITEM_RES_ITEMINFO);

	PacketMsg.Make (FORMATMSGID_DROPITEM, pInfoChar->m_dwCharID, dwItemID, RGB (255, 255, 255), FALSE, FORMATMSGTYPE_BATTLE);
	m_pMainFrame->SendToScreenChar (pInfoChar, &PacketMsg);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetTargetCharID								 */
/* 内容		:対象キャラIDを取得												 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

void CLibInfoCharSvr::GetTargetCharID(CInfoCharSvr *pInfoChar, int nTarget, int nArea, ARRAYDWORD &adstCharID)
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;

	adstCharID.RemoveAll ();

	nCount = GetCountLogIn ();

	switch (nArea) {
	case CHARAREA_SELF:			/* 自分 */
		adstCharID.Add (pInfoChar->m_dwCharID);
		break;
	case CHARAREA_FRONT:		/* 1歩前 */
	case CHARAREA_AROUND:		/* 周囲 */
		break;
	case CHARAREA_SCREEN:		/* 画面内 */
		for (i = 0; i < nCount; i ++) {
			pInfoCharTmp = (PCInfoCharSvr)GetPtrLogIn (i);
			if (pInfoCharTmp->m_dwMapID != pInfoChar->m_dwMapID) {
				continue;
			}
			if (pInfoCharTmp->IsAtackTarget () == FALSE) {
				continue;
			}
			/* 画面外？ */
			if (!(
				(abs (pInfoCharTmp->m_nMapX - pInfoChar->m_nMapX) < DRAW_PARTS_X * 2 + 2) &&
				(abs (pInfoCharTmp->m_nMapY - pInfoChar->m_nMapY) < DRAW_PARTS_Y * 2 + 2))) {
				continue;
			}
			if (nTarget == CHARTARGET_ALLY) {
				if (pInfoCharTmp->IsNPC ()) {
					continue;
				}
			} else {
				if (pInfoCharTmp->IsNPC () == FALSE) {
					continue;
				}
			}
			adstCharID.Add (pInfoCharTmp->m_dwCharID);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::IsHitAtack									 */
/* 内容		:攻撃がヒットするか判定											 */
/* 日付		:2008/09/20														 */
/* ========================================================================= */

BOOL CLibInfoCharSvr::IsHitAtack(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget)
{
	BOOL bRet;
	int nAverage;

	bRet = FALSE;

	nAverage = (int)pInfoChar->m_wPHitAverage;
	if (nAverage == 0) {
		/* 命中率と回避率が０だった場合は従来通り当たりとしておく */
		nAverage = 100;
	}
	nAverage -= (int)pCharTarget->m_wPAvoidAverage;
	nAverage  = max (nAverage, 0);
	nAverage  = min (nAverage, 100);

	if (genrand () % 100 >= 100 - nAverage) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Damage										 */
/* 内容		:ダメージ処理													 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

void CLibInfoCharSvr::Damage(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget, DWORD dwPoint, int nEffectID, BOOL bCritical)
{
	BOOL bDefense, bResult;
	int nReserveChgEfect, anDirection[] = {1, 0, 3, 2, 6, 7, 4, 5};
	DWORD dwStartTime, dwPointTmp;
	COLORREF clMsg;
	PCInfoCharSvr pCharTmp;
	PCInfoMapBase pInfoMap;
	CPacketMAP_FORMATMSG PacketMsg;
	CPacketCHAR_STATUS PacketCHAR_STATUS;
	CPacketCHAR_STATE PacketCHAR_STATE;
	CPacketCHAR_TEXTEFFECT PacketCHAR_TEXTEFFECT;
	CPacketCHAR_SET_EFFECT PacketCHAR_SET_EFFECT;

	clMsg = RGB (255, 255, 255);

	pCharTmp = pInfoChar;
	if (pInfoChar->m_dwParentCharID) {
		/* 親がいる場合は親の情報を使用する */
		pCharTmp = (PCInfoCharSvr)GetPtrLogIn (pInfoChar->m_dwParentCharID);
	}

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pCharTarget->m_dwMapID);

	bDefense = (pCharTarget->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) ? TRUE : FALSE;
	if (bDefense) {
		if (pCharTarget->m_nDirection != anDirection[pInfoChar->m_nDirection]) {
			/* 正面でない時は防御無効 */
			bDefense = FALSE;
		}
	}

	nReserveChgEfect = nEffectID;
	if (bDefense) {
		/* 防御中はダメージ半分 */
		dwPoint /= 2;
		nReserveChgEfect = 3;
		/* 防御成功モーション再生 */
		pCharTarget->SetMotion (CHARMOTIONLISTID_DEFENSE_SUCCESS_UP);

	} else {
		/* ダメージ通知として状態は変えずにクライアントへ通知 */
		PacketCHAR_STATE.Make (pCharTarget->m_dwCharID, CHARMOVESTATE_DAMAGE);
		m_pMainFrame->SendToScreenChar (pCharTarget, &PacketCHAR_STATE);
		if (pCharTarget->IsLogoutDelete ()) {
			/* ダメージモーション再生 */
			pCharTarget->SetMotion (CHARMOTIONLISTID_DAMAGE_UP);
		}
	}
	/* エフェクト再生 */
	if (nReserveChgEfect) {
		PacketCHAR_SET_EFFECT.Make (pCharTarget->m_dwCharID, nReserveChgEfect);
		m_pMainFrame->SendToScreenChar (pCharTarget, &PacketCHAR_SET_EFFECT);
	}

	dwPointTmp = min (dwPoint, pCharTarget->m_dwHP);
	pCharTarget->m_dwHP -= dwPointTmp;

	SendFormatMsg (pCharTarget->m_dwSessionID, FORMATMSGID_DAMAGE1, pCharTmp->m_dwCharID, dwPoint);
	SendFormatMsg (pCharTmp->m_dwSessionID, FORMATMSGID_DAMAGE2, pCharTarget->m_dwCharID, dwPoint);

	PacketCHAR_STATUS.Make (pCharTarget);
	m_pMainFrame->SendToClient (pCharTarget->m_dwSessionID, &PacketCHAR_STATUS);

	/* 文字エフェクトを通知 */
	PacketCHAR_TEXTEFFECT.Make (
			pCharTarget->m_dwCharID,
			dwPoint,
			(bCritical) ? 2 : 1,
			(bCritical) ? TEXTEFFECTNUMSIZE_LARGE : TEXTEFFECTNUMSIZE_MEDIUM);
	m_pMainFrame->SendToScreenChar (pCharTarget, &PacketCHAR_TEXTEFFECT);

	/* 気絶？ */
	if (pCharTarget->m_dwHP == 0) {
		pCharTarget->SetMoveState (CHARMOVESTATE_SWOON);
//Todo:暫定
		dwStartTime = 5000;
		bResult		= TRUE;
		if (pCharTarget->IsLogoutDelete ()) {
			/* 死亡エフェクト再生 */
			pCharTarget->SetMotion (CHARMOTIONLISTID_DEAD_UP);
			dwStartTime = 2500;
		} else {
			/* 付いて行くのと付いて来ているのを解除させる為2回呼ぶ */
			Tail (pCharTarget, NULL, FALSE);
			Tail (pCharTarget, NULL, FALSE);
			pCharTarget->m_nReserveChgEfect = 2;
			if (pInfoMap->m_bRecovery == FALSE) {
				bResult = FALSE;
			}
		}
		if (bResult) {
			pCharTarget->AddProcInfo (CHARPROCID_SWOON, dwStartTime, 0);
		}

		SendFormatMsg (pCharTarget->m_dwSessionID, FORMATMSGID_SWOON3, pCharTmp->m_dwCharID);
		SendFormatMsg (pCharTmp->m_dwSessionID, FORMATMSGID_SWOON2, pCharTarget->m_dwCharID);

		PacketMsg.Make (FORMATMSGID_SWOON1, pCharTmp->m_dwCharID, pCharTarget->m_dwCharID, clMsg, FALSE, FORMATMSGTYPE_BATTLE);
		m_pMainFrame->SendToScreenChar (pCharTmp, &PacketMsg);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetAtackDamage								 */
/* 内容		:攻撃ダメージを取得												 */
/* 日付		:2008/08/15														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetAtackDamage(CInfoCharSvr *pInfoChar, CInfoCharSvr *pCharTarget)
{
	int nHitAverage, nAT, nDF, nPoint;
	DWORD dwRet;
	PCInfoItemTypeBase pInfoItemType;

	dwRet = pInfoChar->GetDamage ();
	if (dwRet != 0) {
		return dwRet;
	}
	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsRight);

//Todo:
	nHitAverage = pInfoChar->m_wPHitAverage;
	nHitAverage -= pCharTarget->m_wPAvoidAverage;
	nHitAverage = 100;

	nAT = pInfoChar->m_wPAtack;
	nDF = pInfoChar->m_wPDefense;

	/* 武器装備中？ */
	if (pInfoItemType) {
		nAT += pInfoItemType->m_dwValue;
	}

	/* 直接攻撃のダメージ＝{ 攻撃力 - ( 守備力 / 2 ) } ±16％ */
	nPoint = nAT - (nDF / 2);

	/* ＋－の補正 */
	if (genrand () % 2) {
		nPoint += (nPoint * ((genrand () % 16) + 1) / 100);
	} else {
		nPoint -= (nPoint * ((genrand () % 16) + 1) / 100);
	}
	nPoint = max (nPoint, 0);

	/* ノーダメージ？ */
	if (nPoint == 0) {
		if ((genrand () % 100) > 66) {
			nPoint = 1;
		}
	}

	dwRet = nPoint;
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::GetAtackEffectID								 */
/* 内容		:装備に応じた攻撃時のエフェクトIDを取得							 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

DWORD CLibInfoCharSvr::GetAtackEffectID(CInfoCharSvr *pInfoChar, BOOL bCritical)
{
	int nCount;
	DWORD dwRet;
	PCInfoItemTypeBase pInfoItemType;
	PCInfoItemWeapon pInfoItemWeapon;
	PARRAYDWORD padwEffectID;

	dwRet = pInfoChar->GetHitEffectID ();
	if (dwRet != 0) {
		return dwRet;
	}
//Todo:初期値
	dwRet = 1;

	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItem->GetItemTypePtr (pInfoChar->m_dwEquipItemIDArmsRight);
	if (pInfoItemType == NULL) {
		goto Exit;
	}
	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (pInfoItemType->m_dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		goto Exit;
	}

	padwEffectID = &pInfoItemWeapon->m_adwEffectIDAtack;
	if (bCritical) {
		padwEffectID = &pInfoItemWeapon->m_adwEffectIDCritical;
	}
	nCount = padwEffectID->GetSize ();
	if (nCount <= 0) {
		goto Exit;
	}
	dwRet = padwEffectID->GetAt (genrand () % nCount);

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::UseItemProcHP									 */
/* 内容		:アイテム使用処理(HP増減)										 */
/* 日付		:2008/07/05														 */
/* ========================================================================= */

int CLibInfoCharSvr::UseItemProcHP(CInfoCharSvr *pInfoChar, DWORD dwItemID)
{
	int i, nCount, nRet, nTmp, nPoint;
	PCInfoItem pInfoItem;
	PCInfoItemTypeBase pInfoItemType;
	PCInfoCharSvr pCharTarget;
	CmyString strMsg;
	ARRAYDWORD aCharID;
	CPacketCHAR_STATUS PacketCHAR_STATUS;
	CPacketADMIN_PLAYSOUND PacketPLAYSOUND;
	CPacketCHAR_TEXTEFFECT PacketCHAR_TEXTEFFECT;

	nRet = 0;

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (pInfoItem->m_dwItemTypeID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}

	nTmp = (int)pInfoItemType->m_dwValue2;
	if (nTmp > 0) {
		if (pInfoChar->m_dwMaxHP == pInfoChar->m_dwHP) {
			goto Exit;
		}
	}
	GetTargetCharID (pInfoChar, pInfoItemType->m_byTarget, pInfoItemType->m_byArea, aCharID);

	nCount = aCharID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		nTmp = (int)pInfoItemType->m_dwValue2;
		if (nTmp == 0) {
			nTmp = 1;
		}
		nPoint = genrand () % abs (nTmp);
		if (nTmp < 0) {
			nPoint *= -1;
		}
		nPoint += (int)pInfoItemType->m_dwValue;

		pCharTarget = (PCInfoCharSvr)GetPtrLogIn (aCharID[i]);
		if (pCharTarget == NULL) {
			continue;
		}
		/* 回復 */
		if (nPoint >= 0) {
			nPoint = min ((DWORD)nPoint, pCharTarget->m_dwMaxHP - pCharTarget->m_dwHP);

			strMsg.Format ("HPが %d 回復しました", nPoint);

			pCharTarget->m_dwHP += nPoint;

			/* 文字エフェクトを通知 */
			PacketCHAR_TEXTEFFECT.Make (pCharTarget->m_dwCharID, nPoint, 3);
			m_pMainFrame->SendToScreenChar (pCharTarget, &PacketCHAR_TEXTEFFECT);

			if (pCharTarget->m_dwSessionID) {
				SendSystemMsg (pCharTarget->m_dwSessionID, (LPCSTR)strMsg, SYSTEMMSGTYPE_NOLOG);

				PacketCHAR_STATUS.Make (pCharTarget);
				m_pSock->SendTo (pCharTarget->m_dwSessionID, &PacketCHAR_STATUS);
			}
			/* エフェクトの設定 */
			if (pInfoItemType->m_dwUseEffectID != 0) {
				pCharTarget->m_nReserveChgEfect = (int)pInfoItemType->m_dwUseEffectID;
			}

		/* ダメージ */
		} else {
			if (pInfoChar == pCharTarget) {
				continue;
			}
			Damage (pInfoChar, pCharTarget, abs (nPoint), (int)pInfoItemType->m_dwUseEffectID, FALSE);
		}
	}

	nRet = -1;
	/* 無くなるか判定 */
	if (pInfoItemType->m_byDelAverage != 0) {
		if (genrand () % 100 >= 100 - pInfoItemType->m_byDelAverage) {
			nRet = 1;
		}
	}
	/* 効果音の再生 */
	if (pInfoItemType->m_dwUseSoundID != 0) {
		PacketPLAYSOUND.Make (pInfoItemType->m_dwUseSoundID);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketPLAYSOUND);
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::UseItemProcLIGHT								 */
/* 内容		:アイテム使用処理(灯り)											 */
/* 日付		:2008/10/19														 */
/* ========================================================================= */

int CLibInfoCharSvr::UseItemProcLIGHT(CInfoCharSvr *pInfoChar, DWORD dwItemID)
{
	int nRet;
	PCInfoItem pInfoItem;
	PCInfoItemTypeBase pInfoItemType;
	CPacketADMIN_PLAYSOUND PacketPLAYSOUND;
	CmyString strMsg;

	nRet = 0;

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (pInfoItem->m_dwItemTypeID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}

	pInfoChar->m_nLightLevel = (int)pInfoItemType->m_dwValue;
	pInfoChar->m_dwLightTime = timeGetTime () + pInfoItemType->m_dwValue2;
	pInfoChar->m_bChgStatus = TRUE;

	strMsg.Format ("%s を使いました", (LPCSTR)pInfoItem->m_strName);
	SendSystemMsg (pInfoChar->m_dwSessionID, (LPCSTR)strMsg, SYSTEMMSGTYPE_NOLOG);

	nRet = -1;
	/* 無くなるか判定 */
	if (pInfoItemType->m_byDelAverage != 0) {
		if (genrand () % 100 >= 100 - pInfoItemType->m_byDelAverage) {
			nRet = 1;
		}
	}
	/* 効果音の再生 */
	if (pInfoItemType->m_dwUseSoundID != 0) {
		PacketPLAYSOUND.Make (pInfoItemType->m_dwUseSoundID);
		m_pMainFrame->SendToClient (pInfoChar->m_dwSessionID, &PacketPLAYSOUND);
	}

Exit:
	return nRet;
}

/* Copyright(C)URARA-works 2007 */
