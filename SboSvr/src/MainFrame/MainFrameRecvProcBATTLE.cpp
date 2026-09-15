/// @file MainFrameRecvProcBATTLE.cpp
/// @brief サーバーメインフレーム(戦闘系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/27
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoCharSvr.h"
#include "LibInfoMapBase.h"
#include "LibInfoItem.h"
#include "LibInfoMotion.h"
#include "InfoItemType/InfoItemTypeBase.h"
#include "InfoCharSvr.h"
#include "InfoMapBase.h"
#include "MainFrame.h"
#include "AttackDecision.h"
#include "TextOutput.h"
#include "../Platform/SvrPlatform.h"

// docs/battle-redesign.md S2: 攻撃間隔が求まらない場合のフォールバック(ms)。
#define ATACK_INTERVAL_FALLBACK_MS 200
// 攻撃間隔の下限(ms)。フォールバックにもこれを適用する。
#define ATACK_INTERVAL_MIN_CLAMP_MS 100

// 攻撃モーションの候補(振り/突き、それぞれ4方向)の最大数。
#define ATACK_MOTION_CANDIDATE_MAX 8

namespace {

/// 装備武器の攻撃モーション種別(INFOITEMARMS_MOTION_*)から、起こりうる
/// モーションリストIDの先頭(+0〜+3で上下左右)を列挙する。
/// SboCli/src/LibInfo/LibInfoCharCli.cpp の CLibInfoCharCli::SetMotionInfo()
/// (CHARMOTIONID_ATACK の分岐)と同じ優先順位で上書きする。振り/突きが両方
/// 選べる武器は、クライアントが50%ランダムで選ぶため両方を候補にする。
void CollectAtackMotionListBases(DWORD dwMotionType, DWORD adwListBase[], int nMaxCount, int &nCount)
{
	nCount = 0;
	if (nMaxCount <= 0) {
		return;
	}

	// 既定は振り。振り/突きが両方立っていれば両方を候補にする。
	if ((dwMotionType & INFOITEMARMS_MOTION_SWING) && (dwMotionType & INFOITEMARMS_MOTION_POKE)) {
		adwListBase[nCount ++] = CHARMOTIONLISTID_SWING_UP;
		if (nCount < nMaxCount) {
			adwListBase[nCount ++] = CHARMOTIONLISTID_POKE_UP;
		}
	} else if (dwMotionType & INFOITEMARMS_MOTION_POKE) {
		adwListBase[nCount ++] = CHARMOTIONLISTID_POKE_UP;
	} else {
		adwListBase[nCount ++] = CHARMOTIONLISTID_SWING_UP;
	}

	// 弓は振り/突きを上書きする。
	if (dwMotionType & INFOITEMARMS_MOTION_BOW) {
		nCount = 0;
		adwListBase[nCount ++] = CHARMOTIONLISTID_BOW_UP;
	}
	// 未定義(武器無し)・打撃は弓もさらに上書きする。
	if ((dwMotionType == 0) || (dwMotionType & INFOITEMARMS_MOTION_BLOW)) {
		nCount = 0;
		adwListBase[nCount ++] = CHARMOTIONLISTID_GLOVE_UP;
	}
	// 釣りは最後に全てを上書きする。
	if (dwMotionType & INFOITEMARMS_MOTION_FISHING) {
		nCount = 0;
		adwListBase[nCount ++] = CHARMOTIONLISTID_FISHING_UP;
	}
}

/// 指定モーション(向き固定1つ)の全体時間(ms)を求める。未定義(コマが無い)なら0。
DWORD GetOneMotionDurationMs(CLibInfoMotion *pLibInfoMotion, DWORD dwMotionTypeID, DWORD dwMotionListID)
{
	int i, nCount;
	ARRAYMOTIONINFO aMotion;
	unsigned int adwWait[64];

	pLibInfoMotion->GetMotionInfo(dwMotionTypeID, dwMotionListID, aMotion);
	nCount = (int)aMotion.size();
	if ((nCount <= 0) || (nCount > (int)(sizeof(adwWait) / sizeof(adwWait[0])))) {
		return 0;
	}
	for (i = 0; i < nCount; i ++) {
		adwWait[i] = (unsigned int)aMotion[i]->m_byWait;
	}

	return (DWORD)AttackDecision::GetMotionDurationMs(adwWait, nCount);
}

/// 攻撃者の現在の装備武器から、攻撃間隔の下限(ms)を求める。
/// docs/battle-redesign.md S2: 起こりうる攻撃モーション(向き・振り/突きの候補
/// すべて)の全体時間の最小値の80%。求められなければ ATACK_INTERVAL_FALLBACK_MS、
/// どちらも ATACK_INTERVAL_MIN_CLAMP_MS 未満にはしない。
DWORD GetAtackIntervalMs(CLibInfoItem *pLibInfoItem, CLibInfoMotion *pLibInfoMotion, PCInfoCharSvr pInfoChar)
{
	DWORD dwMotionType;
	DWORD adwListBase[2];
	int nBaseCount, i, j, nCandidateCount;
	unsigned int adwCandidateMs[ATACK_MOTION_CANDIDATE_MAX];

	dwMotionType = pLibInfoItem->GetMotionIDAtack(pInfoChar->m_dwEquipItemIDArmsRight);
	CollectAtackMotionListBases(dwMotionType, adwListBase, 2, nBaseCount);

	nCandidateCount = 0;
	for (i = 0; i < nBaseCount; i ++) {
		for (j = 0; j < 4; j ++) {	// 4方向(上下左右)
			if (nCandidateCount >= ATACK_MOTION_CANDIDATE_MAX) {
				break;
			}
			adwCandidateMs[nCandidateCount ++] =
				GetOneMotionDurationMs(pLibInfoMotion, pInfoChar->m_dwMotionTypeID, adwListBase[i] + j);
		}
	}

	return (DWORD)AttackDecision::ComputeAttackIntervalMs(
		adwCandidateMs, nCandidateCount, ATACK_INTERVAL_FALLBACK_MS, ATACK_INTERVAL_MIN_CLAMP_MS);
}

}	// namespace

