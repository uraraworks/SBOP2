/// @file MainFrameRecvProcCHAR.cpp
/// @brief サーバーメインフレーム(キャラ系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/12/31
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoAccount.h"
#include "LibInfoItem.h"
#include "LibInfoMapBase.h"
#include "LibInfoMapParts.h"
#include "LibInfoSkill.h"
#include "InfoAccount.h"
#include "InfoMapParts.h"
#include "InfoMapEventBase.h"
#include "LibInfoCharSvr.h"
#include "LibInfoTalkEvent.h"
#include "InfoCharSvr.h"
#include "MgrData.h"
#include "TextOutput.h"
#include "MainFrame.h"
#include "../Platform/SvrPlatform.h"
#include "MoveStateDecision.h"
#include "PushDecision.h"

static LPCSTR GetMovePacketName(int nCmdSub)
{
	switch (nCmdSub) {
	case SBOCOMMANDID_SUB_CHAR_MOVE_START:
		return "MOVE_START";
	case SBOCOMMANDID_SUB_CHAR_MOVE_DIR_CHANGE:
		return "MOVE_DIR_CHANGE";
	case SBOCOMMANDID_SUB_CHAR_MOVE_STOP:
		return "MOVE_STOP";
	case SBOCOMMANDID_SUB_CHAR_MOVEPOS:
		return "MOVEPOS";
	}
	return "UNKNOWN";
}

// ─────────────────────────────────────────────
// S3a: 押せる物(Push=1 NPC)への押し要求。docs/push-object-redesign.md 参照。
// PushDecision(純粋関数)へ渡すための小さなヘルパー群。
// ─────────────────────────────────────────────

// 接触とみなすすき間の許容(px)。docs/push-object-redesign.md 2章4項の値。
static const int PUSH_CONTACT_GAP_ALLOWED_PX = 4;
// 申告された本人座標とサーバー座標の許容ズレ(px)。移動速度超過チェックの
// +32px余裕(MainFrameRecvProcCHAR.cpp内 nAllowDist計算)に合わせた。
static const int PUSH_SELF_POS_TOLERANCE_PX = MAPPARTSSIZE;
// 却下時のRES_PUSH補正送信は毎回行うことにしたため未使用(現在は
// PushDecision::REASON_NOT_CONTACT等ログの間引きにはPUSH_REJECT_LOG_INTERVAL_MSのみ使用)。
// CInfoCharSvr::m_dwLastPushRejectSyncTimeも同様に未使用。将来また間引きが
// 必要になった場合のために定義自体は残す。
static const DWORD PUSH_REJECT_SYNC_INTERVAL_MS = 1000;
// 経過時間(dwElapsedMs)の上限(ms)。クライアントは約100ms間隔でREQ_PUSHを送るため、
// 前回受理が無い最初の1回をこの値扱いにしないと許容距離が0px相当になり毎回却下される。
// 逆に上限を設けないと、しばらく送らずに溜めてから送ることで1回で大きく押せてしまう
// (悪用対策)。速度超過チェックの許容間隔より少し余裕を持たせた値。
static const DWORD PUSH_ELAPSED_MAX_MS = 300;
// クライアント時刻の逆行がこの値(ms)以上なら、SDL_GetTicks系の時刻が
// ページ再読み込み等で0から数え直されたもの(=クライアント再起動)とみなし、
// 却下せず基準(m_dwLastPushClientTime)を取り直す。再ログイン後は
// CLibInfoCharSvr::LogIn()でも0にリセットしているが、同一セッション内で
// クライアントだけ再読み込みされるケースの保険として、ここでも吸収する。
static const DWORD PUSH_RELOAD_REWIND_THRESHOLD_MS = 5000;
// 却下ログの出力間隔(ms)。100ms間隔の連続却下でログが溢れないよう、
// 補正送信の間引き(PUSH_REJECT_SYNC_INTERVAL_MS)とは別に1秒に1回までに絞る。
static const DWORD PUSH_REJECT_LOG_INTERVAL_MS = 1000;
// 受理ログの出力間隔(ms)。却下ログと同じく1秒に1回までに絞る。
static const DWORD PUSH_ACCEPT_LOG_INTERVAL_MS = 1000;

// 診断用。通常は0。原因調査時に1にする。
// 却下ログの詳細化・受理ログの追加を行う。
#define PUSH_DEBUG_LOG 0

// 却下理由(PushDecision::REASON)を文字列にする。診断ログ用。
static LPCSTR GetPushRejectReasonName(PushDecision::REASON eReason)
{
	switch (eReason) {
	case PushDecision::REASON_NONE:                 return "NONE";
	case PushDecision::REASON_SPEED_OVER:           return "SPEED_OVER";
	case PushDecision::REASON_AXIS_INVALID:         return "AXIS_INVALID";
	case PushDecision::REASON_SELF_POS_MISMATCH:    return "SELF_POS_MISMATCH";
	case PushDecision::REASON_NOT_CONTACT:          return "NOT_CONTACT";
	case PushDecision::REASON_SWAP_NOT_OWNER:       return "SWAP_NOT_OWNER";
	case PushDecision::REASON_SWAP_TARGET_MISMATCH: return "SWAP_TARGET_MISMATCH";
	case PushDecision::REASON_SWAP_LATERAL_CHANGE:  return "SWAP_LATERAL_CHANGE";
	case PushDecision::REASON_SWAP_TOO_FAR:         return "SWAP_TOO_FAR";
	}
	return "UNKNOWN";
}

// PushDecision::CheckSpeed と同じ式で許容距離を計算する(診断ログ用)。
static double GetPushAllowedDistance(unsigned int dwElapsedMs, int nPixelsPerSec)
{
	return (static_cast<double>(dwElapsedMs) / 1000.0) * nPixelsPerSec * 1.1 + 8.0;
}

#if PUSH_DEBUG_LOG
// 診断用。原因特定後に無効化。
// RecvProcCHAR_REQ_PUSH内で「本人(pInfoPlayer)がまだ特定できていない」段階の
// 黙殺returnログ(向き範囲外・本人セッションが見つからない)を、セッションID単位で
// 1秒1回までに抑制するための簡易マップ。本人特定後はCInfoCharSvr側のメンバで抑制する。
#include <map>
static std::map<DWORD, DWORD> s_mapPushPreLogTime;

// dwSessionIDについて、dwNowTimeを基準に1秒1回までログを許可するか判定する。
// 許可する場合はs_mapPushPreLogTimeを更新してTRUEを返す。
static BOOL AllowPushPreLog(DWORD dwSessionID, DWORD dwNowTime)
{
	std::map<DWORD, DWORD>::iterator it = s_mapPushPreLogTime.find(dwSessionID);
	if ((it != s_mapPushPreLogTime.end()) && ((dwNowTime - it->second) < PUSH_REJECT_LOG_INTERVAL_MS)) {
		return FALSE;
	}
	s_mapPushPreLogTime[dwSessionID] = dwNowTime;
	return TRUE;
}
#endif

static PushDecision::RECT_PX ToPushRect(const RECT &rc)
{
	PushDecision::RECT_PX r;
	r.nLeft		= rc.left;
	r.nTop		= rc.top;
	r.nRight	= rc.right;
	r.nBottom	= rc.bottom;
	return r;
}

static void PushDirVector(int nDir, int &dx, int &dy)
{
	dx = 0;
	dy = 0;
	switch (nDir) {
	case PushDecision::DIR_UP:    dy = -1; break;
	case PushDecision::DIR_DOWN:  dy =  1; break;
	case PushDecision::DIR_LEFT:  dx = -1; break;
	case PushDecision::DIR_RIGHT: dx =  1; break;
	}
}

// 押せる物が1px先へ進めるか、押している本人・押せる物自身を除く同じマップの
// 全キャラ(m_bBlockは見ない)と当たるかを見る。
// S3b: 判定本体は Common::CLibInfoCharBase::IsPushAreaFree（クライアントの押し予測と
// 共用）へ移設済み。ここは PushDecision::RECT_PX ⇔ RECT の橋渡しだけを行う。
static BOOL IsPushCharAreaFree(
	CLibInfoCharSvr *pLibInfoChar,
	DWORD dwMapID,
	PCInfoCharSvr pExclude1,
	PCInfoCharSvr pExclude2,
	const PushDecision::RECT_PX &rcMoveTo)
{
	RECT rcTmp;

	SetRect(&rcTmp, rcMoveTo.nLeft, rcMoveTo.nTop, rcMoveTo.nRight, rcMoveTo.nBottom);
	return pLibInfoChar->IsPushAreaFree(pExclude1, pExclude2, dwMapID, rcTmp);
}

// 押せる物がマップ的に1px先(nDir)へ進めるか。
// クライアントの自キャラ移動判定と同じ CLibInfoCharBase::CanMoveDirection
// (Common/LibInfo/LibInfoCharBase.cpp。S3でクライアントと共用するために移設した)を使う。
// nCurX/nCurYは呼び出し側が1pxずつ進めている「確定済み」座標で、pInfoObjの実座標は
// 一時的にしか書き換えない(最終的な反映はDecidePushの結果を見てから行う)。
static BOOL IsPushMapFree(CLibInfoCharBase *pLibInfoChar, CInfoMapBase *pInfoMap, PCInfoCharSvr pInfoObj, int nCurX, int nCurY, int nDir)
{
	int nSaveX, nSaveY;
	BOOL bResult;

	if (pInfoMap == NULL) {
		return FALSE;
	}

	nSaveX = pInfoObj->m_nMapX;
	nSaveY = pInfoObj->m_nMapY;
	pInfoObj->m_nMapX = nCurX;
	pInfoObj->m_nMapY = nCurY;
	bResult = pLibInfoChar->CanMoveDirection(pInfoMap, pInfoObj, nDir);
	pInfoObj->m_nMapX = nSaveX;
	pInfoObj->m_nMapY = nSaveY;
	return bResult;
}

// ─────────────────────────────────────────────
// S5: 入れ替わり(SWAP)。docs/push-object-redesign.md 2章7項参照。
// ─────────────────────────────────────────────

// 専有者からSWAP/PUSH要求がこの間隔(ms)以上途絶えたら、入れ替わりを打ち切る。
static const DWORD PUSH_SWAP_OWNER_TIMEOUT_MS = 2000;

static int PushOppositeDir(int nDir)
{
	switch (nDir) {
	case PushDecision::DIR_UP:    return PushDecision::DIR_DOWN;
	case PushDecision::DIR_DOWN:  return PushDecision::DIR_UP;
	case PushDecision::DIR_LEFT:  return PushDecision::DIR_RIGHT;
	case PushDecision::DIR_RIGHT: return PushDecision::DIR_LEFT;
	}
	return nDir;
}

static PushDecision::POINT_PX ToPushPoint(const POINT &pt)
{
	PushDecision::POINT_PX p;
	p.x = pt.x;
	p.y = pt.y;
	return p;
}

// 押せる物(pInfoObj)がnStepDir方向へ1pxずつ進めるかを見るIsPositionFreeFuncを作る。
// nCurX/nCurYは呼び出し元のローカル変数への参照(呼ばれるたびに1pxずつ進める
// 「確定済み」座標)。pExcludeは当たり判定から除外するキャラ(押している本人、
// または入れ替わり中の専有者)。IsPushCharAreaFree/IsPushMapFreeは通常の押し
// (RecvProcCHAR_REQ_PUSH内のisFree)と同じものを使う。
static PushDecision::IsPositionFreeFunc MakeIsFreeForObjStepper(
	CLibInfoCharSvr *pLibInfoChar,
	CInfoMapBase *pInfoObjMap,
	PCInfoCharSvr pInfoObj,
	PCInfoCharSvr pExclude,
	int nStepDir,
	int &nCurX,
	int &nCurY)
{
	return [&nCurX, &nCurY, pLibInfoChar, pInfoObjMap, pInfoObj, pExclude, nStepDir](const PushDecision::RECT_PX &rcMoveTo) -> bool {
		if (!IsPushMapFree(pLibInfoChar, pInfoObjMap, pInfoObj, nCurX, nCurY, nStepDir)) {
			return false;
		}
		if (!IsPushCharAreaFree(pLibInfoChar, pInfoObj->m_dwMapID, pExclude, pInfoObj, rcMoveTo)) {
			return false;
		}
		int nStepX, nStepY;
		PushDirVector(nStepDir, nStepX, nStepY);
		nCurX += nStepX;
		nCurY += nStepY;
		return true;
	};
}

