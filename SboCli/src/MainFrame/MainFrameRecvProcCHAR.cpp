/// @file MainFrameRecvProcCHAR.cpp
/// @brief クライアントメインフレーム(キャラ系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/12/31
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "SboCli_priv.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "InfoAccount.h"
#include "InfoMapBase.h"
#include "LibInfoMotion.h"
#include "LibInfoMotionType.h"
#include "LibInfoCharCli.h"
#include "LibInfoItem.h"
#include "InfoCharCli.h"
#include "LayerMap.h"
#include "StateProcBase.h"
#include "StateProcMAP.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrWindow.h"
#include "MainFrame.h"

namespace {

}

#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
static void WritePosSyncDebugLog(DWORD dwCharID, int nBeforeX, int nBeforeY, int nSyncX, int nSyncY, int nApplyX, int nApplyY, int nDeltaMax, LPCSTR pszAdjustType, BOOL bUpdate)
{
	char szDbg[256];

	sprintf_s(
			szDbg,
			"[POS_SYNC][CHAR:%u][補正:%s][誤差:%dpx][before:%d,%d][sync:%d,%d][apply:%d,%d][update:%d]\n",
			dwCharID,
			pszAdjustType,
			nDeltaMax,
			nBeforeX,
			nBeforeY,
			nSyncX,
			nSyncY,
			nApplyX,
			nApplyY,
			bUpdate ? 1 : 0);
	SboDbgLog("%s", szDbg);
}
#endif


void CMainFrame::RecvProcCHAR(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_CHAR_RES_CHARINFO: RecvProcCHAR_RES_CHARINFO(pData); break; // キャラ情報応答
	case SBOCOMMANDID_SUB_CHAR_CHARINFO: RecvProcCHAR_CHARINFO(pData); break; // キャラ情報通知
	case SBOCOMMANDID_SUB_CHAR_MOVE_START: RecvProcCHAR_MOVE_START(pData); break; // 移動開始通知(Dead Reckoning)
	case SBOCOMMANDID_SUB_CHAR_MOVE_DIR_CHANGE: RecvProcCHAR_MOVE_DIR_CHANGE(pData); break; // 移動方向変更通知(Dead Reckoning)
	case SBOCOMMANDID_SUB_CHAR_MOVE_STOP: RecvProcCHAR_MOVE_STOP(pData); break; // 移動停止通知(Dead Reckoning)
	case SBOCOMMANDID_SUB_CHAR_POS_SYNC: RecvProcCHAR_POS_SYNC(pData); break; // 座標同期通知(Dead Reckoning)
	case SBOCOMMANDID_SUB_CHAR_STATE: RecvProcCHAR_STATE(pData); break; // 状態通知
	case SBOCOMMANDID_SUB_CHAR_PROCSTATE: RecvProcCHAR_PROCSTATE(pData); break; // 行動状態通知
	case SBOCOMMANDID_SUB_CHAR_CHAT: RecvProcCHAR_CHAT(pData); break; // チャット通知
	case SBOCOMMANDID_SUB_CHAR_CHARID: RecvProcCHAR_CHARID(pData); break; // キャラID通知
	case SBOCOMMANDID_SUB_CHAR_RES_PUTGET: RecvProcCHAR_RES_PUTGET(pData); break; // アイテムを拾う置く応答
	case SBOCOMMANDID_SUB_CHAR_SET_EFFECT: RecvProcCHAR_SET_EFFECT(pData); break; // エフェクトを指定
	case SBOCOMMANDID_SUB_CHAR_ITEMINFO: RecvProcCHAR_ITEMINFO(pData); break; // アイテム情報通知
	case SBOCOMMANDID_SUB_CHAR_RES_TAIL: RecvProcCHAR_RES_TAIL(pData); break; // 付いて行く応答
	case SBOCOMMANDID_SUB_CHAR_MODIFY_PARAM: RecvProcCHAR_MODIFY_PARAM(pData); break; // パラメータ変更通知
	case SBOCOMMANDID_SUB_CHAR_GRP: RecvProcCHAR_GRP(pData); break; // 画像通知
	case SBOCOMMANDID_SUB_CHAR_MOTION: RecvProcCHAR_MOTION(pData); break; // モーション情報通知
	case SBOCOMMANDID_SUB_CHAR_MOTIONTYPE: RecvProcCHAR_MOTIONTYPE(pData); break; // モーション種別情報通知
	case SBOCOMMANDID_SUB_CHAR_SET_EFCBALLOON: RecvProcCHAR_SET_EFCBALLOON(pData); break; // 噴出しを指定
	case SBOCOMMANDID_SUB_CHAR_SET_MOTION: RecvProcCHAR_SET_MOTION(pData); break; // モーションを指定
	case SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT: RecvProcCHAR_RES_CHECKMAPEVENT(pData); break; // マップイベントチェック応答
	case SBOCOMMANDID_SUB_CHAR_STATUS: RecvProcCHAR_STATUS(pData); break; // ステータス情報通知
	case SBOCOMMANDID_SUB_CHAR_TEXTEFFECT: RecvProcCHAR_TEXTEFFECT(pData); break; // 文字エフェクト通知
	case SBOCOMMANDID_SUB_CHAR_STATE_CHARGE: RecvProcCHAR_STATE_CHARGE(pData); break; // 溜め状態通知
	case SBOCOMMANDID_SUB_CHAR_RES_TALKEVENT: RecvProcCHAR_RES_TALKEVENT(pData); break; // 会話イベント情報応答
	case SBOCOMMANDID_SUB_CHAR_SKILLINFO: RecvProcCHAR_SKILLINFO(pData); break; // スキル情報通知
	}
}