// 攻撃要求却下ログの出力間隔(ms)。改造クライアントの連打でログが溢れないよう
// 押し要求却下ログ(PUSH_REJECT_LOG_INTERVAL_MS)と同じ方式でキャラごとに間引く。
static const DWORD ATACK_REJECT_LOG_INTERVAL_MS = 1000;

// 攻撃要求却下ログを1キャラ1秒1回に抑制して出力する。抑制中に捨てた件数は
// 次に出すログへ「(他N件)」として付記する。
static void LogAtackReject(CTextOutput *pLog, PCInfoCharSvr pInfoChar, LPCSTR pszReason)
{
	DWORD dwNow = SboPlatform::GetTickMs();

	if ((pInfoChar->m_dwLastAtackRejectLogTime == 0) ||
		(dwNow - pInfoChar->m_dwLastAtackRejectLogTime >= ATACK_REJECT_LOG_INTERVAL_MS)) {
		pInfoChar->m_dwLastAtackRejectLogTime = dwNow;

		CmyString strSuppressed;
		if (pInfoChar->m_nAtackRejectSuppressedCount > 0) {
			strSuppressed.Format(_T("(他%d件)"), pInfoChar->m_nAtackRejectSuppressedCount);
		}
		pLog->Write("攻撃要求却下(%s) dwCharID:%u%s",
			pszReason, pInfoChar->m_dwCharID, strSuppressed.GetUtf8Pointer());
		pInfoChar->m_nAtackRejectSuppressedCount = 0;
	} else {
		pInfoChar->m_nAtackRejectSuppressedCount ++;
	}
}

void CMainFrame::RecvProcBATTLE(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_BATTLE_REQ_ATACK:	RecvProcBATTLE_REQ_ATACK(pData, dwSessionID);	break;	// 攻撃要求
	}
}

void CMainFrame::RecvProcBATTLE_REQ_ATACK(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	DWORD dwNow;
	DWORD dwAtackIntervalMs;
	PCInfoCharSvr pInfoChar;
	PCInfoMapBase pInfoMap;
	CPacketBATTLE_REQ_ATACK Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}
	bResult = pInfoChar->CheckSessionID(dwSessionID);
	if (bResult == FALSE) {
		RequestDisconnect(dwSessionID);
		goto Exit;
	}

	// 付いて行き中・気絶中は攻撃できない
	bResult = pInfoChar->IsEnableBattle();
	if (bResult == FALSE) {
		LogAtackReject(m_pLog, pInfoChar, "戦闘不可状態");
		goto Exit;
	}
	// 戦闘可能マップかどうか(改造クライアント対策。マップ不明も拒否)
	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr(pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		LogAtackReject(m_pLog, pInfoChar, "マップ不明");
		goto Exit;
	}
	bResult = pInfoMap->IsEnableBattle();
	if (bResult == FALSE) {
		LogAtackReject(m_pLog, pInfoChar, "戦闘不可マップ");
		goto Exit;
	}
	// 攻撃間隔(改造クライアント対策の連打拒否。通信の揺らぎは許容する)
	// S2: 固定値ではなく、装備武器で起こりうる攻撃モーションの全体時間(最短候補)の
	// 80%を下限にする(押しっぱなし連打はモーション終了ごとの間隔になる想定)。
	dwAtackIntervalMs = GetAtackIntervalMs(m_pLibInfoItem, m_pLibInfoMotion, pInfoChar);
	dwNow = SboPlatform::GetTickMs();
	bResult = AttackDecision::IsAttackIntervalOk(
		dwNow, pInfoChar->m_dwLastAtackAcceptedTime, pInfoChar->m_dwPrevAtackAcceptedTime, dwAtackIntervalMs);
	if (bResult == FALSE) {
		LogAtackReject(m_pLog, pInfoChar, "間隔不足");
		goto Exit;
	}

	pInfoChar->m_dwPrevAtackAcceptedTime = pInfoChar->m_dwLastAtackAcceptedTime;
	pInfoChar->m_dwLastAtackAcceptedTime = dwNow;

	m_pLibInfoChar->ProcAtack(pInfoChar);
Exit:
	return;
}