// PUSHTYPE_SWAP要求(入れ替わり)を処理する。開始判定・継続検証・向き変更検出
// による自走(eject)開始のいずれもここでまとめて行い、RES_PUSHの送信まで行う。
// 呼び出し時点で「専有者以外からの要求」「eject中」は呼び出し元で既に
// 却下・returnされている前提(専有者、またはまだ誰も専有していない状態のみ)。
static void ProcSwapPush(
	CTextOutput *pLog,
	CLibInfoCharSvr *pLibInfoChar,
	CUraraSockTCPSBO *pSock,
	PCInfoCharSvr pInfoPlayer,
	PCInfoCharSvr pInfoObj,
	DWORD dwSessionID,
	DWORD dwNowTime,
	CPacketCHAR_REQ_PUSH &Packet,
	int nDir,
	const PushDecision::RECT_PX &rcSelf,
	const PushDecision::RECT_PX &rcObj,
	PCInfoMapBase pInfoObjMap)
{
	CPacketCHAR_RES_PUSH PacketResPush;
	POINT ptFinal;
	BOOL bAccepted = FALSE;
	PushDecision::POINT_PX ptAccepted;
	ptAccepted.x = pInfoObj->m_nMapX;
	ptAccepted.y = pInfoObj->m_nMapY;

	// GetCollisionRectは端を含む矩形(横x..x+31、縦y-15..y)なので、
	// right-left/bottom-topでは1px不足する(31/15)。+1して実サイズ(32/16)にする
	// (クライアント側も同様に修正。docs/push-object-redesign.md 2章7項)。
	int nSelfWidthAlongDir = (nDir == PushDecision::DIR_LEFT || nDir == PushDecision::DIR_RIGHT)
		? (rcSelf.nRight - rcSelf.nLeft + 1)
		: (rcSelf.nBottom - rcSelf.nTop + 1);

	PushDecision::POINT_PX ptSelfNow = ToPushPoint(Packet.m_ptSelf);
	PushDecision::POINT_PX ptRequestedObj = ToPushPoint(Packet.m_ptObjTarget);

#if PUSH_DEBUG_LOG
	// 診断用。原因特定後に無効化。
	if (pLog != NULL) {
		pLog->Write("SWAP受信 s:%u act:%d dir:%d tgt:%d,%d self:%d,%d obj:%d,%d",
			dwSessionID, pInfoObj->m_bSwapActive, nDir,
			ptRequestedObj.x, ptRequestedObj.y, ptSelfNow.x, ptSelfNow.y,
			pInfoObj->m_nMapX, pInfoObj->m_nMapY);
	}
#endif

	if (!pInfoObj->m_bSwapActive) {
		// 未開始: 開始条件を検証する。
		// クライアントは最初のSWAP要求を送るまでに既にk px本人がボールへ
		// めり込んで進んでいる(ボタン押下から最初のSWAP判定までに数px〜十数px
		// 進んでいることがある)ため、P0(開始時の本人位置)はサーバーが持つ
		// 「今」の本人座標ではなく、今回の要求(目標ボール座標・申告自己座標)
		// から逆算する(親からの追加指示。docs/push-object-redesign.md 2章7項)。
		// これによりクライアント側の目標計算 B0-(P-P0) と構造的に一致する。
		BOOL bIsBall = (pInfoObj->m_nMoveType == CHARMOVETYPE_BALL);
		int nRetreatDir = PushOppositeDir(nDir);
		PushDecision::POINT_PX ptB0;
		ptB0.x = pInfoObj->m_nMapX;
		ptB0.y = pInfoObj->m_nMapY;

		int nAxisDx, nAxisDy;
		PushDirVector(nDir, nAxisDx, nAxisDy);

		// k: 要求されたボール目標がB0から-d方向へ動いている量(d軸成分)。
		// 横方向がB0からずれていれば却下。
		BOOL bLateralOK;
		int k;
		if (nAxisDx != 0) {
			bLateralOK = (ptRequestedObj.y == ptB0.y);
			k = (ptB0.x - ptRequestedObj.x) * nAxisDx;
		} else {
			bLateralOK = (ptRequestedObj.x == ptB0.x);
			k = (ptB0.y - ptRequestedObj.y) * nAxisDy;
		}

		if (bLateralOK && (k >= 0) && (k <= nSelfWidthAlongDir)) {
			PushDecision::POINT_PX ptP0;
			ptP0.x = ptSelfNow.x - k * nAxisDx;
			ptP0.y = ptSelfNow.y - k * nAxisDy;

			// 接触判定は「導出したP0の本人矩形」と「B0のボール矩形」で行う
			// (今の本人はkpxめり込んでおり、接触許容±4pxに収まらないため)。
			PushDecision::RECT_PX rcSelfAtP0;
			rcSelfAtP0.nLeft   = rcSelf.nLeft   - k * nAxisDx;
			rcSelfAtP0.nRight  = rcSelf.nRight  - k * nAxisDx;
			rcSelfAtP0.nTop    = rcSelf.nTop    - k * nAxisDy;
			rcSelfAtP0.nBottom = rcSelf.nBottom - k * nAxisDy;

			int nCurXFwd = ptB0.x;
			int nCurYFwd = ptB0.y;
			PushDecision::IsPositionFreeFunc isFreeForObjFwd =
				MakeIsFreeForObjStepper(pLibInfoChar, pInfoObjMap, pInfoObj, pInfoPlayer, nDir, nCurXFwd, nCurYFwd);

			int nCurXRet = ptB0.x;
			int nCurYRet = ptB0.y;
			PushDecision::IsPositionFreeFunc isFreeForObjRetreat =
				MakeIsFreeForObjStepper(pLibInfoChar, pInfoObjMap, pInfoObj, pInfoPlayer, nRetreatDir, nCurXRet, nCurYRet);

			BOOL bContact = PushDecision::CheckContact(rcSelfAtP0, rcObj, nDir, PUSH_CONTACT_GAP_ALLOWED_PX);
			BOOL bCanStart = PushDecision::CanStartSwap(bIsBall, rcObj, nDir, isFreeForObjFwd);
			BOOL bRetreatClear = bCanStart && PushDecision::IsSwapRetreatPathClear(rcObj, nDir, nSelfWidthAlongDir, isFreeForObjRetreat);

#if PUSH_DEBUG_LOG
			// 診断用。原因特定後に無効化。
			if (pLog != NULL) {
				pLog->Write("SWAP開始判定 lat:%d k:%d w:%d ct:%d cs:%d rt:%d",
					bLateralOK, k, nSelfWidthAlongDir, bContact, bCanStart, bRetreatClear);
			}
#endif

			if (bContact && bCanStart && bRetreatClear) {
				PushDecision::SWAP_STATE state = PushDecision::StartSwap(dwSessionID, ptP0, ptB0, nDir);

				pInfoObj->m_bSwapActive = TRUE;
				pInfoObj->m_dwSwapOwnerSessionID = dwSessionID;
				pInfoObj->m_ptSwapP0.x = ptP0.x;
				pInfoObj->m_ptSwapP0.y = ptP0.y;
				pInfoObj->m_ptSwapB0.x = ptB0.x;
				pInfoObj->m_ptSwapB0.y = ptB0.y;
				pInfoObj->m_nSwapDir = nDir;
				pInfoObj->m_dwLastSwapReqTime = dwNowTime;

				// 開始した今回の要求も直後にUpdateSwapへ渡して検証・受理する
				// (P0を上記の通り逆算しているため、この最初の要求は構造的に
				// 目標一致するはずで、正常なら受理される)。
				int nCurXRet2 = ptB0.x;
				int nCurYRet2 = ptB0.y;
				PushDecision::IsPositionFreeFunc isFreeForObjRetreat2 =
					MakeIsFreeForObjStepper(pLibInfoChar, pInfoObjMap, pInfoObj, pInfoPlayer, nRetreatDir, nCurXRet2, nCurYRet2);

				PushDecision::SWAP_UPDATE_RESULT updateResult = PushDecision::UpdateSwap(
					state, dwSessionID, ptSelfNow, nDir, ptRequestedObj, rcObj,
					nSelfWidthAlongDir, isFreeForObjRetreat2);

#if PUSH_DEBUG_LOG
				// 診断用。原因特定後に無効化。
				if (pLog != NULL) {
					pLog->Write("SWAP開始後初回 acc:%d end:%d rs:%d",
						updateResult.bAccepted, updateResult.bShouldEnd, updateResult.eReason);
				}
#endif

				if (updateResult.bAccepted) {
					bAccepted = TRUE;
					ptAccepted = updateResult.ptAcceptedObj;
					if (updateResult.bShouldEnd) {
						pInfoObj->m_bSwapActive = FALSE;
					}
				}
				// 却下ならbAccepted=FALSEのまま(開始はしたが今回の1回分は
				// 受理しない。次のSWAP要求で再検証される)。
			}
		}
		// 開始条件を満たさなければ何もしない(bAccepted=FALSEのまま、現在座標を返す)。

	} else {
		// 入れ替わり中の更新(専有者からの要求であることは呼び出し元で確定済み)。
		PushDecision::SWAP_STATE state;
		state.bActive = TRUE;
		state.dwOwnerSessionID = pInfoObj->m_dwSwapOwnerSessionID;
		state.ptP0.x = pInfoObj->m_ptSwapP0.x;
		state.ptP0.y = pInfoObj->m_ptSwapP0.y;
		state.ptB0.x = pInfoObj->m_ptSwapB0.x;
		state.ptB0.y = pInfoObj->m_ptSwapB0.y;
		state.nDir = pInfoObj->m_nSwapDir;

		pInfoObj->m_dwLastSwapReqTime = dwNowTime;

		int nRetreatDir = PushOppositeDir(state.nDir);
		int nCurXRet = state.ptB0.x;
		int nCurYRet = state.ptB0.y;
		PushDecision::IsPositionFreeFunc isFreeForObjRetreat =
			MakeIsFreeForObjStepper(pLibInfoChar, pInfoObjMap, pInfoObj, pInfoPlayer, nRetreatDir, nCurXRet, nCurYRet);

		// rcObjはpInfoObjの"今の"当たり矩形。UpdateSwap内部の経路確認はB0基準の
		// 相対位置で行うため、B0時点の矩形に組み直して渡す(入れ替わり中、
		// pInfoObjは毎回SetPos(ptAcceptedObj)されているのでサイズは不変)。
		PushDecision::RECT_PX rcObjAtB0;
		{
			int nOffX = pInfoObj->m_nMapX - state.ptB0.x;
			int nOffY = pInfoObj->m_nMapY - state.ptB0.y;
			rcObjAtB0.nLeft   = rcObj.nLeft   - nOffX;
			rcObjAtB0.nTop    = rcObj.nTop    - nOffY;
			rcObjAtB0.nRight  = rcObj.nRight  - nOffX;
			rcObjAtB0.nBottom = rcObj.nBottom - nOffY;
		}

		PushDecision::SWAP_UPDATE_RESULT updateResult = PushDecision::UpdateSwap(
			state, dwSessionID, ptSelfNow, nDir, ptRequestedObj, rcObjAtB0,
			nSelfWidthAlongDir, isFreeForObjRetreat);

#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。mv=本人の移動量(P0からの軸成分)。
		if (pLog != NULL) {
			int nMvAxisDx, nMvAxisDy;
			PushDirVector(state.nDir, nMvAxisDx, nMvAxisDy);
			int nMv = (ptSelfNow.x - state.ptP0.x) * nMvAxisDx + (ptSelfNow.y - state.ptP0.y) * nMvAxisDy;
			pLog->Write("SWAP更新 acc:%d end:%d rs:%d mv:%d tgt:%d,%d",
				updateResult.bAccepted, updateResult.bShouldEnd, updateResult.eReason,
				nMv, ptRequestedObj.x, ptRequestedObj.y);
		}
#endif

		if (updateResult.bAccepted) {
			bAccepted = TRUE;
			ptAccepted = updateResult.ptAcceptedObj;
			if (updateResult.bShouldEnd) {
				pInfoObj->m_bSwapActive = FALSE;
			}
		} else if (updateResult.bShouldEnd) {
			// 向き変更を検出。入れ替わりを終え、自走(eject)を開始する。
			pInfoObj->m_bSwapActive = FALSE;
			pInfoObj->m_bEjectActive = TRUE;
			// DB配置のNPC(ボール等)は SetLibInfoChar されておらず m_pLibInfoCharSvr が
			// NULLのため、自走処理(ProcEjectMove)が最初のreturnで黙って終わり転がらない。
			// 自走を始める時に必ずセットする。
			pInfoObj->SetLibInfoChar(pLibInfoChar);
			pInfoObj->m_dwEjectOwnerCharID = pInfoPlayer->m_dwCharID;
			pInfoObj->m_nEjectDir = nDir;
			pInfoObj->m_dwLastEjectProcTime = dwNowTime;
			pInfoObj->m_dEjectPxRemainder = 0.0;
#if PUSH_DEBUG_LOG
			// 診断用。原因特定後に無効化。
			if (pLog != NULL) {
				pLog->Write("EJECT開始 dir:%d obj:%d,%d", nDir, pInfoObj->m_nMapX, pInfoObj->m_nMapY);
			}
#endif
			pInfoObj->SetDirection(nDir);
			bAccepted = TRUE; // 仕様: RES_PUSHは受理・現在座標で返す
			ptAccepted.x = pInfoObj->m_nMapX;
			ptAccepted.y = pInfoObj->m_nMapY;
		}
		// それ以外の却下(目標不一致・横方向変化・塞がり等)はbAccepted=FALSEの
		// まま、SWAP状態はそのまま維持する(クライアントは補正座標を受けて
		// 送り直す)。
	}

	if (bAccepted) {
		pInfoObj->SetPos(ptAccepted.x, ptAccepted.y);
		// ボールの向きはdの逆(見る側の推測航法が向きで先読みするため)。
		// ただしeject開始直後は既にSetDirection(nDir)を呼んでいるので、
		// m_bSwapActiveがFALSEになっている場合は上書きしない。
		if (pInfoObj->m_bSwapActive) {
			pInfoObj->SetDirection(PushOppositeDir(nDir));
		}
		{
			int nMoveStateOnMove = pInfoObj->IsStateBattle() ? CHARMOVESTATE_BATTLEMOVE : CHARMOVESTATE_MOVE;
			if (pInfoObj->m_nMoveState != nMoveStateOnMove) {
				pInfoObj->SetMoveState(nMoveStateOnMove);
			}
		}
		pInfoObj->m_bChgPos = TRUE;
		pInfoObj->m_dwLastPushedTime = dwNowTime;
		pInfoObj->m_dwPushingCharID = pInfoPlayer->m_dwCharID;

		ptFinal.x = ptAccepted.x;
		ptFinal.y = ptAccepted.y;
		PacketResPush.Make(pInfoObj->m_dwCharID, ptFinal, TRUE);
		pSock->SendTo(dwSessionID, &PacketResPush);
	} else {
		ptFinal.x = pInfoObj->m_nMapX;
		ptFinal.y = pInfoObj->m_nMapY;
		PacketResPush.Make(pInfoObj->m_dwCharID, ptFinal, FALSE);
		pSock->SendTo(dwSessionID, &PacketResPush);
	}
}