void CMainFrame::RecvProcCHAR_POS_SYNC(PBYTE pData)
{
	int nDeltaX, nDeltaY, nDeltaMax;
	int nStateMove, nStateStop;
	int nSetX, nSetY, nBeforeX, nBeforeY, nApplyX, nApplyY;
	LPCSTR pszAdjustType;
	DWORD dwRecvTime;
	PCInfoCharCli pInfoChar;
	CPacketCHAR_POS_SYNC Packet;

	Packet.Set(pData);

	/*
	   WebSocket 経路で壊れた POS_SYNC が混ざることがあり、
	   異常な方向や座標をそのまま食べると大ジャンプや描画負荷増大を招く。
	   正常範囲外の値は同期対象から外して、既存の MOVE_* / 次の正常同期を待つ。
	*/
	if ((Packet.m_nDirection < 0) || (Packet.m_nDirection > 7) ||
		(abs(Packet.m_pos.x) > 100000) ||
		(abs(Packet.m_pos.y) > 100000)) {
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
		SboDbgLog("[POS_SYNC][DROP_INVALID] charID=%u dir=%d pos=(%d,%d) update=%d ts=%u",
			Packet.m_dwCharID,
			Packet.m_nDirection,
			Packet.m_pos.x,
			Packet.m_pos.y,
			Packet.m_bUpdate ? 1 : 0,
			Packet.m_dwTimeStamp);
#endif
		return;
	}

	if (m_pMgrData->GetCharID() == Packet.m_dwCharID) {
		return;
	}

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	nStateMove = CHARMOVESTATE_MOVE;
	nStateStop = CHARMOVESTATE_STAND;
	if (pInfoChar->IsStateBattle()) {
		nStateMove = CHARMOVESTATE_BATTLEMOVE;
		nStateStop = CHARMOVESTATE_BATTLE;
		if (pInfoChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			nStateStop = CHARMOVESTATE_BATTLE_DEFENSE;
		}
	}

	nBeforeX = pInfoChar->m_nMapX;
	nBeforeY = pInfoChar->m_nMapY;
	nApplyX = nBeforeX;
	nApplyY = nBeforeY;
	pszAdjustType = "none";

	nDeltaX = Packet.m_pos.x - pInfoChar->m_nMapX;
	nDeltaY = Packet.m_pos.y - pInfoChar->m_nMapY;
	nDeltaMax = abs(nDeltaX);
	if (nDeltaMax < abs(nDeltaY)) {
		nDeltaMax = abs(nDeltaY);
	}

	if ((nDeltaMax > 0) && (nDeltaMax < 8)) {
		/*
		   Web 版は細かい移動や向き変更直後に 1〜7px 程度の差分で止まることがある。
		   ここを無視すると「届いているのに少し前の位置/向きのまま」に見えるため、
		   小差分はそのまま同期して表示遅延を防ぐ。
		*/
		pInfoChar->SetPos(Packet.m_pos.x, Packet.m_pos.y);
		nApplyX = Packet.m_pos.x;
		nApplyY = Packet.m_pos.y;
		pszAdjustType = "micro";
	} else if (Packet.m_bUpdate && (nDeltaMax >= 8) && (nDeltaMax < 32)) {
		/*
		   MOVE_* が抜ける環境では POS_SYNC(update) が唯一の最新位置になることがある。
		   予測移動中でも 8〜31px の更新差分を握りつぶすと、
		   ちょん移動や向き変更が数秒遅れて最後に飛んで見える。
		   update 付きの中差分は即時反映して、見た目の取り残されを減らす。
		*/
		pInfoChar->SetPos(Packet.m_pos.x, Packet.m_pos.y);
		nApplyX = Packet.m_pos.x;
		nApplyY = Packet.m_pos.y;
		pszAdjustType = "update-sync";
	} else if (nDeltaMax >= 32) {
		pInfoChar->SetPos(Packet.m_pos.x, Packet.m_pos.y);
		nApplyX = Packet.m_pos.x;
		nApplyY = Packet.m_pos.y;
		pszAdjustType = "snap";
	} else if ((pInfoChar->m_bPredictedMove == FALSE) && (nDeltaMax >= 8)) {
		nSetX = pInfoChar->m_nMapX + (nDeltaX / 2);
		nSetY = pInfoChar->m_nMapY + (nDeltaY / 2);
		pInfoChar->SetPos(nSetX, nSetY);
		nApplyX = nSetX;
		nApplyY = nSetY;
		pszAdjustType = "lerp";
	} else if (pInfoChar->m_bPredictedMove && (nDeltaMax >= 8)) {
		pszAdjustType = "skip";
	}
	pInfoChar->SetDirection(Packet.m_nDirection);
	pInfoChar->m_nPredictSyncX = Packet.m_pos.x;
	pInfoChar->m_nPredictSyncY = Packet.m_pos.y;

	// POS_SYNC のタイムスタンプは送信元プロセス基準なので、別ブラウザ/別プロセス間では
	// 受信側ローカル時計と一致しない。予測移動はローカル受信時刻を基準にする。
	dwRecvTime = SDL_GetTicks();
	// [DBG-POSSYNC-TIME] timeGetTime は Win32 専用のため SDL_GetTicks() で代替
	// 量を減らすため m_bUpdate==TRUE（移動開始）のパケットのみ出力する
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
	if (Packet.m_bUpdate) {
		DWORD dwSdlNow = SDL_GetTicks();
		SboDbgLog("[POS_SYNC][CHAR:%u][update] pkt.TimeStamp=%u SDL_GetTicks=%u diff=%d",
			Packet.m_dwCharID,
			Packet.m_dwTimeStamp,
			dwSdlNow,
			(int)((int)dwSdlNow - (int)Packet.m_dwTimeStamp));
	}
#endif
	if (Packet.m_bUpdate) {
		pInfoChar->StartPredictedMove(Packet.m_nDirection, Packet.m_pos.x, Packet.m_pos.y, dwRecvTime);
		/*
		   Web 版では MOVE_* が欠けても POS_SYNC(update) は届くことがある。
		   この時に予測移動だけ開始して移動状態へ入れないと、
		   座標だけ動いて歩行アニメが止まったままに見えてしまう。
		*/
		if (pInfoChar->m_nMoveState != nStateMove) {
			pInfoChar->ChgMoveState(nStateMove);
		}
	} else {
		pInfoChar->DeleteAllMovePosQue();
		pInfoChar->StopPredictedMove(Packet.m_pos.x, Packet.m_pos.y);
		if (pInfoChar->IsStateMove()) {
			pInfoChar->ChgMoveState(nStateStop);
		}
	}

	pInfoChar->m_bRedraw = TRUE;
	m_bRenewCharInfo = TRUE;

#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
	WritePosSyncDebugLog(
			Packet.m_dwCharID,
			nBeforeX,
			nBeforeY,
			Packet.m_pos.x,
			Packet.m_pos.y,
			nApplyX,
			nApplyY,
			nDeltaMax,
			pszAdjustType,
			Packet.m_bUpdate);
#endif
}


void CMainFrame::RecvProcCHAR_RES_CHARINFO(PBYTE pData)
{
	DWORD dwMapIDBack;
	int nMapXBack, nMapYBack;
	BOOL bSyncEventTile, bPlayerPosChanged, bResetMoveInterpolation;
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	PCInfoAccount pInfoAccount;
	PCLayerMap pLayerMap;
	CPacketCHAR_RES_CHARINFO Packet;

	pInfoChar = NULL;
	Packet.Set(pData);
	dwMapIDBack = 0;
	nMapXBack = nMapYBack = 0;
	bSyncEventTile = FALSE;
	bPlayerPosChanged = FALSE;
	bResetMoveInterpolation = FALSE;
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
	SboDbgLog("[RES_CHARINFO入口] charID=%u moveType=%d mapID=%u pos=(%d,%d)",
		Packet.m_pInfo->m_dwCharID,
		Packet.m_pInfo->m_nMoveType,
		Packet.m_pInfo->m_dwMapID,
		Packet.m_pInfo->m_nMapX,
		Packet.m_pInfo->m_nMapY);
#endif

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_pInfo->m_dwCharID);
	if (pInfoChar == NULL) {
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetNew(Packet.m_pInfo->m_nMoveType);
		pInfoChar->Copy(Packet.m_pInfo);
		pInfoChar->SetViewState(INFOCHARCLI_VIEWSTATE_FADEIN);
		m_pLibInfoChar->Add(pInfoChar);
		bResetMoveInterpolation = TRUE;
	} else {
		// 移動種別が変わった？
		if (Packet.m_pInfo->m_nMoveType != pInfoChar->m_nMoveType) {
			pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetNew(Packet.m_pInfo->m_nMoveType);
			m_pLibInfoChar->SetPtr(Packet.m_pInfo->m_dwCharID, pInfoChar);
			bResetMoveInterpolation = TRUE;
		}
	}
	dwMapIDBack = pInfoChar->m_dwMapID;
	nMapXBack = pInfoChar->m_nMapX;
	nMapYBack = pInfoChar->m_nMapY;

