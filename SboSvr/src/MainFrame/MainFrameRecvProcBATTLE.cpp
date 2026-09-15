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
#include "InfoCharSvr.h"
#include "InfoMapBase.h"
#include "MainFrame.h"
#include "AttackDecision.h"
#include "TextOutput.h"
#include "../Platform/SvrPlatform.h"

// 攻撃の最小間隔(ms)。S1暫定。S2で武器ごとの攻撃間隔に置き換える。
#define ATACK_MIN_INTERVAL_MS 300

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
	dwNow = SboPlatform::GetTickMs();
	bResult = AttackDecision::IsAttackIntervalOk(
		dwNow, pInfoChar->m_dwLastAtackAcceptedTime, pInfoChar->m_dwPrevAtackAcceptedTime, ATACK_MIN_INTERVAL_MS);
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