void CMainFrame::RecvProcCHAR(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO:	RecvProcCHAR_REQ_CHARINFO(pData, dwSessionID);	break;	// キャラ情報要求
#if SBO_ENABLE_LEGACY_MOVEPOS_RECV
	case SBOCOMMANDID_SUB_CHAR_MOVEPOS:	RecvProcCHAR_MOVEPOS(pData, dwSessionID);	break;	// 移動通知
#else
	case SBOCOMMANDID_SUB_CHAR_MOVEPOS:
		m_pLog->Write("旧MOVEPOS受信を拒否 dwSessionID:%u [理由:互換受信無効]", dwSessionID);
		break;
#endif
	case SBOCOMMANDID_SUB_CHAR_MOVE_START:	RecvProcCHAR_MOVEPOS(pData, dwSessionID);	break;	// 移動開始通知(Dead Reckoning)
	case SBOCOMMANDID_SUB_CHAR_MOVE_DIR_CHANGE:	RecvProcCHAR_MOVEPOS(pData, dwSessionID);	break;	// 移動方向変更通知(Dead Reckoning)
	case SBOCOMMANDID_SUB_CHAR_MOVE_STOP:	RecvProcCHAR_MOVEPOS(pData, dwSessionID);	break;	// 移動停止通知(Dead Reckoning)
	case SBOCOMMANDID_SUB_CHAR_POS_SYNC:	break;	// 座標同期通知(Dead Reckoning): サーバー受信対象外
	case SBOCOMMANDID_SUB_CHAR_STATE:	RecvProcCHAR_STATE(pData, dwSessionID);	break;	// 状態通知
	case SBOCOMMANDID_SUB_CHAR_REQ_CHAT:	RecvProcCHAR_REQ_CHAT(pData, dwSessionID);	break;	// チャット要求
	case SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO2:	RecvProcCHAR_REQ_CHARINFO2(pData, dwSessionID);	break;	// キャラ情報要求(複数)
	case SBOCOMMANDID_SUB_CHAR_REQ_PUTGET:	RecvProcCHAR_REQ_PUTGET(pData, dwSessionID);	break;	// アイテムを拾う置く要求
	case SBOCOMMANDID_SUB_CHAR_REQ_USEITEM:	RecvProcCHAR_REQ_USEITEM(pData, dwSessionID);	break;	// アイテムを使う要求
	case SBOCOMMANDID_SUB_CHAR_REQ_DRAGITEM:	RecvProcCHAR_REQ_DRAGITEM(pData, dwSessionID);	break;	// アイテム位置変更要求
	case SBOCOMMANDID_SUB_CHAR_REQ_PUSH:	RecvProcCHAR_REQ_PUSH(pData, dwSessionID);	break;	// 押す要求
	case SBOCOMMANDID_SUB_CHAR_REQ_TAIL:	RecvProcCHAR_REQ_TAIL(pData, dwSessionID);	break;	// 付いて行く要求
	case SBOCOMMANDID_SUB_CHAR_REQ_MODIFY_PARAM:	RecvProcCHAR_REQ_MODIFY_PARAM(pData, dwSessionID);	break;	// パラメータ変更要求
	case SBOCOMMANDID_SUB_CHAR_REQ_EQUIP:	RecvProcCHAR_REQ_EQUIP(pData, dwSessionID);	break;	// 装備要求
	case SBOCOMMANDID_SUB_CHAR_PROC_FISHING:	RecvProcCHAR_PROC_FISHING(pData, dwSessionID);	break;	// 釣り要求
	case SBOCOMMANDID_SUB_CHAR_REQ_CHECKMAPEVENT:	RecvProcCHAR_REQ_CHECKMAPEVENT(pData, dwSessionID);	break;	// マップイベントチェック要求
	case SBOCOMMANDID_SUB_CHAR_STATE_CHARGE:	RecvProcCHAR_STATE_CHARGE(pData, dwSessionID);	break;	// 溜め状態通知
	case SBOCOMMANDID_SUB_CHAR_REQ_RECOVERY:	RecvProcCHAR_REQ_RECOVERY(pData, dwSessionID);	break;	// 気絶後復活要求
	case SBOCOMMANDID_SUB_CHAR_REQ_TALKEVENT:	RecvProcCHAR_REQ_TALKEVENT(pData, dwSessionID);	break;	// 会話イベント情報要求
	case SBOCOMMANDID_SUB_CHAR_REQ_ADDSKILL:	RecvProcCHAR_REQ_ADDSKILL(pData, dwSessionID);	break;	// スキル追加要求
	case SBOCOMMANDID_SUB_CHAR_REQ_USESKILL:	RecvProcCHAR_REQ_USESKILL(pData, dwSessionID);	break;	// スキルを使う要求
	}
}

void CMainFrame::RecvProcCHAR_REQ_CHARINFO(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharBase pInfoChar;
	CPacketCHAR_REQ_CHARINFO Packet;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}

	PacketRES_CHARINFO.Make(pInfoChar);
	m_pSock->SendTo(dwSessionID, &PacketRES_CHARINFO);
}