#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
	// [DBG-CHARINFO-1] Copy 前のバックアップ値と新規キャラかどうかをログ出力
	{
		BOOL bIsNewChar = (dwMapIDBack == 0) ? TRUE : FALSE;
		SboDbgLog("[RES_CHARINFO] Copy前 mapIDBack=%u posBack=(%d,%d) newChar=%d packetMap=%u packetPos=(%d,%d)",
			dwMapIDBack, nMapXBack, nMapYBack, bIsNewChar,
			Packet.m_pInfo->m_dwMapID,
			Packet.m_pInfo->m_nMapX,
			Packet.m_pInfo->m_nMapY);
	}
#endif

	pInfoChar->Copy(Packet.m_pInfo);
	if ((bResetMoveInterpolation) ||
		(dwMapIDBack != pInfoChar->m_dwMapID) ||
		(abs(nMapXBack - pInfoChar->m_nMapX) >= MAPPARTSSIZE) ||
		(abs(nMapYBack - pInfoChar->m_nMapY) >= MAPPARTSSIZE) ||
		(Packet.m_bChgScreenPos)) {
		pInfoChar->SnapMoveInterpolation();
	}
	pInfoChar->MakeCharGrp();
	pInfoChar->m_bRedraw = TRUE;
	m_pLibInfoChar->RenewMotionInfo(pInfoChar);

	pInfoAccount = m_pMgrData->GetAccount();
	if (pInfoAccount->m_dwCharID == Packet.m_pInfo->m_dwCharID) {
		m_pMgrData->SetPlayerChar(pInfoChar);
	}
	pInfoCharPlayer = m_pMgrData->GetPlayerChar();
	if ((pInfoChar != NULL) &&
		(pInfoChar != pInfoCharPlayer) &&
		(pInfoCharPlayer != NULL) &&
		(pInfoChar->m_dwMapID != pInfoCharPlayer->m_dwMapID)) {
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
		SboDbgLog("[RES_CHARINFO->DeleteOtherMapChar] charID=%u charMap=%u playerMap=%u pos=(%d,%d)",
			pInfoChar->m_dwCharID,
			pInfoChar->m_dwMapID,
			pInfoCharPlayer->m_dwMapID,
			pInfoChar->m_nMapX,
			pInfoChar->m_nMapY);
#endif
		m_pLibInfoChar->Delete(pInfoChar->m_dwCharID);
		PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount());
		return;
	}

	// 自キャラ？
	if (m_pMgrData->GetCharID() == pInfoChar->m_dwCharID) {
		if ((dwMapIDBack != 0) &&
			((dwMapIDBack != pInfoChar->m_dwMapID) ||
			 (nMapXBack != pInfoChar->m_nMapX) ||
			 (nMapYBack != pInfoChar->m_nMapY))) {
			bPlayerPosChanged = TRUE;
		}
		if (bPlayerPosChanged &&
			((dwMapIDBack != pInfoChar->m_dwMapID) ||
			 (abs(nMapXBack - pInfoChar->m_nMapX) >= MAPPARTSSIZE) ||
			 (abs(nMapYBack - pInfoChar->m_nMapY) >= MAPPARTSSIZE))) {
			bSyncEventTile = TRUE;
		}
		if (bPlayerPosChanged || Packet.m_bChgScreenPos) {
			pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
			// [DBG-CHARINFO-2] SetCenterPos 呼び出し判定をログ出力
			SboDbgLog("[RES_CHARINFO] bPlayerPosChanged=%d bChgScreenPos=%d SetCenterPos呼出=%d 引数=(%d,%d)",
				bPlayerPosChanged, Packet.m_bChgScreenPos,
				(pLayerMap != NULL) ? 1 : 0,
				pInfoChar->m_nMapX, pInfoChar->m_nMapY);
#endif
			if (pLayerMap) {
				pLayerMap->SetCenterPos(pInfoChar->m_nMapX, pInfoChar->m_nMapY);
			}
			RenewItemArea();
			bSyncEventTile = TRUE;
		}
		if (bSyncEventTile && (m_nGameState == GAMESTATE_MAP) && (m_pStateProc != NULL)) {
			((CStateProcMAP *)m_pStateProc)->SyncLastEventTile(
				pInfoChar->m_dwMapID,
				pInfoChar->m_nMapX,
				pInfoChar->m_nMapY);
		}
	}

	if (pInfoChar) {
		PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARINFO, pInfoChar->m_dwCharID);
	}
	PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount());
}


void CMainFrame::RecvProcCHAR_CHARINFO(PBYTE pData)
{
	int i, nCount;
	DWORD dwMapIDBack;
	int nMapXBack, nMapYBack;
	BOOL bResetMoveInterpolation;
	PCInfoCharCli pInfoChar, pInfoCharTmp;
	CPacketCHAR_CHARINFO Packet;

	pInfoChar = NULL;
	Packet.Set(pData);

	nCount = Packet.m_pLibInfo->GetCount();
	for (i = 0; i < nCount; i++) {
		pInfoCharTmp = (PCInfoCharCli)Packet.m_pLibInfo->GetPtr(i);

		// 自キャラ？
		if (m_pMgrData->GetCharID() == pInfoCharTmp->m_dwCharID) {
			continue;
		}
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(pInfoCharTmp->m_dwCharID);
		bResetMoveInterpolation = FALSE;
		if (pInfoChar == NULL) {
			pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetNew(pInfoCharTmp->m_nMoveType);
			pInfoChar->Copy(pInfoCharTmp);
			m_pLibInfoChar->Add(pInfoChar);
			bResetMoveInterpolation = TRUE;
		} else {
			dwMapIDBack = pInfoChar->m_dwMapID;
			nMapXBack = pInfoChar->m_nMapX;
			nMapYBack = pInfoChar->m_nMapY;
			pInfoChar->Copy(pInfoCharTmp);
			if ((dwMapIDBack != pInfoChar->m_dwMapID) ||
				(abs(nMapXBack - pInfoChar->m_nMapX) >= MAPPARTSSIZE) ||
				(abs(nMapYBack - pInfoChar->m_nMapY) >= MAPPARTSSIZE)) {
				bResetMoveInterpolation = TRUE;
			}
		}
		if (bResetMoveInterpolation) {
			pInfoChar->SnapMoveInterpolation();
		}
		pInfoChar->MakeCharGrp();
		m_pLibInfoChar->RenewMotionInfo(pInfoChar);
	}
	// 重なり調整
	m_pLibInfoChar->SortY();

	PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount());
	if (nCount == 1) {
		if (pInfoChar) {
			PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARINFO, pInfoChar->m_dwCharID);
		}
	}
}