void CMainFrame::RecvProcCHAR_MOVEPOS(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	int i, nCount, nResult, nCmdSub, nStopState;
	int nNextPosX, nNextPosY, nMoveDist, nAllowDist, nSpeedLevel;
	int nCharPixelsPerSec, nAllowPixelsPerSec;
	DWORD dwNowTime = 0, dwElapsed, dwPacketTime;
	BOOL bHasPacketTime;
	LPCSTR pszPacketName;
	PCInfoCharSvr pInfoChar, pInfoCharTmp, pInfoCharFront;
	DWORD dwMapID, dwCharID;
	int nDirection, nPacketPosX, nPacketPosY;
	BOOL bUpdate;
	BOOL bPosChanged;
	CPacketBase PacketBase;
	CPacketBase *pRelayPacket;
	std::vector<PCInfoCharSvr> apInfoChar;

	CPacketCHAR_MOVEPOS PacketMovePos;
	CPacketCHAR_MOVE_START PacketMoveStart;
	CPacketCHAR_MOVE_DIR_CHANGE PacketMoveDirChange;
	CPacketCHAR_MOVE_STOP PacketMoveStop;
	CPacketCHAR_POS_SYNC PacketPosSync;

	PacketBase.Set(pData);
	pRelayPacket = NULL;
	nCmdSub = PacketBase.m_byCmdSub;

	bUpdate = FALSE;
	dwPacketTime = 0;
	bHasPacketTime = FALSE;
	nSpeedLevel = 1;
	pszPacketName = GetMovePacketName(nCmdSub);

	switch (nCmdSub) {
	case SBOCOMMANDID_SUB_CHAR_MOVE_START:
		PacketMoveStart.Set(pData);
		dwMapID = PacketMoveStart.m_dwMapID;
		dwCharID = PacketMoveStart.m_dwCharID;
		nDirection = PacketMoveStart.m_nDirection;
		nPacketPosX = PacketMoveStart.m_pos.x;
		nPacketPosY = PacketMoveStart.m_pos.y;
		bUpdate = PacketMoveStart.m_bUpdate;
		nSpeedLevel = PacketMoveStart.m_nSpeedLevel;
		dwPacketTime = PacketMoveStart.m_dwTimeStamp;
		bHasPacketTime = TRUE;
		break;
	case SBOCOMMANDID_SUB_CHAR_MOVE_DIR_CHANGE:
		PacketMoveDirChange.Set(pData);
		dwMapID = PacketMoveDirChange.m_dwMapID;
		dwCharID = PacketMoveDirChange.m_dwCharID;
		nDirection = PacketMoveDirChange.m_nDirection;
		nPacketPosX = PacketMoveDirChange.m_pos.x;
		nPacketPosY = PacketMoveDirChange.m_pos.y;
		bUpdate = PacketMoveDirChange.m_bUpdate;
		nSpeedLevel = PacketMoveDirChange.m_nSpeedLevel;
		dwPacketTime = PacketMoveDirChange.m_dwTimeStamp;
		bHasPacketTime = TRUE;
		break;
	case SBOCOMMANDID_SUB_CHAR_MOVE_STOP:
		PacketMoveStop.Set(pData);
		dwMapID = PacketMoveStop.m_dwMapID;
		dwCharID = PacketMoveStop.m_dwCharID;
		nDirection = PacketMoveStop.m_nDirection;
		nPacketPosX = PacketMoveStop.m_pos.x;
		nPacketPosY = PacketMoveStop.m_pos.y;
		bUpdate = PacketMoveStop.m_bUpdate;
		nSpeedLevel = PacketMoveStop.m_nSpeedLevel;
		dwPacketTime = PacketMoveStop.m_dwTimeStamp;
		bHasPacketTime = TRUE;
		break;
#if SBO_ENABLE_LEGACY_MOVEPOS_RECV
	case SBOCOMMANDID_SUB_CHAR_MOVEPOS:
		PacketMovePos.Set(pData);
		dwMapID = PacketMovePos.m_dwMapID;
		dwCharID = PacketMovePos.m_dwCharID;
		nDirection = PacketMovePos.m_nDirection;
		nPacketPosX = PacketMovePos.m_pos.x;
		nPacketPosY = PacketMovePos.m_pos.y;
		bUpdate = PacketMovePos.m_bUpdate;
		break;
#endif
	default:
		return;
	}

	// 移動トレースログ抑制（いったん無効化）
	//m_pLog->Write(
	//	"[MOVE_RECV] dwSessionID:%u [PACKET:%s][CHARID:%u][MAP:%u][POS:%d,%d][DIR:%d][UPDATE:%d][TS:%u][SPD:%d]",
	//	dwSessionID,
	//	pszPacketName,
	//	dwCharID,
	//	dwMapID,
	//	nPacketPosX,
	//	nPacketPosY,
	//	nDirection,
	//	bUpdate ? 1 : 0,
	//	dwPacketTime,
	//	nSpeedLevel);

	if (nSpeedLevel <= 0) {
		nSpeedLevel = 1;
	}

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtr(dwCharID);
	if (pInfoChar == NULL) {
		// 移動トレースログ抑制（いったん無効化）
		//m_pLog->Write(
		//	"[MOVE_RECV_DROP] dwSessionID:%u [PACKET:%s][理由:CHAR_NOT_FOUND][CHARID:%u]",
		//	dwSessionID,
		//	pszPacketName,
		//	dwCharID);
		return;
	}
	if (pInfoChar->m_dwMapID != dwMapID) {
		// 移動トレースログ抑制（いったん無効化）
		//m_pLog->Write(
		//	"[MOVE_RECV_DROP] dwSessionID:%u [PACKET:%s][理由:MAP_MISMATCH][CHAR:%s][SERVER_MAP:%u][PACKET_MAP:%u]",
		//	dwSessionID,
		//	pszPacketName,
		//	(LPCSTR)pInfoChar->m_strCharName,
		//	pInfoChar->m_dwMapID,
		//	dwMapID);
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		// 移動トレースログ抑制（いったん無効化）
		//m_pLog->Write(
		//	"[MOVE_RECV_DROP] dwSessionID:%u [PACKET:%s][理由:SESSION_MISMATCH][CHAR:%s]",
		//	dwSessionID,
		//	pszPacketName,
		//	(LPCSTR)pInfoChar->m_strCharName);
		RequestDisconnect(dwSessionID);
		return;
	}
	if (pInfoChar->m_bStateFadeInOut) {
		// ワープ遷移中（フェードイン・アウト中）は、クライアントが旧座標のまま
		// 送ってくる残留移動パケットが届くことがあるため破棄する。
		// 同一マップ内ワープの場合は上の m_dwMapID 不一致チェック（205行付近）を
		// すり抜けてしまうため、ここで明示的にガードする。
		return;
	}
	bResult = pInfoChar->IsEnableMove();
	if (bResult == FALSE) {
		if (pInfoChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			if (((pInfoChar->m_nMapX == nPacketPosX) && (pInfoChar->m_nMapY == nPacketPosY))) {
				pInfoChar->SetDirection(nDirection);
				pInfoChar->m_bChgPos = TRUE;
				pInfoChar->m_bChgUpdatePos = bUpdate;
				if (nCmdSub == SBOCOMMANDID_SUB_CHAR_MOVE_STOP) {
					pInfoChar->m_bChgUpdatePos = TRUE;
				}
				// 防御中の向き変更は処理する
				return;
			}
		}
		bResult = pInfoChar->IsStateBattle();
		if (bResult == FALSE) {
			// 既に MOVE 状態のキャラからの継続パケット
			// （MOVE_DIR_CHANGE / MOVE_STOP）は許可する。
			// これを許可しないと 2 パケット目以降が弾かれて MoveSync が機能しない。
			if (pInfoChar->IsStateMove() == FALSE) {
				// 移動できない状態なので無視
				// 移動トレースログ抑制（いったん無効化）
				//m_pLog->Write(
				//	"[MOVE_RECV_DROP] dwSessionID:%u [PACKET:%s][理由:MOVE_DISABLED][CHAR:%s][STATE:%d]",
				//	dwSessionID,
				//	pszPacketName,
				//	(LPCSTR)pInfoChar->m_strCharName,
				//	pInfoChar->m_nMoveState);
				return;
			}
		}
	}
	if (pInfoChar->m_dwHP == 0) {
		// HP0で歩こうとした時は気絶させる
		// 移動トレースログ抑制（いったん無効化）
		//m_pLog->Write(
		//	"[MOVE_RECV_DROP] dwSessionID:%u [PACKET:%s][理由:HP_ZERO][CHAR:%s]",
		//	dwSessionID,
		//	pszPacketName,
		//	(LPCSTR)pInfoChar->m_strCharName);
		pInfoChar->SetMoveState(CHARMOVESTATE_SWOON);
		return;
	}

	nNextPosX = nPacketPosX;
	nNextPosY = nPacketPosY;

	// Dead Reckoning Phase6: サーバーは独自予測を行わないためMOVE_STOPスナップ不要
	// （スナップするとクライアントの停止位置がサーバーに反映されずイベント検出が失敗する）
	//if (nCmdSub == SBOCOMMANDID_SUB_CHAR_MOVE_STOP) {
	//	int nStopDeltaX, nStopDeltaY;
	//	nStopDeltaX = nNextPosX - pInfoChar->m_nMapX;
	//	nStopDeltaY = nNextPosY - pInfoChar->m_nMapY;
	//	if ((abs (nStopDeltaX) <= HALF_TILE) && (abs (nStopDeltaY) <= HALF_TILE)) {
	//		nNextPosX = pInfoChar->m_nMapX;
	//		nNextPosY = pInfoChar->m_nMapY;
	//		nPacketPosX = nNextPosX;
	//		nPacketPosY = nNextPosY;
	//	}
	//}

	nStopState = CHARMOVESTATE_STAND;
	if (pInfoChar->IsStateBattle()) {
		nStopState = CHARMOVESTATE_BATTLE;
		if (pInfoChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
			nStopState = CHARMOVESTATE_BATTLE_DEFENSE;
		}
	}

	// 不正速度チェック（Phase 6: Dead Reckoning サーバー権威）
	if (!((pInfoChar->m_nMapX == nNextPosX) && (pInfoChar->m_nMapY == nNextPosY))) {
		dwNowTime = SboPlatform::GetTickMs();
		dwElapsed = 0;

		if (bHasPacketTime && (dwPacketTime != 0) && (pInfoChar->m_dwLastRecvMovePacketTime != 0)) {
			if (dwPacketTime <= pInfoChar->m_dwLastRecvMovePacketTime) {
				// 移動トレースログ抑制（いったん無効化）
				//m_pLog->Write(
				//	"移動時刻逆行を拒否 dwSessionID:%u [PACKET:%s][CHAR:%s][受信:%u][前回:%u][座標:%d,%d]",
				//	dwSessionID,
				//	pszPacketName,
				//	(LPCSTR)pInfoChar->m_strCharName,
				//	dwPacketTime,
				//	pInfoChar->m_dwLastRecvMovePacketTime,
				//	nPacketPosX,
				//	nPacketPosY);
				return;
			}
			dwElapsed = dwPacketTime - pInfoChar->m_dwLastRecvMovePacketTime;
		} else if (pInfoChar->m_dwLastRecvMoveTime != 0) {
			dwElapsed = dwNowTime - pInfoChar->m_dwLastRecvMoveTime;
 	}

		if (dwElapsed != 0) {
			if (dwElapsed > 3000) {
				dwElapsed = 3000;
			}

			nMoveDist = abs(nNextPosX - pInfoChar->m_nMapX);
			if (nMoveDist < abs(nNextPosY - pInfoChar->m_nMapY)) {
				nMoveDist = abs(nNextPosY - pInfoChar->m_nMapY);
			}

			// 不正速度チェックはパケット自己申告の nSpeedLevel を信用せず、
			// サーバーが保持するキャラの移動待ち(GetMoveWait)から算出した
			// 実移動速度(px/秒)を基準にする（改造クライアントによる
			// nSpeedLevel 詐称でのスピードハックを防止）。
			// スキル/アイテムによる移動待ちの変化直後（特に「遅→速」の
			// 切り替わり）で正規プレイを誤って拒否しないよう、+10%の
			// 安全マージンを掛けたうえで、既存の+32px余裕
			// （フレームジャンクによるまとめ送信の許容）も残す。
			nCharPixelsPerSec = m_pLibInfoChar->GetCharMovePixelsPerSec(pInfoChar);
			nAllowPixelsPerSec = (int)(((LONGLONG)nCharPixelsPerSec * 110) / 100);
			nAllowDist = (int)((dwElapsed * (LONGLONG)nAllowPixelsPerSec) / 1000) + 32;
			if (nMoveDist > nAllowDist) {
				BOOL bDoSyncSend;

				bDoSyncSend = FALSE;
				if ((pInfoChar->m_dwLastMoveRejectSyncTime == 0) ||
					(dwNowTime - pInfoChar->m_dwLastMoveRejectSyncTime >= 1000)) {
					pInfoChar->m_dwLastMoveRejectSyncTime = dwNowTime;
					bDoSyncSend = TRUE;
				}

				m_pLog->Write(
					"移動速度超過を拒否 dwSessionID:%u [PACKET:%s][CHAR:%s][現在:%d,%d][受信:%d,%d][移動:%dpx][許容:%dpx][経過:%ums][基準px/s:%d][申告段階:%d]%s",
					dwSessionID,
					pszPacketName,
					pInfoChar->m_strCharName.GetUtf8Pointer(),
					pInfoChar->m_nMapX,
					pInfoChar->m_nMapY,
					nNextPosX,
					nNextPosY,
					nMoveDist,
					nAllowDist,
					dwElapsed,
					nAllowPixelsPerSec,
					nSpeedLevel,
					bDoSyncSend ? "[補正:送信]" : "[補正:抑制中]");

				if (bDoSyncSend) {
					// クライアントが誤った座標のまま動き続けないよう、
					// サーバー権威の現在座標を本人へ送り返して補正する。
					CPacketCHAR_RES_CHARINFO PacketResCharInfoSync;
					PacketResCharInfoSync.Make(pInfoChar);
					m_pSock->SendTo(dwSessionID, &PacketResCharInfoSync);
				}
				return;
			}
		}
		pInfoChar->m_dwLastRecvMoveTime = dwNowTime;
		if (bHasPacketTime && (dwPacketTime != 0)) {
			pInfoChar->m_dwLastRecvMovePacketTime = dwPacketTime;
		}
	} else if (bHasPacketTime && (dwPacketTime != 0)) {
		if (pInfoChar->m_dwLastRecvMovePacketTime < dwPacketTime) {
			pInfoChar->m_dwLastRecvMovePacketTime = dwPacketTime;
		}
	}

	// 移動した？
	bPosChanged = !((pInfoChar->m_nMapX == nPacketPosX) && (pInfoChar->m_nMapY == nPacketPosY));
	if (bPosChanged) {
		// 付いて来ているキャラ一覧を作成
		pInfoCharTmp = pInfoChar;
		while (1) {
			if (pInfoCharTmp->m_dwTailCharID == 0) {
				break;
			}
			pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(pInfoCharTmp->m_dwTailCharID);
			if (pInfoCharTmp == NULL) {
				break;
			}
			apInfoChar.push_back(pInfoCharTmp);
		}

		// 前のキャラの座標へ進むように設定していく
		nCount = apInfoChar.size();
		for (i = nCount - 1; i >= 0; i --) {
			pInfoCharTmp	= apInfoChar[i];
			pInfoCharFront	= (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(pInfoCharTmp->m_dwFrontCharID);
			if (pInfoCharFront == NULL) {
				continue;
			}
			nResult = pInfoCharTmp->GetDirection(pInfoCharFront->m_ptCharBack.x, pInfoCharFront->m_ptCharBack.y);
			if (nResult != -1) {
				pInfoCharTmp->SetDirection(nResult);
			}
			pInfoCharTmp->SetPos(pInfoCharFront->m_ptCharBack.x, pInfoCharFront->m_ptCharBack.y);
			pInfoCharTmp->m_bChgPos	= TRUE;
			pInfoCharTmp->m_bChgUpdatePos	= TRUE;
		}
	}

	pInfoChar->SetPos(nNextPosX, nNextPosY);
	pInfoChar->SetDirection(nDirection);
	pInfoChar->m_bChgPos = TRUE;
	pInfoChar->m_bChgUpdatePos = bUpdate;

	if (nCmdSub == SBOCOMMANDID_SUB_CHAR_MOVE_STOP) {
		// 停止通知は最終位置を確定同期させる
		pInfoChar->m_bChgUpdatePos = TRUE;
	}
	{
		// MoveState を変えるかどうかは純粋関数(MoveStateDecision::Decide)に判断させる。
		// 位置が変わっていない MOVE_START / MOVE_DIR_CHANGE(＝向きだけの変更)では
		// 座り中(CHARMOVESTATE_SIT)等の現在の状態を崩さないよう SetMoveState を呼ばない。
		int nMoveStateOnMove = pInfoChar->IsStateBattle() ? CHARMOVESTATE_BATTLEMOVE : CHARMOVESTATE_MOVE;
		int nMoveStateNext = MoveStateDecision::Decide(
			nCmdSub == SBOCOMMANDID_SUB_CHAR_MOVE_STOP,
			pInfoChar->IsStateMove() != FALSE,
			bPosChanged,
			nMoveStateOnMove,
			nStopState);
		if (nMoveStateNext != MoveStateDecision::NO_CHANGE) {
			pInfoChar->SetMoveState(nMoveStateNext);
		}
	}

	// PC はクライアント→サーバへ届いた MOVE_* パケットを
	// そのまま他クライアントへ転送する（受信即中継方式）。
	// ProcChgPos 経由だと 1 歩の移動状態が 1 フレーム未満で終わって
	// MOVE_START が他クライアントに届かないため、直接中継に切り替えた。
	// Web 版では SendToScreenChar() の画面内判定や受信側座標差で
	// 近距離でも MOVE_* が欠落するケースがあるため、
	// PC の移動系だけは同一マップ全体へ中継して配送漏れを避ける。
	// NPC は従来通り ProcChgPos で MoveSync を発行する。
	if (!pInfoChar->IsNPC()) {
		// 移動トレースログ抑制（いったん無効化）
		//m_pLog->Write(
		//	"[MOVE_RELAY] [PACKET:%s][CHAR:%s][POS:%d,%d][DIR:%d][UPDATE:%d]",
		//	pszPacketName,
		//	(LPCSTR)pInfoChar->m_strCharName,
		//	nPacketPosX,
		//	nPacketPosY,
		//	nDirection,
		//	bUpdate ? 1 : 0);
		switch (nCmdSub) {
		case SBOCOMMANDID_SUB_CHAR_MOVE_START:
			pRelayPacket = &PacketMoveStart;
			break;
		case SBOCOMMANDID_SUB_CHAR_MOVE_DIR_CHANGE:
			pRelayPacket = &PacketMoveDirChange;
			break;
		case SBOCOMMANDID_SUB_CHAR_MOVE_STOP:
			pRelayPacket = &PacketMoveStop;
			break;
		}
		if (pRelayPacket) {
			nCount = m_pLibInfoChar->GetCountLogIn();
			for (i = 0; i < nCount; i ++) {
				pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(i);
				if (pInfoCharTmp->IsLogin() == FALSE) {
					continue;
				}
				if (pInfoCharTmp->m_dwMapID != pInfoChar->m_dwMapID) {
					continue;
				}
				if (pInfoCharTmp->m_dwSessionID == 0) {
					continue;
				}
				if (pInfoCharTmp->m_dwSessionID == dwSessionID) {
					continue;
				}
				m_pSock->SendTo(pInfoCharTmp->m_dwSessionID, pRelayPacket);
			}
		}

		/*
		   POS_SYNC フォールバックを毎歩送ると Web の viewer 側で受信量が増えすぎ、
		   FPS 低下や補間の飛びが出やすい。
		   補助同期は「移動開始」「停止」「向き変更の節目」だけに絞り、
		   連続歩行中の細かい刻みは MOVE_* の予測移動に任せる。
		*/
		if ((nCmdSub == SBOCOMMANDID_SUB_CHAR_MOVE_START) ||
			(nCmdSub == SBOCOMMANDID_SUB_CHAR_MOVE_STOP) ||
			((nCmdSub == SBOCOMMANDID_SUB_CHAR_MOVE_DIR_CHANGE) && (bUpdate == FALSE))) {
			PacketPosSync.Make(
				pInfoChar->m_dwMapID,
				pInfoChar->m_dwCharID,
				nDirection,
				nPacketPosX,
				nPacketPosY,
				(nCmdSub == SBOCOMMANDID_SUB_CHAR_MOVE_STOP) ? FALSE : TRUE,
				nSpeedLevel,
				dwPacketTime);
			nCount = m_pLibInfoChar->GetCountLogIn();
			for (i = 0; i < nCount; i ++) {
				pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(i);
				if (pInfoCharTmp->IsLogin() == FALSE) {
					continue;
				}
				if (pInfoCharTmp->m_dwMapID != pInfoChar->m_dwMapID) {
					continue;
				}
				if (pInfoCharTmp->m_dwSessionID == 0) {
					continue;
				}
				if (pInfoCharTmp->m_dwSessionID == dwSessionID) {
					continue;
				}
				m_pSock->SendTo(pInfoCharTmp->m_dwSessionID, &PacketPosSync);
			}
		}
	}
}

void CMainFrame::RecvProcCHAR_STATE(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	int nState;
	PCInfoCharSvr pInfoChar;
	PCInfoMapBase pInfoMap;
	CPacketCHAR_STATE Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}
	nState = Packet.m_nState;

	switch (nState) {
	case CHARMOVESTATE_MOVE:	// 移動中
		bResult = pInfoChar->IsEnableMove();
		if (bResult == FALSE) {
			nState = pInfoChar->m_nMoveState;
		}
		break;
	case CHARMOVESTATE_BATTLE:	// 戦闘中
	case CHARMOVESTATE_BATTLEATACK:	// 戦闘攻撃中
	case CHARMOVESTATE_BATTLE_DEFENSE:	// 防御中
		bResult = pInfoChar->IsEnableBattle();
		if (bResult != FALSE) {
			// docs/battle-redesign.md S2: Tabによる明示切替が無くなり、クライアントが
			// 攻撃と同時に自動で戦闘状態へ遷移するため、戦闘不可マップでの遷移も
			// サーバーで拒否する(改造クライアント対策)。マップ不明も拒否。
			pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr(pInfoChar->m_dwMapID);
			bResult = (pInfoMap != NULL) && pInfoMap->IsEnableBattle();
		}
		if (bResult == FALSE) {
			nState = pInfoChar->m_nMoveState;
		}
		break;
	}

	pInfoChar->SetMoveState(nState);
}

void CMainFrame::RecvProcCHAR_REQ_CHAT(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_REQ_CHAT Packet;
	CPacketMAP_SYSTEMMSG PacketMAP_SYSTEMMSG;
	CmyString strTmp, strChar;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}
	TrimViewString(strChar, (LPCTSTR)Packet.m_strChat);

	switch (Packet.m_nType) {
	case CHATTYPE_NORMAL:	// 通常
		pInfoChar->SetSpeak(strChar);
		break;
//	case CHATTYPE_PARTY:	// パーティチャット
//	case CHATTYPE_MAP:	// マップ内放送
	case CHATTYPE_ADMIN:	// 一斉放送
		strTmp.Format(_T("%s:%s"), (LPCTSTR)pInfoChar->m_strCharName, (LPCTSTR)strChar);
		PacketMAP_SYSTEMMSG.Make(strTmp);
		m_pSock->SendTo(0, &PacketMAP_SYSTEMMSG);
		break;
	}
}

void CMainFrame::RecvProcCHAR_REQ_CHARINFO2(PBYTE pData, DWORD dwSessionID)
{
	int i, nCount;
	PCInfoCharBase pInfoChar;
	CPacketCHAR_REQ_CHARINFO2 Packet;
	CPacketCHAR_CHARINFO PacketCHAR_CHARINFO;
	CLibInfoCharSvr LibInfoCharTmp;

	Packet.Set(pData);

	LibInfoCharTmp.Create(m_pMgrData);

	nCount = Packet.m_adwCharID.size();
	for (i = 0; i < nCount; i ++) {
		pInfoChar = (PCInfoCharBase)m_pLibInfoChar->GetPtrLogIn(Packet.m_adwCharID[i]);
		if (pInfoChar == NULL) {
			continue;
		}
		LibInfoCharTmp.CopyAdd(pInfoChar);
	}

	PacketCHAR_CHARINFO.Make(&LibInfoCharTmp);
	m_pSock->SendTo(dwSessionID, &PacketCHAR_CHARINFO);
}

void CMainFrame::RecvProcCHAR_REQ_PUTGET(PBYTE pData, DWORD dwSessionID)
{
	int nResult, i, nCount, nMapEventType;
	BOOL bResult;
	POINT ptPos, ptMapPos;
	PCInfoCharSvr pInfoChar;
	PCInfoItem pInfoItem;
	PCInfoMapBase pInfoMap;
	CPacketCHAR_REQ_PUTGET Packet;
	CPacketCHAR_RES_PUTGET PacketCHAR_RES_PUTGET;
	CPacketITEM_RES_ITEMINFO PacketITEM_RES_ITEMINFO;
	CPacketCHAR_ITEMINFO PacketCHAR_ITEMINFO;
	std::vector<POINT> aptPos;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr(pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}

	pInfoChar->GetFrontPos(ptPos);

	// アイテムを拾う？
	if (Packet.m_dwItemID == 0) {
		RECT rcFeet;
		/* 足元の当たり判定矩形を HALF_TILE 広げてアイテムを探す（向き非依存） */
		pInfoChar->GetCollisionRect(rcFeet);
		rcFeet.left   -= HALF_TILE;
		rcFeet.top    -= HALF_TILE;
		rcFeet.right  += HALF_TILE;
		rcFeet.bottom += HALF_TILE;
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtrInRect(pInfoChar->m_dwMapID, &rcFeet);
		if (pInfoItem) {
			bResult = pInfoChar->IsItemAdd();
			if (bResult) {
				// アイテムを追加
				m_pLibInfoItem->AddItem(pInfoChar->m_dwCharID, pInfoItem->m_dwItemID, &pInfoChar->m_adwItemID);

				PacketITEM_RES_ITEMINFO.Make(pInfoItem);
				m_pSock->SendTo(pInfoChar->m_dwSessionID, &PacketITEM_RES_ITEMINFO);

				PacketCHAR_ITEMINFO.Make(pInfoChar->m_dwCharID, &pInfoChar->m_adwItemID);
				m_pSock->SendTo(pInfoChar->m_dwSessionID, &PacketCHAR_ITEMINFO);

				PacketCHAR_RES_PUTGET.Make(pInfoItem->m_dwItemID, RESULTID_CHAR_RES_PUTGET_GET);
				m_pSock->SendTo(pInfoChar->m_dwSessionID, &PacketCHAR_RES_PUTGET);

				PacketITEM_RES_ITEMINFO.Make(pInfoItem);
				SendToMapChar(pInfoChar->m_dwMapID, &PacketITEM_RES_ITEMINFO);
			} else {
				// 持てない
			}
		}

	} else {
		bResult = pInfoChar->HaveItem(Packet.m_dwItemID);
		if (bResult == FALSE) {
			return;
		}
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(pInfoChar->m_dwMapID, &ptPos);
		if (pInfoItem) {
			if (pInfoItem->m_bPutOn == FALSE) {
				// すでに置かれている場所には置けない
				return;
			}
		}
		// アイテムは足元（自分の立っている有効な地面）に置くため、前方タイルの
		// 移動可否ゲートは廃止する。これが残っていると、ゴミ箱や壁が前方に来る位置で
		// 投棄もされず地面にも置けない状態になっていた。
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr(Packet.m_dwItemID);
		if (pInfoItem == NULL) {
			return;
		}
		pInfoChar->DeleteItem(pInfoItem->m_dwItemID);
		m_pLibInfoItem->DeleteItem(pInfoItem->m_dwItemID, pInfoChar);

		PacketCHAR_ITEMINFO.Make(pInfoChar->m_dwCharID, &pInfoChar->m_adwItemID);
		m_pSock->SendTo(pInfoChar->m_dwSessionID, &PacketCHAR_ITEMINFO);

		PacketITEM_RES_ITEMINFO.Make(pInfoItem);
		SendToMapChar(pInfoChar->m_dwMapID, &PacketITEM_RES_ITEMINFO);

		nResult = RESULTID_CHAR_RES_PUTGET_PUT;

		// ゴミ箱処理（ドット単位の任意座標でも前方リーチ矩形でゴミ箱を確実に判定）
		if (m_pLibInfoChar->IsTrashBoxInFront(pInfoChar)) {
			nResult = RESULTID_CHAR_RES_PUTGET_DELETE;
			PacketCHAR_RES_PUTGET.Make(pInfoItem->m_dwItemID, nResult);
			m_pSock->SendTo(pInfoChar->m_dwSessionID, &PacketCHAR_RES_PUTGET);

			CPacketITEM_DELETEITEMINFO PacketITEM_DELETEITEMINFO;
			PacketITEM_DELETEITEMINFO.Make(Packet.m_dwItemID);
			if (pInfoItem->m_dwMapID) {
				SendToMapChar(pInfoItem->m_dwMapID, &PacketITEM_DELETEITEMINFO);
			}
			SendToAdminChar(&PacketITEM_DELETEITEMINFO);

			m_pLibInfoItem->Delete(Packet.m_dwItemID);
		}

		if (nResult == RESULTID_CHAR_RES_PUTGET_PUT) {
			PacketCHAR_RES_PUTGET.Make(pInfoItem->m_dwItemID, nResult);
			m_pSock->SendTo(pInfoChar->m_dwSessionID, &PacketCHAR_RES_PUTGET);
		}
	}
}

void CMainFrame::RecvProcCHAR_REQ_USEITEM(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_REQ_USEITEM Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}
	m_pLibInfoChar->UseItem(pInfoChar, Packet.m_dwItemID);
}

void CMainFrame::RecvProcCHAR_REQ_DRAGITEM(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_REQ_DRAGITEM Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}
	// 自分が持っていないアイテムの並べ替えは拒否する
	bResult = pInfoChar->HaveItem(Packet.m_dwItemID);
	if (bResult == FALSE) {
		return;
	}
	m_pLibInfoChar->DragItem(pInfoChar, Packet.m_dwItemID, Packet.m_ptNewPos);
}

void CMainFrame::RecvProcCHAR_REQ_PUSH(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoObj, pInfoPlayer;
	CPacketCHAR_REQ_PUSH Packet;
	CPacketCHAR_RES_PUSH PacketResPush;
	DWORD dwNowTime;
	DWORD dwElapsedMs;
	DWORD dwRawElapsedMs; // 診断用。上限クランプ前の生の経過時間(ms)。原因特定後に無効化
	DWORD dwPrevClientTime; // 診断用。前回受理したクライアント時刻(ms)。原因特定後に無効化
	int nPixelsPerSec;
	int nSaveSelfX, nSaveSelfY;
	RECT rcSelfWin, rcObjWin;
	PushDecision::RECT_PX rcSelf, rcObj;
	PushDecision::POINT_PX ptReportedSelf, ptServerSelf;
	int dx, dy;
	int nCurX, nCurY;
	int nDir;
	POINT ptFinal;

	Packet.Set(pData);

	dwNowTime = SboPlatform::GetTickMs();

#if PUSH_DEBUG_LOG
	// 診断用。原因特定後に無効化。受信した生の値をそのまま出す(本人特定前なのでセッション単位で抑制)。
	if (AllowPushPreLog(dwSessionID, dwNowTime)) {
		m_pLog->Write(
			"押し要求を受信(診断) dwSessionID:%u [OBJ_ID:%u][向き:%d][種別:%d]"
			"[目標座標:%d,%d][申告自己座標:%d,%d][クライアント時刻:%u][パケットサイズ:%u]",
			dwSessionID,
			Packet.m_dwObjCharID,
			Packet.m_nDirection,
			Packet.m_nPushType,
			Packet.m_ptObjTarget.x, Packet.m_ptObjTarget.y,
			Packet.m_ptSelf.x, Packet.m_ptSelf.y,
			Packet.m_dwTimeStamp,
			Packet.GetSize());
	}
#endif

	nDir = Packet.m_nDirection;
	if ((nDir < PushDecision::DIR_UP) || (nDir > PushDecision::DIR_RIGHT)) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if (AllowPushPreLog(dwSessionID, dwNowTime)) {
			m_pLog->Write("押し要求を破棄 [理由:向き範囲外] dwSessionID:%u [向き:%d]", dwSessionID, nDir);
		}
#endif
		return;
	}

	// 押している本人は送信元セッションで決める(改造クライアント対策)。
	// パケット内のIDは「押される側(押せる物)」でしか無く、これを本人特定に使わない。
	pInfoPlayer = (PCInfoCharSvr)m_pLibInfoChar->GetPtrSessionID(dwSessionID);
	if (pInfoPlayer == NULL) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if (AllowPushPreLog(dwSessionID, dwNowTime)) {
			m_pLog->Write("押し要求を破棄 [理由:本人が見つからない] dwSessionID:%u", dwSessionID);
		}
#endif
		return;
	}
	pInfoObj = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwObjCharID);
	if (pInfoObj == NULL) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((pInfoPlayer->m_dwLastPushDiagLogTime == 0) ||
			(dwNowTime - pInfoPlayer->m_dwLastPushDiagLogTime >= PUSH_REJECT_LOG_INTERVAL_MS)) {
			pInfoPlayer->m_dwLastPushDiagLogTime = dwNowTime;
			m_pLog->Write("押し要求を破棄 [理由:押せる物が見つからない] dwSessionID:%u [CHAR:%s][OBJ_ID:%u]",
				dwSessionID, pInfoPlayer->m_strCharName.GetUtf8Pointer(), Packet.m_dwObjCharID);
		}
#endif
		return;
	}
	if (pInfoObj == pInfoPlayer) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((pInfoPlayer->m_dwLastPushDiagLogTime == 0) ||
			(dwNowTime - pInfoPlayer->m_dwLastPushDiagLogTime >= PUSH_REJECT_LOG_INTERVAL_MS)) {
			pInfoPlayer->m_dwLastPushDiagLogTime = dwNowTime;
			m_pLog->Write("押し要求を破棄 [理由:押せる物が自分自身] dwSessionID:%u [CHAR:%s]",
				dwSessionID, pInfoPlayer->m_strCharName.GetUtf8Pointer());
		}
#endif
		return;
	}
	if (pInfoObj->IsLogin() == FALSE) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((pInfoPlayer->m_dwLastPushDiagLogTime == 0) ||
			(dwNowTime - pInfoPlayer->m_dwLastPushDiagLogTime >= PUSH_REJECT_LOG_INTERVAL_MS)) {
			pInfoPlayer->m_dwLastPushDiagLogTime = dwNowTime;
			m_pLog->Write("押し要求を破棄 [理由:押せる物がログインしていない] dwSessionID:%u [CHAR:%s][OBJ_ID:%u]",
				dwSessionID, pInfoPlayer->m_strCharName.GetUtf8Pointer(), Packet.m_dwObjCharID);
		}
#endif
		return;
	}
	if (pInfoObj->m_bPush == FALSE) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((pInfoPlayer->m_dwLastPushDiagLogTime == 0) ||
			(dwNowTime - pInfoPlayer->m_dwLastPushDiagLogTime >= PUSH_REJECT_LOG_INTERVAL_MS)) {
			pInfoPlayer->m_dwLastPushDiagLogTime = dwNowTime;
			m_pLog->Write("押し要求を破棄 [理由:m_bPushでない] dwSessionID:%u [CHAR:%s][OBJ:%s]",
				dwSessionID, pInfoPlayer->m_strCharName.GetUtf8Pointer(), pInfoObj->m_strCharName.GetUtf8Pointer());
		}