void CMainFrame::RecvProcCHAR_MOVE_START(PBYTE pData)
{
	CPacketCHAR_MOVE_START Packet;

	Packet.Set(pData);
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
	// (A') MOVE_START 受信ログ：開始通知の有無と CharID を確認
	SboDbgLog("[MOVE_START] charID=%u dir=%d pos=(%d,%d) bUpdate=%d",
		Packet.m_dwCharID, Packet.m_nDirection,
		Packet.m_pos.x, Packet.m_pos.y, Packet.m_bUpdate);
#endif
	RecvProcCHAR_MOVE_CORE(
			Packet.m_dwCharID,
			Packet.m_nDirection,
			Packet.m_pos.x,
			Packet.m_pos.y,
			Packet.m_bUpdate,
			FALSE);
}


void CMainFrame::RecvProcCHAR_MOVE_DIR_CHANGE(PBYTE pData)
{
	CPacketCHAR_MOVE_DIR_CHANGE Packet;

	Packet.Set(pData);
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
	// (A'') MOVE_DIR_CHANGE 受信ログ：方向変更通知の有無
	SboDbgLog("[MOVE_DIR_CHANGE] charID=%u dir=%d pos=(%d,%d) bUpdate=%d",
		Packet.m_dwCharID, Packet.m_nDirection,
		Packet.m_pos.x, Packet.m_pos.y, Packet.m_bUpdate);
#endif
	RecvProcCHAR_MOVE_CORE(
			Packet.m_dwCharID,
			Packet.m_nDirection,
			Packet.m_pos.x,
			Packet.m_pos.y,
			Packet.m_bUpdate,
			FALSE);
}


void CMainFrame::RecvProcCHAR_MOVE_STOP(PBYTE pData)
{
	CPacketCHAR_MOVE_STOP Packet;

	Packet.Set(pData);
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
	// (A) MOVE_STOP 受信ログ：停止通知が届いているか確認するため
	SboDbgLog("[MOVE_STOP] charID=%u dir=%d pos=(%d,%d) bUpdate=%d",
		Packet.m_dwCharID, Packet.m_nDirection,
		Packet.m_pos.x, Packet.m_pos.y, Packet.m_bUpdate);
#endif
	RecvProcCHAR_MOVE_CORE(
			Packet.m_dwCharID,
			Packet.m_nDirection,
			Packet.m_pos.x,
			Packet.m_pos.y,
			Packet.m_bUpdate,
			TRUE);
}


void CMainFrame::RecvProcCHAR_MOVE_CORE(DWORD dwCharID, int nDirection, int nPacketPosX, int nPacketPosY, BOOL bUpdate, BOOL bForceStop)
{
	int nState, nStateStand, nStateMove, nResult, nStateStop;
	BOOL bDirectionChanged, bPositionChanged;
	BOOL bResult;
	DWORD dwRecvTime;
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	PCLayerMap pLayerMap;

	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);

#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
	// 受信 CharID と自キャラ CharID を比較してログ出力
	SboDbgLog("[MOVE_CORE] pktCharID=%u selfCharID=%u isSelf=%d pos=(%d,%d) bForceStop=%d",
		dwCharID,
		m_pMgrData->GetCharID(),
		(m_pMgrData->GetCharID() == dwCharID) ? 1 : 0,
		nPacketPosX, nPacketPosY, bForceStop);
#endif

	// 自キャラ？
	if (m_pMgrData->GetCharID() == dwCharID) {
		if (m_pStateProc) {
			m_pStateProc->KeyProc(0, FALSE);
		}
		m_pLibInfoChar->SortY();
		return;
	}
	pInfoCharPlayer = m_pMgrData->GetPlayerChar();
	dwRecvTime = timeGetTime();

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		CPacketCHAR_REQ_CHARINFO PacketCHAR_REQ_CHARINFO;

#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
		SboDbgLog("[MOVE_CORE] pInfoChar==NULL → REQ_CHARINFO pktCharID=%u",
			dwCharID);