#endif
		return;
	}
	if (pInfoObj->m_nMoveType == CHARMOVETYPE_PUTNPC) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((pInfoPlayer->m_dwLastPushDiagLogTime == 0) ||
			(dwNowTime - pInfoPlayer->m_dwLastPushDiagLogTime >= PUSH_REJECT_LOG_INTERVAL_MS)) {
			pInfoPlayer->m_dwLastPushDiagLogTime = dwNowTime;
			m_pLog->Write("押し要求を破棄 [理由:PUTNPC] dwSessionID:%u [CHAR:%s][OBJ:%s][m_nMoveType:%d]",
				dwSessionID, pInfoPlayer->m_strCharName.GetUtf8Pointer(), pInfoObj->m_strCharName.GetUtf8Pointer(),
				pInfoObj->m_nMoveType);
		}
#endif
		return;
	}
	if (pInfoObj->m_dwMapID != pInfoPlayer->m_dwMapID) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((pInfoPlayer->m_dwLastPushDiagLogTime == 0) ||
			(dwNowTime - pInfoPlayer->m_dwLastPushDiagLogTime >= PUSH_REJECT_LOG_INTERVAL_MS)) {
			pInfoPlayer->m_dwLastPushDiagLogTime = dwNowTime;
			m_pLog->Write("押し要求を破棄 [理由:別マップ] dwSessionID:%u [CHAR:%s][OBJ:%s][本人MapID:%u][OBJ MapID:%u]",
				dwSessionID, pInfoPlayer->m_strCharName.GetUtf8Pointer(), pInfoObj->m_strCharName.GetUtf8Pointer(),
				pInfoPlayer->m_dwMapID, pInfoObj->m_dwMapID);
		}
#endif
		// 押せる物・本人は特定できているので、確定座標を返して補正する。
		ptFinal.x = pInfoObj->m_nMapX;
		ptFinal.y = pInfoObj->m_nMapY;
		PacketResPush.Make(pInfoObj->m_dwCharID, ptFinal, FALSE);
		m_pSock->SendTo(dwSessionID, &PacketResPush);
		return;
	}

	// 自走(eject)中はPUSH/SWAPどちらの要求も誰からも受け付けない
	// (親からの追加指示。docs/push-object-redesign.md 2章7項)。ここでreturnして
	// 関数末尾のm_bRelease即停止処理も通さないことで、eject中は止めない。
	if (pInfoObj->m_bEjectActive) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		m_pLog->Write("EJECT中却下 type:%d dir:%d", Packet.m_nPushType, nDir);
#endif
		ptFinal.x = pInfoObj->m_nMapX;
		ptFinal.y = pInfoObj->m_nMapY;
		PacketResPush.Make(pInfoObj->m_dwCharID, ptFinal, FALSE);
		m_pSock->SendTo(dwSessionID, &PacketResPush);
		return;
	}

	// 入れ替わり(SWAP)中に専有者以外から要求が来た場合はここで却下する
	// (通常の押し要求(PUSHTYPE_PUSH)・SWAP要求のどちらも対象。
	// docs/push-object-redesign.md 2章7項)。専有者からの要求はSWAP/PUSHどちらも
	// この先の分岐(dwNowTime計算後)で処理する。
	if (pInfoObj->m_bSwapActive && (dwSessionID != pInfoObj->m_dwSwapOwnerSessionID)) {
		ptFinal.x = pInfoObj->m_nMapX;
		ptFinal.y = pInfoObj->m_nMapY;
		PacketResPush.Make(pInfoObj->m_dwCharID, ptFinal, FALSE);
		m_pSock->SendTo(dwSessionID, &PacketResPush);
		return;
	}

	// 時刻逆行チェック(前回受理したクライアント時刻との比較)。
	// 判定そのものはPushDecision::CheckClientTime(純粋関数)に切り出してある。
	// - 小さな逆行(パケットの入れ替わり・重複)は却下する。ただし押せる物と本人は
	//   特定できているため、黙って無視せずRES_PUSHで現在座標を返し、クライアントが
	//   予測だけで動き続けないようにする。
	// - 大きな逆行(PUSH_RELOAD_REWIND_THRESHOLD_MS以上)はクライアント再起動
	//   (ページ再読み込み等でSDL_GetTicks系が0から数え直された)とみなし、却下せず
	//   基準(m_dwLastPushClientTime)を0に戻して「前回受理なし」として通常どおり判定する。
	dwPrevClientTime = pInfoPlayer->m_dwLastPushClientTime; // 診断用。原因特定後に無効化

	PushDecision::CLIENT_TIME_RESULT timeResult = PushDecision::CheckClientTime(
		Packet.m_dwTimeStamp, pInfoPlayer->m_dwLastPushClientTime,
		PUSH_RELOAD_REWIND_THRESHOLD_MS, PUSH_ELAPSED_MAX_MS);

	if (timeResult.bRejected) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((pInfoPlayer->m_dwLastPushDiagLogTime == 0) ||
			(dwNowTime - pInfoPlayer->m_dwLastPushDiagLogTime >= PUSH_REJECT_LOG_INTERVAL_MS)) {
			pInfoPlayer->m_dwLastPushDiagLogTime = dwNowTime;
			m_pLog->Write("押し要求を破棄 [理由:時刻逆行] dwSessionID:%u [CHAR:%s][OBJ:%s]"
				"[今回クライアント時刻:%u][前回受理クライアント時刻:%u]",
				dwSessionID, pInfoPlayer->m_strCharName.GetUtf8Pointer(), pInfoObj->m_strCharName.GetUtf8Pointer(),
				Packet.m_dwTimeStamp, pInfoPlayer->m_dwLastPushClientTime);
		}