#endif
		// 画面内？
		if (pLayerMap) {
			bResult = pLayerMap->IsInScreen(nPacketPosX, nPacketPosY);
			if (bResult) {
				PacketCHAR_REQ_CHARINFO.Make(dwCharID);
				m_pSock->Send(&PacketCHAR_REQ_CHARINFO);
			}
		}
		return;
	}
	nState = -1;
	bDirectionChanged = FALSE;
	bPositionChanged = FALSE;

	nStateStand = CHARMOVESTATE_STAND;
	nStateMove = CHARMOVESTATE_MOVE;
	if (pInfoChar->IsStateBattle()) {
		nStateStand = CHARMOVESTATE_BATTLE;
		if (pInfoChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			nStateStand = CHARMOVESTATE_BATTLE_DEFENSE;
		}
		nStateMove = CHARMOVESTATE_BATTLEMOVE;
	}

	bDirectionChanged = (pInfoChar->m_nDirection != nDirection) ? TRUE : FALSE;
	bPositionChanged = !((pInfoChar->m_nMapX == nPacketPosX) && (pInfoChar->m_nMapY == nPacketPosY));

	/*
	   Web 版では短い入力や方向転換で、位置差分がほぼ無い MOVE_DIR_CHANGE が先に届くことがある。
	   ここを STAND 扱いにすると、見る側は向き変更を保留したまま歩行アニメだけが残りやすい。
	   停止通知以外で向きまたは位置が変わった場合は、移動継続として扱う。
	*/
	if (!bForceStop) {
		if (bPositionChanged) {
			nState = nStateMove;
		} else if (bDirectionChanged) {
			/*
			   位置が変わらない向き変更だけのパケットは、その場の向き反映を優先する。
			   ここを常に MOVE 扱いすると、MOVE_STOP 欠落時に足踏みだけが残りやすい。
			   既に移動中のキャラに対する方向転換だけ MOVE 継続とみなす。
			*/
			if (pInfoChar->IsStateMove()) {
				nState = nStateMove;
			} else if (pInfoChar->m_nMoveState != CHARMOVESTATE_SIT) {
				nState = nStateStand;
			}
		}
	} else if (bDirectionChanged && (pInfoChar->m_nMoveState != CHARMOVESTATE_SIT)) {
		nState = nStateStand;
	}

	bResult = TRUE;
	if (pInfoCharPlayer) {
		int nViewRangeX;
		int nViewRangeY;

		nViewRangeX = DRAW_PARTS_X * MAPPARTSSIZE + MAPPARTSSIZE;
		nViewRangeY = DRAW_PARTS_Y * MAPPARTSSIZE + MAPPARTSSIZE;
		bResult = FALSE;
		if (pInfoCharPlayer->m_dwMapID == pInfoChar->m_dwMapID) {
			if ((abs(nPacketPosX - pInfoCharPlayer->m_nMapX) <= nViewRangeX) &&
				(abs(nPacketPosY - pInfoCharPlayer->m_nMapY) <= nViewRangeY)) {
				bResult = TRUE;
			}
		}
	}
	if (bResult == FALSE) {
		// 画面外: ウェイポイントと予測を両方リセット
		nState = CHARMOVESTATE_DELETEREADY;
		pInfoChar->m_bWaypointMove = FALSE;
		pInfoChar->StopPredictedMove(nPacketPosX, nPacketPosY);

	} else if (pInfoChar->IsNPC()) {
		// ─────────────────────────────────────────────────────────────
		// NPC はウェイポイント追従方式（外挿なし）
		//
		// サーバーが 100ms ごとに送ってくる受信点を順に消化するだけで
		// 外挿は不要。Dead Reckoning を使うと補正グライドが向きと無関係な
		// 方向に滑り「右向きのまま左に進む」等の不一致が生じるため。
		// ─────────────────────────────────────────────────────────────

		/*
		   ウェイポイント方式でも「直近の移動同期情報」は更新しておく。
		   RecvProcCHAR_STATE の停止処理が鮮度判定に m_dwPredictRecvTime、
		   停止フォールバック座標に m_nPredictSyncX/Y を参照するため、
		   ここを更新しないと古い座標へ StopPredictedMove(=SetPos) が発火して
		   「数キャラ分先へ一瞬ワープして戻る」症状になる。
		*/
		pInfoChar->m_dwPredictRecvTime = dwRecvTime;
		pInfoChar->m_nPredictSyncX = nPacketPosX;
		pInfoChar->m_nPredictSyncY = nPacketPosY;

		if (bForceStop) {
			// 停止パケット: 現在位置と停止位置の差が大きければ即時スナップ
			nStateStop = CHARMOVESTATE_STAND;
			if (pInfoChar->IsStateBattle()) {
				nStateStop = CHARMOVESTATE_BATTLE;
				if (pInfoChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
					nStateStop = CHARMOVESTATE_BATTLE_DEFENSE;
				}
			}
			int nStopDx = abs(pInfoChar->m_nMapX - nPacketPosX);
			int nStopDy = abs(pInfoChar->m_nMapY - nPacketPosY);
			if (max(nStopDx, nStopDy) > MAPPARTSSIZE) {
				// 大きくズレている: 即時スナップ＋キュークリア
				pInfoChar->m_bWaypointMove = FALSE;
				pInfoChar->StopPredictedMove(nPacketPosX, nPacketPosY);
				pInfoChar->SetDirection(nDirection);
				pInfoChar->ChgMoveState(nStateStop);
			} else {
				// 近距離: 停止ウェイポイントとしてキューに積む
				pInfoChar->AddMovePosQue(nStateStop, nDirection, nPacketPosX, nPacketPosY);
				// ウェイポイント追従が未開始なら開始
				if (!pInfoChar->m_bWaypointMove) {
					pInfoChar->m_bWaypointMove = TRUE;
					pInfoChar->m_bPredictedMove = FALSE;
					pInfoChar->m_dwWaypointLastTime = dwRecvTime;
				}
			}
			nState = -1;
			pInfoChar->m_bRedraw = TRUE;
		} else if (nState == nStateMove) {
			// 移動パケット: ウェイポイントとしてキューに積む
			// 向きは移動ベクトル/到達時に反映するため、ここでは SetDirection しない
			pInfoChar->AddMovePosQue(nState, nDirection, nPacketPosX, nPacketPosY);
			// 暴走防止: 何らかの異常でキューが溜まり続けた場合は古い経由点から間引く
			while (pInfoChar->m_apMovePosQue.size() > 32) {
				pInfoChar->DeleteMovePosQue(0);
			}
			pInfoChar->m_bPredictedMove = FALSE;
			if (!pInfoChar->m_bWaypointMove) {
				// 追従開始: 移動状態へ遷移してウェイポイント処理を起動
				pInfoChar->m_bWaypointMove = TRUE;
				pInfoChar->m_dwWaypointLastTime = dwRecvTime;
				// まだ移動状態でなければ ChgMoveState で移動アニメを開始
				if (!pInfoChar->IsStateMove()) {
					pInfoChar->ChgMoveState(nState);
					nState = -1;
				} else {
					nState = -1;
				}
			} else {
				nState = -1;
			}
		} else {
			// 位置変化なし・向き変更のみ
			if (bDirectionChanged) {
				if (pInfoChar->m_bWaypointMove) {
					// 追従中: 経路の順序を保つためキュー経由で反映する
					pInfoChar->AddMovePosQue(nState, nDirection, nPacketPosX, nPacketPosY);
				} else {
					// 追従していない(立ち状態等)はキューが消化されないため即時反映する
					pInfoChar->SetDirection(nDirection);
					pInfoChar->m_bRedraw = TRUE;
				}
			}
			nState = -1;
		}
	} else {
		// ─────────────────────────────────────────────────────────────
		// PC（他プレイヤー）: 従来の Dead Reckoning 予測を維持
		// パケット頻度が低く外挿なしでは滑らかさが出ないため。
		// ─────────────────────────────────────────────────────────────
		if (bForceStop) {
			nStateStop = CHARMOVESTATE_STAND;
			if (pInfoChar->IsStateBattle()) {
				nStateStop = CHARMOVESTATE_BATTLE;
				if (pInfoChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
					nStateStop = CHARMOVESTATE_BATTLE_DEFENSE;
				}
			}

			pInfoChar->DeleteAllMovePosQue();
			pInfoChar->SetPos(nPacketPosX, nPacketPosY);
			pInfoChar->SetDirection(nDirection);
			pInfoChar->StopPredictedMove(nPacketPosX, nPacketPosY);
			pInfoChar->ChgMoveState(nStateStop);
			nState = -1;
			pInfoChar->m_bRedraw = TRUE;
		} else {
			bResult = pInfoChar->IsStateMove();
			if (bResult) {
				if (bDirectionChanged) {
					pInfoChar->SetDirection(nDirection);
					pInfoChar->m_bRedraw = TRUE;
				}
				if ((nState != -1) ||
					(pInfoChar->m_nMapX != nPacketPosX) ||
					(pInfoChar->m_nMapY != nPacketPosY) ||
					(pInfoChar->m_nDirection != nDirection)) {
					pInfoChar->AddMovePosQue(nState, nDirection, nPacketPosX, nPacketPosY);
				}
				if (nState == nStateMove) {
					pInfoChar->StartPredictedMove(nDirection, nPacketPosX, nPacketPosY, dwRecvTime);
				} else if ((nState == nStateStand) || bForceStop) {
					pInfoChar->StopPredictedMove(nPacketPosX, nPacketPosY);
				}
				nState = -1;
			} else {
				pInfoChar->SetDirection(nDirection);
				if (nState == nStateMove) {
					pInfoChar->StartPredictedMove(nDirection, nPacketPosX, nPacketPosY, dwRecvTime);
				} else {
					pInfoChar->SetPos(nPacketPosX, nPacketPosY);
					pInfoChar->StopPredictedMove(nPacketPosX, nPacketPosY);
				}
				pInfoChar->m_bRedraw = TRUE;
			}
		}
	}

	if (nState != -1) {
		pInfoChar->ChgMoveState(nState);
	}
	m_bRenewCharInfo = TRUE;
}


void CMainFrame::RecvProcCHAR_STATE(PBYTE pData)
{
	BOOL bChgBGM;
	BOOL bKeepPredicted;
	int nState;
	DWORD dwNowTime;
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_STATE Packet;

	Packet.Set(pData);
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
	// (B) CHAR_STATE 受信ログ：どの状態変化が届いているか確認するため
	SboDbgLog("[CHAR_STATE] charID=%u nState=%d",
		Packet.m_dwCharID, Packet.m_nState);
#endif

	pInfoCharPlayer = m_pMgrData->GetPlayerChar();

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}
	nState = Packet.m_nState;
	if (nState == CHARMOVESTATE_DELETE) {
		nState = CHARMOVESTATE_DELETEREADY;
	}
	if ((pInfoChar != pInfoCharPlayer) &&
		(nState == CHARMOVESTATE_DELETEREADY)) {
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
		SboDbgLog("[CHAR_STATE->DeleteRemoteNow] charID=%u mapID=%u pos=(%d,%d)",
			pInfoChar->m_dwCharID,
			pInfoChar->m_dwMapID,
			pInfoChar->m_nMapX,
			pInfoChar->m_nMapY);
#endif
		m_pLibInfoChar->Delete(pInfoChar->m_dwCharID);
		PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount());
		return;
	}

	/*
	   Web の移動同期では MOVE_* / POS_SYNC の方が細かく正しい状態を持っている。
	   リモートキャラに対して短周期で届く STAND/BATTLE 系 CHAR_STATE をそのまま反映すると、
	   ちょんちょん移動や向き変更直後の見た目を潰しやすい。
	   停止系状態は、直近で MoveSync が動いている間だけ MOVE_* 側を優先する。
	*/
	if ((pInfoChar != pInfoCharPlayer) &&
		((nState == CHARMOVESTATE_MOVE) ||
		 (nState == CHARMOVESTATE_BATTLEMOVE))) {
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
		SboDbgLog("[CHAR_STATE->IgnoreRemoteMoveState] charID=%u nState=%d pos=(%d,%d) pred=%d que=%d move=%d",
			pInfoChar->m_dwCharID,
			nState,
			pInfoChar->m_nMapX,
			pInfoChar->m_nMapY,
			pInfoChar->m_bPredictedMove ? 1 : 0,
			(int)pInfoChar->m_apMovePosQue.size(),
			pInfoChar->IsStateMove() ? 1 : 0);
#endif
		pInfoChar->SetChgWait(FALSE);
		return;
	}

	if ((pInfoChar != pInfoCharPlayer) &&
		((nState == CHARMOVESTATE_STAND) ||
		 (nState == CHARMOVESTATE_BATTLE) ||
		 (nState == CHARMOVESTATE_BATTLE_DEFENSE))) {
		DWORD dwNowTimeState;
		DWORD dwPredictAge;
		BOOL bRecentMoveSync;

		dwNowTimeState = SDL_GetTicks();
		dwPredictAge = 0xFFFFFFFF;
		if ((pInfoChar->m_dwPredictRecvTime != 0) &&
			(dwNowTimeState >= pInfoChar->m_dwPredictRecvTime)) {
			dwPredictAge = dwNowTimeState - pInfoChar->m_dwPredictRecvTime;
		}
		bRecentMoveSync = FALSE;
		if ((dwPredictAge <= 400) &&
			(pInfoChar->m_bPredictedMove || (pInfoChar->m_apMovePosQue.size() > 0) || pInfoChar->IsStateMove())) {
			bRecentMoveSync = TRUE;
		} else if (dwPredictAge <= 400) {
			bRecentMoveSync = TRUE;
		}
		if (bRecentMoveSync) {
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
			SboDbgLog("[CHAR_STATE->SkipByMoveSync] charID=%u nState=%d pos=(%d,%d) dt=%u pred=%d que=%d move=%d",
				pInfoChar->m_dwCharID,
				nState,
				pInfoChar->m_nMapX,
				pInfoChar->m_nMapY,
				(dwPredictAge != 0xFFFFFFFF) ? dwPredictAge : 0,
				pInfoChar->m_bPredictedMove ? 1 : 0,
				(int)pInfoChar->m_apMovePosQue.size(),
				pInfoChar->IsStateMove() ? 1 : 0);
#endif
			pInfoChar->SetChgWait(FALSE);
			return;
		}
	}

	if (nState == CHARMOVESTATE_BATTLEATACK) {
		m_pLibInfoChar->RenewMotionInfo(pInfoChar);
	}
	pInfoChar->ChgMoveState(nState);
	pInfoChar->SetChgWait(FALSE);
	bKeepPredicted = FALSE;
	if ((pInfoChar != pInfoCharPlayer) &&
		(nState != CHARMOVESTATE_MOVE) &&
		(nState != CHARMOVESTATE_BATTLEMOVE)) {
		/*
		   Web 版では MOVE_* 直後に CHAR_STATE=STAND/BATTLE が先に届くことがあり、
		   短い移動や向き変更が StopPredictedMove で潰されやすい。
		   直近で移動予測を開始している間は停止処理を少し猶予する。
		*/
		dwNowTime = SDL_GetTicks();
		if ((pInfoChar->m_bPredictedMove || (pInfoChar->m_apMovePosQue.size() > 0)) &&
			(pInfoChar->m_dwPredictRecvTime != 0) &&
			(dwNowTime >= pInfoChar->m_dwPredictRecvTime) &&
			(dwNowTime - pInfoChar->m_dwPredictRecvTime <= 250)) {
			bKeepPredicted = TRUE;
		}
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
		if (bKeepPredicted) {
			SboDbgLog("[CHAR_STATE->KeepPredicted] charID=%u nState=%d pos=(%d,%d) dt=%u",
				pInfoChar->m_dwCharID,
				nState,
				pInfoChar->m_nMapX,
				pInfoChar->m_nMapY,
				dwNowTime - pInfoChar->m_dwPredictRecvTime);
		} else {
			// (C) StopPredictedMove 呼び出しログ：予測移動停止が実際に発火しているか確認するため
			SboDbgLog("[CHAR_STATE->StopPredicted] charID=%u nState=%d pos=(%d,%d)",
				pInfoChar->m_dwCharID, nState, pInfoChar->m_nMapX, pInfoChar->m_nMapY);
		}
#endif
		if (bKeepPredicted == FALSE) {
			int nStopX, nStopY;

			nStopX = pInfoChar->m_nMapX;
			nStopY = pInfoChar->m_nMapY;
			if (pInfoChar->m_nPredictSyncX != 0 || pInfoChar->m_nPredictSyncY != 0) {
				/*
				   Web 版では短い移動の MOVE_STOP が欠けることがある。
				   その場合でも直近の POS_SYNC/MOVE_* で受けた確定位置へ戻して止めると、
				   行き過ぎた位置で足踏みしたまま固まる症状を減らせる。
				*/
				nStopX = pInfoChar->m_nPredictSyncX;
				nStopY = pInfoChar->m_nPredictSyncY;
			}
			pInfoChar->StopPredictedMove(nStopX, nStopY);
		}
	}

	if (pInfoChar == pInfoCharPlayer) {
		bChgBGM = FALSE;
		pInfoMap = m_pMgrData->GetMap();
		if (pInfoMap && (pInfoMap->m_bRecovery == FALSE)) {
			// 気絶した？
			if (pInfoChar->m_nMoveState == CHARMOVESTATE_SWOON) {
				bChgBGM = TRUE;
				m_pMgrWindow->MakeWindowSWOON();
			} else {
				m_pMgrWindow->Delete(WINDOWTYPE_SWOON);
			}
		}
		ChgMoveState(bChgBGM);
	}
}