#endif
		// 押せる物・本人は特定できているので、確定座標を返して補正する。
		ptFinal.x = pInfoObj->m_nMapX;
		ptFinal.y = pInfoObj->m_nMapY;
		PacketResPush.Make(pInfoObj->m_dwCharID, ptFinal, FALSE);
		m_pSock->SendTo(dwSessionID, &PacketResPush);
		return;
	}

	if (timeResult.bTreatedAsReload) {
#if PUSH_DEBUG_LOG
		// 診断用。原因特定後に無効化。
		if ((pInfoPlayer->m_dwLastPushDiagLogTime == 0) ||
			(dwNowTime - pInfoPlayer->m_dwLastPushDiagLogTime >= PUSH_REJECT_LOG_INTERVAL_MS)) {
			pInfoPlayer->m_dwLastPushDiagLogTime = dwNowTime;
			m_pLog->Write("押し時刻の大幅な逆行を検知(読み込み直しとみなし基準をリセット) dwSessionID:%u [CHAR:%s]"
				"[今回クライアント時刻:%u][前回受理クライアント時刻:%u]",
				dwSessionID, pInfoPlayer->m_strCharName.GetUtf8Pointer(),
				Packet.m_dwTimeStamp, pInfoPlayer->m_dwLastPushClientTime);
		}
#endif
		pInfoPlayer->m_dwLastPushClientTime = 0;
	}

	dwElapsedMs = timeResult.dwElapsedMs;
	// 診断用。原因特定後に無効化。上限クランプ前の生の経過時間(ms)。
	if ((Packet.m_dwTimeStamp != 0) && (dwPrevClientTime != 0) && !timeResult.bTreatedAsReload) {
		dwRawElapsedMs = Packet.m_dwTimeStamp - dwPrevClientTime;
	} else {
		dwRawElapsedMs = PUSH_ELAPSED_MAX_MS;
	}

	nPixelsPerSec = m_pLibInfoChar->GetCharMovePixelsPerSec(pInfoPlayer);

	// 押している本人の当たり矩形(申告座標基準)。サーバー座標は変えずに一時的に
	// 申告座標へ差し替えてGetCollisionRectを取る。
	nSaveSelfX = pInfoPlayer->m_nMapX;
	nSaveSelfY = pInfoPlayer->m_nMapY;
	pInfoPlayer->m_nMapX = Packet.m_ptSelf.x;
	pInfoPlayer->m_nMapY = Packet.m_ptSelf.y;
	pInfoPlayer->GetCollisionRect(rcSelfWin);
	pInfoPlayer->m_nMapX = nSaveSelfX;
	pInfoPlayer->m_nMapY = nSaveSelfY;
	rcSelf = ToPushRect(rcSelfWin);

	pInfoObj->GetCollisionRect(rcObjWin);
	rcObj = ToPushRect(rcObjWin);

	ptReportedSelf.x = Packet.m_ptSelf.x;
	ptReportedSelf.y = Packet.m_ptSelf.y;
	ptServerSelf.x = nSaveSelfX;
	ptServerSelf.y = nSaveSelfY;

	// pInfoObj->m_pInfoMapはSetMap()経由(スキル/アイテム生成/NPC発生)でしか
	// セットされず、DBから配置されたNPC(押せるボール等)ではNULLのままになる
	// ため使わない。他の受信処理(RecvProcCHAR_REQ_PUTGET等)と同じく
	// m_pLibInfoMap->GetPtr()でpInfoObjのマップIDから引く。SWAP/PUSH共通で使う。
	PCInfoMapBase pInfoObjMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr(pInfoObj->m_dwMapID);

	if (pInfoObj->m_bSwapActive && (Packet.m_nPushType == PUSHTYPE_PUSH)) {
		// 入れ替わり中に専有者(=ここに来た時点でdwSessionIDは専有者で確定済み。
		// 専有者以外は関数冒頭で既に却下してreturn済み)からPUSHTYPE_PUSHが来た場合。
		// 向きが同じなら却下(SWAPパケットで継続させる)、違えば自走(eject)を開始する
		// (docs/push-object-redesign.md 2章7項)。
		pInfoObj->m_dwLastSwapReqTime = dwNowTime;
		if (nDir != pInfoObj->m_nSwapDir) {
			pInfoObj->m_bSwapActive = FALSE;
			pInfoObj->m_bEjectActive = TRUE;
			// DB配置のNPCは m_pLibInfoCharSvr がNULLのため、自走を始める時にセットする
			// (ProcSwapPush の自走開始と同じ理由)。
			pInfoObj->SetLibInfoChar(m_pLibInfoChar);
			pInfoObj->m_dwEjectOwnerCharID = pInfoPlayer->m_dwCharID;
			pInfoObj->m_nEjectDir = nDir;
			pInfoObj->m_dwLastEjectProcTime = dwNowTime;
			pInfoObj->m_dEjectPxRemainder = 0.0;
			pInfoObj->SetDirection(nDir);
#if PUSH_DEBUG_LOG
			// 診断用。原因特定後に無効化。
			m_pLog->Write("EJECT開始 dir:%d obj:%d,%d", nDir, pInfoObj->m_nMapX, pInfoObj->m_nMapY);
#endif
			ptFinal.x = pInfoObj->m_nMapX;
			ptFinal.y = pInfoObj->m_nMapY;
			PacketResPush.Make(pInfoObj->m_dwCharID, ptFinal, TRUE);
			m_pSock->SendTo(dwSessionID, &PacketResPush);
		} else {
#if PUSH_DEBUG_LOG
			// 診断用。原因特定後に無効化。
			m_pLog->Write("SWAP中PUSH却下 dir:%d", nDir);
#endif
			ptFinal.x = pInfoObj->m_nMapX;
			ptFinal.y = pInfoObj->m_nMapY;
			PacketResPush.Make(pInfoObj->m_dwCharID, ptFinal, FALSE);
			m_pSock->SendTo(dwSessionID, &PacketResPush);
		}
	} else if (Packet.m_nPushType == PUSHTYPE_SWAP) {
		ProcSwapPush(m_pLog, m_pLibInfoChar, m_pSock, pInfoPlayer, pInfoObj, dwSessionID, dwNowTime, Packet, nDir, rcSelf, rcObj, pInfoObjMap);
	} else {

	dx = Packet.m_ptObjTarget.x - pInfoObj->m_nMapX;
	dy = Packet.m_ptObjTarget.y - pInfoObj->m_nMapY;

	// isFree: 1px先が空いているか(マップ+本人と押せる物自身を除く全キャラ)。
	// nCurX/nCurYは呼び出されるたびに1pxずつ進める「確定済み」座標で、
	// pInfoObjの実座標(m_nMapX/Y)は受理が確定するまで書き換えない。
	nCurX = pInfoObj->m_nMapX;
	nCurY = pInfoObj->m_nMapY;
	PushDecision::IsPositionFreeFunc isFree = [&](const PushDecision::RECT_PX &rcMoveTo) -> bool {
		if (!IsPushMapFree(m_pLibInfoChar, pInfoObjMap, pInfoObj, nCurX, nCurY, nDir)) {
			return false;
		}
		if (!IsPushCharAreaFree(m_pLibInfoChar, pInfoObj->m_dwMapID, pInfoPlayer, pInfoObj, rcMoveTo)) {
			return false;
		}
		int nStepX, nStepY;
		PushDirVector(nDir, nStepX, nStepY);
		nCurX += nStepX;
		nCurY += nStepY;
		return true;
	};

#if PUSH_DEBUG_LOG
	// 診断用。原因特定後に無効化。DecidePush直前の元値(受理・却下どちらになるかによらず出す)。
	if ((pInfoPlayer->m_dwLastPushDecideLogTime == 0) ||
		(dwNowTime - pInfoPlayer->m_dwLastPushDecideLogTime >= PUSH_REJECT_LOG_INTERVAL_MS)) {
		pInfoPlayer->m_dwLastPushDecideLogTime = dwNowTime;
		m_pLog->Write(
			"押し判定直前(診断) dwSessionID:%u [CHAR:%s][OBJ:%s][向き:%d]"
			"[OBJ座標:%d,%d][目標座標:%d,%d][dx:%d,dy:%d][経過ms:%u(生:%u)][速度px/s:%d]",
			dwSessionID,
			pInfoPlayer->m_strCharName.GetUtf8Pointer(),
			pInfoObj->m_strCharName.GetUtf8Pointer(),
			nDir,
			pInfoObj->m_nMapX, pInfoObj->m_nMapY,
			Packet.m_ptObjTarget.x, Packet.m_ptObjTarget.y,
			dx, dy,
			dwElapsedMs, dwRawElapsedMs,
			nPixelsPerSec);
	}
#endif

	PushDecision::PUSH_RESULT result = PushDecision::DecidePush(
		rcSelf,
		ptReportedSelf,
		ptServerSelf,
		PUSH_SELF_POS_TOLERANCE_PX,
		rcObj,
		nDir,
		dx, dy,
		dwElapsedMs,
		nPixelsPerSec,
		PUSH_CONTACT_GAP_ALLOWED_PX,
		isFree);

	if (result.bAccepted) {
		int nStepX, nStepY;
		int nNewX, nNewY;

		if (result.nAcceptedDistance == 0) {
			// 要求距離0(dx==dy==0、クライアントが押すのをやめた時等)。
			// 押せる物は動かさない(位置・向き・移動状態のいずれも変更しない)。
			// 時刻の基準(m_dwLastPushClientTime)だけは更新する。これを更新しないと
			// 押すのをやめている間はここに来るたびに前回受理時刻が古いまま溜まり、
			// 次に実際に押し始めた時のdwElapsedMsが不当に大きくなる
			// (=速度検証の許容距離が不当に緩くなる)ため。
			nNewX = pInfoObj->m_nMapX;
			nNewY = pInfoObj->m_nMapY;
			pInfoPlayer->m_dwLastPushClientTime = Packet.m_dwTimeStamp;
		} else {
			PushDirVector(nDir, nStepX, nStepY);
			nNewX = pInfoObj->m_nMapX + nStepX * result.nAcceptedDistance;
			nNewY = pInfoObj->m_nMapY + nStepY * result.nAcceptedDistance;

			pInfoObj->SetPos(nNewX, nNewY);
			pInfoObj->SetDirection(nDir);
			pInfoObj->m_bChgPos = TRUE;

			{
				int nMoveStateOnMove = pInfoObj->IsStateBattle() ? CHARMOVESTATE_BATTLEMOVE : CHARMOVESTATE_MOVE;
				if (pInfoObj->m_nMoveState != nMoveStateOnMove) {
					pInfoObj->SetMoveState(nMoveStateOnMove);
				}
			}
			pInfoObj->m_dwLastPushedTime = dwNowTime;
			pInfoObj->m_dwPushingCharID = pInfoPlayer->m_dwCharID;

			pInfoPlayer->m_dwLastPushClientTime = Packet.m_dwTimeStamp;

#if PUSH_DEBUG_LOG
			// 診断用。原因特定後に無効化。受理ログも却下ログと同じく1秒に1回までに絞る。
			if ((pInfoPlayer->m_dwLastPushAcceptLogTime == 0) ||
				(dwNowTime - pInfoPlayer->m_dwLastPushAcceptLogTime >= PUSH_ACCEPT_LOG_INTERVAL_MS)) {
				pInfoPlayer->m_dwLastPushAcceptLogTime = dwNowTime;
				int nRequestedDistanceLog = abs(dx) + abs(dy);
				m_pLog->Write(
					"押し要求を受理(診断) dwSessionID:%u [CHAR:%s][OBJ:%s][受理距離:%d][要求距離:%d(dx:%d,dy:%d)][経過ms:%u(生:%u)][新座標:%d,%d]",
					dwSessionID,
					pInfoPlayer->m_strCharName.GetUtf8Pointer(),
					pInfoObj->m_strCharName.GetUtf8Pointer(),
					result.nAcceptedDistance,
					nRequestedDistanceLog, dx, dy,
					dwElapsedMs, dwRawElapsedMs,
					nNewX, nNewY);
			}
#endif
		}

		ptFinal.x = nNewX;
		ptFinal.y = nNewY;
		PacketResPush.Make(pInfoObj->m_dwCharID, ptFinal, TRUE);
		m_pSock->SendTo(dwSessionID, &PacketResPush);
	} else {
		// 却下時のRES_PUSH補正送信は毎回行う(押し要求は100ms間隔=最大10回/秒なので
		// 送信自体は間引かない。クライアントのずれをすぐ戻すため)。
		// m_dwLastPushRejectSyncTimeは間引きに使っていたが不要になった。
		// メンバ自体は他用途に転用しやすいよう残す(未使用)。
		// ログのみ従来どおり本人ごと1秒に1回までとし、100ms間隔の連続却下でログが
		// 溢れないようにする。抑制した件数は次に出すログへ添えて捨てない。
		BOOL bDoSyncSend = TRUE;
		BOOL bDoLog = FALSE;

		if ((pInfoPlayer->m_dwLastPushRejectLogTime == 0) ||
			(dwNowTime - pInfoPlayer->m_dwLastPushRejectLogTime >= PUSH_REJECT_LOG_INTERVAL_MS)) {
			pInfoPlayer->m_dwLastPushRejectLogTime = dwNowTime;
			bDoLog = TRUE;
		}

		if (bDoLog) {
			CmyString strSuppressed;
			if (pInfoPlayer->m_nPushRejectSuppressedCount > 0) {
				strSuppressed.Format(_T("(他%d件)"), pInfoPlayer->m_nPushRejectSuppressedCount);
			}
#if PUSH_DEBUG_LOG
			// 診断用。原因特定後は下のシンプル版に戻して無効化する。
			{
				int nRequestedDistanceLog = abs(dx) + abs(dy);
				double dAllowedDistanceLog = GetPushAllowedDistance(dwElapsedMs, nPixelsPerSec);
				m_pLog->Write(
					"押し要求を却下 dwSessionID:%u [CHAR:%s][OBJ:%s][理由:%d:%s]"
					"[要求距離:%d(dx:%d,dy:%d)][経過ms:%u(生:%u)][速度px/s:%d][許容距離:%.1f]"
					"[申告座標:%d,%d][サーバー座標:%d,%d][OBJ座標:%d,%d][目標座標:%d,%d][向き:%d]"
					"[前回受理時刻:%u][今回時刻:%u]%s%s",
					dwSessionID,
					pInfoPlayer->m_strCharName.GetUtf8Pointer(),
					pInfoObj->m_strCharName.GetUtf8Pointer(),
					result.eReason, GetPushRejectReasonName(result.eReason),
					nRequestedDistanceLog, dx, dy,
					dwElapsedMs, dwRawElapsedMs,
					nPixelsPerSec, dAllowedDistanceLog,
					Packet.m_ptSelf.x,
					Packet.m_ptSelf.y,
					nSaveSelfX,
					nSaveSelfY,
					pInfoObj->m_nMapX, pInfoObj->m_nMapY,
					Packet.m_ptObjTarget.x, Packet.m_ptObjTarget.y,
					nDir,
					dwPrevClientTime, Packet.m_dwTimeStamp,
					bDoSyncSend ? "[補正:送信]" : "[補正:抑制中]",
					strSuppressed.GetUtf8Pointer());
			}
#else
			m_pLog->Write(
				"押し要求を却下 dwSessionID:%u [CHAR:%s][OBJ:%s][理由:%d][申告座標:%d,%d][サーバー座標:%d,%d]%s%s",
				dwSessionID,
				pInfoPlayer->m_strCharName.GetUtf8Pointer(),
				pInfoObj->m_strCharName.GetUtf8Pointer(),
				result.eReason,
				Packet.m_ptSelf.x,
				Packet.m_ptSelf.y,
				nSaveSelfX,
				nSaveSelfY,
				bDoSyncSend ? "[補正:送信]" : "[補正:抑制中]",
				strSuppressed.GetUtf8Pointer());
#endif
			pInfoPlayer->m_nPushRejectSuppressedCount = 0;
		} else {
			pInfoPlayer->m_nPushRejectSuppressedCount ++;
		}

		if (bDoSyncSend) {
			ptFinal.x = pInfoObj->m_nMapX;
			ptFinal.y = pInfoObj->m_nMapY;
			PacketResPush.Make(pInfoObj->m_dwCharID, ptFinal, FALSE);
			m_pSock->SendTo(dwSessionID, &PacketResPush);
		}
	}

	} // else (Packet.m_nPushType == PUSHTYPE_PUSH)

	// S4(docs/push-object-redesign.md): クライアントは指を離した(押すのをやめた)瞬間の
	// 最後の1通に「離した」印(m_bRelease)を付けて送ってくる。これが付いていて、かつ
	// 押していた本人からの要求であれば、150msのタイマー停止(CInfoCharSvr::TimerProcMOVE)
	// を待たずにここで即座に停止させ、周囲へのMOVE_STOP配信を早める
	// (見る側の推測航法の行き過ぎを軽減する)。受理・却下どちらでも効かせる
	// (押し続けたまま向きだけ変えて却下される場合等もあるため)。
	// タイマー側の150msタイムアウト停止は、このパケットが届かなかった場合の保険として残す。
	if (Packet.m_bRelease && pInfoObj->IsStateMove() && (pInfoObj->m_dwPushingCharID == pInfoPlayer->m_dwCharID)) {
		int nStopState = CHARMOVESTATE_STAND;
		if (pInfoObj->IsStateBattle()) {
			nStopState = CHARMOVESTATE_BATTLE;
		}
		pInfoObj->SetMoveState(nStopState);
		pInfoObj->m_dwPushingCharID = 0;
	}
}

void CMainFrame::RecvProcCHAR_REQ_TAIL(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	int nResult;
	PCInfoCharSvr pInfoChar, pInfoCharTarget;
	CPacketCHAR_REQ_TAIL Packet;

	nResult = RES_TAIL_RESULT_NONE;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}
	pInfoCharTarget = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwTargetCharID);
	m_pLibInfoChar->Tail(pInfoChar, pInfoCharTarget, Packet.m_bTail);
}

void CMainFrame::RecvProcCHAR_REQ_MODIFY_PARAM(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar, pInfoCharTmp, pInfoPlayer;
	CPacketCHAR_REQ_MODIFY_PARAM Packet;
	CPacketCHAR_MODIFY_PARAM PacketMODIFY_PARAM;
	CPacketCHAR_RES_CHARINFO PacketRES_CHARINFO;
	SIZE sizeDistance;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	// Packet.m_dwCharID は m_nType によって「操作対象のNPC」か「送信元自身」のどちらもあり得るため、
	// 先頭で一律に CheckSessionID はせず、種別ごとに主体を確認する。
	pInfoPlayer = (PCInfoCharSvr)m_pLibInfoChar->GetPtrSessionID(dwSessionID);
	if (pInfoPlayer == NULL) {
		return;
	}

	switch (Packet.m_nType) {
	case PARAMID_CHAR_REQ_MODIFY_ANIME:	// アニメーション番号の変更(得点NPC)
		if (pInfoChar->m_nMoveType != CHARMOVETYPE_SCORE) {
			return;
		}
		// サーバー側のプレイヤー座標は移動同期の遅れぶんずれるため、しきい値は3マス程度に広げる。
		m_pLibInfoChar->GetDistance(sizeDistance, pInfoPlayer, pInfoChar);
		if ((sizeDistance.cx < 0) || (sizeDistance.cx > MAPPARTSSIZE * 3) || (sizeDistance.cy > MAPPARTSSIZE * 3)) {
			return;
		}
		pInfoChar->m_nAnime ++;
		if (pInfoChar->m_nAnime >= 10) {
			pInfoChar->m_nAnime = 0;
		}
		PacketMODIFY_PARAM.Make(pInfoChar->m_dwCharID, PARAMID_CHAR_MODIFY_ANIME, pInfoChar->m_nAnime);
		SendToScreenChar(pInfoChar, &PacketMODIFY_PARAM);
		break;

	case PARAMID_CHAR_REQ_MODIFY_STYLECOPY_PUT:	// 容姿のコピー(取り込み): NPCへ自分自身の容姿を取り込ませる
		if (pInfoChar->m_nMoveType != CHARMOVETYPE_STYLECOPY_PUT) {
			return;
		}
		if ((DWORD)Packet.m_nParam != pInfoPlayer->m_dwCharID) {
			// 取り込み元に他人のキャラを指定するのは明らかな偽装
			m_pLog->Write("■REQ_MODIFY_STYLECOPY_PUT不正 dwSessionID:%u [Param:%u][自身CharID:%u]", dwSessionID, (DWORD)Packet.m_nParam, pInfoPlayer->m_dwCharID);
			RequestDisconnect(dwSessionID);
			return;
		}
		// サーバー側のプレイヤー座標は移動同期の遅れぶんずれるため、しきい値は3マス程度に広げる。
		m_pLibInfoChar->GetDistance(sizeDistance, pInfoPlayer, pInfoChar);
		if ((sizeDistance.cx < 0) || (sizeDistance.cx > MAPPARTSSIZE * 3) || (sizeDistance.cy > MAPPARTSSIZE * 3)) {
			return;
		}
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn((DWORD)Packet.m_nParam);
		if (pInfoCharTmp == NULL) {
			break;
		}
		pInfoChar->m_wFamilyID	= pInfoCharTmp->m_wFamilyID;	// 種族ID
		pInfoChar->m_wGrpIDNPC	= pInfoCharTmp->m_wGrpIDNPC;	// 画像ID(NPC)
		pInfoChar->m_wGrpIDCloth	= pInfoCharTmp->m_wGrpIDCloth;	// 画像ID(服)
		pInfoChar->m_wGrpIDEye	= pInfoCharTmp->m_wGrpIDEye;	// 画像ID(目)
		pInfoChar->m_wGrpIDEyeColor	= pInfoCharTmp->m_wGrpIDEyeColor;	// 画像ID(目色)
		pInfoChar->m_wGrpIDHairType	= pInfoCharTmp->m_wGrpIDHairType;	// 画像ID(髪)
		pInfoChar->m_wGrpIDHairColor	= pInfoCharTmp->m_wGrpIDHairColor;	// 画像ID(髪色)
		pInfoChar->m_wGrpIDSP	= pInfoCharTmp->m_wGrpIDSP;	// 画像ID(特殊服)
		pInfoChar->m_wGrpIDAcce	= pInfoCharTmp->m_wGrpIDAcce;	// 画像ID(アクセサリ)
		pInfoChar->m_wGrpIDArmsMain	= pInfoCharTmp->m_wGrpIDArmsMain;	// 画像ID(持ち物:メイン)
		pInfoChar->m_wGrpIDArmsSub	= pInfoCharTmp->m_wGrpIDArmsSub;	// 画像ID(持ち物:サブ)
		pInfoChar->m_clName	= pInfoCharTmp->m_clName;	// 名前の色
		pInfoChar->m_nSex	= pInfoCharTmp->m_nSex;	// 性別
		pInfoChar->m_dwMotionTypeID	= pInfoCharTmp->m_dwMotionTypeID;	// モーション種別ID
		PacketRES_CHARINFO.Make(pInfoChar);
		SendToScreenChar(pInfoChar, &PacketRES_CHARINFO);
		break;
	case PARAMID_CHAR_REQ_MODIFY_STYLECOPY_GET:	// 容姿のコピー(反映): NPCの容姿を自分自身に反映する
		if (pInfoChar != pInfoPlayer) {
			// 主体に他人のプレイヤーキャラを指定するのは明らかな偽装
			m_pLog->Write("■REQ_MODIFY_STYLECOPY_GET不正 dwSessionID:%u [指定CharID:%u][自身CharID:%u]", dwSessionID, pInfoChar->m_dwCharID, pInfoPlayer->m_dwCharID);
			RequestDisconnect(dwSessionID);
			return;
		}
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn((DWORD)Packet.m_nParam);
		if (pInfoCharTmp == NULL) {
			break;
		}
		if (pInfoCharTmp->m_nMoveType != CHARMOVETYPE_STYLECOPY_GET) {
			break;
		}
		// サーバー側のプレイヤー座標は移動同期の遅れぶんずれるため、しきい値は3マス程度に広げる。
		m_pLibInfoChar->GetDistance(sizeDistance, pInfoPlayer, pInfoCharTmp);
		if ((sizeDistance.cx < 0) || (sizeDistance.cx > MAPPARTSSIZE * 3) || (sizeDistance.cy > MAPPARTSSIZE * 3)) {
			break;
		}
		pInfoChar->m_wGrpIDCloth	= pInfoCharTmp->m_wGrpIDCloth;	// 画像ID(服)
		pInfoChar->m_wGrpIDHairType	= pInfoCharTmp->m_wGrpIDHairType;	// 画像ID(髪)
		pInfoChar->m_wGrpIDSP	= pInfoCharTmp->m_wGrpIDSP;	// 画像ID(特殊服)
		pInfoChar->m_nSex	= pInfoCharTmp->m_nSex;	// 性別
		pInfoChar->m_dwMotionTypeID	= pInfoCharTmp->m_dwMotionTypeID;	// モーション種別ID
		PacketRES_CHARINFO.Make(pInfoChar);
		SendToScreenChar(pInfoChar, &PacketRES_CHARINFO);
	}
}