void CMainFrame::RecvProcCHAR_PROCSTATE(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}
	pInfoChar->SetProcState(Packet.m_dwPara);
}


void CMainFrame::RecvProcCHAR_CHAT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_CHAT Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}
	pInfoChar->SetSpeak(Packet.m_strChat);
	m_pMgrSound->PlaySound(SOUNDID_CHAT);

	PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARCHAT, Packet.m_dwCharID);
	FlashMainWindow();
}


void CMainFrame::RecvProcCHAR_CHARID(PBYTE pData)
{
	int i, nCount;
	PCInfoCharCli pInfoChar;
	CPacketCHAR_CHARID Packet;
	CPacketCHAR_REQ_CHARINFO2 PacketCHAR_CHARINFO2;

	Packet.Set(pData);

	// 知っているキャラを削除する
	nCount = Packet.m_adwCharID.size();
	for (i = nCount - 1; i >= 0; i--) {
		pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_adwCharID[i]);
		// 知らないキャラ？
		if (pInfoChar == NULL) {
			continue;
		}
		Packet.m_adwCharID.erase(Packet.m_adwCharID.begin() + i);
	}
	if (Packet.m_adwCharID.size() <= 0) {
		return;
	}

	// 残ったキャラの情報を要求
	PacketCHAR_CHARINFO2.Make(&Packet.m_adwCharID);
	m_pSock->Send(&PacketCHAR_CHARINFO2);
}


void CMainFrame::RecvProcCHAR_RES_PUTGET(PBYTE pData)
{
	PCInfoItem pInfoItem;
	CPacketCHAR_RES_PUTGET Packet;
	CmyString strTmp;

	Packet.Set(pData);

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(Packet.m_dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	// ブラウザ版(wchar_t 版 vswprintf)では書式中の %s が char* 扱いになり、
	// ワイド文字列を渡す Format("%s...") が正しく整形できない。%s に依存せず
	// 文字列連結で組み立てる（operator+= がプラットフォーム毎のエンコードを処理する）。
	switch (Packet.m_nResult) {
	case RESULTID_CHAR_RES_PUTGET_PUT: // 置いた
		strTmp = (LPCTSTR)pInfoItem->m_strName;
		strTmp += "を置きました";
		break;
	case RESULTID_CHAR_RES_PUTGET_GET: // 取得した
		strTmp = (LPCTSTR)pInfoItem->m_strName;
		strTmp += "を拾いました";
		break;
	case RESULTID_CHAR_RES_PUTGET_DELETE: // 削除した
		strTmp = (LPCTSTR)pInfoItem->m_strName;
		strTmp += "は処分されました";
		break;
	}

	if (strTmp.IsEmpty()) {
		return;
	}
	m_pMgrData->AddSystemMsg(FALSE, strTmp, RGB(255, 255, 255));
	PostMainFrameMessage(MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
}


void CMainFrame::RecvProcCHAR_SET_EFFECT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_SET_EFFECT Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->SetEffectID(Packet.m_dwEffectID);
}


void CMainFrame::RecvProcCHAR_ITEMINFO(PBYTE pData)
{
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_ITEMINFO Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->SetItem(&Packet.m_adwItemID);

	pInfoCharPlayer = m_pMgrData->GetPlayerChar();
	if (pInfoChar == pInfoCharPlayer) {
		PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARINFO, pInfoChar->m_dwCharID);
	}
}


void CMainFrame::RecvProcCHAR_RES_TAIL(PBYTE pData)
{
	PCInfoCharCli pInfoChar, pInfoCharPlayer, pInfoCharTarget, pInfoCharPara;
	CPacketCHAR_RES_TAIL Packet;
	CmyString strTmp, strNamePara;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}
	pInfoCharTarget = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwTargetCharID);
	pInfoCharPara = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwPara);
	pInfoCharPlayer = m_pMgrData->GetPlayerChar();
	if (pInfoChar == pInfoCharPlayer) {
		switch (Packet.m_nResult) {
		case RES_TAIL_RESULT_OK: // ついていく
			if (pInfoCharTarget) {
				strTmp.Format(_T("%sについていきます"), (LPCTSTR)pInfoCharTarget->m_strCharName);
			}
			pInfoCharPlayer->m_dwFrontCharID = Packet.m_dwTargetCharID;
			break;
		case RES_TAIL_RESULT_PARGE: // ついていく解除
			strTmp.Format(_T("ついていくのをやめました"));
			pInfoCharPlayer->m_dwFrontCharID = Packet.m_dwTargetCharID;
			break;
		case RES_TAIL_RESULT_TAILPARGE: // つれていく解除
			pInfoCharPlayer->m_dwTailCharID = 0;
			strTmp.Format(_T("つれていくのをやめました"));
			break;
		case RES_TAIL_RESULT_NG_TAIL: // 既についていっている
			if (pInfoCharPara) {
				strNamePara = (LPCSTR)pInfoCharPara->m_strCharName;
			}
			if (pInfoCharTarget) {
				strTmp.Format(_T("%sには%sがついてきています"), (LPCTSTR)pInfoCharTarget->m_strCharName, (LPCTSTR)strNamePara);
			}
			break;
		case RES_TAIL_RESULT_NG_STATE: // ついていけない状態
			strTmp.Format(_T("%sは取り込み中のようです"), (LPCTSTR)pInfoCharTarget->m_strCharName);
			break;
		case RES_TAIL_RESULT_NG_SWOON: // ついていけない
			strTmp.Format(_T("気絶中なのでついていくことはできません"));
			break;
		case RES_TAIL_RESULT_NG_PLACE: // ついていけない場所
			strTmp.Format(_T("%sがいる場所ではついていくことができません"), (LPCTSTR)pInfoCharTarget->m_strCharName);
			break;
		}
		if (strTmp.IsEmpty() == FALSE) {
			m_pMgrData->AddSystemMsg(FALSE, strTmp, RGB(255, 255, 255));
			PostMainFrameMessage(MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
		}

	} else if (pInfoCharTarget == pInfoCharPlayer) {
		switch (Packet.m_nResult) {
		case RES_TAIL_RESULT_OK:
			strTmp.Format(_T("%sがついてきます"), (LPCTSTR)pInfoChar->m_strCharName);
			m_pMgrData->AddSystemMsg(FALSE, strTmp, RGB(255, 255, 255));
			PostMainFrameMessage(MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
			pInfoCharPlayer->m_dwTailCharID = Packet.m_dwCharID;
			break;
		case RES_TAIL_RESULT_PARGE:
			strTmp.Format(_T("%sはついていくのをやめました"), (LPCTSTR)pInfoChar->m_strCharName);
			m_pMgrData->AddSystemMsg(FALSE, strTmp, RGB(255, 255, 255));
			PostMainFrameMessage(MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
			pInfoCharPlayer->m_dwTailCharID = Packet.m_dwCharID;
			break;
		}
	}
}


void CMainFrame::RecvProcCHAR_MODIFY_PARAM(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_MODIFY_PARAM Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}

	switch (Packet.m_nType) {
	case PARAMID_CHAR_MODIFY_ANIME: // アニメーション番号の変更
		pInfoChar->m_nAnime = Packet.m_nParam;
		break;
	}
}


void CMainFrame::RecvProcCHAR_GRP(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_GRP Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}
	Packet.SetParam(pInfoChar);
	pInfoChar->MakeCharGrp();
	pInfoChar->m_bRedraw = TRUE;
	m_pLibInfoChar->RenewMotionInfo(pInfoChar);
}


void CMainFrame::RecvProcCHAR_MOTION(PBYTE pData)
{
	CPacketCHAR_MOTION Packet;

	Packet.Set(pData);
	m_pLibInfoMotion->SetList(Packet.m_dwMotionTypeID, Packet.m_dwMotionListID, Packet.m_pInfo);
	m_pLibInfoChar->RenewMotionInfo((DWORD)0);

	PostMainFrameMessage(MAINFRAMEMSG_RENEWMOTION, Packet.m_dwMotionListID);
}


void CMainFrame::RecvProcCHAR_MOTIONTYPE(PBYTE pData)
{
	PBYTE pDataTmp;
	CPacketCHAR_MOTIONTYPE Packet;

	Packet.Set(pData);
	pDataTmp = Packet.m_pInfo->GetSendData(Packet.m_dwMotionTypeID);
	m_pLibInfoMotionType->SetSendData(pDataTmp);

	PostMainFrameMessage(MAINFRAMEMSG_RENEWMOTIONTYPE, Packet.m_dwMotionTypeID);
	SAFE_DELETE(pDataTmp);
}


void CMainFrame::RecvProcCHAR_SET_EFCBALLOON(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_SET_EFCBALLOON Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->SetBalloonID(Packet.m_dwBalloonID);
}


void CMainFrame::RecvProcCHAR_SET_MOTION(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}

	if (Packet.m_dwPara == -1) {
		m_pLibInfoChar->RenewMotionInfo(pInfoChar);
	} else {
		m_pLibInfoChar->SetMotionInfo(pInfoChar, CHARMOTIONID_INTERRUUPT, Packet.m_dwPara);
		pInfoChar->InitMotionInfo(CHARMOTIONID_INTERRUUPT);
	}
}


void CMainFrame::RecvProcCHAR_RES_CHECKMAPEVENT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}

	if (Packet.m_dwPara & 0x80000000) {
		int nTileX = (int)((Packet.m_dwPara >> 16) & 0x7FFF);
		int nTileY = (int)(Packet.m_dwPara & 0xFFFF);
		pInfoChar->m_bWaitCheckMapEvent = FALSE;
		if ((m_nGameState == GAMESTATE_MAP) && (m_pStateProc != NULL)) {
			((CStateProcMAP *)m_pStateProc)->StartAutoWalkToEvent(nTileX, nTileY);
		}
	} else {
		pInfoChar->m_bWaitCheckMapEvent = FALSE;
		if ((m_nGameState == GAMESTATE_MAP) && (m_pStateProc != NULL)) {
			((CStateProcMAP *)m_pStateProc)->ResetMapEventCheckSendState();
		}
	}
}


void CMainFrame::RecvProcCHAR_STATUS(PBYTE pData)
{
	DWORD dwHPBack;
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_STATUS Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}
	pInfoCharPlayer = m_pMgrData->GetPlayerChar();

	dwHPBack = pInfoChar->m_dwHP;
	Packet.SetParam(pInfoChar);
	PostMainFrameMessage(MAINFRAMEMSG_RENEWSTATUS, Packet.m_dwCharID);
	if (pInfoChar == pInfoCharPlayer) {
		// HPが減った？
		if (dwHPBack > pInfoChar->m_dwHP) {
			PostMainFrameMessage(MAINFRAMEMSG_DAMAGE, Packet.m_dwCharID);
		}
	}
}


void CMainFrame::RecvProcCHAR_TEXTEFFECT(PBYTE pData)
{
	PCInfoCharCli pInfoChar;
	CPacketCHAR_TEXTEFFECT Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}

	if (Packet.m_dwData == -1) {
		pInfoChar->AddTextEffect((LPCSTR)Packet.m_strData, Packet.m_nColor, Packet.m_nSize);
	} else {
		pInfoChar->AddTextEffect(Packet.m_dwData, Packet.m_nColor, Packet.m_nSize);
	}
}


void CMainFrame::RecvProcCHAR_STATE_CHARGE(PBYTE pData)
{
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_PARA1 Packet;

	Packet.Set(pData);

	pInfoCharPlayer = m_pMgrData->GetPlayerChar();

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->m_bChargeAtack = (Packet.m_dwPara == 0) ? FALSE : TRUE;
	if (pInfoChar == pInfoCharPlayer) {
		ChgMoveState(FALSE);
	}
}


void CMainFrame::RecvProcCHAR_RES_TALKEVENT(PBYTE pData)
{
	PCInfoCharCli pInfoCharPlayer;
	CPacketCHAR_RES_TALKEVENT Packet;

	Packet.Set(pData);

	if (Packet.m_dwParam == 0) {
		pInfoCharPlayer = m_pMgrData->GetPlayerChar();
		if (pInfoCharPlayer) {
			pInfoCharPlayer->m_bWaitCheckMapEvent = FALSE;
		}
		if ((m_nGameState == GAMESTATE_MAP) && (m_pStateProc != NULL)) {
			((CStateProcMAP *)m_pStateProc)->ResetMapEventCheckSendState();
		}
	}

	m_pMgrData->SetInfoTalkEvent(Packet.m_pInfo);

	PostMainFrameMessage(MAINFRAMEMSG_RENEWTALKEVENT, Packet.m_dwParam);
	PostAdminMessage(ADMINMSG_RENEWTALKEVENT, Packet.m_dwParam);
}


void CMainFrame::RecvProcCHAR_SKILLINFO(PBYTE pData)
{
	PCInfoCharCli pInfoChar, pInfoCharPlayer;
	CPacketCHAR_SKILLINFO Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	// 知らないキャラ？
	if (pInfoChar == NULL) {
		return;
	}

	pInfoChar->SetSkill(&Packet.m_adwSkillID);

	pInfoCharPlayer = m_pMgrData->GetPlayerChar();
	if (pInfoChar == pInfoCharPlayer) {
		PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARINFO, pInfoChar->m_dwCharID);
	}
}