void CMainFrame::RecvProcCHAR_REQ_EQUIP(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_REQ_EQUIP Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}

	// 装備する？
	if (Packet.m_nUnsetType == -1) {
		m_pLibInfoChar->Equip(pInfoChar, Packet.m_dwItemID);

	// 装備解除
	} else {
		m_pLibInfoChar->UnEquip(pInfoChar, Packet.m_dwItemID);
	}
}

void CMainFrame::RecvProcCHAR_PROC_FISHING(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult, bRet;
	int nProcState;
	POINT ptPos;
	PCInfoCharSvr pInfoChar;
	PCInfoMapBase pInfoMap;
	CPacketCHAR_PARA1 Packet;

	bRet = FALSE;

	Packet.Set(pData);
	nProcState = CHARPROCSTATEID_NORMAL;

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}
	bResult = pInfoChar->IsEnableBattle();
	if (bResult == FALSE) {
		goto Exit;
	}
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr(pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		goto Exit;
	}
	pInfoChar->GetFrontMapPos(ptPos);
	bResult = pInfoMap->IsFlg(ptPos.x, ptPos.y, BIT_PARTSHIT_FISHING);
	if (bResult == FALSE) {
		goto Exit;
	}
	nProcState = CHARPROCSTATEID_FISHING;
	pInfoChar->AddProcInfo(CHARPROCID_FISHING, 2000, Packet.m_dwPara);

	bRet = TRUE;
Exit:
	if (pInfoChar && (bRet == FALSE)) {
		bResult = pInfoChar->IsEnableBattle();
		if (bResult) {
			pInfoChar->SetMoveState(CHARMOVESTATE_BATTLE);
		}
	}
	pInfoChar->SetProcState(nProcState);
}

void CMainFrame::RecvProcCHAR_REQ_CHECKMAPEVENT(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		pInfoChar = NULL;
		goto Exit;
	}
	// Phase 8: 自由移動中もイベントを取りこぼさないよう、移動可否でチェック要求を拒否しない
	pInfoChar->m_bWaitCheckMapEvent = TRUE;

Exit:
	if (pInfoChar && (pInfoChar->m_bWaitCheckMapEvent == FALSE)) {
		Packet.Make(SBOCOMMANDID_SUB_CHAR_RES_CHECKMAPEVENT, pInfoChar->m_dwCharID, 0);
		SendToClient(pInfoChar->m_dwSessionID, &Packet);
	}
}

void CMainFrame::RecvProcCHAR_STATE_CHARGE(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}

	pInfoChar->m_bChargeAtack = (Packet.m_dwPara == 0) ? FALSE : TRUE;
	// 周りのキャラにも通知
	Packet.Make(SBOCOMMANDID_SUB_CHAR_STATE_CHARGE, Packet.m_dwCharID, Packet.m_dwPara);
	SendToScreenChar(pInfoChar, &Packet);
}

void CMainFrame::RecvProcCHAR_REQ_RECOVERY(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_PARA1 Packet;
	CPacketMAP_PARA1 PacketMAP_PARA1;
	CPacketCHAR_STATE PacketCHAR_STATE;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}
	if (pInfoChar->m_nMoveState != CHARMOVESTATE_SWOON) {
		return;
	}
	pInfoChar->AddProcInfo(CHARPROCID_SWOON, 2000, 1);

	// 周りのキャラに通知
	PacketCHAR_STATE.Make(pInfoChar->m_dwCharID, CHARMOVESTATE_DELETE);
	SendToScreenChar(pInfoChar, &PacketCHAR_STATE);

	pInfoChar->m_bStateFadeInOut = TRUE;
	PacketMAP_PARA1.Make(SBOCOMMANDID_SUB_MAP_FADEINOUT, pInfoChar->m_dwMapID, 1);
	SendToClient(pInfoChar->m_dwSessionID, &PacketMAP_PARA1);
}

void CMainFrame::RecvProcCHAR_REQ_TALKEVENT(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar, pInfoPlayer;
	CInfoTalkEvent InfoTalkEventTmp, *pInfoTalkEvent;
	CPacketCHAR_PARA1 Packet;
	CPacketCHAR_RES_TALKEVENT PacketCHAR_RES_TALKEVENT;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	pInfoTalkEvent = (PCInfoTalkEvent)m_pLibInfoTalkEvent->GetPtr(Packet.m_dwCharID);
	if (pInfoTalkEvent == NULL) {
		pInfoTalkEvent = &InfoTalkEventTmp;
		pInfoTalkEvent->m_dwTalkEventID = Packet.m_dwCharID;
	} else {
		// REQ_ADDSKILLの正当性確認用に、送信元セッションのプレイヤーへ
		// 「最後に会話イベントを要求したNPCのIDと時刻」を記録する。
		// 中身が取得できないNPC(会話イベント未設定)は対象外とする。
		pInfoPlayer = (PCInfoCharSvr)m_pLibInfoChar->GetPtrSessionID(dwSessionID);
		if (pInfoPlayer != NULL) {
			pInfoPlayer->m_dwLastTalkEventNPCID = Packet.m_dwCharID;
			pInfoPlayer->m_dwLastTalkEventTime = SboPlatform::GetTickMs();
		}
	}
	PacketCHAR_RES_TALKEVENT.Make(pInfoTalkEvent, Packet.m_dwPara);
	m_pSock->SendTo(dwSessionID, &PacketCHAR_RES_TALKEVENT);
}

void CMainFrame::RecvProcCHAR_REQ_ADDSKILL(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	int i, nCount;
	PARRAYDWORD paSkill;
	PCInfoCharSvr pInfoChar, pInfoNPC;
	PCInfoSkillBase pInfoSkill;
	PCInfoTalkEvent pInfoTalkEvent;
	PCInfoTalkEventBase pInfoTalkEventBase;
	DWORD dwNowTime;
	SIZE sizeDistance;
	BOOL bFoundAddSkill;
	CPacketCHAR_PARA1 Packet;
	CPacketCHAR_SKILLINFO PacketCHAR_SKILLINFO;
	CPacketMAP_FORMATMSG PacketMsg;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}
	pInfoSkill = (PCInfoSkillBase)m_pLibInfoSkill->GetPtr(Packet.m_dwPara);
	if (pInfoSkill == NULL) {
		return;
	}

	paSkill = pInfoChar->GetSkill();
	nCount = paSkill->size();
	for (i = 0; i < nCount; i ++) {
		if (paSkill->at(i) == Packet.m_dwPara) {
			break;
		}
	}
	// すでに持っている？
	if (i < nCount) {
		return;
	}

	// 改造クライアントによる任意スキル付与を防ぐため、直前に会話イベント(TALKEVENTTYPE_ADDSKILL)を
	// 経由して正規に要求されたものかを確認する。切断はしない(会話の途中でタイムアウトする
	// 正規ケースがあるため)。条件を満たさない場合はログに記録して黙って無視する。
	pInfoNPC = NULL;
	if (pInfoChar->m_dwLastTalkEventNPCID != 0) {
		pInfoNPC = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(pInfoChar->m_dwLastTalkEventNPCID);
	}
	if (pInfoNPC == NULL) {
		// 会話相手のNPCがログインしていない(存在しない/退出済み)
		m_pLog->Write("■REQ_ADDSKILL不正 dwSessionID:%u [CharID:%u][SkillID:%u][理由:会話相手NPC不在]", dwSessionID, Packet.m_dwCharID, Packet.m_dwPara);
		return;
	}
	dwNowTime = SboPlatform::GetTickMs();
	// 会話を読み進める時間を考慮し、記録から5分以内を正規の範囲とする
	if ((dwNowTime - pInfoChar->m_dwLastTalkEventTime) > (5 * 60 * 1000)) {
		m_pLog->Write("■REQ_ADDSKILL不正 dwSessionID:%u [CharID:%u][SkillID:%u][理由:会話イベント要求から時間経過]", dwSessionID, Packet.m_dwCharID, Packet.m_dwPara);
		return;
	}
	m_pLibInfoChar->GetDistance(sizeDistance, pInfoChar, pInfoNPC);
	if ((sizeDistance.cx < 0) || (sizeDistance.cx > MAPPARTSSIZE * 3) || (sizeDistance.cy > MAPPARTSSIZE * 3)) {
		m_pLog->Write("■REQ_ADDSKILL不正 dwSessionID:%u [CharID:%u][SkillID:%u][理由:会話相手NPCから離れている]", dwSessionID, Packet.m_dwCharID, Packet.m_dwPara);
		return;
	}
	// 会話相手NPCの会話イベント定義の中に、種別TALKEVENTTYPE_ADDSKILLで
	// m_dwDataが要求スキルIDと一致する項目があるかを確認する。
	// m_apTalkEventはページ・分岐を問わず全項目がフラットに格納されているため、そのまま全走査する。
	pInfoTalkEvent = (PCInfoTalkEvent)m_pLibInfoTalkEvent->GetPtr(pInfoChar->m_dwLastTalkEventNPCID);
	bFoundAddSkill = FALSE;
	if (pInfoTalkEvent != NULL) {
		nCount = pInfoTalkEvent->m_apTalkEvent.GetSize();
		for (i = 0; i < nCount; i ++) {
			pInfoTalkEventBase = pInfoTalkEvent->m_apTalkEvent.GetAt(i);
			if (pInfoTalkEventBase == NULL) {
				continue;
			}
			if ((pInfoTalkEventBase->m_nEventType == TALKEVENTTYPE_ADDSKILL) && (pInfoTalkEventBase->m_dwData == Packet.m_dwPara)) {
				bFoundAddSkill = TRUE;
				break;
			}
		}
	}
	if (bFoundAddSkill == FALSE) {
		m_pLog->Write("■REQ_ADDSKILL不正 dwSessionID:%u [CharID:%u][SkillID:%u][理由:会話イベントにADDSKILL項目なし]", dwSessionID, Packet.m_dwCharID, Packet.m_dwPara);
		return;
	}

	paSkill->push_back(Packet.m_dwPara);
	PacketCHAR_SKILLINFO.Make(Packet.m_dwCharID, paSkill);
	m_pSock->SendTo(dwSessionID, &PacketCHAR_SKILLINFO);

	PacketMsg.Make(FORMATMSGID_ADDSKILL, Packet.m_dwCharID, Packet.m_dwPara, RGB(255, 255, 255), FALSE, FORMATMSGTYPE_NOLOG);
	m_pSock->SendTo(dwSessionID, &PacketMsg);
}

void CMainFrame::RecvProcCHAR_REQ_USESKILL(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	PCInfoCharSvr pInfoChar;
	CPacketCHAR_PARA1 Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		return;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		return;
	}
	m_pLibInfoChar->UseSkill(pInfoChar, Packet.m_dwPara);
}
